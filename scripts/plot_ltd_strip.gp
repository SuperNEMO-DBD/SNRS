
# Usage: $ gnuplot -e "stripId=2" -e "ltdDir='${SNRS_DATA_DIR}/geometry/source_foils/ltd'" plot_ltd_strip.gp
# Usage: $ gnuplot -e "stripId=34" -e "ltdDir='${SNRS_DATA_DIR}/geometry/source_foils/ltd'" plot_ltd_strip.gp
# stripId=$1
print "Strip ID :", stripId
print "LTD dir  :", ltdDir
everyPoint=10

fBlockId(x) = x < 3 ? 0 : x < 9 ? 1 : x < 15 ? 2 : x < 21 ? 3 : x < 27 ? 4 : x < 33 ? 5 : 6
blockId = fBlockId(stripId)
print "Block ID :", blockId

mm=1.0
array ymin[7] 
array ymax[7] 
ymin[7]=2099 * mm
ymax[7]=2497 * mm
ymin[6]=1263 * mm
ymax[6]=2081 * mm
ymin[5]=427 * mm
ymax[5]=1245 * mm
ymin[4]=-409 * mm
ymax[4]=409 * mm
ymin[3]=-1245 * mm
ymax[3]=-427 * mm
ymin[2]=-2081 * mm
ymax[2]=-1263 * mm
ymin[1]=-2497 * mm
ymax[1]=-2099 * mm
# set xrange [-50*mm:50*mm]
# set yrange [ymin[blockId+1]:ymax[blockId+1]]
# set zrange [-1500 * mm:1500 * mm]
set grid

ltdStripFilename = sprintf("%s/images/ltd-strip-%d.plot", ltdDir, stripId);
print "Strip laser tracking data file : ", ltdStripFilename
zBandLtdStripFilename = sprintf("%s/images/ltd-strip-%d-zband-test.plot", ltdDir, stripId);
print "Strip laser tracking data file (z-band) : ", zBandLtdStripFilename

a = 55.0
b = 57.0
x0=42.0
y0=2303.5
#set dummy y
#f(y) = x0 - a * sqrt( 1.0 - (y - y0)**2 / b**2 )
#set key out

############################
set title sprintf("LTD strip #%d (YX)", stripId)
set size ratio -1
set xrange [*:*]
set yrange [-100:100]
set xlabel "y (mm)"
set ylabel "x (mm)"
plot ltdStripFilename every everyPoint u 2:1 title "LTD" with dots, \
     zBandLtdStripFilename u 2:1 title "Z=0 band" with points pt 6 ps 0.45
pause -1

set terminal push
set terminal pngcairo
set output sprintf("%s/images/ltd-strip-%d-yx.png", ltdDir, stripId)
replot
set output
set terminal pop

############################
# set view equal xyz
set title sprintf("LTD strip #%d (3D)", stripId)
set xyplane relative 0
set view 60,30
unset xrange
unset yrange 
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
