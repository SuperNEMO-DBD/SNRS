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
#include <snrs/shmap.hpp>
#include <snrs/ltd.hpp>
#include <snrs/sngeom.hpp>
#include <snrs/fss.hpp>

int main()
{
  datatools::logger::priority logging = datatools::logger::PRIO_ALWAYS;
  try {

    int stripId = 34;
    
    sngeom myGeom;
    DT_LOG_NOTICE(logging, "geom ok");
    
    const strip & myStrip = myGeom.get_strips()[stripId];
    std::clog << "\nStrip #" << stripId << " : \n";
    myStrip.print(std::clog);
    DT_LOG_NOTICE(logging, "Strip ok");

    int blockId = myStrip.blkid;
    const block & myBlock = myGeom.get_blocks()[blockId];
    std::clog << "\nBlock #" << blockId << " : \n";
    myBlock.print(std::clog);
    DT_LOG_NOTICE(logging, "Block ok");
   
    ltd myLtd(myGeom, logging);
    {
      std::string ltd_ifilename = "/data/SuperNEMO/RealSources/3D_measurements/source_foils/tmp/ltd-strip-" + std::to_string(stripId) + ".dat";
      myLtd.load(ltd_ifilename);
    }
    myLtd.print(std::clog);
    DT_LOG_NOTICE(logging, "Ltd ok");

    shmap myMap;
    std::string shmap_filename = "/data/SuperNEMO/RealSources/3D_measurements/source_foils/tmp/shmap-strip-" + std::to_string(stripId) + ".dat";
    myMap.load(shmap_filename);
    std::clog << "\nShaping map: \n";
    myMap.dump(std::clog);
    DT_LOG_NOTICE(logging, "Shaping map ok");

    double muL = 1.0;
    double muT = 1.0;
    // muL = 100.0;
    muL = 0.1;
    // muT = 0.1;
    // muL = 2.1;
    // muT = 2.1;
    elastic_pad myPad(myStrip.get_model().width,
                      myStrip.get_model().height,
                      myStrip.get_model().thickness,
                      20,
                      200,
                      geomtools::vector_3d(0.0,
                                           myGeom.get_strip_ypos()[stripId],
                                           0.0),
                      muL,
                      muT);
    std::clog << "\nSource pad: \n";
    myPad.dump(std::clog);
   
    {
      std::string pad_plot_filename = "/data/SuperNEMO/RealSources/3D_measurements/source_foils/tmp/strip-" + std::to_string(stripId) + "-pad-0.plot";
      std::ofstream opadplot(pad_plot_filename);
      myPad.print(opadplot);
      opadplot.close();
      std::clog << "\nPad plot: " << pad_plot_filename << "\n";
    }
    DT_LOG_NOTICE(logging, "Pad ok");
    
    double lambda = 1.0;
    double viscosity = 1.0e8;
    viscosity = 0.0;
    viscosity = 1.0;
    viscosity = 1.0e7;
    foil_shaping_system myFss(myPad, myMap, lambda, viscosity);
    DT_LOG_NOTICE(logging, "FSS is running...");
    double dt = 0.005;
    uint16_t maxloops = 20;
    int nruns = 10;
    elastic_pad lastPad(myPad);
    for (int irun = 0; irun < nruns; irun++) {
      myFss.run(dt, maxloops);
      DT_LOG_NOTICE(logging, "FSS run ok");
      double d = myPad.distance(lastPad);
      std::clog << "Distance to the last pad: " <<d  << "\n";
      lastPad = myPad;
      {
        std::string pad_plot_filename = "/data/SuperNEMO/RealSources/3D_measurements/source_foils/tmp/strip-" + std::to_string(stripId) + "-pad-0-shaped-run-" + std::to_string(irun) + ".plot";
        std::ofstream opadplot(pad_plot_filename);
        myPad.print(opadplot);
        opadplot.close();
        std::clog << "\nShaped pad plot: " << pad_plot_filename << "\n";
        
      }
    }
    DT_LOG_NOTICE(logging, "Shaped pad ok");
    
  } catch (std::exception & error) {
    DT_LOG_ERROR(logging, error.what());
    return 1;
  }
  return 0;
}
