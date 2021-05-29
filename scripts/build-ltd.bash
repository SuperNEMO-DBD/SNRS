#!/usr/bin/env bash

srcDir=
buildDir=
rawLtdDir=
snrsDataDir=
doPlot=true

function app_exit()
{
    local errorCode=$1
    shift 1
    local errorMsg="$@"
    if [ "x${errorMsg}" != "x" ]; then
	echo >&2 "[error] ${errorMsg}"
    fi
    exit ${errorCode}
}

while [ -n "$1" ]; do
    opt="$1"
    if [ "x${opt}" = "x--source-dir" ]; then
	shift 1
	srcDir="$1"
    elif [ "x${opt}" = "x--build-dir" ]; then
	shift 1
	buildDir="$1"
    elif [ "x${opt}" = "x--raw-ltd-dir" ]; then
	shift 1
	rawLtdDir="$1"
    elif [ "x${opt}" = "x--snrs-data-dir" ]; then
	shift 1
	snrsDataDir="$1"
    elif [ "x${opt}" = "x--plot" ]; then
	doPlot=true
    elif [ "x${opt}" = "x--no-plot" ]; then
	doPlot=false
    else
	app_exit 1 "Invalid option '${opt}'!"
    fi
    shift 1
done

snrsConfigExe=$(which snrs-config)
if [ -n "${snrsConfigExe}" ]; then
    snrsBuildLtdExe="$(which snrs-build-ltd)"
    snrsLtdPlotScript="$(snrs-config --scriptdir)/plot_ltd_strip.gp"
    snrsMacroDir="$(snrs-config --scriptdir)"
else
    if [ -z ${srcDir} ]; then
	app_exit 1 "Missing source directory '--source-dir path'!"
    fi
    if [ -z ${buildDir} ]; then
	app_exit 1 "Missing build directory '--build-dir path'!"
    fi
    snrsBuildLtdExe="${buildDir}/snrs-build-ltd"
    snrsLtdPlotScript="${srcDir}/scripts/plot_ltd_strip.gp"
    snrsMacroDir="${srcDir}/scripts"
fi
echo >&2 "[info] snrsBuildFsfExe='${snrsBuildFsfExe}'"
echo >&2 "[info] snrsLtdPlotScript='${snrsLtdPlotScript}'"
echo >&2 "[info] snrsMacroDir='${snrsMacroDir}'"

# snrsBuildLtdExe=$(which snrs-build-ltd)
# if [ -z '${snrsBuildLtdExe}' ]; then
#     snrsBuildLtdExe=${buildDir}/snrs-build-ltd
# fi
# snrsLtdPlotScript=$(snrs-config --scriptdir)/plot_ltd_strip.gp
# if [ -z '${snrsLtdPlotScript}' ]; then
#     snrsLtdPlotScript=${srcDir}/scripts/plot_ltd_strip.gp
# fi
# rawLtdDir="/data/SuperNEMO/RealSources/3D_measurements/source_foils"
# snrsDataDir="/data/SuperNEMO/snrs_data"
echo >&2 "[info] snrsBuildLtdExe='${snrsBuildLtdExe}'"
echo >&2 "[info] snrsLtdPlotScript='${snrsLtdPlotScript}'"

if [ "x${rawLtdDir}" = "x" ]; then
    if [ "x${RAW_LTD_DATA_DIR}" = "x" ]; then
	app_exit 1 "RAW_LTD_DATA_DIR environment variable is not set!"
    fi
    echo >&2 "[info] using RAW_LTD_DATA_DIR='${RAW_LTD_DATA_DIR}'"
    rawLtdDir="${RAW_LTD_DATA_DIR}"
fi
echo >&2 "[info] Raw LTD directory   : '${rawLtdDir}'"

if [ ! -d ${rawLtdDir} ]; then
    app_exit 1 "Raw LTD directory '${rawLtdDir}' does not exist!"
fi

if [ "x${snrsDataDir}" = "x" ]; then
    if [ "x${SNRS_DATA_DIR}" = "x" ]; then
	app_exit 1 "SNRS_DATA_DIR environment variable is not set!"
    fi
    echo >&2 "[info] using SNRS_DATA_DIR='${SNRS_DATA_DIR}'"
    snrsDataDir="${SNRS_DATA_DIR}"
fi
echo >&2 "[info] SNRS data directory : '${snrsDataDir}'"

if [ ! -d ${snrsDataDir} ]; then
    echo >&2 "[info] Creating SNRS data directory '${snrsDataDir}'..."
    mkdir -p ${snrsDataDir}
    if [ $? -ne 0 ]; then
	app_exit 1 "Failed to create SNRS data directory '${snrsDataDir}'!"
    fi
fi

snrsLtdDir="${snrsDataDir}/geometry/source_foils/ltd"

itepLists="2 3 8 9 14 15 20 21 22 23 24 25 26 27 28 31 32 33 34" 
# itepLists="2 3 8 9 14"
# itepLists="15 20 21 22 23"
# itepLists="24 25 26 27 28"
# itepLists="31 32 33 34"
# itepLists="33 34"
echo >&2 "[info] list of ITEP source strips : '${itepLists}'"

mkdir -p "${snrsLtdDir}"
if [ $? -ne 0 ]; then
    app_exit 1 "Failed to create SNRS LTD data directory '${snrsLtdDir}'!"
fi
if [ ${doPlot} = true ]; then
    mkdir -p "${snrsLtdDir}/images"
    if [ $? -ne 0 ]; then
	app_exit 1 "Failed to create SNRS LTD images directory '${snrsLtdDir}/images'!"
    fi
fi

plotOpt=
if [ ${doPlot} = true ]; then
    plotOpt="--plot"
fi
# if [ -f "${snrsLtdDir}/processed-strips.lis" ]; then
#     rm -f "${snrsLtdDir}/processed-strips.lis"
# fi
touch "${snrsLtdDir}/processed-strips.lis"
for stripId in ${itepLists} ; do
    processStrip=true
    grep ^${stripId} ${snrsLtdDir}/processed-strips.lis
    if [ $? -eq 0 ]; then
	echo >&2 "[info] LTD data for ITEP-like strip ${stripId} already exist."
	processStrip=false
    fi
    if [ ${processStrip} = true ]; then
	echo >&2 "[info] Processing raw LTD data for ITEP-like strip ${stripId}..."
	${snrsBuildLtdExe} \
	    --verbosity "notice" \
	    --strip-id ${stripId} \
	    --raw-ltd-directory ${rawLtdDir} \
	    --output-ltd-directory ${snrsLtdDir}	\
	    ${plotOpt} \
	    --test-reload
	processingStatus=$?
	if [ $processingStatus -ne 0 ]; then
	    app_exit 1 "Failed to run `basename ${snrsBuildLtdExe}` for strip ${stripId}!"
	fi
	echo "${stripId}" >> "${snrsLtdDir}/processed-strips.lis"
    fi
    if [ ${doPlot} = true ]; then
	echo >&2 "[info] Generating LTD plots for strip ${stripId}..."
	echo -e "\n\n" | gnuplot -e "stripId=${stripId}" \
				 -e "macroDir='${snrsMacroDir}'" \
				 -e "ltdDir='${snrsLtdDir}'" ${snrsLtdPlotScript}
    else
	echo >&2 "[info] Do not generate LTD plots for strip ${stripId}."
    fi
done
tree ${snrsDataDir}/geometry/source_foils/ltd/

app_exit 0

# end
