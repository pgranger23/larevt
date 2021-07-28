art_make(NO_PLUGINS
         LIB_LIBRARIES
           canvas::canvas
           fhiclcpp::fhiclcpp
           ROOT::Core
           ROOT::Hist
           ROOT::RIO
           cetlib::cetlib
           cetlib_except::cetlib_except
         )

install_headers()
install_fhicl()
install_source()
