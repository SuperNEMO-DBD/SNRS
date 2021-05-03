#ifndef SNRS_SNGEOM_HPP
#define SNRS_SNGEOM_HPP

// Standard library:
#include <vector>
#include <map>
#include <string>

// Bayeux:
#include <bayeux/datatools/utils.h>
#include <bayeux/datatools/logger.h>

namespace snrs {

  static int INVALID_STRIP_ID = -1;
  static int ANY_STRIP_ID = -2;
  static int INVALID_PAD_ID = -1;
  static int ANY_PAD_ID = -2;
  
  struct block
  {
    block(int id_, double ymin_, double ymax_);
    double get_ycenter() const;
    double get_width() const;
    void print(std::ostream & out_) const;
  
    int    id      = -1;                        ///< Block ID
    double ymin    = datatools::invalid_real(); ///< Y min
    double ymax    = datatools::invalid_real(); ///< Y max
    double ycenter = datatools::invalid_real(); ///< Y center

    friend std::ostream & operator<<(std::ostream & out_, const block & blk_);
  
  };

  enum strip_geom_t
    {
     STRIP_GEOM_COPPER = 0,
     STRIP_GEOM_ITEP   = 1,
     STRIP_GEOM_LAPP   = 2
    };

  struct strip_model_t
  {
    std::string  name;                     // Name
    strip_geom_t geom = STRIP_GEOM_COPPER; // Type of geometry
    double       thickness = 0.0;          // Thickness X
    double       width = 0.0;              // Width Y
    double       height = 0.0;             // Height Z
    double       total_mass = 0.0;         // Total mass
    double       Se82_mass = 0.0;          // Se82 mass
    bool         front = false;            // Front
    double       Se82_enrichment = 0.0;    // Se82 enrichment
    double       activity_Tl208 = 0.0;     // Activity Tl208
    double       activity_Bi214 = 0.0;     // Activity Bi214
    int          position = -1;            // Position
    double       density = 0.0;            // Density
    std::string  material;                 // Material name
    bool         has_film = false;         // Wrapping film
    double       film_thickness = -1.0;    // Thickness of the wrapping film (is any)
    std::string  film_material;            // Film material name
  };

  typedef std::map<std::string, strip_model_t> strip_model_db_t;
 
  struct strip
  {
    strip() = default;
    strip(int id_,
          int blkid_,
          const strip_model_t & model_,
          double ycenter_,
          double zcenter_);
    double get_ycenter() const;
    double get_zcenter() const;
    double ymin() const;
    double ymax() const;
    double zmin() const;
    double zmax() const;
    void print(std::ostream & out_) const;
    const strip_model_t & get_model() const;
  
    int id    = -1;
    int blkid = -1; // block id
    const strip_model_t * model = nullptr;
    double ycenter = 0.0;
    double zcenter = 0.0;
  
  };

  /// \brief Simplified geometry informations about the SuperNEMO source foils
  class sngeom
  {
  public:
  
    sngeom(datatools::logger::priority logging_ = datatools::logger::PRIO_NOTICE);

    const std::vector<block> & get_blocks() const;

    const std::vector<double> & get_strip_ypos() const;

    const strip_model_db_t & get_strip_models() const;

    const std::vector<strip> & get_strips() const;

  private:

    void _init_foil_models_();

  private:

    datatools::logger::priority logging = datatools::logger::PRIO_NOTICE;  
    std::vector<block>  _blocks_;
    std::vector<double> _strip_ypos_;
    strip_model_db_t    _strip_model_db_;
    std::vector<strip>  _strips_;
 
  };
  
}// namespace snrs 

#endif // SNRS_SNGEOM_HPP
