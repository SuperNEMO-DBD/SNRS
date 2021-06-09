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
#include <bayeux/geomtools/mapping.h>

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

    const geomtools::mapping theMapping = geoMgr.get_mapping();
    const geomtools::geom_info_dict_type & geom_infos
      = theMapping.get_geom_infos();
    for (const auto & ginfo : geom_infos) {
      std::clog << "Geom info: " << ginfo.first
                << " : logical='" << ginfo.second.get_logical().get_name() << "' " << '\n';
    }

    int stripId = 32;
    int padId = 0;
    {
      std::clog << "Source strip #: " << '\n';
      geomtools::geom_id gId(1102, 0, stripId);
      std::clog << "  GID = " << gId << '\n';
      const geomtools::geom_info & gInfo = theMapping.get_geom_infos().find(gId)->second;
      const auto & gLog = gInfo.get_logical();
      const auto & gShape = gLog.get_shape();
      gShape.tree_dump(std::clog, "Shape:", "  ");
      const geomtools::placement & gPlcmt = gInfo.get_world_placement();
      std::string plcmtStr;
      gPlcmt.to_string(plcmtStr, gPlcmt);
      std::clog << "  -> Placement = " << plcmtStr << '\n';
    }

    {
      std::clog << "Source pad #: " << '\n';
      geomtools::geom_id gId(1131, 0, stripId, padId);
      std::clog << "  GID = " << gId << '\n';
      const geomtools::geom_info & gInfo = theMapping.get_geom_infos().find(gId)->second;
      const geomtools::placement & gPlcmt = gInfo.get_world_placement();
      const auto & gLog = gInfo.get_logical();
      const auto & gShape = gLog.get_shape();
      gShape.tree_dump(std::clog, "Pad:", "  ");
      std::string plcmtStr;
      gPlcmt.to_string(plcmtStr, gPlcmt);
      std::clog << "  -> Placement = " << plcmtStr << '\n';
    }
 
    for (int sideId = 0; sideId < 2; sideId++) {
      geomtools::geom_id gId(1132, 0, stripId, padId, sideId);
      std::clog << "  GID = " << gId << '\n';
      const geomtools::geom_info & gInfo = theMapping.get_geom_infos().find(gId)->second;
      const geomtools::placement & gPlcmt = gInfo.get_world_placement();
      std::string plcmtStr;
      gPlcmt.to_string(plcmtStr, gPlcmt);
      std::clog << "  -> Placement = " << plcmtStr << '\n';
    }
    
    
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
