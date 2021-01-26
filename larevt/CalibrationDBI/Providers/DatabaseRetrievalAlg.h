/**
 * \file DatabaseRetrievalAlg.h
 *
 * \ingroup WebDBI
 *
 * \brief Class def header for a class DatabaseRetrievalAlg
 *
 * @author kterao, eberly@slac.stanford.edu
 */

/** \addtogroup WebDBI

    @{*/
#ifndef DATABASERETRIEVALALG_H
#define DATABASERETRIEVALALG_H

#include "larevt/CalibrationDBI/Providers/DBFolder.h"


namespace lariov {
  [[deprecated("Please use DBFolder instead")]]
  using DatabaseRetrievalAlg = DBFolder;
}

#endif
/** @} */ // end of doxygen group
