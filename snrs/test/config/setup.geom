#####################
# Experimental hall #
#
[name="hall.model" type="geomtools::simple_shaped_model"]
  #@config The experimental hall
  shape_type   : string = "box"
  x            : real as length = 1000.0 mm
  y            : real as length = 1000.0 mm
  z            : real as length = 4000.0 mm
  material.ref : string = "air"
  internal_item.labels : string[3] = "strip_34" "strip_33" "strip_32"
  internal_item.model.strip_34       : string  = "snemo_strip_34_realistic.model"
  internal_item.placement.strip_34   : string  = "0 0 0 (mm) "
  internal_item.model.strip_33       : string  = "snemo_strip_33_realistic.model"
  internal_item.placement.strip_33   : string  = "0 136.5 0 (mm) "
  internal_item.model.strip_32       : string  = "snemo_strip_32_realistic.model"
  internal_item.placement.strip_32   : string  = "0 290 0 (mm) "
  visibility.color  : string = "cyan"
  

[name="world" type="geomtools::simple_world_model"]
  #@config The top-level world volume
  material.ref : string = "vacuum"
  world.x      : real as length = 1500.0 mm
  world.y      : real as length = 1500.0 mm
  world.z      : real as length = 4500.0 mm
  setup.model  : string = "hall.model"
  setup.x      : real as length = 0.0 mm
  setup.y      : real as length = 0.0 mm
  setup.z      : real as length = 0.0 mm
  setup.phi    : real as angle  = 0.0 degree
  setup.theta  : real as angle  = 0.0 degree
  visibility.color : string = "grey"
  mapping.daughter_id.setup : string = "[hall:hall=0]"

# end