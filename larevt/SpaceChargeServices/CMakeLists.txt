simple_plugin(SpaceChargeServiceStandard "service"
              larevt_SpaceCharge
              art::Framework_Principal
              ROOT::Core
            ) ### MIGRATE-ACTION-RECOMMENDED (migrate-3.09.03) - deprecated: use cet_build_plugin() with explicit source lists and plugin base types

install_headers()
install_fhicl()
install_source()
