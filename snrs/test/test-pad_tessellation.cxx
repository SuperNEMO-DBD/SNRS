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
#include <bayeux/datatools/temporary_files.h>
#include <bayeux/geomtools/gnuplot_draw.h>
#include <bayeux/geomtools/tessellation.h>
#include <bayeux/geomtools/face_identifier.h>
#include <bayeux/geomtools/gdml_writer.h>
#if GEOMTOOLS_WITH_GNUPLOT_DISPLAY == 1
#include <bayeux/geomtools/gnuplot_i.h>
#include <bayeux/geomtools/gnuplot_drawer.h>
#endif // GEOMTOOLS_WITH_GNUPLOT_DISPLAY


// This project:
// Ourselves:
#include <snrs/pad_tessellation.hpp>
#include <snrs/pad.hpp>

int main()
{
  int error_code = EXIT_SUCCESS;
  datatools::logger::priority logging = datatools::logger::PRIO_NOTICE;
  bool doDraw = true;
  
  try {
    std::string fsfDir = "/data/SuperNEMO/snrs_data/geometry/source_foils/fsf";
    int stripId = 34;
    int padId = 0;
    snrs::pad myPad;
    std::string shapedPadDataFilename = fsfDir
      + '/'
      + "strip-" + std::to_string(stripId)
      + "-pad-" + std::to_string(padId)
      + "-shaped.dat";        std::ifstream shapedPadDataFile(shapedPadDataFilename);
    myPad.load(shapedPadDataFile);
    DT_LOG_NOTICE(logging, "Load is done.");
    std::clog << "Shaped pad (reloaded):" << '\n';
    myPad.print(std::clog);
    
    geomtools::vector_3d padOrigin;
    snrs::pad_tessellator myPadTessellator(myPad, padOrigin);

    geomtools::tessellated_solid myTessellatedPad;
   
    myPadTessellator.make_tessellated_solid(myTessellatedPad);
    
    geomtools::placement padPlacement;
    padPlacement.set_identity();

    datatools::temp_file tmp_file;
    tmp_file.set_remove_at_destroy(true);
    tmp_file.create("/tmp", "snrs-test-pad_tessellation");

    {
      tmp_file.out() << "# draw (index 0)" << std::endl;
      geomtools::gnuplot_draw::draw_tessellated(tmp_file.out(),
                                                padPlacement,
                                                myTessellatedPad,
                                                geomtools::i_wires_3d_rendering::WR_NONE
                                                | geomtools::i_wires_3d_rendering::WR_BASE_BOUNDINGS
                                                | geomtools::tessellated_solid::WR_TESSELLA_ALL_SEGMENTS
                                                );
      tmp_file.out() << std::endl << std::endl;
      tmp_file.close();
    }
    
#if GEOMTOOLS_WITH_GNUPLOT_DISPLAY == 1
    if (doDraw) {
      Gnuplot g1("lines");
      {
        std::ostringstream title;
        title << "set title 'Tessellated pad #34'";
        g1.cmd(title.str());
      }
      g1.cmd("set grid");
      g1.cmd("set size ratio -1");
      g1.cmd("set view equal xyz");
      g1.cmd("set xrange [*:*]");
      g1.cmd("set yrange [*:*]");
      g1.cmd("set zrange [1000:1350]");
      g1.cmd("set xyplane at 1000");
      g1.set_xlabel("x").set_ylabel("y").set_zlabel("z");

      {
        std::ostringstream plot_cmd;
        plot_cmd << "splot '" << tmp_file.get_filename () << "' index 0 notitle  with lines ";
        // plot_cmd << ", '' index 1 using 1:2:3:4 title 'Locate' with points pt 6 ps 0.3 linecolor variable ";
        g1.cmd (plot_cmd.str ());
        g1.showonscreen (); // window output
        geomtools::gnuplot_drawer::wait_for_key ();
        usleep (200);
      }

    }
#endif // GEOMTOOLS_WITH_GNUPLOT_DISPLAY

    
  } catch (std::exception & error) {
    DT_LOG_ERROR(datatools::logger::PRIO_ERROR, error.what());
    error_code = EXIT_FAILURE;
  } catch (...) {
    DT_LOG_ERROR(datatools::logger::PRIO_ERROR, "An unexpected error occured!");
    error_code = EXIT_FAILURE;
  }
  return error_code;
}
        
