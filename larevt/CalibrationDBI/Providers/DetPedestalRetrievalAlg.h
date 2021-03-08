/**
 * \file DetPedestalRetrievalAlg.h
 *
 * \ingroup WebDBI
 *
 * \brief Class def header for a class DetPedestalRetrievalAlg
 *
 * @author eberly@slac.stanford.edu
 */

/** \addtogroup WebDBI

    @{*/
#ifndef WEBDBI_DETPEDESTALRETRIEVALALG_H
#define WEBDBI_DETPEDESTALRETRIEVALALG_H

// C/C++ standard libraries
#include <string>

// LArSoft libraries
#include "larevt/CalibrationDBI/IOVData/DetPedestal.h"
#include "larevt/CalibrationDBI/IOVData/Snapshot.h"
#include "larevt/CalibrationDBI/IOVData/IOVDataConstants.h"
#include "larevt/CalibrationDBI/Interface/CalibrationDBIFwd.h"
#include "larevt/CalibrationDBI/Interface/DetPedestalProvider.h"
#include "larevt/CalibrationDBI/Providers/DBFolder.h"

#include "fhiclcpp/fwd.h"

namespace lariov {

  /**
   * @brief Retrieves channel information: pedestal and RMS
   *
   * Configuration parameters
   * =========================
   *
   * - *DatabaseRetrievalAlg* (parameter set, mandatory): configuration for the
   *   database; see lariov::DatabaseRetrievalAlg
   * - *UseDB* (boolean, default: false): retrieve information from the database
   * - *UseFile* (boolean, default: false): retrieve information from a file;
   *   not implemented yet
   * - *DefaultCollMean* (real, default: 400.0): pedestal value returned for
   *   collection plane channels when /UseDB/ and /UseFile/ parameters are false
   * - *DefaultIndMean* (real, default: 2048.0): pedestal value returned for
   *   induction plane channels when /UseDB/ and /UseFile/ parameters are false
   * - *DefaultCollRms* (real, default: 0.3): pedestal RMS value returned for
   *   collection plane channels when /UseDB/ and /UseFile/ parameters are false
   * - *DefaultIndRms* (real, default: 0.3): pedestal RMS value returned for
   *   induction plane channels when /UseDB/ and /UseFile/ parameters are false
   * - *DefaultMeanErr* (real, default: 0.0): error on the pedestal value
   *   for all channels returned when /UseDB/ and /UseFile/ parameters are false
   * - *DefaultRmsErr* (real, default: 0.0): error on the RMS value
   *   for all channels returned when /UseDB/ and /UseFile/ parameters are false
   */
  class DetPedestalRetrievalAlg : public DetPedestalProvider {

    public:

      /// Constructors
      DetPedestalRetrievalAlg(const std::string& foldername,
                              const std::string& url,
                              const std::string& tag="");

      DetPedestalRetrievalAlg(fhicl::ParameterSet const& p);

      /// Retrieve pedestal information
      float PedMean(DBTimeStamp_t ts, DBChannelID_t ch) const override;
      float PedRms(DBTimeStamp_t ts, DBChannelID_t ch) const override;
      float PedMeanErr(DBTimeStamp_t ts, DBChannelID_t ch) const override;
      float PedRmsErr(DBTimeStamp_t ts, DBChannelID_t ch) const override;

      //hardcoded information about database folder - useful for debugging cross checks
      static constexpr unsigned int NCOLUMNS = 5;
      static constexpr const char* FIELD_NAMES[NCOLUMNS]
        = {"channel", "mean", "mean_err", "rms", "rms_err"};
      static constexpr const char* FIELD_TYPES[NCOLUMNS]
        = {"unsigned int", "float", "float", "float", "float"};

    private:

      /// Do actual database updates.

      Snapshot<DetPedestal> const& DBUpdate(DBTimeStamp_t ts) const;

      DBFolder fDBFolder;

      // Time stamps.

      mutable DBTimeStamp_t fCurrentTimeStamp;  // Time stamp of cached data.

      DataSource::ds fDataSource;
      mutable Snapshot<DetPedestal> fData;
      const DetPedestal& Pedestal(DBTimeStamp_t ts, DBChannelID_t ch) const;
  };
}//end namespace lariov

#endif
 /** @} */ // end of doxygen group
