/**
 * \file DetPedestalProvider
 *
 * \brief Class def header for a class DetPedestalProvider
 *
 * @author eberly@slac.stanford.edu
 */

#ifndef DETPEDESTALPROVIDER_H
#define DETPEDESTALPROVIDER_H

// LArSoft libraries
#include "larcorealg/CoreUtils/UncopiableAndUnmovableClass.h"
#include "larcoreobj/SimpleTypesAndConstants/RawTypes.h" // raw::ChannelID_t
#include "larevt/CalibrationDBI/IOVData/IOVDataConstants.h"
#include "larevt/CalibrationDBI/Interface/CalibrationDBIFwd.h"

namespace lariov {

  /**
     \class DetPedestalProvider
     Pure abstract interface class for retrieving detector pedestals.
     Includes a feature to encourage database use: an Update method that can be used to update
     an implementation's local state to ensure that the correct information is retrieved
  */
  class DetPedestalProvider : private lar::UncopiableAndUnmovableClass {

  public:
    virtual ~DetPedestalProvider() = default;

    /// Retrieve pedestal information
    virtual float PedMean(DBTimeStamp_t ts, raw::ChannelID_t ch) const = 0;
    virtual float PedRms(DBTimeStamp_t ts, raw::ChannelID_t ch) const = 0;
    virtual float PedMeanErr(DBTimeStamp_t ts, raw::ChannelID_t ch) const = 0;
    virtual float PedRmsErr(DBTimeStamp_t ts, raw::ChannelID_t ch) const = 0;
  };
} //end namespace lariov

#endif
