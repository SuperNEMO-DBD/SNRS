#!/usr/bin/env bash

echo >&2 "[notice] PWD=$(pwd)"
export SNRS_TESTING_DIR=$(pwd)/../../../snrs/test
export SNRS_RESOURCE_PATH=$(pwd)/../../../resources
export SNRS_BUILD_DIR=$(pwd)/../..

batch=false
batch=true
echo >&2 "[notice] SNRS_TESTING_DIR=${SNRS_TESTING_DIR}"
echo >&2 "[notice] SNRS_BUILD_DIR=${SNRS_BUILD_DIR}"
echo >&2 "[notice] SNRS_RESOURCE_PATH=${SNRS_BUILD_DIR}"

batchOpt="--interactive"
if [ ${batch} = true ]; then
    batchOpt="--batch"
fi

cat > _script.bat <<EOF
q
EOF

# export SNRS_TESTING_DIR=

bxgeomtools_inspector \
    --logging "debug" \
    ${batchOpt} \
    --manager-config "${SNRS_TESTING_DIR}/config/geometry_manager.conf" \
    --load-dll "snrs@${SNRS_BUILD_DIR}" \
    _script.bat


rm -f _script.bat
    

exit 0

# end

