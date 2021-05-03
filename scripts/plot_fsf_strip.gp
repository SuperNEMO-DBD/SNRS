
# Usage: $ gnuplot -e "stripId=34" -e "fsfDir='${SNRS_DATA_DIR}/geometry/source_foils/fsf'" plot_fsf_strip.gp
# stripId=$1
# fsfDir=$2
print "Strip ID :", stripId
print "FSF dir  :", fsfDir
everyPoint=10

set key out
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

ltdStripFilename = sprintf("%s/../ltd/images/ltd-strip-%d.plot", fsfDir, stripId);
print "Strip laser tracking data file : ", ltdStripFilename
fsfZbandsStripFilename = sprintf("%s/images/strip-%d-pad-0-fsfs-zbands.plot", fsfDir, stripId);
print "FSFS strip zbands data file : ", fsfZbandsStripFilename
fsfZfitsStripFilename = sprintf("%s/images/strip-%d-pad-0-fsfs-zfits.plot", fsfDir, stripId);
print "FSFS strip zfits data file : ", fsfZfitsStripFilename
padStripFilename = sprintf("%s/images/strip-%d-pad-0.plot", fsfDir, stripId);
print "Pad strip plot file : ", padStripFilename
shapedPadStripFilename = sprintf("%s/images/strip-%d-pad-0-shaped.plot", fsfDir, stripId);
print "Shaped pad strip plot file : ", shapedPadStripFilename
fitReportFilename = sprintf("%s/strip-%d-pad-0-shaping.report", fsfDir, stripId);
print "Fit report file : ", fitReportFilename

set grid

###################################
set xrange [:]
set yrange [:]
set xlabel "Z-band index"
set ylabel "Fit parameters (mm)"

padNy=`cat @fitReportFilename | grep "#@pad-ny=" | cut -d'=' -f2`
padNz=`cat @fitReportFilename | grep "#@pad-nz=" | cut -d'=' -f2`
yRef=`cat @fitReportFilename | grep "#@strip.pos.y=" | cut -d'=' -f2`
print "padNy = ", padNy
print "padNz = ", padNz
print "y-ref = ", yRef, " mm"

set title sprintf("Elliptic fit parameters for strip %d", stripId)
plot \
     fitReportFilename using 1:(column(7)!=1?0.0/0.0:column(9)) title "a" with points pt 6 ps 0.5, \
     fitReportFilename using 1:(column(7)!=1?0.0/0.0:column(10)) title "b" with points pt 6 ps 0.5, \
     fitReportFilename using 1:(column(7)!=1?0.0/0.0:column(11)) title "x0" with points pt 6 ps 0.5, \
     fitReportFilename using 1:(column(7)!=1?0.0/0.0:column(12) - yRef) title sprintf("y0 - %d mm",yRef) with points pt 6 ps 0.5
pause -1

set terminal push
set terminal pngcairo
set output sprintf("%s/images/strip-%d-pad-0-fit-report.png", fsfDir, stripId);
replot
set output
set terminal pop

###################################
set title sprintf("Strip #%d - Shaping Fit (%d z-levels)", stripId, padNz+1); 
set key inside left
set view equal xyz
set xyplane at -1400
set size ratio -1
set xlabel "x (mm)" offset -2,1
set ylabel "y (mm)" offset +12,+1
set zlabel "z (mm)" offset 0,+8
set view 12,69,0.95,1
set xrange [-50:+50]
set yrange [-200:+200]
set zrange [-1400:+1400]
splot \
     fsfZbandsStripFilename every 1 using 1:(($2)-yRef):3 title "LTD Z-bands" with dots, \
     fsfZfitsStripFilename u 1:(($2)-yRef):3 title "Z-fits" with lines
pause -1
# pause mouse close

set terminal push
set terminal pngcairo
set output sprintf("%s/images/strip-%d-pad-0-fit-zbands.png", fsfDir, stripId);
replot
set output
set terminal pop

###################################
set key inside
set title sprintf("Strip #%d - Fitted 3D-mesh (%d x %d tiles)", stripId, padNy, padNz); 
set view equal xyz
set xyplane at -1400
set size ratio -1
set xlabel "x (mm)" offset -2,1
set ylabel "y (mm)" offset +12,+1
set zlabel "z (mm)" offset 0,+8
set view 12,69,0.95,1
set xrange [-100:+100]
set yrange [-200:+200]
set zrange [-1400:+1400]
# set xzeroaxis lt -1 lw 1
splot \
      shapedPadStripFilename  u 4:5:6 title "Mesh"  with lines, \
      fsfZbandsStripFilename every 2 using 1:(($2)-yRef):3 title "LTD Z-bands" with dots
pause -1
# pause mouse close

set terminal push
set terminal pngcairo
set output sprintf("%s/images/strip-%d-pad-0-mesh.png", fsfDir, stripId);
replot
set output
set terminal pop

###################################
set key inside
set title sprintf("Strip #%d - Fitted 3D-mesh (%d x %d tiles, top zoom)", stripId, padNy, padNz); 
set view equal xyz
set xyplane at 690
set size ratio -1
set xlabel "x (mm)"
set ylabel "y (mm)"
set zlabel "z (mm)"
unset view
set view 65,65,1,1
set xrange [-40:+40]
set yrange [-80:+80]
set zrange [+690:+1400]
splot \
      shapedPadStripFilename  u 4:5:6 title "Mesh"  with lines, \
      fsfZbandsStripFilename every 1 using 1:(($2)-yRef):3 title "LTD Z-bands" with points pt 6 ps 0.15
pause -1
# pause mouse close

set terminal push
set terminal pngcairo
set output sprintf("%s/images/strip-%d-pad-0-mesh-zoom-top.png", fsfDir, stripId);
replot
set output
set terminal pop

# end
