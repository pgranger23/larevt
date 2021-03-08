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
        fDataSource(datasource) 
        {}

  ChannelStatus GetChannelStatus(raw::ChannelID_t ch) const {
   if (fDataSource == DataSource::Default) {
      return ChannelStatus{0, kGOOD};
    }
    if (fNoisyData.HasChannel(rawToDBChannel(ch))) {
      return fNoisyData.GetRow(rawToDBChannel(ch));
    }
    return fDBdata.GetRow(rawToDBChannel(ch));
  }

  ChannelSet_t GoodChannels() const {
    return GetChannelsWithStatus(kGOOD);
  }

  ChannelSet_t BadChannels() const {
    ChannelSet_t dead = GetChannelsWithStatus(kDEAD);
    ChannelSet_t ln = GetChannelsWithStatus(kLOWNOISE);
    dead.insert(ln.begin(),ln.end());
    return dead;
  }

  ChannelSet_t NoisyChannels() const {
    return GetChannelsWithStatus(kNOISY);
  
}

  ChannelSet_t GetChannelsWithStatus(chStatus status) const {
    if (fDataSource == DataSource::Default && status != kGOOD) {
      return {};
    }
    ChannelSet_t retSet;
    DBChannelID_t maxChannel = art::ServiceHandle<geo::Geometry const>()->Nchannels() - 1;
    for (DBChannelID_t ch=0; ch != maxChannel; ++ch) {
     if (fDataSource == DataSource::Default || this->GetChannelStatus(ch).Status() == status) {
       retSet.insert(ch);
       }
    }
    return retSet;
  }

private:
  Snapshot<ChannelStatus> const& fNoisyData;
  Snapshot<ChannelStatus> const& fDBdata;
  DataSource::ds fDataSource;
};
  //----------------------------------------------------------------------------
  SIOVChannelStatusProvider::SIOVChannelStatusProvider(fhicl::ParameterSet const& pset)
    : fDBFolder(pset.get<fhicl::ParameterSet>("DatabaseRetrievalAlg"))
    , fCurrentTimeStamp(0)
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
      std::cout << "Using default channel status value: "<< kGOOD <<"\n";
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
  ChannelSet_t
  SIOVChannelStatusProvider::GoodChannels(DBTimeStamp_t ts) const {
    auto data = GetData(ts);
    return data->GoodChannels();
  }


  //----------------------------------------------------------------------------
  ChannelSet_t
  SIOVChannelStatusProvider::BadChannels(DBTimeStamp_t ts) const {
    auto data = GetData(ts);
    return data->BadChannels(); 
  }


  //----------------------------------------------------------------------------
  ChannelSet_t
  SIOVChannelStatusProvider::NoisyChannels(DBTimeStamp_t ts) const {
    auto data = GetData(ts);
    return data->NoisyChannels(); 
  }


  //----------------------------------------------------------------------------
  void SIOVChannelStatusProvider::AddNoisyChannel(DBTimeStamp_t ts, raw::ChannelID_t ch)
  {
    ChannelStatus cs{rawToDBChannel(ch), kNOISY};
    fNewNoisy.AddOrReplaceRow(cs);
  }



  //----------------------------------------------------------------------------

} // namespace lariov
