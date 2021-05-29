# Usage: $ gnuplot -e "stripId=34" -e "fsfDir='${SNRS_DATA_DIR}/geometry/source_foils/fsf'" -e "macroDir='/path/to/macroDir'" plot_fsf_strip.gp
# stripId=$1
# fsfDir=$2
print "Strip ID  : ", stripId
print "FSF dir   : ", fsfDir
print "Macro dir : ", macroDir

genImages=1
# genImages=0

load sprintf('%s/common.gp', macroDir)
load sprintf('%s/snBlocks.gp', macroDir)
load sprintf('%s/snStrips.gp', macroDir)

blockId = fBlockId(stripId)
print "Block ID : ", blockId
ycenter=yStripCenter[stripId+1]
print "Strip ycenter : ", ycenter
fsfShapingReportStripFilename = sprintf("%s/strip-%d-pad-0-shaping.report", fsfDir, stripId);
print "FSFS strip shaping report : ", fsfShapingReportStripFilename

concave=0
firstX0=`grep '^0 ' @fsfShapingReportStripFilename  | cut -d' ' -f11 `
concave=(firstX0<0.0) ? 1 : 0
print "Concave : ", concave


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


set key out
set grid
everyPoint=10

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
     fitReportFilename using 1:(column(7)!=1?0.0/0.0:column(12) - yRef) title sprintf("y0 - (%d mm)",yRef) with points pt 6 ps 0.5
pause -1

if (genImages > 0) {
    set terminal push
    set terminal pngcairo
    set output sprintf("%s/images/strip-%d-pad-0-fit-report.png", fsfDir, stripId);
    replot
    set output
    set terminal pop
}

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


if (genImages > 0) {
    set terminal push
    set terminal pngcairo
    set output sprintf("%s/images/strip-%d-pad-0-fit-zbands.png", fsfDir, stripId);
    replot
    set output
    set terminal pop
}


###################################
set title sprintf("Strip #%d - Fit vs Z-band (%d z-levels)", stripId, padNz+1); 
set xlabel "y (mm)" offset 0,0
set ylabel "x (mm)" offset 0,0
xSkip=5.0
set xrange [ycenter-85:ycenter+85]
set yrange [-45:550]
set size ratio -1

everyMin=225
everyMax=315
if (concave) {
    everyMin=45
    everyMax=135
}

plot for [kz=0:100] fsfZbandsStripFilename index kz using ($2):(($1)+xSkip*kz) notitle with points pt 6 ps 0.25, \
     for [kz=0:100] fsfZfitsStripFilename  every ::everyMin::everyMax index kz using ($2):(($1)+xSkip*kz) notitle with lines
pause -1

if (genImages > 0) {
    set terminal push
    set terminal pngcairo
    set output sprintf("%s/images/strip-%d-pad-0-fit-vs-zbands.png", fsfDir, stripId);
    replot
    set output
    set terminal pop
}

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

if (genImages > 0) {
    set terminal push
    set terminal pngcairo
    set output sprintf("%s/images/strip-%d-pad-0-mesh.png", fsfDir, stripId);
    replot
    set output
    set terminal pop
}

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

if (genImages > 0) {
    set terminal push
    set terminal pngcairo
    set output sprintf("%s/images/strip-%d-pad-0-mesh-zoom-top.png", fsfDir, stripId);
    replot
    set output
    set terminal pop
}
###################################

set size noratio
effWidth=stripWidth # -1*mm
ylim1=-0.5*effWidth
ylim2=+0.5*effWidth

set yrange [*:*]
set xrange [-80:+80]
set xlabel "y (mm)"  offset 0,0
set ylabel "z (mm)"  offset 0,0

set title sprintf("Strip #%d - Fitted 3D-mesh (%d x %d tiles, YZ-view)", stripId, padNy, padNz); 
set xlabel "y"
set ylabel "z"
set arrow from ylim1,-1500 to ylim1,+1500 nohead linecolor rgb "red"
set arrow from ylim2,-1500 to ylim2,+1500 nohead linecolor rgb "red"
plot ltdStripFilename u (($2)-ycenter):3 notitle w dots linecolor rgb "cyan", \
     shapedPadStripFilename u 5:6 notitle w lines , \
     fsfZbandsStripFilename u (($2)-ycenter):3 notitle w points pt 6 ps 0.35 linecolor rgb "blue"
pause -1

if (genImages > 0) {
    set terminal push
    set terminal pngcairo
    set output sprintf("%s/images/strip-%d-pad-0-mesh-yz.png", fsfDir, stripId);
    replot
    set output
    set terminal pop
}
unset arrow


# end
