// Standard Library:
#include <iostream>
#include <exception>
#include <fstream>

// Boost
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

// Bayeux:
#include <bayeux/datatools/exception.h>
#include <bayeux/datatools/logger.h>
#include <bayeux/datatools/properties.h>
#include <bayeux/datatools/utils.h>
#include <bayeux/datatools/units.h>
#include <bayeux/geomtools/manager.h>

// This project:
#include <snrs/sngeom.hpp>
#include <snrs/ltd.hpp>

namespace bpo = boost::program_options;

struct app_config_type
{
  static const uint32_t INVALID_STRIP_ID = 1000;
  static const uint32_t INVALID_PAD_ID   = 1000;
  static const uint32_t DEFAULT_PAD_ID   =    0;

  datatools::logger::priority logging = datatools::logger::PRIO_NOTICE;
  uint32_t    stripId = INVALID_STRIP_ID;
  uint32_t    padId   = DEFAULT_PAD_ID;
  std::string rawLtdDir;
  std::string outputLtdDir;
  bool        doPlot = false;
  bool        doTestReload= false;  
};

void app_help(const bpo::options_description & od_);


// std::string ltd_dir = "/data/SuperNEMO/RealSources/3D_measurements/source_foils";
// std::string data_dir = "/tmp";
// int stripId = 33;
 
int main(int argc_, char * argv_[])
{
  int error_code = EXIT_SUCCESS;
  try {
    std::string verbosityLabel;
    app_config_type appConfig;

    bpo::options_description optDesc("Options");
    // clang-format off
    optDesc.add_options()("help,h", "print this help message")
      
      ("verbosity,V", bpo::value<std::string>(&verbosityLabel)->value_name("level"),
       "set the verbosity level\n"
       "Example: \n"
       "  -V \"debug\" ")
  
      ("strip-id,s", bpo::value<uint32_t>(&appConfig.stripId)->value_name("number"),
       "Strip ID\n"
       "Examples: \n"
       "  -s 34 ")
  
      ("pad-id,p", bpo::value<uint32_t>(&appConfig.padId)->value_name("number")->default_value(0),
       "Pad ID\n"
       "Examples: \n"
       "  -p 0 ")
       
      ("raw-ltd-directory,R", bpo::value<std::string>(&appConfig.rawLtdDir)->value_name("path"),
       "set the input raw Laser Tracking Data (LTD) directory\n"
       "Example: \n"
       "  -R \"/data/SuperNEMO/RealSources/SNLTD_3D_measurements\"")
       
      ("output-ltd-directory,L", bpo::value<std::string>(&appConfig.outputLtdDir)->value_name("path"),
       "set the output preprocessed Laser Tracking Data (LTD) directory\n"
       "Example: \n"
       "  -L \"/data/SuperNEMO/Geometry/RealisticSources\"")
        
      ("plot,P", bpo::value<bool>(&appConfig.doPlot)->default_value(false)->zero_tokens(),
       "generate plot files\n"
       "Example: \n"
       "  -P")
         
      ("test-reload,T", bpo::value<bool>(&appConfig.doTestReload)->default_value(false)->zero_tokens(),
       "perform a reload test from generated file\n"
       "Example: \n"
       "  -T")
 
      ;
    // clang-format on
    bpo::variables_map vMap;
    bpo::store(bpo::parse_command_line(argc_, argv_, optDesc), vMap);
    bpo::notify(vMap);
    
    // Handle any non-bound options
    if (vMap.count("help") != 0u) {
      app_help(optDesc);
      return 0;
    }

    if (vMap.count("verbosity") != 0u) {
      appConfig.logging = datatools::logger::get_priority(verbosityLabel);
      if (appConfig.logging == datatools::logger::PRIO_UNDEFINED) {
        DT_THROW(std::logic_error, "Invalid logging level!");
      }
    }

    DT_THROW_IF(appConfig.rawLtdDir.empty(), std::logic_error, "Missing raw LTD input directory!");
    DT_THROW_IF(appConfig.outputLtdDir.empty(), std::logic_error, "Missing LTD output directory!");
    DT_THROW_IF(appConfig.stripId == app_config_type::INVALID_STRIP_ID, std::logic_error, "Missing strip ID!");

    DT_LOG_NOTICE(appConfig.logging, "Input raw LTD directory  : " << appConfig.rawLtdDir);
    DT_LOG_NOTICE(appConfig.logging, "Output LTD directory : " << appConfig.outputLtdDir);
    DT_LOG_NOTICE(appConfig.logging, "Strip ID : " << appConfig.stripId);
    DT_LOG_NOTICE(appConfig.logging, "Pad ID   : " << appConfig.padId);

    datatools::fetch_path_with_env(appConfig.rawLtdDir);
    datatools::fetch_path_with_env(appConfig.outputLtdDir);
    if (not boost::filesystem::is_directory(appConfig.rawLtdDir)) {
      DT_THROW(std::runtime_error, "Raw LTD input directory '" << appConfig.rawLtdDir << "' does not exist!");
    }
    if (not boost::filesystem::is_directory(appConfig.outputLtdDir)) {
      DT_THROW(std::runtime_error, "LTD output directory '" << appConfig.outputLtdDir << "' does not exist!");
    }
   
    snrs::sngeom myGeom;
    DT_LOG_NOTICE(appConfig.logging, "Geom ok");
    const snrs::strip & myStrip = myGeom.get_strips()[appConfig.stripId];
    int blockId = myStrip.blkid;

    DT_LOG_NOTICE(appConfig.logging, "Strip ID = " << appConfig.stripId);
    DT_LOG_NOTICE(appConfig.logging, "Block ID = " << blockId);
    snrs::ltd myLtd(myGeom, appConfig.logging);
    {
      std::string rawLtdFilename = appConfig.rawLtdDir + '/' + "source_foils" + '/' + snrs::ltd::block_filename(blockId);
      DT_LOG_NOTICE(appConfig.logging, "Importing raw laser tracking points from '"
                    << rawLtdFilename << "'...");
      double densityRadius = 1.0 * CLHEP::cm;
      bool useVoxelization = true;
      myLtd.import(rawLtdFilename, appConfig.stripId, densityRadius, useVoxelization);
      DT_LOG_NOTICE(appConfig.logging, "Import is done.");
    }
    myLtd.print(std::clog);
    {
      std::string ltdFilename =
        appConfig.outputLtdDir + '/' + "ltd-strip-" + std::to_string(appConfig.stripId) + ".dat";
      DT_LOG_NOTICE(appConfig.logging, "Storing laser tracking points in '"
                    << ltdFilename << "'...");
      std::ofstream ltdFile(ltdFilename);
      myLtd.store(ltdFile);
      ltdFile.close();
      DT_LOG_NOTICE(appConfig.logging, "Storage is done.");
    }

    if (appConfig.doPlot) {
      std::string plotLtdFilename =
        appConfig.outputLtdDir + '/' + "images" + '/' + "ltd-strip-" + std::to_string(appConfig.stripId) + ".plot";
      DT_LOG_NOTICE(appConfig.logging, "Storing laser tracking points in '"
                    << plotLtdFilename << "'...");
      std::ofstream plotLtdFile(plotLtdFilename);
      myLtd.plot(plotLtdFile);
      plotLtdFile.close();
      DT_LOG_NOTICE(appConfig.logging, "Plot is done.");
    }

    if (appConfig.doTestReload) {
      DT_LOG_NOTICE(appConfig.logging, "Reloading LTC data...");
      snrs::ltd myLtd2(myGeom, appConfig.logging);
      std::string ltdFilename =
        appConfig.outputLtdDir + '/' + "ltd-strip-" + std::to_string(appConfig.stripId) + ".dat";
      DT_LOG_NOTICE(appConfig.logging, "Loading laser tracking points in '"
                    << ltdFilename << "'...");
      myLtd2.load(ltdFilename);
      DT_LOG_NOTICE(appConfig.logging, "Load is done.");
      std::clog << "LTD (loaded):" << '\n';
      myLtd2.print(std::clog);

      DT_LOG_NOTICE(appConfig.logging, "Extracting a z-band...");
      double z0 = 0.0 * CLHEP::mm;
      double dz = 1.5 * CLHEP::mm;
      snrs::ltd_zband zband;
      myLtd2.extract_zband(z0, dz, zband);
      if (appConfig.doPlot) {
        std::string zbandLtdPlotFilename =
          appConfig.outputLtdDir + '/' + "images" + '/' + "ltd-strip-" + std::to_string(appConfig.stripId) + "-zband-test.plot";
        std::ofstream zbandLtdPlotFile(zbandLtdPlotFilename);
        for (auto point_id : zband.point_ids) {
          const geomtools::vector_3d & pi = myLtd2.get(point_id);
          zbandLtdPlotFile << pi.x() << ' ' << pi.y() << ' '  << pi.z() << '\n';
        }
        zbandLtdPlotFile << '\n';
        zbandLtdPlotFile.close();
      }
    }
    
  } catch (std::exception & error) {
    DT_LOG_ERROR(datatools::logger::PRIO_ERROR, error.what());
    error_code = EXIT_FAILURE;
  } catch (...) {
    DT_LOG_ERROR(datatools::logger::PRIO_ERROR, "An unexpected error occured!");
    error_code = EXIT_FAILURE;
  }
  return error_code;
}

void app_help(const bpo::options_description & od_)
{
  std::cout << "snrs-fsfs : SuperNEMO Realistic Source Modeling\n";
  std::cout << "Usage:\n"
            << "  snrs-fsfs [options]\n"
            << od_ << "\n";
}
