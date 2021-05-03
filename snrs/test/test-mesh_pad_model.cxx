// Standard Library:
#include <iostream>
#include <exception>
#include <fstream>

// Bayeux:
#include <bayeux/datatools/exception.h>
#include <bayeux/datatools/logger.h>
#include <bayeux/datatools/properties.h>
#include <bayeux/datatools/utils.h>
#include <bayeux/datatools/units.h>
#include <bayeux/geomtools/manager.h>

// This project:
// Ourselves:
#include <snrs/sngeom.hpp>
#include <snrs/pad.hpp>
#include <snrs/mesh_pad_model.hpp>

int main()
{
  datatools::logger::priority logging = datatools::logger::PRIO_NOTICE;
  try {
    
    std::string geoMgrConfigPath = "${SNRS_TESTING_DIR}/config/geometry_manager.conf";
    datatools::fetch_path_with_env(geoMgrConfigPath);

    datatools::properties geoMgrConfig;
    datatools::properties::read_config(geoMgrConfigPath, geoMgrConfig);

    geomtools::manager geoMgr;
    geoMgr.initialize(geoMgrConfig);
    geoMgr.tree_dump(std::clog, "The geometry manager : ");

    
     
  } catch (std::exception & error) {
    DT_LOG_ERROR(logging, error.what());
    return 1;
  }
  return 0;
}


void force_load_snrs()
{
  snrs::sngeom MyGeom;
}
