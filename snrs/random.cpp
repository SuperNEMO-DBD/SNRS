// This project:
#include <random.hpp>

// GSL:
#include <gsl/gsl_randist.h>

randomizer::randomizer(unsigned long int seed_)
{
  _T_ = gsl_rng_default;
  _r_ = gsl_rng_alloc(_T_);
  gsl_rng_set(_r_, seed_);
}

randomizer::randomizer(gsl_rng * rng_)
  : _T_(nullptr)
  , _r_(rng_)
{
}

randomizer::~randomizer()
{
  if (_T_) {
    gsl_rng_free(_r_);
  }
  _r_ = nullptr;
}

gsl_rng * randomizer::rng()
{
  return _r_;
}

double randomizer::operator()()
{
  return gsl_rng_uniform(_r_);
}

uint32_t randomizer::uniform_int(uint32_t n_)
{
  return gsl_rng_uniform_int(_r_, n_);
}

double randomizer::gauss(double sigma_)
{
  return gsl_ran_gaussian(_r_, sigma_);
}

double randomizer::gaussian_tail(double a_, double sigma_)
{
  return gsl_ran_gaussian_tail(_r_, a_, sigma_);
}

double randomizer::exponential(double mu_)
{
  return gsl_ran_exponential(_r_, mu_);
}

double randomizer::laplace(double a_)
{
  return gsl_ran_laplace(_r_, a_);
}

double randomizer::cauchy(double a_)
{
  return gsl_ran_cauchy(_r_, a_);
}

double randomizer::flat(double a_, double b_)
{
  return gsl_ran_flat(_r_, a_, b_);
}

std::tuple<double,double> randomizer::isotropic_2d()
{
  double ux, uy;
  gsl_ran_dir_2d(_r_, &ux, &uy);
  return std::tuple<double,double>(ux, uy);
}

std::tuple<double,double,double> randomizer::isotropic_3d()
{
  double ux, uy, uz;
  gsl_ran_dir_3d(_r_, &ux, &uy, &uz);
  return std::tuple<double,double,double>(ux, uy, uz);
}

