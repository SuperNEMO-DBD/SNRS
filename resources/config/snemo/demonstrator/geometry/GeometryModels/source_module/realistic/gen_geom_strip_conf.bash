#!/usr/bin/env bash

strip_itep_ids="3 8 9 14 15 20 21 22 23 24 25 26 27 28 31 32 33 34"
strips_itep_geom_file="strips_itep_like.geom"

if [ -f ${strips_itep_geom_file} ]; then
    rm -f ${strips_itep_geom_file}
fi

touch ${strips_itep_geom_file}

cat > ${strips_itep_geom_file} <<EOF
# -*- mode: conf-unix; -*-
#@description A sample list of geometry ID categories/types
#@key_label   "name"
#@meta_label  "type"

EOF

for strip_id in ${strip_itep_ids} ; do
    echo >&2 "[info] ITEP strip ID = ${strip_id}"
    cat strips_itep_like.geom.template | \
	sed -e "s/@STRIPID@/${strip_id}/g" >> ${strips_itep_geom_file} 
done

cat ${strips_itep_geom_file}

exit 0

# end
