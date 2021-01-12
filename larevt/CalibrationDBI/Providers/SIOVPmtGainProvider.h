/**
 * \file SIOVPmtGainProvider.h
 *
 * \ingroup WebDBI
 *
 * \brief Class def header for a class SIOVPmtGainProvider
 *
 * @author eberly@slac.stanford.edu
 */

#ifndef SIOVPMTGAINPROVIDER_H
#define SIOVPMTGAINPROVIDER_H

#include "larevt/CalibrationDBI/IOVData/PmtGain.h"
#include "larevt/CalibrationDBI/IOVData/Snapshot.h"
#include "larevt/CalibrationDBI/IOVData/IOVDataConstants.h"
#include "larevt/CalibrationDBI/Interface/PmtGainProvider.h"
#include "DatabaseRetrievalAlg.h"

namespace lariov {

  /**
   * @brief Retrieves information: pmt gain
   *
   * Configuration parameters
   * =========================
   *
   * - *DatabaseRetrievalAlg* (parameter set, mandatory): configuration for the
   *   database; see lariov::DatabaseRetrievalAlg
   * - *UseDB* (boolean, default: false): retrieve information from the database
   * - *UseFile* (boolean, default: false): retrieve information from a file;
   *   not implemented yet
   * - *DefaultGain* (real, default: ): Gain returned
   *   when /UseDB/ and /UseFile/ parameters are false
   * - *DefaultGainErr* (real, default: ): Gain uncertainty returned
   *   when /UseDB/ and /UseFile/ parameters are false
   */
  class SIOVPmtGainProvider : public PmtGainProvider {

    public:

      /// Constructors
      SIOVPmtGainProvider(fhicl::ParameterSet const& p);

      /// Update event time stamp.
      void UpdateTimeStamp(DBTimeStamp_t ts);

      /// Retrieve gain information
      const PmtGain& PmtGainObject(DBChannelID_t ch) const;
      float Gain(DBChannelID_t ch) const override;
      float GainErr(DBChannelID_t ch) const override;
      CalibrationExtraInfo const& ExtraInfo(DBChannelID_t ch) const override;

    private:

      /// Do actual database updates.

      Snapshot<PmtGain> const& DBUpdate(DBTimeStamp_t ts) const;

      DatabaseRetrievalAlg fRetrievalAlg;

      // Time stamps.

      DBTimeStamp_t fEventTimeStamp;            // Most recently seen time stamp.
      mutable DBTimeStamp_t fCurrentTimeStamp;  // Time stamp of cached data.

      DataSource::ds fDataSource;

      mutable Snapshot<PmtGain> fData;
  };
}//end namespace lariov

#endif
