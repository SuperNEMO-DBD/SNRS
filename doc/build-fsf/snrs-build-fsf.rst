======================================================================================
SNRS - Foil Shaping Fit from calibrated LTD datasets
======================================================================================

:author: F.Mauger
:date: 2021-05-29


The SNRS  package contains  some tools to  build a  realistic geometry
model for a given source strip based  on a fit of *laser tracker* data
build from the raw LTD dataset (DocDb \#5400).

.. 2 (LAPP-ITEP 7)
   
This work applies to strips
3 (LAPP-ITEP-3),
8 (LAPP-ITEP 2), 9 (ITEP-11),
14 (ITEP-10), 15 (ITEP-9),
20 (ITEP-8), 21 (ITEP-7),
22 (ITEP-6), 23 (ITEP-5),
24 (ITEP-4), 25 (ITEP-3),
26 (ITEP-2), 27 (ITEP-1),
28 (LAPP-ITEP-21), 31 (LAPP-ITEP-1), 
32 (LAPP-ITEP-5), 33 (LAPP-ITEP-6) and
34 (LAPP-ITEP-4) with meshes of 10x100 tiles.

Strip 2 (LAPP-ITEP 7) has not been processed.


Programs:

* ``snrs-build-fsf`` :

  * processing of the calibrated LTD datasets,
  * fitting an elliptic model for each ITEP-style strip,
  * post-processing (smoothing) of the fit data, 
  * generating 3D-meshes for each source strip and associated
    surrounding mylar films.

* ``build-fsf.bash`` : wrapper  scripts to automate the  launch of the
  ``snrs-build-fsf`` executable for a set of selected source strips


Building FSF datasets
=======================================================

#. Preparing the setup: 

   Define the path to the working  SNRS data base directory from and
   where high-level data files will be loaded and stored:
 
   .. code:: shell
	     
      $ export SNRS_DATA_DIR="/data/SuperNEMO/snrs_data"
   ..
   
#. Run the  FSF algorithm on a selection of source strips:
   
   .. code:: shell
	  
      $ ./scripts/build-fsf.bash
   ..

#. Generated FSF datasets are stored in ``${SNRS_DATA_DIR}/geometry/source_foils/fsf``:
  
   .. code:: shell
	  
      $ cat ${SNRS_DATA_DIR}/geometry/source_foils/fsf/processed-strips.lis
      $ find ${SNRS_DATA_DIR}/geometry/source_foils/fsf -maxdepth 1 -name "strip-*" -exec basename \{\} \;
      ...
      strip-34-pad-0-shaping.report
      strip-34-pad-0-shaped.dat
      strip-34-pad-0-tessellated.dat
   ..

   Description of the generated files (example for strip 34) :

   * ``strip-34-pad-0-shaping.report`` is  a CSV formatted  ASCII file
     which contains the result of the fit for the strip (for now, only
     elliptical fit is available)
   * ``strip-34-pad-0-shaped.dat`` is a ASCII  file which contains the
     data (in  a specific  format) that describe  the geometry  of the
     source pad after fitting
   * ``strip-34-pad-0-tessellated.dat`` is a ASCII file which contains
     the  data (in  a specific  format known  by Bayeux  >=3.5) that
     describe the tesselated shape of the source pad.


#. Publishing the mesh datasets:

   The SNRS package provides some specific actions to:

   - build the 3D mesh datasets,
   - install them in the source directory for the official release.

   .. code:: raw
	     
      $ make snrsBuildFsfDataset
   ..

   * The mesh datasets for selenium source foil and mylar films are stored in:
     ``resources/data/geometry/source_foils/fsf``.
   * The geometry models are defined in:
     ``resources/config/snemo/demonstrator/geometry/GeometryModels/``
     ``source_module/realistic/strips_itep_like.geom``.
   

#. Generate documentation:
   
   .. code:: raw
	     
      $ make docBuildFsf
   ..
       
.. raw:: pdf

   PageBreak
..
	    

Datasets per individual source strip 
=====================================

* Plot data (CSV  ASCII format) and images files are  generated in the
  ``${SNRS_DATA_DIR}/geometry/source_foils/fsf/images/`` directory and addressed through a symbolic link.
* Report on elliptic fit :

  The top-left graph  shows the evolution of the  four parameters *a*,
  *b*, *x0* and *y0* depending on the Z-band (0 : bottom, 100 : top).

  * the *a* and *x0* parameters are necessarily strongly correlated.
  * the *y0* parameter represents ideally  the position of the central
    vertical  axis of  the strip  at Z  coordinate. It  is shown  here
    biased by  the theoretical  position of the  vertical axis  of the
    strip.  In principle, it should  display a horizontal line but the
    real positions of the strips  show some significant vertical shift
    for most strips.


.. raw:: pdf

   PageBreak
..


Strip 3
--------

+--------------------------------------------------------------------+--------------------------------------------------------------------+
|  Fit report                                                        |  Fitting of Z-bands LTD (3D view)                                  |
|                                                                    |                                                                    |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-3-pad-0-fit-report.png                     |  .. image:: tmpimg/strip-3-pad-0-fit-zbands.png                    |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
| Generated 3D-mesh                                                  | Generated 3D-mesh (zoom at the top of the strip)                   |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-3-pad-0-mesh.png                           |  .. image:: tmpimg/strip-3-pad-0-mesh-zoom-top.png                 |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
| Generated 3D-mesh (YZ view)                                        |  Fitting of Z-bands LTD  (YX view)                                 |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-3-pad-0-mesh-yz.png                        |  .. image:: tmpimg/strip-3-pad-0-fit-vs-zbands.png                 |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
   
.. raw:: pdf

   PageBreak
..


Strip 8
--------

+--------------------------------------------------------------------+--------------------------------------------------------------------+
|  Fit report                                                        |  Fitting of Z-bands LTD                                            |
|                                                                    |                                                                    |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-8-pad-0-fit-report.png                     |  .. image:: tmpimg/strip-8-pad-0-fit-zbands.png                    |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
| Generated 3D-mesh                                                  | Generated 3D-mesh (zoom at the top of the strip)                   |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-8-pad-0-mesh.png                           |  .. image:: tmpimg/strip-8-pad-0-mesh-zoom-top.png                 |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
| Generated 3D-mesh (YZ view)                                        |  Fitting of Z-bands LTD (YX view)                                  |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-8-pad-0-mesh-yz.png                        |  .. image:: tmpimg/strip-8-pad-0-fit-vs-zbands.png                 |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
   
.. raw:: pdf

   PageBreak
..



Strip 9
--------

+--------------------------------------------------------------------+--------------------------------------------------------------------+
|  Fit report                                                        |  Fitting of Z-bands LTD                                            |
|                                                                    |                                                                    |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-9-pad-0-fit-report.png                     |  .. image:: tmpimg/strip-9-pad-0-fit-zbands.png                    |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
| Generated 3D-mesh                                                  | Generated 3D-mesh (zoom at the top of the strip)                   |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-9-pad-0-mesh.png                           |  .. image:: tmpimg/strip-9-pad-0-mesh-zoom-top.png                 |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
| Generated 3D-mesh (YZ view)                                        |  Fitting of Z-bands LTD (YX view)                                  |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-9-pad-0-mesh-yz.png                        |  .. image:: tmpimg/strip-9-pad-0-fit-vs-zbands.png                 |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
   
.. raw:: pdf

   PageBreak
..



Strip 14
--------

+--------------------------------------------------------------------+--------------------------------------------------------------------+
|  Fit report                                                        |  Fitting of Z-bands LTD                                            |
|                                                                    |                                                                    |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-14-pad-0-fit-report.png                    |  .. image:: tmpimg/strip-14-pad-0-fit-zbands.png                   |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
| Generated 3D-mesh                                                  | Generated 3D-mesh (zoom at the top of the strip)                   |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-14-pad-0-mesh.png                          |  .. image:: tmpimg/strip-14-pad-0-mesh-zoom-top.png                |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
| Generated 3D-mesh (YZ view)                                        |  Fitting of Z-bands LTD (YX view)                                  |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-14-pad-0-mesh-yz.png                       |  .. image:: tmpimg/strip-14-pad-0-fit-vs-zbands.png                |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
   

.. raw:: pdf

   PageBreak
..


Strip 15
--------

+--------------------------------------------------------------------+--------------------------------------------------------------------+
|  Fit report                                                        |  Fitting of Z-bands LTD                                            |
|                                                                    |                                                                    |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-15-pad-0-fit-report.png                    |  .. image:: tmpimg/strip-15-pad-0-fit-zbands.png                   |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
| Generated 3D-mesh                                                  | Generated 3D-mesh (zoom at the top of the strip)                   |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-15-pad-0-mesh.png                          |  .. image:: tmpimg/strip-15-pad-0-mesh-zoom-top.png                |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
| Generated 3D-mesh (YZ view)                                        |  Fitting of Z-bands LTD (YX view)                                  |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-15-pad-0-mesh-yz.png                       |  .. image:: tmpimg/strip-15-pad-0-fit-vs-zbands.png                |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 

Strip 15 is strongly deformed and  has been installed in a special way
to avoid contact with the field wires of the first planes of tracker cells
on both sides of the source frame.
One consequence is a rather large shift of the vertical axis (yellow
curve above).

.. raw:: pdf

   PageBreak
..


Strip 20
--------

+--------------------------------------------------------------------+--------------------------------------------------------------------+
|  Fit report                                                        |  Fitting of Z-bands LTD                                            |
|                                                                    |                                                                    |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-20-pad-0-fit-report.png                    |  .. image:: tmpimg/strip-20-pad-0-fit-zbands.png                   |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
| Generated 3D-mesh                                                  | Generated 3D-mesh (zoom at the top of the strip)                   |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-20-pad-0-mesh.png                          |  .. image:: tmpimg/strip-20-pad-0-mesh-zoom-top.png                |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
| Generated 3D-mesh (YZ view)                                        |  Fitting of Z-bands LTD (YX view)                                  |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-20-pad-0-mesh-yz.png                       |  .. image:: tmpimg/strip-20-pad-0-fit-vs-zbands.png                |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 

.. raw:: pdf

   PageBreak
..


Strip 21
--------

+--------------------------------------------------------------------+--------------------------------------------------------------------+
|  Fit report                                                        |  Fitting of Z-bands LTD                                            |
|                                                                    |                                                                    |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-21-pad-0-fit-report.png                    |  .. image:: tmpimg/strip-21-pad-0-fit-zbands.png                   |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
| Generated 3D-mesh                                                  | Generated 3D-mesh (zoom at the top of the strip)                   |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-21-pad-0-mesh.png                          |  .. image:: tmpimg/strip-21-pad-0-mesh-zoom-top.png                |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
| Generated 3D-mesh (YZ view)                                        |  Fitting of Z-bands LTD (YX view)                                  |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-21-pad-0-mesh-yz.png                       |  .. image:: tmpimg/strip-21-pad-0-fit-vs-zbands.png                |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
 
.. raw:: pdf

   PageBreak
..


Strip 22
--------

+--------------------------------------------------------------------+--------------------------------------------------------------------+
|  Fit report                                                        |  Fitting of Z-bands LTD                                            |
|                                                                    |                                                                    |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-22-pad-0-fit-report.png                    |  .. image:: tmpimg/strip-22-pad-0-fit-zbands.png                   |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
| Generated 3D-mesh                                                  | Generated 3D-mesh (zoom at the top of the strip)                   |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-22-pad-0-mesh.png                          |  .. image:: tmpimg/strip-22-pad-0-mesh-zoom-top.png                |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
| Generated 3D-mesh (YZ view)                                        |  Fitting of Z-bands LTD (YX view)                                  |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-22-pad-0-mesh-yz.png                       |  .. image:: tmpimg/strip-22-pad-0-fit-vs-zbands.png                |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 

.. raw:: pdf

   PageBreak
..


Strip 23
--------

+--------------------------------------------------------------------+--------------------------------------------------------------------+
|  Fit report                                                        |  Fitting of Z-bands LTD                                            |
|                                                                    |                                                                    |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-23-pad-0-fit-report.png                    |  .. image:: tmpimg/strip-23-pad-0-fit-zbands.png                   |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
| Generated 3D-mesh                                                  | Generated 3D-mesh (zoom at the top of the strip)                   |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-23-pad-0-mesh.png                          |  .. image:: tmpimg/strip-23-pad-0-mesh-zoom-top.png                |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
| Generated 3D-mesh (YZ view)                                        |  Fitting of Z-bands LTD (YX view)                                  |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-23-pad-0-mesh-yz.png                       |  .. image:: tmpimg/strip-23-pad-0-fit-vs-zbands.png                |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 

.. raw:: pdf

   PageBreak
..


Strip 24
--------

+--------------------------------------------------------------------+--------------------------------------------------------------------+
|  Fit report                                                        |  Fitting of Z-bands LTD                                            |
|                                                                    |                                                                    |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-24-pad-0-fit-report.png                    |  .. image:: tmpimg/strip-24-pad-0-fit-zbands.png                   |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
| Generated 3D-mesh                                                  | Generated 3D-mesh (zoom at the top of the strip)                   |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-24-pad-0-mesh.png                          |  .. image:: tmpimg/strip-24-pad-0-mesh-zoom-top.png                |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
| Generated 3D-mesh (YZ view)                                        |  Fitting of Z-bands LTD (YX view)                                  |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-24-pad-0-mesh-yz.png                       |  .. image:: tmpimg/strip-24-pad-0-fit-vs-zbands.png                |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 

.. raw:: pdf

   PageBreak
..


Strip 25
--------

+--------------------------------------------------------------------+--------------------------------------------------------------------+
|  Fit report                                                        |  Fitting of Z-bands LTD                                            |
|                                                                    |                                                                    |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-25-pad-0-fit-report.png                    |  .. image:: tmpimg/strip-25-pad-0-fit-zbands.png                   |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
| Generated 3D-mesh                                                  | Generated 3D-mesh (zoom at the top of the strip)                   |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-25-pad-0-mesh.png                          |  .. image:: tmpimg/strip-25-pad-0-mesh-zoom-top.png                |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
| Generated 3D-mesh (YZ view)                                        |  Fitting of Z-bands LTD (YX view)                                  |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-25-pad-0-mesh-yz.png                       |  .. image:: tmpimg/strip-25-pad-0-fit-vs-zbands.png                |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 

.. raw:: pdf

   PageBreak
..



Strip 26
--------

+--------------------------------------------------------------------+--------------------------------------------------------------------+
|  Fit report                                                        |  Fitting of Z-bands LTD                                            |
|                                                                    |                                                                    |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-26-pad-0-fit-report.png                    |  .. image:: tmpimg/strip-26-pad-0-fit-zbands.png                   |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
| Generated 3D-mesh                                                  | Generated 3D-mesh (zoom at the top of the strip)                   |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-26-pad-0-mesh.png                          |  .. image:: tmpimg/strip-26-pad-0-mesh-zoom-top.png                |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
| Generated 3D-mesh (YZ view)                                        |  Fitting of Z-bands LTD (YX view)                                  |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-26-pad-0-mesh-yz.png                       |  .. image:: tmpimg/strip-26-pad-0-fit-vs-zbands.png                |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 

.. raw:: pdf

   PageBreak
..



Strip 27
--------

+--------------------------------------------------------------------+--------------------------------------------------------------------+
|  Fit report                                                        |  Fitting of Z-bands LTD                                            |
|                                                                    |                                                                    |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-27-pad-0-fit-report.png                    |  .. image:: tmpimg/strip-27-pad-0-fit-zbands.png                   |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
| Generated 3D-mesh                                                  | Generated 3D-mesh (zoom at the top of the strip)                   |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-27-pad-0-mesh.png                          |  .. image:: tmpimg/strip-27-pad-0-mesh-zoom-top.png                |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
| Generated 3D-mesh (YZ view)                                        |  Fitting of Z-bands LTD (YX view)                                  |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-27-pad-0-mesh-yz.png                       |  .. image:: tmpimg/strip-27-pad-0-fit-vs-zbands.png                |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
 
.. raw:: pdf

   PageBreak
..


Strip 28
--------

+--------------------------------------------------------------------+--------------------------------------------------------------------+
|  Fit report                                                        |  Fitting of Z-bands LTD                                            |
|                                                                    |                                                                    |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-28-pad-0-fit-report.png                    |  .. image:: tmpimg/strip-28-pad-0-fit-zbands.png                   |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
| Generated 3D-mesh                                                  | Generated 3D-mesh (zoom at the top of the strip)                   |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-28-pad-0-mesh.png                          |  .. image:: tmpimg/strip-28-pad-0-mesh-zoom-top.png                |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
| Generated 3D-mesh (YZ view)                                        |  Fitting of Z-bands LTD (YX view)                                  |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-28-pad-0-mesh-yz.png                       |  .. image:: tmpimg/strip-28-pad-0-fit-vs-zbands.png                |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 

.. raw:: pdf

   PageBreak
..


Strip 31
--------

+--------------------------------------------------------------------+--------------------------------------------------------------------+
|  Fit report                                                        |  Fitting of Z-bands LTD                                            |
|                                                                    |                                                                    |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-31-pad-0-fit-report.png                    |  .. image:: tmpimg/strip-31-pad-0-fit-zbands.png                   |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
| Generated 3D-mesh                                                  | Generated 3D-mesh (zoom at the top of the strip)                   |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-31-pad-0-mesh.png                          |  .. image:: tmpimg/strip-31-pad-0-mesh-zoom-top.png                |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
| Generated 3D-mesh (YZ view)                                        |  Fitting of Z-bands LTD (YX view)                                  |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-31-pad-0-mesh-yz.png                       |  .. image:: tmpimg/strip-31-pad-0-fit-vs-zbands.png                |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 

.. raw:: pdf

   PageBreak
..


Strip 32
--------

+--------------------------------------------------------------------+--------------------------------------------------------------------+
|  Fit report                                                        |  Fitting of Z-bands LTD                                            |
|                                                                    |                                                                    |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-32-pad-0-fit-report.png                    |  .. image:: tmpimg/strip-32-pad-0-fit-zbands.png                   |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
| Generated 3D-mesh                                                  | Generated 3D-mesh (zoom at the top of the strip)                   |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-32-pad-0-mesh.png                          |  .. image:: tmpimg/strip-32-pad-0-mesh-zoom-top.png                |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
| Generated 3D-mesh (YZ view)                                        |  Fitting of Z-bands LTD (YX view)                                  |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-32-pad-0-mesh-yz.png                       |  .. image:: tmpimg/strip-32-pad-0-fit-vs-zbands.png                |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 

.. raw:: pdf

   PageBreak
..



Strip 33
--------

+--------------------------------------------------------------------+--------------------------------------------------------------------+
|  Fit report                                                        |  Fitting of Z-bands LTD                                            |
|                                                                    |                                                                    |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-33-pad-0-fit-report.png                    |  .. image:: tmpimg/strip-33-pad-0-fit-zbands.png                   |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
| Generated 3D-mesh                                                  | Generated 3D-mesh (zoom at the top of the strip)                   |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-33-pad-0-mesh.png                          |  .. image:: tmpimg/strip-33-pad-0-mesh-zoom-top.png                |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
| Generated 3D-mesh (YZ view)                                        |  Fitting of Z-bands LTD (YX view)                                  |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-33-pad-0-mesh-yz.png                       |  .. image:: tmpimg/strip-33-pad-0-fit-vs-zbands.png                |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 

.. raw:: pdf

   PageBreak
..



Strip 34
--------

+--------------------------------------------------------------------+--------------------------------------------------------------------+
|  Fit report                                                        |  Fitting of Z-bands LTD                                            |
|                                                                    |                                                                    |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-34-pad-0-fit-report.png                    |  .. image:: tmpimg/strip-34-pad-0-fit-zbands.png                   |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
| Generated 3D-mesh                                                  | Generated 3D-mesh (zoom at the top of the strip)                   |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-34-pad-0-mesh.png                          |  .. image:: tmpimg/strip-34-pad-0-mesh-zoom-top.png                |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
| Generated 3D-mesh (YZ view)                                        |  Fitting of Z-bands LTD (YX view)                                  |
|                                                                    |                                                                    |
| .. image:: tmpimg/strip-34-pad-0-mesh-yz.png                       |  .. image:: tmpimg/strip-34-pad-0-fit-vs-zbands.png                |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 

.. raw:: pdf

   PageBreak
..



       
       


