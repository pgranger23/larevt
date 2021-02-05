/**
 * @file   SIOVChannelStatusProvider.cxx
 * @brief  Channel quality provider with information from configuration file
 * @author Brandon Eberly (eberly@fnal.gov)
 * @date   August 24, 2015
 * @see    SIOVChannelStatusProvider.h
 */

// Our header
#include "SIOVChannelStatusProvider.h"

// LArSoft libraries
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "fhiclcpp/ParameterSet.h"
#include "larcore/Geometry/Geometry.h"
#include "larevt/CalibrationDBI/IOVData/IOVDataConstants.h"
#include "larevt/CalibrationDBI/IOVData/ChannelStatusData.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

// C/C++ standard libraries
#include <fstream>

namespace lariov {

     /// Converts LArSoft channel ID in the one proper for the DB
     static DBChannelID_t rawToDBChannel(raw::ChannelID_t channel)
       { return DBChannelID_t(channel); }

class SIOVChannelStatusData : public ChannelStatusData {
public:
  SIOVChannelStatusData(Snapshot<ChannelStatus> const& noisy,
                        Snapshot<ChannelStatus> const& dbdata, 
                        DataSource::ds datasource)
      : fNoisyData(noisy), 
        fDBdata(dbdata),
        fDataSource(datasource){}

  ChannelStatus GetChannelStatus(raw::ChannelID_t ch) const {
   if (fDataSource == DataSource::Default) {
      return ChannelStatus{0, kGOOD};
    }
    if (fNoisyData.HasChannel(rawToDBChannel(ch))) {
      return fNoisyData.GetRow(rawToDBChannel(ch));
    }
    return fDBdata.GetRow(rawToDBChannel(ch));
  }

private:
  Snapshot<ChannelStatus> const& fNoisyData;
  Snapshot<ChannelStatus> const& fDBdata;
  DataSource::ds fDataSource;
};
  //----------------------------------------------------------------------------
  SIOVChannelStatusProvider::SIOVChannelStatusProvider(fhicl::ParameterSet const& pset)
    : fDBFolder(pset.get<fhicl::ParameterSet>("DatabaseRetrievalAlg"))
    , fEventTimeStamp(0)
    , fCurrentTimeStamp(0)
    , fDefault{0,kGOOD}
  {
    bool UseDB    = pset.get<bool>("UseDB", false);
    bool UseFile  = pset.get<bool>("UseFile", false);
    std::string fileName = pset.get<std::string>("FileName", "");

    //priority:  (1) use db, (2) use table, (3) use defaults
    //If none are specified, use defaults
    if ( UseDB )      fDataSource = DataSource::Database;
    else if (UseFile) fDataSource = DataSource::File;
    else              fDataSource = DataSource::Default;

    if (fDataSource == DataSource::Default) {
      std::cout << "Using default channel status value: "<< fDefault.Status() <<"\n";
    }
    else if (fDataSource == DataSource::File) {
      cet::search_path sp("FW_SEARCH_PATH");
      std::string abs_fp = sp.find_file(fileName);
      std::cout << "Using channel statuses from local file: "<<abs_fp<<"\n";
      std::ifstream file(abs_fp);
      if (!file) {
        throw cet::exception("SIOVChannelStatusProvider")
          << "File "<<abs_fp<<" is not found.";
      }

      std::string line;
      while (std::getline(file, line)) {
        DBChannelID_t ch = std::stoi(line.substr(0, line.find(',')));
        int status = std::stoi(line.substr(line.find(',')+1));
        ChannelStatus cs{ch, status};
        fData.AddOrReplaceRow(cs);
      }
    } // if source from file
    else {
      std::cout << "Using channel statuses from conditions database\n";
    }
  }

  // This method saves the time stamp of the latest event.

  void SIOVChannelStatusProvider::UpdateTimeStamp(DBTimeStamp_t ts) {
    mf::LogInfo("SIOVChannelStatusProvider") << "SIOVChannelStatusProvider::UpdateTimeStamp called.";
    fNewNoisy.Clear();
    fEventTimeStamp = ts;
  }

  // Maybe update method cached data (private const version).
  // This is the function that does the actual work of updating data from database.

  Snapshot<ChannelStatus> const&
  SIOVChannelStatusProvider::DBUpdate(DBTimeStamp_t ts) const
  {
    if (fDataSource != DataSource::Database or ts == fCurrentTimeStamp) {
      return fData;
    }

    mf::LogInfo("SIOVChannelStatusProvider") << "SIOVChannelStatusProvider::DBUpdate called with new timestamp.";

    fCurrentTimeStamp = ts;

    auto const dataset = fDBFolder.GetDataset(ts);

    Snapshot<ChannelStatus> data{dataset.beginTime(), dataset.endTime()};
    for (auto const channel : dataset.channels()) {
      ChannelStatus cs{channel,
                       static_cast<int>(dataset.GetDataAsLong(channel, "status"))};
      data.AddOrReplaceRow(cs);
    }
    return fData = data;
  }

// Get noisy data
  Snapshot<ChannelStatus> const&
  SIOVChannelStatusProvider::GetNoisyData(DBTimeStamp_t ts) const {
    return fNewNoisy; 
  }

  ChannelStatusDataPtr
  SIOVChannelStatusProvider::GetData(DBTimeStamp_t ts) const {
   auto const& noisydata = GetNoisyData(ts);
   auto const& dbdata = DBUpdate(ts);
   return std::make_shared<SIOVChannelStatusData>(noisydata, dbdata, fDataSource); 
  }
  //----------------------------------------------------------------------------
  //MT note for next week: 
  //1. Introduce ChannelStatusData class/struct
  //2. Use ChannelStatusData in the following function implementation
  //3. and update IsBad, IsNoisy, etc to only use channel and not DBTimeStamp_t
  ChannelStatus SIOVChannelStatusProvider::GetChannelStatus(DBTimeStamp_t ts, raw::ChannelID_t ch) const {
   auto data = GetData(ts); // it will have noisydata and dbdata logic
   return data->GetChannelStatus(ch);
  }


  //----------------------------------------------------------------------------
  SIOVChannelStatusProvider::ChannelSet_t
  SIOVChannelStatusProvider::GetChannelsWithStatus(DBTimeStamp_t ts, chStatus status) const {

    ChannelSet_t retSet;
    retSet.clear();
    DBChannelID_t maxChannel = art::ServiceHandle<geo::Geometry const>()->Nchannels() - 1;
    if (fDataSource == DataSource::Default) {
      if (fDefault.Status() == status) {
        std::vector<DBChannelID_t> chs;
        for (DBChannelID_t ch=0; ch != maxChannel; ++ch) {
          chs.push_back(ch);
        }
        retSet.insert(chs.begin(), chs.end());
      }
    }
    else {
      std::vector<DBChannelID_t> chs;
      for (DBChannelID_t ch=0; ch != maxChannel; ++ch) {
        if (this->GetChannelStatus(ts, ch).Status() == status) chs.push_back(ch);
      }

      retSet.insert(chs.begin(), chs.end());
    }
    return retSet;
  }


  //----------------------------------------------------------------------------
  SIOVChannelStatusProvider::ChannelSet_t
  SIOVChannelStatusProvider::GoodChannels(DBTimeStamp_t ts) const {
    return GetChannelsWithStatus(ts, kGOOD);
  }


  //----------------------------------------------------------------------------
  SIOVChannelStatusProvider::ChannelSet_t
  SIOVChannelStatusProvider::BadChannels(DBTimeStamp_t ts) const {
    ChannelSet_t dead = GetChannelsWithStatus(ts, kDEAD);
    ChannelSet_t ln = GetChannelsWithStatus(ts, kLOWNOISE);
    dead.insert(ln.begin(),ln.end());
    return dead;
  }


  //----------------------------------------------------------------------------
  SIOVChannelStatusProvider::ChannelSet_t
  SIOVChannelStatusProvider::NoisyChannels(DBTimeStamp_t ts) const {
    return GetChannelsWithStatus(ts, kNOISY);
  }


  //----------------------------------------------------------------------------
  void SIOVChannelStatusProvider::AddNoisyChannel(DBTimeStamp_t ts, raw::ChannelID_t ch)
  {
    DBChannelID_t const dbch = rawToDBChannel(ch);
    if (IsPresent(ts, dbch) and !IsBad(ts, dbch)) {
      ChannelStatus cs{dbch, kNOISY};
      fNewNoisy.AddOrReplaceRow(cs);
    }
  }



  //----------------------------------------------------------------------------

} // namespace lariov
