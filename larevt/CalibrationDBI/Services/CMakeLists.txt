art_make(SERVICE_LIBRARIES
           larevt_CalibrationDBI_Providers
           larevt_CalibrationDBI_IOVData
           art::Framework_Services_Registry
           art::Framework_Principal
         ) ### MIGRATE-ACTION-RECOMMENDED (migrate-3.09.03) - deprecated: use art_make_library(), art_dictonary(), and cet_build_plugin() with explicit source lists and plugin base types

install_headers()
install_source()
