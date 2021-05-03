// Standard Library:
#include <iostream>
#include <exception>
#include <fstream>
#include <string>

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
#include <bayeux/geomtools/tessellation.h>

// This project:
#include <snrs/ltd.hpp>
#include <snrs/sngeom.hpp>
#include <snrs/fsfs.hpp>
#include <snrs/pad_tessellation.hpp>
#include <snrs/pad.hpp>

namespace bpo = boost::program_options;

struct app_config_type
{
  static const uint32_t INVALID_STRIP_ID = 1000;
  static const uint32_t INVALID_PAD_ID   = 1000;
  static const uint32_t DEFAULT_PAD_ID   =    0;

  datatools::logger::priority logging = datatools::logger::PRIO_NOTICE;
  uint32_t    stripId = INVALID_STRIP_ID;
  uint32_t    padId   = DEFAULT_PAD_ID;
  std::string ltdDir;
  std::string fsfDir;
  bool        doPlot = false;
  bool        doTestReload= false;  
  double      padThickness = -1.0 * CLHEP::mm; // For debug purpose only
  uint32_t    padNy = 20;
  uint32_t    padNz = 200;
  bool        noElliptic    = false;
  bool        noPolynomial2 = true;
  bool        noPolynomial3 = true;
  bool        noPolynomial4 = true;
  bool        fixedY0 = false;
  double      fitEpsAbs = 1.0;
  double      zbandWidth = 1.5 * CLHEP::mm;
  double      yEdgeSafe = 10.0 * CLHEP::mm;
  double      sigmaX = 0.5 * CLHEP::mm;
  int         kzMin = -1;
  int         kzMax = -1;
  bool        doDisplay = false;
  
  app_config_type() = default;
  void initialize(const datatools::properties & config_);
  void print(std::ostream & out_) const;
};

void app_display(const geomtools::tessellated_solid & ts_);

void app_display_exploded_view(const geomtools::tessellated_solid & sts_,
                               const geomtools::tessellated_solid & bfts_,
                               const geomtools::tessellated_solid & ffts_,
                               double gap_);

void app_help(const bpo::options_description & od_);

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
       
      ("ltd-directory,L", bpo::value<std::string>(&appConfig.ltdDir)->value_name("path"),
       "Set the input Laser Tracking Data (LTD) directory\n"
       "Example: \n"
       "  -L \"${SNRS_DATA_DIR}/geometry/source_foils/ltd\"")
       
      ("fsf-directory,O", bpo::value<std::string>(&appConfig.fsfDir)->value_name("path"),
       "Set the output directory\n"
       "Example: \n"
       "  -O \"${SNRS_DATA_DIR}/geometry/source_foils/fsf\"")
        
      ("plot,P", bpo::value<bool>(&appConfig.doPlot)->default_value(false)->zero_tokens(),
       "Generate plot files\n"
       "Example: \n"
       "  -P")
         
      ("test-reload,T", bpo::value<bool>(&appConfig.doTestReload)->default_value(false)->zero_tokens(),
       "perform a reload test from generated shaped pad file\n"
       "Example: \n"
       "  -T")
         
      ("no-fit-elliptic", bpo::value<bool>(&appConfig.noElliptic)->default_value(false)->zero_tokens(),
       "inhibit the elliptic fit\n"
       "Example: \n"
       "  --no-fit-elliptic")
         
      ("no-fit-polynomial4", bpo::value<bool>(&appConfig.noPolynomial4)->default_value(false)->zero_tokens(),
       "inhibit the polynomial 4 fit\n"
       "Example: \n"
       "  --no-fit-polynomial4")
         
      ("fit-fixed-y0,y", bpo::value<bool>(&appConfig.fixedY0)->default_value(false)->zero_tokens(),
       "force the y0 fit parameter to be fixed\n"
       "Example: \n"
       "  --fit-fixed-y0")
       
      ("pad-ny,Y", bpo::value<uint32_t>(&appConfig.padNy)->default_value(20),
       "force the pad segmentation on the Y axis (pad width)\n"
       "Example: \n"
       "  -Y 10")
       
      ("pad-nz,Z", bpo::value<uint32_t>(&appConfig.padNz)->default_value(200),
       "force the pad segmentation on the Z axis (pad height)\n"
       "Example: \n"
       "  -Z 100")
      
      ("pad-thickness,t", bpo::value<double>(&appConfig.padThickness)->default_value(-1.0 * CLHEP::mm),
       "force the pad thickness (in millimeters) [debug only]\n"
       "Example: \n"
       "  -t 1.0")
      
      ("fit-eps-abs,e", bpo::value<double>(&appConfig.fitEpsAbs)->default_value(1.15),
       "set the fit tolerance (no unit)\n"
       "Example: \n"
       "  -e 1.25")
       
      ("z-band-width,W", bpo::value<double>(&appConfig.zbandWidth)->default_value(1.5 * CLHEP::mm),
       "set the Z-band width (in millimeters)\n"
       "Example: \n"
       "  -W 1.5")
       
      ("y-edge-safe,E", bpo::value<double>(&appConfig.yEdgeSafe)->default_value(10.0 * CLHEP::mm),
       "set strip edge safe resolution (in millimeters)\n"
       "Example: \n"
       "  -E 5.0")
       
      ("sigma-x,S", bpo::value<double>(&appConfig.sigmaX)->default_value(1.5 * CLHEP::mm),
       "set the resolution on X axis (in millimeters)\n"
       "Example: \n"
       "  -S 1.5")
       
      ("kzmin", bpo::value<int>(&appConfig.kzMin)->default_value(-1),
       "set the index of the first Z band\n"
       "Example: \n"
       "  -z 0")
        
      ("kzmax", bpo::value<int>(&appConfig.kzMax)->default_value(-1),
       "set the index of the last Z band\n"
       "Example: \n"
       "  -z 101")
       
      ("display", bpo::value<bool>(&appConfig.doDisplay)->default_value(false)->zero_tokens(),
       "display the fitted tessellated solids\n"
       "Example: \n"
       "  --display")
    
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
    
    if (vMap.count("pad-thickness") != 0u) {
      appConfig.padThickness *= CLHEP::mm;
    }
    
    if (vMap.count("z-band-width") != 0u) {
      appConfig.zbandWidth *= CLHEP::mm;
    }
    
    if (vMap.count("y-edge-safe") != 0u) {
      appConfig.yEdgeSafe *= CLHEP::mm;
    }
   
    if (vMap.count("sigma-x") != 0u) {
      appConfig.sigmaX *= CLHEP::mm;
    }

    DT_THROW_IF(appConfig.ltdDir.empty(), std::logic_error, "Missing input raw LTD directory!");
    DT_THROW_IF(appConfig.fsfDir.empty(), std::logic_error, "Missing output FSF directory!");
    DT_THROW_IF(appConfig.stripId == app_config_type::INVALID_STRIP_ID, std::logic_error, "Missing strip ID!");

    DT_LOG_NOTICE(appConfig.logging, "Raw LTD directory    : " << appConfig.ltdDir);
    DT_LOG_NOTICE(appConfig.logging, "Output FSF directory : " << appConfig.fsfDir);
    DT_LOG_NOTICE(appConfig.logging, "Strip ID : " << appConfig.stripId);
    DT_LOG_NOTICE(appConfig.logging, "Pad ID   : " << appConfig.padId);
   
    DT_LOG_NOTICE(appConfig.logging, "App config : ");
    appConfig.print(std::clog);

    /************************************************************************/
    snrs::sngeom myGeom;
    DT_LOG_NOTICE(appConfig.logging, "geom ok");
    
    const snrs::strip & myStrip = myGeom.get_strips()[appConfig.stripId];
    // std::clog << "\nStrip #" << stripId << " : \n";
    // myStrip.print(std::clog);
    DT_LOG_NOTICE(appConfig.logging, "Strip ok");

    int blockId = myStrip.blkid;
    const snrs::block & myBlock = myGeom.get_blocks()[blockId];
    // std::clog << "\nBlock #" << blockId << " : \n";
    // myBlock.print(std::clog);
    DT_LOG_NOTICE(appConfig.logging, "Block ok");
   
    snrs::ltd myLtd(myGeom, appConfig.logging);
    {
      std::string ltdFilename = appConfig.ltdDir + '/'
        + "ltd-strip-" + std::to_string(appConfig.stripId) + ".dat";
      myLtd.load(ltdFilename);
    }
    // myLtd.print(std::clog);
    DT_LOG_NOTICE(appConfig.logging, "Ltd ok");

    double padThickness = myStrip.get_model().thickness;
    if (appConfig.padThickness > 0.0) {
      padThickness = appConfig.padThickness;
    }
    DT_LOG_NOTICE(appConfig.logging, "padThickness = " << padThickness / CLHEP::mm << " mm");
    snrs::pad myPad(myStrip.get_model().name,
                    appConfig.stripId,
                    appConfig.padId,
                    myStrip.get_model().width,
                    myStrip.get_model().height,
                    padThickness, 
                    appConfig.padNy,
                    appConfig.padNz,
                    geomtools::vector_3d(0.0,
                                         myGeom.get_strip_ypos()[appConfig.stripId],
                                         0.0));
    if (myStrip.get_model().has_film) {
      myPad.set_film_thickness(myStrip.get_model().film_thickness);
      if (! myStrip.get_model().film_material.empty()) {
        myPad.set_film_material(myStrip.get_model().film_material);
      } else {
        myPad.set_film_material("basic::mylar");
      }
    }
    if (! myStrip.get_model().material.empty()) {
      myPad.set_material(myStrip.get_model().material);
    } else {
      myPad.set_material("source_material");
    }
    myPad.make_vertexes();
    std::clog << "\nSource pad: \n";
    myPad.print(std::clog);
    
    if (appConfig.doPlot) {
      std::string padPlotFilename = appConfig.fsfDir
        + '/' + "images"
        + '/' + "strip-" + std::to_string(appConfig.stripId)
        + "-pad-" + std::to_string(appConfig.padId) + ".plot";
      std::ofstream padPlotFile(padPlotFilename);
      myPad.plot(padPlotFile);
      padPlotFile.close();
      std::clog << "\nPad plot: " << padPlotFilename << "\n";
    }
    DT_LOG_NOTICE(appConfig.logging, "Pad ok");

    snrs::foil_shaping_fit_system::fit_config_type myFsfsConfig;
    myFsfsConfig.max_iter       = 4000;
    myFsfsConfig.modulo_iter    = 20;
    myFsfsConfig.epsabs         = appConfig.fitEpsAbs;
    myFsfsConfig.zband_width    = appConfig.zbandWidth;
    myFsfsConfig.y_edge_safe    = appConfig.yEdgeSafe;
    myFsfsConfig.no_elliptic    = appConfig.noElliptic;
    myFsfsConfig.no_polynomial2 = appConfig.noPolynomial2;
    myFsfsConfig.no_polynomial3 = appConfig.noPolynomial3;
    myFsfsConfig.no_polynomial4 = appConfig.noPolynomial4;
    myFsfsConfig.sigma_x        = appConfig.sigmaX;
    myFsfsConfig.fixed_y0       = appConfig.fixedY0;
    std::clog << "FSF config: " << '\n',
    myFsfsConfig.print(std::clog);
    
    snrs::foil_shaping_fit_system myFsfs(myFsfsConfig, myPad, myGeom, myLtd, appConfig.logging);
    DT_LOG_NOTICE(appConfig.logging, "FSFS is running...");
    int kzMin = -1;
    int kzMax = -1;
    if (appConfig.kzMin >= 0) {
      kzMin = appConfig.kzMin;
    }
    if (appConfig.kzMax >= 0) {
      kzMax = appConfig.kzMax;
    }
    myFsfs.run(kzMin, kzMax); 
    
    if (appConfig.doPlot) {
      std::string fsfs_plot_filename = appConfig.fsfDir
        + '/' + "images" + '/'
        + "strip-" + std::to_string(appConfig.stripId)
        + "-pad-" + std::to_string(appConfig.padId)
        + "-fsfs-zbands.plot";
      std::ofstream ofsfsplot(fsfs_plot_filename);
      myFsfs.plot_zbands(ofsfsplot);
      ofsfsplot.close();
      std::clog << "\nFSFS zbands plot: " << fsfs_plot_filename << "\n";
    }

    if (appConfig.doPlot) {
      std::string fsfs_plot_filename = appConfig.fsfDir 
        + '/' + "images" + '/'
        + "strip-" + std::to_string(appConfig.stripId)
        + "-pad-" + std::to_string(appConfig.padId)
        + "-fsfs-zfits.plot";
      std::ofstream ofsfsplot(fsfs_plot_filename);
      myFsfs.plot_zfits(ofsfsplot);
      ofsfsplot.close();
      std::clog << "\nFSFS zfits plot: " << fsfs_plot_filename << "\n";
    }
    DT_LOG_NOTICE(appConfig.logging, "FSFS ok");

    int failure_count = 0;
    int success_count = 0;
    const std::vector<snrs::foil_shaping_fit_system::zfit_context_type> & zfits = myFsfs.get_zfits();
    std::string fsfs_report_filename = appConfig.fsfDir
      + '/'
      + "strip-" + std::to_string(appConfig.stripId)
      + "-pad-" + std::to_string(appConfig.padId)
      + "-shaping.report";
    std::clog << "\nFSFS report: " << fsfs_report_filename << "\n";
    std::ofstream freport(fsfs_report_filename);
    freport << "#@strip-id=" << appConfig.stripId << '\n';
    freport << "#@pad-id=" << appConfig.padId << '\n';
    freport << "#@strip.pos.y=" << myPad.get_original_position().y() / CLHEP::mm << '\n';
    freport << "#@pad-ny=" << appConfig.padNy << '\n';
    freport << "#@pad-nz=" << appConfig.padNz << '\n';
    freport << "#@fit-eps-abs=" << appConfig.fitEpsAbs << '\n';
    freport << "#@zband-width=" << appConfig.zbandWidth << '\n';
    freport << "#@y-edge-safe=" << appConfig.yEdgeSafe << '\n';
    freport << "#@sigma-x=" << appConfig.sigmaX << '\n';
    freport << "#@kz-min=" << appConfig.kzMin << '\n';
    freport << "#@kz-max=" << appConfig.kzMax << '\n';
    freport << "#@kZ" << ' ' << "Success" << ' ' << "Maxiter" << ' ';
    freport << "chi2" << ' ' << "ndof" << ' ' << "p_value" << ' ';
    freport << "FitModel" << ' ' << "NbFitParams" << ' ' << "FitParams[*]" << ' ';
    freport << '\n';
    for (const auto & zfitContext : zfits) {
      freport << zfitContext.kz << ' ' << zfitContext.success << ' ' << zfitContext.maxiter << ' ';
      freport << zfitContext.chi2 << ' ' << zfitContext.ndof << ' ' << zfitContext.p_value;
      if (zfitContext.success) {
        success_count++;
        freport
          << ' ' << zfitContext.params.model;
        if (zfitContext.params.model == snrs::foil_shaping_fit_system::ZFIT_MODEL_ELLIPTIC) {
          freport
            << ' ' << 4
            << ' ' << zfitContext.params.ell_a
            << ' ' << zfitContext.params.ell_b
            << ' ' << zfitContext.params.ell_x0
            << ' ' << zfitContext.params.ell_y0;
        }
        if (zfitContext.params.model == snrs::foil_shaping_fit_system::ZFIT_MODEL_POLYNOMIAL2) {
          freport  << ' ' << 4;
        }
        if (zfitContext.params.model == snrs::foil_shaping_fit_system::ZFIT_MODEL_POLYNOMIAL3) {
          freport  << ' ' << 5;
        }
        if (zfitContext.params.model == snrs::foil_shaping_fit_system::ZFIT_MODEL_POLYNOMIAL4) {
          freport  << ' ' << 6;
        }
        if (zfitContext.params.model >= snrs::foil_shaping_fit_system::ZFIT_MODEL_POLYNOMIAL2) {
          freport
            << ' ' << zfitContext.params.pol_y0
            << ' ' << zfitContext.params.pol_coeffs[0]
            << ' ' << zfitContext.params.pol_coeffs[1]
            << ' ' << zfitContext.params.pol_coeffs[2];
        }
        if (zfitContext.params.model >= snrs::foil_shaping_fit_system::ZFIT_MODEL_POLYNOMIAL3) {
          freport
            << ' ' << zfitContext.params.pol_coeffs[3];
        }
        if (zfitContext.params.model >= snrs::foil_shaping_fit_system::ZFIT_MODEL_POLYNOMIAL4) {
          freport
            << ' ' << zfitContext.params.pol_coeffs[4];
        }
      } else {
        failure_count++;
      }
      freport << ' ' << '\n';
    }
    freport << "#@zband-fit-success=" << success_count << '\n';
    freport << "#@zband-fit-failure=" << failure_count << '\n';
    freport.close();
    std::clog << "Z-band fit success : " << success_count << '\n';
    std::clog << "Z-band fit failure : " << failure_count << '\n';

    if (failure_count == 0) {
      myFsfs.shape_pad(myPad);
      {
        std::string shapedPadDataFilename = appConfig.fsfDir
          + '/'
          + "strip-" + std::to_string(appConfig.stripId)
          + "-pad-" + std::to_string(appConfig.padId)
          + "-shaped.dat";
        std::ofstream shapedPadDataFile(shapedPadDataFilename);
        myPad.store(shapedPadDataFile);
        shapedPadDataFile.close();
        std::clog << "\nShaped pad data: " << shapedPadDataFilename << "\n";
      }
      if (appConfig.doPlot){
        std::string shapedPadPlotFilename = appConfig.fsfDir
          + '/' + "images" + '/'
          + "strip-" + std::to_string(appConfig.stripId)
          + "-pad-" + std::to_string(appConfig.padId)
          + "-shaped.plot";
        std::ofstream shapedPadPlotFile(shapedPadPlotFilename);
        myPad.plot(shapedPadPlotFile);
        shapedPadPlotFile.close();
        std::clog << "\nShaped pad plot: " << shapedPadPlotFilename << "\n";
      }
      DT_LOG_NOTICE(appConfig.logging, "Shaped pad ok");

      if (appConfig.doTestReload) {
        DT_LOG_NOTICE(appConfig.logging, "Reloading LTC data...");
        snrs::pad myPad2;
        std::string shapedPadDataFilename = appConfig.fsfDir
          + '/'
          + "strip-" + std::to_string(appConfig.stripId)
          + "-pad-" + std::to_string(appConfig.padId)
          + "-shaped.dat";
        DT_LOG_NOTICE(appConfig.logging, "Loading shaped pad data from '"
                      << shapedPadDataFilename << "'...");
        std::ifstream shapedPadDataFile(shapedPadDataFilename);
        myPad2.load(shapedPadDataFile);
        DT_LOG_NOTICE(appConfig.logging, "Load is done.");
        std::clog << "Shaped pad (reloaded):" << '\n';
        myPad2.print(std::clog);
      }

      geomtools::vector_3d myPadOrigin(0.0,
                                       myGeom.get_strip_ypos()[appConfig.stripId],
                                       0.0);
      snrs::pad_tessellator myPadTessellator(myPad, myPadOrigin);
      snrs::pad::mesh_data_type & myMeshedPad = myPad.grab_source_mesh();
      myPadTessellator.make_tessellated_solid(myMeshedPad, snrs::pad_tessellator::PAD_TARGET_SOURCE);
      {
        std::string tessellatedPadDataFilename = appConfig.fsfDir
          + '/'
          + "strip-" + std::to_string(appConfig.stripId)
          + "-pad-" + std::to_string(appConfig.padId)
          + "-tessellated.dat";
        std::ofstream tessellatedPadDataFile(tessellatedPadDataFilename);
        myMeshedPad.solid.store(tessellatedPadDataFile);
        tessellatedPadDataFile.close();
        std::clog << "\nTessellated pad data: " << tessellatedPadDataFilename << "\n";
      }
      {
        std::string tilesPadDataFilename = appConfig.fsfDir
          + '/'
          + "strip-" + std::to_string(appConfig.stripId)
          + "-pad-" + std::to_string(appConfig.padId)
          + "-tiles.dat";
        std::ofstream tilesPadDataFile(tilesPadDataFilename);
        myMeshedPad.store_tile_maps(tilesPadDataFile);
        tilesPadDataFile.close();
        std::clog << "\nTiles data: " << tilesPadDataFilename << "\n";
      }
      
      if (appConfig.doDisplay) {
        app_display(myMeshedPad.solid);
      }

      if (myPad.has_film()) {
        snrs::pad::mesh_data_type & myMeshedBackFilm = myPad.grab_back_film_mesh();
        snrs::pad::mesh_data_type & myMeshedFrontFilm = myPad.grab_front_film_mesh();
        {
          myPadTessellator.make_tessellated_solid(myMeshedBackFilm,
                                                  snrs::pad_tessellator::PAD_TARGET_BACK_FILM);
          {
            std::string tessellatedBackFilmFilename = appConfig.fsfDir
              + '/'
              + "strip-" + std::to_string(appConfig.stripId)
              + "-pad-" + std::to_string(appConfig.padId)
              + "-tessellated-back-film.dat";
            std::ofstream tessellatedBackFilmFile(tessellatedBackFilmFilename);
            myMeshedBackFilm.solid.store(tessellatedBackFilmFile);
            tessellatedBackFilmFile.close();
            std::clog << "\nTessellated back film data: " << tessellatedBackFilmFilename << "\n";
          }
          if (appConfig.doDisplay) {
            app_display(myMeshedBackFilm.solid);
          }
          {
            std::string tilesBackFilmDataFilename = appConfig.fsfDir
              + '/'
              + "strip-" + std::to_string(appConfig.stripId)
              + "-pad-" + std::to_string(appConfig.padId)
              + "-tiles-back-film.dat";
            std::ofstream tilesBackFilmDataFile(tilesBackFilmDataFilename);
            myMeshedBackFilm.store_tile_maps(tilesBackFilmDataFile);
            tilesBackFilmDataFile.close();
            std::clog << "\nTiles data: " << tilesBackFilmDataFilename << "\n";
          }
        }
        {
           myPadTessellator.make_tessellated_solid(myMeshedFrontFilm,
                                                   snrs::pad_tessellator::PAD_TARGET_FRONT_FILM);
          {
            std::string tessellatedFrontFilmFilename = appConfig.fsfDir
              + '/'
              + "strip-" + std::to_string(appConfig.stripId)
              + "-pad-" + std::to_string(appConfig.padId)
              + "-tessellated-front-film.dat";
            std::ofstream tessellatedFrontFilmFile(tessellatedFrontFilmFilename);
            myMeshedFrontFilm.solid.store(tessellatedFrontFilmFile);
            tessellatedFrontFilmFile.close();
            std::clog << "\nTessellated front film data: " << tessellatedFrontFilmFilename << "\n";
          }
          if (appConfig.doDisplay) {
            app_display(myMeshedFrontFilm.solid);
          }
          {
            std::string tilesFrontFilmDataFilename = appConfig.fsfDir
              + '/'
              + "strip-" + std::to_string(appConfig.stripId)
              + "-pad-" + std::to_string(appConfig.padId)
              + "-tiles-front-film.dat";
            std::ofstream tilesFrontFilmDataFile(tilesFrontFilmDataFilename);
            myMeshedFrontFilm.store_tile_maps(tilesFrontFilmDataFile);
            tilesFrontFilmDataFile.close();
            std::clog << "\nTiles data: " << tilesFrontFilmDataFilename << "\n";
          }
        }
        
        if (appConfig.doDisplay) {
          app_display_exploded_view(myMeshedPad.solid, myMeshedBackFilm.solid, myMeshedFrontFilm.solid, 10 * CLHEP::cm);
        }
           
      }
          
    } else {
      DT_LOG_NOTICE(appConfig.logging, "Some z-band did not fit!");
      error_code = EXIT_FAILURE;
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
  std::cout << "snrs-build-fsf : SuperNEMO Realistic Source Modeling\n";
  std::cout << "Usage:\n"
            << "  snrs-build-fsf [options]\n"
            << od_ << "\n";
  std::cout << "Example:\n" <<
    "  snrs-build-fsf --verbosity \"notice\" \\\n"
    "   --strip-id 3  \\\n"
    "   --ltd-directory \"${SNRS_DATA_DIR}/geometry/source_foils/ltd\" \\\n"
    "   --fsf-directory \"${SNRS_DATA_DIR}/geometry/source_foils/fsf\" \\\n"
    "   --pad-ny=10 \\\n"
    "   --pad-nz=100 \\\n"
    "   --z-band-width=1.5 \\\n"
    "   --y-edge-safe=5.0 \\\n"
    "   --sigma-x=0.5 \\\n"
    "   --plot\n"
    "   --display\n"
    "   \n"
    ;
    
}

void app_config_type::initialize(const datatools::properties & config_)
{
  if (config_.has_key("pad.ny")) {
    padNy = config_.fetch_positive_integer("pad.ny");
    if (padNy < 2) {
      DT_THROW(std::range_error, "Invalid Y segmentation of the pad!");
    }
  }
  if (config_.has_key("pad.nz")) {
    padNz = config_.fetch_positive_integer("pad.nz");
    if (padNz < 2) {
      DT_THROW(std::range_error, "Invalid Z segmentation of the pad!");
    }
  }
}

void app_config_type::print(std::ostream & out_) const
{
  out_ << "|-- " << "stripId  = " << stripId << '\n';
  out_ << "|-- " << "padId  = " << padId << '\n';
  out_ << "|-- " << "ltdDir  = " << ltdDir << '\n';
  out_ << "|-- " << "fsfDir  = " << fsfDir << '\n';
  out_ << "|-- " << "doPlot  = " << std::boolalpha << doPlot << '\n';
  out_ << "|-- " << "doTestReload  = " << std::boolalpha << doTestReload << '\n';
  out_ << "|-- " << "padThickness  = " << padThickness << '\n';
  out_ << "|-- " << "padNy  = " << padNy << '\n';
  out_ << "|-- " << "padNz  = " << padNz << '\n';
  out_ << "|-- " << "noElliptic  = " << std::boolalpha << noElliptic << '\n';
  out_ << "|-- " << "noPolynomial2  = " << std::boolalpha << noPolynomial2 << '\n';
  out_ << "|-- " << "noPolynomial3  = " << std::boolalpha << noPolynomial3 << '\n';
  out_ << "|-- " << "noPolynomial4  = " << std::boolalpha << noPolynomial4 << '\n';
  out_ << "|-- " << "fixedY0  = " << std::boolalpha << fixedY0 << '\n';
  out_ << "|-- " << "fitEpsAbs  = " << fitEpsAbs << '\n';
  out_ << "|-- " << "zbandWidth  = " << zbandWidth << '\n';
  out_ << "|-- " << "yEdgeSafe  = " << yEdgeSafe << '\n';
  out_ << "|-- " << "sigmaX  = " << sigmaX << '\n';
  out_ << "|-- " << "kzMin  = " << kzMin << '\n';
  out_ << "|-- " << "kzMax  = " << kzMax << '\n';
  out_ << "`-- " << "doDisplay = " << doDisplay << '\n';
}

// - Bayeux/datatools:
#include <datatools/temporary_files.h>
#include <datatools/utils.h>

// This project:
#include <geomtools/gnuplot_draw.h>
#include <geomtools/tessellation.h>
#include <geomtools/face_identifier.h>
#include <geomtools/gdml_writer.h>
#if GEOMTOOLS_WITH_GNUPLOT_DISPLAY == 1
#include <geomtools/gnuplot_i.h>
#include <geomtools/gnuplot_drawer.h>
#endif // GEOMTOOLS_WITH_GNUPLOT_DISPLAY

void app_display(const geomtools::tessellated_solid & ts_)
{
#if GEOMTOOLS_WITH_GNUPLOT_DISPLAY == 1
  datatools::temp_file tmp_file;
  tmp_file.set_remove_at_destroy(true);
  tmp_file.create("/tmp", "snrs_tessellated_pad");

  geomtools::vector_3d tessel_pos;
  geomtools::rotation_3d tessel_rot;
  geomtools::create_rotation(tessel_rot, 0.0, 0.0, 0.0);
  geomtools::gnuplot_draw::draw_tessellated(tmp_file.out(),
                                            tessel_pos,
                                            tessel_rot,
                                            ts_,
                                            geomtools::tessellated_solid::WR_TESSELLA_ALL_SEGMENTS);
  tmp_file.out() << std::endl;
  tmp_file.close();
  usleep(200);

  {
    Gnuplot g1 ("lines");
    {
      std::ostringstream title;
      title << "set title '";
      title << "Tessellated solid from SNRS meshing";
      g1.cmd(title.str());
    }
    g1.cmd("set grid");
    // g1.cmd("set size ratio -1");
    // g1.cmd("set view equal xyz");
    g1.cmd("set xyplane at -1400");
    g1.set_xrange(-100*CLHEP::mm, +100*CLHEP::mm).set_zrange(-1500*CLHEP::mm, +1500*CLHEP::mm);
    g1.set_xlabel("x (mm)").set_ylabel("y (mm)").set_zlabel("z (mm)");

    g1.plotfile_xyz(tmp_file.get_filename (), 1, 2, 3, "3D view");
    g1.showonscreen(); // window output
    geomtools::gnuplot_drawer::wait_for_key();
    usleep(200);
  }

#else  // GEOMTOOLS_WITH_GNUPLOT_DISPLAY
  std::cerr << "No display\n";
#endif // GEOMTOOLS_WITH_GNUPLOT_DISPLAY
  return;
}

void app_display_exploded_view(const geomtools::tessellated_solid & sts_,
                               const geomtools::tessellated_solid & bfts_,
                               const geomtools::tessellated_solid & ffts_,
                               double gap_)
{
#if GEOMTOOLS_WITH_GNUPLOT_DISPLAY == 1
  datatools::temp_file tmp_file;
  tmp_file.set_remove_at_destroy(true);
  tmp_file.create("/tmp", "snrs_tessellated_pad");

  geomtools::vector_3d tessel_pos;
  geomtools::vector_3d back_tessel_pos(-gap_, 0.0, 0.0);
  geomtools::vector_3d front_tessel_pos(gap_, 0.0, 0.0);
  geomtools::rotation_3d tessel_rot;
  geomtools::create_rotation(tessel_rot, 0.0, 0.0, 0.0);
  geomtools::gnuplot_draw::draw_tessellated(tmp_file.out(),
                                            tessel_pos,
                                            tessel_rot,
                                            sts_,
                                            geomtools::tessellated_solid::WR_TESSELLA_ALL_SEGMENTS);
  geomtools::gnuplot_draw::draw_tessellated(tmp_file.out(),
                                            back_tessel_pos,
                                            tessel_rot,
                                            bfts_,
                                            geomtools::tessellated_solid::WR_TESSELLA_ALL_SEGMENTS);
  geomtools::gnuplot_draw::draw_tessellated(tmp_file.out(),
                                            front_tessel_pos,
                                            tessel_rot,
                                            ffts_,
                                            geomtools::tessellated_solid::WR_TESSELLA_ALL_SEGMENTS);
  tmp_file.out() << std::endl;
  tmp_file.close();
  usleep(200);

  {
    Gnuplot g1 ("lines");
    {
      std::ostringstream title;
      title << "set title '";
      title << "Exploded 3D-view of the tessellated source strip with its 2 wrapping Mylar films";
      g1.cmd(title.str());
    }
    g1.cmd("set grid");
    g1.cmd("set view equal xyz");
    g1.cmd("set size ratio -1");
    g1.cmd("set xyplane at -1400");
    g1.set_xrange(-100*CLHEP::mm-gap_, +100*CLHEP::mm+gap_).set_zrange(-1500*CLHEP::mm, +1500*CLHEP::mm);
    g1.set_xlabel("x (mm)").set_ylabel("y (mm)").set_zlabel("z (mm)");

    g1.plotfile_xyz(tmp_file.get_filename (), 1, 2, 3, "3D view");
    g1.showonscreen(); // window output
    geomtools::gnuplot_drawer::wait_for_key();
    usleep(200);
  }

#else  // GEOMTOOLS_WITH_GNUPLOT_DISPLAY
  std::cerr << "No display\n";
#endif // GEOMTOOLS_WITH_GNUPLOT_DISPLAY
  return;
}
