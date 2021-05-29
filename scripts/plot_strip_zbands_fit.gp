# Usage: $ gnuplot -e "stripId=33" fit.gp
stripId=33

set grid
set size ratio -1

# plot sprintf('strip-%d-pad-0-fsfs-zfits.plot', stripId) i 0 u 2:1 notitle with lines
# pause -1



sigma_x=0.5
xSkip=5.0

plot for [kz=0:100] sprintf('strip-%d-pad-0-fsfs-zbands.plot', stripId) index kz using ($2):(($1)+xSkip*kz):(sigma_x) notitle with points pt 6 ps 0.25, \
     for [kz=0:100] sprintf('strip-%d-pad-0-fsfs-zfits.plot', stripId)  every ::25::50 index kz using ($2):(($1)+xSkip*kz) notitle with lines
pause -1