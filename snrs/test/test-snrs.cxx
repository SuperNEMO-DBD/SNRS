// Standard Library:
#include <iostream>
#include <exception>
#include <string>
#include <fstream>

// Bayeux:
#include <bayeux/bayeux.h>
#include <bayeux/datatools/utils.h>

// This project:
#include <snrs/snrs.hpp>
#include <snrs/config.hpp>

int main()
{
  snrs::loadMe();
  datatools::logger::priority logging = datatools::logger::PRIO_NOTICE;
  try {

    std::string filename = "@snrs:data/geometry/source_foils/fsf/strip-3-pad-0-shaped.dat";
    datatools::fetch_path_with_env(filename);
    std::clog << "Resolved filename = " << filename << '\n';

  } catch (std::exception & error) {
    DT_LOG_ERROR(logging, error.what());
    return 1;
  }
  return 0;
}
