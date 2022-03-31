cet_build_plugin(SpaceChargeServiceStandard art::service
  LIBRARIES
  PUBLIC larevt::SpaceCharge
  PRIVATE art::Framework_Principal
  )

install_headers()
install_fhicl()
install_source()
