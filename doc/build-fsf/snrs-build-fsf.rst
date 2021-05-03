======================================================================================
SNRS - Foil Shaping Fit from LTD datasets
======================================================================================

:author: F.Mauger
:date: 2021-04-18


The SNRS package contains some tools to build a realistic geometry model
for a given source strip based on a fit of *laser tracker* data.

Programs:

* ``snrs-build-fsf`` : parser for the raw LTD datasets and generation of calibrated LTD datasets in the SuperNEMO/Falaise frame
* ``build-fsf.bash`` : wrapper scripts to automate the launch of the ``snrs-build-fsf`` executable for a set of selected source strips


Building FSF datasets
=======================================================


#. Preparing the setup: 

   * Define the path to the working SNRS data base directory from and where high-level data files
     will be loaded and stored:
 
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

   * ``strip-34-pad-0-shaping.report`` is a CSV formatted ASCII file which contains the result of the fit for the strip (for now, only elliptical fit is available)
   * ``strip-34-pad-0-shaped.dat`` is a ASCII file which contains the data (in a specific format)
     that describe the geometry of the source pad after fitting 
   * ``strip-34-pad-0-tessellated.dat`` is a ASCII file which contains the data (in a specific format known by Bayeux >=3.4.5)
     that describe the tesselated shape of the source pad.


   
.. raw:: pdf

   PageBreak
..
	    

Datasets per individual source strip 
=====================================

* Plot data (CSV ASCII format) and images files  are generated in the  ``${SNRS_DATA_DIR}/geometry/source_foils/fsf/images/`` directory.


Strip 32
--------

+--------------------------------------------------------------------+--------------------------------------------------------------------+
|  Fit report                                                        |  Fitting of Z-band LDT                                             |
|                                                                    |                                                                    |
|                                                                    |                                                                    |
| .. image:: images/strip-32-pad-0-fit-report.png                    |  .. image:: images/strip-32-pad-0-fit-zbands.png                   |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
| Generated 3D-mesh                                                  | Generated 3D-mesh (zoom at the top of the strip)                   |
|                                                                    |                                                                    |
| .. image:: images/strip-32-pad-0-mesh.png                          |  .. image:: images/strip-32-pad-0-mesh-zoom-top.png                |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
 

   
.. raw:: pdf

   PageBreak
..



Strip 33
--------

+--------------------------------------------------------------------+--------------------------------------------------------------------+
|  Fit report                                                        |  Fitting of Z-band LDT                                             |
|                                                                    |                                                                    |
|                                                                    |                                                                    |
| .. image:: images/strip-33-pad-0-fit-report.png                    |  .. image:: images/strip-33-pad-0-fit-zbands.png                   |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
| Generated 3D-mesh                                                  | Generated 3D-mesh (zoom at the top of the strip)                   |
|                                                                    |                                                                    |
| .. image:: images/strip-33-pad-0-mesh.png                          |  .. image:: images/strip-33-pad-0-mesh-zoom-top.png                |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
 


   
.. raw:: pdf

   PageBreak
..



Strip 34
--------

+--------------------------------------------------------------------+--------------------------------------------------------------------+
|  Fit report                                                        |  Fitting of Z-band LDT                                             |
|                                                                    |                                                                    |
|                                                                    |                                                                    |
| .. image:: images/strip-34-pad-0-fit-report.png                    |  .. image:: images/strip-34-pad-0-fit-zbands.png                   |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
| Generated 3D-mesh                                                  | Generated 3D-mesh (zoom at the top of the strip)                   |
|                                                                    |                                                                    |
| .. image:: images/strip-34-pad-0-mesh.png                          |  .. image:: images/strip-34-pad-0-mesh-zoom-top.png                |
|    :width: 100%                                                    |     :width: 100%                                                   |
|                                                                    |                                                                    |
+--------------------------------------------------------------------+--------------------------------------------------------------------+ 
 

   
.. raw:: pdf

   PageBreak
..



       
       


