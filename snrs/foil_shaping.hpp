#ifndef SNRS_FOIL_SHAPING_HPP
#define SNRS_FOIL_SHAPING_HPP

// Standard Library:
#include <vector>
#include <iostream>

// Bayeux:
#include <bayeux/datatools/utils.h>
#include <bayeux/datatools/clhep_units.h>
#include <bayeux/datatools/logger.h>

namespace snrs {
  
  namespace fs {
    
    enum distortion_model_type
      {
       DISTORTION_FLAT       = 0,
       DISTORTION_ELLIPTIC   = 1,
       DISTORTION_POLYNOMIAL = 2
      };

    static char distortion_symbol(distortion_model_type);
    
    struct elliptic_distortion_data_type
    {
      double a  = datatools::invalid_real();
      double b  = datatools::invalid_real();
      double x0 = datatools::invalid_real();
      double y0 = datatools::invalid_real();
      std::vector<double> node_angles;
      void reset();
      void store(std::ostream &) const;
      void load(std::istream &);
      static double func_elliptic_arc(double t_, void * params_);

    };
 
    struct polynomial_distortion_data_type
    {
      double y0 = datatools::invalid_real();
      std::vector<double> coeffs;
      std::vector<double> node_ys;
      void reset();
      void store(std::ostream &) const;
      void load(std::istream &);
      static double func_polynomial_path(double y_, void * params_);
   };
 
  } // namespace fs 
   
} // namespace snrs 

#endif // SNRS_FOIL_SHAPING_HPP
