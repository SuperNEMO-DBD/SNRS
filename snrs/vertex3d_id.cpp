// This project:
#include <snrs/vertex3d_id.hpp>

namespace snrs {
  
  vertex3d_id::vertex3d_id(uint32_t i_, uint32_t j_, uint32_t k_)
    : _i_(i_) , _j_(j_), _k_(k_)
  {
  }

  bool vertex3d_id::is_valid() const
  {
    return _i_ != INVALID_INDEX and _j_ != INVALID_INDEX and _k_ != INVALID_INDEX;
  }

  void vertex3d_id::invalidate()
  {
    _i_ = INVALID_INDEX;
    _j_ = INVALID_INDEX;
    _k_ = INVALID_INDEX;
  }

  void vertex3d_id::set(uint32_t i_, uint32_t j_, uint32_t k_)
  {
    _i_ = i_;
    _j_ = j_;
    _k_ = k_;
  }

  uint32_t vertex3d_id::get_i() const
  {
    return _i_;
  }

  uint32_t vertex3d_id::get_j() const
  {
    return _j_;
  }

  uint32_t vertex3d_id::get_k() const
  {
    return _k_;
  }

  bool vertex3d_id::match(uint32_t i_, uint32_t j_, uint32_t k_) const
  {
    if (_i_ == INVALID_INDEX) return false;
    if (_j_ == INVALID_INDEX) return false;
    if (_k_ == INVALID_INDEX) return false;
    if (_i_ == ANY_INDEX) {
      if (i_ == INVALID_INDEX) return false;
    } else {
      if (i_ != _i_) return false;
    }
    if (_j_ == ANY_INDEX) {
      if (j_ == INVALID_INDEX) return false;
    } else {
      if (j_ != _j_) return false;
    }
    if (_k_ == ANY_INDEX) {
      if (_k_ == INVALID_INDEX) return false;
    } else {
      if (k_ != _k_) return false;
    }
    return true;
  }

  std::ostream & operator<<(std::ostream & out_, const vertex3d_id & vid_)
  {
    out_ << '[' << vid_._i_ << ',' << vid_._j_ << ',' << vid_._k_ << ']';
    return out_;
  }
 
  bool vertex3d_id::operator==(const vertex3d_id & vid_) const
  {
    if (_i_ == INVALID_INDEX or vid_._i_ == INVALID_INDEX) return false;
    if (_j_ == INVALID_INDEX or vid_._j_ == INVALID_INDEX) return false;
    if (_k_ == INVALID_INDEX or vid_._k_ == INVALID_INDEX) return false;
    if (_i_ != vid_._i_) return false;
    if (_j_ != vid_._j_) return false;
    if (_k_ != vid_._k_) return false;
    return true;
  }
  
  bool vertex3d_id::operator<(const vertex3d_id & vid_) const
  {
    if (_i_ == INVALID_INDEX or vid_._i_ == INVALID_INDEX) return false;
    if (_j_ == INVALID_INDEX or vid_._j_ == INVALID_INDEX) return false;
    if (_k_ == INVALID_INDEX or vid_._k_ == INVALID_INDEX) return false;
    if (_i_ < vid_._i_) return true;
    if (_i_ > vid_._i_) return false;
    if (_j_ < vid_._j_) return true;
    if (_j_ > vid_._j_) return false;
    if (_k_ < vid_._k_) return true;
    return false;
  }

  void vertex3d_id::store(std::ostream & out_) const
  {
    // out_ << "vertex3d_id" << ' ';
    out_ << _i_ << ' ' << _j_ << ' ' << _k_;
  }

  void vertex3d_id::load(std::istream & in_)
  {
    in_ >> _i_ >> _j_ >> _k_ >> std::ws;
  }

} // namespace snrs 
