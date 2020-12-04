///////////////////////////////////////////////////////
//
// ChannelFilter Class
//
// This class has been obsoleted and is now a deprecated interface for
// ChannelStatusService.
//
// Please update your code to use the service directly.
//
//
// Original class: pagebri3@msu.edu
//
///////////////////////////////////////////////////////


// Our header
#include "larevt/Filters/ChannelFilter.h"


// Framework libraries
#include "messagefacility/MessageLogger/MessageLogger.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "canvas/Utilities/Exception.h"

// LArSoft libraries
#include "larevt/CalibrationDBI/Interface/ChannelStatusService.h"
#include "larevt/CalibrationDBI/Interface/ChannelStatusProvider.h"


//
// The following construct is rarely used:
// I have a C++ reference as a member class, that needs to be constructed in
// the initialization list. The construction could throw an exception,
// and I want to do some special handling if that happens.
// The way to do that is to use a special syntax commonly called
// "function try block".
//
// Note that the "special handling" is not that special as it ends rethrowing
// the exception. The purpose here is just to provide a bit of additional
// information about why the failure could have occurred, since art messages
// often don't have enough.
//
filter::ChannelFilter::ChannelFilter() try:
  provider(art::ServiceHandle<lariov::ChannelStatusService const>()->GetProvider())
{

  MF_LOG_ERROR("ChannelFilter") << "ChannelFilter is now deprecated."
    " Replace it with ChannelStatusService";

} // function try
catch (art::Exception& e) { // automatic rethrow happens at end of block
  if (e.categoryCode() == art::errors::ServiceNotFound) {
    MF_LOG_SYSTEM("ChannelFilter") <<
      "Failed to obtain an instance of ChannelStatusService service;"
      " you should update your configuration, *and* update the code using"
      " ChannelFilter, that is deprecated."
      " An example are in ChannelFilter class documentation"
      ;
  }
} // filter::ChannelFilter::ChannelFilter() (function catch)


///////////////////////////////////////////////////////
bool filter::ChannelFilter::BadChannel(lariov::DBTimeStamp_t ts, uint32_t channel) const {
  return provider.IsBad(ts, channel);
}

///////////////////////////////////////////////////////
bool filter::ChannelFilter::NoisyChannel(lariov::DBTimeStamp_t ts, uint32_t channel) const{
  return provider.IsNoisy(ts, channel);
}

///////////////////////////////////////////////////////
std::set<uint32_t> filter::ChannelFilter::SetOfBadChannels(lariov::DBTimeStamp_t ts) const {
  return provider.BadChannels(ts);
}

///////////////////////////////////////////////////////
std::set<uint32_t> filter::ChannelFilter::SetOfNoisyChannels(lariov::DBTimeStamp_t ts) const {
  return provider.NoisyChannels(ts);
}

///////////////////////////////////////////////////////
filter::ChannelFilter::ChannelStatus filter::ChannelFilter::GetChannelStatus(lariov::DBTimeStamp_t ts, uint32_t channel) const
{

  if (provider.IsGood(ts, channel))          return GOOD;
  else if (!provider.IsPresent(ts, channel)) return NOTPHYSICAL;
  else if (provider.IsBad(ts, channel))      return DEAD;
  else if (provider.IsNoisy(ts, channel))    return NOISY;
  else return DEAD; //assume all other status are equivalent to DEAD
}
