#include "fhiclcpp/ParameterSet.h"
#include "larevt/CalibrationDBI/Providers/DBFolder.h"

#include "DatabaseRetrievalAlg.h"

#include <string>

namespace lariov {

  /// Configure using fhicl::ParameterSet
  DatabaseRetrievalAlg::DatabaseRetrievalAlg(fhicl::ParameterSet const& p) :
    DatabaseRetrievalAlg{p.get<std::string>("DBFolderName"),
                         p.get<std::string>("DBUrl"),
                         p.get<std::string>("DBUrl2", ""),
                         p.get<std::string>("DBTag", ""),
                         p.get<bool>("UseSQLite", false),
                         p.get<bool>("TestMode", false)}
  {}
}
