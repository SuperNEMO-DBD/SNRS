==========================================================
Test SNRS geometry setup and vertex generation models
==========================================================


:author: F.Mauger
:date: 2021-05-04


Geometry
========
       
From the source directory:

.. code:: bash

   $ bayeux_3_5_0_setup	  
   $ export SNRS_RESOURCE_PATH="./resources"
   $ export SNRS_TESTING_DIR="./snrs/test"
   $ export SNRS_BUILD_DIR="./_build.d"
   $ bxgeomtools_inspector \
	  --logging "debug" \
	  --manager-config "${SNRS_TESTING_DIR}/config/geometry_manager.conf" \
	  --load-dll "snrs@${SNRS_BUILD_DIR}"
   geomtools> display -yz --output "snrs_geometry_yz.jpeg"
..



Vertex generation
=================

       
From the source directory:

List the available vertex generators
------------------------------------

.. code:: bash
	  
   $ bxgenvtx_production \
	  --logging "fatal" \
	  --load-dll "snrs@${SNRS_BUILD_DIR}" \
	  --geometry-manager "${SNRS_TESTING_DIR}/config/geometry_manager.conf" \
	  --vertex-generator-manager "${SNRS_TESTING_DIR}/config/vg_mgr.conf" \
	  --list
   List of vertex generators : 
   |-- strip_32_pad_0_surface.strange_hot_spot.vg  : (no description)
   |-- strip_32_pad_0_surface.strange_hot_spot_2.vg  : (no description)
   |-- strip_32_pad_0_surface.vg  : (no description)
   |-- strip_33_pad_0_surface.batman_spot.vg  : (no description)
   |-- strip_33_pad_0_surface.strange_hot_spot.vg  : (no description)
   |-- strip_33_pad_0_surface.vg  : (no description)
   |-- strip_34_pad_0_film_bulk.vg  : (no description)
   |-- strip_34_pad_0_surface.strange_hot_spot.vg  : (no description)
   |-- strip_34_pad_0_surface.strange_hot_spot_2.vg  : (no description)
   `-- strip_34_pad_0_surface.vg  : (no description)
..
   
.. image: images/snrs_geometry_yz.jpeg
   :width: 75%

..   

Generate vertexes on the surface of strip 32
--------------------------------------------
 
.. code:: bash

   $ bxgenvtx_production \
	  --logging "debug" \
	  --load-dll "snrs@${SNRS_BUILD_DIR}" \
          --geometry-manager "${SNRS_TESTING_DIR}/config/geometry_manager.conf" \
 	  --vertex-generator-manager "${SNRS_TESTING_DIR}/config/vg_mgr.conf" \
	  --shoot \
	  --prng-seed 314159 \
          --number-of-vertices 1000 \
	  --vertex-generator "strip_32_pad_0_surface.vg" \
	  --output-file "vertices.data" \
	  --visu \
	  --visu-view "yz" \
	  --visu-spot-size "5 mm" \
	  --visu-spot-color "blue" \
	  --visu-max-counts 1000                      
..
   
.. image: images/snrs_vertes_strip_32_surface_yz.jpeg
   :width: 75%

..   

Generate vertexes using a *batman* pattern
--------------------------------------------

.. code:: bash

   $ bxgenvtx_production \
	  --logging "debug" \
	  --load-dll "snrs@${SNRS_BUILD_DIR}" \
          --geometry-manager "${SNRS_TESTING_DIR}/config/geometry_manager.conf" \
 	  --vertex-generator-manager "${SNRS_TESTING_DIR}/config/vg_mgr.conf" \
	  --shoot \
	  --prng-seed 314159 \
          --number-of-vertices 1000 \
	  --vertex-generator "strip_33_pad_0_surface.batman_spot.vg" \
	  --output-file "vertices.data" \
	  --visu \
	  --visu-view "yz" \
	  --visu-spot-size "2 mm" \
	  --visu-spot-color "black" \
	  --visu-max-counts 1000                      
..
   
.. image: images/snrs_vertes_strip_batman_yz.jpeg
   :width: 75%

..
