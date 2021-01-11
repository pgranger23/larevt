/**
 * \file ElectronicsCalib.h
 *
 * \ingroup IOVData
 *
 * \brief Class def header for a class ElectronicsCalib
 *
 * @author eberly@slac.stanford.edu
 */

/** \addtogroup IOVData

    @{*/
#ifndef IOVDATA_ELECTRONICSCALIB_H
#define IOVDATA_ELECTRONICSCALIB_H

#include "CalibrationExtraInfo.h"
#include "ChData.h"

namespace lariov {
  /**
     \class ElectronicsCalib
  */
  class ElectronicsCalib : public ChData {
  public:
    ElectronicsCalib(unsigned int ch,
                     float const gain,
                     float const gain_err,
                     float const shaping_time,
                     float const shaping_time_err)
      : ChData(ch)
      , fGain{gain}
      , fGainErr{gain_err}
      , fShapingTime{shaping_time}
      , fShapingTimeErr{shaping_time_err}
    {}

    float
    Gain() const noexcept
    {
      return fGain;
    }
    float
    GainErr() const noexcept
    {
      return fGainErr;
    }
    float
    ShapingTime() const noexcept
    {
      return fShapingTime;
    }
    float
    ShapingTimeErr() const noexcept
    {
      return fShapingTimeErr;
    }
    CalibrationExtraInfo const&
    ExtraInfo() const noexcept
    {
      return fExtraInfo;
    }

  private:
    float fGain;
    float fGainErr;
    float fShapingTime;
    float fShapingTimeErr;
    CalibrationExtraInfo fExtraInfo{"ElectronicsCalib"};

  }; // end class
} // end namespace lariov

#endif
/** @} */ // end of doxygen group
