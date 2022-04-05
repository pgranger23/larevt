cet_enable_asserts()

cet_build_plugin(FFTTest art::EDAnalyzer
  LIBRARIES PRIVATE
	lardata_Utilities_LArFFT_service
	art::Framework_Services_Registry
	art_root_io::tfile_support ROOT::Core
	art_root_io::TFileService_service
	art::Persistency_Common
	art::Persistency_Provenance
	art::Utilities
  canvas::canvas
	messagefacility::MF_MessageLogger
	fhiclcpp::fhiclcpp
	cetlib::cetlib
  cetlib_except::cetlib_except
)

install_headers()
install_fhicl()
install_source()

