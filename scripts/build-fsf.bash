#!/usr/bin/env bash

interactive=true
interactive=false
srcDir=
buildDir=
snrsDataDir=
snrsResourceDir=
doPlot=false
padNy=10
padNz=100

# zBandWidth=1.5 # mm
# yEdgeSafe=3.0 # mm
# sigmaX=0.5 # mm
# fitEpsAbs=1.0

zBandWidth=1.5 # mm
yEdgeSafe=3.0 # mm
sigmaX=0.25 # mm
fitEpsAbs=1.0

# zBandWidth=2.5 # mm
# yEdgeSafe=5.0 # mm
# sigmaX=0.5 # mm
# fitEpsAbs=1.0

# yEdgeSafe=7.5 # mm
# sigmaX=1.0 # mm
# fitEpsAbs=1.15

shapingMode="3"

# itepLists="3 8 9 14 15 20 21 22 23 24 25 26 27 28 31 32 33 34" 
# itepLists="2 3 8 9 14"
# itepLists="15 20 21 22 23"
# itepLists="24 25 26 27 28"
# itepLists="31 32 33 34"
# itepLists="3" # Fit ok but bad chi2+params
# itepLists="8" # Fit bad but bad chi2+params
# itepLists="9" # Fit ok
# itepLists="14" # Fit ok
# itepLists="15" # Fit ok but one Zband
# itepLists="32 33 34" 
# itepLists="3 8 9 14 15 20 21 22 23 24 25 26 27 28 31" 
# itepLists="3" # Fit ok but bad chi2+params
# itepLists="8" # Fit ok but bad chi2+params
# itepLists="3" # Fit ok but bad chi2+params
# itepLists="32 33" 
# itepLists="14"
# itepLists="15"
# itepLists="3" 
# itepLists="33" 
# itepLists="32" 			
# itepLists="32 33 34" 
itepLists="3 8 9 14 15 20 21 22 23 24 25 26 27 28 31 32 33 34" 
# itepLists="28 31 32 33" 
echo >&2 "[info] list of ITEP source strips : '${itepLists}'"

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
    elif [ "x${opt}" = "x--snrs-data-dir" ]; then
	shift 1
	snrsDataDir="$1"
    elif [ "x${opt}" = "x--snrs-resource-dir" ]; then
	shift 1
	snrsResourceDir="$1"
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
    snrsBuildFsfExe="$(which snrs-build-fsf)"
    snrsFsfPlotScript="$(snrs-config --scriptdir)/plot_fsf_strip.gp"
    snrsMacroDir="$(snrs-config --scriptdir)"
else
    if [ -z ${srcDir} ]; then
	app_exit 1 "Missing source directory '--source-dir path'!"
    fi
    if [ -z ${buildDir} ]; then
	app_exit 1 "Missing build directory '--build-dir path'!"
    fi
    snrsBuildFsfExe="${buildDir}/snrs-build-fsf"
    snrsFsfPlotScript="${srcDir}/scripts/plot_fsf_strip.gp"
    snrsMacroDir="${srcDir}/scripts"
fi
echo >&2 "[info] snrsBuildFsfExe='${snrsBuildFsfExe}'"
echo >&2 "[info] snrsFsfPlotScript='${snrsFsfPlotScript}'"
echo >&2 "[info] snrsMacroDir='${snrsMacroDir}'"
echo >&2 "[info] snrsResourceDir='${snrsResourceDir}'"

if [ "x${snrsResourceDir}" != "x" ]; then
    export SNRS_RESOURCE_PATH="${snrsResourceDir}"
fi
# snrsBuildFsfExe=$(which snrs-build-fsf)
# if [ -z '${snrsBuildFsfExe}' ]; then
#     snrsBuildFsfExe=${buildDir}/snrs-build-fsf
# fi

if [ "x${snrsDataDir}" = "x" ]; then
    if [ "x${SNRS_DATA_DIR}" = "x" ]; then
	app_exit 1 "SNRS_DATA_DIR environment variable is not set!"
    fi
    snrsDataDir="${SNRS_DATA_DIR}"
fi

if [ ! -d ${snrsDataDir} ]; then
    app_exit 1 "SNRS data directory '${snrsDataDir}' does not exists!"
fi
echo >&2 "[info] snrsDataDir='${snrsDataDir}'"

snrsLtdDir=
snrsFsfDir=

snrsLtdDir="${snrsDataDir}/geometry/source_foils/ltd"
if [ ! -d ${snrsLtdDir} ]; then
    app_exit 1 "SNRS LTD data directory '${snrsLtdDir}' does not exists!"
fi

snrsFsfDir="${snrsDataDir}/geometry/source_foils/fsf"
if [ ! -d ${snrsFsfDir} ]; then
    echo >&2 "[info] Creating data directory '${snrsFsfDir}'..."
    mkdir -p ${snrsFsfDir}
    if [ $? -ne 0 ]; then
	app_exit 1 "Failed to create SNRS FSF data directory '${snrsFsfDir}'!"
    fi
fi

echo >&2 "[info] LTD directory : '${snrsLtdDir}'"
echo >&2 "[info] FSF directory : '${snrsFsfDir}'"

mkdir -p "${snrsFsfDir}"
if [ $? -ne 0 ]; then
    app_exit 1 "Failed to create SNRS FSF data directory '${snrsFsfDir}'!"
fi
if [ ${doPlot} = true ]; then
    mkdir -p "${snrsFsfDir}/images"
    if [ $? -ne 0 ]; then
	app_exit 1 "Failed to create SNRS FSF data directory '${snrsFsfDir}/images'!"
    fi
fi

plotOpt=
if [ ${doPlot} = true ]; then
    plotOpt="--plot"
fi

# if [ -f "${snrsFsfDir}/processed-strips.lis" ]; then
#     rm -f "${snrsFsfDir}/processed-strips.lis"
# fi
touch "${snrsFsfDir}/processed-strips.lis"
for stripId in ${itepLists} ; do
    processStrip=true
    grep ^${stripId}$ ${snrsFsfDir}/processed-strips.lis
    if [ $? -eq 0 ]; then
	echo >&2 "[info] FSF data for ITEP-like strip ${stripId} already exist."
	processStrip=false
    fi

    if [ ${processStrip} = true ]; then
	echo >&2 "[info] Generating FSF data from LTD data for ITEP-like strip ${stripId}..."
	padId=0
	effSigmaX=${sigmaX}
	effYEdgeSafe=${yEdgeSafe}
	effzBandWidth=${zBandWidth}
	if [ ${stripId} -eq 3 ]; then
    	    effSigmaX=1.0
	fi
	# if [ ${stripId} -eq 8 ]; then
	# 	effSigmaX=0.5
	# fi
	# if [ ${stripId} -eq 14 ]; then
	# 	# effYEdgeSafe=2.0
	# 	effzBandWidth=2.5
	# fi
	# if [ ${stripId} -eq 14 ]; then
	# 	effYEdgeSafe=2.0
	# fi
	# if [ ${stripId} -eq 15 ]; then
	# 	effYEdgeSafe=0.0
	# fi
	# if [ ${stripId} -eq 28 ]; then
	# 	fitEpsAbs=1.15
	# fi
	${snrsBuildFsfExe} \
    	    --verbosity "notice" \
    	    --strip-id ${stripId} \
    	    --pad-id ${padId} \
    	    --ltd-directory ${snrsLtdDir} \
    	    --fsf-directory ${snrsFsfDir} \
    	    --pad-ny ${padNy} \
   	    --pad-nz ${padNz} \
	    --z-band-width=${effzBandWidth} \
	    --y-edge-safe=${effYEdgeSafe} \
	    --sigma-x=${effSigmaX} \
	    --fit-eps-abs=${fitEpsAbs} \
	    --shaping-mode="${shapingMode}" \
    	    ${plotOpt}
	processingStatus=$?
	if [ $processingStatus -ne 0 ]; then
	    app_exit 1 "Failed to run `basename ${snrsBuildFsfExe}` for strip ${stripId}!"
	fi
	echo "${stripId}" >> "${snrsFsfDir}/processed-strips.lis"
    fi
    plotStrip=${processStrip}
    if [ ${plotStrip} = true ]; then
	if [ ${doPlot} = true ]; then
	    echo >&2 "[info] Generating FSF plots for strip ${stripId}..."
	    if [ ${interactive} = false ]; then
		echo -e "\n\n" | gnuplot -e "stripId=${stripId}" \
					 -e "macroDir='${snrsMacroDir}'" \
					 -e "fsfDir='${snrsFsfDir}'" ${snrsFsfPlotScript}
	    else
		gnuplot -e "stripId=${stripId}" \
			-e "macroDir='${snrsMacroDir}'" \
			-e "fsfDir='${snrsFsfDir}'" ${snrsFsfPlotScript}
	    fi
	fi
    fi
done
tree ${snrsDataDir}/geometry/source_foils/fsf/

app_exit 0

# end
