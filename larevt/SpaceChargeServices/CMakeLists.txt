cet_make_library(LIBRARY_NAME SpaceChargeService INTERFACE
  SOURCE SpaceChargeService.h
  LIBRARIES INTERFACE
  larevt::SpaceCharge
  art::Framework_Services_Registry
)

# Don't want to add this builder to a wrapper, so use
# cet_write_plugin_builder() instead of cet_make_plugin_builder().
cet_write_plugin_builder(lar::SpaceChargeService art::service Modules
  INSTALL_BUILDER
  LIBRARIES CONDITIONAL larevt::SpaceChargeService)

include(lar::SpaceChargeService)

cet_build_plugin(SpaceChargeServiceStandard lar::SpaceChargeService
  LIBRARIES PRIVATE
  art::Framework_Principal
)

install_headers()
install_fhicl()
install_source()
