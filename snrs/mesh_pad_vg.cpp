// Ourselves;
#include <snrs/mesh_pad_vg.hpp>

// Standard library:
#include <vector>

// Bayeux:
#include <bayeux/datatools/utils.h>
#include <bayeux/datatools/exception.h>
#include <bayeux/datatools/units.h>
#include <bayeux/geomtools/utils.h>
#include <bayeux/geomtools/mapping.h>
#include <bayeux/geomtools/geom_info.h>
#include <bayeux/geomtools/logical_volume.h>
#include <bayeux/geomtools/id_selector.h>
#include <bayeux/genvtx/utils.h>
#include <bayeux/genvtx/detail/geom_manager_utils.h>
#include <bayeux/genvtx/triangle_random_tools.h>

// This project:
#include <snrs/mesh_pad_model.hpp>
#include <snrs/pad.hpp>

namespace snrs {
  
  GENVTX_VG_REGISTRATION_IMPLEMENT(mesh_pad_vg, "snrs::mesh_pad_vg")

  struct facet_info_type
  {
    int side = -1;
    const geomtools::facet34 * ptr_facet = nullptr;
  };
  
  struct surface_info_col_type
  {
    std::vector<facet_info_type> facet_infos;
  };

  struct tile_info_type
  {
    const geomtools::facet34 * ptr_back_facet = nullptr;
    const geomtools::facet34 * ptr_front_facet = nullptr;
  };
  
  struct tile_info_col_type
  {
    std::vector<tile_info_type> tile_infos;
  };
 
  struct mesh_pad_vg::pimpl_type
  {
    const geomtools::geom_info * ginfo = nullptr;
    const geomtools::logical_volume * log_vol = nullptr;
    geomtools::placement tessella_placement;
    const geomtools::i_model * geo_model = nullptr;
    const mesh_pad_model * pad_model = nullptr;
    const pad * the_pad = nullptr;
    surface_info_col_type surface_infos;
    tile_info_col_type    bulk_infos;

    void reset()
    {
      surface_infos.facet_infos.clear();
      bulk_infos.tile_infos.clear();
      the_pad = nullptr;
      pad_model = nullptr;
      geo_model = nullptr;
      log_vol = nullptr;
      ginfo = nullptr;
    }

  };
  
  int mesh_pad_vg::get_mode() const
  {
    return _mode_;
  }
  
  void mesh_pad_vg::set_mode(int mode_)
  {
    DT_THROW_IF (is_initialized(), std::logic_error,
                 "Vertex generator '" << get_name() << "' is already initialized !");
    if ((mode_ == genvtx::utils::MODE_BULK) || (mode_ == genvtx::utils::MODE_SURFACE)) {
      _mode_ = mode_;
    } else {
      _mode_ = genvtx::utils::MODE_INVALID;
    }
    return;
  }
  
  bool mesh_pad_vg::is_mode_valid() const
  {
    return is_mode_bulk() || is_mode_surface();
  }

  bool mesh_pad_vg::is_mode_bulk() const
  {
    return get_mode() == genvtx::utils::MODE_BULK;
  }

  bool mesh_pad_vg::is_mode_surface() const
  {
    return get_mode() == genvtx::utils::MODE_SURFACE;
  }

  bool mesh_pad_vg::is_back_side() const
  {
    return _back_side_;
  }
  
  bool mesh_pad_vg::is_front_side() const
  {
    return _front_side_;
  }

  void mesh_pad_vg::set_back_side(bool sbs_)
  {
    DT_THROW_IF (is_initialized(), std::logic_error,
                 "Vertex generator '" << get_name() << "' is already initialized !");
    _back_side_ = sbs_;
    return;
  }
  
  void mesh_pad_vg::set_front_side(bool sfs_)
  {
    DT_THROW_IF (is_initialized(), std::logic_error,
                 "Vertex generator '" << get_name() << "' is already initialized !");
    _front_side_ = sfs_;
    return;
  }
  
  bool mesh_pad_vg::is_film_bulk() const
  {
    return _film_bulk_;
  }

  void mesh_pad_vg::set_film_bulk(bool fb_)
  {
    DT_THROW_IF(is_initialized(), std::logic_error,
                "Vertex generator '" << get_name() << "' is already initialized !");
    _film_bulk_ = fb_;
    if (_mode_ == genvtx::utils::MODE_INVALID) {
      _mode_ = genvtx::utils::MODE_BULK;
    } else if (_mode_ == genvtx::utils::MODE_SURFACE) {
      DT_THROW_IF(fb_, std::logic_error,
                  "Film bulk is not compatible with surface mode in vertex generator '"
                  << get_name() << "'!");  
    }
    return;
  }
 
  double mesh_pad_vg::get_skin_thickness() const
  {
    return _skin_thickness_;
  }
    
  void mesh_pad_vg::set_skin_thickness(double st_)
  {
    DT_THROW_IF (is_initialized(), std::logic_error,
                 "Vertex generator '" << get_name() << "' is already initialized !");
    DT_THROW_IF (st_ < 0.0, std::logic_error,
                 "Invalid skin thickness in vertex generator '" << get_name() << "'!");
    _skin_thickness_ = st_;
    return;
  }
  
  double mesh_pad_vg::get_skin_skip() const
  {
    return _skin_skip_;
  }
    
  void mesh_pad_vg::set_skin_skip(double ss_)
  {
    DT_THROW_IF (is_initialized(), std::logic_error,
                 "Vertex generator '" << get_name() << "' is already initialized !");
    _skin_skip_ = ss_;
    return;
  }

  bool mesh_pad_vg::get_tile_selection() const
  {
    return _tile_selection_;
  }


  void mesh_pad_vg::set_tile_range(int min_column_, int max_column_,
                                   int min_row_, int max_row_,
                                   bool part_0_, bool part_1_)
  {
    DT_THROW_IF (is_initialized(), std::logic_error,
                 "Vertex generator '" << get_name() << "' is already initialized !");
    DT_THROW_IF(_tile_selection_ == TS_PATTERNS, std::logic_error,
                "Cannot set tile ID selection range because vertex generator '"
                << get_name() << "' uses tile set selection mode !"); 
    if (_tile_selection_ == TS_NONE) {
      _tile_selection_ = TS_RANGE;
    }
    if (min_column_ >= 0) {
      _tiles_min_column_ = min_column_;
    }
    if (max_column_ >= 0) {
      DT_THROW_IF(max_column_ >= 0 and (max_column_ < _tiles_min_column_),
                  std::logic_error,
                  "Invalid tiles min/max column range in vertex generator '"
                  << get_name() << "' !");
      _tiles_max_column_ = max_column_;
    }
    if (min_row_ >= 0) {
      _tiles_min_row_ = min_row_;
    }
    if (max_row_ >= 0) {
      DT_THROW_IF(max_row_ >= 0 and (max_row_ < _tiles_min_row_),
                  std::logic_error,
                  "Invalid tiles min/max row range in vertex generator '"
                  << get_name() << "' !");
      _tiles_max_row_ = max_row_;
    }
    _tiles_part_0_ = part_0_;
    _tiles_part_1_ = part_1_;
    return;
  }

  void mesh_pad_vg::add_tile_pattern(const pad::tile_id & tile_pattern_)
  {
    DT_THROW_IF (is_initialized(), std::logic_error,
                 "Vertex generator '" << get_name() << "' is already initialized !");
    DT_THROW_IF(_tile_selection_ == TS_RANGE, std::logic_error,
                "Cannot add tile ID pattern in selection set because vertex generator '"
                << get_name() << "' uses tile range selection mode !");  
    if (_tile_selection_ == TS_NONE) {
      _tile_selection_ = TS_PATTERNS;
    }
    DT_LOG_DEBUG(get_logging_priority(), "Adding tile pattern " << tile_pattern_);
    _tiles_patterns_.insert(tile_pattern_);
    return;
  }
  
  mesh_pad_vg::mesh_pad_vg()
    : genvtx::i_from_model_vg()
  {
    _initialized_ = false;
    _pimpl_.reset(new pimpl_type);
    _set_defaults_();
    return;
  }
  
  mesh_pad_vg::~mesh_pad_vg()
  {
   if (is_initialized()) reset();
    _pimpl_.reset();
  }

  bool mesh_pad_vg::is_initialized() const
  {
    return _initialized_;
  }
    
  void mesh_pad_vg::_set_defaults_()
  {
    // Internal reset:
    _mode_ = genvtx::utils::MODE_INVALID;
    _back_side_ = false;
    _front_side_ = false;
    _film_bulk_ = false;
    _skin_thickness_ = 0.0 * CLHEP::micrometer;
    _skin_skip_ = 0.0 * CLHEP::micrometer;
    this->i_from_model_vg::_set_defaults();
    return;
  }

  void mesh_pad_vg::_init_()
  {
    DT_THROW_IF (! is_mode_valid (), std::logic_error,
                 "Invalid mode in vertex generator '" << get_name() << "' !");
    DT_THROW_IF (! has_geom_manager (), std::logic_error,
                 "Missing geometry manager in vertex generator '" << get_name() << "' !");
    const geomtools::mapping * mapping_ptr
      = genvtx::detail::access_geometry_mapping(get_geom_manager (), get_mapping_plugin_name());
    DT_THROW_IF (mapping_ptr == nullptr, std::logic_error,
                 "No available geometry mapping was found in vertex generator '"
                 << get_name() << "' !");

    const geomtools::mapping & the_mapping = *mapping_ptr;
    const geomtools::geom_info_dict_type & geom_infos
      = the_mapping.get_geom_infos();
    DT_THROW_IF(_get_source_selector().get_category_info().get_category() != "source_pad",
                std::logic_error,
                "Invalid geometry category '"
                << _get_source_selector().get_category_info().get_category()
                << "' as a source of vertexes for this generator!");
    bool found = false;
    for (geomtools::geom_info_dict_type::const_iterator i
           = geom_infos.begin();
         i != geom_infos.end();
         i++) {
      const geomtools::geom_id & gid = i->first;
      if (_get_source_selector().match(gid)) {
        _pimpl_->ginfo = &(i->second);
        _pimpl_->log_vol = &(_pimpl_->ginfo->get_logical());
        _pimpl_->geo_model = &(_pimpl_->log_vol->get_geometry_model());
        // Trick: use the source foil placement as the general placement for any tiles in the tessellated mesh (source or film)
        // for (auto p : _pimpl_->log_vol->get_physicals()) {
        //   DT_LOG_DEBUG(get_logging_priority(), "Physical: key=" << p.first << " vol=" << p.second->get_name());
        // }
        _pimpl_->tessella_placement = _pimpl_->log_vol->get_physical("source.phys").get_placement().get_placement(0);
        found = true;
        break;
      }
    }
    DT_THROW_IF(! found, std::logic_error,
                "Cannot compute any source of vertex in vertex generator '"
                << get_name() << "' !");
    DT_THROW_IF(_pimpl_->geo_model->get_model_id() != "snrs::mesh_pad_model",
                std::logic_error,
                "Geometry model '" << _pimpl_->geo_model->get_name()
                << " has unexpected geometry model type '"
                << _pimpl_->geo_model->get_model_id() << "'!");
    _pimpl_->pad_model = dynamic_cast<const snrs::mesh_pad_model*>(_pimpl_->geo_model);
    _pimpl_->the_pad = &(_pimpl_->pad_model->get_pad());
    if (is_film_bulk() and not _pimpl_->the_pad->has_film()) {
      DT_THROW(std::logic_error,
               "Invalid film bulk mode because the target pad has no film in vertex generator '"
               << get_name() << "' !");
    }
    
    // Mode bulk:
    if (is_mode_bulk()) {
      _skin_skip_ = -0.5 * _pimpl_->the_pad->get_thickness();
      double skin_tolerance = 0.0;
      // Using skin thickness as a tolerance:
      if (datatools::is_valid(_skin_thickness_)) {
        skin_tolerance = _skin_thickness_;
      }
      // Force skin thickness to the pad tickness
      _skin_thickness_ = _pimpl_->the_pad->get_thickness() - skin_tolerance;
      std::size_t nbTiles = _pimpl_->the_pad->get_source_mesh().tile_map.size();
      _pimpl_->bulk_infos.tile_infos.reserve(nbTiles);
      const pad::mesh_data_type * mesh = &(_pimpl_->the_pad->get_source_mesh());
      const pad::tile_map_type & tileMap = mesh->tile_map;
      for (const auto & tileRecord : tileMap) {
        const pad::tile_id & backTileId = tileRecord.first;
        // Only use back tiles has reference:
        if (backTileId.get_side() != pad::FACE_BACK) continue;
        pad::tile_id frontTileId = backTileId;
        frontTileId.set_side(pad::FACE_FRONT);
        if (_tile_selection_ == TS_RANGE) {
          if (not _tiles_part_0_) {
            if (backTileId.get_part() == 0) continue;
          }
          if (not _tiles_part_1_) {
            if (backTileId.get_part() == 1) continue;
          }
          if (_tiles_min_row_ >= 0) {
            if (backTileId.get_row() < _tiles_min_row_) continue;
          }
          if (_tiles_max_row_ >= 0) {
            if (backTileId.get_row() > _tiles_max_row_) continue;
          }
          if (_tiles_min_column_ >= 0) {
            if (backTileId.get_column() < _tiles_min_column_) continue;
          }
          if (_tiles_max_column_ >= 0) {
            if (backTileId.get_column() > _tiles_max_column_) continue;
          }
        }
        if (_tile_selection_ == TS_PATTERNS) {
          // DT_LOG_DEBUG(get_logging_priority(), "Tile selection 'patterns'");
          bool valid_tile_id = false;
          for (const pad::tile_id & pattern : _tiles_patterns_) {
            if (pattern.match(backTileId)) {
              valid_tile_id = true;
              break;
            }
          }
          if (not valid_tile_id) continue;
        }
        DT_LOG_DEBUG(get_logging_priority(), "Accumulating tile IDs=(back=" << backTileId << ",front=" << frontTileId << ')');
        unsigned int backFacetId = tileRecord.second;
        unsigned int frontFacetId = tileMap.find(frontTileId)->second;
        tile_info_type tileInfo;
        // Reference both back and front tiles:
        tileInfo.ptr_back_facet  = &mesh->solid.facets().find(backFacetId)->second;
        tileInfo.ptr_front_facet = &mesh->solid.facets().find(frontFacetId)->second;
        _pimpl_->bulk_infos.tile_infos.push_back(tileInfo);
      }
      DT_LOG_DEBUG(get_logging_priority(),
                   "Number of tiles : " << _pimpl_->bulk_infos.tile_infos.size());
    }
    
    // Mode surface/film bulk:
    if (is_mode_surface() or is_film_bulk()) {

      if (is_film_bulk()) {
        // Compute internal skip from outer film surface :
        _skin_skip_ = -0.5 * _pimpl_->the_pad->get_film_thickness();
        double skin_tolerance = 0.0;
        if (datatools::is_valid(_skin_thickness_)) {
          skin_tolerance = _skin_thickness_;
        }
        _skin_thickness_ = _pimpl_->the_pad->get_film_thickness() - skin_tolerance;
      }
      std::size_t nbFacets = _pimpl_->the_pad->get_source_mesh().tile_map.size();
      _pimpl_->surface_infos.facet_infos.reserve(nbFacets);

      if (_tile_selection_ == TS_RANGE) {
        DT_LOG_DEBUG(get_logging_priority(), "Tile selection 'range'");
      }
      if (_tile_selection_ == TS_PATTERNS) {
        DT_LOG_DEBUG(get_logging_priority(), "Tile selection 'patterns'");
      }
      
      // Back side:
      if (is_back_side()) {
        const pad::mesh_data_type * mesh = nullptr;
        if (_pimpl_->the_pad->has_film()) {
          mesh = &(_pimpl_->the_pad->get_back_film_mesh());
        } else {
          mesh = &(_pimpl_->the_pad->get_source_mesh());
        }
        const pad::tile_map_type & tileMap = mesh->tile_map;
        DT_LOG_DEBUG(get_logging_priority(),
                     "Back tile map size : " << tileMap.size());
        for (const auto & tileRecord : tileMap) {
          const pad::tile_id & tileId = tileRecord.first;
          if (tileId.get_side() != pad::FACE_BACK) continue;
          if (_tile_selection_ == TS_RANGE) {
            if (not _tiles_part_0_) {
              if (tileId.get_part() == 0) continue;
            }
            if (not _tiles_part_1_) {
              if (tileId.get_part() == 1) continue;
            }
            if (_tiles_min_row_ >= 0) {
              if (tileId.get_row() < _tiles_min_row_) continue;
            }
            if (_tiles_max_row_ >= 0) {
              if (tileId.get_row() > _tiles_max_row_) continue;
            }
            if (_tiles_min_column_ >= 0) {
              if (tileId.get_column() < _tiles_min_column_) continue;
            }
            if (_tiles_max_column_ >= 0) {
              if (tileId.get_column() > _tiles_max_column_) continue;
            }
          }
          if (_tile_selection_ == TS_PATTERNS) {
            bool valid_tile_id = false;
            for (const pad::tile_id & pattern : _tiles_patterns_) {
              if (pattern.match(tileId)) {
                valid_tile_id = true;
                break;
              }
            }
            if (not valid_tile_id) continue;
          }
          DT_LOG_DEBUG(get_logging_priority(), "Accumulating tile ID=" << tileId);
          unsigned int facetId = tileRecord.second;
          facet_info_type facetInfo;
          facetInfo.side = pad::FACE_BACK;
          facetInfo.ptr_facet = &mesh->solid.facets().find(facetId)->second;
          _pimpl_->surface_infos.facet_infos.push_back(facetInfo);
        }
      }

      // Front side:
      if (is_front_side()) {
        const pad::mesh_data_type * mesh = nullptr;
        if (_pimpl_->the_pad->has_film()) {
          mesh = &(_pimpl_->the_pad->get_front_film_mesh());
        } else {
          mesh = &(_pimpl_->the_pad->get_source_mesh());
        }
        const pad::tile_map_type & tileMap = mesh->tile_map;
        DT_LOG_DEBUG(get_logging_priority(),
                     "Front tile map size : " << tileMap.size());
        for (const auto & tileRecord : tileMap) {
          const pad::tile_id & tileId = tileRecord.first;
          if (tileId.get_side() != pad::FACE_FRONT) continue;
          if (_tile_selection_ == TS_RANGE) {
            if (not _tiles_part_0_) {
              if (tileId.get_part() == 0) continue;
            }
            if (not _tiles_part_1_) {
              if (tileId.get_part() == 1) continue;
            }
            if (_tiles_min_row_ >= 0) {
              if (tileId.get_row() < _tiles_min_row_) continue;
            }
            if (_tiles_max_row_ >= 0) {
              if (tileId.get_row() > _tiles_max_row_) continue;
            }
            if (_tiles_min_column_ >= 0) {
              if (tileId.get_column() < _tiles_min_column_) continue;
            }
            if (_tiles_max_column_ >= 0) {
              if (tileId.get_column() > _tiles_max_column_) continue;
            }
          }
          if (_tile_selection_ == TS_PATTERNS) {
            DT_LOG_DEBUG(get_logging_priority(), "Tile selection 'patterns'");
            bool valid_tile_id = false;
            for (const pad::tile_id & pattern : _tiles_patterns_) {
              if (pattern.match(tileId)) {
                valid_tile_id = true;
                break;
              }
            }
            if (not valid_tile_id) continue;
          }
          DT_LOG_DEBUG(get_logging_priority(), "Accumulating tile ID=" << tileId);
          unsigned int facetId = tileRecord.second;
          facet_info_type facetInfo;
          facetInfo.side = pad::FACE_FRONT;
          facetInfo.ptr_facet = &mesh->solid.facets().find(facetId)->second;
          _pimpl_->surface_infos.facet_infos.push_back(facetInfo);
        }        
      }
      DT_LOG_DEBUG(get_logging_priority(),
                   "Number of tiles : " << _pimpl_->surface_infos.facet_infos.size());
    } // Mode surface/film bulk
    
    return;
  }
  
  void mesh_pad_vg::_reset_()
  {
    _pimpl_->reset();
    this->i_from_model_vg::_reset();
    _set_defaults_();
    return;
  }
    
  void mesh_pad_vg::reset()
  {
    DT_THROW_IF (! is_initialized (), std::logic_error,
                 "Vertex generator '" << get_name() << "' is not initialized !");
    _reset_ ();
    _initialized_ = false;
    return;
  }
  
  void mesh_pad_vg::initialize(const ::datatools::properties & setup_,
                               ::datatools::service_manager & service_manager_,
                               ::genvtx::vg_dict_type & generators_)
  {
    DT_THROW_IF (is_initialized (), std::logic_error, "Vertex generator '" << get_name() << "' is already initialized !");

    this->::genvtx::i_from_model_vg::_initialize(setup_, service_manager_, generators_);
    int mode = genvtx::utils::MODE_INVALID;
    std::string mode_str;
    bool back_side  = false;
    bool front_side = false;
    bool film_bulk = false;
    double skin_thickness = datatools::invalid_real();
    double skin_skip = datatools::invalid_real();
    
    DT_THROW_IF (! setup_.has_key ("mode"),
                 std::logic_error,
                 "Missing 'mode' property in vertex generator '" << get_name() << "' !");
    mode_str = setup_.fetch_string ("mode");
    if (mode_str == "bulk") mode = genvtx::utils::MODE_BULK;
    if (mode_str == "surface") mode = genvtx::utils::MODE_SURFACE;
    
    if (mode == genvtx::utils::MODE_BULK) {
      if (setup_.has_key("film_bulk")) {
        film_bulk = true;
      }
    }

    if (mode == genvtx::utils::MODE_SURFACE or film_bulk) {
      if (setup_.has_key("back_side")) {
        back_side = setup_.fetch_boolean ("back_side");
      }
      if (setup_.has_key("front_side")) {
        front_side = setup_.fetch_boolean ("front_side");
      }
      bool some_side = back_side || front_side;
      DT_THROW_IF (! some_side, std::logic_error,
                   "Missing some activated side(s) property in vertex generator '" << get_name() << "' !");
    }
    
    if (setup_.has_key("skin_thickness")) {
      skin_thickness = setup_.fetch_real_with_explicit_dimension("skin_thickness", "length");
    }
    
    if (setup_.has_key("skin_skip")) {
      DT_THROW_IF(film_bulk, std::logic_error, "Skin skip should not been set in film bulk mode in vertex generator '" << get_name() << "' !");
      skin_skip = setup_.fetch_real_with_explicit_dimension("skin_skip", "length");
    }
    DT_LOG_DEBUG(get_logging_priority(), "skin_thickness = " << skin_thickness / CLHEP::mm << " mm");
    DT_LOG_DEBUG(get_logging_priority(), "skin_skip      = " << skin_skip / CLHEP::mm << " mm");

    // Tile selection:
    bool tile_range = false;
    bool tile_set = false;
    int tiles_min_row = -1;
    int tiles_max_row = -1;
    int tiles_min_column = -1;
    int tiles_max_column = -1;
    bool tiles_part_0 = _tiles_part_0_;
    bool tiles_part_1 = _tiles_part_1_;

    if (setup_.has_key ("tiles.patterns")) {
      std::vector<std::string> tile_patterns;
      setup_.fetch("tiles.patterns", tile_patterns);
      if (tile_patterns.size()) {
        tile_set = true;
        for (const auto & token : tile_patterns) {
          DT_LOG_DEBUG(get_logging_priority(), "Processing pattern '" << token << "'...");
          pad::tile_id tilePattern;
          if (! tilePattern.from_string(token)) {
            DT_THROW(std::logic_error,
                     "Invalid tile selection pattern '" << token << "' in vertex generator '"
                     << get_name() << "' !");
          }
          add_tile_pattern(tilePattern);
        }
      }
    }
    
    if (setup_.has_key ("tiles.min_row")) {
      tiles_min_row = setup_.fetch_positive_integer("tiles.min_row");
      tile_range = true;
    }
   
    if (setup_.has_key ("tiles.max_row")) {
      tiles_max_row = setup_.fetch_positive_integer("tiles.max_row");
      tile_range = true;
    }

    DT_THROW_IF(tiles_max_row >= 0 and (tiles_max_row < tiles_min_row),
                std::logic_error,
                "Invalid tiles min/max row range in vertex generator '" << get_name() << "' !");
        
    if (setup_.has_key ("tiles.min_column")) {
      tiles_min_column = setup_.fetch_positive_integer("tiles.min_column");
      tile_range = true;
    }
   
    if (setup_.has_key ("tiles.max_column")) {
      tiles_max_column = setup_.fetch_positive_integer("tiles.max_column");
      tile_range = true;
    }

    if (setup_.has_key ("tiles.part_0")) {
      tiles_part_0 = setup_.fetch_boolean("tiles.part_0");
      tile_range = true;
    }
 
    if (setup_.has_key ("tiles.part_1")) {
      tiles_part_1 = setup_.fetch_boolean("tiles.part_1");
      tile_range = true;
    }
    DT_THROW_IF(tile_range and tile_set,
                std::logic_error,
                "Incompatible properties for tile selection modes in vertex generator '" << get_name() << "'!");

    set_mode(mode);
    set_film_bulk(film_bulk);
    if (is_mode_surface () or is_film_bulk()) {
      set_back_side(back_side);
      set_front_side(front_side);
    }
    if (not is_film_bulk() and datatools::is_valid(skin_skip)) {
      set_skin_skip(skin_skip);
    }
    if (datatools::is_valid(skin_thickness)) {
      set_skin_thickness(skin_thickness);
    }
    if (tile_range) {
      set_tile_range(tiles_min_column, tiles_max_column,
                     tiles_min_row, tiles_max_row,
                     tiles_part_0, tiles_part_1);
    }
    
    _init_();
    _initialized_ = true;
    return;
  }

  void mesh_pad_vg::tree_dump(std::ostream & out_,
                              const std::string & title_,
                              const std::string & indent_,
                              bool inherit_) const
  {
    namespace du = datatools;
    std::string indent;
    if (! indent_.empty ()) indent = indent_;
    this->i_from_model_vg::tree_dump(out_, title_, indent_, true);
    out_ << indent << du::i_tree_dumpable::tag
         << "Mode           : '" << _mode_ << "'" << std::endl;
    out_ << indent << du::i_tree_dumpable::tag
         << "Film bulk      : " << std::boolalpha << _film_bulk_ << std::endl;
    if (is_mode_surface() or is_film_bulk()) {
      out_ << indent << du::i_tree_dumpable::tag
           << "Back side    : " << _back_side_ << std::endl;
      out_ << indent << du::i_tree_dumpable::tag
           << "Front side   : " << _front_side_ << std::endl;
    }
    out_ << indent << tag << "Skin thickness : "
         << _skin_thickness_ / CLHEP::micrometer << " um"
         << std::endl;

    out_ << indent << tag << "Skin skip      : "
         << _skin_skip_ / CLHEP::micrometer << " um"
         << std::endl;

    out_ << indent << tag << "Tile selection mode : ";
    if (_tile_selection_ == TS_RANGE) {
      out_ << "range";
    } else if (_tile_selection_ == TS_PATTERNS) {
      out_ << "patterns";
    } else {
      out_ << "none";
    }
    out_ << std::endl;
    if (_tile_selection_ == TS_RANGE) {
      out_ << indent << skip_tag << tag << "Min column : " << _tiles_min_column_ << std::endl;
      out_ << indent << skip_tag << tag << "Max column : " << _tiles_max_column_ << std::endl;
      out_ << indent << skip_tag << tag << "Min row    : " << _tiles_min_row_ << std::endl;
      out_ << indent << skip_tag << tag << "Max row    : " << _tiles_max_row_ << std::endl;
      out_ << indent << skip_tag << tag << "Part 0     : " << std::boolalpha << _tiles_part_0_ << std::endl;
      out_ << indent << skip_tag << last_tag << "Part 1     : " << std::boolalpha << _tiles_part_1_ << std::endl;
    } else if (_tile_selection_ == TS_PATTERNS) {
      std::size_t count = 0;
      for (const auto & pattern : _tiles_patterns_) {
        out_ << indent_ << skip_tag;
        if (count + 1 == _tiles_patterns_.size()) {
          out_ << last_tag;
        } else {
          out_ << tag;
        }
        out_ << pattern;
        out_ << std::endl;
        count++;
      }
    }
    out_ << indent << du::i_tree_dumpable::inherit_tag (inherit_)
         << "end"
         << std::endl;
    return;
  }


  void mesh_pad_vg::_shoot_vertex(::mygsl::rng & random_,
                                  ::geomtools::vector_3d & vertex_)
  {
    DT_THROW_IF (! is_initialized (), std::logic_error, "Vertex generator '" << 
                 get_name() << "' is not initialized !");
    geomtools::invalidate(vertex_);
    if (is_mode_surface() or is_film_bulk()) {
      this->_shoot_vertex_surface(random_, vertex_);
    }
    if (is_mode_bulk()) {
      this->_shoot_vertex_bulk(random_, vertex_);
    }
    return;
  }
  

  void mesh_pad_vg::_shoot_vertex_bulk(::mygsl::rng & random_,
                                       ::geomtools::vector_3d & vertex_)
  {
    static const geomtools::vector_3d ux(1.0, 0.0, 0.0);
    unsigned int nb_tiles = _pimpl_->bulk_infos.tile_infos.size();
    unsigned int index_tile = random_.uniform_int(nb_tiles);
    const tile_info_type & tileInfo = _pimpl_->bulk_infos.tile_infos[index_tile];
    const geomtools::facet34 & backFacet = *tileInfo.ptr_back_facet;
    const geomtools::facet34 & frontFacet = *tileInfo.ptr_front_facet;
    const geomtools::vector_3d & bVtx0 = backFacet.get_vertex(0).get_position();
    const geomtools::vector_3d & bVtx1 = backFacet.get_vertex(1).get_position();
    const geomtools::vector_3d & bVtx2 = backFacet.get_vertex(2).get_position();
    const geomtools::vector_3d & fVtx0 = frontFacet.get_vertex(0).get_position();
    const geomtools::vector_3d & fVtx1 = frontFacet.get_vertex(1).get_position();
    const geomtools::vector_3d & fVtx2 = frontFacet.get_vertex(2).get_position();
    geomtools::vector_3d normal = backFacet.get_normal();    
    if (normal.dot(ux) > 0) normal *= -1.0;  
    geomtools::vector_3d local_vertex = 
      genvtx::triangle_random_surface(bVtx0, bVtx1, bVtx2, random_);
    double skip = _skin_skip_ + _skin_thickness_ * random_.flat(-0.5, 0.5);
    local_vertex -= normal * skip;
    // Compute the vertex position in the strip frame:
    geomtools::vector_3d source_strip_vertex;
    _pimpl_->tessella_placement.child_to_mother(local_vertex, source_strip_vertex);
    // Compute the vertex position in the world frame:
    const geomtools::placement & world_plct
      = _pimpl_->ginfo->get_world_placement();
    DT_LOG_DEBUG(get_logging_priority(), "World placement : " << world_plct);
    world_plct.child_to_mother(source_strip_vertex, vertex_);
    return;
  }

  
  void mesh_pad_vg::_shoot_vertex_surface(::mygsl::rng & random_,
                                          ::geomtools::vector_3d & vertex_) 
  {
    static const geomtools::vector_3d ux(1.0, 0.0, 0.0);
    unsigned int nb_facets = _pimpl_->surface_infos.facet_infos.size();
    unsigned int index_facet = random_.uniform_int(nb_facets);
    const facet_info_type & facetInfo = _pimpl_->surface_infos.facet_infos[index_facet];
    bool back = (facetInfo.side == pad::FACE_BACK);
    bool front = (facetInfo.side == pad::FACE_FRONT);
    DT_LOG_DEBUG(get_logging_priority(), "Back facet  : " << std::boolalpha << back);
    DT_LOG_DEBUG(get_logging_priority(), "Front facet : " << std::boolalpha << front);
    const geomtools::facet34 & facet = *facetInfo.ptr_facet;
    const geomtools::vector_3d & vtx0 = facet.get_vertex(0).get_position();
    const geomtools::vector_3d & vtx1 = facet.get_vertex(1).get_position();
    const geomtools::vector_3d & vtx2 = facet.get_vertex(2).get_position();
    geomtools::vector_3d normal = facet.get_normal();    
    if (back and normal.dot(ux) > 0) normal *= -1.0;  
    if (front and normal.dot(ux) < 0) normal *= -1.0;
    DT_LOG_DEBUG(get_logging_priority(), "Normal : " << normal / CLHEP::mm  << " mm");
    geomtools::vector_3d local_vertex = 
      genvtx::triangle_random_surface(vtx0, vtx1, vtx2, random_);
    // local_vertex = vtx0; // Test
    double skip = 0.0;
    bool has_skip = false;
    if (datatools::is_valid(_skin_skip_) and _skin_skip_ != 0.0) {
      skip += _skin_skip_;
      DT_LOG_DEBUG(get_logging_priority(), "Skin skip = " << _skin_skip_ / CLHEP::mm  << " mm");
      has_skip = true;
    }
    if (datatools::is_valid(_skin_thickness_) and _skin_thickness_ > 0.0) {
      DT_LOG_DEBUG(get_logging_priority(), "Skin thickness = " << _skin_thickness_ / CLHEP::mm  << " mm");
      skip += _skin_thickness_ * random_.flat(-0.5, 0.5);
      has_skip = true;
    }
    DT_LOG_DEBUG(get_logging_priority(), "Skip : " << skip / CLHEP::mm  << " mm");
    if (has_skip) {
      local_vertex += normal * skip;
    }
    DT_LOG_DEBUG(get_logging_priority(), "Log volume : " << _pimpl_->log_vol->get_name());
    DT_LOG_DEBUG(get_logging_priority(), "  -> internal volumes : " << _pimpl_->log_vol->get_physicals().size());
    // Compute the vertex position in the strip frame:
    geomtools::vector_3d source_strip_vertex;
    _pimpl_->tessella_placement.child_to_mother(local_vertex, source_strip_vertex);
    // Compute the vertex position in the world frame:
    const geomtools::placement & world_plct
      = _pimpl_->ginfo->get_world_placement();
    DT_LOG_DEBUG(get_logging_priority(), "World placement : " << world_plct);
    world_plct.child_to_mother(source_strip_vertex, vertex_);
    return;
  }

} // namespace snrs 

