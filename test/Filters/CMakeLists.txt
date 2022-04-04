cet_enable_asserts()

cet_build_plugin(SimpleChannelStatusTest art::EDAnalyzer
  LIBRARIES PRIVATE
  larevt::ChannelStatusProvider
  larevt::ChannelStatusService
  larevt::Filters
  )

cet_test(ChannelStatusServiceTest HANDBUILT
  DATAFILES test_channel_status.fcl
  TEST_EXEC lar
  TEST_ARGS --rethrow-all --config ./test_channel_status.fcl
)

cet_test(SimpleChannelStatus_test
  SOURCE SimpleChannelStatus_test.cxx
  LIBRARIES larevt_Filters
            larevt_CalibrationDBI_IOVData
            fhiclcpp::fhiclcpp
  USE_BOOST_UNIT
)

