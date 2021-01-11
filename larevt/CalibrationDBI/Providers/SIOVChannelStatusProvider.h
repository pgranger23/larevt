/**
 * @file   SIOVChannelStatusProvider.h
 * @brief  Channel quality provider with information from configuration file
 * @author Brandon Eberly (eberly@fnal.gov)
 * @date   August 24, 2015
 * @see    ChannelStatusService.h SIOVChannelStatusProvider.cpp
 */


#ifndef SIOVCHANNELSTATUSPROVIDER_H
#define SIOVCHANNELSTATUSPROVIDER_H 1

// LArSoft libraries
#include "larcoreobj/SimpleTypesAndConstants/RawTypes.h"
#include "larevt/CalibrationDBI/Interface/ChannelStatusProvider.h"
#include "larevt/CalibrationDBI/Providers/DatabaseRetrievalAlg.h"
#include "larevt/CalibrationDBI/IOVData/ChannelStatus.h"
#include "larevt/CalibrationDBI/IOVData/Snapshot.h"
#include "larevt/CalibrationDBI/IOVData/IOVDataConstants.h"
#include "larevt/CalibrationDBI/Interface/CalibrationDBIFwd.h"

// Utility libraries
namespace fhicl { class ParameterSet; }

/// Filters for channels, events, etc
namespace lariov {


  /** **************************************************************************
   * @brief Class providing information about the quality of channels
   *
   * This class serves information read from a FHiCL configuration file and/or a database.
   *
   * LArSoft interface to this class is through the service
   * SIOVChannelStatusService.
   */
  class SIOVChannelStatusProvider: public ChannelStatusProvider {

    public:

      /// Constructor
      SIOVChannelStatusProvider(fhicl::ParameterSet const& pset);

      virtual ~SIOVChannelStatusProvider() = default;

      //
      // non-interface methods
      //
      /// Returns Channel Status
      const ChannelStatus& GetChannelStatus(DBTimeStamp_t ts, raw::ChannelID_t channel) const;

      //
      // interface methods
      //

      /// @name Single channel queries
      /// @{
      /// Returns whether the specified channel is physical and connected to wire
      bool IsPresent(DBTimeStamp_t ts, raw::ChannelID_t channel) const override {
        return GetChannelStatus(ts, channel).IsPresent();
      }

      /// Returns whether the specified channel is bad in the current run
      bool IsBad(DBTimeStamp_t ts, raw::ChannelID_t channel) const override {
        return GetChannelStatus(ts, channel).IsDead() || GetChannelStatus(ts, channel).IsLowNoise() || !IsPresent(ts, channel);
      }

      /// Returns whether the specified channel is noisy in the current run
      bool IsNoisy(DBTimeStamp_t ts, raw::ChannelID_t channel) const override {
        return GetChannelStatus(ts, channel).IsNoisy();
      }

      /// Returns whether the specified channel is physical and good
      bool IsGood(DBTimeStamp_t ts, raw::ChannelID_t channel) const override {
        return GetChannelStatus(ts, channel).IsGood();
      }
      /// @}

      Status_t Status(DBTimeStamp_t ts, raw::ChannelID_t channel) const override {
        return (Status_t) this->GetChannelStatus(ts, channel).Status();
      }

      /// @name Global channel queries
      /// @{
      /// Returns a copy of set of good channel IDs for the current run
      ChannelSet_t GoodChannels(DBTimeStamp_t ts) const override;

      /// Returns a copy of set of bad channel IDs for the current run
      ChannelSet_t BadChannels(DBTimeStamp_t ts) const override;

      /// Returns a copy of set of noisy channel IDs for the current run
      ChannelSet_t NoisyChannels(DBTimeStamp_t ts) const override;
      /// @}


      /// Update event time stamp.
      void UpdateTimeStamp(DBTimeStamp_t ts);

      /// @name Configuration functions
      /// @{
      /// Prepares the object to provide information about the specified time
      bool Update(DBTimeStamp_t);

      /// Allows a service to add to the list of noisy channels
      void AddNoisyChannel(DBTimeStamp_t ts, raw::ChannelID_t ch);

      ///@}


      /// Converts LArSoft channel ID in the one proper for the DB
      static DBChannelID_t rawToDBChannel(raw::ChannelID_t channel)
        { return DBChannelID_t(channel); }

    private:

      /// Do actual database updates.

      bool DBUpdate(DBTimeStamp_t ts) const;

      DatabaseRetrievalAlg fRetrievalAlg;

      // Time stamps.

      DBTimeStamp_t fEventTimeStamp;            // Most recently seen time stamp.
      mutable DBTimeStamp_t fCurrentTimeStamp;  // Time stamp of cached data.

      DataSource::ds fDataSource;
      mutable Snapshot<ChannelStatus> fData;    // Lazily updated once per IOV.
      Snapshot<ChannelStatus> fNewNoisy;        // Updated once per event.
      ChannelStatus fDefault;

      ChannelSet_t GetChannelsWithStatus(DBTimeStamp_t ts, chStatus status) const;

  }; // class SIOVChannelStatusProvider


} // namespace lariov


#endif // SIOVCHANNELSTATUSPROVIDER_H
