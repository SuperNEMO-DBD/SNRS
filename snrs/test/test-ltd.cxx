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
#include <snrs/sngeom.hpp>
#include <snrs/ltd.hpp>

int main()
{
  datatools::logger::priority logging = datatools::logger::PRIO_NOTICE;
  try {

    std::string ltd_dir = "/data/SuperNEMO/RealSources/3D_measurements/source_foils";
    std::string data_dir = "/tmp";
    int stripId = 33;
 
    snrs::sngeom myGeom;
    DT_LOG_NOTICE(logging, "geom ok");
    const snrs::strip & myStrip = myGeom.get_strips()[stripId];
    int blockId = myStrip.blkid;

    snrs::ltd myLtd(myGeom, logging);
    {
      std::string raw_ltd_ifilename = ltd_dir + '/' + snrs::ltd::block_filename(blockId);
      DT_LOG_NOTICE(logging, "Importing raw laser tracking points from '"
                    << raw_ltd_ifilename << "'...");
      double densityRadius = 1.0 * CLHEP::cm;
      bool useVoxelization = true;
      myLtd.import(raw_ltd_ifilename, stripId, densityRadius, useVoxelization);
      DT_LOG_NOTICE(logging, "Import is done.");
    }
    myLtd.print(std::clog);
    {
      std::string ltd_ofilename =
        data_dir + "/tmp/" + "ltd-strip-" + std::to_string(stripId) + ".dat";
      DT_LOG_NOTICE(logging, "Storing laser tracking points in '"
                    << ltd_ofilename << "'...");
      std::ofstream oltd(ltd_ofilename);
      myLtd.store(oltd);
      oltd.close();
      DT_LOG_NOTICE(logging, "Storage is done.");
    }

    {
      std::string plot_ltd_ofilename =
        data_dir + "/tmp/" + "ltd-strip-" + std::to_string(stripId) + ".plot";
      DT_LOG_NOTICE(logging, "Storing laser tracking points in '"
                    << plot_ltd_ofilename << "'...");
      std::ofstream poltd(plot_ltd_ofilename);
      myLtd.plot(poltd);
      poltd.close();
      DT_LOG_NOTICE(logging, "Plot is done.");
    }

    {
      DT_LOG_NOTICE(logging, "Reloading LTC data...");
      snrs::ltd myLtd2(myGeom, logging);
      std::string ltd_ifilename =
        data_dir + "/tmp/" + "ltd-strip-" + std::to_string(stripId) + ".dat";
      DT_LOG_NOTICE(logging, "Loading laser tracking points in '"
                    << ltd_ifilename << "'...");
      myLtd2.load(ltd_ifilename);
      DT_LOG_NOTICE(logging, "Load is done.");
      std::clog << "LTD (loaded):" << '\n';
      myLtd2.print(std::clog);

      double z0 = 0.0 * CLHEP::mm;
      double dz = 1.5 * CLHEP::mm;
      snrs::ltd_zband zband;
      myLtd2.extract_zband(z0, dz, zband);
      std::string zltd_ifilename =
        data_dir + "/tmp/" + "ltd-strip-" + std::to_string(stripId) + "-zband.plot";
      std::ofstream ozltd(zltd_ifilename);
      for (auto point_id : zband.point_ids) {
        const geomtools::vector_3d & pi = myLtd2.get(point_id);
        ozltd << pi.x() << ' ' << pi.y() << ' '  << pi.z() << '\n';
      }
      ozltd << '\n';
      ozltd.close();
      
    }
    
  } catch (std::exception & error) {
    DT_LOG_ERROR(logging, error.what());
    return 1;
  }
  return 0;
}
