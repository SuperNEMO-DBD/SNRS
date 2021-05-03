# -*- mode: conf-unix; -*-
# Author : F.Mauger
# Date   : April 2021
# Realistic geometry model of (LAPP-)ITEP source strips 

 
###################################################################
#
# This model generates automatically the "snemo_strip_32_realistic.source.model"
# model and its associated logical volume as well as the
# "snemo_strip_32_realistic.back_film.model" and "snemo_strip_32_realistic.front_film.model" models.
# 
[name="snemo_strip_32_realistic.model" type="snrs::mesh_pad_model"]

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
mapping.daughter_id.back_film   : string = "[source_strip_film:film=0]"
mapping.daughter_id.source      : string = "[source_pad:pad=0]"
mapping.daughter_id.front_film  : string = "[source_strip_film:film=1]"

 
###################################################################
#
# This model generates automatically the "snemo_strip_33_realistic.source.model"
# model and its associated logical volume as well as the
# "snemo_strip_33_realistic.back_film.model" and "snemo_strip_33_realistic.front_film.model" models.
# 
[name="snemo_strip_33_realistic.model" type="snrs::mesh_pad_model"]

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
mapping.daughter_id.back_film   : string = "[source_strip_film:film=0]"
mapping.daughter_id.source      : string = "[source_pad:pad=0]"
mapping.daughter_id.front_film  : string = "[source_strip_film:film=1]"


###################################################################
#
# This model generates automatically the "snemo_strip_34_realistic.source.model"
# model and its associated logical volume as well as the
# "snemo_strip_34_realistic.back_film.model" and "snemo_strip_34_realistic.front_film.model" models.
# 
[name="snemo_strip_34_realistic.model" type="snrs::mesh_pad_model"]

#@config Strip 34 model (LAPP-ITEP-4)

#@description Strip ID:
strip_id : integer = 34

# #@description Depth of the gas volume around the source strip (along the X-axis)
# depth : real as length = 52 mm

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
mapping.daughter_id.back_film   : string = "[source_strip_film:film=0]"
mapping.daughter_id.source      : string = "[source_pad:pad=0]"
mapping.daughter_id.front_film  : string = "[source_strip_film:film=1]"

# end