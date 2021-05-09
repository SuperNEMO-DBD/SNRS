#ifndef SNRS_MESH_PAD_VG_HPP
#define SNRS_MESH_PAD_VG_HPP

// Standard Library:
#include <vector>
#include <map>
#include <set>
#include <memory>

// Bayeux:
#include <bayeux/datatools/utils.h>
#include <bayeux/datatools/clhep_units.h>
#include <bayeux/geomtools/tessellation.h>
#include <bayeux/genvtx/i_from_model_vg.h>
#include <bayeux/genvtx/utils.h>

// This project:
#include <snrs/pad.hpp>
#include <snrs/mesh_pad_model.hpp>

namespace snrs {
  
  class mesh_pad_vg
    : public genvtx::i_from_model_vg
  {
  public:

    int get_mode() const;

    void set_mode(int);
    
    bool is_mode_valid() const;

    bool is_mode_bulk() const;

    bool is_mode_surface() const;

    bool is_back_side() const;

    bool is_front_side() const;

    void set_back_side(bool);

    void set_front_side(bool);

    bool is_film_bulk() const;

    void set_film_bulk(bool);

    double get_skin_skip() const;
    
    void set_skin_skip(double);

    double get_skin_thickness() const;
    
    void set_skin_thickness(double);

    enum tile_selection_type
      {
       TS_NONE  = 0,
       TS_RANGE = 1,
       TS_PATTERNS   = 2
      };
    
    bool get_tile_selection() const;

    void set_tile_range(int min_column_, int max_column_,
                        int min_row_, int max_row_,
                        bool part_0_, bool part_1_); 
  
    void add_tile_pattern(const pad::tile_id & tile_pattern_);

    void tree_dump(std::ostream & out_ = std::clog,
                   const std::string & title_ = "",
                   const std::string & indent_ = "",
                   bool inherit_ = false) const override;

    /// Constructor
    mesh_pad_vg();

    /// Destructor
    ~mesh_pad_vg() override;

    /// Initialization
    ///
    /// Example:
    /// @code
    /// mode           : string = "surface"
    /// back_side      : boolean = true
    /// front_side     : boolean = true
    /// skin.thickness : real as length = 0.1 um
    ///
    /// mapping.plugin_name   : string = "mapping"
    /// materials.plugin_name : string = "materials"
    /// origin : string = " category='source_pad' module=0 strip=5 pad=0 " 
    /// @endcode
    ///
    void initialize(const ::datatools::properties &,
                    ::datatools::service_manager &,
                    ::genvtx::vg_dict_type &) override;

    /// Reset
    void reset() override;

    /// Check initialization status
    bool is_initialized() const override;
    
 protected :

    /// Randomize vertex
    void _shoot_vertex(::mygsl::rng & random_,
                       ::geomtools::vector_3d & vertex_) override;

    /// Randomize vertex in bulk
    void _shoot_vertex_bulk(::mygsl::rng & random_,
                            ::geomtools::vector_3d & vertex_);

    /// Randomize vertex on_surface
    void _shoot_vertex_surface(::mygsl::rng & random_,
                            ::geomtools::vector_3d & vertex_);
  
  private:

    void _init_();

    void _reset_();

    void _set_defaults_();

  private:

    // Management:
    bool _initialized_ = false; //!< Initialization flag

    // Configuration:
    int  _mode_ = genvtx::utils::MODE_INVALID; //!< Mode : "bulk" of "surface"
    bool _film_bulk_ = false;
    bool _back_side_ = false; //!< Flag for inner side surface generation mode
    bool _front_side_ = false; //!< Flag for outer side surface generation mode
    double _skin_thickness_ = datatools::invalid_real(); //!< Skin thickness (length >=0 um)
    double _skin_skip_ = datatools::invalid_real(); //!< Skin skip (length)
    
    // Tile selection:
    tile_selection_type _tile_selection_ = TS_NONE; //!< Tile selection mode:
    // Range mode:
    int  _tiles_min_column_ = -1;
    int  _tiles_max_column_ = -1;
    int  _tiles_min_row_ = -1;
    int  _tiles_max_row_ = -1;
    bool _tiles_part_0_ = true;    
    bool _tiles_part_1_ = true;
    // Set mode:
    std::set<pad::tile_id> _tiles_patterns_;
    
    // Working pimpl:
    struct pimpl_type;
    std::unique_ptr<pimpl_type> _pimpl_;

    /// Registration macro
    /// @arg mesh_pad_vg the class to be registered
    GENVTX_VG_REGISTRATION_INTERFACE(mesh_pad_vg)
 
  };

} // namespace snrs 
  
#endif // SNRS_MESH_PAD_VG_HPP
