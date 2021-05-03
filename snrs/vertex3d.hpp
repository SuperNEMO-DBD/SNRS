#ifndef SNRS_VERTEX3D_HPP
#define SNRS_VERTEX3D_HPP

// Standard Library:
#include <iostream>

// Bayeux:
#include <bayeux/geomtools/utils.h>

// This project:
#include <vertex3d_id.hpp>

namespace snrs {
  
  struct vertex3d
  {
    // Neighbours:
    vertex3d_id back_id;   ///< -X
    vertex3d_id front_id;  ///< +X
    vertex3d_id left_id;   ///< -Y
    vertex3d_id right_id;  ///< +Y
    vertex3d_id bottom_id; ///< -Z
    vertex3d_id top_id;    ///< +Z

    // Positioning:
    geomtools::vector_3d pos;

    vertex3d();
    
    friend std::ostream & operator<<(std::ostream & out_, const vertex3d & v_);

    void store(std::ostream & out_) const;

    void load(std::istream & in_);  
 
    void reset();
 
  };

} // namespace snrs

#endif // SNRS_VERTEX3D_HPP
