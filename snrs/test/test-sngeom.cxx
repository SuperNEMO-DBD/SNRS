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

int main()
{
  datatools::logger::priority logging = datatools::logger::PRIO_NOTICE;
  try {
   
    snrs::sngeom geom;
    const snrs::block & blk0 = geom.get_blocks()[0];
    const snrs::strip & strp1 = geom.get_strips()[1];

    
  } catch (std::exception & error) {
    DT_LOG_ERROR(logging, error.what());
    return 1;
  }
  return 0;
}
