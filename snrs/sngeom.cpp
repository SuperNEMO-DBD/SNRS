// Ourselves:
#include <snrs/sngeom.hpp>

// Standard library:
#include <list>
#include <iostream>

// Bayeux:
#include <bayeux/datatools/clhep_units.h>

namespace snrs {
                
  block::block(int id_, double ymin_, double ymax_)
    : id(id_)
    , ymin(ymin_)
    , ymax(ymax_)
  {
    ycenter = 0.5 * (ymin+ymax);
  }  

  double block::get_ycenter() const
  {
    return ycenter;
  }

  double block::get_width() const
  {
    return ymax - ymin;
  }

  void block::print(std::ostream & out_) const
  {
    out_ << "|-- Block ID : " << id << '\n';
    out_ << "|-- Y-min    : " << ymin / CLHEP::mm << " mm" << '\n';
    out_ << "|-- Y-max    : " << ymax / CLHEP::mm << " mm"  << '\n';
    out_ << "`-- Y-center : " << ycenter / CLHEP::mm << " mm"  << '\n';
  }

  std::ostream & operator<<(std::ostream & out_, const block & blk_)
  {
    out_ << "(id=" << blk_.id << ", ymin=" << blk_.ymin << ", ymax=" << blk_.ymax << ")"; 
    return out_;
  }
  
  strip::strip(int id_,
               int blkid_,
               const strip_model_t & model_,
               double ycenter_,
               double zcenter_)
    : id(id_)
    , blkid(blkid_)
    , model(&model_)
    , ycenter(ycenter_)
    , zcenter(zcenter_)
  {
  }

  double strip::get_ycenter() const
  {
    return ycenter;
  }

  double strip::get_zcenter() const
  {
    return zcenter;
  }

  double strip::ymin() const
  {
    return ycenter - 0.5 * model->width;
  }

  double strip::ymax() const
  {
    return ymin() + model->width;
  }

  double strip::zmin() const
  {
    return zcenter - 0.5 * model->height;
  }

  double strip::zmax() const
  {
    return zmin() + model->height;
  }

  const strip_model_t & strip::get_model() const
  {
    return *model;
  }

  void strip::print(std::ostream & out_) const
  {
    out_ << "|-- Strip ID : " << id << " (position)" << '\n';
    out_ << "|-- Block ID : " << blkid << '\n';
    out_ << "|-- Model    : ";
    if (model) {
      out_ << "'" << model->name << "'";
    } else {
      out_ << "<none>";
    }
    out_ << '\n';
    out_ << "|-- Y-center  : " << ycenter / CLHEP::mm << " mm" << '\n';
    out_ << "|-- Y-min     : " << ymin() / CLHEP::mm << " mm" << '\n';
    out_ << "|-- Y-max     : " << ymax() / CLHEP::mm << " mm" << '\n';
    out_ << "`-- Z-center  : " << zcenter / CLHEP::mm << " mm" << '\n';
  }

  void sngeom::_init_foil_models_()
  {
    DT_LOG_TRACE_ENTERING(logging);
    typedef std::tuple<std::string, // 0 : Name
                       std::string, // 1 : Fabrication process
                       std::string, // 2 : Dominant 82Se flavour
                       double,      // 3 : Total Se82 mass (g)
                       double,      // 4 : Total strip mass (g)
                       int,         // 5 : Position in the frame (reverse + 1)
                       bool,        // 6 : Back/front
                       double,      // 7 : 82Se enrichment
                       double,      // 8 : 208Tl activity μBq/kg (complete foil)
                       double,      // 9 : 214Bi activity μBq/kg (complete foil)
                       double       // 10 : thickness μm
                       > foil_model_record_t;
    // Source model list:
    typedef std::list<foil_model_record_t> sml_t;
    // A.Jeremie, DocDB-4705-v1, Slide 9 (wrong positioning)
    // A.Jeremie, DocDB-4457-v5 (good positioning)
    sml_t sml = { /* 0            1               2              3        4       5   6        7     8      9      10  */   
                 {"Copper-2"    , "LAL/LSM"     , "none"        ,   0.0 ,   0.0 , 35, false,   0.0 ,   0.0,    0.0, 74.0  },
                 {"LAPP-ITEP-4" , "backing film", "ground Tomsk", 212.11, 252.43, 34, true ,  96.1 , 131.0,  525.0, 230.0 },
                 {"LAPP-ITEP-6" , "backing film", "ground Tomsk", 202.13, 241.32, 33, false,  96.1 , 131.0,  525.0, 220.0 },
                 {"LAPP-ITEP-5" , "backing film", "ground Tomsk", 203.31, 242.65, 32, true ,  96.1 , 131.0, 1374.0, 210.0 },
                 {"LAPP-ITEP-1" , "backing film", "John Baker"  , 185.19, 222.52, 31, false,  96.9 , 106.0,  525.0, 200.0 },
                 {"LAPP-6"      , "raw Mylar"   , "Jared/Tomsk" , 204.12, 243.75, 30, true ,  96.71, 131.0, 1374.0, -1    },
                 {"LAPP-15"     , "raw Mylar"   , "French"      , 177.71, 214.16, 29, false,  96.65, 106.0,  525.0, -1    },
                 {"LAPP-ITEP-21", "backing film", "mixed"       , 196.25, 234.81, 28, true ,  97.97, 131.0,  290.0, 280.0 },
                 {"ITEP-1"      , "backing film", "NEMO-3"      , 142.8 , 170.4 , 27, false,  96.92,  24.0,  290.0, 200.0 },
                 {"ITEP-2"      , "backing film", "NEMO-3"      , 132.9 , 159.5 , 26, true ,  96.92,  24.0,  290.0, 190.0 },
                 {"ITEP-3"      , "backing film", "NEMO-3"      , 132.4 , 159.2 , 25, false,  96.92,  24.0,  290.0, 200.0 },
                 {"ITEP-4"      , "backing film", "NEMO-3"      , 130.9 , 157.1 , 24, true ,  96.92,  24.0,  290.0, 200.0 },
                 {"ITEP-5"      , "backing film", "NEMO-3"      , 221.5 , 255.7 , 23, false,  98.31,  24.0,  290.0, 300.0 },
                 {"ITEP-6"      , "backing film", "NEMO-3"      , 197.1 , 230.0 , 22, true ,  98.31,  24.0,  290.0, 280.0 },
                 {"ITEP-7"      , "backing film", "NEMO-3"      , 223.5 , 257.6 , 21, false,  99.92,  24.0,  290.0, 300.0 },
                 {"ITEP-8"      , "backing film", "NEMO-3"      , 224.5 , 259.0 , 20, true ,  99.6 ,  24.0,  290.0, 280.0 },
                 {"LAPP-7"      , "raw Mylar"   , "Dubna"       , 172.6 , 208.85, 19, false,  99.88,  22.0,  595.0, -1    },
                 {"LAPP-12"     , "raw Mylar"   , "Dubna"       , 183.46, 220.71, 18, true ,  99.88,  22.0,  595.0, -1    },
                 {"LAPP-18"     , "raw Mylar"   , "French"      , 141.78, 174.18, 17, false,  96.65, 106.0, 1374.0, -1    },
                 {"LAPP-10"     , "raw Mylar"   , "Dubna"       , 170.09, 205.75, 16, true ,  99.88,  22.0,  595.0, -1    },
                 {"ITEP-9"      , "backing film", "NEMO-3"      , 214.0 , 247.5 , 15, false,  97.0 ,  24.0,  290.0, 280.0 },
                 {"ITEP-10"     , "backing film", "NEMO-3"      , 192.6 , 224.8 , 14, true ,  97.5 ,  24.0,  290.0, 250.0 },
                 {"LAPP-11"     , "raw Mylar"   , "Dubna"       , 168.48, 204.06, 13, false,  99.88,  22.0,  595.0, -1    },
                 {"LAPP-19"     , "raw Mylar"   , "Dubna"       , 183.64, 220.13, 12, true ,  99.88,  22.0,  595.0, -1    },
                 {"LAPP-17"     , "raw Mylar"   , "Dubna"       , 182.51, 218.92, 11, false,  99.88,  22.0,  595.0, -1    },
                 {"LAPP-16"     , "raw Mylar"   , "French"      , 177.53, 214.73, 10, true ,  97.13, 106.0, 1374.0, -1    },
                 {"ITEP-11"     , "backing film", "NEMO-3"      , 173.5 , 204.2 ,  9, false,  98.0 ,  24.0,  290.0, 240.0 },
                 {"LAPP-ITEP-2" , "backing film", "John Baker"  , 213.65, 254.14,  8, true ,  96.9 , 106.0, 1374.0, 220.0 },
                 {"LAPP-13"     , "raw Mylar"   , "French"      , 171.16, 207.63,  7, false,  96.65, 106.0, 1374.0, -1    },
                 {"LAPP-14"     , "raw Mylar"   , "French"      , 180.45, 217.52,  6, true ,  96.65, 106.0, 1374.0, -1    },
                 {"LAPP-20"     , "raw Mylar"   , "leftovers"   , 193.67, 232.07,  5, false,  97.94, 131.0,  525.0, -1    },
                 {"LAPP-9"      , "raw Mylar"   , "Tomsk/Dubna" , 176.0 , 212.36,  4, true ,  97.5 , 131.0,  525.0, -1    },
                 {"LAPP-ITEP-3" , "backing film", "John Baker"  , 204.06, 243.49,  3, false,  96.9 , 106.0, 1374.0, 240.0 },
                 {"LAPP-ITEP-7" , "backing film", "ground Tomsk", 185.17, 222.5 ,  2, true ,  96.1 , 131.0,  525.0, 270.0 },
                 {"LAPP-8"      , "raw Mylar"   , "Tomsk"       , 183.39, 220.54,  1, false,  96.1 , 131.0,  525.0, -1    },
                 {"Copper-1"    , "LAL/LSM"     , "none"        ,   0.0 ,   0.0 ,  0, true ,   0.0 ,   0.0,    0.0, 74.0  }
    };
    typedef std::map<std::string,std::string> sematmap_t;
    sematmap_t smm = { 
                      {"LAPP-ITEP-1"  , "snemo::se82_enriched96.9_pva0.1000_density2.823"  },
                      {"LAPP-ITEP-2"  , "snemo::se82_enriched96.9_pva0.1000_density2.960"  },
                      {"LAPP-ITEP-3"  , "snemo::se82_enriched96.9_pva0.1000_density2.592"  },
                      {"LAPP-ITEP-4"  , "snemo::se82_enriched96.1_pva0.1000_density2.811"  },
                      {"LAPP-ITEP-5"  , "snemo::se82_enriched96.1_pva0.1000_density2.951"  },
                      {"LAPP-ITEP-6"  , "snemo::se82_enriched96.1_pva0.1000_density2.800"  },
                      {"LAPP-ITEP-7"  , "snemo::se82_enriched96.1_pva0.1000_density2.258"  },
                      {"LAPP-ITEP-21" , "snemo::se82_enriched97.0_pva0.1000_density2.137"  },
                      {"ITEP-1"       , "snemo::se82_enriched96.92_pva0.0787_density2.122" },
                      {"ITEP-2"       , "snemo::se82_enriched96.92_pva0.0780_density2.081" },
                      {"ITEP-3"       , "snemo::se82_enriched96.92_pva0.0790_density1.971" },
                      {"ITEP-4"       , "snemo::se82_enriched96.92_pva0.0783_density1.949" },
                      {"ITEP-5"       , "snemo::se82_enriched98.31_pva0.0786_density2.198" },
                      {"ITEP-6"       , "snemo::se82_enriched98.31_pva0.0820_density2.104" },
                      {"ITEP-7"       , "snemo::se82_enriched99.92_pva0.0771_density2.217" },
                      {"ITEP-8"       , "snemo::se82_enriched99.6_pva0.0784_density2.388"  },
                      {"ITEP-9"       , "snemo::se82_enriched97_pva0.0801_density2.276"    },
                      {"ITEP-10"      , "snemo::se82_enriched97.5_pva0.0810_density2.300"  },
                      {"ITEP-11"      , "snemo::se82_enriched98_pva0.0820_density2.160"    }
    };
    for (const auto & lStrmod : sml) {
      strip_model_t strMod;
      strMod.name = std::get<0>(lStrmod);
      if (strMod.name.substr(0, 7) == "Copper-") {
        strMod.geom      = STRIP_GEOM_COPPER;
        strMod.thickness = 74.0 * CLHEP::micrometer;
        strMod.width     = 125.0 * CLHEP::mm;
        strMod.height    = 250.0 * CLHEP::cm;
      } else if (strMod.name.substr(0, 5) == "ITEP-" or strMod.name.substr(0, 10) == "LAPP-ITEP-") {
        // ITEP and LAPP-ITEP:
        strMod.geom      = STRIP_GEOM_ITEP;
        strMod.width     = 135.5 * CLHEP::mm;
        strMod.height    = 270.0 * CLHEP::cm;
        strMod.thickness = 241.0 * CLHEP::micrometer;
        strMod.material  = smm.find(strMod.name)->second;
      } else {
        // LAPP
        strMod.geom      = STRIP_GEOM_LAPP;
        strMod.thickness = 344.0 * CLHEP::micrometer;
        strMod.width     = 135.5 * CLHEP::mm;
        strMod.height    = 270.0 * CLHEP::cm;
      }
      strMod.Se82_mass       = std::get<3>(lStrmod) * CLHEP::gram;
      strMod.total_mass      = std::get<4>(lStrmod) * CLHEP::gram;
      strMod.position        = std::get<5>(lStrmod);
      strMod.front           = std::get<6>(lStrmod);
      strMod.Se82_enrichment = std::get<7>(lStrmod);
      strMod.activity_Tl208  = std::get<8>(lStrmod) * CLHEP::becquerel / CLHEP::kg;
      strMod.activity_Bi214  = std::get<9>(lStrmod) * CLHEP::becquerel / CLHEP::kg;
      strMod.density         = datatools::invalid_real();
      strMod.has_film      = false;
      if (strMod.geom == STRIP_GEOM_ITEP) {
        strMod.thickness     = std::get<10>(lStrmod) * CLHEP::micrometer;
        strMod.has_film      = true;
        strMod.film_thickness = 12.0 * CLHEP::micrometer;
        strMod.film_material  = "basic::mylar";
      } else if (strMod.geom == STRIP_GEOM_COPPER) {
        strMod.thickness     = std::get<10>(lStrmod) * CLHEP::micrometer;
      }
      _strip_model_db_[strMod.name] = strMod;
      
      DT_LOG_NOTICE(logging, "Strip model '" << strMod.name << "' has been added.");
    }
    DT_LOG_TRACE_EXITING(logging);
  }

  sngeom::sngeom(datatools::logger::priority logging_)
  {
 
    static const uint32_t NB_BLOCKS = 7;
    static const uint32_t NB_STRIPS = 36;
    logging = logging_;
    block blk0(0, -2497.0 * CLHEP::mm, -2099.0 * CLHEP::mm);
    block blk1(1, -2081.0 * CLHEP::mm, -1263.0 * CLHEP::mm);
    block blk2(2, -1245.0 * CLHEP::mm,  -427.0 * CLHEP::mm);
    block blk3(3,  -409.0 * CLHEP::mm,   409.0 * CLHEP::mm);
    block blk4(4,  +427.0 * CLHEP::mm, +1245.0 * CLHEP::mm);
    block blk5(5, +1263.0 * CLHEP::mm, +2081.0 * CLHEP::mm);
    block blk6(6, +2099.0 * CLHEP::mm, +2497.0 * CLHEP::mm);
    _blocks_.push_back(blk0);
    _blocks_.push_back(blk1);
    _blocks_.push_back(blk2);
    _blocks_.push_back(blk3);
    _blocks_.push_back(blk4);
    _blocks_.push_back(blk5);
    _blocks_.push_back(blk6);
    DT_LOG_NOTICE(logging, "6 blocks have been added.");
    for (int blkId = 0; blkId < NB_BLOCKS; blkId++) {
      DT_LOG_NOTICE(logging, "Block #" << blkId << " = " << _blocks_[blkId]);
    }

    _init_foil_models_();
    
    std::vector<double> & strip_pos = _strip_ypos_;
    strip_pos.clear();
    strip_pos.assign(NB_STRIPS, 0.0);
    {
      DT_LOG_NOTICE(logging, "Computing strip positions...");
      double copper_w = 125.0 * CLHEP::mm;
      double other_w  = 135.5 * CLHEP::mm;
      double gap      = 1.0 * CLHEP::mm;
      int strId = 0;
      int blkId = 0;  
      strip_pos[strId] = _blocks_[blkId].ymin + 0.5 * copper_w;
      ++strId; // 1
      strip_pos[strId] = strip_pos[strId - 1] + 0.5 * (copper_w + other_w) + gap;
      ++strId; // 2
      strip_pos[strId] = strip_pos[strId - 1] + gap +  other_w;

      blkId++; // 1
      ++strId; // 3
      strip_pos[strId] = _blocks_[blkId].ymin + 0.5 * other_w;
      for (int i = 1; i < 6; i++) {
        ++strId; // 4, 5, 6, 7, 8
        strip_pos[strId] = strip_pos[strId - 1] + gap + other_w;
      }

      blkId++; // 2
      ++strId; // 9
      strip_pos[strId] = _blocks_[blkId].ymin + 0.5 * other_w;
      for (int i = 1; i < 6; i++) {
        ++strId; // 10, 11, 12, 13, 14
        strip_pos[strId] = strip_pos[strId - 1] + gap + other_w;
      }

      blkId++; // 3
      ++strId; // 15
      strip_pos[strId] = _blocks_[blkId].ymin + 0.5 * other_w;
      for (int i = 1; i < 6; i++) {
        ++strId; // 16, 17, 18, 19, 20
        strip_pos[strId] = strip_pos[strId - 1] + gap + other_w;
      }

      blkId++; // 4
      ++strId; // 21
      strip_pos[strId] = _blocks_[blkId].ymin + 0.5 * other_w;
      for (int i = 1; i < 6; i++) {
        ++strId; // 22, 23, 24, 25, 26
        strip_pos[strId] = strip_pos[strId - 1] + gap + other_w;
      }

      blkId++; // 5
      ++strId; // 22
      strip_pos[strId] = _blocks_[blkId].ymin + 0.5 * other_w;
      for (int i = 1; i < 6; i++) {
        ++strId; // 27, 28, 29, 30, 31, 32
        strip_pos[strId] = strip_pos[strId - 1] + gap + other_w;
      }

      blkId++; // 6
      ++strId; // 33
      strip_pos[strId] = _blocks_[blkId].ymin + 0.5 * other_w;
      ++strId; // 34
      strip_pos[strId] = strip_pos[strId - 1] + other_w + gap;
      ++strId; // 35
      strip_pos[strId] = strip_pos[strId - 1] + 0.5 * (other_w + copper_w) + gap;
  
      for (int strId = 0; strId < 36; strId++) {
        DT_LOG_NOTICE(logging, "Position of strip #" << strId << " = " << strip_pos[strId] / CLHEP::mm << " mm");
      }
    }
    {
      strip s0;
      _strips_.assign(NB_STRIPS, s0);
    }
    for (int strpId = 0; strpId < NB_STRIPS; strpId++) {
      DT_LOG_NOTICE(logging, "Creating strip #" << strpId << " ...");
      int blkId = 0;
      if (strpId >= 3 and strpId <= 8) {
        blkId = 1;
      } else if (strpId >= 9 and strpId <= 14) {
        blkId = 2;
      } else if (strpId >= 15 and strpId <= 20) {
        blkId = 3;
      } else if (strpId >= 21 and strpId <= 26) {
        blkId = 4;
      } else if (strpId >= 27 and strpId <= 32) {
        blkId = 5;
      } else if (strpId >= 33) {
        blkId = 6;
      }
      std::string modName;
      for (auto p : _strip_model_db_) {
        if (p.second.position == strpId) {
          modName = p.second.name;
          break;
        }
      }
      strip s(strpId,
              blkId,
              _strip_model_db_.find(modName)->second,
              strip_pos[strpId],
              0.0 * CLHEP::mm); 
      DT_LOG_NOTICE(logging, "Strip #" << strpId << " :");
      s.print(std::cerr);
      _strips_[strpId] = s;
    }
  
  }
 
  const std::vector<block> & sngeom::get_blocks() const
  {
    return _blocks_;
  }

  const std::vector<double> & sngeom::get_strip_ypos() const
  {
    return _strip_ypos_;
  }

  const strip_model_db_t & sngeom::get_strip_models() const
  {
    return _strip_model_db_;
  }
  
  const std::vector<strip> & sngeom::get_strips() const
  {
    return _strips_;
  }
 
} // namespace snrs 
