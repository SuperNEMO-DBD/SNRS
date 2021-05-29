// Ourselves;
#include <snrs/pad_tessellation.hpp>

// Standard library:
// #include <fstream>

// Bayeux:
#include <bayeux/datatools/exception.h>

namespace snrs {

  pad_tessellator::pad_tessellator(const pad & pad_, const geomtools::vector_3d & origin_)
    : _pad_(pad_)
    , _origin_(origin_)
  {
  }
  
  uint32_t pad_tessellator::number_of_vertexes() const
  {
    uint32_t mx = 2; 
    uint32_t my = _pad_.get_ny() + 1; 
    uint32_t mz = _pad_.get_nz() + 1;
    return mx * my * mz;
  }

  int32_t pad_tessellator::compute_vertex_index(const vertex3d_id & vid_) const
  {
    DT_THROW_IF(not _pad_.is_valid(vid_),
                std::range_error,
                "Invalid vertex ID!");
    uint32_t mx = 2; 
    uint32_t my = _pad_.get_ny() + 1; 
    uint32_t mz = _pad_.get_nz() + 1; 
    int32_t vindex = my * mz * vid_.get_i() + mz * vid_.get_j() + vid_.get_k();
    return vindex;
  }

  vertex3d_id pad_tessellator::compute_vertex_id(int32_t vindex_) const
  {
    DT_THROW_IF(vindex_ < 0 or vindex_ >= number_of_vertexes(),
                std::range_error,
                "Invalid vertex index!");
    uint32_t my = _pad_.get_ny() + 1; 
    uint32_t mz = _pad_.get_nz() + 1; 
    uint32_t k = vindex_ % mz;
    uint32_t vij = (vindex_ - k ) / mz;
    uint32_t j = vij % my;
    uint32_t i = vij / my;
    return vertex3d_id(i, j, k);    
  }

  void pad_tessellator::make_tessellated_solid(pad::mesh_data_type & mesh_,
                                               target_type target_) const
  {
    datatools::logger::priority logging = datatools::logger::PRIO_NOTICE;
    DT_THROW_IF(not _pad_.has_film() && target_ != PAD_TARGET_SOURCE,
                std::logic_error,
                "Cannot make a tessellated film because this pad has no film!");
    mesh_.reset();
    double yref = _pad_.get_original_position().y();
    uint32_t nx = 2; 
    uint32_t ny = _pad_.get_ny(); 
    uint32_t nz = _pad_.get_nz();
    DT_LOG_DEBUG(logging, "yref = " << yref / CLHEP::mm << " mm");
    DT_LOG_DEBUG(logging, "Ny = " << ny);
    DT_LOG_DEBUG(logging, "Nz = " << nz);
    for (int i = pad::FACE_BACK; i <= pad::FACE_FRONT; i++) {
      for(uint32_t j = 0; j <= ny; j++) {
        for(uint32_t k = 0; k <= nz; k++) {
          vertex3d_id vid(i, j, k);
          uint32_t vindex = compute_vertex_index(vid);
          const vertex3d * vijk = nullptr;
          if (target_ == PAD_TARGET_SOURCE) {
            vijk = &_pad_.get_vertex(vid);
          } else if (target_ == PAD_TARGET_BACK_FILM) {
            vijk = &_pad_.get_back_film_vertex(vid);
          } else if (target_ == PAD_TARGET_FRONT_FILM) {
            vijk = &_pad_.get_front_film_vertex(vid);
          }
          DT_LOG_DEBUG(logging, "======>  vijk.pos = " << vijk->pos  / CLHEP::mm << " mm");
          geomtools::vector_3d vpos(vijk->pos);
          DT_LOG_DEBUG(logging, "======>  vpos = " << vpos / CLHEP::mm << " mm");
          mesh_.solid.add_vertex(vindex, vpos.x(), vpos.y(), vpos.z());
        }
      }
    }
    int32_t facet_idx = 0;
    for(uint32_t j = 0; j < ny; j++) {
      for(uint32_t k = 0; k < nz; k++) {
        // Back vertexes:
        vertex3d_id bAid(pad::FACE_BACK,  j,   k);
        vertex3d_id bBid(pad::FACE_BACK,  j+1, k);
        vertex3d_id bCid(pad::FACE_BACK,  j+1, k+1);
        vertex3d_id bDid(pad::FACE_BACK,  j,   k+1);
        // Front vertexes:
        vertex3d_id fAid(pad::FACE_FRONT, j,   k);
        vertex3d_id fBid(pad::FACE_FRONT, j+1, k);
        vertex3d_id fCid(pad::FACE_FRONT, j+1, k+1);
        vertex3d_id fDid(pad::FACE_FRONT, j,   k+1);
        int32_t bAidx = compute_vertex_index(bAid);
        int32_t bBidx = compute_vertex_index(bBid);
        int32_t bCidx = compute_vertex_index(bCid);
        int32_t bDidx = compute_vertex_index(bDid);
        int32_t fAidx = compute_vertex_index(fAid);
        int32_t fBidx = compute_vertex_index(fBid);
        int32_t fCidx = compute_vertex_index(fCid);
        int32_t fDidx = compute_vertex_index(fDid);
        // yz facets (mapped):
        // Back facet:
        /* 
         *     Db          Cb
         *      +----------+
         *      |Ab>Cb>Db /|
         *      |        / |
         *      | Up    /  |
         *      | Tile /   |
         *      |     /    |
         *      |    /     |
         *      |   / Down |
         *      |  /  Tile |
         *      | /        |
         *      |/ Ab>Bb>Cb|
         *      +----------+
         *     Ab          Bb
         */
        mesh_.solid.add_facet3(facet_idx, bAidx, bCidx, bBidx);
        pad::tile_id tid_bd(0, j, k, 0);
        if (mesh_.tile_map.count(tid_bd)) {
          DT_LOG_WARNING(logging, "Back-down tile ID=" << tid_bd << " is already registered in the tile map!")
        }
        mesh_.tile_map[tid_bd] = facet_idx;
        mesh_.reverse_tile_map[facet_idx] = tid_bd;
        facet_idx++;
        
        mesh_.solid.add_facet3(facet_idx, bAidx, bDidx, bCidx);
        pad::tile_id tid_bu(0, j, k, 1);
        if (mesh_.tile_map.count(tid_bu)) {
          DT_LOG_WARNING(logging, "Back-up tile ID=" << tid_bu << " is already registered in the tile map!")
        }
        mesh_.tile_map[tid_bu] = facet_idx;
        mesh_.reverse_tile_map[facet_idx] = tid_bu;
        facet_idx++;
        
        // Front facet:
        /*
         *     Df          Cf
         *      +----------+
         *      |Af>Cf>Df /|
         *      |        / |
         *      | Up    /  |
         *      | Tile /   |
         *      |     /    |
         *      |    /     |
         *      |   / Down |
         *      |  /  Tile |
         *      | /        |
         *      |/ Af>Bf>Cf|
         *      +----------+
         *     Af          Bf
         *
         */
        mesh_.solid.add_facet3(facet_idx, fAidx, fBidx, fCidx);
        pad::tile_id tid_fd(1, j, k, 0);
        if (mesh_.tile_map.count(tid_fd)) {
          DT_LOG_WARNING(logging, "Front-down tile ID=" << tid_fd << " is already registered in the tile map!")
        }
        mesh_.tile_map[tid_fd] = facet_idx;
        mesh_.reverse_tile_map[facet_idx] = tid_fd;
        facet_idx++;
        
        mesh_.solid.add_facet3(facet_idx, fAidx, fCidx, fDidx);
        pad::tile_id tid_fu(1, j, k, 1);
        if (mesh_.tile_map.count(tid_fu)) {
          DT_LOG_WARNING(logging, "Front-up tile ID=" << tid_fu << " is already registered in the tile map!")
        }
        mesh_.tile_map[tid_fu] = facet_idx;
        mesh_.reverse_tile_map[facet_idx] = tid_fu;
        facet_idx++;
        
        // xz facets (unmapped):
        if (j == 0) {
          mesh_.solid.add_facet3(facet_idx, bAidx, fAidx, fDidx);
          facet_idx++;        
          mesh_.solid.add_facet3(facet_idx, bAidx, fDidx, bDidx);
          facet_idx++;                  
        }
        if (j == ny - 1) {
          mesh_.solid.add_facet3(facet_idx, bBidx, bCidx, fCidx);
          facet_idx++;        
          mesh_.solid.add_facet3(facet_idx, bBidx, fCidx, fBidx);
          facet_idx++;                  
        }
        // xy facets (unmapped):
        if (k == 0) {
          mesh_.solid.add_facet3(facet_idx, bAidx, bBidx, fBidx);
          facet_idx++;        
          mesh_.solid.add_facet3(facet_idx, bAidx, fBidx, fAidx);
          facet_idx++;                  
        }
        if (k == nz - 1) {
          mesh_.solid.add_facet3(facet_idx, bDidx, fDidx, fCidx);
          facet_idx++;        
          mesh_.solid.add_facet3(facet_idx, bDidx, fCidx, bCidx);
          facet_idx++;                  
        }
      }
    }
    mesh_.solid.lock();
    DT_THROW_IF(not mesh_.solid.is_valid(), std::logic_error, "Resulting tessellated solid is not valid!");
    DT_THROW_IF(mesh_.tile_map.size() != mesh_.reverse_tile_map.size(),
                std::logic_error,
                "Computed tile maps have not the same size (map=" << mesh_.tile_map.size()
                << " vs reverse map=" << mesh_.reverse_tile_map.size() << ") !");
  }
  
} // namespace snrs 
