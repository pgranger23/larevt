cet_enable_asserts()

find_package(Boost REQUIRED)

simple_plugin ( SimpleChannelStatusTest "module"
                    larevt_Filters
                    messagefacility::MF_MessageLogger
                    
                    fhiclcpp::fhiclcpp
                    cetlib::cetlib cetlib_except
              ) ### MIGRATE-ACTION-RECOMMENDED (migrate-3.09.03) - deprecated: use cet_build_plugin() with explicit source lists and plugin base types

cet_test(ChannelStatusServiceTest HANDBUILT
  DATAFILES test_channel_status.fcl
  TEST_EXEC lar
  TEST_ARGS --rethrow-all --config ./test_channel_status.fcl
)

cet_test(SimpleChannelStatus_test
  SOURCES SimpleChannelStatus_test.cxx
  LIBRARIES larevt_Filters
            larevt_CalibrationDBI_IOVData
            fhiclcpp::fhiclcpp
  USE_BOOST_UNIT
)

