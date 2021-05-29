#ifndef SNRS_LTD_HPP
#define SNRS_LTD_HPP

// Standard library:
#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <set>
#include <iostream>

// Bayeux:
#include <datatools/utils.h>
#include <datatools/clhep_units.h>
#include <datatools/logger.h>
#include <geomtools/utils.h>

// This project:
#include <snrs/sngeom.hpp>

namespace snrs {

  struct ltd_voxel_id
  {
    ltd_voxel_id() = default;
    ltd_voxel_id(int i_, int j_, int k_);
    bool is_valid() const;
    bool operator==(const ltd_voxel_id & other_) const;
    bool operator<(const ltd_voxel_id & other_) const;
    friend std::ostream & operator<<(std::ostream & out_, const ltd_voxel_id & vid_);
    int i = -1;
    int j = -1;
    int k = -1;
  };

  struct ltd_voxel
  {
    std::set<uint32_t> point_ids;
  };

  class ltd_voxelization
  {
  public:

    typedef std::vector<ltd_voxel> line_t;
    typedef std::vector<line_t> plane_t;
    typedef std::vector<plane_t> bulk_t;
   
    ltd_voxelization() = default;
 
    ltd_voxelization(double xmin_,
                     double xmax_,
                     double ymin_,
                     double ymax_,
                     double zmin_,
                     double zmax_,
                     double dimension_);
 
    void initialize(double xmin_,
                    double xmax_,
                    double ymin_,
                    double ymax_,
                    double zmin_,
                    double zmax_,
                    double dimension_);

    ltd_voxel_id find(const geomtools::vector_3d & point_) const;

    void add(const geomtools::vector_3d & point_, uint32_t point_id_);

    bool has_voxel(const ltd_voxel_id & id_) const;

    void vicinity(const ltd_voxel_id & id_, std::set<ltd_voxel_id> & vic_) const;
  
    const ltd_voxel & get_voxel(const ltd_voxel_id & id_) const;

    uint32_t get_counter() const;

    uint32_t get_voxel_counter(const ltd_voxel_id & id_) const;

    void print(std::ostream & out_) const;

    void plot(std::ostream & out_) const;

    void load(std::istream  & in_);
  
    void store(std::ostream & out_) const;
  
    void reset();
  
  private:
  
    void _initialize_();
  
  private:
  
    double _xmin_ = datatools::invalid_real();
    double _xmax_ = datatools::invalid_real();
    double _ymin_ = datatools::invalid_real();
    double _ymax_ = datatools::invalid_real();
    double _zmin_ = datatools::invalid_real();
    double _zmax_ = datatools::invalid_real();
    double _dimension_ = datatools::invalid_real();
  
    // Internals:
    uint16_t _nx_ = 0;
    uint16_t _ny_ = 0;
    uint16_t _nz_ = 0;
    bulk_t   _voxels_;
    uint32_t _counter_ = 0;

  };

  struct ltd_zband
  {
    void reset();

    int id = -1;
    double z = datatools::invalid_real();
    double dz = datatools::invalid_real();
    double xmin = datatools::invalid_real();
    double xmax = datatools::invalid_real();
    double ymin = datatools::invalid_real();
    double ymax = datatools::invalid_real();
    std::set<uint32_t> point_ids;

    friend std::ostream & operator<<(std::ostream & out_, const ltd_zband & zband_);
  };

  /// \brief Laser tracking data I/O
  class ltd
  {
  public:

    static geomtools::vector_3d origin_uvw();

    static double xy_tilt_angle();
  
    static std::string block_filename(int block_id_);
  
    static std::tuple<double, double> strip_limits_y(int strip_id_);
  
    ltd(const sngeom & geom_,
        datatools::logger::priority logging_ = datatools::logger::PRIO_NOTICE);

    void import(const std::string & ltd_filename_,
                int strip_id_,
                double density_radius_ = 1.0 * CLHEP::cm,
                bool use_voxelisation_ = false);

    void print(std::ostream & out_) const;

    void load(const std::string & ltd_filename_);
  
    void store(std::ostream & out_) const;
 
    void plot(std::ostream & out_) const;

    uint32_t size() const;
  
    const geomtools::vector_3d & get(int i_) const;
  
    double get_w(int i_) const;
 
    void reset();

    void extract_zband(double z_, double dz_, ltd_zband & zband_) const;

    void extract_zband_min_max(double zmin_, double zmax_, ltd_zband & zband_) const;

    void y_cut_zband(double ymin_, double ymax_, ltd_zband & zband_) const;

    void generate_y_cut_zband(double ymin_, double ymax_,
                              const ltd_zband & source_, ltd_zband & target_) const;

  public:
  
    datatools::logger::priority logging = datatools::logger::PRIO_NOTICE;

  private:
  
    const sngeom * _geom_ = nullptr;
    int _strip_id_ = -1;
    double _density_radius_ = datatools::invalid_real();
    std::vector<geomtools::vector_3d> _points_;
    std::vector<double> _weights_;
    bool _use_voxelization_ = false;
    ltd_voxelization _voxelization_;
  
  };
  
} // namespace snrs 

#endif // SNRS_LTD_HPP
