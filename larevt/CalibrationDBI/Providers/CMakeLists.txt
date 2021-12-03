
find_package(libwda REQUIRED PUBLIC)

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
        )

install_headers()
install_source()
