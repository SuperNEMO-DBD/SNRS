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
#include <bayeux/datatools/units.h>
#include <bayeux/geomtools/manager.h>
#include <bayeux/geomtools/mapping.h>
#include <bayeux/geomtools/display_data.h>
#include <bayeux/geomtools/blur_spot.h>
#include <bayeux/genvtx/manager.h>
#include <bayeux/mygsl/rng.h>
#include <bayeux/geomtools/gnuplot_drawer.h>

// This project:
// Ourselves:
#include <snrs/sngeom.hpp>
#include <snrs/pad.hpp>
#include <snrs/mesh_pad_model.hpp>
#include <snrs/mesh_pad_vg.hpp>

int main()
{
  datatools::logger::priority logging = datatools::logger::PRIO_NOTICE;
  try {
    bool draw = true;
    
    std::string geoMgrConfigPath
      = "${SNRS_TESTING_DIR}/config/geometry_manager.conf";
    datatools::fetch_path_with_env(geoMgrConfigPath);
    datatools::properties geoMgrConfig;
    datatools::properties::read_config(geoMgrConfigPath, geoMgrConfig);
    geomtools::manager geoMgr;
    geoMgr.initialize(geoMgrConfig);
    geoMgr.tree_dump(std::clog, "The geometry manager : ");

    std::string gid_repr("[1131:0.32.0]");
    std::istringstream iss(gid_repr);
    geomtools::geom_id padGid;
    iss >> padGid;
    std::clog << "Pad GID = " << padGid << '\n';
    const geomtools::geom_info & padGeomInfo
      = geoMgr.get_mapping().get_geom_infos().find(padGid)->second;
    const geomtools::logical_volume & padLogVol =
      padGeomInfo.get_logical();
    
    std::string vgMgrConfigPath
      = "${SNRS_TESTING_DIR}/config/vg_mgr.conf";
    datatools::fetch_path_with_env(vgMgrConfigPath);
    genvtx::manager vgMgr;
    datatools::properties vgMgrConfig;
    datatools::properties::read_config(vgMgrConfigPath, vgMgrConfig);

    std::string vgName1 = "strip_32_pad_0_surface.strange_hot_spot.vg";
    std::string vgName2 = "strip_33_pad_0_surface.batman_spot.vg";
    std::string vgName3 = "strip_32_pad_0_surface.strange_hot_spot_2.vg";
    std::string vgName4 = "strip_34_pad_0_surface.strange_hot_spot.vg";
    std::string vgName5 = "strip_34_pad_0_surface.strange_hot_spot_2.vg";
    std::string vgName6 = "strip_33_pad_0_surface.strange_hot_spot.vg";
    std::string vgName7 = "strip_34_pad_0_film_bulk.vg";

    int32_t seed = 314159;
    vgMgr.set_geometry_manager(geoMgr);
    vgMgr.set_rng_id("taus2");
    vgMgr.set_random_seed(seed);
    vgMgr.initialize(vgMgrConfig);
    vgMgr.tree_dump(std::clog, "The vertex generator manager: ");
    
    const genvtx::i_vertex_generator & myConstVg1 = vgMgr.get(vgName1);
    myConstVg1.tree_dump(std::cerr, "VG #1:");
    const genvtx::i_vertex_generator & myConstVg2 = vgMgr.get(vgName2);
    myConstVg2.tree_dump(std::cerr, "VG #2:");
    const genvtx::i_vertex_generator & myConstVg7 = vgMgr.get(vgName7);
    myConstVg7.tree_dump(std::cerr, "VG #7:");

    // A display object that contains all vertex rendering infos:
    geomtools::display_data dd;

    // The 3D representation of a vertex :
    double tiny = 1.0;
    double vertex_dim = 1.0 * CLHEP::mm;
    vertex_dim *= tiny;
    geomtools::blur_spot vertex_spot(3, 1 * CLHEP::mm);
    vertex_spot.set_errors(vertex_dim, vertex_dim, vertex_dim);
    std::size_t vtx_id = 0;

    // First generator:
    vgMgr.activate_current_vg(vgName1);
    std::size_t vtx_counter = 0;
    std::size_t nshoots = 5000;
    while (vtx_counter < nshoots && vgMgr.can_shoot_vertex()) {
      geomtools::vector_3d vertex;
      vgMgr.shoot_vertex(vertex);
      {
        std::ostringstream vertex_name_oss;
        vertex_name_oss << "vertex_" << vtx_id++;
        geomtools::display_data::display_item & vertex_spot_DI
          = dd.add_static_item(vertex_name_oss.str(),
                               "group::vertex",
                               "blue");
        geomtools::placement vertex_plcmt;
        vertex_plcmt.set_translation(vertex);
        vertex_spot.generate_wires(vertex_spot_DI.wires, vertex_plcmt);
      }
      vtx_counter++;
    }

    // Second generator:
    vtx_counter = 0;
    vgMgr.activate_current_vg(vgName2);
    nshoots = 5000;
    while (vtx_counter < nshoots && vgMgr.can_shoot_vertex()) {
      geomtools::vector_3d vertex;
      vgMgr.shoot_vertex(vertex);
      {
        std::ostringstream vertex_name_oss;
        vertex_name_oss << "vertex_" << vtx_id++;
        geomtools::display_data::display_item & vertex_spot_DI
          = dd.add_static_item(vertex_name_oss.str(),
                               "group::vertex",
                               "black");
        geomtools::placement vertex_plcmt;
        vertex_plcmt.set_translation(vertex);
        vertex_spot.generate_wires(vertex_spot_DI.wires, vertex_plcmt);
      }
      vtx_counter++;
    }
    
    // Third generator:
    vtx_counter = 0;
    vgMgr.activate_current_vg(vgName3);
    nshoots = 5000;
    while (vtx_counter < nshoots && vgMgr.can_shoot_vertex()) {
      geomtools::vector_3d vertex;
      vgMgr.shoot_vertex(vertex);
      {
        std::ostringstream vertex_name_oss;
        vertex_name_oss << "vertex_" << vtx_id++;
        geomtools::display_data::display_item & vertex_spot_DI
          = dd.add_static_item(vertex_name_oss.str(),
                               "group::vertex",
                               "red");
        geomtools::placement vertex_plcmt;
        vertex_plcmt.set_translation(vertex);
        vertex_spot.generate_wires(vertex_spot_DI.wires, vertex_plcmt);
      }
      vtx_counter++;
    }
    
    // Fourth generator:
    vtx_counter = 0;
    vgMgr.activate_current_vg(vgName4);
    nshoots = 5000;
    while (vtx_counter < nshoots && vgMgr.can_shoot_vertex()) {
      geomtools::vector_3d vertex;
      vgMgr.shoot_vertex(vertex);
      {
        std::ostringstream vertex_name_oss;
        vertex_name_oss << "vertex_" << vtx_id++;
        geomtools::display_data::display_item & vertex_spot_DI
          = dd.add_static_item(vertex_name_oss.str(),
                               "group::vertex",
                               "green");
        geomtools::placement vertex_plcmt;
        vertex_plcmt.set_translation(vertex);
        vertex_spot.generate_wires(vertex_spot_DI.wires, vertex_plcmt);
      }
      vtx_counter++;
    }
    
    // Fifth generator:
    vtx_counter = 0;
    vgMgr.activate_current_vg(vgName5);
    nshoots = 10000;
    while (vtx_counter < nshoots && vgMgr.can_shoot_vertex()) {
      geomtools::vector_3d vertex;
      vgMgr.shoot_vertex(vertex);
      {
        std::ostringstream vertex_name_oss;
        vertex_name_oss << "vertex_" << vtx_id++;
        geomtools::display_data::display_item & vertex_spot_DI
          = dd.add_static_item(vertex_name_oss.str(),
                               "group::vertex",
                               "blue");
        geomtools::placement vertex_plcmt;
        vertex_plcmt.set_translation(vertex);
        vertex_spot.generate_wires(vertex_spot_DI.wires, vertex_plcmt);
      }
      vtx_counter++;
    }
   
    // Sixth generator:
    vtx_counter = 0;
    vgMgr.activate_current_vg(vgName6);
    nshoots = 10000;
    while (vtx_counter < nshoots && vgMgr.can_shoot_vertex()) {
      geomtools::vector_3d vertex;
      vgMgr.shoot_vertex(vertex);
      {
        std::ostringstream vertex_name_oss;
        vertex_name_oss << "vertex_" << vtx_id++;
        geomtools::display_data::display_item & vertex_spot_DI
          = dd.add_static_item(vertex_name_oss.str(),
                               "group::vertex",
                               "blue");
        geomtools::placement vertex_plcmt;
        vertex_plcmt.set_translation(vertex);
        vertex_spot.generate_wires(vertex_spot_DI.wires, vertex_plcmt);
      }
      vtx_counter++;
    }
   
    // Seventh generator:
    vtx_counter = 0;
    vgMgr.activate_current_vg(vgName7);
    nshoots = 1000;
    while (vtx_counter < nshoots && vgMgr.can_shoot_vertex()) {
      geomtools::vector_3d vertex;
      vgMgr.shoot_vertex(vertex);
      {
        std::ostringstream vertex_name_oss;
        vertex_name_oss << "vertex_" << vtx_id++;
        geomtools::display_data::display_item & vertex_spot_DI
          = dd.add_static_item(vertex_name_oss.str(),
                               "group::vertex",
                               "orange");
        geomtools::placement vertex_plcmt;
        vertex_plcmt.set_translation(vertex);
        vertex_spot.generate_wires(vertex_spot_DI.wires, vertex_plcmt);
      }
      vtx_counter++;
    }

    std::string visu_object;
    // Now the rendering :
    if (draw) {
      if (visu_object.empty()) {
        visu_object = geoMgr.get_world_name ();
      }
      std::cerr << "Visu object label : '" << visu_object << "'" << std::endl;
      geomtools::gnuplot_drawer GPD;
      GPD.grab_properties().store(geomtools::gnuplot_drawer::world_name_key(),
                                  geoMgr.get_world_name ());
      geomtools::placement dd_pl;
      dd_pl.set_translation(0.0, 0.0, 0.0);
      GPD.add_display_data(dd, dd_pl);
      // GPD.set_view(geomtools::gnuplot_drawer::view_3d());
      GPD.set_view(geomtools::gnuplot_drawer::view_2d_yz());
      GPD.set_mode(geomtools::gnuplot_drawer::mode_wired());
      int view_code = GPD.draw(geoMgr,
                               visu_object,
                               100);
      if (view_code != 0) {
        std::cerr << "ERROR: " << "Cannot display the object with label '"
                  << geoMgr.get_world_name () << "' !" << std::endl;
      }
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
