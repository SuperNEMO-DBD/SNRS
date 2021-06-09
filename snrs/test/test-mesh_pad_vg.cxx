// Standard Library:
#include <iostream>
#include <exception>
#include <fstream>

// Bayeux:
#include <bayeux/geomtools/geomtools_config.h>
#include <bayeux/datatools/exception.h>
#include <bayeux/datatools/logger.h>
#include <bayeux/datatools/properties.h>
#include <bayeux/datatools/utils.h>
#include <bayeux/datatools/temporary_files.h>
#include <bayeux/datatools/units.h>
#include <bayeux/geomtools/manager.h>
#include <bayeux/geomtools/mapping.h>
#include <bayeux/mygsl/rng.h>
#if GEOMTOOLS_WITH_GNUPLOT_DISPLAY == 1
#include <bayeux/geomtools/gnuplot_i.h>
#include <bayeux/geomtools/gnuplot_draw.h>
#include <bayeux/geomtools/gnuplot_drawer.h>
#endif // GEOMTOOLS_WITH_GNUPLOT_DISPLAY

// This project:
// Ourselves:
#include <snrs/sngeom.hpp>
#include <snrs/pad.hpp>
#include <snrs/mesh_pad_model.hpp>
#include <snrs/mesh_pad_vg.hpp>

int main(int argc_, char ** argv_)
{
  datatools::logger::priority logging = datatools::logger::PRIO_NOTICE;
  try {
    bool draw = false;
    int vgVersion=1;
    int stripId = 32;
    int iarg = 1;
    while (iarg < argc_) {
      std::string token(argv_[iarg]);
      if (token == "--version-1") {
        vgVersion = 1;
      } else if (token == "--version-2") {
        vgVersion = 2;
      } else if (token == "--strip-32") {
        stripId = 32;
      } else if (token == "--strip-34") {
        stripId = 34;
      } else if (token == "--draw") {
        draw = true;
      }
      iarg++;
    }
    std::string geoMgrConfigPath
      = "${SNRS_TESTING_DIR}/config/geometry_manager.conf";
    datatools::fetch_path_with_env(geoMgrConfigPath);
    datatools::properties geoMgrConfig;
    datatools::properties::read_config(geoMgrConfigPath, geoMgrConfig);
    geomtools::manager geoMgr;
    geoMgr.initialize(geoMgrConfig);
    geoMgr.tree_dump(std::clog, "The geometry manager : ");

    int padId = 0;
    std::string gid_repr("[1131:0."
                         + std::to_string(stripId) + "."
                         + std::to_string(padId) + "]");
    std::istringstream iss(gid_repr);
    geomtools::geom_id padGid;
    iss >> padGid;
    std::clog << "Pad GID = " << padGid << '\n';
    const geomtools::geom_info & padGeomInfo
      = geoMgr.get_mapping().get_geom_infos().find(padGid)->second;
    const geomtools::logical_volume & padLogVol =
      padGeomInfo.get_logical();

    datatools::temp_file tmp_file;
    tmp_file.set_remove_at_destroy(false);
    tmp_file.create(".", "snrs-test-mesh_pad_vg_draw_");
    std::ostream & output = tmp_file.out();

    // std::cerr << "[DEVEL] WR_BASE_GRID=" << geomtools::i_wires_3d_rendering::WR_BASE_GRID << '\n';
    // std::cerr << "[DEVEL] WR_TESSELLA_ALL_SEGMENTS=" << geomtools::tessellated_solid::WR_TESSELLA_ALL_SEGMENTS << '\n';
    uint32_t draw_options = geomtools::i_wires_3d_rendering::WR_BASE_GRID
      | geomtools::tessellated_solid::WR_TESSELLA_ALL_SEGMENTS; 
    geomtools::gnuplot_draw::draw_tessellated(output,
                                              padGeomInfo.get_world_placement(),
                                              dynamic_cast<const geomtools::tessellated_solid &>(padLogVol.get_shape()),
                                              draw_options);
    output << '\n';
    
    std::string meshPadVgConfigPath
      = "${SNRS_TESTING_DIR}/config/mesh_pad_vg.conf";
    if (vgVersion == 2) {
      meshPadVgConfigPath
      = "${SNRS_TESTING_DIR}/config/mesh_pad_vg2.conf";
    }
    datatools::fetch_path_with_env(meshPadVgConfigPath);
    snrs::mesh_pad_vg meshPadVg;
    datatools::properties meshPadVgConfig;
    datatools::properties::read_config(meshPadVgConfigPath, meshPadVgConfig);

    std::clog << "Initializing random generator...\n";
    int32_t seed = 314159;
    mygsl::rng random(seed);
    meshPadVg.set_external_prng(random);
    meshPadVg.set_geom_manager(geoMgr);
    meshPadVg.initialize_standalone(meshPadVgConfig);

    std::size_t nbvtx = 10000;
    std::clog << "Start vertex generation loop [" << nbvtx << "]\n";
    for (std::size_t ivtx = 0; ivtx < nbvtx; ivtx++) {
      geomtools::vector_3d vertex;
      meshPadVg.shoot_vertex(vertex);
      output << vertex.x() << ' ' << vertex.y() << ' ' << vertex.z() << '\n';
    }
    output << '\n';
    output << '\n';

   if (draw) {
#if GEOMTOOLS_WITH_GNUPLOT_DISPLAY == 1
      Gnuplot g1;
      g1.cmd("set title 'SNRS mesh pad vertex generator test' ");
      g1.cmd("set view equal xyz");
      g1.cmd("set xyplane at -750");
      g1.cmd("set zrange [-750:-500]");
      g1.set_xlabel("x").set_ylabel("y").set_zlabel("z");
      {
        std::ostringstream plot_cmd;
        plot_cmd << "splot '" << tmp_file.get_filename()
                 << "' index 0 title '' with lines lw 1";
        plot_cmd << ", '' index 1 title '' with points pt 6 ps 0.5";
 
        g1.cmd(plot_cmd.str());
        g1.showonscreen(); // window output
        geomtools::gnuplot_drawer::wait_for_key();
        usleep(200);
      }
#endif // GEOMTOOLS_WITH_GNUPLOT_DISPLAY == 1
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
