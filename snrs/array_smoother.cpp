// Ourselves:
#include <snrs/array_smoother.hpp>

// Standard library:
#include <fstream>

// Bayeux:
#include <bayeux/mygsl/kernel_smoother.h>
#include <bayeux/mygsl/tabulated_function.h>

// GSL:
#include <gsl/gsl_fit.h>

namespace snrs {

  array_smoother::array_smoother(double width_, bool print_)
    : _width_(width_)
    , _print_(print_)
  {
    return;
  }

  void array_smoother::operator()(std::vector<double> & values_, const std::string & label_)
  {
    datatools::logger::priority locLogging = datatools::logger::PRIO_DEBUG;
    mygsl::tabulated_function xy;
    std::vector<double> startX;
    std::vector<double> startY;
    std::vector<double> stopX;
    std::vector<double> stopY;
    double limX = 4.0;
    for (unsigned int i = 0; i < values_.size(); i++) {
      double xi = i * 1.0;
      xy.add_point(xi, values_[i]);
      if (xi <= limX + 0.5) {
        startX.push_back(xi);
        startY.push_back(values_[i]);
      }
      if (xi >= values_.size() - limX - 1.5) {
        stopX.push_back(xi);
        stopY.push_back(values_[i]);
      }
    }
    DT_LOG_DEBUG(locLogging, "startX.size = " << startX.size());
    DT_LOG_DEBUG(locLogging, "startY.size = " << startY.size());
    DT_LOG_DEBUG(locLogging, "stopX.size  = " << stopX.size());
    DT_LOG_DEBUG(locLogging, "stopY.size  = " << stopY.size());
    double c0Start, c1Start, cov00Start, cov01Start, cov11Start, sumqStart;
    double c0Stop,  c1Stop,  cov00Stop,  cov01Stop,  cov11Stop,  sumqStop;
    int fit_result = gsl_fit_linear(startX.data(), 1,
                                    startY.data(), 1,
                                    startX.size(),
                                    &c0Start, &c1Start, &cov00Start, &cov01Start, &cov11Start, &sumqStart);
    fit_result = gsl_fit_linear(stopX.data(), 1,
                                stopY.data(), 1,
                                stopX.size(),
                                &c0Stop, &c1Stop, &cov00Stop, &cov01Stop, &cov11Stop, &sumqStop);

    unsigned int iMax = (unsigned int) _width_;
    for (unsigned int i = 1; i < iMax; i++) {
      double xi = 0.0 - i;
      double yi, yiErr; 
      gsl_fit_linear_est(xi, c0Start, c1Start, cov00Start, cov01Start, cov11Start, &yi, &yiErr);
      xy.add_point(xi, yi);
      xi = values_.size() - 1.0 + i;
      gsl_fit_linear_est(xi, c0Stop, c1Stop, cov00Stop, cov01Stop, cov11Stop, &yi, &yiErr);
      xy.add_point(xi, yi);      
    }
    xy.lock_table("linear");

    if (_print_ and not label_.empty()) {
      std::ofstream fout(label_ + "array_smoother_debug.data");
      std::vector<double> keys, values;
      xy.export_to_vectors(keys, values);
      for (int i = 0; i < keys.size(); i++) {
        fout << keys[i] << ' ' << values[i] << '\n';
      }
      fout.close();
    }
    
    xy.tree_dump(std::cerr, "Tabulated: ", "*** [devel] ");
    mygsl::nw_sampled_function_smoother nwSmooth(xy, _width_);
    for (unsigned int i = 0; i < values_.size(); i++) {
     double xi = i * 1.0;
     double fi = nwSmooth(xi);
     values_[i] = fi;
    }
    return;
  }
 
} // namespace snrs 
