#ifndef SNRS_PAD_TESSELLATION_HPP
#define SNRS_PAD_TESSELLATION_HPP

// Standard Library:
#include <vector>

// Bayeux:
#include <bayeux/datatools/utils.h>
#include <bayeux/datatools/clhep_units.h>
#include <bayeux/geomtools/tessellation.h>

// This project:
#include <snrs/pad.hpp>

namespace snrs {

  class pad_tessellator
  {
  public:

    enum target_type
      {
       PAD_TARGET_SOURCE = 0,
       PAD_TARGET_BACK_FILM = 1,
       PAD_TARGET_FRONT_FILM = 2
      };

    pad_tessellator(const pad & pad_, const geomtools::vector_3d & origin_);

    uint32_t number_of_vertexes() const;

    int32_t compute_vertex_index(const vertex3d_id & vid_) const;
    
    vertex3d_id compute_vertex_id(int32_t vindex_) const;
   
    void make_tessellated_solid(pad::mesh_data_type & mesh_,
                                target_type target_ = PAD_TARGET_SOURCE) const;

  private:
    
    const pad & _pad_;
    geomtools::vector_3d _origin_;
    
  };
  
} // namespace snrs 
  
#endif // SNRS_PAD_TESSELLATION_HPP
