/**
 * \file DetPedestal.h
 *
 * \ingroup IOVData
 *
 * \brief Class def header for a class DetPedestal
 *
 * @author eberly@slac.stanford.edu
 */

/** \addtogroup IOVData

    @{*/
#ifndef IOVDATA_DETPEDESTAL_H
#define IOVDATA_DETPEDESTAL_H 1

#include "ChData.h"

namespace lariov {
  /**
     \class DetPedestal
  */
  class DetPedestal : public ChData {
  public:
    DetPedestal(unsigned int ch,
                float const mean,
                float const rms,
                float const mean_err,
                float const rms_err)
      : ChData(ch), fPedMean{mean}, fPedRms{rms}, fPedMeanErr{mean_err}, fPedRmsErr{rms_err}
    {}

    float
    PedMean() const noexcept
    {
      return fPedMean;
    }
    float
    PedRms() const noexcept
    {
      return fPedRms;
    }
    float
    PedMeanErr() const noexcept
    {
      return fPedMeanErr;
    }
    float
    PedRmsErr() const noexcept
    {
      return fPedRmsErr;
    }

  private:
    float fPedMean;
    float fPedRms;
    float fPedMeanErr;
    float fPedRmsErr;

  }; // end class
} // end namespace lariov

#endif
/** @} */ // end of doxygen group
