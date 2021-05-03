#!/usr/bin/env bash

src_dir=$(pwd)
build_dir="$(pwd)/_build.d"
install_dir="$(pwd)/_install.d"

if [ -d ${build_dir} ]; then
    rm -fr ${build_dir}
fi

mkdir -p ${build_dir}
cd ${build_dir}

if [ -z ${BX_BAYEUX_INSTALL_DIR} ]; then
    bayeux_3_4_5_setup
fi

rawLtdOpt=
if [ -n "${RAW_LTD_DATA_DIR}" ]; then
   rawLtdOpt=-DRAW_LTD_DATA_DIR="${RAW_LTD_DATA_DIR}" 
fi

cmake -DCMAKE_INSTALL_PREFIX=${install_dir} \
      -DBayeux_DIR=$(bxquery --cmakedir) \
      -DSNRS_DEVELOPER_BUILD=ON \
      ${rawLtdOpt} \
    ${src_dir}
if [ $? -ne 0 ]; then
    echo >&2 "[error] CMake configuration failed!"
    exit 1
fi

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
