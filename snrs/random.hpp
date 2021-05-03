#ifndef SNRS_RANDOM_HPP
#define SNRS_RANDOM_HPP

// Standard library:
#include <tuple>

// GSL:
#include <gsl/gsl_rng.h>

namespace snrs {
  
struct randomizer
{
public:
  
  randomizer(unsigned long int seed_);

  randomizer(gsl_rng * rng_);
  
  ~randomizer();

  double operator()();
  
  gsl_rng * rng();

  uint32_t uniform_int(uint32_t n_);
  
  double gauss(double sigma_);

  double gaussian_tail(double a_, double sigma_);

  double exponential(double mu_);

  double laplace(double a_);

  double cauchy(double a_);

  double flat(double a_, double b_);

  std::tuple<double,double> isotropic_2d();

  std::tuple<double,double,double> isotropic_3d();

private:
  
  const gsl_rng_type * _T_ = nullptr;
  gsl_rng *            _r_ = nullptr;
  
};

} // namespace snrs

#endif // SNRS_RANDOM_HPP
