
Build FSF data
==============


From the build directory:


.. code:: shell

   $ export SNRS_DATA_DIR=$(pwd)/snrs_data
   $ ./snrs-build-fsf --verbosity "notice" \
	  --strip-id 34  \
	  --ltd-directory "${SNRS_DATA_DIR}/geometry/source_foils/ltd" \
	  --fsf-directory "${SNRS_DATA_DIR}/geometry/source_foils/fsf" \
	  --pad-ny=10 \
	  --pad-nz=100 \
	  --z-band-width=1.5 \
	  --y-edge-safe=3.0 \
	  --sigma-x=0.25 \
	  --fit-eps-abs=1.0 \
	  --plot \
	  --display


