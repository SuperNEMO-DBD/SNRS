#!/usr/bin/env bash

export SNRS_TESTING_DIR=$(pwd)/../snrs/test
export SNRS_RESOURCE_PATH=$(pwd)/../resources
export SNRS_BUILD_DIR=$(pwd)

batch=false
echo >&2 "[notice] SNRS_TESTING_DIR=${SNRS_TESTING_DIR}"
echo >&2 "[notice] SNRS_BUILD_DIR=${SNRS_BUILD_DIR}"
echo >&2 "[notice] SNRS_RESOURCE_PATH=${SNRS_BUILD_DIR}"

batchOpt="--interactive"
if [ ${batch} = true ]; then
    batchOpt="--batch"
fi

cat <<EOF



EOF

# export SNRS_TESTING_DIR=

bxgeomtools_inspector \
    --logging "debug" \
    ${batchOpt} \
    --manager-config "${SNRS_TESTING_DIR}/config/geometry_manager.conf" \
    --load-dll "snrs@${SNRS_BUILD_DIR}"
    

exit 0

# end

