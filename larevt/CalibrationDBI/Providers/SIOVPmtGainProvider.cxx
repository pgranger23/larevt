#include "SIOVPmtGainProvider.h"

// art/LArSoft libraries
#include "cetlib_except/exception.h"
#include "larcore/Geometry/Geometry.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

#include <fstream>

namespace lariov {

  //constructor
  SIOVPmtGainProvider::SIOVPmtGainProvider(fhicl::ParameterSet const& p) :
    fRetrievalAlg(p.get<fhicl::ParameterSet>("DatabaseRetrievalAlg")),
    fEventTimeStamp(0),
    fCurrentTimeStamp(0)
  {
    IOVTimeStamp tmp = IOVTimeStamp::MaxTimeStamp();
    tmp.SetStamp(tmp.Stamp()-1, tmp.SubStamp());
    fData.SetIoV(tmp, IOVTimeStamp::MaxTimeStamp());

    bool UseDB      = p.get<bool>("UseDB", false);
    bool UseFile    = p.get<bool>("UseFile", false);
    std::string fileName = p.get<std::string>("FileName", "");

    //priority:  (1) use db, (2) use table, (3) use defaults
    //If none are specified, use defaults
    if ( UseDB )      fDataSource = DataSource::Database;
    else if (UseFile) fDataSource = DataSource::File;
    else              fDataSource = DataSource::Default;

    if (fDataSource == DataSource::Default) {
      auto const default_gain     = p.get<float>("DefaultGain");
      auto const default_gain_err = p.get<float>("DefaultGainErr");

      art::ServiceHandle<geo::Geometry const> geo; // FIXME: Cannot use services in providers
      for (unsigned int od=0; od!=geo->NOpDets(); ++od) {
        if (geo->IsValidOpChannel(od)) {
          PmtGain defaultGain{od,
                              default_gain,
                              default_gain_err};
          fData.AddOrReplaceRow(defaultGain);
        }
      }

    }
    else if (fDataSource == DataSource::File) {
      cet::search_path sp("FW_SEARCH_PATH");
      std::string abs_fp = sp.find_file(fileName);
      std::cout << "Using pmt gains from local file: "<<abs_fp<<"\n";
      std::ifstream file(abs_fp);
      if (!file) {
        throw cet::exception("SIOVPmtGainProvider")
          << "File "<<abs_fp<<" is not found.";
      }

      std::string line;
      while (std::getline(file, line)) {
        if (line[0] == '#') continue;
        size_t current_comma = line.find(',');
        DBChannelID_t ch = (DBChannelID_t)std::stoi(line.substr(0, current_comma));
        float gain     = std::stof( line.substr(current_comma+1, line.find(',',current_comma+1)-(current_comma+1)) );

        current_comma = line.find(',',current_comma+1);
        float gain_err = std::stof( line.substr(current_comma+1) );

        PmtGain dp{ch, gain, gain_err};
        fData.AddOrReplaceRow(dp);
      }
    }
    else {
      std::cout << "Using pmt gains from conditions database"<<std::endl;
    }
  }

  // This method saves the time stamp of the latest event.

  void SIOVPmtGainProvider::UpdateTimeStamp(DBTimeStamp_t ts)
  {
    mf::LogInfo("SIOVPmtGainProvider") << "SIOVPmtGainProvider::UpdateTimeStamp called.";
    fEventTimeStamp = ts;
  }

  // Maybe update method cached data (private const version).
  // This is the function that does the actual work of updating data from database.

  Snapshot<PmtGain> const&
  SIOVPmtGainProvider::DBUpdate(DBTimeStamp_t ts) const
  {
    if (fDataSource != DataSource::Database or ts == fCurrentTimeStamp) {
      return fData;
    }

    mf::LogInfo("SIOVPmtGainProvider") << "SIOVPmtGainProvider::DBUpdate called with new timestamp.";

    fCurrentTimeStamp = ts;

    auto const dataset = fRetrievalAlg.GetDataset(ts);

    Snapshot<PmtGain> data{dataset.beginTime(), dataset.endTime()};
    for (auto const channel : dataset.channels()) {
      PmtGain pg{channel,
                 dataset.GetDataAsFloat(channel, "gain"),
                 dataset.GetDataAsFloat(channel, "gain_sigma")};
      data.AddOrReplaceRow(pg);
    }

    return fData = data;
  }

  const PmtGain& SIOVPmtGainProvider::PmtGainObject(DBChannelID_t ch) const {
    return DBUpdate(fEventTimeStamp).GetRow(ch);
  }

  float SIOVPmtGainProvider::Gain(DBChannelID_t ch) const {
    return PmtGainObject(ch).Gain();
  }

  float SIOVPmtGainProvider::GainErr(DBChannelID_t ch) const {
    return PmtGainObject(ch).GainErr();
  }

  CalibrationExtraInfo const& SIOVPmtGainProvider::ExtraInfo(DBChannelID_t ch) const {
    return PmtGainObject(ch).ExtraInfo();
  }


}//end namespace lariov
