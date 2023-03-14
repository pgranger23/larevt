/**
 * @file   SimpleChannelStatus_test.cxx
 * @brief  Test of SimpleChannelStatus
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   July 17th, 2015
 */

// Boost libraries
/*
 * Boost Magic: define the name of the module;
 * and do that before the inclusion of Boost unit test headers
 * because it will change what they provide.
 * Among the those, there is a main() function and some wrapping catching
 * unhandled exceptions and considering them test failures, and probably more.
 * This also makes fairly complicate to receive parameters from the command line
 * (for example, a random seed).
 */
#define BOOST_TEST_MODULE (simple_channel_status_test)
#include <boost/test/test_tools.hpp>  // BOOST_CHECK(), BOOST_CHECK_EQUAL()
#include <cetlib/quiet_unit_test.hpp> // BOOST_AUTO_TEST_CASE()

// LArSoft libraries
#include "larcoreobj/SimpleTypesAndConstants/RawTypes.h"
#include "larevt/Filters/SimpleChannelStatus.h"

// framework libraries
#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/coding.h"

// C/C++ standard library
#include <algorithm> // std::equal(), std::transform()
#include <iostream>
#include <memory> // std::unique_ptr<>
#include <ostream>
#include <set>

constexpr unsigned int TimeStamp_not_used = 0;

namespace std {

  template <typename T>
  fhicl::detail::ps_sequence_t encode(std::set<T> const& s)
  {
    fhicl::detail::ps_sequence_t result;
    result.reserve(s.size());
    std::transform(s.begin(), s.end(), std::inserter(result, result.end()), [](T const& value) {
      return std::any(fhicl::detail::encode(value));
    });
    return result;
  } // encode(set<T>)

  template <typename T>
  std::ostream& operator<<(std::ostream& out, std::set<T> const& s)
  {
    out << "{";
    typename std::set<T>::const_iterator begin = s.cbegin(), end = s.cend();
    if (begin != end) {
      out << " " << *begin;
      while (++begin != end)
        out << ", " << *begin;
      out << " ";
    } // if not empty
    out << "}";
    return out;
  } // operator<< (ostream, set<T>)

} // namespace std

//------------------------------------------------------------------------------
class StatusConfiguration {
public:
  const raw::ChannelID_t fMaxChannel;
  const raw::ChannelID_t fMaxPresentChannel;
  const std::set<unsigned int> fBadChannels;
  const std::set<unsigned int> fNoisyChannels;

  StatusConfiguration(const raw::ChannelID_t MaxChannel,
                      const raw::ChannelID_t MaxPresentChannel,
                      const std::set<unsigned int> BadChannels,
                      const std::set<unsigned int> NoisyChannels)
    : fMaxChannel(MaxChannel)
    , fMaxPresentChannel(MaxPresentChannel)
    , fBadChannels(BadChannels)
    , fNoisyChannels(NoisyChannels)
  {}

  std::unique_ptr<lariov::SimpleChannelStatus> operator()() const { return CreateStatus(); }

  fhicl::ParameterSet CreateConfiguration() const
  {
    fhicl::ParameterSet cfg;

    std::any any{fhicl::detail::encode(fNoisyChannels)};
    cfg.put("NoisyChannels", fNoisyChannels);
    cfg.put("BadChannels", fBadChannels);

    return cfg;
  } // CreateConfiguration()

  std::unique_ptr<lariov::SimpleChannelStatus> CreateStatus() const
  {
    fhicl::ParameterSet config = CreateConfiguration();

    lariov::SimpleChannelStatus* pStatus =
      new lariov::SimpleChannelStatus(config, fMaxChannel, fMaxPresentChannel);

    std::cout << "\nConfiguration:"
              << "\n  { " << config.to_string() << " }"
              << "\nLoaded from configuration:"
              << "\n  - " << pStatus->BadChannels(TimeStamp_not_used).size()
              << " bad channels: " << pStatus->BadChannels(TimeStamp_not_used) << "\n  - "
              << pStatus->NoisyChannels(TimeStamp_not_used).size()
              << " noisy channels: " << pStatus->NoisyChannels(TimeStamp_not_used)
              << "\n  - largest channel ID: " << pStatus->MaxChannel()
              << ", largest present: " << pStatus->MaxChannelPresent() << std::endl;

    return std::unique_ptr<lariov::SimpleChannelStatus>(pStatus);
  } // CreateStatus()

}; // class StatusConfiguration

void test_simple_status()
{

  StatusConfiguration statusCreator(
    /* MaxChannel */ 20,
    /* MaxPresentChannel */ 15,
    /* BadChannels */ {1, 9, 13},
    /* NoisyChannels */ {6, 8, 10, 11, 12, 13});

  BOOST_TEST_CHECKPOINT("Creating simple status");
  std::unique_ptr<lariov::SimpleChannelStatus> StatusOwner = statusCreator.CreateStatus();

  lariov::SimpleChannelStatus const* pSimpleStatus = StatusOwner.get();

  // check the values of the extremes
  BOOST_CHECK_EQUAL(pSimpleStatus->MaxChannel(), statusCreator.fMaxChannel);
  BOOST_CHECK_EQUAL(pSimpleStatus->MaxChannelPresent(), statusCreator.fMaxPresentChannel);

  // downcast to the interface to test interface stuff
  lariov::ChannelStatusProvider const* pStatus = pSimpleStatus;

  /**
   *
   * Public interface:
   *
   * bool isPresent(raw::ChannelID_t channel) const
   *
   * bool isGood(raw::ChannelID_t channel) const
   *
   * bool isBad(raw::ChannelID_t channel) const
   *
   * bool isNoisy(raw::ChannelID_t channel) const
   *
   * ChannelSet_t GoodChannels() const
   *
   * ChannelSet_t BadChannels() const
   *
   * ChannelSet_t NoisyChannels() const
   *
   */

  // ChannelStatusBaseInterface::BadChannels()
  std::set<raw::ChannelID_t> StatusBadChannels = pStatus->BadChannels(TimeStamp_not_used);
  BOOST_CHECK_EQUAL(StatusBadChannels.size(), statusCreator.fBadChannels.size());
  BOOST_CHECK_EQUAL(StatusBadChannels, statusCreator.fBadChannels);

  // ChannelStatusBaseInterface::NoisyChannels()
  std::set<raw::ChannelID_t> StatusNoisyChannels = pStatus->NoisyChannels(TimeStamp_not_used);
  BOOST_CHECK_EQUAL(StatusNoisyChannels.size(), statusCreator.fNoisyChannels.size());
  BOOST_CHECK_EQUAL(StatusNoisyChannels, statusCreator.fNoisyChannels);

  std::set<raw::ChannelID_t> GoodChannels;

  for (raw::ChannelID_t channel = 0; channel <= statusCreator.fMaxChannel; ++channel) {

    const bool bPresent =
      raw::isValidChannelID(channel) && (channel <= statusCreator.fMaxPresentChannel);
    const bool bBad = (statusCreator.fBadChannels.count(channel) > 0);
    const bool bNoisy = (statusCreator.fNoisyChannels.count(channel) > 0);
    const bool bGood = bPresent && !bBad && !bNoisy;

    if (bGood) GoodChannels.insert(channel);

    BOOST_CHECK_EQUAL(pStatus->IsPresent(TimeStamp_not_used, channel), bPresent);
    BOOST_CHECK_EQUAL(pStatus->IsBad(TimeStamp_not_used, channel), bBad);
    BOOST_CHECK_EQUAL(pStatus->IsNoisy(TimeStamp_not_used, channel), bNoisy);

    BOOST_CHECK_EQUAL(pStatus->IsGood(TimeStamp_not_used, channel), bGood);

  } // for channel

  // ChannelStatusBaseInterface::GoodChannels()
  std::set<raw::ChannelID_t> StatusGoodChannels = pStatus->GoodChannels(TimeStamp_not_used);
  BOOST_CHECK_EQUAL(StatusGoodChannels.size(), GoodChannels.size());
  BOOST_CHECK_EQUAL(StatusGoodChannels, GoodChannels);

} // test_simple_status()

//
// test
//
BOOST_AUTO_TEST_CASE(SimpleStatusTest)
{
  test_simple_status();
}
