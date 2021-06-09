// Ourselves;
#include <snrs/fsfs.hpp>

// Standard library:
#include <string>
#include <fstream>
#include <cmath>

// GSL:
#include <gsl/gsl_integration.h>
#include <gsl/gsl_cdf.h>
#include <gsl/gsl_fit.h>

// Bayeux:
#include <bayeux/datatools/exception.h>
#include <bayeux/mygsl/min_max.h>
#include <bayeux/mygsl/mean.h>
#include <bayeux/mygsl/tabulated_function.h>
#include <bayeux/datatools/logger.h>

// This project:
#include <snrs/foil_shaping.hpp>
#include <snrs/array_smoother.hpp>

namespace snrs {
  
  void foil_shaping_fit_system::zfit_params_type::reset()
  {
    model  = ZFIT_MODEL_INVALID;
    ell_x0 = datatools::invalid_real();
    ell_y0 = datatools::invalid_real();
    ell_a  = datatools::invalid_real();
    ell_b  = datatools::invalid_real();
    pol_coeffs.clear();
    pol_y0 = datatools::invalid_real();
 }

  void foil_shaping_fit_system::zfit_context_type::reset()
  {
    params.reset();
    kz = -1;
    z = datatools::invalid_real();
    success = false;
    maxiter = false;
    chi2 = datatools::invalid_real();
    ndof = 0;
    p_value = datatools::invalid_real();
    residuals.clear();
    return;
  }

  void foil_shaping_fit_system::fit_config_type::print(std::ostream & out_) const
  {
    out_ << "max_iter=" << max_iter << '\n';
    out_ << "modulo_iter=" << modulo_iter << '\n';
    out_ << "epsabs=" << epsabs << '\n';
    out_ << "zband_width=" << zband_width << '\n';
    out_ << "y_edge_safe=" << y_edge_safe << '\n';
    out_ << "no_elliptic=" << no_elliptic << '\n';
    out_ << "no_polynomial2=" << no_polynomial2 << '\n';
    out_ << "no_polynomial3=" << no_polynomial3 << '\n';
    out_ << "no_polynomial4=" << no_polynomial4 << '\n';
    out_ << "sigma_x=" << sigma_x << '\n';
    out_ << "fixed_y0=" << fixed_y0 << '\n';
    out_ << "safe_film_gap=" << safe_film_gap << '\n';
    return;
  }

  foil_shaping_fit_system::foil_shaping_fit_system(const fit_config_type & fit_config_,
                                                   pad & pad_,
                                                   const sngeom & geom_,
                                                   const ltd & ltd_data_,
                                                   datatools::logger::priority logging_)
  {
    logging = logging_;
    _fit_config_ = fit_config_;
    _pad_ = &pad_;
    _geom_ = &geom_;
    _ltd_data_ = &ltd_data_;
    _initialize_();
    return;
  }
  
  const pad & foil_shaping_fit_system::get_pad() const
  {
    return *_pad_;
  }
 
  const sngeom & foil_shaping_fit_system::get_geom() const
  {
    return *_geom_;
  }
 
  const ltd & foil_shaping_fit_system::get_ltd_data() const
  {
    return *_ltd_data_;
  }

  const std::vector<ltd_zband> &
  foil_shaping_fit_system::get_zbands() const
  {
    return _zbands_;
  }
  
  const std::vector<foil_shaping_fit_system::zfit_context_type> &
  foil_shaping_fit_system::get_zfits() const
  {
    return _zfits_;
  }

  void foil_shaping_fit_system::_initialize_()
  {
    DT_LOG_NOTICE(logging, " **********************************************");
    DT_LOG_NOTICE(logging, " * Strip ID" <<  _fit_config_.strip_id << " ");
    DT_LOG_NOTICE(logging, " **********************************************");
    _zbands_.clear(); 
    _zfits_.clear();
    {
      ltd_zband zband0;
      _zbands_.assign(_pad_->get_nz() + 1, zband0);
    }
    {
      zfit_context_type zfit0;
      _zfits_.assign(_pad_->get_nz() + 1, zfit0);
    }
    double dz = _fit_config_.zband_width;
    for (int kz = 0; kz <= _pad_->get_nz(); kz++) {
      vertex3d_id vtxId(pad::FACE_BACK, 0, kz);
      double zk = (*_pad_)[vtxId].pos.z();
      _zfits_[kz].kz = kz;
      _zfits_[kz].z  = zk;
      // if (kz == 0) {
      //   _ltd_data_->extract_zband_min_max(zk, zk + dz, _zbands_[kz]);     
      // } else if (kz == _pad_->get_nz()) {
      //   _ltd_data_->extract_zband_min_max(zk - dz, zk, _zbands_[kz]);     
      // } else {
      //   _ltd_data_->extract_zband(zk, dz, _zbands_[kz]);
      // }
      _ltd_data_->extract_zband(zk, dz, _zbands_[kz]);
      _zbands_[kz].id = kz;
      DT_LOG_DEBUG(logging, " #Z-band = " << _zbands_[kz]);
    }
    double yRef =_geom_->get_strip_ypos()[_fit_config_.strip_id];
    std::string stripLeftEdgeDataFilename("@snrs:data/geometry/source_foils/ltd/ltd_calib_strip"
                                          + std::to_string(_fit_config_.strip_id) + "-left_edge.data");
    datatools::fetch_path_with_env(stripLeftEdgeDataFilename);
    DT_LOG_NOTICE(logging, "Loading left edge tabulated data for strip #"
                  << _fit_config_.strip_id << " from file '" << stripLeftEdgeDataFilename << "'...");
    {
      std::ifstream stripLeftEdgeDataFile(stripLeftEdgeDataFilename);
      DT_THROW_IF(!stripLeftEdgeDataFile, std::runtime_error, "Cannot open file '" << stripLeftEdgeDataFilename << "'!");
      while (! stripLeftEdgeDataFile.eof()) {
        double y, z;
        stripLeftEdgeDataFile >> y >> z >> std::ws;
        DT_LOG_NOTICE(logging, "Loading: z=" << z << "  y=" << y);
        _stripLeftEdgeTF_.add_point(z, y);
      }
    }
    _stripLeftEdgeTF_.relock_table("cspline");
  
    if (datatools::logger::is_debug(logging)) {
      std::string data_dir = ".";
      std::string filename =
        data_dir + "/_ltd-strip-" + std::to_string(_fit_config_.strip_id) + "-zbands.plot";
      std::ofstream ozbands(filename);
      plot_zbands(ozbands);
      ozbands.close();
      DT_LOG_DEBUG(logging, "Debug zbands plot");
    }

    for (int kz = 0; kz <= _pad_->get_nz(); kz++) {
      ltd_zband & zBand = _zbands_[kz];
      double zk = zBand.z;
      DT_LOG_DEBUG(logging, "Z-band @ kz=" << kz << " zk=" << zk / CLHEP::mm);
      double dzk = zBand.dz;
      mygsl::min_max xMinMax;
      mygsl::min_max yMinMax;
      for (auto point_id : zBand.point_ids) {
        const geomtools::vector_3d & pZ = _ltd_data_->get(point_id);
        xMinMax.add(pZ.x());
        yMinMax.add(pZ.y());
      } 
      double xMed = xMinMax.get_median();
      double yMed = yMinMax.get_median();
      double ySafe   = _fit_config_.y_edge_safe;
      double yLtdMin = _stripLeftEdgeTF_.eval(zk / CLHEP::mm) * CLHEP::mm;
      double yLtdMax = yLtdMin + 2 * (yRef - yLtdMin);
      if (yLtdMax > zBand.ymax) {
        yLtdMax = zBand.ymax - 2 * (yLtdMin - zBand.ymin);
      }
      // yMin = yMinMax.get_min();
      // yMax = yMinMax.get_max();
      _zTabLtdYmin_.push_back(yLtdMin);
      _zTabLtdYmax_.push_back(yLtdMax);
      double yMinFit = yLtdMin + ySafe;
      double yMaxFit = yLtdMax - ySafe;
      DT_LOG_NOTICE(logging, "  zBand   = " << zBand);
      DT_LOG_NOTICE(logging, "  yLtdMin = " << yLtdMin / CLHEP::mm << " mm");
      DT_LOG_NOTICE(logging, "  yLtdMax = " << yLtdMax / CLHEP::mm << " mm");
      DT_LOG_NOTICE(logging, "  yMinFit = " << yMinFit / CLHEP::mm << " mm");
      DT_LOG_NOTICE(logging, "  yMaxFit = " << yMaxFit / CLHEP::mm << " mm");
      DT_LOG_NOTICE(logging, "  Y-cut on Z-band using ySafe = " << ySafe / CLHEP::mm << " mm");
      ltd_zband zbandCut;
      _ltd_data_->generate_y_cut_zband(yMinFit, yMaxFit, zBand, zbandCut);
      zBand = zbandCut;
      DT_LOG_NOTICE(logging, "  zBand (cut) = " << zBand);
      // _ltd_data_->clean_y_zband(yMinFit, yMaxFit, zBand);
    }

    if (datatools::logger::is_debug(logging))
    {
      std::string data_dir = ".";
      std::string filename =
        data_dir + "/ltd-strip-" + std::to_string(_fit_config_.strip_id) + "-zfits.plot";
      std::ofstream ozfits(filename);
      plot_zfits(ozfits);
      ozfits.close();
      DT_LOG_DEBUG(logging, "Debug zfits plot");
    }

    DT_LOG_NOTICE(logging, "Shaping mode = " << _fit_config_.shaping_mode);  
    return;
  }

  void foil_shaping_fit_system::_guess_fit_(int kz_, zfit_model_type model_)
  {
    const ltd_zband & zBand = _zbands_[kz_];
    double zk = zBand.z;
    double dzk = zBand.dz;
    mygsl::min_max xMinMax;
    // mygsl::arithmetic_mean xMean;
    mygsl::min_max yMinMax;
    // mygsl::arithmetic_mean yMean;
    DT_LOG_NOTICE(logging, "#Z-band points = " << zBand.point_ids.size());
    for (auto point_id : zBand.point_ids) {
      const geomtools::vector_3d & pZ = _ltd_data_->get(point_id);
      xMinMax.add(pZ.x());
      yMinMax.add(pZ.y());
      // xMean.add(pZ.x());
      // yMean.add(pZ.y());
    }   
    DT_LOG_NOTICE(logging, "xMin     = " << xMinMax.get_min() / CLHEP::mm << " mm");
    DT_LOG_NOTICE(logging, "xMax     = " << xMinMax.get_max() / CLHEP::mm << " mm");
    DT_LOG_NOTICE(logging, "yMin     = " << yMinMax.get_min() / CLHEP::mm << " mm");
    DT_LOG_NOTICE(logging, "yMax     = " << yMinMax.get_max() / CLHEP::mm << " mm");
    double xMed = xMinMax.get_median();
    double yMed = yMinMax.get_median();
    geomtools::vector_3d pRef(xMed, yMed, zk);
    const geomtools::vector_3d * pClosest = nullptr;
    const geomtools::vector_3d * pFarestLeft = nullptr;
    const geomtools::vector_3d * pFarestRight = nullptr;
    double closestDist = datatools::invalid_real();
    double farestLeftDist = datatools::invalid_real();
    double farestRightDist = datatools::invalid_real();
    std::ofstream fZpoints("zpoints-strip" 
                           + std::to_string(_fit_config_.strip_id)
                           + "-pad" + std::to_string(_fit_config_.pad_id)
                           + "-zband" + std::to_string(kz_)
                           + ".data");
    for (auto point_id : zBand.point_ids) {
      const geomtools::vector_3d & pZ = _ltd_data_->get(point_id);
      fZpoints << pZ.x() << ' ' << pZ.y() << ' ' << pZ.z() << '\n';
      double dist = (pZ - pRef).mag();
      if (pClosest == nullptr or dist < closestDist) {
        closestDist = dist;
        pClosest = &pZ;
      }
      if (pZ.y() < pRef.y() and (pFarestLeft == nullptr or dist > farestLeftDist)) {
        farestLeftDist = dist;
        pFarestLeft = &pZ;
      }
      if (pZ.y() > pRef.y() and (pFarestRight == nullptr or dist > farestRightDist)) {
        farestRightDist = dist;
        pFarestRight = &pZ;
      }
    }
    DT_LOG_NOTICE(logging, "xMed     = " << xMed / CLHEP::mm << " mm");
    DT_LOG_NOTICE(logging, "yMed     = " << yMed / CLHEP::mm << " mm");
    DT_LOG_NOTICE(logging, "pClosest        = " << *pClosest << " mm");
    DT_LOG_NOTICE(logging, "pFarestLeft     = " << *pFarestLeft << " mm");
    DT_LOG_NOTICE(logging, "pFarestRight    = " << *pFarestRight << " mm");
    DT_LOG_NOTICE(logging, "closestDist     = " << closestDist / CLHEP::mm << " mm");
    DT_LOG_NOTICE(logging, "farestLeftDist  = " << farestLeftDist / CLHEP::mm << " mm");
    DT_LOG_NOTICE(logging, "farestRightDist = " << farestRightDist / CLHEP::mm << " mm");
    fZpoints.close();
    zfit_params_type & zfit_params = _zfits_[kz_].params;
    zfit_params.reset();
    double y0 = yMed;
    DT_LOG_NOTICE(logging, "Guess y0 = " << y0 / CLHEP::mm << " mm");
    if (_fit_config_.fixed_y0) {
      y0 = _pad_->get_original_position().y();
      DT_LOG_NOTICE(logging, "Force y0 to the pad original position = " << y0 / CLHEP::mm << " mm");
    }

    if (model_ == ZFIT_MODEL_ELLIPTIC) {
      DT_LOG_NOTICE(logging, "Guess for the elliptic fit model");
      geomtools::vector_3d uDiff = *pClosest - pRef;
      uDiff.setY(0.0);
      uDiff.setZ(0.0);
      geomtools::vector_3d uRef = uDiff.unit();
      DT_LOG_DEBUG(logging, "uRef=" << uRef);
      zfit_params.model  = model_;
      zfit_params.ell_a  = yMinMax.get_half_width();
      if ( zfit_params.ell_a < 40.0 * CLHEP::mm) {
        zfit_params.ell_a = 40.0 * CLHEP::mm;
      }
      zfit_params.ell_b  = yMinMax.get_half_width() + 10.0 * CLHEP::mm;
      if ( zfit_params.ell_b < zfit_params.ell_a + 10.0 * CLHEP::cm) {
        zfit_params.ell_a = zfit_params.ell_a + 10.0 * CLHEP::cm;
      }
      zfit_params.ell_y0 = y0;
      zfit_params.ell_x0 = (zfit_params.ell_a - closestDist);    
      if (uRef.x() > 0.0) zfit_params.ell_x0 *= -1;
      DT_LOG_NOTICE(logging, "zfit_params.ell_a  = " << zfit_params.ell_a);
      DT_LOG_NOTICE(logging, "zfit_params.ell_b  = " << zfit_params.ell_b);
      DT_LOG_NOTICE(logging, "zfit_params.ell_y0 = " << zfit_params.ell_y0);
      DT_LOG_NOTICE(logging, "zfit_params.ell_x0 = " << zfit_params.ell_x0);
    } else { 
      DT_LOG_NOTICE(logging, "Guess for the polynomial fit model");
      if (model_ >= ZFIT_MODEL_POLYNOMIAL2) {
        geomtools::vector_3d lDiff = pRef - *pFarestLeft;
        geomtools::vector_3d ulDiff = lDiff.unit();
        ulDiff.setZ(0.0);
        geomtools::vector_3d rDiff = *pFarestRight - pRef; 
        geomtools::vector_3d urDiff = rDiff.unit();
        double thetaLeft = std::atan2(ulDiff.x(), ulDiff.y());
        double thetaRight = std::atan2(urDiff.x(), urDiff.y());
        urDiff.setZ(0.0);
        zfit_params.model  = model_;
        zfit_params.pol_y0 = y0;
        zfit_params.pol_coeffs.push_back(pClosest->x());
        zfit_params.pol_coeffs.push_back(0.5*(std::tan(thetaLeft) + std::tan(thetaRight)));
        zfit_params.pol_coeffs.push_back((std::tan(thetaRight) - std::tan(thetaLeft)) / yMinMax.get_half_width());
      }
      if (model_ >= ZFIT_MODEL_POLYNOMIAL3) {
        DT_LOG_NOTICE(logging, "Guess for the polynomial >2 fit model");
        zfit_params.pol_coeffs.push_back(1e-4);
      }
      if (model_ >= ZFIT_MODEL_POLYNOMIAL4) {
        DT_LOG_NOTICE(logging, "Guess for the polynomial >3 fit model");
        zfit_params.pol_coeffs.push_back(1e-6);
      }
    }
    return;
  }
 
  void foil_shaping_fit_system::run(int kzmin_, int kzmax_)
  {
    int kzmin = 0;
    if (kzmin_ >= 0) {
      kzmin = kzmin_;
    }
    int kzmax = _pad_->get_nz();
    if (kzmax_ >= 0) {
      kzmax = kzmax_;
      DT_THROW_IF(kzmax < kzmin, std::range_error, "Invalid kZ range!")
    }
    for (int kz = kzmin; kz <= kzmax; kz++) {
      DT_LOG_NOTICE(logging, "Run fit for kz=" << kz);
      _fit_z_(kz);
    }
    return;
  }

  void foil_shaping_fit_system::smooth_elliptic_zfits()
  {
    std::vector<double> aPars;
    std::vector<double> bPars;
    std::vector<double> x0Pars;
    std::vector<double> y0Pars;
    aPars.reserve(_zfits_.size());
    bPars.reserve(_zfits_.size());
    x0Pars.reserve(_zfits_.size());
    y0Pars.reserve(_zfits_.size());
    for (unsigned int iz = 0; iz < _zfits_.size(); iz++) {
      const zfit_context_type & zFitContext = _zfits_[iz];
      const zfit_params_type & zFitParams = zFitContext.params;
      DT_THROW_IF(zFitParams.model != ZFIT_MODEL_ELLIPTIC,
                  std::logic_error, "Unsupported zfit model!");
      aPars.push_back(zFitParams.ell_a);
      bPars.push_back(zFitParams.ell_b);
      x0Pars.push_back(zFitParams.ell_x0);
      y0Pars.push_back(zFitParams.ell_y0);
    }

    double smoothWidth = _fit_config_.smooth_width;
    bool smoothPrint = true;
    array_smoother aSmoother(smoothWidth, smoothPrint);
    aSmoother(aPars, "strip" + std::to_string(_fit_config_.strip_id) + "_a_");
    aSmoother(bPars, "strip" + std::to_string(_fit_config_.strip_id) + "_b_");
    aSmoother(x0Pars, "strip" + std::to_string(_fit_config_.strip_id) + "_x0_");
    aSmoother(y0Pars, "strip" + std::to_string(_fit_config_.strip_id) + "_y0_");

    for (unsigned int iz = 0; iz < _zfits_.size(); iz++) {
      zfit_context_type & zFitContext = _zfits_[iz];
      zfit_params_type & zFitParams = zFitContext.params;
      zFitParams.ell_a = aPars[iz];
      zFitParams.ell_b = bPars[iz];
      zFitParams.ell_x0 = x0Pars[iz];
      zFitParams.ell_y0 = y0Pars[iz];
    }
    return;
  }

  void foil_shaping_fit_system::plot_zbands(std::ostream & out_) const
  {
    int dkz = 1;
    for (int kz = 0; kz <= _pad_->get_nz(); kz += dkz) {
      for (auto point_id : _zbands_[kz].point_ids) {
        const geomtools::vector_3d & pz = _ltd_data_->get(point_id);
        out_ << pz.x() << ' ' << pz.y() << ' '  << pz.z() << '\n';
      }
      out_ << '\n';
      out_ << '\n';
    }
    return;
  }
  
  void foil_shaping_fit_system::plot_zfits(std::ostream & out_) const
  {
    for (int kz = 0; kz <= _pad_->get_nz(); kz++) {
      const zfit_context_type & zfit_context = _zfits_[kz];
      if (not zfit_context.success) continue;
      const zfit_params_type & zfit_params = zfit_context.params;
      if (zfit_params.model == ZFIT_MODEL_ELLIPTIC) {
        bool concave = false;
        if (zfit_params.ell_x0 < 0.0) concave = true;
        if (not concave) {
        }
        for (int i = 0; i <= 360; i++) {
          double thDeg = (i * 1.0 - 90.0);
          double xe = zfit_params.ell_x0 + zfit_params.ell_a * std::cos(thDeg * M_PI / 180.);
          double ye = zfit_params.ell_y0 + zfit_params.ell_b * std::sin(thDeg * M_PI / 180.);
          double ze = zfit_context.z;
          out_ << xe << ' ' << ye << ' ' << ze << '\n';
          if (kz == 0 and i == 0) {
            out_ << xe << ' ' << ye << ' ' << ze << '\n';
          }
        }
      } else {
        double ymin = zfit_params.pol_coeffs[0];
      }
      out_ << '\n';
      out_ << '\n';
    }
    return;
  }

  const foil_shaping_fit_system::fit_config_type &
  foil_shaping_fit_system::get_fit_config() const
  {
    return _fit_config_;
  }
 
  void foil_shaping_fit_system::_fit_z_(int kz_)
  {
    bool stopped = false;
    std::vector<zfit_model_type> fit_models;
    if (not _fit_config_.no_elliptic)    fit_models.push_back(ZFIT_MODEL_ELLIPTIC);
    if (not _fit_config_.no_polynomial2) fit_models.push_back(ZFIT_MODEL_POLYNOMIAL2);
    if (not _fit_config_.no_polynomial3) fit_models.push_back(ZFIT_MODEL_POLYNOMIAL3);
    if (not _fit_config_.no_polynomial4) fit_models.push_back(ZFIT_MODEL_POLYNOMIAL4);
    // Try several fit models:
    for (uint32_t iFitModel = 0; iFitModel < fit_models.size(); iFitModel++) {
      zfit_model_type zfitModel = fit_models[iFitModel];
      DT_LOG_NOTICE(logging, "Applying fit model " << iFitModel);
      _guess_fit_(kz_, zfitModel);
      DT_LOG_NOTICE(logging, "Guess is done.");
      fsfs_sys sys(*_ltd_data_, *this, kz_, zfitModel);
      zfit_context_type & zfit_context = _zfits_[kz_];
      mygsl::multidimensional_minimization multiDimMin;
      // multiDimMin.logging = datatools::logger::PRIO_DEBUG;
      multiDimMin.unset_step_action();
      // XXX
      // multiDimMin.set_default_step_action ();
      multiDimMin.set_max_iter(_fit_config_.max_iter);
      multiDimMin.set_modulo_iter(_fit_config_.modulo_iter);
      std::string methodName = "steepest_descent";
      methodName = "vector_bfgs";
      multiDimMin.init(methodName, sys);
      double epsabs = _fit_config_.epsabs;
      DT_LOG_NOTICE(logging, "epsabs=" << epsabs);
      int code = multiDimMin.minimize(epsabs);
      DT_LOG_NOTICE(logging, "Minimize code=" << code);

      bool goodFit = false;
      goodFit = (code == GSL_SUCCESS);
      if ((code == GSL_CONTINUE)
          and (multiDimMin.get_n_iter() == multiDimMin.get_max_iter())) {
        zfit_context.maxiter = true;
        if (_fit_config_.success_on_max_iter) {
          goodFit = true;
          DT_LOG_WARNING(logging, "Max iter reached (considered as a good fit...)");
          zfit_context.success = true;
        } else {
          zfit_context.success = false;
        }
      }
      if (goodFit) {
        DT_LOG_NOTICE(logging, "Found minimum for kz=" << kz_
                      << " in " << multiDimMin.get_n_iter() << " iterations");
        double chi2 = multiDimMin.get_fval();
        DT_LOG_NOTICE(logging, "  Chi2 = " << chi2);
        double ndof = sys.get_number_of_data () - sys.get_number_of_free_params ();
        DT_LOG_NOTICE(logging, "  ndof = " << ndof);
        DT_LOG_NOTICE(logging, "  Chi2/ndof= " << chi2 / ndof);
        DT_LOG_NOTICE(logging, "  Update zfits data for kz=" << kz_);
        zfit_context.success = true;
        zfit_context.chi2 = chi2;
        zfit_context.ndof = ndof;
        zfit_context.p_value = 1.0 - gsl_cdf_chisq_P(zfit_context.chi2, zfit_context.ndof);
        DT_LOG_NOTICE(logging, "  P-value = " << zfit_context.p_value);
        zfit_params_type & zfit_params = zfit_context.params;
        zfit_params.reset();
        zfit_params.model = zfitModel;
        if (zfit_params.model == ZFIT_MODEL_ELLIPTIC) {
          DT_LOG_NOTICE(logging, "Elliptic");
          zfit_params.ell_a  = sys.get_param(0).get_value_safe();
          zfit_params.ell_b  = sys.get_param(1).get_value_safe();
          zfit_params.ell_x0 = sys.get_param(2).get_value_safe();
          zfit_params.ell_y0 = sys.get_param(3).get_value_safe();
          DT_LOG_NOTICE(logging, "  a  = " << zfit_params.ell_a / CLHEP::mm << " mm");
          DT_LOG_NOTICE(logging, "  b  = " << zfit_params.ell_b / CLHEP::mm << " mm");
          DT_LOG_NOTICE(logging, "  x0 = " << zfit_params.ell_x0 / CLHEP::mm << " mm");
          DT_LOG_NOTICE(logging, "  y0 = " << zfit_params.ell_y0 / CLHEP::mm << " mm");
        } else {
          DT_LOG_NOTICE(logging, "Polynomial");
          if (zfit_params.model >= ZFIT_MODEL_POLYNOMIAL2) {
            zfit_params.pol_y0         = sys.get_param(0).get_value_safe();
            zfit_params.pol_coeffs[0]  = sys.get_param(1).get_value_safe();
            zfit_params.pol_coeffs[1]  = sys.get_param(2).get_value_safe();
            zfit_params.pol_coeffs[2]  = sys.get_param(3).get_value_safe();
          }
          if (zfit_params.model >= ZFIT_MODEL_POLYNOMIAL3) {
            zfit_params.pol_coeffs[3]  = sys.get_param(4).get_value_safe();
          } if (zfit_params.model >= ZFIT_MODEL_POLYNOMIAL4) {
            zfit_params.pol_coeffs[4]  = sys.get_param(5).get_value_safe();
          }
        }
      } else {
        DT_LOG_WARNING(logging, "Cannot find minimum for kz="
                       << kz_ << " after " << multiDimMin.get_n_iter() << " iterations");
      }
      if (goodFit) break;
    }
    return;
  }
  
  fsfs_sys::fsfs_sys(const ltd & ltd_data_,
                     const foil_shaping_fit_system & fsfs_,
                     int kz_,
                     foil_shaping_fit_system::zfit_model_type model_)
    : multidimensional_minimization_system()
    , _ltd_data_(ltd_data_)
    , _fsfs_(fsfs_)
    , _kz_(kz_)
    , _model_(model_)
  {
    multidimensional_minimization_system::set_numeric_eval_df(true);
    _zband_ = &(_fsfs_.get_zbands()[_kz_]);
    _zfit_params_ = &(_fsfs_.get_zfits()[_kz_].params);
    if (_model_ == foil_shaping_fit_system::ZFIT_MODEL_ELLIPTIC) {
      add_param(mygsl::param_entry::make_free_min("a",
                                                  10.0 * CLHEP::mm,
                                                  _zfit_params_->ell_a,
                                                  0.5 * CLHEP::mm));
      add_param(mygsl::param_entry::make_free_min("b",
                                                  10.0 * CLHEP::mm,
                                                  _zfit_params_->ell_b,
                                                  0.5 * CLHEP::mm));
      add_param(mygsl::param_entry::make_free("x0",
                                              _zfit_params_->ell_x0,
                                              0.5 * CLHEP::mm));
      if (not _fsfs_.get_fit_config().fixed_y0) {
        add_param(mygsl::param_entry::make_free_range("y0",                                
                                                      _zfit_params_->ell_y0 - 2 * CLHEP::cm,
                                                      _zfit_params_->ell_y0 + 2 * CLHEP::cm,
                                                      _zfit_params_->ell_y0,
                                                      0.5 * CLHEP::mm));
      } else {
        add_param(mygsl::param_entry::make_const("y0",
                                                 _zfit_params_->ell_y0));
      }
    } else {
      if (_model_ >= foil_shaping_fit_system::ZFIT_MODEL_POLYNOMIAL2) {
        // if (not _fsfs_.get_fit_config().fixed_y0) {
        //   add_param(mygsl::param_entry::make_free_range("y0",       
        //                                                 _zfit_params_->pol_y0 - 2 * CLHEP::cm,
        //                                                 _zfit_params_->pol_y0 + 2 * CLHEP::cm,
        //                                                 _zfit_params_->pol_y0,
        //                                                 0.5 * CLHEP::mm));
        // } else {
        DT_LOG_NOTICE(fsfs_.logging, "Add parameter y0");
        add_param(mygsl::param_entry::make_const("y0",
                                                 _zfit_params_->pol_y0));
        //}
        DT_LOG_NOTICE(fsfs_.logging, "Add parameter c0");
        add_param(mygsl::param_entry::make_free("c0",
                                                _zfit_params_->pol_coeffs[0],
                                                0.5));
        DT_LOG_NOTICE(fsfs_.logging, "Add parameter c1");
        add_param(mygsl::param_entry::make_free("c1",
                                                _zfit_params_->pol_coeffs[1],
                                                0.01));
        DT_LOG_NOTICE(fsfs_.logging, "Add parameter c2");
        add_param(mygsl::param_entry::make_free("c2",
                                                _zfit_params_->pol_coeffs[2],
                                                0.005));
      }
      if (_model_ >= foil_shaping_fit_system::ZFIT_MODEL_POLYNOMIAL3) {
        DT_LOG_NOTICE(fsfs_.logging, "Add parameter c3");
        add_param(mygsl::param_entry::make_free("c3",
                                                _zfit_params_->pol_coeffs[3],
                                                5e-6));
      }
      if (_model_ >= foil_shaping_fit_system::ZFIT_MODEL_POLYNOMIAL4) {
        DT_LOG_NOTICE(fsfs_.logging, "Add parameter c4");
        add_param(mygsl::param_entry::make_free("c4",
                                                _zfit_params_->pol_coeffs[4],
                                                5e-8));
      }
    }
    lock_params();
    init();
    if (datatools::logger::is_debug(fsfs_.logging)) {
      this->mygsl::multiparameter_system::print(std::cerr, "Multiparameter system:");
    }
    return;
  }

  uint32_t fsfs_sys::get_number_of_data() const
  {
    return _zband_->point_ids.size();
  }

  const foil_shaping_fit_system & fsfs_sys::get_fsfs() const
  {
    return _fsfs_;
  }
 
  foil_shaping_fit_system::zfit_model_type fsfs_sys::get_model() const
  {
    return _model_;
  }

  chi2_computer::chi2_computer(const ltd & ltd_data_,
                               const ltd_zband & zband_,
                               const fsfs_sys & mpsys_,
                               foil_shaping_fit_system::zfit_model_type fit_model_,
                               datatools::logger::priority logging_)
    : ltd_data(ltd_data_)
    , zband(zband_)
    , mpsys(mpsys_)
    , fit_model(fit_model_)
    , logging(logging_)
  {
  }

  int chi2_computer::operator()(double & chi2_,
                                uint32_t & ndof_,
                                std::vector<foil_shaping_fit_system::zfit_residual_type> & residuals_) const
  {
    chi2_ = datatools::invalid_real();
    ndof_ = 0;
    residuals_.clear();
    double chi2 = 0.0;
    double term = 0.0;
    // Resolution along the surface normal
    double sigma_normal = mpsys.get_fsfs().get_fit_config().sigma_x;
    uint32_t counter = 0;
    if (fit_model == foil_shaping_fit_system::ZFIT_MODEL_ELLIPTIC) { 
      // Ellipsoid:
      double a  = mpsys.get_param(0).get_value_safe();
      double b  = mpsys.get_param(1).get_value_safe();
      double x0 = mpsys.get_param(2).get_value_safe();
      double y0 = mpsys.get_param(3).get_value_safe();
      bool concave = false;
      if (x0 < 0.0) concave = true;
      DT_LOG_TRACE(logging, "concave = " << concave);
      DT_LOG_TRACE(logging, "a  = " << a / CLHEP::mm << " mm");
      DT_LOG_TRACE(logging, "b  = " << b / CLHEP::mm << " mm");
      DT_LOG_TRACE(logging, "x0 = " << x0 / CLHEP::mm << " mm");
      DT_LOG_TRACE(logging, "y0 = " << y0 / CLHEP::mm << " mm");
      for (int point_id : zband.point_ids) {
        DT_LOG_TRACE(logging, "point_id #" << counter++ <<  " : "  << point_id);
        const geomtools::vector_3d & p = ltd_data.get(point_id);
        double x = p.x();
        double y = p.y();
        DT_LOG_TRACE(logging, "  x = " << x);
        DT_LOG_TRACE(logging, "  y = " << y);
        double dy = y - y0;
        DT_LOG_TRACE(logging, "  dy = " << dy);
        double tmp = 0.0;
        double A = 1000.0;
        double dxdy = 0.0;
        if (dy >= b) {
          DT_LOG_TRACE(logging, "  y >= y0 + b ");
          // tmp = A * (dy - b);
          // dxdy = A;
          if (concave) {
            tmp *= -1.0;
            dxdy *= -1.0;
          }
        } else if (dy <= -b) {
          DT_LOG_TRACE(logging, "  y <= y0 - b ");
          // tmp = -A * (dy + b);      
          // dxdy = A;
          if (concave) {
            tmp *= -1.0;
            dxdy *= -1.0;
          }
        } else {
          DT_LOG_TRACE(logging, "  y0 + b > y > y0 - b ");
          tmp = - a * std::sqrt(1.0 - gsl_pow_2(dy / b));
          dxdy = a * dy / (std::sqrt(1.0 - gsl_pow_2(dy / b)) * b * b);
          if (concave) {
            tmp *=  -1.0;
          }
        }
        DT_LOG_TRACE(logging, "  tmp = " << tmp);
        double xth = x0 + tmp;
        DT_LOG_TRACE(logging, "  xth = " <<  xth);
        DT_LOG_TRACE(logging, "  dxdy = " << dxdy);
        double dx = xth - x;
        double alpha = std::atan(dxdy);
        double dt = dx * std::cos(alpha);
        dt = dx;
        term = gsl_pow_2(dt / sigma_normal);
        DT_LOG_TRACE(logging, "  term = " << term);
        chi2 += term;
        foil_shaping_fit_system::zfit_residual_type zres;
        zres.id = point_id;
        zres.residual = term;
        residuals_.push_back(zres);
      }
    } else if (fit_model >= foil_shaping_fit_system::ZFIT_MODEL_POLYNOMIAL2) { 
      // Polynomial:
      double y0 = mpsys.get_param(0).get_value_safe();
      double c0 = mpsys.get_param(1).get_value_safe();
      double c1 = mpsys.get_param(2).get_value_safe();
      double c2 = mpsys.get_param(3).get_value_safe();
      double c3 = 0.0;
      double c4 = 0.0;
      if (fit_model >= foil_shaping_fit_system::ZFIT_MODEL_POLYNOMIAL3) c3 = mpsys.get_param(4).get_value_safe();
      if (fit_model >= foil_shaping_fit_system::ZFIT_MODEL_POLYNOMIAL4) c4 = mpsys.get_param(5).get_value_safe();
      DT_LOG_TRACE(logging, "y0 = " << y0 / CLHEP::mm << " mm");
      DT_LOG_TRACE(logging, "c0  = " << c0);
      DT_LOG_TRACE(logging, "c1  = " << c1);
      DT_LOG_TRACE(logging, "c2  = " << c2);
      DT_LOG_TRACE(logging, "c3  = " << c3);
      DT_LOG_TRACE(logging, "c4  = " << c4);
      for (int point_id : zband.point_ids) {
        DT_LOG_TRACE(logging, "point_id #" << counter++ <<  " : "  << point_id);
        const geomtools::vector_3d & p = ltd_data.get(point_id);
        double x = p.x();
        double y = p.y();
        DT_LOG_TRACE(logging, "  x = " << x);
        DT_LOG_TRACE(logging, "  y = " << y);
        double yp = y - y0;
        DT_LOG_TRACE(logging, "  yp= " << yp);
        double xth = c0  + c1  * yp + c2 * yp * yp;
        double dxdy = c1 + 2 * c2 * yp;
        if (fit_model >= foil_shaping_fit_system::ZFIT_MODEL_POLYNOMIAL3) {
          xth += c3 * gsl_pow_3(yp);
          dxdy += 3 * c3 * gsl_pow_2(yp);
        }
        if (fit_model >= foil_shaping_fit_system::ZFIT_MODEL_POLYNOMIAL4) {
          xth += c4 * gsl_pow_4(yp);
          dxdy += 4 * c4 * gsl_pow_3(yp);
        }
        DT_LOG_TRACE(logging, "  xth  = " << xth);
        DT_LOG_TRACE(logging, "  dxdy = " << dxdy);
        double dx = xth - x;
        double alpha = std::atan(dxdy);
        double dt = dx * std::cos(alpha);
        // dt = dx;
        term = gsl_pow_2(dt / sigma_normal);
        DT_LOG_TRACE(logging, "  term = " << term);
        chi2 += term;
        foil_shaping_fit_system::zfit_residual_type zres;
        zres.id = point_id;
        zres.residual = term;
        residuals_.push_back(zres);
      }
    }
    DT_LOG_TRACE(logging, "=====> chi2 = " << chi2);
    chi2_ = chi2;
    ndof_ = counter - mpsys.get_number_of_free_params();
    return 0;
  }

  double fsfs_sys::compute_chi_square()
  {
    chi2_computer ch2Comp(_ltd_data_, *_zband_, *this, _model_, _fsfs_.logging);
    double chi2;
    uint32_t ndof;
    std::vector<foil_shaping_fit_system::zfit_residual_type> residuals; 
    int code = ch2Comp(chi2, ndof, residuals);
    return chi2;
  }

  int fsfs_sys::_eval_f(double & f_)
  {
    f_ = compute_chi_square();
    return GSL_SUCCESS;
  }

  bool foil_shaping_fit_system::shape_pad(pad & pad_) const
  {
    DT_LOG_NOTICE(logging, "Shaping pad...");
    if (_fit_config_.shaping_mode == 0) {
      DT_LOG_NOTICE(logging, "No shaping...");
      return false;
    } else if (_fit_config_.shaping_mode == 1) {
      shape_pad1(pad_);
    } else if (_fit_config_.shaping_mode == 2) {
      shape_pad2(pad_);
    } else if (_fit_config_.shaping_mode == 3) {
      shape_pad_elliptic_1(pad_);
    }
    return true;
  }
  
  void foil_shaping_fit_system::shape_pad1(pad & pad_) const
  {
    DT_LOG_NOTICE(logging, "Shaping pad mode=1...");
    double width = pad_.get_width();
    double dy = pad_.get_dy();
    if (pad_.has_distortion()) {
        pad_.grab_distortion().reset();
    }
    double yref = pad_.get_original_position().y();
    uint32_t quad_sz = 1000;
    gsl_integration_workspace * w = gsl_integration_workspace_alloc(quad_sz);
    for (const auto & zfitContext : _zfits_) {
      int kz = zfitContext.kz;
      double z = zfitContext.z;
      DT_LOG_NOTICE(logging, "kz = " << kz << " ,  z = " << z / CLHEP::mm << " mm");
      {
        pad::z_distortion_data_type zDist0;
        pad_.grab_distortion().z_distortions.push_back(zDist0);
      }
      pad::z_distortion_data_type & zDist = pad_.grab_distortion().z_distortions.back();
      zDist.kz = kz;
      // Extract fit parameters and build the geometry model of the pad:
      if (zfitContext.params.model == ZFIT_MODEL_ELLIPTIC) {
        DT_LOG_NOTICE(logging, "  Fit model: elliptic fit");
        // Elliptic fit:
        zDist.model = fs::DISTORTION_ELLIPTIC;
        fs::elliptic_distortion_data_type & ellDist = zDist.elliptic_distortion;
        ellDist.a = zfitContext.params.ell_a;
        ellDist.b = zfitContext.params.ell_b;
        ellDist.x0 = zfitContext.params.ell_x0;
        ellDist.y0 = zfitContext.params.ell_y0;
        bool concave = false;
        if (ellDist.x0 < 0.0) concave = true;
        DT_LOG_NOTICE(logging, "  concave=" << std::boolalpha << concave);
        gsl_function F;
        F.function = fs::elliptic_distortion_data_type::func_elliptic_arc;
        F.params = &ellDist;
        double result;
        double error;
        double dtheta = 0.5 * M_PI / 180;
        int j = 0;
        double deltaLength = dy;
        DT_LOG_NOTICE(logging, "  deltaLength=" << (deltaLength / CLHEP::mm) << " mm");
        std::vector<double> thetas;
        std::vector<double> arcs;
        thetas.reserve(1000);
        arcs.reserve(1000);
        thetas.push_back(0.0);
        arcs.push_back(0.0);
        double cumulLength = 0.0 * CLHEP::mm;
        double theta = 0.0;
        while (theta < 0.75 * M_PI) {
          gsl_integration_qags(&F, theta, theta + dtheta, 0, 1e-4, quad_sz, w, &result, &error);
          cumulLength += result;
          theta += dtheta;
          thetas.push_back(theta);
          arcs.push_back(cumulLength);
        }
        for (uint32_t i = 0; i < thetas.size(); i++) {
          DT_LOG_DEBUG(logging, "  i=" << i
                       << " theta=" << (thetas[i] * 180 / M_PI) << " L=" << (arcs[i] / CLHEP::mm));
        }
        uint32_t ny = pad_.get_ny();
        std::vector<double> tileAngles;
        tileAngles.assign(ny + 1, 0.0);
        tileAngles[ny / 2] = 0.0;
        DT_LOG_NOTICE(logging, "  Interpolating angle...");
        int iLast = 0;
        for (int jsy = 1;  jsy <= ny / 2; jsy++) {
          double jArcLength = jsy * deltaLength;
          while (arcs[iLast] < jArcLength) {
            iLast++;
          }
          double arcPrevious   = arcs[iLast-1];
          double arcLast       = arcs[iLast];
          double thetaPrevious = thetas[iLast-1];
          double thetaLast     = thetas[iLast];
          double jTheta = thetaPrevious + (thetaLast - thetaPrevious) * (jArcLength - arcPrevious) / (arcLast - arcPrevious);
          tileAngles[ny / 2 + jsy] = jTheta;
          tileAngles[ny / 2 - jsy] = -jTheta;
        }
        DT_LOG_NOTICE(logging, "  Interpolation is done:");
        for (uint32_t jsy = 0; jsy < tileAngles.size(); jsy++) {
          DT_LOG_DEBUG(logging,
                       "  jsy=" << jsy << " tileAngle=" << (tileAngles[jsy] * 180 / M_PI) << "°  "
                       << "Arc length=" << (jsy * deltaLength / CLHEP::mm) << " mm");
        }
        ellDist.node_angles = tileAngles;
        DT_LOG_NOTICE(logging, "  Building the elliptic mesh...");
        double c = hypot(ellDist.a, ellDist.b);
        double thickness = pad_.get_thickness();
        DT_LOG_NOTICE(logging, "  Thickness = " << thickness / CLHEP::mm << " mm");
        for (uint32_t jy = 0; jy <= ny; jy++) {
          double tj = tileAngles[jy];
          if (not concave) {
            tj = M_PI - tileAngles[jy];
          }
          double xj = ellDist.a * std::cos(tj);
          double yj = ellDist.b * std::sin(tj);
          geomtools::vector_3d normalj(ellDist.b * std::cos(tj), ellDist.a * std::sin(tj), 0.0);
          double mnormalj = normalj.mag();
          normalj /= mnormalj;
          geomtools::vector_3d p1(xj + ellDist.x0, yj + ellDist.y0 - yref, z);
          p1 -= 0.5 * thickness * normalj;
          geomtools::vector_3d p2 = p1 + thickness * normalj;
          if (not concave) {
            pad_[vertex3d_id(pad::FACE_BACK, jy, kz)].pos  = p2;
            pad_[vertex3d_id(pad::FACE_FRONT, jy, kz)].pos = p1;
          } else {
            pad_[vertex3d_id(pad::FACE_FRONT, jy, kz)].pos = p2;
            pad_[vertex3d_id(pad::FACE_BACK, jy, kz)].pos  = p1;
          }
          if (pad_.has_film()) {
            geomtools::vector_3d b1 = p1 - (pad_.get_film_thickness() + _fit_config_.safe_film_gap) * normalj;
            geomtools::vector_3d b2 = p1 - _fit_config_.safe_film_gap * normalj;
            geomtools::vector_3d f1 = p2 + _fit_config_.safe_film_gap * normalj;
            geomtools::vector_3d f2 = p2 + (pad_.get_film_thickness() + _fit_config_.safe_film_gap) * normalj;
            if (not concave) {
              pad_.grab_back_film_vertex(vertex3d_id(pad::FACE_BACK, jy, kz)).pos  = b2;
              pad_.grab_back_film_vertex(vertex3d_id(pad::FACE_FRONT, jy, kz)).pos = b1;
              pad_.grab_front_film_vertex(vertex3d_id(pad::FACE_BACK, jy, kz)).pos  = f2;
              pad_.grab_front_film_vertex(vertex3d_id(pad::FACE_FRONT, jy, kz)).pos = f1;
            } else {
              pad_.grab_back_film_vertex(vertex3d_id(pad::FACE_FRONT, jy, kz)).pos = b2;
              pad_.grab_back_film_vertex(vertex3d_id(pad::FACE_BACK, jy, kz)).pos  = b1;
              pad_.grab_front_film_vertex(vertex3d_id(pad::FACE_FRONT, jy, kz)).pos = f2;
              pad_.grab_front_film_vertex(vertex3d_id(pad::FACE_BACK, jy, kz)).pos  = f1;
            }
          }
        }
        DT_LOG_NOTICE(logging, "  Elliptic mesh is done." );
      } else {
        if (zfitContext.params.model >= ZFIT_MODEL_POLYNOMIAL2) {
          // Polynomial fit:
          DT_LOG_NOTICE(logging, "  Fit model: polynomial fit");
          zDist.model = fs::DISTORTION_POLYNOMIAL;
          fs::polynomial_distortion_data_type & polDist = zDist.polynomial_distortion;
          polDist.y0 = zfitContext.params.pol_y0;
          polDist.coeffs.assign(3, 0.0);
          polDist.coeffs[0] = zfitContext.params.pol_coeffs[0];
          polDist.coeffs[1] = zfitContext.params.pol_coeffs[1];
          polDist.coeffs[2] = zfitContext.params.pol_coeffs[2];
          if (zfitContext.params.model >= ZFIT_MODEL_POLYNOMIAL3) {
            polDist.coeffs[3] = zfitContext.params.pol_coeffs[3];           
          } else if (zfitContext.params.model >= ZFIT_MODEL_POLYNOMIAL4) {
            polDist.coeffs[4] = zfitContext.params.pol_coeffs[4];           
          }
          gsl_function F;
          F.function = fs::polynomial_distortion_data_type::func_polynomial_path;
          F.params = &polDist;
          double result;
          double error;
          double delta_y = dy / 100;
          int j = 0;
          double deltaLength = dy;
          DT_LOG_DEBUG(logging, "deltaLength=" << (deltaLength / CLHEP::mm) << " mm");
          std::vector<double> ysR;
          std::vector<double> pathsR;
          ysR.reserve(1000);
          pathsR.reserve(1000);
          ysR.push_back(0.0);
          pathsR.push_back(0.0);
          std::vector<double> ysL;
          std::vector<double> pathsL;
          ysL.reserve(1000);
          pathsL.reserve(1000);
          ysL.push_back(0.0);
          pathsL.push_back(0.0);
          uint32_t ny = pad_.get_ny();
          double cumulLengthR = 0.0 * CLHEP::mm;
          double yR = 0.0;
          double cumulLengthL = 0.0 * CLHEP::mm;
          double yL = 0.0;
          while (yR < 0.5 * pad_.get_width()) {
            gsl_integration_qags(&F, yR, yR + delta_y, 0, 1e-4, quad_sz, w, &result, &error);
            cumulLengthR += result;
            yR += delta_y;
            ysR.push_back(yR);
            pathsR.push_back(cumulLengthR);
          }
          while (yL > 0.5 * pad_.get_width()) {
            gsl_integration_qags(&F, yL - delta_y, yL, 0, 1e-4, quad_sz, w, &result, &error);
            cumulLengthL += result;
            yL -= delta_y;
            ysL.push_back(yL);
            pathsL.push_back(cumulLengthL);
          }
          for (uint32_t i = 0; i < ysR.size(); i++) {
            DT_LOG_DEBUG(logging, "i=" << i << " yR=" << (ysR[i] / CLHEP::mm) << " mm  pathR=" << (pathsR[i] / CLHEP::mm));
          }
          for (uint32_t i = 0; i < ysL.size(); i++) {
            DT_LOG_DEBUG(logging, "i=" << i << " yL=" << (ysL[i] / CLHEP::mm) << " mm  pathL=" << (pathsL[i] / CLHEP::mm));
          }
          std::vector<double> tileYs;
          tileYs.assign(ny + 1, 0.0);
          tileYs[ny / 2] = 0.0;
          int iLast = 0;
          // Right branch:
          for (int jsy = 1;  jsy <= ny / 2; jsy++) {
            double jPathLength = jsy * deltaLength;
            while (pathsR[iLast] < jPathLength) {
              iLast++;
            }
            double pathPrevious  = pathsR[iLast-1];
            double pathLast      = pathsR[iLast];
            double yPrevious = ysR[iLast-1];
            double yLast     = ysR[iLast];
            double jY = yPrevious + (yLast - yPrevious) * (jPathLength - yPrevious) / (yLast - yPrevious);
            tileYs[ny / 2 + jsy] = jY;
          }
          // Left branch:
          for (int jsy = 1;  jsy <= ny / 2; jsy++) {
            double jPathLength = jsy * deltaLength;
            while (pathsL[iLast] < jPathLength) {
              iLast++;
            }
            double pathPrevious  = pathsL[iLast-1];
            double pathLast      = pathsL[iLast];
            double yPrevious = ysL[iLast-1];
            double yLast     = ysL[iLast];
            double jY = yPrevious + (yLast - yPrevious) * (jPathLength - yPrevious) / (yLast - yPrevious);
            tileYs[ny / 2 - jsy] = jY;
          }   
          for (uint32_t jsy = 0; jsy < tileYs.size(); jsy++) {
            DT_LOG_DEBUG(logging, "jsy=" << jsy << " tileY=" << (tileYs[jsy] / CLHEP::mm) << " mm AL=" << (jsy * deltaLength / CLHEP::mm) << " mm");
          }
          polDist.node_ys = tileYs;
          double thickness = pad_.get_thickness();
          DT_LOG_NOTICE(logging, "  Pad thickness = " << thickness / CLHEP::mm );
          DT_LOG_NOTICE(logging, "  Building polynomial mesh..." );          
          for (uint32_t jy = 0; jy <= ny; jy++) {
            double yj = tileYs[jy];
            double xj = polDist.coeffs[0];
            double dyj = yj - polDist.y0;
            xj += polDist.coeffs[1] * dyj;
            xj += polDist.coeffs[2] * gsl_pow_2(dyj);
            double dxjdy = polDist.coeffs[1];
            dxjdy += 2 * polDist.coeffs[2] * dyj;
            if (zfitContext.params.model >= ZFIT_MODEL_POLYNOMIAL3) {
              xj += polDist.coeffs[3] * gsl_pow_3(dyj);
              dxjdy += 3 * polDist.coeffs[3] * gsl_pow_2(dyj);
            }
            if (zfitContext.params.model >= ZFIT_MODEL_POLYNOMIAL4) {
              xj += polDist.coeffs[4] * gsl_pow_4(dyj);
              dxjdy += 4 * polDist.coeffs[4] * gsl_pow_3(dyj);
            }
            geomtools::vector_3d tangentj(dxjdy, 1.0, 0.0);
            geomtools::vector_3d normalj(1.0, -dxjdy, 0.0);
            double mnormalj = normalj.mag();
            normalj /= mnormalj;
            geomtools::vector_3d p1(xj, yj - yref, z);
            p1 -= 0.5 * thickness * normalj;
            geomtools::vector_3d p2 = p1 + thickness * normalj;
            if (pad_.has_film()) {
              geomtools::vector_3d b1 = p1 - (pad_.get_film_thickness() + _fit_config_.safe_film_gap) * normalj;
              geomtools::vector_3d b2 = p1 - _fit_config_.safe_film_gap * normalj;
              geomtools::vector_3d f1 = p2 + _fit_config_.safe_film_gap * normalj;
              geomtools::vector_3d f2 = p2 + (pad_.get_film_thickness() + _fit_config_.safe_film_gap) * normalj;
              pad_.grab_back_film_vertex(vertex3d_id(pad::FACE_FRONT, jy, kz)).pos = b2;
              pad_.grab_back_film_vertex(vertex3d_id(pad::FACE_BACK, jy, kz)).pos  = b1;
              pad_.grab_front_film_vertex(vertex3d_id(pad::FACE_FRONT, jy, kz)).pos = f2;
              pad_.grab_front_film_vertex(vertex3d_id(pad::FACE_BACK, jy, kz)).pos  = f1;
            }
          }
          DT_LOG_NOTICE(logging, "  Polynomial mesh is done." );
        } // end of polynomial fit
      }
      // break;
    } // end of loop on Z
    gsl_integration_workspace_free(w);
    return;
  }

  void foil_shaping_fit_system::shape_pad2(pad & pad_) const
  {
    DT_LOG_NOTICE(logging, "Shaping pad mode=2 for strip #" << _fit_config_.strip_id << "...");
    double width = pad_.get_width(); 
    double dy = pad_.get_dy();
    if (pad_.has_distortion()) {
      pad_.grab_distortion().reset();
    }
    const strip & theStrip = _geom_->get_strips()[_fit_config_.strip_id];
    double stripYmin = theStrip.ymin();
    double stripYmax = theStrip.ymax();
    double yref = pad_.get_original_position().y();
    std::vector<double> zTabLtdYmin;
    std::vector<double> zTabLtdYmax;
    for (const auto & zfitContext : _zfits_) {
      int kz = zfitContext.kz;
      double z = zfitContext.z;
      unsigned int dkz = 4;
      int kzMin = kz - dkz;
      int kzMax = kz + dkz;
      if (kzMin < 0) kzMin = 0;
      if (kzMax > pad_.get_nz()) kzMax = pad_.get_nz();
      std::vector<double> tabKz;
      std::vector<double> tabYmin;
      std::vector<double> tabYmax;
      std::vector<double> tabYweight;
      for (int ikz = kzMin; ikz <= kzMax; ikz++) {
        const ltd_zband & zBandi = _zbands_[ikz];
        tabKz.push_back(ikz * 1.0);
        tabYmin.push_back(zBandi.ymin);
        tabYmax.push_back(zBandi.ymax);
        tabYweight.push_back(1.0/(1.0 * CLHEP::mm * 1.0 * CLHEP::mm));
      }
      double c0, c1, cov00, cov01, cov11, sumq;
      int fit_result = gsl_fit_wlinear(tabKz.data(), 1,
                                       tabYweight.data(), 1,
                                       tabYmin.data(), 1,
                                       tabKz.size(),
                                       &c0, &c1, &cov00, &cov01, &cov11, &sumq);
      const ltd_zband & zBand = _zbands_[kz];
      double yLtdMin = zBand.ymin;
      double yLtdMax = zBand.ymax;
      double yLtdMinErr;
      gsl_fit_linear_est(kz * 1.0, c0, c1, cov00, cov01, cov11, &yLtdMin, &yLtdMinErr);
      fit_result = gsl_fit_wlinear(tabKz.data(), 1,
                                   tabYweight.data(), 1,
                                   tabYmax.data(), 1,
                                   tabKz.size(),
                                   &c0, &c1, &cov00, &cov01, &cov11, &sumq);
      double yLtdMaxErr;
      gsl_fit_linear_est(kz * 1.0, c0, c1, cov00, cov01, cov11, &yLtdMax, &yLtdMaxErr);

      // yMin -= yMinErr * std::sqrt(kzMax - kzMin + 1);
      // yMax = std::min(yMin+width, yMax);
      
      DT_LOG_NOTICE(logging, "kz = " << kz << " ,  z = " << z / CLHEP::mm << " mm");
      DT_LOG_NOTICE(logging, "zBand = " << zBand);
      DT_LOG_NOTICE(logging, "yLtdMin = " << yLtdMin / CLHEP::mm << " +/- " << yLtdMinErr << " mm");
      DT_LOG_NOTICE(logging, "yLtdMax = " << yLtdMax / CLHEP::mm << " +/- " << yLtdMaxErr << " mm");
      zTabLtdYmin.push_back(yLtdMin);
      zTabLtdYmax.push_back(yLtdMax);
    }
    for (const auto & zfitContext : _zfits_) {
      int kz = zfitContext.kz;
      const ltd_zband & zBand = _zbands_[kz];
      double z = zfitContext.z;
      double yLtdMin = zTabLtdYmin[kz];
      double yLtdMax = zTabLtdYmax[kz];
      DT_LOG_NOTICE(logging, "kz = " << kz << " ,  z = " << z / CLHEP::mm << " mm");
      DT_LOG_NOTICE(logging, "zBand = " << zBand);
      DT_LOG_NOTICE(logging, "yLtdMin = " << yLtdMin / CLHEP::mm << " mm");
      DT_LOG_NOTICE(logging, "yLtdMax = " << yLtdMax / CLHEP::mm << " mm");
      {
        pad::z_distortion_data_type zDist0;
        pad_.grab_distortion().z_distortions.push_back(zDist0);
      }
      pad::z_distortion_data_type & zDist = pad_.grab_distortion().z_distortions.back();
      zDist.kz = kz;
      // Extract fit parameters and build the geometry model of the pad:
      if (zfitContext.params.model == ZFIT_MODEL_ELLIPTIC) {
        DT_LOG_NOTICE(logging, "  Fit model: elliptic fit");
        // Elliptic fit:
        zDist.model = fs::DISTORTION_ELLIPTIC;
        fs::elliptic_distortion_data_type & ellDist = zDist.elliptic_distortion;
        ellDist.a = zfitContext.params.ell_a;
        ellDist.b = zfitContext.params.ell_b;
        ellDist.x0 = zfitContext.params.ell_x0;
        ellDist.y0 = zfitContext.params.ell_y0;
        DT_LOG_NOTICE(logging, "  a=" << ellDist.a);
        DT_LOG_NOTICE(logging, "  b=" << ellDist.b);
        DT_LOG_NOTICE(logging, "  x0=" << ellDist.x0);
        DT_LOG_NOTICE(logging, "  y0=" << ellDist.y0);
        bool concave = false;
        if (ellDist.x0 < 0.0) concave = true;
        DT_LOG_NOTICE(logging, "  concave=" << std::boolalpha << concave);
        gsl_function F;
        F.function = fs::elliptic_distortion_data_type::func_elliptic_arc;
        F.params = &ellDist;
        double safeWidth = theStrip.get_model().width - 0.5 * CLHEP::mm;
        std::vector<double> thetas1;
        std::vector<double> arcs1;
        thetas1.reserve(1000);
        arcs1.reserve(1000);
        int iFirst = -1;
        int iLast  = -1;
        {
          double cumulLength1 = 0.0 * CLHEP::mm;
          double effYmin1 = yLtdMin;
          double effYmax1 = yLtdMax;
          DT_LOG_NOTICE(logging, "  yLtdMin=" << yLtdMin / CLHEP::mm << " mm");
          DT_LOG_NOTICE(logging, "  yLtdMax=" << yLtdMax / CLHEP::mm << " mm");
          double dy1 = effYmin1 - ellDist.y0;
          double dy2 = effYmax1 - ellDist.y0;
          double dthetaSafe = 1.0 *  CLHEP::degree;
          double tLtdMin1 = std::asin(dy1 / ellDist.b);
          double tLtdMax1 = std::asin(dy2 / ellDist.b);
          if (std::isnan(tLtdMin1)) {
            tLtdMin1 = - M_PI/2 + dthetaSafe;
          }
          if (std::isnan(tLtdMax1)) {
            tLtdMax1 = + M_PI/2 - dthetaSafe;
          }
          DT_LOG_NOTICE(logging, "  tLtdMin1=" << tLtdMin1 / CLHEP::degree << "°");
          DT_LOG_NOTICE(logging, "  tLtdMax1=" << tLtdMax1 / CLHEP::degree << "°");
          double dtheta1 = 0.25 *  CLHEP::degree;
          double theta1 = - M_PI/2 + dtheta1;
          double maxTheta1 = + M_PI/2 - dtheta1;
          thetas1.push_back(theta1);
          arcs1.push_back(0.0);
          double result1;
          double error1;
          int iLtd1 = -1;
          int iLtd2 = -1;
          double arc_length = 0.0; 
          int icount = 0;
          uint32_t quad_sz = 2000;
          gsl_integration_workspace * w = gsl_integration_workspace_alloc(quad_sz);
          int iLtdWidth = -1;
          while (theta1 < maxTheta1) {
            // DT_LOG_NOTICE(logging, "  Run qags for theta1=" << theta1);
            gsl_integration_qags(&F, theta1, theta1 + dtheta1, 0, 1e-4, quad_sz, w, &result1, &error1);
            cumulLength1 += result1;
            theta1 += dtheta1;
            icount++;
            if (iLtd1  < 0 ) {
              if (theta1 > tLtdMin1) {
                iLtd1 = icount - 1;
              }
            } else {
              if (iLtd2 < 0) {
                arc_length += result1;
                if (arc_length > safeWidth and iLtdWidth < 0) {
                  iLtdWidth = icount;
                }
                if (theta1 > tLtdMax1) {
                  iLtd2 = icount;
                }
              }
            }
            thetas1.push_back(theta1);
            arcs1.push_back(cumulLength1);
          }
          gsl_integration_workspace_free(w);
          DT_LOG_NOTICE(logging, "  arcs1.size()=" << arcs1.size());
          DT_LOG_NOTICE(logging, "  iLtd1=" << iLtd1);
          DT_LOG_NOTICE(logging, "  iLtd2=" << iLtd2);
          DT_LOG_NOTICE(logging, "  arc_length=" << arc_length / CLHEP::mm << " mm");
          if (arc_length > safeWidth) {
            DT_LOG_NOTICE(logging, "  BEWARE ! arc_length > safeWidth -> Fix/hack it!");
            iLtd2 = iLtdWidth;
            arc_length = safeWidth;
            DT_LOG_NOTICE(logging, "  iLtd2=" << iLtd2 << " new");
          }
          int iLtdMid = -1;
          for (unsigned int i = iLtd1; i < iLtd2; i++) {
            if (arcs1[i] >= arcs1[iLtd1] + 0.5 * arc_length) {
              iLtdMid = i;
              break;
            }
          }
          DT_LOG_NOTICE(logging, "  iLtdMid=" << iLtdMid);
          double arcMid   = arcs1[iLtdMid];
          double arcFirst = arcMid - 0.5 * safeWidth;
          double arcLast  = arcMid + 0.5 * safeWidth;
          DT_LOG_NOTICE(logging, "  arcMid   =" << arcMid / CLHEP::mm << " mm");
          DT_LOG_NOTICE(logging, "  arcFirst =" << arcFirst / CLHEP::mm << " mm");
          DT_LOG_NOTICE(logging, "  arcLast  =" << arcLast / CLHEP::mm << " mm");
          for (unsigned int i = 0; i < arcs1.size(); i++) {
            if (iFirst < 0) {
              if (arcs1[i] > arcFirst) {
                iFirst = i - 1;
              }
            } else {
              if (iLast < 0) {
                if (arcs1[i] > arcLast) {
                  iLast = i;
                  break;
                }
              }
            }
          }
          DT_LOG_NOTICE(logging, "  iFirst=" << iFirst);
          DT_LOG_NOTICE(logging, "  iLast=" << iLast);
        }
        double yMin = ellDist.y0 + ellDist.b * std::sin(thetas1[iFirst]);
        double yMax = ellDist.y0 + ellDist.b * std::sin(thetas1[iLast]);
        DT_LOG_NOTICE(logging, "  yMin=" << (yMin / CLHEP::mm) << " mm");
        DT_LOG_NOTICE(logging, "  yMax=" << (yMax / CLHEP::mm) << " mm");

        // Build tiles through interpolation:
        double deltaLength = dy;
        DT_LOG_NOTICE(logging, "  deltaLength=" << (deltaLength / CLHEP::mm) << " mm");
        uint32_t ny = pad_.get_ny();
        std::vector<double> tileAngles;
        tileAngles.assign(ny + 1, 0.0);
        tileAngles[0.0] = thetas1[iFirst];
        DT_LOG_NOTICE(logging, "  Interpolating angle...");
        int iCurrent = iFirst;
        for (int jsy = 1;  jsy <= ny; jsy++) {
          DT_LOG_NOTICE(logging, "    jsy=" << jsy);
          DT_LOG_NOTICE(logging, "    jstart @ " << iCurrent);
          double jArcLength = jsy * deltaLength;
          DT_LOG_NOTICE(logging, "    jArcLength =" << jArcLength / CLHEP::mm << " mm");
          double dArc = arcs1[iCurrent] - arcs1[iFirst];
          DT_LOG_NOTICE(logging, "    dArc =" << dArc / CLHEP::mm << " mm");
          while (dArc < jArcLength) {
            if (iCurrent + 1 == arcs1.size()) {
              break;
            } else {
              iCurrent++;
            }
            dArc = arcs1[iCurrent] - arcs1[iFirst];
          }
          DT_LOG_NOTICE(logging, "    found @ " << iCurrent);
          double arcPrevious   = arcs1[iCurrent-1] - arcs1[iFirst];
          double arcLast       = arcs1[iCurrent] - arcs1[iFirst];
          double thetaPrevious = thetas1[iCurrent-1];
          double thetaLast     = thetas1[iCurrent];
          DT_LOG_NOTICE(logging, "    thetaPrevious = " << thetaPrevious / CLHEP::degree << "°");
          DT_LOG_NOTICE(logging, "    thetaLast     = " << thetaLast / CLHEP::degree << "°");
          DT_LOG_NOTICE(logging, "    arcPrevious   = " << arcPrevious / CLHEP::mm << " mm");
          DT_LOG_NOTICE(logging, "    arcLast       = " << arcLast / CLHEP::mm << " mm");
          double jTheta = thetaPrevious + (thetaLast - thetaPrevious) * (jArcLength - arcPrevious) / (arcLast - arcPrevious);
          if (jTheta >= M_PI /2 ) jTheta = M_PI/2 - 0.25 * CLHEP::degree;
          DT_LOG_NOTICE(logging, "    jTheta        = " << jTheta / CLHEP::degree << "°");
          tileAngles[jsy] = jTheta;
        }
        DT_LOG_NOTICE(logging, "  Interpolation is done:");
        for (uint32_t jsy = 0; jsy < tileAngles.size(); jsy++) {
          DT_LOG_NOTICE(logging,
                       "  jsy=" << jsy << " tileAngle=" << (tileAngles[jsy] * 180 / M_PI) << "°  "
                       << "Arc length=" << (jsy * deltaLength / CLHEP::mm) << " mm");
        }
        ellDist.node_angles = tileAngles;
        DT_LOG_NOTICE(logging, "  Building the elliptic mesh...");
        double c = hypot(ellDist.a, ellDist.b);
        double thickness = pad_.get_thickness();
        DT_LOG_NOTICE(logging, "  Thickness = " << thickness / CLHEP::mm << " mm");
        for (uint32_t jy = 0; jy <= ny; jy++) {
          double tj = tileAngles[jy];
          if (not concave) {
            tj = M_PI - tileAngles[jy];
          }
          double xj = ellDist.a * std::cos(tj);
          double yj = ellDist.b * std::sin(tj);
          geomtools::vector_3d normalj(ellDist.b * std::cos(tj), ellDist.a * std::sin(tj), 0.0);
          double mnormalj = normalj.mag();
          normalj /= mnormalj;
          geomtools::vector_3d p1(xj + ellDist.x0, yj + ellDist.y0 - yref, z);
          p1 -= 0.5 * thickness * normalj;
          geomtools::vector_3d p2 = p1 + thickness * normalj;
          if (not concave) {
            pad_[vertex3d_id(pad::FACE_BACK, jy, kz)].pos  = p2;
            pad_[vertex3d_id(pad::FACE_FRONT, jy, kz)].pos = p1;
          } else {
            pad_[vertex3d_id(pad::FACE_FRONT, jy, kz)].pos = p2;
            pad_[vertex3d_id(pad::FACE_BACK, jy, kz)].pos  = p1;
          }
          if (pad_.has_film()) {
            geomtools::vector_3d b1 = p1 - (pad_.get_film_thickness() + _fit_config_.safe_film_gap) * normalj;
            geomtools::vector_3d b2 = p1 - _fit_config_.safe_film_gap * normalj;
            geomtools::vector_3d f1 = p2 + _fit_config_.safe_film_gap * normalj;
            geomtools::vector_3d f2 = p2 + (pad_.get_film_thickness() + _fit_config_.safe_film_gap) * normalj;
            if (not concave) {
              pad_.grab_back_film_vertex(vertex3d_id(pad::FACE_BACK, jy, kz)).pos  = b2;
              pad_.grab_back_film_vertex(vertex3d_id(pad::FACE_FRONT, jy, kz)).pos = b1;
              pad_.grab_front_film_vertex(vertex3d_id(pad::FACE_BACK, jy, kz)).pos  = f2;
              pad_.grab_front_film_vertex(vertex3d_id(pad::FACE_FRONT, jy, kz)).pos = f1;
            } else {
              pad_.grab_back_film_vertex(vertex3d_id(pad::FACE_FRONT, jy, kz)).pos = b2;
              pad_.grab_back_film_vertex(vertex3d_id(pad::FACE_BACK, jy, kz)).pos  = b1;
              pad_.grab_front_film_vertex(vertex3d_id(pad::FACE_FRONT, jy, kz)).pos = f2;
              pad_.grab_front_film_vertex(vertex3d_id(pad::FACE_BACK, jy, kz)).pos  = f1;
            }
          }
        }
        DT_LOG_NOTICE(logging, "  Elliptic mesh is done." );
      } else {
        DT_THROW(std::logic_error, "Unsupported mesh builder from polynomial model!")
        if (zfitContext.params.model >= ZFIT_MODEL_POLYNOMIAL2) {
          // Polynomial fit:
          DT_LOG_NOTICE(logging, "  Fit model: polynomial fit");
          zDist.model = fs::DISTORTION_POLYNOMIAL;
          fs::polynomial_distortion_data_type & polDist = zDist.polynomial_distortion;
          polDist.y0 = zfitContext.params.pol_y0;
          polDist.coeffs.assign(3, 0.0);
          polDist.coeffs[0] = zfitContext.params.pol_coeffs[0];
          polDist.coeffs[1] = zfitContext.params.pol_coeffs[1];
          polDist.coeffs[2] = zfitContext.params.pol_coeffs[2];
          if (zfitContext.params.model >= ZFIT_MODEL_POLYNOMIAL3) {
            polDist.coeffs[3] = zfitContext.params.pol_coeffs[3];           
          } else if (zfitContext.params.model >= ZFIT_MODEL_POLYNOMIAL4) {
            polDist.coeffs[4] = zfitContext.params.pol_coeffs[4];           
          }
          gsl_function F;
          F.function = fs::polynomial_distortion_data_type::func_polynomial_path;
          F.params = &polDist;
          double result;
          double error;
          double delta_y = dy / 100;
          int j = 0;
          double deltaLength = dy;
          DT_LOG_DEBUG(logging, "deltaLength=" << (deltaLength / CLHEP::mm) << " mm");
          std::vector<double> ysR;
          std::vector<double> pathsR;
          ysR.reserve(1000);
          pathsR.reserve(1000);
          ysR.push_back(0.0);
          pathsR.push_back(0.0);
          std::vector<double> ysL;
          std::vector<double> pathsL;
          ysL.reserve(1000);
          pathsL.reserve(1000);
          ysL.push_back(0.0);
          pathsL.push_back(0.0);
          uint32_t ny = pad_.get_ny();
          double cumulLengthR = 0.0 * CLHEP::mm;
          double yR = 0.0;
          double cumulLengthL = 0.0 * CLHEP::mm;
          double yL = 0.0;
          uint32_t quad_sz = 2000;
          gsl_integration_workspace * w = gsl_integration_workspace_alloc(quad_sz);
          while (yR < 0.5 * pad_.get_width()) {
            gsl_integration_qags(&F, yR, yR + delta_y, 0, 1e-4, quad_sz, w, &result, &error);
            cumulLengthR += result;
            yR += delta_y;
            ysR.push_back(yR);
            pathsR.push_back(cumulLengthR);
          }
          while (yL > 0.5 * pad_.get_width()) {
            gsl_integration_qags(&F, yL - delta_y, yL, 0, 1e-4, quad_sz, w, &result, &error);
            cumulLengthL += result;
            yL -= delta_y;
            ysL.push_back(yL);
            pathsL.push_back(cumulLengthL);
          }
          gsl_integration_workspace_free(w);
          for (uint32_t i = 0; i < ysR.size(); i++) {
            DT_LOG_DEBUG(logging, "i=" << i << " yR=" << (ysR[i] / CLHEP::mm) << " mm  pathR=" << (pathsR[i] / CLHEP::mm));
          }
          for (uint32_t i = 0; i < ysL.size(); i++) {
            DT_LOG_DEBUG(logging, "i=" << i << " yL=" << (ysL[i] / CLHEP::mm) << " mm  pathL=" << (pathsL[i] / CLHEP::mm));
          }
          std::vector<double> tileYs;
          tileYs.assign(ny + 1, 0.0);
          tileYs[ny / 2] = 0.0;
          int iLast = 0;
          // Right branch:
          for (int jsy = 1;  jsy <= ny / 2; jsy++) {
            double jPathLength = jsy * deltaLength;
            while (pathsR[iLast] < jPathLength) {
              iLast++;
            }
            double pathPrevious  = pathsR[iLast-1];
            double pathLast      = pathsR[iLast];
            double yPrevious = ysR[iLast-1];
            double yLast     = ysR[iLast];
            double jY = yPrevious + (yLast - yPrevious) * (jPathLength - yPrevious) / (yLast - yPrevious);
            tileYs[ny / 2 + jsy] = jY;
          }
          // Left branch:
          for (int jsy = 1;  jsy <= ny / 2; jsy++) {
            double jPathLength = jsy * deltaLength;
            while (pathsL[iLast] < jPathLength) {
              iLast++;
            }
            double pathPrevious  = pathsL[iLast-1];
            double pathLast      = pathsL[iLast];
            double yPrevious = ysL[iLast-1];
            double yLast     = ysL[iLast];
            double jY = yPrevious + (yLast - yPrevious) * (jPathLength - yPrevious) / (yLast - yPrevious);
            tileYs[ny / 2 - jsy] = jY;
          }   
          for (uint32_t jsy = 0; jsy < tileYs.size(); jsy++) {
            DT_LOG_DEBUG(logging, "jsy=" << jsy << " tileY=" << (tileYs[jsy] / CLHEP::mm) << " mm AL=" << (jsy * deltaLength / CLHEP::mm) << " mm");
          }
          polDist.node_ys = tileYs;
          double thickness = pad_.get_thickness();
          DT_LOG_NOTICE(logging, "  Pad thickness = " << thickness / CLHEP::mm );
          DT_LOG_NOTICE(logging, "  Building polynomial mesh..." );
          
          for (uint32_t jy = 0; jy <= ny; jy++) {
            double yj = tileYs[jy];
            double xj = polDist.coeffs[0];
            double dyj = yj - polDist.y0;
            xj += polDist.coeffs[1] * dyj;
            xj += polDist.coeffs[2] * gsl_pow_2(dyj);
            double dxjdy = polDist.coeffs[1];
            dxjdy += 2 * polDist.coeffs[2] * dyj;
            if (zfitContext.params.model >= ZFIT_MODEL_POLYNOMIAL3) {
              xj += polDist.coeffs[3] * gsl_pow_3(dyj);
              dxjdy += 3 * polDist.coeffs[3] * gsl_pow_2(dyj);
            }
            if (zfitContext.params.model >= ZFIT_MODEL_POLYNOMIAL4) {
              xj += polDist.coeffs[4] * gsl_pow_4(dyj);
              dxjdy += 4 * polDist.coeffs[4] * gsl_pow_3(dyj);
            }
            geomtools::vector_3d tangentj(dxjdy, 1.0, 0.0);
            geomtools::vector_3d normalj(1.0, -dxjdy, 0.0);
            double mnormalj = normalj.mag();
            normalj /= mnormalj;
            geomtools::vector_3d p1(xj, yj - yref, z);
            p1 -= 0.5 * thickness * normalj;
            geomtools::vector_3d p2 = p1 + thickness * normalj;
            if (pad_.has_film()) {
              geomtools::vector_3d b1 = p1 - (pad_.get_film_thickness() + _fit_config_.safe_film_gap) * normalj;
              geomtools::vector_3d b2 = p1 - _fit_config_.safe_film_gap * normalj;
              geomtools::vector_3d f1 = p2 + _fit_config_.safe_film_gap * normalj;
              geomtools::vector_3d f2 = p2 + (pad_.get_film_thickness() + _fit_config_.safe_film_gap) * normalj;
              pad_.grab_back_film_vertex(vertex3d_id(pad::FACE_FRONT, jy, kz)).pos = b2;
              pad_.grab_back_film_vertex(vertex3d_id(pad::FACE_BACK, jy, kz)).pos  = b1;
              pad_.grab_front_film_vertex(vertex3d_id(pad::FACE_FRONT, jy, kz)).pos = f2;
              pad_.grab_front_film_vertex(vertex3d_id(pad::FACE_BACK, jy, kz)).pos  = f1;
            }
          }
          DT_LOG_NOTICE(logging, "  Polynomial mesh is done." );
        } // end of polynomial fit
      }
      // break;
    } // end of loop on Z
    return;
  }

  void foil_shaping_fit_system::shape_pad_elliptic_1(pad & pad_) const
  {
    DT_LOG_NOTICE(logging, "Shaping pad mode=3 : elliptic fit only for strip #" << _fit_config_.strip_id << "...");
    double width = pad_.get_width(); 
    double dy = pad_.get_dy();
    if (pad_.has_distortion()) {
      pad_.grab_distortion().reset();
    }
    const strip & theStrip = _geom_->get_strips()[_fit_config_.strip_id];
    double stripYmin = theStrip.ymin();
    double stripYmax = theStrip.ymax();
    double yRef = pad_.get_original_position().y();
    DT_LOG_NOTICE(logging, "yRef = " << yRef / CLHEP::mm << " mm");
    double theta0 = 0.0 * CLHEP::degree;
    // double thetaRef = std::asin((yRef - ellDist.y0) / ellDist.b);
    // DT_LOG_NOTICE(logging, "thetaRef = " << thetaRef / CLHEP::degree << "°");
    size_t zConcaveCounter = 0;
    size_t zConvexCounter = 0;
    for (const auto & zfitContext : _zfits_) {
      int kz = zfitContext.kz;
      const ltd_zband & zBand = _zbands_[kz];
      double z = zfitContext.z;
      double yLtdMin = _zTabLtdYmin_[kz];
      double yLtdMax = _zTabLtdYmax_[kz];
      DT_LOG_NOTICE(logging, "kz = " << kz << " ,  z = " << z / CLHEP::mm << " mm");
      DT_LOG_NOTICE(logging, "  zBand   = " << zBand);
      DT_LOG_NOTICE(logging, "  yLtdMin = " << yLtdMin / CLHEP::mm << " mm");
      DT_LOG_NOTICE(logging, "  yLtdMax = " << yLtdMax / CLHEP::mm << " mm");
      {
        pad::z_distortion_data_type zDist0;
        pad_.grab_distortion().z_distortions.push_back(zDist0);
      }
      pad::z_distortion_data_type & zDist = pad_.grab_distortion().z_distortions.back();
      zDist.kz = kz;
      // Extract fit parameters and build the geometry model of the pad:
      DT_THROW_IF(zfitContext.params.model != ZFIT_MODEL_ELLIPTIC,
                  std::logic_error, "Unsupported mesh builder from non elliptic fit model!");
      DT_LOG_NOTICE(logging, "  Fit model: elliptic fit");
      // Elliptic fit:
      zDist.model = fs::DISTORTION_ELLIPTIC;
      fs::elliptic_distortion_data_type & ellDist = zDist.elliptic_distortion;
      ellDist.a = zfitContext.params.ell_a;
      ellDist.b = zfitContext.params.ell_b;
      ellDist.x0 = zfitContext.params.ell_x0;
      ellDist.y0 = zfitContext.params.ell_y0;
      DT_LOG_NOTICE(logging, "  a=" << ellDist.a);
      DT_LOG_NOTICE(logging, "  b=" << ellDist.b);
      DT_LOG_NOTICE(logging, "  x0=" << ellDist.x0);
      DT_LOG_NOTICE(logging, "  y0=" << ellDist.y0);
      bool concave = false;
      if (ellDist.x0 < 0.0) concave = true;
      if (concave) {
        zConcaveCounter++;
      } else {
        zConvexCounter++;
      }
      DT_LOG_NOTICE(logging, "  concave=" << std::boolalpha << concave);
      gsl_function F;
      F.function = fs::elliptic_distortion_data_type::func_elliptic_arc;
      F.params = &ellDist;
      double safeWidth = theStrip.get_model().width - 0.5 * CLHEP::mm;
      std::vector<double> thetas1;
      std::vector<double> arcs1;
      thetas1.reserve(1000);
      arcs1.reserve(1000);
      int iFirst = -1;
      int iLast  = -1;
      {
        double cumulLength1 = 0.0 * CLHEP::mm;
        double effYmin1 = yLtdMin;
        double effYmax1 = yLtdMax;
        DT_LOG_NOTICE(logging, "  yLtdMin=" << yLtdMin / CLHEP::mm << " mm");
        DT_LOG_NOTICE(logging, "  yLtdMax=" << yLtdMax / CLHEP::mm << " mm");
        double dy1 = effYmin1 - ellDist.y0;
        double dy2 = effYmax1 - ellDist.y0;
        double dthetaSafe = 1.0 *  CLHEP::degree;
        double tLtdMin1 = std::asin(dy1 / ellDist.b);
        double tLtdMax1 = std::asin(dy2 / ellDist.b);
        if (std::isnan(tLtdMin1)) {
          DT_LOG_WARNING(logging, "  tLtdMin1 is NAN => Force a fix");
          tLtdMin1 = - M_PI/2 + dthetaSafe;
        }
        if (std::isnan(tLtdMax1)) {
          DT_LOG_WARNING(logging, "  tLtdMax1 is NAN => Force a fix");
          tLtdMax1 = + M_PI/2 - dthetaSafe;
        }
        DT_LOG_NOTICE(logging, "  tLtdMin1=" << tLtdMin1 / CLHEP::degree << "°");
        DT_LOG_NOTICE(logging, "  tLtdMax1=" << tLtdMax1 / CLHEP::degree << "°");
        double dtheta1 = 0.1 *  CLHEP::degree;
        double theta1 = - M_PI/2 + dtheta1;
        double maxTheta1 = + M_PI/2 - dtheta1;
        thetas1.push_back(theta1);
        arcs1.push_back(0.0);
        double result1;
        double error1;
        int iLtd1 = -1;
        int iLtd2 = -1;
        double arc0 = datatools::invalid_real();
        int iArc0 = -1;
        double arcRef = datatools::invalid_real();
        int iRef = -1;
        double arcMed = datatools::invalid_real();
        double yMed = (0.25 * ellDist.y0 + 0.75 * yRef);
        int iMed = -1;
        double arc_length = 0.0; 
        int icount = 0;
        uint32_t quad_sz = 2000;
        gsl_integration_workspace * w = gsl_integration_workspace_alloc(quad_sz);
        int iLtdWidth = -1;
        while (theta1 < maxTheta1) {
          // DT_LOG_NOTICE(logging, "  qags for theta1=" << theta1 / CLHEP::degree << "°");
          gsl_integration_qags(&F, theta1, theta1 + dtheta1, 0, 1e-4, quad_sz, w, &result1, &error1);
          cumulLength1 += result1;
          theta1 += dtheta1;
          icount++;
          double y = ellDist.y0 + ellDist.b * std::sin(theta1);
          if (iRef < 0) {
            if (y > yRef) {
              iRef = icount;
              arcRef = cumulLength1;
            }
          } 
          if (iMed < 0) {
            if (y > yMed) {
              iMed = icount;
              arcMed = cumulLength1;
            }
          } 
          if (iArc0 < 0) {
            if (theta1 > theta0) {
              iArc0 = icount;
              arc0 = cumulLength1;
            }
          }
          if (iLtd1  < 0 ) {
            if (theta1 > tLtdMin1) {
              iLtd1 = icount - 1;
            }
          } else {
            if (iLtd2 < 0) {
              arc_length += result1;
              if (arc_length > safeWidth and iLtdWidth < 0) {
                iLtdWidth = icount;
              }
              if (theta1 > tLtdMax1) {
                iLtd2 = icount;
              }
            }
          }
          thetas1.push_back(theta1);
          arcs1.push_back(cumulLength1);
        }
        gsl_integration_workspace_free(w);   
        DT_LOG_NOTICE(logging, "  arcs1.size() = " << arcs1.size());
        DT_LOG_NOTICE(logging, "  cumulLength1 = " << cumulLength1 / CLHEP::mm << " mm");
        DT_LOG_NOTICE(logging, "  iArc0   = " << iArc0);
        DT_LOG_NOTICE(logging, "  arc0    = " << arc0 / CLHEP::mm << " mm");
        DT_LOG_NOTICE(logging, "  iRef    = " << iRef);
        DT_LOG_NOTICE(logging, "  arcRef  = " << arcRef/ CLHEP::mm << " mm");
        DT_LOG_NOTICE(logging, "  ==> iLtd1 = " << iLtd1);
        DT_LOG_NOTICE(logging, "  ==> iLtdWidth = " << iLtdWidth);
        DT_LOG_NOTICE(logging, "  iLtd2 = " << iLtd2);
        DT_LOG_NOTICE(logging, "  arc_length=" << arc_length / CLHEP::mm << " mm");
        if (arc_length > safeWidth) {
          DT_LOG_NOTICE(logging, "  BEWARE ! arc_length > safeWidth -> Fix/hack it!");
          iLtd2 = iLtdWidth;
          arc_length = safeWidth;
          DT_LOG_NOTICE(logging, "  iLtd2=" << iLtd2 << " new");
        }
        int iLtdMid = -1;
        for (unsigned int i = iLtd1; i < iLtd2; i++) {
          if (arcs1[i] >= arcs1[iLtd1] + 0.5 * arc_length) {
            iLtdMid = i;
            break;
          }
        }
        iFirst = iLtd1;
        iLast = iLtdWidth;

        // From yRef
        iFirst=-1;
        iLast=-1;
        for (unsigned int i = iRef; i > 0; i--) {
          if (iFirst < 0 and (arcs1[iRef] - arcs1[i]) > 0.5*theStrip.get_model().width) {
            iFirst = i;
            break;
          }
        }
        for (unsigned int i = iRef; i < arcs1.size(); i++) {
          if (iLast < 0 and (arcs1[i] - arcs1[iRef]) > 0.5*theStrip.get_model().width) {
            iLast = i;
            break;
          }
        }
        // // From yMed
        // iFirst=-1;
        // iLast=-1;
        // for (unsigned int i = iMed; i > 0; i--) {
        //   if (iFirst < 0 and (arcs1[iMed] - arcs1[i]) > 0.5*theStrip.get_model().width) {
        //     iFirst = i;
        //     break;
        //   }
        // }
        // for (unsigned int i = iMed; i < arcs1.size(); i++) {
        //   if (iLast < 0 and (arcs1[i] - arcs1[iMed]) > 0.5*theStrip.get_model().width) {
        //     iLast = i;
        //     break;
        //   }
        // }
        // // From y0
        // for (unsigned int i = iArc0; i > 0; i--) {
        //   if (iFirst < 0 and (arcs1[iArc0] - arcs1[i]) > 0.5*theStrip.get_model().width) {
        //     iFirst = i;
        //     break;
        //   }
        // }
        // for (unsigned int i = iArc0; i < arcs1.size(); i++) {
        //   if (iLast < 0 and (arcs1[i] - arcs1[iArc0]) > 0.5*theStrip.get_model().width) {
        //     iLast = i;
        //     break;
        //   }
        // }
        
        // DT_LOG_NOTICE(logging, "  iLtdMid=" << iLtdMid);
        // double arcMid   = arcs1[iLtdMid];
        // double arcFirst = arcMid - 0.5 * safeWidth;
        // double arcLast  = arcMid + 0.5 * safeWidth;
        // DT_LOG_NOTICE(logging, "  arcMid   =" << arcMid / CLHEP::mm << " mm");
        // DT_LOG_NOTICE(logging, "  arcFirst =" << arcFirst / CLHEP::mm << " mm");
        // DT_LOG_NOTICE(logging, "  arcLast  =" << arcLast / CLHEP::mm << " mm");
        // for (unsigned int i = 0; i < arcs1.size(); i++) {
        //   if (iFirst < 0) {
        //     if (arcs1[i] > arcFirst) {
        //       iFirst = i - 1;
        //     }
        //   } else {
        //     if (iLast < 0) {
        //       if (arcs1[i] > arcLast) {
        //         iLast = i;
        //         break;
        //       }
        //     }
        //   }
        // }
        DT_LOG_NOTICE(logging, "  iFirst=" << iFirst);
        DT_LOG_NOTICE(logging, "  iLast=" << iLast);
      }
      double yMin = ellDist.y0 + ellDist.b * std::sin(thetas1[iFirst]);
      double yMax = ellDist.y0 + ellDist.b * std::sin(thetas1[iLast]);
      DT_LOG_NOTICE(logging, "  yMin=" << (yMin / CLHEP::mm) << " mm");
      DT_LOG_NOTICE(logging, "  yMax=" << (yMax / CLHEP::mm) << " mm");
      double deltaLength = dy;
      DT_LOG_NOTICE(logging, "  deltaLength=" << (deltaLength / CLHEP::mm) << " mm");
       
      // Build tiles through interpolation:
      uint32_t ny = pad_.get_ny();
      std::vector<double> tileAngles;
      tileAngles.assign(ny + 1, 0.0);
      tileAngles[0.0] = thetas1[iFirst];
      DT_LOG_NOTICE(logging, "  Interpolating angle...");
      int iCurrent = iFirst;
      for (int jsy = 1;  jsy <= ny; jsy++) {
        DT_LOG_NOTICE(logging, "    jsy        = " << jsy);
        DT_LOG_NOTICE(logging, "    jstart     @ " << iCurrent);
        double jArcLength = jsy * deltaLength;
        DT_LOG_NOTICE(logging, "    jArcLength = " << jArcLength / CLHEP::mm << " mm");
        double dArc = arcs1[iCurrent] - arcs1[iFirst];
        DT_LOG_NOTICE(logging, "    dArc       = " << dArc / CLHEP::mm << " mm");
        while (dArc < jArcLength) {
          if (iCurrent + 1 == arcs1.size()) {
            break;
          } else {
            iCurrent++;
          }
          dArc = arcs1[iCurrent] - arcs1[iFirst];
        }
        DT_LOG_NOTICE(logging, "    found @ " << iCurrent);
        double arcPrevious   = arcs1[iCurrent-1] - arcs1[iFirst];
        double arcLast       = arcs1[iCurrent] - arcs1[iFirst];
        double thetaPrevious = thetas1[iCurrent-1];
        double thetaLast     = thetas1[iCurrent];
        DT_LOG_NOTICE(logging, "    thetaPrevious = " << thetaPrevious / CLHEP::degree << "°");
        DT_LOG_NOTICE(logging, "    thetaLast     = " << thetaLast / CLHEP::degree << "°");
        DT_LOG_NOTICE(logging, "    arcPrevious   = " << arcPrevious / CLHEP::mm << " mm");
        DT_LOG_NOTICE(logging, "    arcLast       = " << arcLast / CLHEP::mm << " mm");
        double jTheta = thetaPrevious + (thetaLast - thetaPrevious) * (jArcLength - arcPrevious) / (arcLast - arcPrevious);
        if (jTheta >= M_PI /2 ) jTheta = M_PI/2 - 0.25 * CLHEP::degree;
        DT_LOG_NOTICE(logging, "    jTheta        = " << jTheta / CLHEP::degree << "°");
        tileAngles[jsy] = jTheta;
      }
      DT_LOG_NOTICE(logging, "  Interpolation is done:");
      for (uint32_t jsy = 0; jsy < tileAngles.size(); jsy++) {
        DT_LOG_NOTICE(logging,
                      "  jsy=" << jsy << " tileAngle=" << (tileAngles[jsy] * 180 / M_PI) << "°  "
                      << "Arc length=" << (jsy * deltaLength / CLHEP::mm) << " mm");
      }
      ellDist.node_angles = tileAngles;
      DT_LOG_NOTICE(logging, "  Building the elliptic mesh...");
      double c = hypot(ellDist.a, ellDist.b);
      double thickness = pad_.get_thickness();
      DT_LOG_NOTICE(logging, "  Thickness = " << thickness / CLHEP::mm << " mm");
      for (uint32_t jy = 0; jy <= ny; jy++) {
        double tj = tileAngles[jy];
        if (not concave) {
          tj = M_PI - tileAngles[jy];
        }
        double xj = ellDist.a * std::cos(tj);
        double yj = ellDist.b * std::sin(tj);
        geomtools::vector_3d normalj(ellDist.b * std::cos(tj), ellDist.a * std::sin(tj), 0.0);
        double mnormalj = normalj.mag();
        normalj /= mnormalj;
        geomtools::vector_3d p1(xj + ellDist.x0, yj + ellDist.y0 - yRef, z);
        p1 -= 0.5 * thickness * normalj;
        geomtools::vector_3d p2 = p1 + thickness * normalj;
        if (not concave) {
          pad_[vertex3d_id(pad::FACE_BACK, jy, kz)].pos  = p2;
          pad_[vertex3d_id(pad::FACE_FRONT, jy, kz)].pos = p1;
        } else {
          pad_[vertex3d_id(pad::FACE_FRONT, jy, kz)].pos = p2;
          pad_[vertex3d_id(pad::FACE_BACK, jy, kz)].pos  = p1;
        }
        if (pad_.has_film()) {
          geomtools::vector_3d b1 = p1 - (pad_.get_film_thickness() + _fit_config_.safe_film_gap) * normalj;
          geomtools::vector_3d b2 = p1 - _fit_config_.safe_film_gap * normalj;
          geomtools::vector_3d f1 = p2 + _fit_config_.safe_film_gap * normalj;
          geomtools::vector_3d f2 = p2 + (pad_.get_film_thickness() + _fit_config_.safe_film_gap) * normalj;
          if (not concave) {
            pad_.grab_back_film_vertex(vertex3d_id(pad::FACE_BACK, jy, kz)).pos   = f2;
            pad_.grab_back_film_vertex(vertex3d_id(pad::FACE_FRONT, jy, kz)).pos  = f1;
            pad_.grab_front_film_vertex(vertex3d_id(pad::FACE_BACK, jy, kz)).pos  = b1;
            pad_.grab_front_film_vertex(vertex3d_id(pad::FACE_FRONT, jy, kz)).pos = b2;
          } else {
            pad_.grab_back_film_vertex(vertex3d_id(pad::FACE_BACK, jy, kz)).pos   = b2;
            pad_.grab_back_film_vertex(vertex3d_id(pad::FACE_FRONT, jy, kz)).pos  = b1;
            pad_.grab_front_film_vertex(vertex3d_id(pad::FACE_BACK, jy, kz)).pos  = f1;
            pad_.grab_front_film_vertex(vertex3d_id(pad::FACE_FRONT, jy, kz)).pos = f2;
          }
        } // has film
      } // for
      DT_LOG_NOTICE(logging, "  Elliptic mesh is done." );
    } // end of loop on Z
    pad::convexity_type overallPadConvexity = pad::CONVEXITY_UNDEF;
    if (zConcaveCounter == _zfits_.size()) {
      overallPadConvexity = pad::CONVEXITY_CONCAVE;
    }
    if (zConvexCounter == _zfits_.size()) {
      overallPadConvexity = pad::CONVEXITY_CONVEX;
    }
    pad_.set_convexity(overallPadConvexity);
    return;
  }

} // namespace snrs 
