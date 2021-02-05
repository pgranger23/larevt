#ifndef CHANNELSTATUSDATA_H
#define CHANNELSTATUSDATA_H

#include <memory>

#include "larevt/CalibrationDBI/IOVData/Snapshot.h"

namespace lariov {

class ChannelStatusData {
public:
  virtual ~ChannelStatusData() = default;
  virtual ChannelStatus GetChannelStatus(raw::ChannelID_t ch) const = 0;
};

using ChannelStatusDataPtr = std::shared_ptr<ChannelStatusData>;
//Mt note: this should go to SIOVProvider eventually
}
#endif
