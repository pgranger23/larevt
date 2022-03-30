
find_package(libwda REQUIRED EXPORT)

art_make(LIB_LIBRARIES
           larevt_CalibrationDBI_IOVData
           wda::wda
           SQLite::SQLite3
           messagefacility::MF_MessageLogger
           fhiclcpp::fhiclcpp
           canvas::canvas
           cetlib::cetlib
           cetlib_except::cetlib_except
           larcorealg_Geometry
           art::Framework_Services_Registry
           ROOT::Core
           art::Utilities
        ) ### MIGRATE-ACTION-RECOMMENDED (migrate-3.09.03) - deprecated: use art_make_library(), art_dictonary(), and cet_build_plugin() with explicit source lists and plugin base types

install_headers()
install_source()
