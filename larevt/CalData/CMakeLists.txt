cet_build_plugin(CalWireAna art::EDAnalyzer
  LIBRARIES PRIVATE
  larevt::ChannelStatusService
  larevt::ChannelStatusProvider
  lardata::Utilities_LArFFT_service
  lardata::Utilities
  larcore::Geometry_Geometry_service
  lardataobj::RawData
  lardataobj::RecoBase
  art_root_io::TFileService_service
  ROOT::Hist
  ROOT::MathCore
  )

simple_plugin(CalWire art::EDProducer
  LIBRARIES PRIVATE
  larcoreobj::headers
  larcorealg::Geometry
  lardataobj::RecoBase
  lardataobj::RawData
  lardata::ArtDataHelper
  lardata::Utilities
  art::Framework_Principal
  art::Framework_Services_Registry
  ROOT::Hist
  ROOT::MathCore
  ROOT::RIO
  )

simple_plugin(CalWireT962 art::EDProducer
  LIBRARIES PRIVATE
  larcoreobj::headers
  larcorealg::Geometry
  lardataobj::RecoBase
  lardataobj::RawData
  lardata::ArtDataHelper
  lardata::Utilities
  art::Framework_Principal
  art::Framework_Services_Registry
  ROOT::Hist
  ROOT::MathCore
  ROOT::RIO
  )

install_headers()
install_fhicl()
install_source()

add_subdirectory(test)
