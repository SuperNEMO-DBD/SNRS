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

// This project:
// Ourselves:
#include <snrs/pad.hpp>

// Standard library:
#include <cmath>

int main()
{
  datatools::logger::priority logging = datatools::logger::PRIO_NOTICE;
  try {
    
    geomtools::vector_3d padPos(0.0, 0.0, 0.0);
    
    snrs::pad myPad(135.5 * CLHEP::mm,
                    370.0 * CLHEP::mm,
                    2000  * CLHEP::micrometer,
                    10,
                    20,
                    padPos);
    for (int i = snrs::pad::FACE_BACK; i <= snrs::pad::FACE_FRONT; i++) {
      for (int j = 2; j <= 8; j++) {
        for (int k = 4; k <= 12; k++) {
          double dx = 1.5 * CLHEP::cm * std::exp(-0.5 * ((j - 5)*(j - 5) + (k - 8) * (k - 8)));
          snrs::vertex3d_id vid(i, j, k);
          double x = myPad[vid].pos.x();
          myPad[vid].pos.setX(x + dx);
        }
      }
    }
    std::clog << "Pad : " << '\n';
    myPad.print(std::clog);
    std::clog << '\n';

    {
      std::ofstream ofplot("pad.plot");
      myPad.plot(ofplot);
      ofplot.close();
    }


    /* Run:
     * $ ./test-pad > pad.plot 
     * gnuplot> set view equal xyz
     * gnuplot> splot [-100:100][-100:100][-160:160] 'pad.plot' u 4:5:6  w lines
     */
    
  } catch (std::exception & error) {
    DT_LOG_ERROR(logging, error.what());
    return 1;
  }
  return 0;
}
