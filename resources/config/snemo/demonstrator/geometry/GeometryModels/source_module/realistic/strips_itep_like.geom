# -*- mode: conf-unix; -*-
# Author : F.Mauger
# Date   : May 2021
# Realistic geometry model of (LAPP-)ITEP source strips 

 
###################################################################
#
# This model represents the SuperNEMO strip 3 pad 0 realistic geometry
#
# model and its associated logical volume as well as the
# "snemo_strip_3_realistic.back_film.model" and "snemo_strip_3_realistic.front_film.model" models.
# 
[name="snemo_strip_3_pad.realistic.model" type="snrs::mesh_pad_model"]

#@config Strip 3 model

#@description Strip ID:
strip_id : integer = 3

# #@description Safe shift along the X-axis:
# x_shift : real as length = -1.0 mm

# #@description Safe shift along the Y-axis:
# y_shift : real as length = -1.0 mm

# Visibility :
visibility.hidden            : boolean = false
visibility.color             : string = "green"
source.visibility.hidden     : boolean = false
source.visibility.color      : string = "magenta"
back_film.visibility.hidden  : boolean = true
back_film.visibility.color   : string = "cyan"
front_film.visibility.hidden : boolean = true
front_film.visibility.color  : string = "cyan"

# Mapping the daughter volumes :
mapping.daughter_id.back_film   : string = "[source_pad_film:film=0]"
mapping.daughter_id.source      : string = "[source_pad_bulk]"
mapping.daughter_id.front_film  : string = "[source_pad_film:film=1]"

 
###################################################################
#
# This model represents the SuperNEMO strip 8 pad 0 realistic geometry
#
# model and its associated logical volume as well as the
# "snemo_strip_8_realistic.back_film.model" and "snemo_strip_8_realistic.front_film.model" models.
# 
[name="snemo_strip_8_pad.realistic.model" type="snrs::mesh_pad_model"]

#@config Strip 8 model

#@description Strip ID:
strip_id : integer = 8

# #@description Safe shift along the X-axis:
# x_shift : real as length = -1.0 mm

# #@description Safe shift along the Y-axis:
# y_shift : real as length = -1.0 mm

# Visibility :
visibility.hidden            : boolean = false
visibility.color             : string = "green"
source.visibility.hidden     : boolean = false
source.visibility.color      : string = "magenta"
back_film.visibility.hidden  : boolean = true
back_film.visibility.color   : string = "cyan"
front_film.visibility.hidden : boolean = true
front_film.visibility.color  : string = "cyan"

# Mapping the daughter volumes :
mapping.daughter_id.back_film   : string = "[source_pad_film:film=0]"
mapping.daughter_id.source      : string = "[source_pad_bulk]"
mapping.daughter_id.front_film  : string = "[source_pad_film:film=1]"

 
###################################################################
#
# This model represents the SuperNEMO strip 9 pad 0 realistic geometry
#
# model and its associated logical volume as well as the
# "snemo_strip_9_realistic.back_film.model" and "snemo_strip_9_realistic.front_film.model" models.
# 
[name="snemo_strip_9_pad.realistic.model" type="snrs::mesh_pad_model"]

#@config Strip 9 model

#@description Strip ID:
strip_id : integer = 9

# #@description Safe shift along the X-axis:
# x_shift : real as length = -1.0 mm

# #@description Safe shift along the Y-axis:
# y_shift : real as length = -1.0 mm

# Visibility :
visibility.hidden            : boolean = false
visibility.color             : string = "green"
source.visibility.hidden     : boolean = false
source.visibility.color      : string = "magenta"
back_film.visibility.hidden  : boolean = true
back_film.visibility.color   : string = "cyan"
front_film.visibility.hidden : boolean = true
front_film.visibility.color  : string = "cyan"

# Mapping the daughter volumes :
mapping.daughter_id.back_film   : string = "[source_pad_film:film=0]"
mapping.daughter_id.source      : string = "[source_pad_bulk]"
mapping.daughter_id.front_film  : string = "[source_pad_film:film=1]"

 
###################################################################
#
# This model represents the SuperNEMO strip 14 pad 0 realistic geometry
#
# model and its associated logical volume as well as the
# "snemo_strip_14_realistic.back_film.model" and "snemo_strip_14_realistic.front_film.model" models.
# 
[name="snemo_strip_14_pad.realistic.model" type="snrs::mesh_pad_model"]

#@config Strip 14 model

#@description Strip ID:
strip_id : integer = 14

# #@description Safe shift along the X-axis:
# x_shift : real as length = -1.0 mm

# #@description Safe shift along the Y-axis:
# y_shift : real as length = -1.0 mm

# Visibility :
visibility.hidden            : boolean = false
visibility.color             : string = "green"
source.visibility.hidden     : boolean = false
source.visibility.color      : string = "magenta"
back_film.visibility.hidden  : boolean = true
back_film.visibility.color   : string = "cyan"
front_film.visibility.hidden : boolean = true
front_film.visibility.color  : string = "cyan"

# Mapping the daughter volumes :
mapping.daughter_id.back_film   : string = "[source_pad_film:film=0]"
mapping.daughter_id.source      : string = "[source_pad_bulk]"
mapping.daughter_id.front_film  : string = "[source_pad_film:film=1]"

 
###################################################################
#
# This model represents the SuperNEMO strip 15 pad 0 realistic geometry
#
# model and its associated logical volume as well as the
# "snemo_strip_15_realistic.back_film.model" and "snemo_strip_15_realistic.front_film.model" models.
# 
[name="snemo_strip_15_pad.realistic.model" type="snrs::mesh_pad_model"]

#@config Strip 15 model

#@description Strip ID:
strip_id : integer = 15

# #@description Safe shift along the X-axis:
# x_shift : real as length = -1.0 mm

# #@description Safe shift along the Y-axis:
# y_shift : real as length = -1.0 mm

# Visibility :
visibility.hidden            : boolean = false
visibility.color             : string = "green"
source.visibility.hidden     : boolean = false
source.visibility.color      : string = "magenta"
back_film.visibility.hidden  : boolean = true
back_film.visibility.color   : string = "cyan"
front_film.visibility.hidden : boolean = true
front_film.visibility.color  : string = "cyan"

# Mapping the daughter volumes :
mapping.daughter_id.back_film   : string = "[source_pad_film:film=0]"
mapping.daughter_id.source      : string = "[source_pad_bulk]"
mapping.daughter_id.front_film  : string = "[source_pad_film:film=1]"

 
###################################################################
#
# This model represents the SuperNEMO strip 20 pad 0 realistic geometry
#
# model and its associated logical volume as well as the
# "snemo_strip_20_realistic.back_film.model" and "snemo_strip_20_realistic.front_film.model" models.
# 
[name="snemo_strip_20_pad.realistic.model" type="snrs::mesh_pad_model"]

#@config Strip 20 model

#@description Strip ID:
strip_id : integer = 20

# #@description Safe shift along the X-axis:
# x_shift : real as length = -1.0 mm

# #@description Safe shift along the Y-axis:
# y_shift : real as length = -1.0 mm

# Visibility :
visibility.hidden            : boolean = false
visibility.color             : string = "green"
source.visibility.hidden     : boolean = false
source.visibility.color      : string = "magenta"
back_film.visibility.hidden  : boolean = true
back_film.visibility.color   : string = "cyan"
front_film.visibility.hidden : boolean = true
front_film.visibility.color  : string = "cyan"

# Mapping the daughter volumes :
mapping.daughter_id.back_film   : string = "[source_pad_film:film=0]"
mapping.daughter_id.source      : string = "[source_pad_bulk]"
mapping.daughter_id.front_film  : string = "[source_pad_film:film=1]"

 
###################################################################
#
# This model represents the SuperNEMO strip 21 pad 0 realistic geometry
#
# model and its associated logical volume as well as the
# "snemo_strip_21_realistic.back_film.model" and "snemo_strip_21_realistic.front_film.model" models.
# 
[name="snemo_strip_21_pad.realistic.model" type="snrs::mesh_pad_model"]

#@config Strip 21 model

#@description Strip ID:
strip_id : integer = 21

# #@description Safe shift along the X-axis:
# x_shift : real as length = -1.0 mm

# #@description Safe shift along the Y-axis:
# y_shift : real as length = -1.0 mm

# Visibility :
visibility.hidden            : boolean = false
visibility.color             : string = "green"
source.visibility.hidden     : boolean = false
source.visibility.color      : string = "magenta"
back_film.visibility.hidden  : boolean = true
back_film.visibility.color   : string = "cyan"
front_film.visibility.hidden : boolean = true
front_film.visibility.color  : string = "cyan"

# Mapping the daughter volumes :
mapping.daughter_id.back_film   : string = "[source_pad_film:film=0]"
mapping.daughter_id.source      : string = "[source_pad_bulk]"
mapping.daughter_id.front_film  : string = "[source_pad_film:film=1]"

 
###################################################################
#
# This model represents the SuperNEMO strip 22 pad 0 realistic geometry
#
# model and its associated logical volume as well as the
# "snemo_strip_22_realistic.back_film.model" and "snemo_strip_22_realistic.front_film.model" models.
# 
[name="snemo_strip_22_pad.realistic.model" type="snrs::mesh_pad_model"]

#@config Strip 22 model

#@description Strip ID:
strip_id : integer = 22

# #@description Safe shift along the X-axis:
# x_shift : real as length = -1.0 mm

# #@description Safe shift along the Y-axis:
# y_shift : real as length = -1.0 mm

# Visibility :
visibility.hidden            : boolean = false
visibility.color             : string = "green"
source.visibility.hidden     : boolean = false
source.visibility.color      : string = "magenta"
back_film.visibility.hidden  : boolean = true
back_film.visibility.color   : string = "cyan"
front_film.visibility.hidden : boolean = true
front_film.visibility.color  : string = "cyan"

# Mapping the daughter volumes :
mapping.daughter_id.back_film   : string = "[source_pad_film:film=0]"
mapping.daughter_id.source      : string = "[source_pad_bulk]"
mapping.daughter_id.front_film  : string = "[source_pad_film:film=1]"

 
###################################################################
#
# This model represents the SuperNEMO strip 23 pad 0 realistic geometry
#
# model and its associated logical volume as well as the
# "snemo_strip_23_realistic.back_film.model" and "snemo_strip_23_realistic.front_film.model" models.
# 
[name="snemo_strip_23_pad.realistic.model" type="snrs::mesh_pad_model"]

#@config Strip 23 model

#@description Strip ID:
strip_id : integer = 23

# #@description Safe shift along the X-axis:
# x_shift : real as length = -1.0 mm

# #@description Safe shift along the Y-axis:
# y_shift : real as length = -1.0 mm

# Visibility :
visibility.hidden            : boolean = false
visibility.color             : string = "green"
source.visibility.hidden     : boolean = false
source.visibility.color      : string = "magenta"
back_film.visibility.hidden  : boolean = true
back_film.visibility.color   : string = "cyan"
front_film.visibility.hidden : boolean = true
front_film.visibility.color  : string = "cyan"

# Mapping the daughter volumes :
mapping.daughter_id.back_film   : string = "[source_pad_film:film=0]"
mapping.daughter_id.source      : string = "[source_pad_bulk]"
mapping.daughter_id.front_film  : string = "[source_pad_film:film=1]"

 
###################################################################
#
# This model represents the SuperNEMO strip 24 pad 0 realistic geometry
#
# model and its associated logical volume as well as the
# "snemo_strip_24_realistic.back_film.model" and "snemo_strip_24_realistic.front_film.model" models.
# 
[name="snemo_strip_24_pad.realistic.model" type="snrs::mesh_pad_model"]

#@config Strip 24 model

#@description Strip ID:
strip_id : integer = 24

# #@description Safe shift along the X-axis:
# x_shift : real as length = -1.0 mm

# #@description Safe shift along the Y-axis:
# y_shift : real as length = -1.0 mm

# Visibility :
visibility.hidden            : boolean = false
visibility.color             : string = "green"
source.visibility.hidden     : boolean = false
source.visibility.color      : string = "magenta"
back_film.visibility.hidden  : boolean = true
back_film.visibility.color   : string = "cyan"
front_film.visibility.hidden : boolean = true
front_film.visibility.color  : string = "cyan"

# Mapping the daughter volumes :
mapping.daughter_id.back_film   : string = "[source_pad_film:film=0]"
mapping.daughter_id.source      : string = "[source_pad_bulk]"
mapping.daughter_id.front_film  : string = "[source_pad_film:film=1]"

 
###################################################################
#
# This model represents the SuperNEMO strip 25 pad 0 realistic geometry
#
# model and its associated logical volume as well as the
# "snemo_strip_25_realistic.back_film.model" and "snemo_strip_25_realistic.front_film.model" models.
# 
[name="snemo_strip_25_pad.realistic.model" type="snrs::mesh_pad_model"]

#@config Strip 25 model

#@description Strip ID:
strip_id : integer = 25

# #@description Safe shift along the X-axis:
# x_shift : real as length = -1.0 mm

# #@description Safe shift along the Y-axis:
# y_shift : real as length = -1.0 mm

# Visibility :
visibility.hidden            : boolean = false
visibility.color             : string = "green"
source.visibility.hidden     : boolean = false
source.visibility.color      : string = "magenta"
back_film.visibility.hidden  : boolean = true
back_film.visibility.color   : string = "cyan"
front_film.visibility.hidden : boolean = true
front_film.visibility.color  : string = "cyan"

# Mapping the daughter volumes :
mapping.daughter_id.back_film   : string = "[source_pad_film:film=0]"
mapping.daughter_id.source      : string = "[source_pad_bulk]"
mapping.daughter_id.front_film  : string = "[source_pad_film:film=1]"

 
###################################################################
#
# This model represents the SuperNEMO strip 26 pad 0 realistic geometry
#
# model and its associated logical volume as well as the
# "snemo_strip_26_realistic.back_film.model" and "snemo_strip_26_realistic.front_film.model" models.
# 
[name="snemo_strip_26_pad.realistic.model" type="snrs::mesh_pad_model"]

#@config Strip 26 model

#@description Strip ID:
strip_id : integer = 26

# #@description Safe shift along the X-axis:
# x_shift : real as length = -1.0 mm

# #@description Safe shift along the Y-axis:
# y_shift : real as length = -1.0 mm

# Visibility :
visibility.hidden            : boolean = false
visibility.color             : string = "green"
source.visibility.hidden     : boolean = false
source.visibility.color      : string = "magenta"
back_film.visibility.hidden  : boolean = true
back_film.visibility.color   : string = "cyan"
front_film.visibility.hidden : boolean = true
front_film.visibility.color  : string = "cyan"

# Mapping the daughter volumes :
mapping.daughter_id.back_film   : string = "[source_pad_film:film=0]"
mapping.daughter_id.source      : string = "[source_pad_bulk]"
mapping.daughter_id.front_film  : string = "[source_pad_film:film=1]"

 
###################################################################
#
# This model represents the SuperNEMO strip 27 pad 0 realistic geometry
#
# model and its associated logical volume as well as the
# "snemo_strip_27_realistic.back_film.model" and "snemo_strip_27_realistic.front_film.model" models.
# 
[name="snemo_strip_27_pad.realistic.model" type="snrs::mesh_pad_model"]

#@config Strip 27 model

#@description Strip ID:
strip_id : integer = 27

# #@description Safe shift along the X-axis:
# x_shift : real as length = -1.0 mm

# #@description Safe shift along the Y-axis:
# y_shift : real as length = -1.0 mm

# Visibility :
visibility.hidden            : boolean = false
visibility.color             : string = "green"
source.visibility.hidden     : boolean = false
source.visibility.color      : string = "magenta"
back_film.visibility.hidden  : boolean = true
back_film.visibility.color   : string = "cyan"
front_film.visibility.hidden : boolean = true
front_film.visibility.color  : string = "cyan"

# Mapping the daughter volumes :
mapping.daughter_id.back_film   : string = "[source_pad_film:film=0]"
mapping.daughter_id.source      : string = "[source_pad_bulk]"
mapping.daughter_id.front_film  : string = "[source_pad_film:film=1]"

 
###################################################################
#
# This model represents the SuperNEMO strip 28 pad 0 realistic geometry
#
# model and its associated logical volume as well as the
# "snemo_strip_28_realistic.back_film.model" and "snemo_strip_28_realistic.front_film.model" models.
# 
[name="snemo_strip_28_pad.realistic.model" type="snrs::mesh_pad_model"]

#@config Strip 28 model

#@description Strip ID:
strip_id : integer = 28

# #@description Safe shift along the X-axis:
# x_shift : real as length = -1.0 mm

# #@description Safe shift along the Y-axis:
# y_shift : real as length = -1.0 mm

# Visibility :
visibility.hidden            : boolean = false
visibility.color             : string = "green"
source.visibility.hidden     : boolean = false
source.visibility.color      : string = "magenta"
back_film.visibility.hidden  : boolean = true
back_film.visibility.color   : string = "cyan"
front_film.visibility.hidden : boolean = true
front_film.visibility.color  : string = "cyan"

# Mapping the daughter volumes :
mapping.daughter_id.back_film   : string = "[source_pad_film:film=0]"
mapping.daughter_id.source      : string = "[source_pad_bulk]"
mapping.daughter_id.front_film  : string = "[source_pad_film:film=1]"


###################################################################
#
# This model represents the SuperNEMO strip 31 pad 0 realistic geometry
#
# model and its associated logical volume as well as the
# "snemo_strip_31_realistic.back_film.model" and "snemo_strip_31_realistic.front_film.model" models.
# 
[name="snemo_strip_31_pad.realistic.model" type="snrs::mesh_pad_model"]

#@config Strip 31 model

#@description Strip ID:
strip_id : integer = 31

# #@description Safe shift along the X-axis:
# x_shift : real as length = -1.0 mm

# #@description Safe shift along the Y-axis:
# y_shift : real as length = -1.0 mm

# Visibility :
visibility.hidden            : boolean = false
visibility.color             : string = "green"
source.visibility.hidden     : boolean = false
source.visibility.color      : string = "magenta"
back_film.visibility.hidden  : boolean = true
back_film.visibility.color   : string = "cyan"
front_film.visibility.hidden : boolean = true
front_film.visibility.color  : string = "cyan"

# Mapping the daughter volumes :
mapping.daughter_id.back_film   : string = "[source_pad_film:film=0]"
mapping.daughter_id.source      : string = "[source_pad_bulk]"
mapping.daughter_id.front_film  : string = "[source_pad_film:film=1]"

 
###################################################################
#
# This model represents the SuperNEMO strip 32 pad 0 realistic geometry
#
# model and its associated logical volume as well as the
# "snemo_strip_32_realistic.back_film.model" and "snemo_strip_32_realistic.front_film.model" models.
# 
[name="snemo_strip_32_pad.realistic.model" type="snrs::mesh_pad_model"]

#@config Strip 32 model

#@description Strip ID:
strip_id : integer = 32

# #@description Safe shift along the X-axis:
# x_shift : real as length = -1.0 mm

# #@description Safe shift along the Y-axis:
# y_shift : real as length = -1.0 mm

# Visibility :
visibility.hidden            : boolean = false
visibility.color             : string = "green"
source.visibility.hidden     : boolean = false
source.visibility.color      : string = "magenta"
back_film.visibility.hidden  : boolean = true
back_film.visibility.color   : string = "cyan"
front_film.visibility.hidden : boolean = true
front_film.visibility.color  : string = "cyan"

# Mapping the daughter volumes :
mapping.daughter_id.back_film   : string = "[source_pad_film:film=0]"
mapping.daughter_id.source      : string = "[source_pad_bulk]"
mapping.daughter_id.front_film  : string = "[source_pad_film:film=1]"

###################################################################
#
# This model represents the SuperNEMO strip 33 pad 0 realistic geometry
#
# model and its associated logical volume as well as the
# "snemo_strip_33_realistic.back_film.model" and "snemo_strip_33_realistic.front_film.model" models.
# 
[name="snemo_strip_33_pad.realistic.model" type="snrs::mesh_pad_model"]

#@config Strip 33 model

#@description Strip ID:
strip_id : integer = 33

# Visibility :
visibility.hidden            : boolean = false
visibility.color             : string = "green"
source.visibility.hidden     : boolean = false
source.visibility.color      : string = "magenta"
back_film.visibility.hidden  : boolean = true
back_film.visibility.color   : string = "cyan"
front_film.visibility.hidden : boolean = true
front_film.visibility.color  : string = "cyan"

# Mapping the daughter volumes :
mapping.daughter_id.back_film   : string = "[source_pad_film:film=0]"
mapping.daughter_id.source      : string = "[source_pad_bulk]"
mapping.daughter_id.front_film  : string = "[source_pad_film:film=1]"


###################################################################
#
# This model represents the SuperNEMO strip 34 pad 0 realistic geometry
#
# model and its associated logical volume as well as the
# "snemo_strip_34_realistic.back_film.model" and "snemo_strip_34_realistic.front_film.model" models.
# 
[name="snemo_strip_34_pad.realistic.model" type="snrs::mesh_pad_model"]

#@config Strip 34 model (LAPP-ITEP-4)

#@description Strip ID:
strip_id : integer = 34

# #@description Depth of the gas volume around the source strip (along the X-axis)
# depth : real as length = 58 mm

# pad.load_file             : string as path = "@snrs:data/geometry/source_foils/fsf/strip-34-pad-0-shaped.dat"
# pad.source.load_mesh      : string as path = "@snrs:data/geometry/source_foils/fsf/strip-34-pad-0-tessellated.dat"
# pad.source.load_tiles     : string as path = "@snrs:data/geometry/source_foils/fsf/strip-34-pad-0-tiles.dat"
# pad.back_film.load_mesh   : string as path = "@snrs:data/geometry/source_foils/fsf/strip-34-pad-0-tessellated-back-film.dat"
# pad.back_film.load_tiles  : string as path = "@snrs:data/geometry/source_foils/fsf/strip-34-pad-0-tiles-back-film.dat"
# pad.front_film.load_mesh  : string as path = "@snrs:data/geometry/source_foils/fsf/strip-34-pad-0-tessellated-front-film.dat"
# pad.front_film.load_tiles : string as path = "@snrs:data/geometry/source_foils/fsf/strip-34-pad-0-tiles-front-film.dat"
# material.ref              : string = "tracking_gas"
# source_material.ref       : string = "snemo::se82_enriched96.1_pva0.1000_density2.811"
# film_material.ref         : string = "basic::mylar"

# Visibility :
visibility.hidden             : boolean = false
visibility.color              : string = "green"
source.visibility.color       : string = "magenta"
back_film.visibility.color  : string = "cyan"
front_film.visibility.color : string = "cyan"

# Mapping the daughter volumes :
mapping.daughter_id.back_film   : string = "[source_pad_film:film=0]"
mapping.daughter_id.source      : string = "[source_pad_bulk]"
mapping.daughter_id.front_film  : string = "[source_pad_film:film=1]"

 
###################################################################
#
# This model represents the realistic layout of the strip 3
# 
[name="snemo_strip_3.realistic.model" type="geomtools::stacked_model"]
stacked.axis            : string = "x"
stacked.number_of_items : integer = 1
stacked.model_0  : string = "snemo_strip_3_pad.realistic.model"
stacked.label_0  : string = "pad_bundle_0"
material.ref     : string = "tracking_gas"
visibility.color : string = "green"
# Mapping the source foil from its parent category :
mapping.daughter_id.pad_bundle_0 : string = "[source_pad:pad=0]"

 
###################################################################
#
# This model represents the realistic layout of the strip 8
# 
[name="snemo_strip_8.realistic.model" type="geomtools::stacked_model"]
stacked.axis            : string = "x"
stacked.number_of_items : integer = 1
stacked.model_0  : string = "snemo_strip_8_pad.realistic.model"
stacked.label_0  : string = "pad_bundle_0"
material.ref     : string = "tracking_gas"
visibility.color : string = "green"
# Mapping the source foil from its parent category :
mapping.daughter_id.pad_bundle_0 : string = "[source_pad:pad=0]"

 
###################################################################
#
# This model represents the realistic layout of the strip 9
# 
[name="snemo_strip_9.realistic.model" type="geomtools::stacked_model"]
stacked.axis            : string = "x"
stacked.number_of_items : integer = 1
stacked.model_0  : string = "snemo_strip_9_pad.realistic.model"
stacked.label_0  : string = "pad_bundle_0"
material.ref     : string = "tracking_gas"
visibility.color : string = "green"
# Mapping the source foil from its parent category :
mapping.daughter_id.pad_bundle_0 : string = "[source_pad:pad=0]"

 
###################################################################
#
# This model represents the realistic layout of the strip 14
# 
[name="snemo_strip_14.realistic.model" type="geomtools::stacked_model"]
stacked.axis            : string = "x"
stacked.number_of_items : integer = 1
stacked.model_0  : string = "snemo_strip_14_pad.realistic.model"
stacked.label_0  : string = "pad_bundle_0"
material.ref     : string = "tracking_gas"
visibility.color : string = "green"
# Mapping the source foil from its parent category :
mapping.daughter_id.pad_bundle_0 : string = "[source_pad:pad=0]"

 
###################################################################
#
# This model represents the realistic layout of the strip 15
# 
[name="snemo_strip_15.realistic.model" type="geomtools::stacked_model"]
stacked.axis            : string = "x"
stacked.number_of_items : integer = 1
stacked.model_0  : string = "snemo_strip_15_pad.realistic.model"
stacked.label_0  : string = "pad_bundle_0"
material.ref     : string = "tracking_gas"
visibility.color : string = "green"
# Mapping the source foil from its parent category :
mapping.daughter_id.pad_bundle_0 : string = "[source_pad:pad=0]"

 
###################################################################
#
# This model represents the realistic layout of the strip 20
# 
[name="snemo_strip_20.realistic.model" type="geomtools::stacked_model"]
stacked.axis            : string = "x"
stacked.number_of_items : integer = 1
stacked.model_0  : string = "snemo_strip_20_pad.realistic.model"
stacked.label_0  : string = "pad_bundle_0"
material.ref     : string = "tracking_gas"
visibility.color : string = "green"
# Mapping the source foil from its parent category :
mapping.daughter_id.pad_bundle_0 : string = "[source_pad:pad=0]"

 
###################################################################
#
# This model represents the realistic layout of the strip 21
# 
[name="snemo_strip_21.realistic.model" type="geomtools::stacked_model"]
stacked.axis            : string = "x"
stacked.number_of_items : integer = 1
stacked.model_0  : string = "snemo_strip_21_pad.realistic.model"
stacked.label_0  : string = "pad_bundle_0"
material.ref     : string = "tracking_gas"
visibility.color : string = "green"
# Mapping the source foil from its parent category :
mapping.daughter_id.pad_bundle_0 : string = "[source_pad:pad=0]"

 
###################################################################
#
# This model represents the realistic layout of the strip 22
# 
[name="snemo_strip_22.realistic.model" type="geomtools::stacked_model"]
stacked.axis            : string = "x"
stacked.number_of_items : integer = 1
stacked.model_0  : string = "snemo_strip_22_pad.realistic.model"
stacked.label_0  : string = "pad_bundle_0"
material.ref     : string = "tracking_gas"
visibility.color : string = "green"
# Mapping the source foil from its parent category :
mapping.daughter_id.pad_bundle_0 : string = "[source_pad:pad=0]"
 
 
###################################################################
#
# This model represents the realistic layout of the strip 23
# 
[name="snemo_strip_23.realistic.model" type="geomtools::stacked_model"]
stacked.axis            : string = "x"
stacked.number_of_items : integer = 1
stacked.model_0  : string = "snemo_strip_23_pad.realistic.model"
stacked.label_0  : string = "pad_bundle_0"
material.ref     : string = "tracking_gas"
visibility.color : string = "green"
# Mapping the source foil from its parent category :
mapping.daughter_id.pad_bundle_0 : string = "[source_pad:pad=0]"

 
###################################################################
#
# This model represents the realistic layout of the strip 24
# 
[name="snemo_strip_24.realistic.model" type="geomtools::stacked_model"]
stacked.axis            : string = "x"
stacked.number_of_items : integer = 1
stacked.model_0  : string = "snemo_strip_24_pad.realistic.model"
stacked.label_0  : string = "pad_bundle_0"
material.ref     : string = "tracking_gas"
visibility.color : string = "green"
# Mapping the source foil from its parent category :
mapping.daughter_id.pad_bundle_0 : string = "[source_pad:pad=0]"
 
 
###################################################################
#
# This model represents the realistic layout of the strip 25
# 
[name="snemo_strip_25.realistic.model" type="geomtools::stacked_model"]
stacked.axis            : string = "x"
stacked.number_of_items : integer = 1
stacked.model_0  : string = "snemo_strip_25_pad.realistic.model"
stacked.label_0  : string = "pad_bundle_0"
material.ref     : string = "tracking_gas"
visibility.color : string = "green"
# Mapping the source foil from its parent category :
mapping.daughter_id.pad_bundle_0 : string = "[source_pad:pad=0]"

 
###################################################################
#
# This model represents the realistic layout of the strip 26
# 
[name="snemo_strip_26.realistic.model" type="geomtools::stacked_model"]
stacked.axis            : string = "x"
stacked.number_of_items : integer = 1
stacked.model_0  : string = "snemo_strip_26_pad.realistic.model"
stacked.label_0  : string = "pad_bundle_0"
material.ref     : string = "tracking_gas"
visibility.color : string = "green"
# Mapping the source foil from its parent category :
mapping.daughter_id.pad_bundle_0 : string = "[source_pad:pad=0]"

 
###################################################################
#
# This model represents the realistic layout of the strip 27
# 
[name="snemo_strip_27.realistic.model" type="geomtools::stacked_model"]
stacked.axis            : string = "x"
stacked.number_of_items : integer = 1
stacked.model_0  : string = "snemo_strip_27_pad.realistic.model"
stacked.label_0  : string = "pad_bundle_0"
material.ref     : string = "tracking_gas"
visibility.color : string = "green"
# Mapping the source foil from its parent category :
mapping.daughter_id.pad_bundle_0 : string = "[source_pad:pad=0]"

 
###################################################################
#
# This model represents the realistic layout of the strip 28
# 
[name="snemo_strip_28.realistic.model" type="geomtools::stacked_model"]
stacked.axis            : string = "x"
stacked.number_of_items : integer = 1
stacked.model_0  : string = "snemo_strip_28_pad.realistic.model"
stacked.label_0  : string = "pad_bundle_0"
material.ref     : string = "tracking_gas"
visibility.color : string = "green"
# Mapping the source foil from its parent category :
mapping.daughter_id.pad_bundle_0 : string = "[source_pad:pad=0]"

 
###################################################################
#
# This model represents the realistic layout of the strip 31
# 
[name="snemo_strip_31.realistic.model" type="geomtools::stacked_model"]
stacked.axis            : string = "x"
stacked.number_of_items : integer = 1
stacked.model_0  : string = "snemo_strip_31_pad.realistic.model"
stacked.label_0  : string = "pad_bundle_0"
material.ref     : string = "tracking_gas"
visibility.color : string = "green"
# Mapping the source foil from its parent category :
mapping.daughter_id.pad_bundle_0 : string = "[source_pad:pad=0]"

 
###################################################################
#
# This model represents the realistic layout of the strip 32
# 
[name="snemo_strip_32.realistic.model" type="geomtools::stacked_model"]
stacked.axis            : string = "x"
stacked.number_of_items : integer = 1
stacked.model_0  : string = "snemo_strip_32_pad.realistic.model"
stacked.label_0  : string = "pad_bundle_0"
material.ref     : string = "tracking_gas"
visibility.color : string = "green"
# Mapping the source foil from its parent category :
mapping.daughter_id.pad_bundle_0 : string = "[source_pad:pad=0]"

 
###################################################################
#
# This model represents the realistic layout of the strip 33
# 
[name="snemo_strip_33.realistic.model" type="geomtools::stacked_model"]
stacked.axis            : string = "x"
stacked.number_of_items : integer = 1
stacked.model_0  : string = "snemo_strip_33_pad.realistic.model"
stacked.label_0  : string = "pad_bundle_0"
material.ref     : string = "tracking_gas"
visibility.color : string = "green"
# Mapping the source foil from its parent category :
mapping.daughter_id.pad_bundle_0 : string = "[source_pad:pad=0]"

 
###################################################################
#
# This model represents the realistic layout of the strip 34
# 
[name="snemo_strip_34.realistic.model" type="geomtools::stacked_model"]
stacked.axis            : string = "x"
stacked.number_of_items : integer = 1
stacked.model_0  : string = "snemo_strip_34_pad.realistic.model"
stacked.label_0  : string = "pad_bundle_0"
material.ref     : string = "tracking_gas"
visibility.color : string = "green"
# Mapping the source foil from its parent category :
mapping.daughter_id.pad_bundle_0 : string = "[source_pad:pad=0]"


# end