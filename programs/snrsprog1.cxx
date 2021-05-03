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
#include <snrs/random.hpp>
#include <snrs/simann.hpp>
#include <snrs/elastic_pad.hpp>

int main()
{
  datatools::logger::priority logging = datatools::logger::PRIO_NOTICE;
  try {

    // geomtools::manager geoMgr;
    // std::string geoMgrCfgFile{"@snrsg:config/geoMgr.conf"};
    // DT_LOG_NOTICE(logging, "geoMgrCfgFile=" << geoMgrCfgFile);
    // datatools::fetch_path_with_env(geoMgrCfgFile);
    // datatools::properties geoMgrCfg;
    // datatools::properties::read_config(geoMgrCfgFile,
    //                                    geoMgrCfg);
    // geoMgr.initialize(geoMgrCfg);
    
    unsigned long int seed = 314159;
    randomizer rng(seed);

    elastic_pad ePad;
    ePad.initialize();
    vtx_id vid(3, 6);
    std::cerr << "VID = " << vid << '\n';
    ePad[vid].z = 13.0 * CLHEP::mm;
    std::cerr << "V = " << ePad[vid] << '\n';

    double E = ePad.compute_energy();
    std::cerr << "E = " << E << '\n';
     
    {
      std::ofstream fPadNet("padNet.data");
      ePad.print(fPadNet);
      fPadNet.close();
    }
    
    simann simAnn;
    
  } catch (std::exception & error) {
    DT_LOG_ERROR(logging, error.what());
    return 1;
  }
  return 0;
}
