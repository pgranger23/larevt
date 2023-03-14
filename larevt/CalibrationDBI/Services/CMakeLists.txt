include(lar::CalibrationDBIServiceBuilders)

cet_build_plugin(SIOVChannelStatusService lar::ChannelStatusService
  LIBRARIES PRIVATE
  larevt::CalibrationDBI_Providers
  art::Framework_Principal
)

cet_build_plugin(SIOVDetPedestalService lar::DetPedestalService
  LIBRARIES PRIVATE
  larevt::CalibrationDBI_Providers
  art::Framework_Principal
)

cet_build_plugin(SIOVElectronicsCalibService lar::ElectronicsCalibService
  LIBRARIES PRIVATE
  larevt::CalibrationDBI_Providers
  art::Framework_Principal
)

cet_build_plugin(SIOVPmtGainService lar::PmtGainService
  LIBRARIES PRIVATE
  larevt::CalibrationDBI_Providers
  art::Framework_Principal
)

install_headers()
install_source()
