#ifndef SNRS_FSFS_HPP
#define SNRS_FSFS_HPP

// Standard Library:
#include <vector>

// Bayeux:
#include <bayeux/datatools/bit_mask.h>
#include <bayeux/datatools/clhep_units.h>
#include <bayeux/datatools/logger.h>
#include <bayeux/mygsl/multidimensional_minimization.h>
#include <bayeux/mygsl/multiparameter_system.h>
#include <bayeux/mygsl/tabulated_function.h>

// This project:
#include <snrs/ltd.hpp>
#include <snrs/pad.hpp>

namespace snrs {

  /// \brief Foil shaping fit algorithm
  class foil_shaping_fit_system
  {
  public:

    enum zfit_model_type
      {
       ZFIT_MODEL_INVALID     = 0,
       ZFIT_MODEL_ELLIPTIC    = 1,
       ZFIT_MODEL_POLYNOMIAL2 = 2,
       ZFIT_MODEL_POLYNOMIAL3 = 3,
       ZFIT_MODEL_POLYNOMIAL4 = 4
      };
  
    struct zfit_params_type
    {
      zfit_model_type model = ZFIT_MODEL_INVALID;
      // Elliptic fit:
      double ell_x0 = datatools::invalid_real();
      double ell_y0 = datatools::invalid_real();
      double ell_a = datatools::invalid_real();
      double ell_b = datatools::invalid_real();
      // Polynomial fit:
      double pol_y0 = datatools::invalid_real();
      std::vector<double> pol_coeffs;
      void reset();
    };

    struct zfit_residual_type
    {
      int    id = -1;
      double residual = datatools::invalid_real();
    };
  
    struct zfit_context_type
    {
      int kz = -1;
      double z = datatools::invalid_real();
      bool success = false;
      bool maxiter = false;
      double chi2 = datatools::invalid_real();
      uint32_t ndof = 0;
      double p_value = datatools::invalid_real();
      std::vector<zfit_residual_type> residuals;
      zfit_params_type params;
    
      void reset();
    };

    struct fit_config_type
    {
      int      strip_id = -1;
      int      pad_id = -1;
      uint32_t max_iter       = 3000;
      bool     success_on_max_iter = true;
      int      modulo_iter    = 10;
      double   epsabs         = 1.0;
      double   zband_width    = 1.0 * CLHEP::mm;
      double   y_edge_safe    = 10.0 * CLHEP::mm;
      bool     no_elliptic    = false;
      bool     no_polynomial2 = false;
      bool     no_polynomial3 = false;
      bool     no_polynomial4 = false;
      double   sigma_x        = 0.5 * CLHEP::mm;
      bool     fixed_y0       = false;
      double   safe_film_gap  = 2.0 * CLHEP::micrometer;
      int      shaping_mode   = 3;
      double   smooth_width   = 4.0; // 5.0;
      void print(std::ostream & out_) const;
    };

    foil_shaping_fit_system(const fit_config_type & fit_config_,
                            pad & pad_,
                            const sngeom & geom_,
                            const ltd & ltd_data_,
                            datatools::logger::priority logging_ = datatools::logger::PRIO_NOTICE);
  
    const pad & get_pad() const;
  
    const sngeom & get_geom() const;
  
    const ltd & get_ltd_data() const;

    const std::vector<ltd_zband> & get_zbands() const;
  
    const std::vector<zfit_context_type> & get_zfits() const;

    void plot_zbands(std::ostream & out_) const;
  
    void plot_zfits(std::ostream & out_) const;
     
    void run(int kzmin_ = -1, int kzmax_ = -1);

    const fit_config_type & get_fit_config() const;

    void smooth_elliptic_zfits();
    
    bool shape_pad(pad & pad_) const;

    void shape_pad1(pad & pad_) const;

    void shape_pad2(pad & pad_) const;

    void shape_pad_elliptic_1(pad & pad_) const;
  
  private:

    void _initialize_();

    void _guess_fit_(int kz_, zfit_model_type model_);
    
    void _fit_z_(int kz_);
  
  public:

    datatools::logger::priority logging = datatools::logger::PRIO_NOTICE;  
    
  private:

    fit_config_type _fit_config_;
    mygsl::tabulated_function _stripLeftEdgeTF_;    
    pad *       _pad_ = nullptr;       ///< Working pad
    const sngeom * _geom_ = nullptr;   ///< Geometry description
    const ltd * _ltd_data_ = nullptr;  ///< Input LTC data
    std::vector<ltd_zband> _zbands_;
    std::vector<zfit_context_type> _zfits_;
    std::vector<double> _zTabLtdYmin_;
    std::vector<double> _zTabLtdYmax_;

  };

  struct fsfs_sys;
 
  /// \brief Functor computing the chi2 to be minimized from LTD data in a z-band and a given fit model
  struct chi2_computer
  {
    chi2_computer(const ltd & ltd_data_,
                  const ltd_zband & zband_,
                  const fsfs_sys & mpsys_,
                  // const mygsl::multiparameter_system & mpsys_,
                  foil_shaping_fit_system::zfit_model_type fit_model_,
                  datatools::logger::priority logging_);
  
    int operator()(double & chi2_,
                   uint32_t & ndof_,
                   std::vector<foil_shaping_fit_system::zfit_residual_type> & residuals_) const;

    const ltd & ltd_data;
    const ltd_zband & zband;
    const fsfs_sys & mpsys;
    foil_shaping_fit_system::zfit_model_type fit_model;
    datatools::logger::priority logging;
  };

  /// \brief Bayeux/mygsl  multidimensional minimization interface for the FSF model
  struct fsfs_sys
    : public mygsl::multidimensional_minimization_system
  {
    fsfs_sys(const ltd & ltd_data_,
             const foil_shaping_fit_system & fsfs_,
             int kz_,
             foil_shaping_fit_system::zfit_model_type model_ = foil_shaping_fit_system::ZFIT_MODEL_ELLIPTIC);

    uint32_t get_number_of_data() const;

    double compute_chi_square();

    foil_shaping_fit_system::zfit_model_type get_model() const;

    const foil_shaping_fit_system & get_fsfs() const;
  
  protected:

    int _eval_f(double & f_) override;

  private:
  
    const ltd & _ltd_data_;
    const foil_shaping_fit_system & _fsfs_;
    int _kz_ = -1;
    foil_shaping_fit_system::zfit_model_type _model_ = foil_shaping_fit_system::ZFIT_MODEL_INVALID;
    const ltd_zband * _zband_ = nullptr;
    const foil_shaping_fit_system::zfit_params_type * _zfit_params_  = nullptr;

  };

} // namespace snrs 

#endif // SNRS_FSFS_HPP
