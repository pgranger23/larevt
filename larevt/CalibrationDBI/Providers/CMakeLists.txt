cet_find_library(LIBWDA NAMES wda PATHS ENV LIBWDA_LIB NO_DEFAULT_PATH)
cet_find_library(SQLITE NAMES sqlite3_ups PATHS ENV SQLITE_LIB NO_DEFAULT_PATH)

include_directories($ENV{LIBWDA_FQ_DIR}/include)

art_make(LIB_LIBRARIES
           larevt_CalibrationDBI_IOVData
           LIBWDA
           SQLITE
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
