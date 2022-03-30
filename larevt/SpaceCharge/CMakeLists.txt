art_make(NO_PLUGINS
         LIB_LIBRARIES
           canvas::canvas
           fhiclcpp::fhiclcpp
           ROOT::Core
           ROOT::Hist
           ROOT::RIO
           cetlib::cetlib
           cetlib_except::cetlib_except
         ) ### MIGRATE-ACTION-RECOMMENDED (migrate-3.09.03) - deprecated: use art_make_library(), art_dictonary(), and cet_build_plugin() with explicit source lists and plugin base types

install_headers()
install_fhicl()
install_source()
