// Ourselves;
#include <snrs/mesh_pad_model.hpp>

// Bayeux:
#include <bayeux/datatools/utils.h>
#include <bayeux/datatools/exception.h>
#include <bayeux/datatools/units.h>
#include <bayeux/geomtools/utils.h>

namespace snrs {
  
  // registration :
  GEOMTOOLS_MODEL_REGISTRATION_IMPLEMENT(mesh_pad_model,"snrs::mesh_pad_model")

  const pad & mesh_pad_model::get_pad() const
  {
    return _pad_;
  }

  const std::string & mesh_pad_model::get_material_name() const
  {
    DT_THROW_IF(! is_constructed(), std::logic_error, "Operation not allowed ! Model has not been constructed yet");
    return _material_name_;
  }

  void mesh_pad_model::set_material_name(const std::string & mn_)
  {
    DT_THROW_IF(is_constructed(), std::logic_error, "Operation not allowed ! Model has already been constructed yet");
    _material_name_ = mn_;
    return;
  }

  const std::string & mesh_pad_model::get_source_material_name() const
  {
    DT_THROW_IF(! is_constructed(), std::logic_error, "Operation not allowed ! Model has not been constructed yet");
    return _source_material_name_;
  }

  void mesh_pad_model::set_source_material_name(const std::string & mn_)
  {
    DT_THROW_IF(is_constructed(), std::logic_error, "Operation not allowed ! Model has already been constructed yet");
    _source_material_name_ = mn_;
    return;
  }
  
  const std::string & mesh_pad_model::get_film_material_name() const
  {
    DT_THROW_IF(! is_constructed(), std::logic_error, "Operation not allowed ! Model has not been constructed yet");
    return _film_material_name_;
  }

  void mesh_pad_model::set_film_material_name(const std::string & mn_)
  {
    DT_THROW_IF(is_constructed(), std::logic_error, "Operation not allowed ! Model has already been constructed yet");
    _film_material_name_ = mn_;
    return;
  }

  const geomtools::box & mesh_pad_model::get_box() const
  {
    DT_THROW_IF(! is_constructed(), std::logic_error, "Operation not allowed ! Model has not been constructed yet");
    return _solid_;
  }

  const geomtools::box & mesh_pad_model::get_solid() const
  {
    DT_THROW_IF(! is_constructed(), std::logic_error, "Operation not allowed ! Model has not been constructed yet");
    return _solid_;
  }

  std::string mesh_pad_model::get_model_id() const
  {
    return "snrs::mesh_pad_model";
  }

  mesh_pad_model::mesh_pad_model()
    : i_boxed_model()
  {
    _material_name_ = "";
    _source_material_name_ = "";
    _film_material_name_ = "";
    return;
  }

  mesh_pad_model::~mesh_pad_model()
  {
    return;
  }

  void mesh_pad_model::_at_construct(const datatools::properties & config_,
                                     geomtools::models_col_type * models_)
  {
    auto logging = get_logging_priority();
    logging = datatools::logger::PRIO_NOTICE;
    DT_LOG_TRACE(logging,  "Entering...");
    int strip_id = -1;
    int pad_id   = 0;
    DT_THROW_IF(not config_.has_key("strip_id"),
                std::logic_error,
                "Missing strip ID property ('strip_id')!");
    strip_id = config_.fetch_positive_integer("strip_id");
    if (config_.has_key("pad_id")) {
      pad_id = config_.fetch_positive_integer("pad_id");
    }
    std::string pad_load_filename;
    std::string pad_source_mesh_filename;
    std::string pad_source_tiles_filename;
    if (config_.has_key("pad.load_file")) {
      pad_load_filename = config_.fetch_path("pad.load_file");
    }
    if (config_.has_key("pad.source.load_mesh")) {
      pad_source_mesh_filename = config_.fetch_path("pad.source.load_mesh");
    }
    if (config_.has_key("pad.source.load_tiles")) {
      pad_source_tiles_filename = config_.fetch_path("pad.source.load_tiles");
    }
    if (strip_id >= 0 and pad_load_filename.empty()) {
      pad_load_filename         = "@snrs:data/geometry/source_foils/fsf/strip-" + std::to_string(strip_id) + "-pad-" + std::to_string(pad_id) + "-shaped.dat";
      pad_source_mesh_filename  = "@snrs:data/geometry/source_foils/fsf/strip-" + std::to_string(strip_id) + "-pad-" + std::to_string(pad_id) + "-tessellated.dat";
      pad_source_tiles_filename = "@snrs:data/geometry/source_foils/fsf/strip-" + std::to_string(strip_id) + "-pad-" + std::to_string(pad_id) + "-tiles.dat";
    }
    {
      datatools::fetch_path_with_env(pad_load_filename);
      std::ifstream fpad(pad_load_filename);
      DT_THROW_IF(!fpad, std::logic_error, "Cannot open file '" << pad_load_filename << "'!");
      DT_LOG_NOTICE(logging, "Loading pad from '" << pad_load_filename << "'...");
      _pad_.load(fpad);
    }
    if (_pad_.get_strip_id() != strip_id) {
      DT_LOG_NOTICE(logging, "Requested strip ID=" << strip_id << " and strip ID="  << _pad_.get_strip_id() << " from file do not match!");
    }
    if (_pad_.get_pad_id() != pad_id) {
      DT_LOG_NOTICE(logging, "Requested pad ID=" << pad_id << " and pad ID="  << _pad_.get_pad_id() << " from file do not match!");
    }
    DT_LOG_NOTICE(logging, "Pad :");
    _pad_.print(std::clog);
    DT_THROW_IF(not _pad_.has_distortion(),
                std::logic_error,
                "This pad has no distortion and is not eligible to this model!");
    {
      datatools::fetch_path_with_env(pad_source_mesh_filename);
      std::ifstream fSourceMeshFile(pad_source_mesh_filename);
      DT_THROW_IF(!fSourceMeshFile, std::logic_error, "Cannot open file '" << pad_source_mesh_filename << "'!");
      _pad_.grab_source_mesh().solid.load(fSourceMeshFile);
      _pad_.grab_source_mesh().solid.lock();
      DT_THROW_IF(not _pad_.grab_source_mesh().solid.is_locked(),
                  std::logic_error,
                  "Source mesh solid is not locked!");
    }
    {
      datatools::fetch_path_with_env(pad_source_tiles_filename);
      std::ifstream fSourceTilesFile(pad_source_tiles_filename);
      DT_THROW_IF(!fSourceTilesFile, std::logic_error, "Cannot open file '" << pad_source_tiles_filename << "'!");
      _pad_.grab_source_mesh().load_tile_maps(fSourceTilesFile);
    }
    double max_depth = 58.0 * CLHEP::mm;
    DT_LOG_NOTICE(logging, "max_depth = " << max_depth / CLHEP::mm << " mm");
    double max_width = _pad_.get_width();
    DT_LOG_NOTICE(logging, "max_width = " << max_width / CLHEP::mm << " mm");
    const mygsl::min_max & xrange = _pad_.grab_source_mesh().solid.get_bounding_box_x();
    // xrange.tree_dump(std::cerr, "X-range: ", "*************** [debug] ");
    DT_LOG_NOTICE(logging, "mesh X depth = " << xrange.get_width() / CLHEP::mm << " mm");
    DT_THROW_IF(xrange.get_width() >= max_depth,
                std::logic_error,
                "Mesh is too thick to fit the mother box for strip #" << strip_id << " pad #" << pad_id);
    const mygsl::min_max & yrange = _pad_.grab_source_mesh().solid.get_bounding_box_y();
    // yrange.tree_dump(std::cerr, "Y-range: ", "*************** [debug] ");
    DT_LOG_NOTICE(logging, "mesh Y width = " << yrange.get_width() / CLHEP::mm << " mm");
    DT_THROW_IF(yrange.get_width() >= max_width,
                std::logic_error,
                "Mesh is too large to fit the mother box for strip #" << strip_id << " pad #" << pad_id);
    DT_LOG_NOTICE(logging, "xrange min = " << xrange.get_min() / CLHEP::mm << " mm");
    DT_LOG_NOTICE(logging, "xrange max = " << xrange.get_max() / CLHEP::mm << " mm");
    DT_LOG_NOTICE(logging, "yrange min = " << yrange.get_min() / CLHEP::mm << " mm");
    DT_LOG_NOTICE(logging, "yrange max = " << yrange.get_max() / CLHEP::mm << " mm");
    double safe_gap = 2 * CLHEP::micrometer;
    double safe_film_gap = 25 * CLHEP::micrometer;
    
    // Compute X-shift if needed:
    if (xrange.get_max() > max_depth / 2) {
      _x_shift_ = 0.5 * max_depth - xrange.get_max() - safe_gap;
      if (_pad_.has_film()) {
        _x_shift_ -= safe_film_gap;
      }
    }
    if (xrange.get_min() < -max_depth / 2) {
      _x_shift_ = - 0.5 * max_depth - xrange.get_min() + safe_gap;
      if (_pad_.has_film()) {
        _x_shift_ += safe_film_gap;
      }
    }
    DT_LOG_NOTICE(logging, "_x_shift_ = " << _x_shift_ / CLHEP::mm << " mm");
    DT_THROW_IF((xrange.get_max() + _x_shift_) > 0.5 * max_depth,
                std::logic_error,
                "Shifted mesh still not match the box max bound on X-axis for strip #" << strip_id << " pad #" << pad_id); 
    DT_THROW_IF((xrange.get_min() + _x_shift_) < -0.5 * max_depth,
                std::logic_error,
                "Shifted mesh still not match the box min bound on X-axis for strip #" << strip_id << " pad #" << pad_id); 
    
    // Compute Y-shift if needed:
    if (yrange.get_max() > max_width / 2) {
      _y_shift_ = 0.5 * max_width - yrange.get_max() - safe_gap;
      if (_pad_.has_film()) {
        _y_shift_ -= safe_film_gap;
      }
    }
    if (yrange.get_min() < -max_width / 2) {
      _y_shift_ = - 0.5 * max_width - yrange.get_min() + safe_gap;
      if (_pad_.has_film()) {
        _y_shift_ += safe_film_gap;
      }
    }
    DT_LOG_NOTICE(logging, "_y_shift_ = " << _y_shift_ / CLHEP::mm << " mm");
    DT_THROW_IF((yrange.get_max() + _y_shift_) > 0.5 * max_width,
                std::logic_error,
                "Shifted mesh still not match the box max bound on Y-axis for strip #" << strip_id << " pad #" << pad_id); 
    DT_THROW_IF((yrange.get_min() + _y_shift_) < -0.5 * max_width,
                std::logic_error,
                "Shifted mesh still not match the box min bound on Y-axis for strip #" << strip_id << " pad #" << pad_id); 

    // double min_depth = std::max(std::abs(xrange.get_min()), std::abs(xrange.get_max()));
    // double min_width = std::max(std::abs(yrange.get_min()), std::abs(yrange.get_max()));
    // if (_pad_.has_film()) {
    //   // Safe margin:
    //   min_depth += 2 * _pad_.get_film_thickness() + 10 * CLHEP::micrometer;
    //   min_width += 25 * CLHEP::micrometer;
    // }
    // DT_LOG_NOTICE(logging, "min_depth = " << min_depth / CLHEP::mm << " mm");
    // DT_LOG_NOTICE(logging, "min_width = " << min_depth / CLHEP::mm << " mm");
    double depth = max_depth;
    double width = max_width;
    DT_LOG_NOTICE(logging, "depth     = " << depth / CLHEP::mm << " mm");
    DT_LOG_NOTICE(logging, "width     = " << width / CLHEP::mm << " mm");

    double default_length_unit = 1.0 * CLHEP::mm;
    if (config_.has_key ("length_unit")) {
      const std::string length_unit_str = config_.fetch_string("length_unit");
      default_length_unit = datatools::units::get_length_unit_from(length_unit_str);
    }

    if (config_.has_key("x_shift")) {
      _x_shift_ = config_.fetch_real("x_shift");
      if (! config_.has_explicit_unit("x_shift")) {
        _x_shift_ *= default_length_unit;
      }
    }

    if (config_.has_key("y_shift")) {
      _y_shift_ = config_.fetch_real("y_shift");
      if (! config_.has_explicit_unit("y_shift")) {
        _y_shift_ *= default_length_unit;
      }
    }

    // if (config_.has_key("depth")) {
    //   depth = config_.fetch_real("depth");
    //   if (! config_.has_explicit_unit("depth")) {
    //     depth *= default_length_unit;
    //   }
    // }
    // DT_THROW_IF(depth < min_depth or depth > max_depth,
    //              std::range_error,
    //             "Invalid depth value (depth=" << depth/CLHEP::mm
    //             << " mm is not in [" << min_depth/CLHEP::mm << "," << max_depth/CLHEP::mm << "] mm!");

    // if (config_.has_key("width")) {
    //   width = config_.fetch_real("width");
    //   if (! config_.has_explicit_unit("width")) {
    //     width *= default_length_unit;
    //   }
    // }
    // DT_THROW_IF(width < min_width or width > max_width,
    //              std::range_error,
    //             "Invalid width value (width=" << width/CLHEP::mm
    //             << " mm is not in [" << min_width/CLHEP::mm << "," << max_width/CLHEP::mm << "] mm!");
     
    if (_pad_.has_film()) {
      std::string pad_back_film_mesh_filename  = "@snrs:data/geometry/source_foils/fsf/strip-" + std::to_string(strip_id) + "-pad-" + std::to_string(pad_id) + "-tessellated-back-film.dat";
      std::string pad_back_film_tiles_filename = "@snrs:data/geometry/source_foils/fsf/strip-" + std::to_string(strip_id) + "-pad-" + std::to_string(pad_id) + "-tiles-back-film.dat";
      std::string pad_front_film_mesh_filename  = "@snrs:data/geometry/source_foils/fsf/strip-" + std::to_string(strip_id) + "-pad-" + std::to_string(pad_id) + "-tessellated-back-film.dat";
      std::string pad_front_film_tiles_filename = "@snrs:data/geometry/source_foils/fsf/strip-" + std::to_string(strip_id) + "-pad-" + std::to_string(pad_id) + "-tiles-back-film.dat";
      // Back and front film mesh:es and tile maps:
      if (config_.has_key("pad.back_film.load_mesh")) {
        pad_back_film_mesh_filename = config_.fetch_path("pad.back_film.load_mesh");
      }
      if (config_.has_key("pad.back_film.load_tiles")) {
        pad_back_film_tiles_filename = config_.fetch_path("pad.back_film.load_tiles");
      }
      if (config_.has_key("pad.front_film.load_mesh")) {
        pad_front_film_mesh_filename = config_.fetch_path("pad.front_film.load_mesh");
      }
      if (config_.has_key("pad.front_film.load_tiles")) {
        pad_front_film_tiles_filename = config_.fetch_path("pad.front_film.load_tiles");
      } 
      {
        datatools::fetch_path_with_env(pad_back_film_mesh_filename);
        std::ifstream fBackFilmMeshFile(pad_back_film_mesh_filename);
        DT_THROW_IF(!fBackFilmMeshFile, std::logic_error, "Cannot open file '" << pad_back_film_mesh_filename << "'!");
        _pad_.grab_back_film_mesh().solid.load(fBackFilmMeshFile);
        _pad_.grab_back_film_mesh().solid.lock();
      }  
      {
        datatools::fetch_path_with_env(pad_back_film_tiles_filename);
        std::ifstream fBackFilmTilesFile(pad_back_film_tiles_filename);
        DT_THROW_IF(!fBackFilmTilesFile, std::logic_error, "Cannot open file '" << pad_back_film_tiles_filename << "'!");
        _pad_.grab_back_film_mesh().load_tile_maps(fBackFilmTilesFile);
      }
      {
        datatools::fetch_path_with_env(pad_front_film_mesh_filename);
        std::ifstream fFrontFilmMeshFile(pad_front_film_mesh_filename);
        DT_THROW_IF(!fFrontFilmMeshFile, std::logic_error, "Cannot open file '" << pad_front_film_mesh_filename << "'!");
        _pad_.grab_front_film_mesh().solid.load(fFrontFilmMeshFile);
        _pad_.grab_front_film_mesh().solid.lock();
        _pad_.grab_front_film_mesh().solid.tree_dump(std::cerr, "Front film mesh: ", "[debug] ");
      }   
      {
        datatools::fetch_path_with_env(pad_front_film_tiles_filename);
        std::ifstream fFrontFilmTilesFile(pad_front_film_tiles_filename);
        DT_THROW_IF(!fFrontFilmTilesFile, std::logic_error, "Cannot open file '" << pad_front_film_tiles_filename << "'!");
        _pad_.grab_front_film_mesh().load_tile_maps(fFrontFilmTilesFile);
      }
    }

    // Material support:
    set_material_name("tracking_gas");
    if (config_.has_key("material.ref")) {
      const std::string material_name = config_.fetch_string("material.ref");
      set_material_name(material_name);
    }
    
    set_source_material_name(_pad_.get_material());
    if (config_.has_key("source_material.ref")) {
      const std::string source_material_name = config_.fetch_string("source_material.ref");
      set_source_material_name(source_material_name);
    }
    
    if (_pad_.has_film()) {
      set_film_material_name("basic::mylar");
      if (config_.has_key("film_material.ref")) {
        const std::string film_material_name = config_.fetch_string("film_material.ref");
        set_film_material_name(film_material_name);
      }
    }
    _solid_.set_x(depth);
    _solid_.set_y(width);
    _solid_.set_z(_pad_.get_height());
    _solid_.lock();
    DT_THROW_IF(! _solid_.is_valid(), std::logic_error, "Invalid envelope box dimensions in model '" << get_name() << "' !");
    grab_logical().set_name(i_model::make_logical_volume_name(get_name()));
    grab_logical().set_shape(_solid_);
    grab_logical().set_material_ref(_material_name_);

    // Internal items:
    std::vector<std::string> exported_properties_prefixes = { "visibility." };
    
    std::string source_model_name
      = geomtools::i_model::extract_basename_from_model_name(get_name())
      + ".source"
      + geomtools::i_model::model_suffix();
    _source_model_.set_name(source_model_name);
    _source_model_.set_solid(_pad_.get_source_mesh().solid);
    _source_model_.set_material_name(_pad_.get_material());
    datatools::properties & sourceModelSetup = _source_model_.parameters();
    config_.export_and_rename_starting_with(sourceModelSetup,
                                            "source.",
                                            "");
    _source_model_.construct(source_model_name, sourceModelSetup, exported_properties_prefixes, models_);
    
    // Set source foil and films:
    geomtools::placement source_placement(_x_shift_, _y_shift_, 0.0, 0.0, 0.0, 0.0);
    DT_LOG_NOTICE(logging, "source placement = " << source_placement);
    _internals_.add_item("source",
                         _source_model_,
                         source_placement);

    std::string back_film_model_name
      = geomtools::i_model::extract_basename_from_model_name(get_name())
      + ".back_film"
      + geomtools::i_model::model_suffix();
    std::string front_film_model_name
      = geomtools::i_model::extract_basename_from_model_name(get_name())
      + ".front_film"
      + geomtools::i_model::model_suffix();
    if (_pad_.has_film()) {
      // Back film:
      geomtools::placement back_film_placement(_x_shift_, _y_shift_, 0.0, 0.0, 0.0, 0.0);
      _back_film_model_.set_name(back_film_model_name);
      _back_film_model_.set_solid(_pad_.get_back_film_mesh().solid);
      _film_material_name_ = _pad_.get_film_material();
      _back_film_model_.set_material_name(_film_material_name_);
      datatools::properties & backFilmModelSetup = _back_film_model_.parameters();
      config_.export_and_rename_starting_with(backFilmModelSetup,
                                              "back_film.",
                                              "");
      _back_film_model_.construct(back_film_model_name, backFilmModelSetup, exported_properties_prefixes, models_);
      _internals_.add_item("back_film",
                           _back_film_model_,
                           back_film_placement);

      // Front film:
      geomtools::placement front_film_placement(_x_shift_, _y_shift_, 0.0, 0.0, 0.0, 0.0);
      _front_film_model_.set_name(front_film_model_name);
      _front_film_model_.set_solid(_pad_.get_front_film_mesh().solid);
      _film_material_name_ = _pad_.get_film_material();
      _front_film_model_.set_material_name(_film_material_name_);
      datatools::properties & frontFilmModelSetup = _front_film_model_.parameters();
      config_.export_and_rename_starting_with(frontFilmModelSetup,
                                              "front_film.",
                                              "");
      _front_film_model_.construct(front_film_model_name, frontFilmModelSetup, exported_properties_prefixes, models_);
      _internals_.add_item("front_film",
                           _front_film_model_,
                           front_film_placement);
    }
                        
    _internals_.init_internal_items(grab_logical(), models_);

    // Register the model in the global collection:
    DT_LOG_NOTICE(logging, "Registering the source model '" << source_model_name << "'...");
    (*models_)[source_model_name] = &_source_model_;
    if (_pad_.has_film()) {
       (*models_)[back_film_model_name] = &_back_film_model_;
       (*models_)[front_film_model_name] = &_front_film_model_;
    }   
    DT_LOG_TRACE(logging, "Exiting.");
    return;
  }

  void mesh_pad_model::_at_destroy(geomtools::models_col_type * models_)
  {
    auto logging = get_logging_priority();
    logging = datatools::logger::PRIO_NOTICE;
    DT_LOG_TRACE(logging,  "Entering...");
    std::string source_model_name
      = geomtools::i_model::extract_basename_from_model_name(get_name())
      + ".source"
      + geomtools::i_model::model_suffix();
   
    if (_pad_.has_film()) {
      std::string back_film_model_name
        = geomtools::i_model::extract_basename_from_model_name(get_name())
        + ".back_film"
        + geomtools::i_model::model_suffix();
      std::string front_film_model_name
        = geomtools::i_model::extract_basename_from_model_name(get_name())
        + ".front_film"
        + geomtools::i_model::model_suffix();
      DT_LOG_NOTICE(logging, "Unregistering the back film model '" << _back_film_model_.get_name() << "'...");
      DT_LOG_NOTICE(logging, "Unregistering the front film model '" << _front_film_model_.get_name() << "'...");
      _back_film_model_.destroy(models_);
      _front_film_model_.destroy(models_);
      models_->erase(back_film_model_name);
      models_->erase(front_film_model_name);
    }
    DT_LOG_NOTICE(logging, "Unregistering the source model '" << source_model_name << "'...");
    models_->erase(source_model_name);
    _source_model_.destroy(models_);
  
    DT_LOG_TRACE(logging, "Exiting.");
    return;
  }

  void mesh_pad_model::tree_dump(std::ostream & out_,
                                 const std::string & title_ ,
                                 const std::string & indent_,
                                 bool inherit_) const
  {
    std::string indent;
    if (! indent_.empty()) indent = indent_;
    i_model::tree_dump(out_, title_, indent, true);

    {
      out_ << indent << datatools::i_tree_dumpable::tag
           << "Pad : " << _pad_.get_strip_id() << ' ' << _pad_.get_pad_id() << " (" << _pad_.get_label() << ')' << std::endl;
    }

    {
      out_ << indent << datatools::i_tree_dumpable::tag
           << "Material : " << get_material_name() << std::endl;
    }

    {
      out_ << indent << datatools::i_tree_dumpable::tag
           << "X-shift : " << _x_shift_ / CLHEP::mm << std::endl;
    }

    {
      out_ << indent << datatools::i_tree_dumpable::tag
           << "Y-shift : " << _y_shift_ / CLHEP::mm << std::endl;
    }

    {
      out_ << indent << datatools::i_tree_dumpable::tag
           << "Source material : " << get_source_material_name() << std::endl;
    }
    {
      out_ << indent << datatools::i_tree_dumpable::tag
           << "Film : " << (_pad_.has_film() ? "yes" : "no") << std::endl;
    }

    if (_pad_.has_film()) {
      out_ << indent << datatools::i_tree_dumpable::tag
           << "Film material : " << get_film_material_name() << std::endl;
    }

    {
      // Items:
      out_ << indent << datatools::i_tree_dumpable::tag
           << "Internal items : " << _internals_.get_items().size() << std::endl;
    }

    {
      out_ << indent << datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Solid : " << std::endl;
      {
        std::ostringstream indent_oss;
        indent_oss << indent;
        indent_oss << datatools::i_tree_dumpable::inherit_skip_tag(inherit_);
        _solid_.tree_dump(out_, "", indent_oss.str());
      }
    }

    return;
  }

} // namespace snrs 



// OCD support for class '::snrs::mesh_pad_model' :
DOCD_CLASS_IMPLEMENT_LOAD_BEGIN(::snrs::mesh_pad_model, ocd_)
{
  ocd_.set_class_name("snrs::mesh_pad_model");
  ocd_.set_class_description("A geometry model implementing a realistic source pad wrapped with films");
  ocd_.set_class_library("snrs");

  // Inherit the OCD support from the parent class:
  geomtools::i_model::init_ocd(ocd_);

  {
    datatools::configuration_property_description & cpd = ocd_.add_configuration_property_info();
    cpd.set_name_pattern("strip_id")
      .set_terse_description("The strip ID")
      .set_traits(datatools::TYPE_INTEGER)
      .set_mandatory(true)
      .add_example("Strip ID #34::             \n"
                   "                           \n"
                   "   strip_id : integer = 34 \n"
                   "                           \n"
                   )
      ;
  }

  {
    datatools::configuration_property_description & cpd = ocd_.add_configuration_property_info();
    cpd.set_name_pattern("length_unit")
      .set_terse_description("The length unit symbol")
      .set_traits(datatools::TYPE_STRING)
      .set_mandatory(false)
      .set_default_value_string("mm")
      .set_long_description("This property set the symbol of the default length\n"
                            "unit.                                             \n"
                            )
      .add_example("Using cm::                                       \n"
                   "                                                 \n"
                   "   length_unit : string = \"cm\"                 \n"
                   "                                                 \n"
                   )
      ;
  }

  {
    datatools::configuration_property_description & cpd = ocd_.add_configuration_property_info();
    cpd.set_name_pattern("material.ref")
      .set_terse_description("The label of the material the mother volume is made of")
      .set_traits(datatools::TYPE_STRING)
      .set_mandatory(false)
      .add_example("Using 'tracking_gas'::                         \n"
                   "                                               \n"
                   "   material.ref : string = \"tracking_gas\"    \n"
                   "                                               \n"
                   )
      ;
  }

  {
    datatools::configuration_property_description & cpd = ocd_.add_configuration_property_info();
    cpd.set_name_pattern("source_material.ref")
      .set_terse_description("The label of the material the source pad is made of")
      .set_traits(datatools::TYPE_STRING)
      .set_mandatory(false)
      .add_example("Using 'copper'::                               \n"
                   "                                               \n"
                   "   source_material.ref : string = \"copper\"   \n"
                   "                                               \n"
                   )
      ;
  }

  {
    datatools::configuration_property_description & cpd = ocd_.add_configuration_property_info();
    cpd.set_name_pattern("film_material.ref")
      .set_terse_description("The label of the material the wrapping films are made of")
      .set_traits(datatools::TYPE_STRING)
      .set_mandatory(false)
      .add_example("Using 'mylar'::                             \n"
                   "                                            \n"
                   "   film_material.ref : string = \"mylar\"   \n"
                   "                                            \n"
                   )
      ;
  }

  {
    datatools::configuration_property_description & cpd = ocd_.add_configuration_property_info();
    cpd.set_name_pattern("replicated.label")
      .set_terse_description("The label used to name the daughter volumes")
      .set_traits(datatools::TYPE_STRING)
      .set_mandatory(false)
      .set_default_value_string("replicated")
      .add_example("Using an alternative label::                      \n"
                   "                                                  \n"
                   "   replicated.label : string = \"go_round_items\" \n"
                   "                                                  \n"
                   )
      ;
  }

  // Add support for internal/daughter volumes:
  geomtools::MWIM::init_ocd(ocd_);

  ocd_.set_configuration_hints("This model is configured through a configuration file that \n"
                               "uses the format of 'datatools::properties' setup file.     \n"
                               "                                                           \n"
                               "Example: ::                                                \n"
                               "                                                           \n"
                               "  strip_id                   : integer  = 34               \n"
                               "                                                           \n"
                               );

  ocd_.set_validation_support(true);
  ocd_.lock();
  return;
}
DOCD_CLASS_IMPLEMENT_LOAD_END()
DOCD_CLASS_SYSTEM_REGISTRATION(::snrs::mesh_pad_model, "snrs::mesh_pad_model")

