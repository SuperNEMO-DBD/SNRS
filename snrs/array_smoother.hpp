#ifndef SNRS_ARRAY_SMOOTHER_HPP
#define SNRS_ARRAY_SMOOTHER_HPP

// Standard library:
#include <vector>
#include <string>

// Bayeux:
#include <bayeux/datatools/logger.h>

namespace snrs {

  class array_smoother
  {
  public:

    array_smoother() = default;

    array_smoother(double width_, bool print_ = false);

    void operator()(std::vector<double> & values_, const std::string & label_ = "");

  public:
    
    datatools::logger::priority logging = datatools::logger::PRIO_WARNING;
    
  private:

    double _width_ = 5.0;
    bool _print_ = false;
    
  };
 
} // namespace snrs 

#endif // SNRS_ARRAY_SMOOTHER_HPP
