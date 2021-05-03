// Ourselves;
#include <snrs/fsfs.hpp>

// Standard library:
// #include <string>
// #include <fstream>
#include <cmath>

// Bayeux:
#include <bayeux/datatools/exception.h>

namespace snrs {

  namespace fs {

    // static
    char distortion_symbol(distortion_model_type model_)
    {
      if (model_ == DISTORTION_FLAT) return 'f';
      if (model_ == DISTORTION_ELLIPTIC) return 'e';
      if (model_ == DISTORTION_POLYNOMIAL) return 'p';
      return 0;
    }
 
    void elliptic_distortion_data_type::reset()
    {
      a = datatools::invalid_real();
      b = datatools::invalid_real();
      x0 = datatools::invalid_real();
      y0 = datatools::invalid_real();
      node_angles.clear();
    }

    void elliptic_distortion_data_type::store(std::ostream & out_) const
    {
      // out_ << "elliptic_distortion_data_type" << '\n';
      out_ << a / CLHEP::mm << ' ';
      out_ << b / CLHEP::mm << ' ';
      out_ << x0 / CLHEP::mm << ' ';
      out_ << y0 / CLHEP::mm << ' ';
      out_ << node_angles.size() << ' ';
      for (auto angle : node_angles) {
        out_ << angle << ' ';
      }
      out_ << '\n'; 
    }

    void elliptic_distortion_data_type::load(std::istream & in_)
    {
      reset();
      in_ >> a >> b >> x0 >> y0 >> std::ws;
      a *= CLHEP::mm;
      b *= CLHEP::mm;
      x0 *= CLHEP::mm;
      y0 *= CLHEP::mm;
      uint32_t anglesSize = 0;
      in_ >> anglesSize >> std::ws;
      node_angles.assign(anglesSize, datatools::invalid_real());
      for (uint32_t iang = 0; iang < anglesSize; iang++) {
        in_ >> node_angles[iang] >> std::ws;
      }
    }
 
    void polynomial_distortion_data_type::reset()
    {
      coeffs.clear();
      node_ys.clear();    
    }

    void polynomial_distortion_data_type::store(std::ostream & out_) const
    {
      // out_ << "polynomial_distortion_data_type" << '\n';
      out_ << coeffs.size() << '\n';
      for (auto coeff : coeffs) {
        out_ << ' ' << coeff;
      }
      out_ << '\n'; 
      out_ << node_ys.size() << '\n';
      for (auto y : node_ys) {
        out_ << ' ' << y / CLHEP::mm;
      }
      out_ << '\n'; 
    }

    void polynomial_distortion_data_type::load(std::istream & in_)
    {
      reset();
      uint32_t coeffsSize = 0;
      in_ >> coeffsSize >> std::ws;
      coeffs.assign(coeffsSize, datatools::invalid_real());
      for (uint32_t icoeff = 0; icoeff < coeffsSize; icoeff++) {
        in_ >> coeffs[icoeff] >> std::ws;
      }
      uint32_t nodesSize = 0;
      in_ >> nodesSize >> std::ws;
      node_ys.assign(nodesSize, datatools::invalid_real());
      for (uint32_t iy = 0; iy < nodesSize; iy++) {
        in_ >> node_ys[iy] >> std::ws;
        node_ys[iy] *= CLHEP::mm;
      }
    }

    double elliptic_distortion_data_type::func_elliptic_arc(double t_, void * params_)
    {
      const elliptic_distortion_data_type & params
        = *static_cast<const elliptic_distortion_data_type *>(params_);
      double a = params.a;
      double b = params.b;
      // double t = 0.5 * M_PI - theta_;
      // XXX
      // x = a cos t -> dx = -a sin t dt
      // y = b sin t -> dy =  b cos t dt
      // ds = sqrt(dx^2+dy^2)
      // ds = dt (a^2 sin^2t + b^2 cos^2t)
      double dsdt = std::hypot(-a * std::sin(t_), b * std::cos(t_));
      return dsdt; 
    }
 
    double polynomial_distortion_data_type::func_polynomial_path(double y_, void * params_)
    {
      const polynomial_distortion_data_type & params
        = *static_cast<const polynomial_distortion_data_type *>(params_);
      double y0 = params.y0;
      double c0 = 0.0;
      double c1 = 0.0;
      double c2 = 0.0;
      double c3 = 0.0;
      double c4 = 0.0;
      double yp = y_ - y0;
      uint32_t csz = params.coeffs.size();
      if (csz >= 3) {
        c0 = params.coeffs[0];
        c1 = params.coeffs[1];
        c2 = params.coeffs[2];
      }
      double dxdy = c1 + 2. * c2 * yp;
      if (csz >= 4) {
        c3 = params.coeffs[3];
        dxdy += 3. * c3 * gsl_pow_2(yp);
      }
      if (csz >= 5) {
        c4 = params.coeffs[4];
        dxdy += 4. * c4 * gsl_pow_3(yp);
      }
      double dsdy = std::hypot(dxdy, 1.0);
      return dsdy; 
    }
   
  } // namespace fs 

} // namespace snrs 
