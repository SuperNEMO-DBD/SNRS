====================================
Test geometry setup
====================================


:author: F.Mauger
:date: 2021-05-04

From the source directory:

.. code:: bash

   $ bayeux_3_5_0_setup	  
   $ export SNRS_RESOURCE_PATH="./resources"
   $ export SNRS_TESTING_DIR="./snrs/test"
   $ export SNRS_BUILD_DIR="./_build.d"
   $ bxgeomtools_inspector \
	  --logging "debug" \
	  ${batchOpt} \
	  --manager-config "${SNRS_TESTING_DIR}/config/geometry_manager.conf" \
	  --load-dll "snrs@${SNRS_BUILD_DIR}"
..
