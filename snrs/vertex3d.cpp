// This project:
#include <snrs/vertex3d.hpp>

// Bayeux:
#include <bayeux/datatools/utils.h>
#include <bayeux/datatools/clhep_units.h>

namespace snrs {
  
  vertex3d::vertex3d()
  {
    reset();
  }

  void vertex3d::reset()
  {
    back_id.invalidate();
    front_id.invalidate();
    left_id.invalidate();
    right_id.invalidate();
    bottom_id.invalidate();
    top_id.invalidate();
    pos.set(datatools::invalid_real(),
            datatools::invalid_real(),
            datatools::invalid_real());
  }
  
  std::ostream & operator<<(std::ostream & out_, const vertex3d & v_)
  {
    out_ << '(' << v_.pos.x() << ',' << v_.pos.y() << ',' << v_.pos.z() << ')';
    return out_;
  }

  void vertex3d::store(std::ostream & out_) const
  {
    // out_ << "vertex3d::store" << ' ';
    back_id.store(out_);
    out_ << '\n';
    front_id.store(out_);
    out_ << '\n';
    left_id.store(out_);
    out_ << '\n';
    right_id.store(out_);
    out_ << '\n';
    bottom_id.store(out_);
    out_ << '\n';
    top_id.store(out_);
    out_ << '\n';
    out_ << pos.x() / CLHEP::mm << ' ' << pos.y() / CLHEP::mm << ' ' << pos.z() / CLHEP::mm << '\n';
  }

  void vertex3d::load(std::istream & in_)
  {
    back_id.load(in_);
    front_id.load(in_);
    left_id.load(in_);
    right_id.load(in_);
    bottom_id.load(in_);
    top_id.load(in_);
    double x, y, z;
    in_ >> x >> y >> z >> std::ws;
    pos.set(x * CLHEP::mm, y * CLHEP::mm, z * CLHEP::mm);
  }

} // namespace snrs
