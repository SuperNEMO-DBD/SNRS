======================================================================================
SNRS - Extraction of source strips calibrated 3D geometry data from LTD datasets
======================================================================================

:author: F.Mauger
:date: 2021-05-29


The SNRS package  contains some tools to build  calibrated 3D geometry
datasets from the *laser tracker* raw data described in DocDB \#5400.

Programs:

* ``snrs-build-ltd`` : parser for the  raw LTD datasets and generation
  of calibrated LTD datasets in the SuperNEMO/Falaise frame
* ``build-ltd.bash`` :  wrapper script  to automate  the lauch  of the
  ``snrs-build-ltd`` executable for a set of selected source strips
* ``plot_ltd_strip.gp`` : Gnuplot script to generate images of the LTD
  datasets for selected source strips


Building calibrated  3D geometry data from LTD datasets
=======================================================


#. Preparing the setup: 

   * Define the path to the raw LTD datasets:
   
     .. code:: shell

	$ export RAW_LTD_DATA_DIR="/data/SuperNEMO/RealSources/3D_measurements/source_foils"
     ..

   * Define the path to the SNRS data base directory where high-level generated data files
     will be stored:
 
     .. code:: shell

	$ export SNRS_DATA_DIR="/data/SuperNEMO/snrs_data"
     ..
   
#. Run the individual source strips' LTD data builder (ITEP strip model only):
   
   .. code:: shell
	  
      $ ./scripts/build-ltd.bash
   ..

#. The datafiles containing the measurements of the fiducal edge for each strip are stored in :
   ``resources/data/geometry/source_foils/ltd``.

#. Generated LTD datasets are stored in ``${SNRS_DATA_DIR}/geometry/source_foils/ltd``:
  
   .. code:: shell
	  
      $ cat ${SNRS_DATA_DIR}/geometry/source_foils/ltd/processed-strips.lis
      ...
      $ find ${SNRS_DATA_DIR}/geometry/source_foils/ltd -maxdepth 1 -name "strip-*" -exec basename \{\} \;
      ltd-strip-2.dat
      ltd-strip-3.dat
      ...
      ltd-strip-32.dat
      ltd-strip-33.dat
      ltd-strip-34.dat
   ..

   **Comments on the format:**

   * This  is an  ASCII  raw  format which  can  be  parsed using  the
     ``snrs::ltd::load`` method to fill a ``snrs::ltd`` instance.
   * The generated files  contains the list of  selected laser tracker
     points associated to the  specific source strip.  Coordinates are
     expressed in the SuperNEMO/Falaise  frame (unit: mm).  Each point
     is also given a local density weight.
   * Some voxelization data  are also stored in the  file (voxel size:
     1cm x 1cm x 1cm).

   
   **TODO:**  Consider  to  process  source strips  with  LAPP  layout
    (8-pads strips) is needed.

#. Generate documentation:
   
   .. code:: raw
	     
      $ make docBuildLtd
   ..
   
..
      .. raw:: pdf
..    PageBreak
..


Individual per source strip datasets 
=====================================

* Plot data (CSV  ASCII format) and images files are  generated in the
  ``${SNRS_DATA_DIR}/geometry/source_foils/ltd/images`` directory.
* The set of points is displayed  in the SuperNEMO/Falaise frame and a
  special  Z-band (Z=0,  DZ=1  mm) is  highlighted  to illustrate  the
  ability  of  the ``snrs::ltd``  class  to  select subsets  of  laser
  tracker points as  an input for a fit  algorithm (see ``snrs::fsfs``
  class).
* For now,  we provide  only the  result of  the processing  of source
  strips of the ITEP layout (large single bands).
    
   +-------+-----------------------------------------+-----------------------------------------+
   | Strip |	    Top view                         |       3D view                           |
   +-------+-----------------------------------------+-----------------------------------------+
   |  2    | .. image:: tmpimg/ltd-strip-2-yx.png    | .. image:: tmpimg/ltd-strip-2-3d.png    |
   |       |    :width: 90%                          |    :width: 90%                          | 
   |       |	                                     |                                         |
   +-------+-----------------------------------------+-----------------------------------------+
   |  3    | .. image:: tmpimg/ltd-strip-3-yx.png    | .. image:: tmpimg/ltd-strip-3-3d.png    |
   |       |    :width: 90%                          |    :width: 90%                          | 
   |       |	                                     |                                         |
   +-------+-----------------------------------------+-----------------------------------------+
   |  8    | .. image:: tmpimg/ltd-strip-8-yx.png    | .. image:: tmpimg/ltd-strip-8-3d.png    |
   |       |    :width: 90%                          |    :width: 90%                          | 
   |       |	                                     |                                         |
   +-------+-----------------------------------------+-----------------------------------------+
   |  9    | .. image:: tmpimg/ltd-strip-9-yx.png    | .. image:: tmpimg/ltd-strip-9-3d.png    |
   |       |    :width: 90%                          |    :width: 90%                          | 
   |       |	                                     |                                         |
   +-------+-----------------------------------------+-----------------------------------------+
   |  14   | .. image:: tmpimg/ltd-strip-14-yx.png   | .. image:: tmpimg/ltd-strip-14-3d.png   |
   |       |    :width: 90%                          |    :width: 90%                          | 
   |       |	                                     |                                         |
   +-------+-----------------------------------------+-----------------------------------------+
   |  15   | .. image:: tmpimg/ltd-strip-15-yx.png   | .. image:: tmpimg/ltd-strip-15-3d.png   |
   |       |    :width: 90%                          |    :width: 90%                          | 
   |       |	                                     |                                         |
   +-------+-----------------------------------------+-----------------------------------------+
   |  20   | .. image:: tmpimg/ltd-strip-20-yx.png   | .. image:: tmpimg/ltd-strip-20-3d.png   |
   |       |    :width: 90%                          |    :width: 90%                          | 
   |       |	                                     |                                         |
   +-------+-----------------------------------------+-----------------------------------------+
   |  21   | .. image:: tmpimg/ltd-strip-21-yx.png   | .. image:: tmpimg/ltd-strip-21-3d.png   |
   |       |    :width: 90%                          |    :width: 90%                          | 
   |       |	                                     |                                         |
   +-------+-----------------------------------------+-----------------------------------------+
   |  22   | .. image:: tmpimg/ltd-strip-22-yx.png   | .. image:: tmpimg/ltd-strip-22-3d.png   |
   |       |    :width: 90%                          |    :width: 90%                          | 
   |       |	                                     |                                         |
   +-------+-----------------------------------------+-----------------------------------------+
   |  23   | .. image:: tmpimg/ltd-strip-23-yx.png   | .. image:: tmpimg/ltd-strip-23-3d.png   |
   |       |    :width: 90%                          |    :width: 90%                          | 
   |       |	                                     |                                         |
   +-------+-----------------------------------------+-----------------------------------------+
   |  24   | .. image:: tmpimg/ltd-strip-24-yx.png   | .. image:: tmpimg/ltd-strip-24-3d.png   |
   |       |    :width: 90%                          |    :width: 90%                          | 
   |       |	                                     |                                         |
   +-------+-----------------------------------------+-----------------------------------------+
   |  25   | .. image:: tmpimg/ltd-strip-25-yx.png   | .. image:: tmpimg/ltd-strip-25-3d.png   |
   |       |    :width: 90%                          |    :width: 90%                          | 
   |       |	                                     |                                         |
   +-------+-----------------------------------------+-----------------------------------------+
   |  26   | .. image:: tmpimg/ltd-strip-26-yx.png   | .. image:: tmpimg/ltd-strip-26-3d.png   |
   |       |    :width: 90%                          |    :width: 90%                          | 
   |       |	                                     |                                         |
   +-------+-----------------------------------------+-----------------------------------------+
   |  27   | .. image:: tmpimg/ltd-strip-27-yx.png   | .. image:: tmpimg/ltd-strip-27-3d.png   |
   |       |    :width: 90%                          |    :width: 90%                          | 
   |       |	                                     |                                         |
   +-------+-----------------------------------------+-----------------------------------------+
   |  28   | .. image:: tmpimg/ltd-strip-28-yx.png   | .. image:: tmpimg/ltd-strip-28-3d.png   |
   |       |    :width: 90%                          |    :width: 90%                          | 
   |       |	                                     |                                         |
   +-------+-----------------------------------------+-----------------------------------------+
   |  31   | .. image:: tmpimg/ltd-strip-31-yx.png   | .. image:: tmpimg/ltd-strip-31-3d.png   |
   |       |    :width: 90%                          |    :width: 90%                          | 
   |       |	                                     |                                         |
   +-------+-----------------------------------------+-----------------------------------------+
   |  32   | .. image:: tmpimg/ltd-strip-32-yx.png   | .. image:: tmpimg/ltd-strip-32-3d.png   |
   |       |    :width: 90%                          |    :width: 90%                          | 
   |       |	                                     |                                         |
   +-------+-----------------------------------------+-----------------------------------------+
   |  33   | .. image:: tmpimg/ltd-strip-33-yx.png   | .. image:: tmpimg/ltd-strip-33-3d.png   |
   |       |    :width: 90%                          |    :width: 90%                          | 
   |       |	                                     |                                         |
   +-------+-----------------------------------------+-----------------------------------------+
   |  34   | .. image:: tmpimg/ltd-strip-34-yx.png   | .. image:: tmpimg/ltd-strip-34-3d.png   |
   |       |    :width: 90%                          |    :width: 90%                          | 
   |       |	                                     |                                         |
   +-------+-----------------------------------------+-----------------------------------------+
	      

 
Comments 
=====================================

* Strip #2  shows a significant  asymmetric deformation  at Z ~  51 cm
  (elliptic fit should  not work in this zone).  The  amplitude of the
  transverse  deformation  is  somewhat limited  (a  few  millimeters)
  compared to other ITEP-shaped strips.



.. end
   


