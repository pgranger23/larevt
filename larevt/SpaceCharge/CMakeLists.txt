cet_make_library(
  SOURCE SpaceChargeStandard.cxx
  LIBRARIES
  PUBLIC
  larcoreobj::geo_vectors
  PRIVATE
  canvas::canvas
  fhiclcpp::fhiclcpp
  cetlib_except::cetlib_except
  ROOT::Core
  ROOT::Hist
  ROOT::RIO
  )

install_headers()
install_fhicl()
install_source()
