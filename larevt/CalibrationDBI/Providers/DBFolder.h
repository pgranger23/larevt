#ifndef DBFOLDER_H
#define DBFOLDER_H

#include "larevt/CalibrationDBI/IOVData/IOVTimeStamp.h"
#include "larevt/CalibrationDBI/Interface/CalibrationDBIFwd.h"
#include "larevt/CalibrationDBI/Providers/DBDataset.h"
#include <string>
#include <vector>

namespace lariov {

  typedef void *Dataset;
  typedef void *Tuple;


  class DBFolder {

    public:
      DBFolder(const std::string& name, const std::string& url, const std::string& url2, 
	       const std::string& tag = "", bool useqlite=false, bool testmode=false);

      int GetNamedChannelData(DBChannelID_t channel, const std::string& name, bool& data);
      int GetNamedChannelData(DBChannelID_t channel, const std::string& name, long& data);
      int GetNamedChannelData(DBChannelID_t channel, const std::string& name, double& data);
      int GetNamedChannelData(DBChannelID_t channel, const std::string& name, std::string& data);
      //int GetNamedChannelData(DBChannelID_t channel, const std::string& name, std::vector<double>& data);

      const std::string& URL() const {return fURL;}
      const std::string& FolderName() const {return fFolderName;}
      const std::string& Tag() const {return fTag;}

      const IOVTimeStamp& CachedStart() const {return fDataset.beginTime();}
      const IOVTimeStamp& CachedEnd() const   {return fDataset.endTime();}

      bool UpdateData(DBTimeStamp_t raw_time);

      void GetSQLiteData(int t, DBDataset& data) const;

      int GetChannelList( std::vector<DBChannelID_t>& channels ) const;

      void DumpDataset(const DBDataset& data) const;

      bool CompareDataset(const DBDataset& data1, const DBDataset& data2) const;

    private:
      // from 12/18/20: we start here next year.
      // and fix binary search in DBDataSet.cxx
      //
      size_t GetColumn(const std::string& name) const;

      std::string fURL;
      std::string fURL2;
      std::string fFolderName;
      std::string fTag;
      bool        fUseSQLite;
      bool        fTestMode;
      std::string fSQLitePath;
      int         fMaximumTimeout;

      // Database cache.

      DBDataset fDataset;

  };
}

#endif
