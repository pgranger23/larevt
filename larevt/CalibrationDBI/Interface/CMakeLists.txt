# Providers.

# Define some virtual library targets to express the transitive
# dependencies conferred by including a header.
cet_make_library(LIBRARY_NAME ChannelStatusProvider INTERFACE
  SOURCE ChannelStatusProvider.h
  LIBRARIES INTERFACE
  larcorealg::headers
  larcoreobj::headers
)

cet_make_library(LIBRARY_NAME DetPedestalProvider INTERFACE
  SOURCE DetPedestalProvider.h
  LIBRARIES INTERFACE
  larcorealg::headers
  larcoreobj::headers
)

cet_make_library(LIBRARY_NAME ElectronLifetimeProvider INTERFACE
  SOURCE ElectronLifetimeProvider.h
  LIBRARIES INTERFACE
  larcorealg::headers
)

cet_make_library(LIBRARY_NAME ElectronicsCalibProvider INTERFACE
  SOURCE ElectronicsCalibProvider.h
  LIBRARIES INTERFACE
  larevt::CalibrationDBI_IOVData
  larcorealg::headers
  larcoreobj::headers
)

cet_make_library(LIBRARY_NAME PmtGainProvider INTERFACE
  SOURCE PmtGainProvider.h
  LIBRARIES INTERFACE
  larevt::CalibrationDBI_IOVData
  larcorealg::headers
  larcoreobj::headers
)

# Service interfaces.
set(builder_dest Modules)
set(builders)
function(make_simple_service_builder STEM)
  # Define an interface library for services and modules that use this
  # service interface.
  cet_make_library(LIBRARY_NAME ${STEM}Service INTERFACE
    SOURCE ${STEM}Service.h
    LIBRARIES INTERFACE
    larevt::${STEM}Provider ${ARGN})
  # Make a plugin builder for services of this type.
  cet_write_plugin_builder(lar::${STEM}Service art::service ${builder_dest}
    INSTALL_BUILDER
    LIBRARIES CONDITIONAL
    larevt::${STEM}Service
  )
  list(APPEND builders lar::${STEM}Service)
  set(builders ${builders} PARENT_SCOPE)
endfunction()

make_simple_service_builder(ChannelStatus larcore::ServiceUtil)
make_simple_service_builder(DetPedestal larcore::ServiceUtil)
make_simple_service_builder(ElectronLifetime larcore::ServiceUtil)
make_simple_service_builder(ElectronicsCalib)
make_simple_service_builder(PmtGain)

cet_collect_plugin_builders(${builder_dest} lar::CalibrationDBIServiceBuilders
  LIST ${builders}
)

install_headers()
