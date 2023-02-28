////////////////////////////////////////////////////////////////////////
//
// ChannelFilter class:
//
// This class has been obsoleted and is now a deprecated interface for
// ChannelStatusService.
//
// Please update your code to use the service directly.
//
//
// Original class: pagebri3@msu.edu
//
////////////////////////////////////////////////////////////////////////
#ifndef CHANNELFILTER_H
#define CHANNELFILTER_H

// LArSoft libraries
namespace lariov {
  class ChannelStatusProvider;
}

// C/C++ standard libraries
#include <set>
#include <stdint.h>

#include "larevt/CalibrationDBI/Interface/CalibrationDBIFwd.h"

namespace filter {

  class /* [[deprecated]] */ ChannelFilter {

  public:
    enum ChannelStatus { GOOD = 0, NOISY = 1, DEAD = 2, NOTPHYSICAL = 3 };

    ChannelFilter();

    bool BadChannel(lariov::DBTimeStamp_t ts, uint32_t channel) const;
    bool NoisyChannel(lariov::DBTimeStamp_t ts, uint32_t channel) const;
    std::set<uint32_t> SetOfBadChannels(lariov::DBTimeStamp_t ts) const;
    std::set<uint32_t> SetOfNoisyChannels(lariov::DBTimeStamp_t ts) const;
    ChannelStatus GetChannelStatus(lariov::DBTimeStamp_t ts, uint32_t channel) const;

  private:
    lariov::ChannelStatusProvider const& provider; ///< object doing the job

  }; //class ChannelFilter
}
#endif // CHANNELFILTER_H
