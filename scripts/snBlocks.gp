# Blocks
array yBlockMin[7] 
array yBlockMax[7] 
yBlockMin[7]=2099 * mm
yBlockMax[7]=2497 * mm
yBlockMin[6]=1263 * mm
yBlockMax[6]=2081 * mm
yBlockMin[5]=427 * mm
yBlockMax[5]=1245 * mm
yBlockMin[4]=-409 * mm
yBlockMax[4]=409 * mm
yBlockMin[3]=-1245 * mm
yBlockMax[3]=-427 * mm
yBlockMin[2]=-2081 * mm
yBlockMax[2]=-1263 * mm
yBlockMin[1]=-2497 * mm
yBlockMax[1]=-2099 * mm

# x == stripId
fBlockId(x) = x < 3 ? 0 : x < 9 ? 1 : x < 15 ? 2 : x < 21 ? 3 : x < 27 ? 4 : x < 33 ? 5 : 6

# end
