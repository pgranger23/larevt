/**
 * \file PmtGain.h
 *
 * \ingroup IOVData
 *
 * \brief Class def header for a class PmtGain
 *
 * @author eberly@slac.stanford.edu
 */

/** \addtogroup IOVData

    @{*/
#ifndef IOVDATA_PMTGAIN_H
#define IOVDATA_PMTGAIN_H

#include "CalibrationExtraInfo.h"
#include "ChData.h"

namespace lariov {
  /**
     \class PmtGain
  */
  class PmtGain : public ChData {
  public:
    PmtGain(unsigned int const ch,
            float const gain,
            float const gain_err)
      : ChData(ch), fGain{gain}, fGainErr{gain_err}
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
    CalibrationExtraInfo const&
    ExtraInfo() const noexcept
    {
      return fExtraInfo;
    }

  private:
    float fGain;
    float fGainErr;
    CalibrationExtraInfo fExtraInfo{"PmtGain"};
  }; // end class
} // end namespace lariov

#endif
/** @} */ // end of doxygen group
