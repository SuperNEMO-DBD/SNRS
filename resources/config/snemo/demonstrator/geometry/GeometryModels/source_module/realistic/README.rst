================================================================================
Configuration files for the description of realistic ITEP-like source strips
================================================================================

:author: F.Mauger
:date: 2021-10-25

Geometry models
===============

The geometry modeling of the  SuperNEMO ITEP-like source strips uses a
 three-level approach for each strip:

 #. Top level:  this is the  mother strip path  which is a  box volume
    filled with tracking  gas and hosts the strip; this  volume can be
    placed in the geometry model of a Falaise block;
 #. Intermediate level: this is the strip itself which is a box volume
    filled with tracking gas and hosts the real source pads and films;
 #. End level:  this description level  is dedicated to  the effective
    description of the deformed source  pad and its two wrapping Mylar
    films (*back*  and *front*); it uses  the ``snrs::mesh_pad_model``
    provided by the SNRS package and  some special data files from the
    ``resources/data/``  directory which  describe  the 3D-meshes  for
    each pad and films.

    
Files
=====

* ``strips_itep_like.geom.template`` : template  configuration file for
  the description of  a ITEP-like source strip and  deformed pad using
  the SNRS geometry model (``snrs::mesh_pad_model``)
* ``gen_geom_strip_conf.bash`` :  Shell script  for the  generation of
  the  configuration  file  for  all  geometry  models  associated  to
  ITEP-like source strips  at positionse 3, 8, 9, 14,  15, 20, 21, 22,
  23, 24, 25, 26, 27, 28, 31, 32, 33 and 34 (pposition 2 is considered
  as flat and use the legacy geometry model from Falaise).
* ``strips_itep_like.geom`` : Generated configuration file.


Usage
=====

**Note**:  For developers  of  the  SNRS package  only  (but could  be
 automated within CMake at build step).

From the ``resources/config/snemo/demonstrator/geometry/GeometryModels/source_module/realistic/`` source directory:

.. code:: shell

   $ bash gen_geom_strip_conf.bash
   $ less strips_itep_like.geom
..


.. end
   
