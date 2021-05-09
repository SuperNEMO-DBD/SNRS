

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

  internal_item.labels             : string[3] = "strip_34" "strip_33" "strip_32"
  internal_item.model.strip_32     : string = "snemo_strip_32.realistic.model"
  internal_item.placement.strip_32 : string = "0 290 0 (mm) "
  internal_item.model.strip_33     : string = "snemo_strip_33.realistic.model"
  internal_item.placement.strip_33 : string = "0 136.5 0 (mm) "
  internal_item.model.strip_34     : string = "snemo_strip_34.realistic.model"
  internal_item.placement.strip_34 : string = "0 0 0 (mm) "

  mapping.daughter_id.strip_32 : string = "[source_strip:strip=32]"
  mapping.daughter_id.strip_33 : string = "[source_strip:strip=33]"
  mapping.daughter_id.strip_34 : string = "[source_strip:strip=34]"


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