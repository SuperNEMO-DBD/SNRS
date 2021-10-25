# -*- mode: conf-unix; -*-
#@description A sample list of geometry ID categories/types
#@key_label   "name"
#@meta_label  "type"

[name="snemo_strip_3_pad.realistic.model" type="snrs::mesh_pad_model"]
strip_id : integer = 3
# #@description Safe shift along the X-axis:
# x_shift : real as length = -1.0 mm
# #@description Safe shift along the Y-axis:
# y_shift : real as length = -1.0 mm
visibility.hidden            : boolean = false
visibility.color             : string = "green"
source.visibility.hidden     : boolean = false
source.visibility.color      : string = "magenta"
back_film.visibility.hidden  : boolean = true
back_film.visibility.color   : string = "cyan"
front_film.visibility.hidden : boolean = true
front_film.visibility.color  : string = "cyan"
mapping.daughter_id.back_film   : string = "[source_pad_film:film=0]"
mapping.daughter_id.source      : string = "[source_pad_bulk]"
mapping.daughter_id.front_film  : string = "[source_pad_film:film=1]"


[name="snemo_strip_3.realistic.model" type="geomtools::simple_shaped_model"]
shape_type   : string = "box"
x            : real as length = 58.0 mm
y            : real as length = 135.5 mm
z            : real as length = 2700.0 mm
material.ref : string = "mix_selenium"
internal_item.labels          : string[1] =  "pad0"
internal_item.model.pad0      : string = "snemo_strip_3_pad.realistic.model"
internal_item.placement.pad0  : string = "0 0 0 (mm) "
mapping.daughter_id.pad0      : string = "[source_pad:pad=0]"


[name="snemo_strip_path_3.realistic.model" type="geomtools::simple_shaped_model"]
shape_type   : string = "box"
x            : real as length = 58.0 mm
y            : real as length = 135.5 mm
z            : real as length = 3000.0 mm
material.ref : string = "tracking_gas"
internal_item.labels           : string[1] =  "strip"
internal_item.model.strip      : string = "snemo_strip_3.realistic.model"
internal_item.placement.strip  : string = "0 0 0 (mm) "
mapping.daughter_id.strip      : string = "[source_strip]"


[name="snemo_strip_8_pad.realistic.model" type="snrs::mesh_pad_model"]
strip_id : integer = 8
# #@description Safe shift along the X-axis:
# x_shift : real as length = -1.0 mm
# #@description Safe shift along the Y-axis:
# y_shift : real as length = -1.0 mm
visibility.hidden            : boolean = false
visibility.color             : string = "green"
source.visibility.hidden     : boolean = false
source.visibility.color      : string = "magenta"
back_film.visibility.hidden  : boolean = true
back_film.visibility.color   : string = "cyan"
front_film.visibility.hidden : boolean = true
front_film.visibility.color  : string = "cyan"
mapping.daughter_id.back_film   : string = "[source_pad_film:film=0]"
mapping.daughter_id.source      : string = "[source_pad_bulk]"
mapping.daughter_id.front_film  : string = "[source_pad_film:film=1]"


[name="snemo_strip_8.realistic.model" type="geomtools::simple_shaped_model"]
shape_type   : string = "box"
x            : real as length = 58.0 mm
y            : real as length = 135.5 mm
z            : real as length = 2700.0 mm
material.ref : string = "mix_selenium"
internal_item.labels          : string[1] =  "pad0"
internal_item.model.pad0      : string = "snemo_strip_8_pad.realistic.model"
internal_item.placement.pad0  : string = "0 0 0 (mm) "
mapping.daughter_id.pad0      : string = "[source_pad:pad=0]"


[name="snemo_strip_path_8.realistic.model" type="geomtools::simple_shaped_model"]
shape_type   : string = "box"
x            : real as length = 58.0 mm
y            : real as length = 135.5 mm
z            : real as length = 3000.0 mm
material.ref : string = "tracking_gas"
internal_item.labels           : string[1] =  "strip"
internal_item.model.strip      : string = "snemo_strip_8.realistic.model"
internal_item.placement.strip  : string = "0 0 0 (mm) "
mapping.daughter_id.strip      : string = "[source_strip]"


[name="snemo_strip_9_pad.realistic.model" type="snrs::mesh_pad_model"]
strip_id : integer = 9
# #@description Safe shift along the X-axis:
# x_shift : real as length = -1.0 mm
# #@description Safe shift along the Y-axis:
# y_shift : real as length = -1.0 mm
visibility.hidden            : boolean = false
visibility.color             : string = "green"
source.visibility.hidden     : boolean = false
source.visibility.color      : string = "magenta"
back_film.visibility.hidden  : boolean = true
back_film.visibility.color   : string = "cyan"
front_film.visibility.hidden : boolean = true
front_film.visibility.color  : string = "cyan"
mapping.daughter_id.back_film   : string = "[source_pad_film:film=0]"
mapping.daughter_id.source      : string = "[source_pad_bulk]"
mapping.daughter_id.front_film  : string = "[source_pad_film:film=1]"


[name="snemo_strip_9.realistic.model" type="geomtools::simple_shaped_model"]
shape_type   : string = "box"
x            : real as length = 58.0 mm
y            : real as length = 135.5 mm
z            : real as length = 2700.0 mm
material.ref : string = "mix_selenium"
internal_item.labels          : string[1] =  "pad0"
internal_item.model.pad0      : string = "snemo_strip_9_pad.realistic.model"
internal_item.placement.pad0  : string = "0 0 0 (mm) "
mapping.daughter_id.pad0      : string = "[source_pad:pad=0]"


[name="snemo_strip_path_9.realistic.model" type="geomtools::simple_shaped_model"]
shape_type   : string = "box"
x            : real as length = 58.0 mm
y            : real as length = 135.5 mm
z            : real as length = 3000.0 mm
material.ref : string = "tracking_gas"
internal_item.labels           : string[1] =  "strip"
internal_item.model.strip      : string = "snemo_strip_9.realistic.model"
internal_item.placement.strip  : string = "0 0 0 (mm) "
mapping.daughter_id.strip      : string = "[source_strip]"


[name="snemo_strip_14_pad.realistic.model" type="snrs::mesh_pad_model"]
strip_id : integer = 14
# #@description Safe shift along the X-axis:
# x_shift : real as length = -1.0 mm
# #@description Safe shift along the Y-axis:
# y_shift : real as length = -1.0 mm
visibility.hidden            : boolean = false
visibility.color             : string = "green"
source.visibility.hidden     : boolean = false
source.visibility.color      : string = "magenta"
back_film.visibility.hidden  : boolean = true
back_film.visibility.color   : string = "cyan"
front_film.visibility.hidden : boolean = true
front_film.visibility.color  : string = "cyan"
mapping.daughter_id.back_film   : string = "[source_pad_film:film=0]"
mapping.daughter_id.source      : string = "[source_pad_bulk]"
mapping.daughter_id.front_film  : string = "[source_pad_film:film=1]"


[name="snemo_strip_14.realistic.model" type="geomtools::simple_shaped_model"]
shape_type   : string = "box"
x            : real as length = 58.0 mm
y            : real as length = 135.5 mm
z            : real as length = 2700.0 mm
material.ref : string = "mix_selenium"
internal_item.labels          : string[1] =  "pad0"
internal_item.model.pad0      : string = "snemo_strip_14_pad.realistic.model"
internal_item.placement.pad0  : string = "0 0 0 (mm) "
mapping.daughter_id.pad0      : string = "[source_pad:pad=0]"


[name="snemo_strip_path_14.realistic.model" type="geomtools::simple_shaped_model"]
shape_type   : string = "box"
x            : real as length = 58.0 mm
y            : real as length = 135.5 mm
z            : real as length = 3000.0 mm
material.ref : string = "tracking_gas"
internal_item.labels           : string[1] =  "strip"
internal_item.model.strip      : string = "snemo_strip_14.realistic.model"
internal_item.placement.strip  : string = "0 0 0 (mm) "
mapping.daughter_id.strip      : string = "[source_strip]"


[name="snemo_strip_15_pad.realistic.model" type="snrs::mesh_pad_model"]
strip_id : integer = 15
# #@description Safe shift along the X-axis:
# x_shift : real as length = -1.0 mm
# #@description Safe shift along the Y-axis:
# y_shift : real as length = -1.0 mm
visibility.hidden            : boolean = false
visibility.color             : string = "green"
source.visibility.hidden     : boolean = false
source.visibility.color      : string = "magenta"
back_film.visibility.hidden  : boolean = true
back_film.visibility.color   : string = "cyan"
front_film.visibility.hidden : boolean = true
front_film.visibility.color  : string = "cyan"
mapping.daughter_id.back_film   : string = "[source_pad_film:film=0]"
mapping.daughter_id.source      : string = "[source_pad_bulk]"
mapping.daughter_id.front_film  : string = "[source_pad_film:film=1]"


[name="snemo_strip_15.realistic.model" type="geomtools::simple_shaped_model"]
shape_type   : string = "box"
x            : real as length = 58.0 mm
y            : real as length = 135.5 mm
z            : real as length = 2700.0 mm
material.ref : string = "mix_selenium"
internal_item.labels          : string[1] =  "pad0"
internal_item.model.pad0      : string = "snemo_strip_15_pad.realistic.model"
internal_item.placement.pad0  : string = "0 0 0 (mm) "
mapping.daughter_id.pad0      : string = "[source_pad:pad=0]"


[name="snemo_strip_path_15.realistic.model" type="geomtools::simple_shaped_model"]
shape_type   : string = "box"
x            : real as length = 58.0 mm
y            : real as length = 135.5 mm
z            : real as length = 3000.0 mm
material.ref : string = "tracking_gas"
internal_item.labels           : string[1] =  "strip"
internal_item.model.strip      : string = "snemo_strip_15.realistic.model"
internal_item.placement.strip  : string = "0 0 0 (mm) "
mapping.daughter_id.strip      : string = "[source_strip]"


[name="snemo_strip_20_pad.realistic.model" type="snrs::mesh_pad_model"]
strip_id : integer = 20
# #@description Safe shift along the X-axis:
# x_shift : real as length = -1.0 mm
# #@description Safe shift along the Y-axis:
# y_shift : real as length = -1.0 mm
visibility.hidden            : boolean = false
visibility.color             : string = "green"
source.visibility.hidden     : boolean = false
source.visibility.color      : string = "magenta"
back_film.visibility.hidden  : boolean = true
back_film.visibility.color   : string = "cyan"
front_film.visibility.hidden : boolean = true
front_film.visibility.color  : string = "cyan"
mapping.daughter_id.back_film   : string = "[source_pad_film:film=0]"
mapping.daughter_id.source      : string = "[source_pad_bulk]"
mapping.daughter_id.front_film  : string = "[source_pad_film:film=1]"


[name="snemo_strip_20.realistic.model" type="geomtools::simple_shaped_model"]
shape_type   : string = "box"
x            : real as length = 58.0 mm
y            : real as length = 135.5 mm
z            : real as length = 2700.0 mm
material.ref : string = "mix_selenium"
internal_item.labels          : string[1] =  "pad0"
internal_item.model.pad0      : string = "snemo_strip_20_pad.realistic.model"
internal_item.placement.pad0  : string = "0 0 0 (mm) "
mapping.daughter_id.pad0      : string = "[source_pad:pad=0]"


[name="snemo_strip_path_20.realistic.model" type="geomtools::simple_shaped_model"]
shape_type   : string = "box"
x            : real as length = 58.0 mm
y            : real as length = 135.5 mm
z            : real as length = 3000.0 mm
material.ref : string = "tracking_gas"
internal_item.labels           : string[1] =  "strip"
internal_item.model.strip      : string = "snemo_strip_20.realistic.model"
internal_item.placement.strip  : string = "0 0 0 (mm) "
mapping.daughter_id.strip      : string = "[source_strip]"


[name="snemo_strip_21_pad.realistic.model" type="snrs::mesh_pad_model"]
strip_id : integer = 21
# #@description Safe shift along the X-axis:
# x_shift : real as length = -1.0 mm
# #@description Safe shift along the Y-axis:
# y_shift : real as length = -1.0 mm
visibility.hidden            : boolean = false
visibility.color             : string = "green"
source.visibility.hidden     : boolean = false
source.visibility.color      : string = "magenta"
back_film.visibility.hidden  : boolean = true
back_film.visibility.color   : string = "cyan"
front_film.visibility.hidden : boolean = true
front_film.visibility.color  : string = "cyan"
mapping.daughter_id.back_film   : string = "[source_pad_film:film=0]"
mapping.daughter_id.source      : string = "[source_pad_bulk]"
mapping.daughter_id.front_film  : string = "[source_pad_film:film=1]"


[name="snemo_strip_21.realistic.model" type="geomtools::simple_shaped_model"]
shape_type   : string = "box"
x            : real as length = 58.0 mm
y            : real as length = 135.5 mm
z            : real as length = 2700.0 mm
material.ref : string = "mix_selenium"
internal_item.labels          : string[1] =  "pad0"
internal_item.model.pad0      : string = "snemo_strip_21_pad.realistic.model"
internal_item.placement.pad0  : string = "0 0 0 (mm) "
mapping.daughter_id.pad0      : string = "[source_pad:pad=0]"


[name="snemo_strip_path_21.realistic.model" type="geomtools::simple_shaped_model"]
shape_type   : string = "box"
x            : real as length = 58.0 mm
y            : real as length = 135.5 mm
z            : real as length = 3000.0 mm
material.ref : string = "tracking_gas"
internal_item.labels           : string[1] =  "strip"
internal_item.model.strip      : string = "snemo_strip_21.realistic.model"
internal_item.placement.strip  : string = "0 0 0 (mm) "
mapping.daughter_id.strip      : string = "[source_strip]"


[name="snemo_strip_22_pad.realistic.model" type="snrs::mesh_pad_model"]
strip_id : integer = 22
# #@description Safe shift along the X-axis:
# x_shift : real as length = -1.0 mm
# #@description Safe shift along the Y-axis:
# y_shift : real as length = -1.0 mm
visibility.hidden            : boolean = false
visibility.color             : string = "green"
source.visibility.hidden     : boolean = false
source.visibility.color      : string = "magenta"
back_film.visibility.hidden  : boolean = true
back_film.visibility.color   : string = "cyan"
front_film.visibility.hidden : boolean = true
front_film.visibility.color  : string = "cyan"
mapping.daughter_id.back_film   : string = "[source_pad_film:film=0]"
mapping.daughter_id.source      : string = "[source_pad_bulk]"
mapping.daughter_id.front_film  : string = "[source_pad_film:film=1]"


[name="snemo_strip_22.realistic.model" type="geomtools::simple_shaped_model"]
shape_type   : string = "box"
x            : real as length = 58.0 mm
y            : real as length = 135.5 mm
z            : real as length = 2700.0 mm
material.ref : string = "mix_selenium"
internal_item.labels          : string[1] =  "pad0"
internal_item.model.pad0      : string = "snemo_strip_22_pad.realistic.model"
internal_item.placement.pad0  : string = "0 0 0 (mm) "
mapping.daughter_id.pad0      : string = "[source_pad:pad=0]"


[name="snemo_strip_path_22.realistic.model" type="geomtools::simple_shaped_model"]
shape_type   : string = "box"
x            : real as length = 58.0 mm
y            : real as length = 135.5 mm
z            : real as length = 3000.0 mm
material.ref : string = "tracking_gas"
internal_item.labels           : string[1] =  "strip"
internal_item.model.strip      : string = "snemo_strip_22.realistic.model"
internal_item.placement.strip  : string = "0 0 0 (mm) "
mapping.daughter_id.strip      : string = "[source_strip]"


[name="snemo_strip_23_pad.realistic.model" type="snrs::mesh_pad_model"]
strip_id : integer = 23
# #@description Safe shift along the X-axis:
# x_shift : real as length = -1.0 mm
# #@description Safe shift along the Y-axis:
# y_shift : real as length = -1.0 mm
visibility.hidden            : boolean = false
visibility.color             : string = "green"
source.visibility.hidden     : boolean = false
source.visibility.color      : string = "magenta"
back_film.visibility.hidden  : boolean = true
back_film.visibility.color   : string = "cyan"
front_film.visibility.hidden : boolean = true
front_film.visibility.color  : string = "cyan"
mapping.daughter_id.back_film   : string = "[source_pad_film:film=0]"
mapping.daughter_id.source      : string = "[source_pad_bulk]"
mapping.daughter_id.front_film  : string = "[source_pad_film:film=1]"


[name="snemo_strip_23.realistic.model" type="geomtools::simple_shaped_model"]
shape_type   : string = "box"
x            : real as length = 58.0 mm
y            : real as length = 135.5 mm
z            : real as length = 2700.0 mm
material.ref : string = "mix_selenium"
internal_item.labels          : string[1] =  "pad0"
internal_item.model.pad0      : string = "snemo_strip_23_pad.realistic.model"
internal_item.placement.pad0  : string = "0 0 0 (mm) "
mapping.daughter_id.pad0      : string = "[source_pad:pad=0]"


[name="snemo_strip_path_23.realistic.model" type="geomtools::simple_shaped_model"]
shape_type   : string = "box"
x            : real as length = 58.0 mm
y            : real as length = 135.5 mm
z            : real as length = 3000.0 mm
material.ref : string = "tracking_gas"
internal_item.labels           : string[1] =  "strip"
internal_item.model.strip      : string = "snemo_strip_23.realistic.model"
internal_item.placement.strip  : string = "0 0 0 (mm) "
mapping.daughter_id.strip      : string = "[source_strip]"


[name="snemo_strip_24_pad.realistic.model" type="snrs::mesh_pad_model"]
strip_id : integer = 24
# #@description Safe shift along the X-axis:
# x_shift : real as length = -1.0 mm
# #@description Safe shift along the Y-axis:
# y_shift : real as length = -1.0 mm
visibility.hidden            : boolean = false
visibility.color             : string = "green"
source.visibility.hidden     : boolean = false
source.visibility.color      : string = "magenta"
back_film.visibility.hidden  : boolean = true
back_film.visibility.color   : string = "cyan"
front_film.visibility.hidden : boolean = true
front_film.visibility.color  : string = "cyan"
mapping.daughter_id.back_film   : string = "[source_pad_film:film=0]"
mapping.daughter_id.source      : string = "[source_pad_bulk]"
mapping.daughter_id.front_film  : string = "[source_pad_film:film=1]"


[name="snemo_strip_24.realistic.model" type="geomtools::simple_shaped_model"]
shape_type   : string = "box"
x            : real as length = 58.0 mm
y            : real as length = 135.5 mm
z            : real as length = 2700.0 mm
material.ref : string = "mix_selenium"
internal_item.labels          : string[1] =  "pad0"
internal_item.model.pad0      : string = "snemo_strip_24_pad.realistic.model"
internal_item.placement.pad0  : string = "0 0 0 (mm) "
mapping.daughter_id.pad0      : string = "[source_pad:pad=0]"


[name="snemo_strip_path_24.realistic.model" type="geomtools::simple_shaped_model"]
shape_type   : string = "box"
x            : real as length = 58.0 mm
y            : real as length = 135.5 mm
z            : real as length = 3000.0 mm
material.ref : string = "tracking_gas"
internal_item.labels           : string[1] =  "strip"
internal_item.model.strip      : string = "snemo_strip_24.realistic.model"
internal_item.placement.strip  : string = "0 0 0 (mm) "
mapping.daughter_id.strip      : string = "[source_strip]"


[name="snemo_strip_25_pad.realistic.model" type="snrs::mesh_pad_model"]
strip_id : integer = 25
# #@description Safe shift along the X-axis:
# x_shift : real as length = -1.0 mm
# #@description Safe shift along the Y-axis:
# y_shift : real as length = -1.0 mm
visibility.hidden            : boolean = false
visibility.color             : string = "green"
source.visibility.hidden     : boolean = false
source.visibility.color      : string = "magenta"
back_film.visibility.hidden  : boolean = true
back_film.visibility.color   : string = "cyan"
front_film.visibility.hidden : boolean = true
front_film.visibility.color  : string = "cyan"
mapping.daughter_id.back_film   : string = "[source_pad_film:film=0]"
mapping.daughter_id.source      : string = "[source_pad_bulk]"
mapping.daughter_id.front_film  : string = "[source_pad_film:film=1]"


[name="snemo_strip_25.realistic.model" type="geomtools::simple_shaped_model"]
shape_type   : string = "box"
x            : real as length = 58.0 mm
y            : real as length = 135.5 mm
z            : real as length = 2700.0 mm
material.ref : string = "mix_selenium"
internal_item.labels          : string[1] =  "pad0"
internal_item.model.pad0      : string = "snemo_strip_25_pad.realistic.model"
internal_item.placement.pad0  : string = "0 0 0 (mm) "
mapping.daughter_id.pad0      : string = "[source_pad:pad=0]"


[name="snemo_strip_path_25.realistic.model" type="geomtools::simple_shaped_model"]
shape_type   : string = "box"
x            : real as length = 58.0 mm
y            : real as length = 135.5 mm
z            : real as length = 3000.0 mm
material.ref : string = "tracking_gas"
internal_item.labels           : string[1] =  "strip"
internal_item.model.strip      : string = "snemo_strip_25.realistic.model"
internal_item.placement.strip  : string = "0 0 0 (mm) "
mapping.daughter_id.strip      : string = "[source_strip]"


[name="snemo_strip_26_pad.realistic.model" type="snrs::mesh_pad_model"]
strip_id : integer = 26
# #@description Safe shift along the X-axis:
# x_shift : real as length = -1.0 mm
# #@description Safe shift along the Y-axis:
# y_shift : real as length = -1.0 mm
visibility.hidden            : boolean = false
visibility.color             : string = "green"
source.visibility.hidden     : boolean = false
source.visibility.color      : string = "magenta"
back_film.visibility.hidden  : boolean = true
back_film.visibility.color   : string = "cyan"
front_film.visibility.hidden : boolean = true
front_film.visibility.color  : string = "cyan"
mapping.daughter_id.back_film   : string = "[source_pad_film:film=0]"
mapping.daughter_id.source      : string = "[source_pad_bulk]"
mapping.daughter_id.front_film  : string = "[source_pad_film:film=1]"


[name="snemo_strip_26.realistic.model" type="geomtools::simple_shaped_model"]
shape_type   : string = "box"
x            : real as length = 58.0 mm
y            : real as length = 135.5 mm
z            : real as length = 2700.0 mm
material.ref : string = "mix_selenium"
internal_item.labels          : string[1] =  "pad0"
internal_item.model.pad0      : string = "snemo_strip_26_pad.realistic.model"
internal_item.placement.pad0  : string = "0 0 0 (mm) "
mapping.daughter_id.pad0      : string = "[source_pad:pad=0]"


[name="snemo_strip_path_26.realistic.model" type="geomtools::simple_shaped_model"]
shape_type   : string = "box"
x            : real as length = 58.0 mm
y            : real as length = 135.5 mm
z            : real as length = 3000.0 mm
material.ref : string = "tracking_gas"
internal_item.labels           : string[1] =  "strip"
internal_item.model.strip      : string = "snemo_strip_26.realistic.model"
internal_item.placement.strip  : string = "0 0 0 (mm) "
mapping.daughter_id.strip      : string = "[source_strip]"


[name="snemo_strip_27_pad.realistic.model" type="snrs::mesh_pad_model"]
strip_id : integer = 27
# #@description Safe shift along the X-axis:
# x_shift : real as length = -1.0 mm
# #@description Safe shift along the Y-axis:
# y_shift : real as length = -1.0 mm
visibility.hidden            : boolean = false
visibility.color             : string = "green"
source.visibility.hidden     : boolean = false
source.visibility.color      : string = "magenta"
back_film.visibility.hidden  : boolean = true
back_film.visibility.color   : string = "cyan"
front_film.visibility.hidden : boolean = true
front_film.visibility.color  : string = "cyan"
mapping.daughter_id.back_film   : string = "[source_pad_film:film=0]"
mapping.daughter_id.source      : string = "[source_pad_bulk]"
mapping.daughter_id.front_film  : string = "[source_pad_film:film=1]"


[name="snemo_strip_27.realistic.model" type="geomtools::simple_shaped_model"]
shape_type   : string = "box"
x            : real as length = 58.0 mm
y            : real as length = 135.5 mm
z            : real as length = 2700.0 mm
material.ref : string = "mix_selenium"
internal_item.labels          : string[1] =  "pad0"
internal_item.model.pad0      : string = "snemo_strip_27_pad.realistic.model"
internal_item.placement.pad0  : string = "0 0 0 (mm) "
mapping.daughter_id.pad0      : string = "[source_pad:pad=0]"


[name="snemo_strip_path_27.realistic.model" type="geomtools::simple_shaped_model"]
shape_type   : string = "box"
x            : real as length = 58.0 mm
y            : real as length = 135.5 mm
z            : real as length = 3000.0 mm
material.ref : string = "tracking_gas"
internal_item.labels           : string[1] =  "strip"
internal_item.model.strip      : string = "snemo_strip_27.realistic.model"
internal_item.placement.strip  : string = "0 0 0 (mm) "
mapping.daughter_id.strip      : string = "[source_strip]"


[name="snemo_strip_28_pad.realistic.model" type="snrs::mesh_pad_model"]
strip_id : integer = 28
# #@description Safe shift along the X-axis:
# x_shift : real as length = -1.0 mm
# #@description Safe shift along the Y-axis:
# y_shift : real as length = -1.0 mm
visibility.hidden            : boolean = false
visibility.color             : string = "green"
source.visibility.hidden     : boolean = false
source.visibility.color      : string = "magenta"
back_film.visibility.hidden  : boolean = true
back_film.visibility.color   : string = "cyan"
front_film.visibility.hidden : boolean = true
front_film.visibility.color  : string = "cyan"
mapping.daughter_id.back_film   : string = "[source_pad_film:film=0]"
mapping.daughter_id.source      : string = "[source_pad_bulk]"
mapping.daughter_id.front_film  : string = "[source_pad_film:film=1]"


[name="snemo_strip_28.realistic.model" type="geomtools::simple_shaped_model"]
shape_type   : string = "box"
x            : real as length = 58.0 mm
y            : real as length = 135.5 mm
z            : real as length = 2700.0 mm
material.ref : string = "mix_selenium"
internal_item.labels          : string[1] =  "pad0"
internal_item.model.pad0      : string = "snemo_strip_28_pad.realistic.model"
internal_item.placement.pad0  : string = "0 0 0 (mm) "
mapping.daughter_id.pad0      : string = "[source_pad:pad=0]"


[name="snemo_strip_path_28.realistic.model" type="geomtools::simple_shaped_model"]
shape_type   : string = "box"
x            : real as length = 58.0 mm
y            : real as length = 135.5 mm
z            : real as length = 3000.0 mm
material.ref : string = "tracking_gas"
internal_item.labels           : string[1] =  "strip"
internal_item.model.strip      : string = "snemo_strip_28.realistic.model"
internal_item.placement.strip  : string = "0 0 0 (mm) "
mapping.daughter_id.strip      : string = "[source_strip]"


[name="snemo_strip_31_pad.realistic.model" type="snrs::mesh_pad_model"]
strip_id : integer = 31
# #@description Safe shift along the X-axis:
# x_shift : real as length = -1.0 mm
# #@description Safe shift along the Y-axis:
# y_shift : real as length = -1.0 mm
visibility.hidden            : boolean = false
visibility.color             : string = "green"
source.visibility.hidden     : boolean = false
source.visibility.color      : string = "magenta"
back_film.visibility.hidden  : boolean = true
back_film.visibility.color   : string = "cyan"
front_film.visibility.hidden : boolean = true
front_film.visibility.color  : string = "cyan"
mapping.daughter_id.back_film   : string = "[source_pad_film:film=0]"
mapping.daughter_id.source      : string = "[source_pad_bulk]"
mapping.daughter_id.front_film  : string = "[source_pad_film:film=1]"


[name="snemo_strip_31.realistic.model" type="geomtools::simple_shaped_model"]
shape_type   : string = "box"
x            : real as length = 58.0 mm
y            : real as length = 135.5 mm
z            : real as length = 2700.0 mm
material.ref : string = "mix_selenium"
internal_item.labels          : string[1] =  "pad0"
internal_item.model.pad0      : string = "snemo_strip_31_pad.realistic.model"
internal_item.placement.pad0  : string = "0 0 0 (mm) "
mapping.daughter_id.pad0      : string = "[source_pad:pad=0]"


[name="snemo_strip_path_31.realistic.model" type="geomtools::simple_shaped_model"]
shape_type   : string = "box"
x            : real as length = 58.0 mm
y            : real as length = 135.5 mm
z            : real as length = 3000.0 mm
material.ref : string = "tracking_gas"
internal_item.labels           : string[1] =  "strip"
internal_item.model.strip      : string = "snemo_strip_31.realistic.model"
internal_item.placement.strip  : string = "0 0 0 (mm) "
mapping.daughter_id.strip      : string = "[source_strip]"


[name="snemo_strip_32_pad.realistic.model" type="snrs::mesh_pad_model"]
strip_id : integer = 32
# #@description Safe shift along the X-axis:
# x_shift : real as length = -1.0 mm
# #@description Safe shift along the Y-axis:
# y_shift : real as length = -1.0 mm
visibility.hidden            : boolean = false
visibility.color             : string = "green"
source.visibility.hidden     : boolean = false
source.visibility.color      : string = "magenta"
back_film.visibility.hidden  : boolean = true
back_film.visibility.color   : string = "cyan"
front_film.visibility.hidden : boolean = true
front_film.visibility.color  : string = "cyan"
mapping.daughter_id.back_film   : string = "[source_pad_film:film=0]"
mapping.daughter_id.source      : string = "[source_pad_bulk]"
mapping.daughter_id.front_film  : string = "[source_pad_film:film=1]"


[name="snemo_strip_32.realistic.model" type="geomtools::simple_shaped_model"]
shape_type   : string = "box"
x            : real as length = 58.0 mm
y            : real as length = 135.5 mm
z            : real as length = 2700.0 mm
material.ref : string = "mix_selenium"
internal_item.labels          : string[1] =  "pad0"
internal_item.model.pad0      : string = "snemo_strip_32_pad.realistic.model"
internal_item.placement.pad0  : string = "0 0 0 (mm) "
mapping.daughter_id.pad0      : string = "[source_pad:pad=0]"


[name="snemo_strip_path_32.realistic.model" type="geomtools::simple_shaped_model"]
shape_type   : string = "box"
x            : real as length = 58.0 mm
y            : real as length = 135.5 mm
z            : real as length = 3000.0 mm
material.ref : string = "tracking_gas"
internal_item.labels           : string[1] =  "strip"
internal_item.model.strip      : string = "snemo_strip_32.realistic.model"
internal_item.placement.strip  : string = "0 0 0 (mm) "
mapping.daughter_id.strip      : string = "[source_strip]"


[name="snemo_strip_33_pad.realistic.model" type="snrs::mesh_pad_model"]
strip_id : integer = 33
# #@description Safe shift along the X-axis:
# x_shift : real as length = -1.0 mm
# #@description Safe shift along the Y-axis:
# y_shift : real as length = -1.0 mm
visibility.hidden            : boolean = false
visibility.color             : string = "green"
source.visibility.hidden     : boolean = false
source.visibility.color      : string = "magenta"
back_film.visibility.hidden  : boolean = true
back_film.visibility.color   : string = "cyan"
front_film.visibility.hidden : boolean = true
front_film.visibility.color  : string = "cyan"
mapping.daughter_id.back_film   : string = "[source_pad_film:film=0]"
mapping.daughter_id.source      : string = "[source_pad_bulk]"
mapping.daughter_id.front_film  : string = "[source_pad_film:film=1]"


[name="snemo_strip_33.realistic.model" type="geomtools::simple_shaped_model"]
shape_type   : string = "box"
x            : real as length = 58.0 mm
y            : real as length = 135.5 mm
z            : real as length = 2700.0 mm
material.ref : string = "mix_selenium"
internal_item.labels          : string[1] =  "pad0"
internal_item.model.pad0      : string = "snemo_strip_33_pad.realistic.model"
internal_item.placement.pad0  : string = "0 0 0 (mm) "
mapping.daughter_id.pad0      : string = "[source_pad:pad=0]"


[name="snemo_strip_path_33.realistic.model" type="geomtools::simple_shaped_model"]
shape_type   : string = "box"
x            : real as length = 58.0 mm
y            : real as length = 135.5 mm
z            : real as length = 3000.0 mm
material.ref : string = "tracking_gas"
internal_item.labels           : string[1] =  "strip"
internal_item.model.strip      : string = "snemo_strip_33.realistic.model"
internal_item.placement.strip  : string = "0 0 0 (mm) "
mapping.daughter_id.strip      : string = "[source_strip]"


[name="snemo_strip_34_pad.realistic.model" type="snrs::mesh_pad_model"]
strip_id : integer = 34
# #@description Safe shift along the X-axis:
# x_shift : real as length = -1.0 mm
# #@description Safe shift along the Y-axis:
# y_shift : real as length = -1.0 mm
visibility.hidden            : boolean = false
visibility.color             : string = "green"
source.visibility.hidden     : boolean = false
source.visibility.color      : string = "magenta"
back_film.visibility.hidden  : boolean = true
back_film.visibility.color   : string = "cyan"
front_film.visibility.hidden : boolean = true
front_film.visibility.color  : string = "cyan"
mapping.daughter_id.back_film   : string = "[source_pad_film:film=0]"
mapping.daughter_id.source      : string = "[source_pad_bulk]"
mapping.daughter_id.front_film  : string = "[source_pad_film:film=1]"


[name="snemo_strip_34.realistic.model" type="geomtools::simple_shaped_model"]
shape_type   : string = "box"
x            : real as length = 58.0 mm
y            : real as length = 135.5 mm
z            : real as length = 2700.0 mm
material.ref : string = "mix_selenium"
internal_item.labels          : string[1] =  "pad0"
internal_item.model.pad0      : string = "snemo_strip_34_pad.realistic.model"
internal_item.placement.pad0  : string = "0 0 0 (mm) "
mapping.daughter_id.pad0      : string = "[source_pad:pad=0]"


[name="snemo_strip_path_34.realistic.model" type="geomtools::simple_shaped_model"]
shape_type   : string = "box"
x            : real as length = 58.0 mm
y            : real as length = 135.5 mm
z            : real as length = 3000.0 mm
material.ref : string = "tracking_gas"
internal_item.labels           : string[1] =  "strip"
internal_item.model.strip      : string = "snemo_strip_34.realistic.model"
internal_item.placement.strip  : string = "0 0 0 (mm) "
mapping.daughter_id.strip      : string = "[source_strip]"


