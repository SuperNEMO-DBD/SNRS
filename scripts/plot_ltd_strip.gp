# Usage: $ gnuplot -e "stripId=2" -e "ltdDir='${SNRS_DATA_DIR}/geometry/source_foils/ltd'" plot_ltd_strip.gp
# Usage: $ gnuplot -e "stripId=34" -e "ltdDir='${SNRS_DATA_DIR}/geometry/source_foils/ltd'" plot_ltd_strip.gp -e "gpDir='/path/to/gpDir'"

# export SNRS_DATA_DIR=$(pwd)/../../_build.d/snrs_data/geometry/source_foils/ltd

# stripId=$1
print "Strip ID  : ", stripId
print "LTD dir   : ", ltdDir
print "Macro dir : ", macroDir

load sprintf('%s/common.gp', macroDir)
load sprintf('%s/snBlocks.gp', macroDir)
load sprintf('%s/snStrips.gp', macroDir)


blockId = fBlockId(stripId)
print "Block ID :", blockId
ycenter=yStripCenter[stripId+1]
print "Strip ycenter :", ycenter
effWidth=stripWidth # -1*mm
ylim1=ycenter-0.5*effWidth
ylim2=ycenter+0.5*effWidth

ltdStripFilename = sprintf("%s/images/ltd-strip-%d.plot", ltdDir, stripId);
print "Strip laser tracking data file : ", ltdStripFilename
zBandLtdStripFilename = sprintf("%s/images/ltd-strip-%d-zband-test.plot", ltdDir, stripId);
print "Strip laser tracking data file (z-band) : ", zBandLtdStripFilename

set grid
everyPoint=10

############################
set title sprintf("LTD strip #%d (YX)", stripId)
set size ratio -1
set xrange [*:*]
dxSource=58*mm
xmin= (stripId==2) ? -20 : -0.5*dxSource
xmax= (stripId==2) ? 20 : +0.5*dxSource
ymin=ycenter-8*cm
ymax=ycenter+8*cm
set xrange [ymin:ymax]
set yrange [xmin:xmax]
set xlabel "y (mm)"
set ylabel "x (mm)"
set arrow 1 from ylim1,xmin to ylim1,xmax nohead linecolor rgb "red"
set arrow 2 from ylim2,xmin to ylim2,xmax nohead linecolor rgb "red"
set arrow 3 from ycenter,xmin to ycenter,xmax nohead linecolor rgb "red"
plot ltdStripFilename every everyPoint u 2:1 title "LTD" with dots, \
     zBandLtdStripFilename u 2:1 title "Z=0 band" with points pt 6 ps 0.45
pause -1

set terminal push
set terminal pngcairo
set output sprintf("%s/images/ltd-strip-%d-yx.png", ltdDir, stripId)
replot
set output
set terminal pop

unset arrow


############################
# set view equal xyz
set title sprintf("LTD strip #%d (3D)", stripId)
set xyplane relative 0
set view 60,30
unset xrange
unset yrange
zmin=-1350
zmax=+1350
set xrange [xmin:xmax]
set yrange [ymin:ymax]
set zrange [zmin:zmax]
set xlabel "x (mm)"
set ylabel "y (mm)"
set zlabel "z (mm)"
splot ltdStripFilename every everyPoint u 1:2:3 title "LTD"  with dots, \
      zBandLtdStripFilename u 1:2:3 title "Z=0 band" with points pt 6 ps 0.35
pause -1

set terminal push
set terminal pngcairo
set output sprintf("%s/images/ltd-strip-%d-3d.png", ltdDir, stripId)
replot
set output
set terminal pop

# end
