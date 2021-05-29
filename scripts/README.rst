====================================
Scripts for developers
====================================



Build LTD plots per strip from the build directory ``../_build.d`` :

.. code:: shell

   $ export SNRS_DATA_DIR=$(pwd)/snrs_data
   $ gnuplot -e "stripId=3" -e "ltdDir='${SNRS_DATA_DIR}/geometry/source_foils/ltd'" -e "gpDir='$(pwd)/../scripts'" $(pwd)/../scripts/plot_ltd_strip.gp
..


Build FSF plots per strip from the build directory ``../_build.d`` :

.. code:: shell

   $ export SNRS_DATA_DIR=$(pwd)/snrs_data
   $ gnuplot -e "stripId=3" -e "fsfDir='${SNRS_DATA_DIR}/geometry/source_foils/fsf'" -e "gpDir='$(pwd)/../scripts'" $(pwd)/../scripts/plot_fsf_strip.gp
..
