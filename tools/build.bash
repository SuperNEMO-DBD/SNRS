#!/usr/bin/env bash

src_dir=$(pwd)
build_dir="$(pwd)/_build.d"
install_dir="$(pwd)/_install.d"
install_dir="/opt/sw/SuperNEMO-DBD/SNRS/install-devel"
snrs_with_tests=false
snrs_with_docs=false

snrs_with_tests=true

if [ -d ${build_dir} ]; then
    rm -fr ${build_dir}
fi

mkdir -p ${build_dir}
cd ${build_dir}

if [ -z ${BX_BAYEUX_INSTALL_DIR} ]; then
    bayeux_3_5_0_setup
fi

# Example:
#  $ export RAW_LTD_DATA_DIR=/data/SuperNEMO/RealSources/SNLTD_3D_measurements

rawLtdOpt=
if [ -n "${RAW_LTD_DATA_DIR}" ]; then
   rawLtdOpt=-DRAW_LTD_DATA_DIR="${RAW_LTD_DATA_DIR}" 
fi

snrs_devel_mode_opt="-DSNRS_DEVELOPER_BUILD=OFF -DSNRS_GENERATE_DATA=OFF"
snrs_doc_opt="-DSNRS_WITH_DOC=OFF"
snrs_tests_opt="-DSNRS_ENABLE_TESTING=OFF"

if [ ${snrs_with_tests} = true ]; then
    snrs_tests_opt="-DSNRS_ENABLE_TESTING=ON"
fi

if [ ${snrs_with_docs} = true ]; then
    snrs_doc_opt="-DSNRS_WITH_DOC=ON"
fi

cmake -DCMAKE_INSTALL_PREFIX=${install_dir} \
      -DBayeux_DIR=$(bxquery --cmakedir) \
      ${snrs_devel_mode_opt} \
      ${snrs_doc_opt} \
      ${snrs_tests_opt} \
      ${rawLtdOpt} \
    ${src_dir}
if [ $? -ne 0 ]; then
    echo >&2 "[error] CMake configuration failed!"
    exit 1
fi

# exit 0

make
if [ $? -ne 0 ]; then
    echo >&2 "[error] Make build failed!"
    exit 1
fi

make install
if [ $? -ne 0 ]; then
    echo >&2 "[error] Make install failed!"
    exit 1
fi

tree ${install_dir}/

exit 0
