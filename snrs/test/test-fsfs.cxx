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
#include <snrs/ltd.hpp>
#include <snrs/sngeom.hpp>
#include <snrs/fsfs.hpp>

int main()
{
  int error_code = EXIT_SUCCESS;
  datatools::logger::priority logging = datatools::logger::PRIO_ALWAYS;
  try {

    int stripId = 33;
    
    snrs::sngeom myGeom;
    DT_LOG_NOTICE(logging, "geom ok");
    
    const snrs::strip & myStrip = myGeom.get_strips()[stripId];
    // std::clog << "\nStrip #" << stripId << " : \n";
    // myStrip.print(std::clog);
    DT_LOG_NOTICE(logging, "Strip ok");

    int blockId = myStrip.blkid;
    const snrs::block & myBlock = myGeom.get_blocks()[blockId];
    // std::clog << "\nBlock #" << blockId << " : \n";
    // myBlock.print(std::clog);
    DT_LOG_NOTICE(logging, "Block ok");
   
    snrs::ltd myLtd(myGeom, logging);
    {
      std::string ltd_ifilename = "/data/SuperNEMO/RealSources/3D_measurements/source_foils/tmp/ltd-strip-" + std::to_string(stripId) + ".dat";
      myLtd.load(ltd_ifilename);
    }
    // myLtd.print(std::clog);
    DT_LOG_NOTICE(logging, "Ltd ok");

    snrs::pad myPad(myStrip.get_model().width,
              myStrip.get_model().height,
              1.0 * CLHEP::mm, //myStrip.get_model().thickness,
              20,
              200,
              geomtools::vector_3d(0.0,
                                   myGeom.get_strip_ypos()[stripId],
                                   0.0));
    std::clog << "\nSource pad: \n";
    // myPad.print(std::clog);
    {
      std::string pad_plot_filename = "/data/SuperNEMO/RealSources/3D_measurements/source_foils/tmp/strip-" + std::to_string(stripId) + "-pad-0.plot";
      std::ofstream opadplot(pad_plot_filename);
      myPad.plot(opadplot);
      opadplot.close();
      std::clog << "\nPad plot: " << pad_plot_filename << "\n";
    }
    DT_LOG_NOTICE(logging, "Pad ok");
    
    snrs::foil_shaping_fit_system myFsfs(myPad, myLtd, datatools::logger::PRIO_NOTICE);
    DT_LOG_NOTICE(logging, "FSFS is running...");
    auto & fit_config = myFsfs.grab_fit_config();
    fit_config.max_iter = 4000;
    fit_config.modulo_iter = 20;
    fit_config.epsabs = 1.15;
    fit_config.zband_width = 1.0 * CLHEP::mm;
    fit_config.y_edge_save = 10.0 * CLHEP::mm;
    fit_config.no_polynomial2 = true;
    fit_config.no_polynomial3 = true;
    fit_config.no_polynomial4 = true;
    myFsfs.run(); 
    
    {
      std::string fsfs_plot_filename = "/data/SuperNEMO/RealSources/3D_measurements/source_foils/tmp/strip-" + std::to_string(stripId) + "-fsfs-zbands.plot";
      std::ofstream ofsfsplot(fsfs_plot_filename);
      myFsfs.plot_zbands(ofsfsplot);
      ofsfsplot.close();
      std::clog << "\nFSFS zbands plot: " << fsfs_plot_filename << "\n";
    }

    {
      std::string fsfs_plot_filename = "/data/SuperNEMO/RealSources/3D_measurements/source_foils/tmp/strip-" + std::to_string(stripId) + "-fsfs-zfits.plot";
      std::ofstream ofsfsplot(fsfs_plot_filename);
      myFsfs.plot_zfits(ofsfsplot);
      ofsfsplot.close();
      std::clog << "\nFSFS zfits plot: " << fsfs_plot_filename << "\n";
    }
    DT_LOG_NOTICE(logging, "FSFS ok");

    int failure_count = 0;
    int success_count = 0;
     const std::vector<snrs::foil_shaping_fit_system::zfit_context_type> & zfits = myFsfs.get_zfits();
    for (const auto & zfitContext : zfits) {
      std::clog << zfitContext.kz << ' ' << zfitContext.success << ' ' << zfitContext.maxiter;
      if (zfitContext.success) {
        success_count++;
        std::clog
          << ' ' << zfitContext.params.model
          << ' ' << zfitContext.params.ell_a
          << ' ' << zfitContext.params.ell_b
          << ' ' << zfitContext.params.ell_x0
          << ' ' << zfitContext.params.ell_y0;
      } else {
        failure_count++;
      }
      std::clog << ' ' << '\n';
    }
    std::clog << "Z-band fit success : " << success_count << '\n';
    std::clog << "Z-band fit failure : " << failure_count << '\n';

    if (failure_count == 0) {
      myFsfs.shape_pad(myPad);
      {
        std::string shpad_plot_filename = "/data/SuperNEMO/RealSources/3D_measurements/source_foils/tmp/strip-" + std::to_string(stripId) + "-pad-0-shaped.plot";
        std::ofstream opadplot(shpad_plot_filename);
        myPad.plot(opadplot);
        opadplot.close();
        std::clog << "\nShaped pad plot: " << shpad_plot_filename << "\n";
      }
      DT_LOG_NOTICE(logging, "Shaped pad ok");
    } else {
      error_code = EXIT_FAILURE;
    }
    
  } catch (std::exception & error) {
    DT_LOG_ERROR(logging, error.what());
    error_code = EXIT_FAILURE;
  } catch (...) {
    DT_LOG_ERROR(logging, "An unexpected erro occured!");
    error_code = EXIT_FAILURE;
  }
  return error_code;
}
