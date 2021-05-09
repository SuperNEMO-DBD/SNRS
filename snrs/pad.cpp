// Ourselves;
#include <snrs/pad.hpp>

// Standard library:
// #include <fstream>

// Boost:
#include <boost/algorithm/string.hpp>

// Bayeux:
#include <bayeux/datatools/utils.h>
#include <bayeux/geomtools/utils.h>

namespace snrs {

  pad::tile_id::tile_id(int32_t side_, int32_t column_, int32_t row_, int32_t part_)
    : _side_(side_) , _column_(column_) , _row_(row_), _part_(part_)
  {
  }

  bool pad::tile_id::is_valid() const
  {
    return _side_ != INVALID_INDEX and _column_ != INVALID_INDEX and _row_ != INVALID_INDEX and _part_ != INVALID_INDEX;
  }

  void pad::tile_id::invalidate()
  {
    _side_   = INVALID_INDEX;
    _column_ = INVALID_INDEX;
    _row_    = INVALID_INDEX;
    _part_   = INVALID_INDEX;
  }

  void pad::tile_id::set(int32_t side_, int32_t column_, int32_t row_, int32_t part_)
  {
    _side_   = side_;
    _column_ = column_;
    _row_    = row_;
    _part_   = part_;
  }

  int32_t pad::tile_id::get_side() const
  {
    return _side_;
  }

  int32_t pad::tile_id::get_column() const
  {
    return _column_;
  }

  int32_t pad::tile_id::get_row() const
  {
    return _row_;
  }

  int32_t pad::tile_id::get_part() const
  {
    return _part_;
  }

  bool pad::tile_id::match(const tile_id & other_) const
  {
    return match(other_.get_side(), other_.get_column(), other_.get_row(), other_.get_part());
  }

  bool pad::tile_id::match(int32_t side_, int32_t column_, int32_t row_, int32_t part_) const
  {
    if (_side_ == INVALID_INDEX) return false;
    if (_column_ == INVALID_INDEX) return false;
    if (_row_ == INVALID_INDEX) return false;
    if (_part_ == INVALID_INDEX) return false;
    if (_side_ == ANY_INDEX) {
      if (side_ == INVALID_INDEX) return false;
    } else{
      if (side_ != _side_) return false;
    }
    if (_column_ == ANY_INDEX) {
      if (column_ == INVALID_INDEX) return false;
    } else {
      if (column_ != _column_) return false;
    }
    if (_row_ == ANY_INDEX) {
      if (row_ == INVALID_INDEX) return false;
    } else {
      if (row_ != _row_) return false;
    }
    if (_part_ == ANY_INDEX) {
      if (_part_ == INVALID_INDEX) return false;
    } else {
      if (part_ != _part_) return false;
    }
    return true;
  }

  std::ostream & operator<<(std::ostream & out_, const pad::tile_id & tid_)
  {
    out_ << '[' << tid_._side_ << ',' << tid_._column_ << ',' << tid_._row_ << ',' << tid_._part_ << ']';
    return out_;
  }

  bool pad::tile_id::from_string(const std::string & repr_)
  {
    std::string repr = boost::trim_copy(repr_);
    // "[a.b.c.d]"
    std::size_t len = repr.size();
    if (len < 9) return false;
    if (repr[0] != '[') return false;
    if (repr[len - 1] != ']') return false;
    std::string intRepr = repr.substr(1, len-2);
    std::vector<std::string> strs;
    boost::split(strs, intRepr,boost::is_any_of(","));
    if (strs.size() != 4) {
      return false;
    }
    int32_t ids[4];
    for (unsigned int i = 0; i < strs.size(); i++) {
      std::string token = strs[i];
      boost::trim(token);
      int32_t id = pad::tile_id::INVALID_INDEX;
      if (token == "*") {
        id = pad::tile_id::ANY_INDEX;
      } else {
        std::istringstream sstoken(token);
        sstoken >> id;
        if (!sstoken) return false;
      }
      ids[i] = id;
    }
    set(ids[0], ids[1], ids[2], ids[3]);
    return true;
  }
 
  bool pad::tile_id::operator==(const tile_id & tid_) const
  {
    if (_side_ == INVALID_INDEX or tid_._side_ == INVALID_INDEX) return false;
    if (_column_ == INVALID_INDEX or tid_._column_ == INVALID_INDEX) return false;
    if (_row_ == INVALID_INDEX or tid_._row_ == INVALID_INDEX) return false;
    if (_part_ == INVALID_INDEX or tid_._part_ == INVALID_INDEX) return false;
    if (_side_   != tid_._side_) return false;
    if (_column_ != tid_._column_) return false;
    if (_row_    != tid_._row_) return false;
    if (_part_   != tid_._part_) return false;
    return true;
  }
  
  bool pad::tile_id::operator<(const tile_id & tid_) const
  {
    if (_side_ == INVALID_INDEX or tid_._side_ == INVALID_INDEX) return false;
    if (_column_ == INVALID_INDEX or tid_._column_ == INVALID_INDEX) return false;
    if (_row_ == INVALID_INDEX or tid_._row_ == INVALID_INDEX) return false;
    if (_part_ == INVALID_INDEX or tid_._part_ == INVALID_INDEX) return false;
    if (_side_ < tid_._side_) return true;
    if (_side_ > tid_._side_) return false;
    if (_column_ < tid_._column_) return true;
    if (_column_ > tid_._column_) return false;
    if (_row_ < tid_._row_) return true;
    if (_row_ > tid_._row_) return false;
    if (_part_ < tid_._part_) return true;
    return false;
  }

  // ================================================================== //
  
  void pad::z_distortion_data_type::reset()
  {
    model = fs::DISTORTION_FLAT;
    elliptic_distortion.reset();
    polynomial_distortion.reset();
  }

  void pad::z_distortion_data_type::store(std::ostream & out_) const
  {
    // out_ << "z_distortion" << '\n';
    out_ << model << ' ';
    if (model == fs::DISTORTION_ELLIPTIC) {
      elliptic_distortion.store(out_);
    } if (model == fs::DISTORTION_POLYNOMIAL) {
      polynomial_distortion.store(out_);
    } 
  }

  void pad::z_distortion_data_type::load(std::istream & in_)
  {
    reset();
    int imodel;
    in_ >> imodel >> std::ws;
    model = static_cast<fs::distortion_model_type>(imodel);
    if (model == fs::DISTORTION_ELLIPTIC) {
      elliptic_distortion.load(in_);
    } if (model == fs::DISTORTION_POLYNOMIAL) {
      polynomial_distortion.load(in_);
    } 
  }

  void pad::distortion_data_type::reset()
  {
    z_distortions.clear();
  }

  void pad::distortion_data_type::store(std::ostream & out_) const
  {
    // out_ << "distortion" << '\n';
    out_ << z_distortions.size() << '\n';
    for (uint32_t kz = 0; kz < z_distortions.size(); kz++) {
      const z_distortion_data_type & zdd = z_distortions[kz];
      zdd.store(out_);
    }    
  }
  
  void pad::distortion_data_type::load(std::istream & in_)
  {
    uint32_t zdistSize = 0;
    in_ >> zdistSize >> std::ws;
    {
      z_distortion_data_type zdd0;
      z_distortions.assign(zdistSize, zdd0);
    }
    for (uint32_t kz = 0; kz < z_distortions.size(); kz++) {
      z_distortion_data_type & zddk = z_distortions[kz];
      zddk.load(in_);
    }
  }

  pad::pad(const std::string & label_,
           int strip_id_,
           int pad_id_,
           double width_,
           double height_,
           double thickness_,
           uint32_t ny_,
           uint32_t nz_,
           const geomtools::vector_3d & original_position_ = geomtools::vector_3d())
  {
    DT_THROW_IF(width_ < 1.0 * CLHEP::mm, std::logic_error,
                "Invalid width!");
    DT_THROW_IF(height_ < 1.0 * CLHEP::mm, std::logic_error,
                "Invalid height!");
    DT_THROW_IF(thickness_ < 0.0 * CLHEP::micrometer, std::logic_error,
                "Invalid thickness!");
    DT_THROW_IF(nz_ < 2, std::logic_error, "Invalid nz!");
    DT_THROW_IF(ny_ < 2, std::logic_error, "Invalid ny!");
    DT_THROW_IF(ny_ % 2 != 0, std::logic_error, "ny must be even!");
    _label_ = label_;
    _strip_id_ = strip_id_;
    _pad_id_ = pad_id_;
    _width_ = width_;
    _height_ = height_;
    _thickness_ = thickness_;
    _ny_ = ny_;
    _nz_ = nz_;
    _original_position_ = original_position_;
  }

  const std::string & pad::get_label() const
  {
    return _label_;
  }

  int pad::get_strip_id() const
  {
    return _strip_id_;
  }

  int pad::get_pad_id() const
  {
    return _pad_id_;
  }

  double pad::get_width() const
  {
    return _width_;
  }

  double pad::get_height() const
  {
    return _height_;
  }

  double pad::get_thickness() const
  {
    return _thickness_;
  }

  double pad::get_surface() const
  {
    return _width_ * _height_;
  }

  double pad::get_volume() const
  {
    return _width_ * _height_ * _thickness_;
  }

  uint32_t pad::get_ny() const
  {
    return _ny_;
  }

  uint32_t pad::get_nz() const
  {
    return _nz_;
  }

  double pad::get_dy() const
  {
    return _dy_;
  }

  double pad::get_dz() const
  {
    return _dz_;
  }

  void pad::set_material(const std::string & material_)
  {
    _material_ = material_;
    return;
  }
  
  const std::string & pad::get_material() const
  {
    return _material_;
  }

  void pad::set_film_material(const std::string & film_material_)
  {
    _film_material_ = film_material_;
    return;
  }
  
  const std::string & pad::get_film_material() const
  {
    return _film_material_;
  }

  bool pad::is_valid(const vertex3d_id & vid_) const
  {
    if (vid_.get_i() > FACE_FRONT) return false;
    if (vid_.get_j() > _ny_) return false;
    if (vid_.get_k() > _nz_) return false;
    return true;
  }

  const std::vector<std::vector<vertex3d>> & pad::get_vertexes(uint32_t face_) const
  {
    DT_THROW_IF(face_ > FACE_FRONT, std::range_error, "Invalid face index!");
    return _vertexes_[face_];
  }

  const std::vector<std::vector<vertex3d>> & pad::get_back_film_vertexes(uint32_t face_) const
  {
    DT_THROW_IF(not has_film(), std::range_error, "This pad has no back film!");
    DT_THROW_IF(face_ > FACE_FRONT, std::range_error, "Invalid face index!");
    return _back_film_vertexes_[face_];
  }

  const std::vector<std::vector<vertex3d>> & pad::get_front_film_vertexes(uint32_t face_) const
  {
    DT_THROW_IF(not has_film(), std::range_error, "This pad has no front film!");
    DT_THROW_IF(face_ > FACE_FRONT, std::range_error, "Invalid face index!");
    return _front_film_vertexes_[face_];
  }

  const vertex3d & pad::get_vertex(const vertex3d_id & vid_) const
  {
    return _vertexes_[vid_.get_i()][vid_.get_j()][vid_.get_k()];
  }

  vertex3d & pad::grab_vertex(const vertex3d_id & vid_)
  {
    return _vertexes_[vid_.get_i()][vid_.get_j()][vid_.get_k()];
  }

  const vertex3d & pad::operator[](const vertex3d_id & vid_) const
  {
    return _vertexes_[vid_.get_i()][vid_.get_j()][vid_.get_k()];
  }

  vertex3d & pad::operator[](const vertex3d_id & vid_)
  {
    return _vertexes_[vid_.get_i()][vid_.get_j()][vid_.get_k()];
  }

  const vertex3d & pad::get_back_film_vertex(const vertex3d_id & vid_) const
  {
    DT_THROW_IF(not has_film(), std::range_error, "This pad has no front film!");
    return _back_film_vertexes_[vid_.get_i()][vid_.get_j()][vid_.get_k()];
  }

  vertex3d & pad::grab_back_film_vertex(const vertex3d_id & vid_)
  {
    DT_THROW_IF(not has_film(), std::range_error, "This pad has no front film!");
    return _back_film_vertexes_[vid_.get_i()][vid_.get_j()][vid_.get_k()];
  }

  const vertex3d & pad::get_front_film_vertex(const vertex3d_id & vid_) const
  {
    DT_THROW_IF(not has_film(), std::range_error, "This pad has no front film!");
    return _front_film_vertexes_[vid_.get_i()][vid_.get_j()][vid_.get_k()];
  }

  vertex3d & pad::grab_front_film_vertex(const vertex3d_id & vid_)
  {
    DT_THROW_IF(not has_film(), std::range_error, "This pad has no front film!");
    return _front_film_vertexes_[vid_.get_i()][vid_.get_j()][vid_.get_k()];
  }

  double pad::distance(const pad & other_) const
  {
    if (_width_ != other_._width_ or _height_ != other_._height_) {
      DT_THROW(std::logic_error, "Dimensions do not match!");
    }
    if (_ny_ != other_._ny_ or _nz_ != other_._nz_) {
      DT_THROW(std::logic_error, "Meshes do not match!");
    }
    double d = 0.0;
    std::vector<std::vector<vertex3d>> * vertexes;
    for (int i = FACE_BACK; i <= FACE_FRONT; i++) {
      for (int j = 0; j <= _ny_; j++) {
        for (int k = 0; k <= _nz_; k++) {
          const vertex3d & v1 = _vertexes_[i][j][k];
          const vertex3d & v2 = other_._vertexes_[i][j][k];
          d += (v2.pos - v1.pos).mag();
        }
      }
    }
    return d;
  }

  double pad::distance(const vertex3d_id & vid1_, const vertex3d_id & vid2_) const
  {
    const vertex3d & v1 = _vertexes_[vid1_.get_i()][vid1_.get_j()][vid1_.get_k()];
    const vertex3d & v2 = _vertexes_[vid1_.get_i()][vid2_.get_j()][vid2_.get_k()];
    return (v2.pos - v1.pos).mag();
  }

  bool pad::has_distortion() const
  {
    return _distortion_.z_distortions.size() > 2;
  }

  bool pad::has_film() const
  {
    return datatools::is_valid(_film_thickness_);
  }

  double pad::get_film_thickness() const
  {
    return _film_thickness_;
  }

  void pad::set_film_thickness(double ft_)
  {
    _film_thickness_ = ft_;
    return;
  }
 
  const pad::distortion_data_type & pad::get_distortion() const
  {
    return _distortion_;
  }

  pad::distortion_data_type & pad::grab_distortion()
  {
    return _distortion_;
  }

  const geomtools::vector_3d & pad::get_original_position() const
  {
    return _original_position_;
  }

  void pad::clear_vertexes()
  {
    for (int i = FACE_BACK; i <= FACE_FRONT; i++) {
      _vertexes_[i].clear();
      _back_film_vertexes_[i].clear();
      _front_film_vertexes_[i].clear();
    }
  }

  void pad::make_vertexes()
  {
    _dy_ = _width_ / _ny_;
    _dz_ = _height_ / _nz_;
    double ymin = _original_position_.y() - 0.5 * _width_;
    double ymax = _original_position_.y() + 0.5 * _width_;
    double zmin = _original_position_.z() - 0.5 * _height_;
    double zmax = _original_position_.z() + 0.5 * _height_;
    // double ymin = - 0.5 * _width_;
    // double ymax = + 0.5 * _width_;
    // double zmin = - 0.5 * _height_;
    // double zmax = + 0.5 * _height_;
    for (int i = FACE_BACK; i <= FACE_FRONT; i++) {
      {
        vertex3d vtx0;
        std::vector<vertex3d> zcol0;
        zcol0.assign(_nz_ + 1, vtx0);
        _vertexes_[i].assign(_ny_ + 1, zcol0);
      }
      double xi = -0.5 * _thickness_ + i * _thickness_;
      for(uint32_t j = 0; j <= _ny_; j++) {
        double yj = ymin + j * _dy_;
        for(uint32_t k = 0; k <= _nz_; k++) {
          double zk = zmin + k * _dz_;
          vertex3d & vjk = _vertexes_[i][j][k];
          vjk.pos.set(xi , yj, zk);
          if (i != FACE_BACK) {
            vjk.back_id.set(i-1, j, k);
          }
          if (i != FACE_FRONT) {
            vjk.front_id.set(i+1, j, k);
          }
          if (j != 0) {
            vjk.left_id.set(i, j-1, k);
          }
          if (j != _ny_) {
            vjk.right_id.set(i, j+1, k);
          }
          if (k != 0) {
            vjk.bottom_id.set(i, j, k-1);
          }
          if (k != _nz_) {
            vjk.top_id.set(i, j, k+1);
          }
        }
      }
    }
    if (has_film()) {
      // Back film:
      for (int i = FACE_BACK; i <= FACE_FRONT; i++) {
        {
          vertex3d vtx0;
          std::vector<vertex3d> zcol0;
          zcol0.assign(_nz_ + 1, vtx0);
          _back_film_vertexes_[i].assign(_ny_ + 1, zcol0);
        }
        double xi = -0.5 * _thickness_ - _film_thickness_ + i * _film_thickness_;
        for(uint32_t j = 0; j <= _ny_; j++) {
          double yj = ymin + j * _dy_;
          for(uint32_t k = 0; k <= _nz_; k++) {
            double zk = zmin + k * _dz_;
            vertex3d & vjk = _back_film_vertexes_[i][j][k];
            vjk.pos.set(xi , yj, zk);
            if (i != FACE_BACK) {
              vjk.back_id.set(i-1, j, k);
            }
            if (i != FACE_FRONT) {
              vjk.front_id.set(i+1, j, k);
            }
            if (j != 0) {
              vjk.left_id.set(i, j-1, k);
            }
            if (j != _ny_) {
              vjk.right_id.set(i, j+1, k);
            }
            if (k != 0) {
              vjk.bottom_id.set(i, j, k-1);
            }
            if (k != _nz_) {
              vjk.top_id.set(i, j, k+1);
            }
          }
        }
      }
      // Front film:
      for (int i = FACE_BACK; i <= FACE_FRONT; i++) {
        {
          vertex3d vtx0;
          std::vector<vertex3d> zcol0;
          zcol0.assign(_nz_ + 1, vtx0);
          _front_film_vertexes_[i].assign(_ny_ + 1, zcol0);
        }
        double xi = +0.5 * _thickness_ + i * _film_thickness_;
        for(uint32_t j = 0; j <= _ny_; j++) {
          double yj = ymin + j * _dy_;
          for(uint32_t k = 0; k <= _nz_; k++) {
            double zk = zmin + k * _dz_;
            vertex3d & vjk = _front_film_vertexes_[i][j][k];
            vjk.pos.set(xi , yj, zk);
            if (i != FACE_BACK) {
              vjk.back_id.set(i-1, j, k);
            }
            if (i != FACE_FRONT) {
              vjk.front_id.set(i+1, j, k);
            }
            if (j != 0) {
              vjk.left_id.set(i, j-1, k);
            }
            if (j != _ny_) {
              vjk.right_id.set(i, j+1, k);
            }
            if (k != 0) {
              vjk.bottom_id.set(i, j, k-1);
            }
            if (k != _nz_) {
              vjk.top_id.set(i, j, k+1);
            }
          }
        }
      }
      
    }
    return;
  }

  void pad::print(std::ostream & out_) const
  {
    out_ << "|-- Label      = " << _label_ << '\n';
    out_ << "|-- Strip ID   = " << _strip_id_ << '\n';
    out_ << "|-- Pad ID     = " << _pad_id_ << '\n';
    out_ << "|-- Width      = " << _width_ / CLHEP::mm << " mm" << '\n';
    out_ << "|-- Height     = " << _height_ / CLHEP::mm << " mm" << '\n';
    out_ << "|-- Thickness  = " << _thickness_ / CLHEP::mm << " mm" << '\n';
    out_ << "|-- Ny         = " << _ny_ << '\n';
    out_ << "|-- Nz         = " << _nz_ << '\n';
    out_ << "|-- Dy         = " << _dy_ / CLHEP::mm << " mm" << '\n';
    out_ << "|-- Dz         = " << _dz_ / CLHEP::mm << " mm" << '\n';
    out_ << "|-- Material   = " << _material_ << '\n';
    out_ << "|-- Film       = " << std::boolalpha << has_film()<< '\n';
    if (has_film()) {
      out_ << "|   |-- Film Thickness = " << _film_thickness_ / CLHEP::micrometer << " um" << '\n';
      out_ << "|   `-- Film material  = " << _film_material_ << '\n';
    }
    out_ << "|-- Distortion = " << std::boolalpha <<
      (has_distortion()? (std::to_string(_distortion_.z_distortions.size()) + " z-fits") : "none") << '\n';
    out_ << "|-- Vertexes = 2 planes of " << _vertexes_[FACE_BACK].size() << " Z-columns of " << (_nz_+1) << " nodes" << '\n';
    for(uint32_t i = FACE_BACK; i < FACE_FRONT; i++) {
      for(uint32_t j = 0; j <= _ny_; j++) {
        for(uint32_t k = 0; k <= _nz_; k++) {
          const vertex3d & v = _vertexes_[i][j][k];
          int printCode = 0;
          if (((j < 2) or (j > _ny_ - 2)) and ((k < 2) or (k > _nz_ - 2))) {
            printCode = 1;
          } else if (k==2) {
            printCode = 2;
          }
          if (printCode == 1) {
            out_ << "|   " << j << ' ' << k << ' '
                 << v.pos.x() << ' ' << v.pos.y() << ' ' << v.pos.z() << ' '
                 << '\n';
          }
          if (printCode == 2) {
            out_ << "    ...\n";
          }
        }
        // out_ << '\n';
      }
    }
    out_ << "|-- Back film vertexes = 2 planes of " << _back_film_vertexes_[FACE_BACK].size() << " Z-columns of " << (_nz_+1) << " nodes" << '\n';
    out_ << "`-- Front film vertexes = 2 planes of " << _front_film_vertexes_[FACE_BACK].size() << " Z-columns of " << (_nz_+1) << " nodes" << '\n';
  }

  void pad::store(std::ostream & out_) const
  {
    out_ << _label_ << '\n';
    out_ << _strip_id_ << '\n';
    out_ << _pad_id_ << '\n';
    out_ << _width_ / CLHEP::mm << '\n';
    out_ << _height_ / CLHEP::mm << '\n';
    out_ << _thickness_ / CLHEP::mm << '\n';
    out_ << _ny_ << '\n';
    out_ << _nz_ << '\n';
    out_ << _dy_ / CLHEP::mm << '\n';
    out_ << _dz_ / CLHEP::mm << '\n';
    out_ << _material_ << '\n';
    out_ << has_film() << '\n';
    if (has_film()) {
      out_ << _film_thickness_ / CLHEP::mm << '\n';
      out_ << _film_material_ << '\n';
    }
    _distortion_.store(out_);
    for(uint32_t i = FACE_BACK; i <= FACE_FRONT; i++) {
      for(uint32_t j = 0; j <= _ny_; j++) {
        for(uint32_t k = 0; k <= _nz_; k++) {
          const vertex3d & v = _vertexes_[i][j][k];
          v.store(out_);
        }
        out_ << '\n';
      }
      out_ << '\n';
    }
  }

  void pad::load(std::istream & in_)
  {
    in_ >> _label_ >> std::ws;
    in_ >> _strip_id_ >> std::ws;
    in_ >> _pad_id_ >> std::ws;
    in_ >> _width_ >> std::ws;
    in_ >> _height_ >> std::ws;
    in_ >> _thickness_ >> std::ws;
    _width_ *= CLHEP::mm;
    _height_ *= CLHEP::mm;
    _thickness_ *= CLHEP::mm;
    in_ >> _ny_ >> std::ws;
    in_ >> _nz_ >> std::ws;
    in_ >> _dy_ >> std::ws;
    in_ >> _dz_ >> std::ws;
    _dy_ *= CLHEP::mm;
    _dz_ *= CLHEP::mm;
    in_ >> _material_ >> std::ws;
    bool with_film = false;
    in_ >> with_film >> std::ws;
    if (with_film) {
      in_ >> _film_thickness_ >> std::ws;
      _film_thickness_ *= CLHEP::mm;
      in_ >> _film_material_ >> std::ws;
    }
    _distortion_.load(in_);
    for(uint32_t i = FACE_BACK; i <= FACE_FRONT; i++) {
      _vertexes_[i].clear();
      {
        vertex3d vtx0;
        std::vector<vertex3d> zcol0;
        zcol0.assign(_nz_ + 1, vtx0);
        _vertexes_[i].assign(_ny_ + 1, zcol0);
      }
      for(uint32_t j = 0; j <= _ny_; j++) {
        for(uint32_t k = 0; k <= _nz_; k++) {
          double x, y, z;
          vertex3d & v = _vertexes_[i][j][k];
          v.load(in_);
        }
      }
    }
  }

  void pad::plot(std::ostream & out_) const
  {
    out_ << "#@label=" << _label_ << '\n';
    out_ << "#@strip-id=" << _strip_id_ << '\n';
    out_ << "#@pad-id=" << _pad_id_ << '\n';
    out_ << "#@width=" << _width_ / CLHEP::mm << '\n';
    out_ << "#@height=" << _height_ / CLHEP::mm << '\n';
    out_ << "#@thickness=" << _thickness_ / CLHEP::mm << '\n';
    out_ << "#@ny=" << _ny_ << '\n';
    out_ << "#@nz=" << _nz_ << '\n';
    out_ << "#@dy=" << _dy_ / CLHEP::mm << '\n';
    out_ << "#@dz=" << _dz_ / CLHEP::mm << '\n';
    out_ << "#@material=" << _material_ << '\n';
    out_ << "#@has_film=" << has_film() << '\n';
    out_ << "#@film_thickness=";
    if (has_film()) {
      out_ << _film_thickness_ / CLHEP::mm;
    } else {
      out_ << -1;
    }
    out_ << "#@film_material=";
    if (has_film()) {
      out_ << _film_material_;
    } 
    out_ << '\n';
    out_ << "#@distortion.size=" << _distortion_.z_distortions.size() << '\n';
    for(uint32_t i = FACE_BACK; i <= FACE_FRONT; i++) {
      for(uint32_t j = 0; j <= _ny_; j++) {
        for(uint32_t k = 0; k <= _nz_; k++) {
          const vertex3d & v = _vertexes_[i][j][k];
          out_ << i << ' ' << j << ' ' << k << ' '
               << v.pos.x() << ' ' << v.pos.y() << ' ' << v.pos.z() << ' '
               << '\n';
        }
        out_ << '\n';
      }
      out_ << '\n';
    }
    return;
  }
  
  const pad::mesh_data_type & pad::get_source_mesh() const
  {
    return _source_mesh_;
  }
  
  const pad::mesh_data_type & pad::get_back_film_mesh() const
  {
    return _back_film_mesh_;
  }
  
  const pad::mesh_data_type & pad::get_front_film_mesh() const
  {
    return _front_film_mesh_;
  }
  
  pad::mesh_data_type & pad::grab_source_mesh()
  {
    return _source_mesh_;
  }
  
  pad::mesh_data_type & pad::grab_back_film_mesh()
  {
    return _back_film_mesh_;
  }
  
  pad::mesh_data_type & pad::grab_front_film_mesh()
  {
    return _front_film_mesh_;
  }

  void pad::mesh_data_type::reset()
  {
    solid.reset();
    tile_map.clear();
    reverse_tile_map.clear();
    return;
  }
  
  void pad::mesh_data_type::store_tile_maps(std::ostream & out_) const
  {
    out_ << tile_map.size() << ' ' << reverse_tile_map.size() << '\n';
    for (const auto & entry : tile_map) {
      out_ << entry.first.get_side() << ' '
           << entry.first.get_column() << ' '
           << entry.first.get_row() << ' '
           << entry.first.get_part() << ' '
           << entry.second << '\n';
    }
  }
  
  void pad::mesh_data_type::load_tile_maps(std::istream & in_)
  {
    tile_map.clear();
    reverse_tile_map.clear();
    int tmsz = 0;
    int rtmsz = 0;
    in_ >> tmsz >> rtmsz >> std::ws;
    if (tmsz < 1 or tmsz != rtmsz) {
      DT_THROW(std::runtime_error, "Corrupted tile maps!");
    }
    for (int i = 0; i < tmsz; i++) {
      int s, c, r, p, k;
      in_ >> s >> c >> r >> p >> k >> std::ws;
      if (!in_) {
        DT_THROW(std::runtime_error, "Invalid tilemap format!");
      }
      tile_map[pad::tile_id(s, c, r, p)] = k;
      reverse_tile_map[k] = pad::tile_id(s, c, r, p);
    }
  }
 
} // namespace snrs 
