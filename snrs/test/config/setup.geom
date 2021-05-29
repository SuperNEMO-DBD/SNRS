[name="snemo_strip_copper.fake.model" type="geomtools::simple_shaped_model"]

  #@config The SuperNEMO source submodule
  shape_type   : string = "box"
  x            : real as length = 200.0 um
  y            : real as length = 125.5 mm
  z            : real as length = 2500.0 mm
  material.ref : string = "copper"

[name="snemo_strip_lapp.fake.model" type="geomtools::simple_shaped_model"]

  #@config The SuperNEMO source submodule
  shape_type   : string = "box"
  x            : real as length = 250.0 um
  y            : real as length = 135.5 mm
  z            : real as length = 2700.0 mm
  material.ref : string = "mix_selenium"

[name="snemo_strip_flat_itep.fake.model" type="geomtools::simple_shaped_model"]

  #@config The SuperNEMO source submodule
  shape_type   : string = "box"
  x            : real as length = 250.0 um
  y            : real as length = 135.5 mm
  z            : real as length = 2700.0 mm
  material.ref : string = "mix_selenium"


####################
# Source submodule #
#
[name="snemo_source_submodule.model" type="geomtools::simple_shaped_model"]
  #@config The SuperNEMO source submodule
  shape_type   : string = "box"
  x            : real as length = 58.0 mm
  y            : real as length = 5400.0 mm
  z            : real as length = 3900.0 mm
  material.ref : string = "tracking_gas"

  internal_item.labels : string[36] = \
    "strip_0" \
    "strip_1" \
    "strip_2" \
    "strip_3" \
    "strip_4" "strip_5" "strip_6" "strip_7" \
    "strip_8"  "strip_9"  \
    "strip_10" "strip_11" "strip_12" "strip_13" \
    "strip_14" "strip_15" \
    "strip_16" "strip_17" "strip_18" "strip_19" \
    "strip_20" \
    "strip_21" "strip_22" "strip_23" \
    "strip_24" "strip_25" "strip_26" \
    "strip_27" "strip_28" "strip_31" \
    "strip_32" "strip_33" "strip_34" \
    "strip_29" "strip_30" \
    "strip_35"

  internal_item.model.strip_0      : string = "snemo_strip_copper.fake.model"
  internal_item.placement.strip_0  : string = "0 -2434.5 0 (mm) "

  internal_item.model.strip_1      : string = "snemo_strip_lapp.fake.model"
  internal_item.placement.strip_1  : string = "0 -2303.25 0 (mm) "

  internal_item.model.strip_2      : string = "snemo_strip_flat_itep.fake.model"
  internal_item.placement.strip_2  : string = "0 -2166.75 0 (mm) "

  internal_item.model.strip_3      : string = "snemo_strip_3.realistic.model"
  internal_item.placement.strip_3  : string = "0 -2013.25 0 (mm) "

  internal_item.model.strip_4      : string = "snemo_strip_lapp.fake.model"
  internal_item.placement.strip_4  : string = "0 -1876.75 0 (mm) "

  internal_item.model.strip_5      : string = "snemo_strip_lapp.fake.model"
  internal_item.placement.strip_5  : string = "0 -1740.25 0 (mm) "

  internal_item.model.strip_6      : string = "snemo_strip_lapp.fake.model"
  internal_item.placement.strip_6  : string = "0 -1603.75 0 (mm) "

  internal_item.model.strip_7      : string = "snemo_strip_lapp.fake.model"
  internal_item.placement.strip_7  : string = "0 -1467.25 0 (mm) "

  internal_item.model.strip_8      : string = "snemo_strip_8.realistic.model"
  internal_item.placement.strip_8  : string = "0 -1330.75 0 (mm) "

  internal_item.model.strip_9      : string = "snemo_strip_9.realistic.model"
  internal_item.placement.strip_9  : string = "0 -1177.25 0 (mm) "

  internal_item.model.strip_10      : string = "snemo_strip_lapp.fake.model"
  internal_item.placement.strip_10  : string = "0 -1040.75 0 (mm) "

  internal_item.model.strip_11      : string = "snemo_strip_lapp.fake.model"
  internal_item.placement.strip_11  : string = "0 -904.25 0 (mm) "

  internal_item.model.strip_12      : string = "snemo_strip_lapp.fake.model"
  internal_item.placement.strip_12  : string = "0 -767.75 0 (mm) "

  internal_item.model.strip_13      : string = "snemo_strip_lapp.fake.model"
  internal_item.placement.strip_13  : string = "0 -631.25 0 (mm) "

  internal_item.model.strip_14     : string = "snemo_strip_14.realistic.model"
  internal_item.placement.strip_14 : string = "0 -494.75 0 (mm) "

  internal_item.model.strip_15     : string = "snemo_strip_15.realistic.model"
  internal_item.placement.strip_15 : string = "0 -341.25 0 (mm) "

  internal_item.model.strip_16      : string = "snemo_strip_lapp.fake.model"
  internal_item.placement.strip_16  : string = "0 -204.75 0 (mm) "

  internal_item.model.strip_17      : string = "snemo_strip_lapp.fake.model"
  internal_item.placement.strip_17  : string = "0 -68.25 0 (mm) "

  internal_item.model.strip_18      : string = "snemo_strip_lapp.fake.model"
  internal_item.placement.strip_18  : string = "0 68.25 0 (mm) "

  internal_item.model.strip_19      : string = "snemo_strip_lapp.fake.model"
  internal_item.placement.strip_19  : string = "0 204.75 0 (mm) "

  internal_item.model.strip_20     : string = "snemo_strip_20.realistic.model"
  internal_item.placement.strip_20 : string = "0 341.25 0 (mm) "

  internal_item.model.strip_21     : string = "snemo_strip_21.realistic.model"
  internal_item.placement.strip_21 : string = "0 494.75 0 (mm) "

  internal_item.model.strip_22     : string = "snemo_strip_22.realistic.model"
  internal_item.placement.strip_22 : string = "0 631.25 0 (mm) "

  internal_item.model.strip_23     : string = "snemo_strip_23.realistic.model"
  internal_item.placement.strip_23 : string = "0 767.75 0 (mm) "

  internal_item.model.strip_24     : string = "snemo_strip_24.realistic.model"
  internal_item.placement.strip_24 : string = "0 904.25 0 (mm) "

  internal_item.model.strip_25     : string = "snemo_strip_25.realistic.model"
  internal_item.placement.strip_25 : string = "0 1040.75 0 (mm) "

  internal_item.model.strip_26     : string = "snemo_strip_26.realistic.model"
  internal_item.placement.strip_26 : string = "0 1177.25 0 (mm) "

  internal_item.model.strip_27     : string = "snemo_strip_27.realistic.model"
  internal_item.placement.strip_27 : string = "0 1330.75 0 (mm) "

  internal_item.model.strip_28     : string = "snemo_strip_28.realistic.model"
  internal_item.placement.strip_28 : string = "0 1467.25 0 (mm) "

  internal_item.model.strip_29      : string = "snemo_strip_lapp.fake.model"
  internal_item.placement.strip_29  : string = "0 1603.75 0 (mm) "

  internal_item.model.strip_30      : string = "snemo_strip_lapp.fake.model"
  internal_item.placement.strip_30  : string = "0 1740.25 0 (mm) "

  internal_item.model.strip_31     : string = "snemo_strip_31.realistic.model"
  internal_item.placement.strip_31 : string = "0 1876.75 0 (mm) "
  
  internal_item.model.strip_32     : string = "snemo_strip_32.realistic.model"
  internal_item.placement.strip_32 : string = "0 2013.25 0 (mm) "

  internal_item.model.strip_33     : string = "snemo_strip_33.realistic.model"
  internal_item.placement.strip_33 : string = "0 2166.75 0 (mm) "

  internal_item.model.strip_34     : string = "snemo_strip_34.realistic.model"
  internal_item.placement.strip_34 : string = "0 2303.25 0 (mm) "

  internal_item.model.strip_35     : string = "snemo_strip_copper.fake.model"
  internal_item.placement.strip_35 : string = "0 2434.5 0 (mm) "

  mapping.daughter_id.strip_0  : string = "[source_strip:strip=0]"
  mapping.daughter_id.strip_1  : string = "[source_strip:strip=1]"
  mapping.daughter_id.strip_2  : string = "[source_strip:strip=2]"
  mapping.daughter_id.strip_3  : string = "[source_strip:strip=3]"
  mapping.daughter_id.strip_4  : string = "[source_strip:strip=4]"
  mapping.daughter_id.strip_5  : string = "[source_strip:strip=5]"
  mapping.daughter_id.strip_6  : string = "[source_strip:strip=6]"
  mapping.daughter_id.strip_7  : string = "[source_strip:strip=7]"
  mapping.daughter_id.strip_8  : string = "[source_strip:strip=8]"
  mapping.daughter_id.strip_9  : string = "[source_strip:strip=9]"
  mapping.daughter_id.strip_10 : string = "[source_strip:strip=10]"
  mapping.daughter_id.strip_11 : string = "[source_strip:strip=11]"
  mapping.daughter_id.strip_12 : string = "[source_strip:strip=12]"
  mapping.daughter_id.strip_13 : string = "[source_strip:strip=13]"
  mapping.daughter_id.strip_14 : string = "[source_strip:strip=14]"
  mapping.daughter_id.strip_15 : string = "[source_strip:strip=15]"
  mapping.daughter_id.strip_16 : string = "[source_strip:strip=16]"
  mapping.daughter_id.strip_17 : string = "[source_strip:strip=17]"
  mapping.daughter_id.strip_18 : string = "[source_strip:strip=18]"
  mapping.daughter_id.strip_19 : string = "[source_strip:strip=19]"
  mapping.daughter_id.strip_20 : string = "[source_strip:strip=20]"
  mapping.daughter_id.strip_21 : string = "[source_strip:strip=21]"
  mapping.daughter_id.strip_22 : string = "[source_strip:strip=22]"
  mapping.daughter_id.strip_23 : string = "[source_strip:strip=23]"
  mapping.daughter_id.strip_24 : string = "[source_strip:strip=24]"
  mapping.daughter_id.strip_25 : string = "[source_strip:strip=25]"
  mapping.daughter_id.strip_26 : string = "[source_strip:strip=26]"
  mapping.daughter_id.strip_27 : string = "[source_strip:strip=27]"
  mapping.daughter_id.strip_28 : string = "[source_strip:strip=28]"
  mapping.daughter_id.strip_29 : string = "[source_strip:strip=29]"
  mapping.daughter_id.strip_30 : string = "[source_strip:strip=30]"
  mapping.daughter_id.strip_31 : string = "[source_strip:strip=31]"
  mapping.daughter_id.strip_32 : string = "[source_strip:strip=32]"
  mapping.daughter_id.strip_33 : string = "[source_strip:strip=33]"
  mapping.daughter_id.strip_34 : string = "[source_strip:strip=34]"
  mapping.daughter_id.strip_35 : string = "[source_strip:strip=35]"


##########
# Module #
#
[name="module.model" type="geomtools::simple_shaped_model"]
  #@config The SuperNEMO module
  shape_type   : string = "box"
  x            : real as length = 950.0 mm
  y            : real as length = 5450.0 mm
  z            : real as length = 3950.0 mm
  material.ref : string = "air"
  
  internal_item.labels : string[1] = "source_submodule""
  internal_item.model.source_submodule     : string  = "snemo_source_submodule.model"
  internal_item.placement.source_submodule : string = "0 0 0 (mm) "
  
  mapping.daughter_id.source_submodule     : string = "[source_submodule]"


#####################
# Experimental hall #
#
[name="hall.model" type="geomtools::simple_shaped_model"]
  #@config The experimental hall
  shape_type   : string = "box"
  x            : real as length = 1000.0 mm
  y            : real as length = 5500.0 mm
  z            : real as length = 4000.0 mm
  material.ref : string = "air"
  
  internal_item.labels : string[1] = "module_0" 
  internal_item.model.module_0     : string = "module.model"
  internal_item.placement.module_0 : string = "0 0 0 (mm) "

  mapping.daughter_id.module_0 : string = "[module:module=0]"

  visibility.color  : string = "cyan"
  

#########
# World #
#
[name="world" type="geomtools::simple_world_model"]
  #@config The top-level world volume
  world.x      : real as length = 1500.0 mm
  world.y      : real as length = 6500.0 mm
  world.z      : real as length = 4500.0 mm
  material.ref : string = "vacuum"

  setup.model  : string = "hall.model"
  setup.x      : real as length = 0.0 mm
  setup.y      : real as length = 0.0 mm
  setup.z      : real as length = 0.0 mm
  setup.phi    : real as angle  = 0.0 degree
  setup.theta  : real as angle  = 0.0 degree

  mapping.daughter_id.setup : string = "[hall:hall=0]"

  visibility.color : string = "grey"


# end