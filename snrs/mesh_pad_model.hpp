#ifndef SNRS_MESH_PAD_MODEL_HPP
#define SNRS_MESH_PAD_MODEL_HPP

// Standard Library:
#include <vector>
#include <map>
#include <string>

// Bayeux:
#include <bayeux/datatools/utils.h>
#include <bayeux/datatools/clhep_units.h>
#include <bayeux/geomtools/tessellation.h>
#include <geomtools/i_boxed_model.h>
#include <geomtools/box.h>
#include <geomtools/physical_volume.h>
#include <geomtools/placement.h>
#include <geomtools/model_macros.h>
#include <geomtools/model_with_internal_items_tools.h>
#include <geomtools/plain_model.h>

// This project:
#include <snrs/pad.hpp>

namespace snrs {

  /// \brief SuperNEMO model for a realistic source pad with 3D-mesh
  class mesh_pad_model
    : public geomtools::i_boxed_model
  {
  public:

    mesh_pad_model();

    ~mesh_pad_model() override;

    std::string get_model_id() const override;

    void tree_dump(std::ostream & out_ = std::clog,
                   const std::string & title_ = "",
                   const std::string & indent_ = "",
                   bool inherit_ = false) const override;

  protected:

    void _at_construct(const datatools::properties & config_,
                       geomtools::models_col_type * models_ = nullptr) override;

    void _at_destroy(geomtools::models_col_type * models_ = nullptr) override;

  public:
    
    const pad & get_pad() const;
 
    const geomtools::box & get_solid() const;
   
    const std::string & get_material_name() const;

    void set_material_name(const std::string &);

    const std::string & get_source_material_name() const;

    void set_source_material_name(const std::string &);

    const std::string & get_film_material_name() const;

    void set_film_material_name(const std::string &);

    const geomtools::box & get_box() const override;

  private:

    pad              _pad_;   ///< Reference pad with all useful informations
    geomtools::box   _solid_; ///< Mother box
    std::string      _material_name_;        ///< Name of the material of the mother volume (gas)
    std::string      _source_material_name_; ///< Name of the source material (selenium mixture)
    std::string      _film_material_name_;   ///< Name of the film material (mylar)
    double           _x_shift_ = 0.0;        ///< Placement shift along the X-axis
    double           _y_shift_ = 0.0;        ///< Placement shift along the y-axis
    geomtools::MWIM        _internals_;        ///< Support for internal items
    geomtools::plain_model _source_model_;     ///< Main source geometry model
    geomtools::plain_model _back_film_model_;  ///< Back film geometry model
    geomtools::plain_model _front_film_model_; ///< Front film geometry model
    
    // registration interface :
    GEOMTOOLS_MODEL_REGISTRATION_INTERFACE(mesh_pad_model)
    
  };

} // namespace snrs 

#include <datatools/ocd_macros.h>
DOCD_CLASS_DECLARATION(snrs::mesh_pad_model)
  
#endif // SNRS_MESH_PAD_MODEL_HPP
