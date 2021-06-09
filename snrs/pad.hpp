#ifndef SNRS_PAD_HPP
#define SNRS_PAD_HPP

// Standard Library:
#include <vector>
#include <map>

// Bayeux:
#include <bayeux/datatools/utils.h>
#include <bayeux/datatools/clhep_units.h>
#include <bayeux/geomtools/tessellation.h>

// This project:
#include <snrs/vertex3d_id.hpp>
#include <snrs/vertex3d.hpp>
#include <snrs/foil_shaping.hpp>

namespace snrs {

  /**
   *
   *  \code
   *
   *    Z
   *    ^        dy
   *    :nz  
   * nz +-----+-----+--//-+
   *    |     |     |     |
   *    :     :     :     :
   *    |3    |nz+4 |     |
   *  3 +-----+-----+--//-+
   *    |     |     |     |
   *    |2    |nz+3 |     | dz
   *  2 +-----+-----+--//-+
   *    |     |     |     |
   *    |1    |nz+2 |     |
   *  1 +-----+-----+--//-+
   *    |     |     |     |
   *    |0    |nz+1 |     |
   *  0 +-----+-----+--//-+ - - -> Y
   *    0     1   2    ny
   *
   *  \endcode
   */
  class pad
  {
  public:

    enum face_type
      {
       FACE_BACK  = 0,
       FACE_FRONT = 1
      };

   
    struct z_distortion_data_type
    {
      int kz = -1;
      fs::distortion_model_type model = fs::DISTORTION_FLAT;
      fs::elliptic_distortion_data_type   elliptic_distortion;
      fs::polynomial_distortion_data_type polynomial_distortion;
      void reset();
      void store(std::ostream &) const;
      void load(std::istream &);
     };
   
    struct distortion_data_type
    {
      std::vector<z_distortion_data_type> z_distortions;
      void store(std::ostream &) const;
      void load(std::istream &);
      void reset();
    };

    struct tile_id
    {
    public:
      
      static const int32_t INVALID_INDEX = -1;
      static const int32_t ANY_INDEX     = -2;
      static const int32_t PART_DOWN = 0;
      static const int32_t PART_UP   = 1;
 
      tile_id() = default;

      tile_id(int32_t side_, int32_t column_, int32_t row_, int32_t part_);
  
      bool is_valid() const;

      void invalidate();

      bool match(int32_t side_, int32_t column_, int32_t row_, int32_t part_) const;

      bool match(const tile_id & other_) const;
  
      bool operator==(const tile_id & tid_) const;
      
      bool operator<(const tile_id & tid_) const;

      void set_side(int32_t side_);

      void set(int32_t side_, int32_t column_, int32_t row_, int32_t part_);
  
      int32_t get_side() const;
 
      int32_t get_column() const;

      int32_t get_row() const;
 
      int32_t get_part() const;
      
      bool from_string(const std::string & repr_);

      friend std::ostream & operator<<(std::ostream & out_, const pad::tile_id & tid_);

    private:
      
      int32_t _side_   = INVALID_INDEX; ///< [0 : back, 1 : front]
      int32_t _column_ = INVALID_INDEX; ///< [0 : mountain, >0 : towards tunnel]
      int32_t _row_    = INVALID_INDEX; ///< [0 : bottom, >0 : towards top]
      int32_t _part_   = INVALID_INDEX; ///< [0 : down, 1 : up]
      
    };
  
    pad() = default;
    
    ~pad() = default;
 
    pad(const std::string & label_,
        int strip_id_,
        int pad_id_,
        double width_,
        double height_,
        double thickness_,
        uint32_t ny_,
        uint32_t nz_,
        const geomtools::vector_3d & original_position_);

    void make_vertexes();
    void clear_vertexes();
    bool is_valid(const vertex3d_id & vid_) const;
    const vertex3d & get_vertex(const vertex3d_id & vid_) const;
    vertex3d & grab_vertex(const vertex3d_id & vid_);
    const vertex3d & operator[](const vertex3d_id & vid_) const;
    vertex3d & operator[](const vertex3d_id & vid_);
    void print(std::ostream & out_) const;
    void plot(std::ostream & out_) const;
    void store(std::ostream & out_) const;
    void load(std::istream & in_);
    double distance(const vertex3d_id & vid1_, const vertex3d_id & vid2_) const;
    double distance(const pad & other_) const;
    const std::string & get_label() const;
    int get_strip_id() const;
    int get_pad_id() const;
    double get_width() const;
    double get_height() const;
    double get_thickness() const;
    double get_surface() const;
    double get_volume() const; 
    uint32_t get_ny() const;
    uint32_t get_nz() const;
    double get_dy() const;
    double get_dz() const;
    bool has_distortion() const;
    bool has_film() const;
    double get_film_thickness() const;
    void set_film_thickness(double);
    void set_material(const std::string & material_);
    void set_film_material(const std::string & film_material_);

    // Overall convexity (if known)
    enum convexity_type
      {
       CONVEXITY_UNDEF   = 0,
       CONVEXITY_CONVEX  = 1,
       CONVEXITY_CONCAVE = 2
      };
    bool has_convexity() const;
    bool is_concave() const;
    bool is_convex() const;
    void set_convexity(convexity_type);
    
    const std::string & get_material() const;
    const std::string & get_film_material() const;
    const distortion_data_type & get_distortion() const;
    distortion_data_type & grab_distortion();
    const std::vector<std::vector<vertex3d>> & get_vertexes(uint32_t face_) const;
    const geomtools::vector_3d & get_original_position() const;
    const vertex3d & get_back_film_vertex(const vertex3d_id & vid_) const;
    vertex3d & grab_back_film_vertex(const vertex3d_id & vid_);
    const vertex3d & get_front_film_vertex(const vertex3d_id & vid_) const;
    vertex3d & grab_front_film_vertex(const vertex3d_id & vid_);
    const std::vector<std::vector<vertex3d>> & get_back_film_vertexes(uint32_t face_) const;
    const std::vector<std::vector<vertex3d>> & get_front_film_vertexes(uint32_t face_) const;

    // Exported data and utilities for geomtools:

    // struct tile_data
    // {
    //   unnsigned int key; ///< facet's key in the geomtools tessellated solid
    //   double surface = datatools::invalid_real();
    // };
    
    typedef std::map<tile_id, unsigned int> tile_map_type;
    typedef std::map<unsigned int, tile_id> elit_map_type;

    struct mesh_data_type
    {
      geomtools::tessellated_solid solid;
      tile_map_type tile_map;
      elit_map_type reverse_tile_map;
      
      void reset();
      void store_tile_maps(std::ostream & out_) const;
      void load_tile_maps(std::istream & in_);
    };
    
    const mesh_data_type & get_source_mesh() const;
    const mesh_data_type & get_back_film_mesh() const;
    const mesh_data_type & get_front_film_mesh() const;
    mesh_data_type & grab_source_mesh();
    mesh_data_type & grab_back_film_mesh();
    mesh_data_type & grab_front_film_mesh();
    
  private:
  
    // Configuration:
    std::string _label_;
    int      _strip_id_ = -1;
    int      _pad_id_ = 0;
    double   _width_  = datatools::invalid_real();
    double   _height_ = datatools::invalid_real();
    double   _thickness_ = datatools::invalid_real();
    uint32_t _ny_ = 0;
    uint32_t _nz_ = 0;
    std::string _material_;
    geomtools::vector_3d _original_position_;
    distortion_data_type _distortion_;
    convexity_type _convexity_ = CONVEXITY_UNDEF; ///< Overall convexity (if known)
    double   _film_thickness_ = datatools::invalid_real();
    std::string _film_material_;
    
    // Computed:
    double   _dy_ = datatools::invalid_real();
    double   _dz_ = datatools::invalid_real();
    std::vector<std::vector<vertex3d>> _vertexes_[2];
    std::vector<std::vector<vertex3d>> _back_film_vertexes_[2];
    std::vector<std::vector<vertex3d>> _front_film_vertexes_[2];
    mesh_data_type _source_mesh_;
    mesh_data_type _back_film_mesh_;
    mesh_data_type _front_film_mesh_;

  };

} // namespace snrs 
  
#endif // SNRS_PAD_HPP
