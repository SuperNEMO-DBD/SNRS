// Ourselves:
#include <snrs/ltd.hpp>

// Standard library:
#include <fstream>
#include <stdexcept>
#include <cmath>

// Bayeux:
#include <bayeux/datatools/exception.h>

namespace snrs {
  
ltd_voxel_id::ltd_voxel_id(int i_, int j_, int k_)
  : i(i_)
  , j(j_)
  , k(k_)
{
}

bool ltd_voxel_id::is_valid() const
{
  if (i < 0) return false;
  if (j < 0) return false;
  if (k < 0) return false;
  return true;
}

// friend
std::ostream & operator<<(std::ostream & out_, const ltd_voxel_id & vid_)
{
  out_ << '(' << vid_.i << ',' << vid_.j << ',' << vid_.k << ')';
  return out_;
}

bool ltd_voxel_id::operator==(const ltd_voxel_id & other_) const
{
  if (i != other_.i) return false;
  if (j != other_.j) return false;
  if (k != other_.k) return false;
  return true;
}

bool ltd_voxel_id::operator<(const ltd_voxel_id & other_) const
{
  if (i < other_.i) return true;
  if (i > other_.i) return false;
  if (j < other_.j) return true;
  if (j > other_.j) return false;
  if (k < other_.k) return true;
  return false;
}

ltd_voxelization::ltd_voxelization(double xmin_,
                                   double xmax_,
                                   double ymin_,
                                   double ymax_,
                                   double zmin_,
                                   double zmax_,
                                   double dimension_)
{
  initialize(xmin_, xmax_, ymin_, ymax_, zmin_, zmax_, dimension_);
}
 
void ltd_voxelization::reset()
{
  _voxels_.clear();
  _nx_ = 0;
  _ny_ = 0;
  _nz_ = 0;
  _counter_ = 0;
  _xmin_ = datatools::invalid_real();
  _xmax_ = datatools::invalid_real();
  _ymin_ = datatools::invalid_real();
  _ymax_ = datatools::invalid_real();
  _zmin_ = datatools::invalid_real();
  _zmax_ = datatools::invalid_real();
  _dimension_ = datatools::invalid_real();
}
  
void ltd_voxelization::initialize(double xmin_,
                                  double xmax_,
                                  double ymin_,
                                  double ymax_,
                                  double zmin_,
                                  double zmax_,
                                  double dimension_)
{
 _xmin_ = xmin_;
 _xmax_ = xmax_;
 _ymin_ = ymin_;
 _ymax_ = ymax_;
 _zmin_ = zmin_;
 _zmax_ = zmax_;
 _dimension_ = dimension_;
 _initialize_();
}

void ltd_voxelization::_initialize_()
{
  _nx_ = (uint16_t) ((_xmax_ - _xmin_) / _dimension_);
  _ny_ = (uint16_t) ((_ymax_ - _ymin_) / _dimension_);
  _nz_ = (uint16_t) ((_zmax_ - _zmin_) / _dimension_);
  _nx_ += 1;
  _ny_ += 1;
  _nz_ += 1;
  _xmax_ = _xmin_ + _nx_ *_dimension_;
  _ymax_ = _ymin_ + _ny_ *_dimension_;
  _zmax_ = _zmin_ + _nz_ *_dimension_;
  ltd_voxel v0;
  line_t l0;
  l0.assign(_nx_, v0);
  plane_t p0;
  p0.assign(_ny_, l0);
  _voxels_.assign(_nz_, p0);
}

void ltd_voxelization::print(std::ostream & out_) const
{
  out_ << "|-- X min  = " << _xmin_ / CLHEP::mm << " mm" << '\n';
  out_ << "|-- X max  = " << _xmax_ / CLHEP::mm << " mm" << '\n';
  out_ << "|-- Y min  = " << _ymin_ / CLHEP::mm << " mm" << '\n';
  out_ << "|-- Y max  = " << _ymax_ / CLHEP::mm << " mm" << '\n';
  out_ << "|-- Z min  = " << _zmin_ / CLHEP::mm << " mm" << '\n';
  out_ << "|-- Z max  = " << _zmax_ / CLHEP::mm << " mm" << '\n';
  out_ << "|-- Dimension = " << _dimension_ / CLHEP::mm << " mm" << '\n';
  out_ << "|-- Nx     = " << _nx_ << '\n';
  out_ << "|-- Ny     = " << _ny_ << '\n';
  out_ << "|-- Nz     = " << _nz_ << '\n';
  out_ << "`-- #Points in voxels = " << _counter_ << " (in " << _voxels_.size() << " planes)" << '\n';
}

void ltd_voxelization::load(std::istream & in_)
{
  _voxels_.clear();
  in_ >> _xmin_ >> _xmax_ >> _ymin_ >> _ymax_ >> _zmin_ >> _zmax_ >> _dimension_ >> std::ws;
  _xmin_ *= CLHEP::mm;
  _xmax_ *= CLHEP::mm;
  _ymin_ *= CLHEP::mm;
  _ymax_ *= CLHEP::mm;
  _zmin_ *= CLHEP::mm;
  _zmax_ *= CLHEP::mm;
  _dimension_ *= CLHEP::mm;
  in_ >> _nx_ >> _ny_ >> _nz_ >> std::ws;
  ltd_voxel v0;
  line_t l0;
  l0.assign(_nx_, v0);
  plane_t p0;
  p0.assign(_ny_, l0);
  _voxels_.assign(_nz_, p0);
  for (int i = 0; i < _nx_; i++) {
    for (int j = 0; j < _ny_; j++) {
      for (int k = 0; k < _nz_; k++) {
         ltd_voxel_id voxId(i, j, k);
         ltd_voxel & vox = const_cast<ltd_voxel &>(get_voxel(voxId));
         uint32_t sz = 0;
         in_ >> sz >> std::ws;
         for (int l = 0; l < sz; l++) {
           uint32_t point_id;
           in_ >> point_id >> std::ws;
           vox.point_ids.insert(point_id);
           _counter_++;
         }
      }
    }
  }
}

void ltd_voxelization::store(std::ostream & out_) const
{
  out_ << _xmin_ / CLHEP::mm << ' ';
  out_ << _xmax_ / CLHEP::mm << '\n';
  out_ << _ymin_ / CLHEP::mm << ' ';
  out_ << _ymax_ / CLHEP::mm << '\n';
  out_ << _zmin_ / CLHEP::mm << ' ';
  out_ << _zmax_ / CLHEP::mm << '\n';
  out_ << _dimension_ / CLHEP::mm << '\n';
  out_ << _nx_ << ' ';
  out_ << _ny_ << ' ';
  out_ << _nz_ << '\n';
  for (int i = 0; i < _nx_; i++) {
    for (int j = 0; j < _ny_; j++) {
      for (int k = 0; k < _nz_; k++) {
         ltd_voxel_id voxId(i, j, k);
         const ltd_voxel & vox = get_voxel(voxId);
         out_ << vox.point_ids.size();
         if (vox.point_ids.size()) {
           for (auto point_id : vox.point_ids) {
             out_ << ' ' << point_id;
           } 
         }
         out_ << '\n';
      }
    }
  }
}

void ltd_voxelization::plot(std::ostream & out_) const
{
  int count = 0;
  double d = _dimension_;
  for (int i = 0; i < _nx_; i++) {
    for (int j = 0; j < _ny_; j++) {
      for (int k = 0; k < _nz_; k++) {
        ltd_voxel_id voxId(i, j, k);
        double x1 = _xmin_ + _dimension_ * i;
        double y1 = _ymin_ + _dimension_ * j;
        double z1 = _zmin_ + _dimension_ * k;
        double x2 = x1 + d;
        double y2 = y1 + d;
        double z2 = z1 + d;
        const ltd_voxel & vox = get_voxel(voxId);
        if (vox.point_ids.size() == 0) continue;
        /*
         *
         *    (x1,y2,z1) ----- (x2,y2,z1)
         *         |               |
         *         |               |
         *         |               |
         *         |               |
         *         |               |
         *    (x1,y1,z1) ----- (x2,y1,z1)
         *
         *
         */
        out_ << x1 << ' ' << y1 << ' ' << z1 << '\n';
        out_ << x2 << ' ' << y1 << ' ' << z1 << '\n';
        out_ << '\n';
        out_ << x1 << ' ' << y2 << ' ' << z1 << '\n';
        out_ << x2 << ' ' << y2 << ' ' << z1 << '\n';
        out_ << '\n';
        out_ << x1 << ' ' << y1 << ' ' << z1 << '\n';
        out_ << x1 << ' ' << y2 << ' ' << z1 << '\n';
        out_ << '\n';
        out_ << x2 << ' ' << y1 << ' ' << z1 << '\n';
        out_ << x2 << ' ' << y2 << ' ' << z1 << '\n';
        out_ << '\n';
          
        out_ << x1 << ' ' << y1 << ' ' << z2 << '\n';
        out_ << x2 << ' ' << y1 << ' ' << z2 << '\n';
        out_ << '\n';
        out_ << x1 << ' ' << y2 << ' ' << z2 << '\n';
        out_ << x2 << ' ' << y2 << ' ' << z2 << '\n';
        out_ << '\n';
        out_ << x1 << ' ' << y1 << ' ' << z2 << '\n';
        out_ << x1 << ' ' << y2 << ' ' << z2 << '\n';
        out_ << '\n';
        out_ << x2 << ' ' << y1 << ' ' << z2 << '\n';
        out_ << x2 << ' ' << y2 << ' ' << z2 << '\n';
        out_ << '\n';
          
        out_ << x1 << ' ' << y1 << ' ' << z1 << '\n';
        out_ << x1 << ' ' << y1 << ' ' << z2 << '\n';
        out_ << '\n';
        out_ << x2 << ' ' << y1 << ' ' << z1 << '\n';
        out_ << x2 << ' ' << y1 << ' ' << z2 << '\n';
        out_ << '\n';
        out_ << x1 << ' ' << y2 << ' ' << z1 << '\n';
        out_ << x1 << ' ' << y2 << ' ' << z2 << '\n';
        out_ << '\n';
        out_ << x2 << ' ' << y2 << ' ' << z1 << '\n';
        out_ << x2 << ' ' << y2 << ' ' << z2 << '\n';
        out_ << x2 << ' ' << y2 << ' ' << z2 << '\n';
        out_ << '\n';
        
        out_ << '\n';
        count++;
        // if (count > 10) return;
     }
    }
  }
}

bool ltd_voxelization::has_voxel(const ltd_voxel_id & id_) const
{
  if (id_.i < 0 or id_.i >= _nx_) return false;
  if (id_.j < 0 or id_.j >= _ny_) return false;
  if (id_.k < 0 or id_.k >= _nz_) return false;
  return true;
}

void ltd_voxelization::vicinity(const ltd_voxel_id & id_, std::set<ltd_voxel_id> & vic_) const
{
  vic_.clear();
  DT_THROW_IF(not has_voxel(id_), std::logic_error, "Voxel is not valid!");
  for (int di = -1; di <= 1; di++) {
    for (int dj = -1; dj <= 1; dj++) {
      for (int dk = -1; dk <= 1; dk++) {
        ltd_voxel_id nId;
        nId.i = id_.i + di;
        nId.j = id_.j + dj;
        nId.k = id_.k + dk;
        if (has_voxel(nId)) {
          vic_.insert(nId);
        }
      }
    }
  }
}

ltd_voxel_id ltd_voxelization::find(const geomtools::vector_3d & point_) const
{
  ltd_voxel_id voxId;
  voxId.i = (int) (point_.x() - _xmin_) / _dimension_;
  voxId.j = (int) (point_.y() - _ymin_) / _dimension_;
  voxId.k = (int) (point_.z() - _zmin_) / _dimension_;
  return voxId;
}
 
const ltd_voxel & ltd_voxelization::get_voxel(const ltd_voxel_id & id_) const
{
  return _voxels_[id_.k][id_.j][id_.i];
}

uint32_t ltd_voxelization::get_counter() const
{
  return _counter_;
}

uint32_t ltd_voxelization::get_voxel_counter(const ltd_voxel_id & id_) const
{
  return get_voxel(id_).point_ids.size();
}

void ltd_voxelization::add(const geomtools::vector_3d & point_,
                           uint32_t point_id_)
{
  auto logging = datatools::logger::PRIO_DEBUG;
  // DT_LOG_DEBUG(logging, "point : " << point_ << " with ID = " << point_id_);
  ltd_voxel_id foundId = find(point_);
  // DT_LOG_DEBUG(logging, "found voxel ID = " << foundId);
  if (not has_voxel(foundId)) {
    // DT_LOG_DEBUG(logging, "but voxel ID = " << foundId << " does not exist");
    return;
  }
  ltd_voxel & vox = _voxels_[foundId.k][foundId.j][foundId.i];
  vox.point_ids.insert(point_id_);
  _counter_++;
}

void ltd_zband::reset()
{
  id = -1;
  z  = datatools::invalid_real();
  dz = datatools::invalid_real();
  point_ids.clear();
}

// static
geomtools::vector_3d ltd::origin_uvw()
{
  return geomtools::vector_3d(-9457.0, 873.0, 11925.0);
}

// static
double ltd::xy_tilt_angle()
{
  double dx = 13 * CLHEP::mm;
  double dy = 5050 * CLHEP::mm;
  return std::atan2(dx, dy);
}
 
// static
std::string ltd::block_filename(int block_id_)
{
  std::ostringstream sout;
  char suffix = 0;
  if (block_id_ == 0) suffix = 'G';
  else if (block_id_ == 1) suffix = 'F';
  else if (block_id_ == 2) suffix = 'E';
  else if (block_id_ == 3) suffix = 'D';
  else if (block_id_ == 4) suffix = 'C';
  else if (block_id_ == 5) suffix = 'B';
  else if (block_id_ == 6) suffix = 'A';
  else {
    DT_THROW(std::range_error, "Invalid block ID = " << block_id_ << "!");
  }
  sout << "block" << suffix << ".dat";
  return sout.str();
}

// static
std::tuple<double, double> ltd::strip_limits_y(int strip_id_)
{
  std::unique_ptr<std::map<int, std::tuple<double, double>>> _strip_yranges;
  if (! _strip_yranges) {
    _strip_yranges.reset(new std::map<int, std::tuple<double, double>>());
    *_strip_yranges = {
                       // Block 0
                       { 0 , { -7086.0,  -6956.0} },
                       { 1 , { -7221.0,  -7086.0} },
                       { 2 , { -7357.0,  -7221.0} },
                       // Block 1
                       { 3 , { -7506.0,  -7380.0} },
                       { 4 , { -7643.0,  -7514.0} },
                       { 5 , { -7782.0,  -7650.0} },
                       { 6 , { -7922.0,  -7789.0} },
                       { 7 , { -8054.0,  -7922.0} },
                       { 8 , { -8188.0,  -8064.0} },
                       // Block 2
                       { 9 , { -8345.0, -8220.0} },
                       {10 , { -8484.0, -8350.0} },
                       {11 , { -8622.0, -8486.0} },
                       {12 , { -8760.0, -8625.0} },
                       {13 , { -8896.0, -8760.0} },
                       {14 , { -9025.0, -8898.0} },
                       // Block 3
                       {15 , { -9180.0, -9052.0} },
                       {16 , { -9316.0, -9186.0} },
                       {17 , { -9455.0, -9316.0} },
                       {18 , { -9594.0, -9460.0} },
                       {19 , { -9733.0, -9600.0} },
                       {20 , { -9864.0, -9734.0} },
                       // Block 4
                       {21 , { -10016.0, -9890.0} },
                       {22 , { -10156.0, -10023.0} },
                       {23 , { -10290.0, -10160.0} },
                       {24 , { -10429.0, -10300.0} },
                       {25 , { -10565.0, -10431.0} },
                       {26 , { -10700.0, -10571.0} },
                       // Block 5                      
                       {27 , { -10856.0, -10725.0} },
                       {28 , { -10994.0, -10860.0} },
                       {29 , { -11130.0, -10995.0} },
                       {30 , { -11268.0, -11136.0} },
                       {31 , { -11400.0, -11272.0} },
                       {32 , { -11536.0, -11409.0} },
                       // Block 6
                       {33 , { -11693.0, -11561.0} },
                       {34 , { -11826.0, -11697.0} },
                       {35 , { -11950.0, -11826.5} }
    };
  }

 return _strip_yranges->find(strip_id_)->second;
}

ltd::ltd(const sngeom & geom_, datatools::logger::priority logging_)
{
  logging = logging_;
  _geom_ = &geom_;
}

void ltd::import(const std::string & ltd_filename_, int strip_id_, double density_radius_, bool use_voxelisation_)
{
  _strip_id_ = strip_id_;
  _density_radius_ = density_radius_;
  _points_.clear();
  _weights_.clear();
  _voxelization_.reset();
  _use_voxelization_ = use_voxelisation_; 
  DT_LOG_NOTICE(logging, "Selected strip = " << _strip_id_);
  const strip & selectedStrip = _geom_->get_strips()[_strip_id_];
  DT_LOG_NOTICE(logging, "  Y-min (Fal) = " << selectedStrip.ymin() / CLHEP::mm << " mm");
  DT_LOG_NOTICE(logging, "  Y-max (Fal) = " << selectedStrip.ymax() / CLHEP::mm << " mm");
  std::ifstream fin(ltd_filename_);
  if (!fin) {
    DT_THROW(std::runtime_error, "Cannot open raw laser tracker data file '" << ltd_filename_ << "'!");
  }
  _points_.reserve(2000000);
  int counter = 0;
  int selCounter = 0;
  auto stripLimitsY = strip_limits_y(_strip_id_);
  double uMin = std::get<0>(stripLimitsY);
  double uMax = std::get<1>(stripLimitsY);
  double vMin = 845.0;
  double vMax = 895.0;
  // Special limits for very deformed strips:
  if (_strip_id_ == 3) {
    vMax = 910.0;
  }
  if (_strip_id_ == 8) {
    vMin = 843.0;
  }
  auto uvw0 = origin_uvw();
  double yMin = -(uMax - uvw0.x());
  double yMax = -(uMin - uvw0.x());
  DT_LOG_NOTICE(logging, " Y-min (LTD) = " << yMin / CLHEP::mm << " mm");
  DT_LOG_NOTICE(logging, " Y-max (LTD) = " << yMax / CLHEP::mm << " mm");
  double xMin = -35.0 * CLHEP::mm;
  double xMax =  35.0 * CLHEP::mm;
  double zMin = -1400.0 * CLHEP::mm;
  double zMax =  1400.0 * CLHEP::mm;
  if (_use_voxelization_) {
    DT_LOG_NOTICE(logging, "Instantiating voxelization...");
    _voxelization_.initialize(xMin, xMax, yMin, yMax, zMin, zMax, density_radius_);
    DT_LOG_NOTICE(logging, "Voxelization : ");
    if (datatools::logger::is_notice(logging)) {
      _voxelization_.print(std::cerr);
    }
  }
  while (!fin.eof()) {
    double u, v, w;
    fin >> u >> v >> w >> std::ws;
    if (!fin) {
      DT_THROW(std::runtime_error, "Cannot parse (u,v,w) LTD points!");
    }
    if (u >= uMin and u <= uMax) {
      if (v >= vMin and v <= vMax) {
        counter++;
        geomtools::vector_3d uvw(u, v, w);
        DT_LOG_DEBUG(logging, "Raw (u,v,w) = " << uvw);
        double y_sn = -(u - uvw0.x());
        double x_sn =   v - uvw0.y();
        double z_sn =   w - uvw0.z();
        double thetaTilt = xy_tilt_angle();
        double dx = std::tan(thetaTilt) * y_sn;
        DT_LOG_DEBUG(logging, "thetaTilt =  " << thetaTilt);
        DT_LOG_DEBUG(logging, "x_sn = " << x_sn);
        DT_LOG_DEBUG(logging, "y_sn = " << y_sn);
        DT_LOG_DEBUG(logging, "z_sn = " << z_sn);
        DT_LOG_DEBUG(logging, "dx   = " << dx);
        x_sn += dx;
        DT_LOG_DEBUG(logging, "x_sn = " << x_sn << " (tilted)");
        geomtools::vector_3d p(x_sn, y_sn, z_sn);
        p.setY(y_sn);
        DT_LOG_DEBUG(logging, "p    =  " << p << " (SN frame)");
        p *= CLHEP::millimeter;
        _points_.push_back(p);
        if (_use_voxelization_) _voxelization_.add(p, _points_.size() - 1);
        selCounter++;
      }
    }
    if (fin.eof()) {
      break;
    }
  }
  DT_LOG_NOTICE(logging, "Total number of raw laser tracker points    : " << counter);
  DT_LOG_NOTICE(logging, "Selected number of raw laser tracker points : " << selCounter);
  DT_LOG_NOTICE(logging, "Number of loaded raw laser tracker points   : " << _points_.size());
  if (_use_voxelization_) {
    DT_LOG_NOTICE(logging, "Voxels                                      : " << _voxelization_.get_counter());
    DT_LOG_NOTICE(logging, "Voxelization is done.");
  }
  DT_LOG_NOTICE(logging, "Computing density weights for loaded laser tracking points...");
  double r2 = _density_radius_ * _density_radius_;
  _weights_.assign(_points_.size(), 0.0);

  int count = 0;
  for (int i = 0; i < _points_.size(); i++) {
    const auto & pi = _points_[i];
    if (_use_voxelization_) {
      ltd_voxel_id voxId = _voxelization_.find(pi);
      if (not _voxelization_.has_voxel(voxId)) {
        DT_THROW(std::logic_error, "No voxel associated to LTD point #" << i);
      }
      std::set<ltd_voxel_id> vic;
      // DT_LOG_DEBUG(logging, "Computing vicinity for voxel " << voxId << " ...");
      _voxelization_.vicinity(voxId, vic);
      for (const auto & vicId : vic) {
        const ltd_voxel & vicVox = _voxelization_.get_voxel(vicId);
        // DT_LOG_DEBUG(logging, "Checking " << vicVox.point_ids.size() << " points in voxel " << vicId << " ...");
        for (auto j : vicVox.point_ids) {
          if (j == i) {
            _weights_[i] += 1.0;
            continue;
          }
          const auto & pj = _points_[j];
          if ((pj - pi).mag2()  < r2) {
            _weights_[i] += 1.0;
          }
        }
      }
    } else {
      for (int j = i + 1; j < _points_.size(); j++) {
        const auto & pj = _points_[j];
        if ((pj - pi).mag2()  < r2) {
          _weights_[i] += 1.0;
          _weights_[j] += 1.0;
        }
      }
    }
    count++;
    if ((count % 10000) == 0) {
      DT_LOG_NOTICE(logging, "Weighted points : " << count);
    }
  }
  double sumW = 0.0;
  for (int i = 0; i < _weights_.size(); i++) {
    double wi = _weights_[i];
    _weights_[i] = 1 / wi;
    sumW += _weights_[i];
  }
  for (int i = 0; i < _weights_.size(); i++) {
    _weights_[i] /= sumW;
  }
  DT_LOG_NOTICE(logging, "Density weights array is built.");
}

void ltd::store(std::ostream & out_) const
{
  out_ << _strip_id_ << '\n';
  out_ << _density_radius_ / CLHEP::mm << '\n';
  out_ << _use_voxelization_ << '\n';
  out_ << _points_.size() << '\n';
  for (int i = 0; i < _points_.size(); i++) {
    const auto & p = _points_[i];
    out_ << p.x() / CLHEP::mm << ' ' << p.y() / CLHEP::mm <<  ' ' << p.z() / CLHEP::mm << ' ' << _weights_[i] << '\n';
  }
  if (_use_voxelization_) {
    _voxelization_.store(out_);
  }
}
 
void ltd::reset()
{
  _strip_id_ = -1;
  _density_radius_ = 1.0 * CLHEP::cm;
  _points_.clear();
  _weights_.clear();
  _voxelization_.reset();
  _use_voxelization_ = false;
}

void ltd::load(const std::string & ltd_filename_)
{
  std::ifstream fin(ltd_filename_);
  if (!fin) {
    throw std::runtime_error("Cannot open strip laser tracker data file!");
  }
  reset();
  fin >> _strip_id_ >> std::ws;
  fin >> _density_radius_ >> std::ws;
  _density_radius_ *= CLHEP::mm;
  fin >> _use_voxelization_ >> std::ws;
  uint32_t npoints;
  fin >> npoints >> std::ws;
  _points_.reserve(npoints);
  _weights_.reserve(npoints);
  for (int i = 0; i < npoints; i++) {
    double x, y, z, w;
    fin >> x >> y >> z >> w >> std::ws;
    x *= CLHEP::mm;
    y *= CLHEP::mm;
    z *= CLHEP::mm;
    _points_.push_back(geomtools::vector_3d(x, y, z));
    _weights_.push_back(w);
  }
  if (_use_voxelization_) {
    DT_LOG_NOTICE(logging, "Loading LTC voxelization data...");
    _voxelization_.load(fin);
  }
}

uint32_t ltd::size() const
{
  return _points_.size();
}
  
const geomtools::vector_3d & ltd::get(int i_) const
{
  return _points_[i_];
}
 
double ltd::get_w(int i_) const
{
  return _weights_[i_];
}

void ltd::print(std::ostream & out_) const
{
  out_ << "Laser tracking Data points: " << '\n';
  out_ << "|-- Density radius   : " << _density_radius_ / CLHEP::mm << " mm" << '\n';
  out_ << "|-- Strip ID         : " << _strip_id_ << '\n';
  out_ << "|-- Number of points : " << _points_.size() << '\n';
  out_ << "|-- Density weights  : " << _weights_.size() << '\n';
  out_ << "`-- Use voxelization : " << std::boolalpha << _use_voxelization_ << '\n';
  if (_use_voxelization_) {
    out_ << "    `-- Voxel full point counter : " << _voxelization_.get_counter() << '\n';
  }
}

void ltd::plot(std::ostream & out_) const
{
  out_ << "#@strip_id=" << _strip_id_ << '\n';
  out_ << "#@density_radius=" << _density_radius_ / CLHEP::mm << '\n';
  out_ << "#@n=" << _points_.size() << '\n';
  for (int i = 0; i < _points_.size(); i++) {
    const auto & p = _points_[i];
    out_ << p.x() << ' ' << p.y() <<  ' ' << p.z() << ' ' << _weights_[i] << '\n';
  }
  out_ << '\n';
}

void ltd::extract_zband(double z_, double dz_, ltd_zband & zband_) const
{
  zband_.reset();
  zband_.z = z_;
  zband_.dz = dz_;
  for (uint32_t i = 0; i < _points_.size(); i++) {
    const geomtools::vector_3d & pi = _points_[i];
    if (std::abs(z_ - pi.z()) < dz_) {
      zband_.point_ids.insert(i);
    }
  }
}

void ltd::clean_y_zband(double ymin_, double ymax_, ltd_zband & zband_) const
{
  DT_LOG_NOTICE(logging, "Found " << zband_.point_ids.size() << " LTD points in the z-band #" << zband_.id);
  int removed = 0;
  int counter = 0;
  // https://stackoverflow.com/questions/2874441/deleting-elements-from-stdset-while-iterating
  for (std::set<uint32_t>::iterator it = zband_.point_ids.begin(); it != zband_.point_ids.end(); ) {
    counter++;
    int i = *it;
    const geomtools::vector_3d & pi = _points_[i  ];
    // DT_LOG_NOTICE(logging, "  [" << counter << "] Point #" << i << " : " << pi / CLHEP::mm << " vs [" << ymin_ << ":" << ymax_ << ']');
    if (pi.y() < ymin_ or pi.y() > ymax_) {
      //DT_LOG_NOTICE(logging, "  Removing point #" << i << "...");
      it = zband_.point_ids.erase(it);
      //DT_LOG_NOTICE(logging, "  Done.");
      removed++;
    } else {
      ++it;
    }
  }
  DT_LOG_NOTICE(logging, "  ... removed " << removed << " LTD points from the z-band #" << zband_.id);
}

} // namespace snrs 
