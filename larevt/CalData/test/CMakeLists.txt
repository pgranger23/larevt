cet_enable_asserts()

simple_plugin(FFTTest "module"
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
			cetlib::cetlib cetlib_except

) ### MIGRATE-ACTION-RECOMMENDED (migrate-3.09.03) - deprecated: use cet_build_plugin() with explicit source lists and plugin base types

install_headers()
install_fhicl()
install_source()

