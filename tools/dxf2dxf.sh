#!/bin/bash

# Import a DXF file into OpenSCAD and save it to a new
# DXF file. This will output a DXF file that only contains
# LINE entity, simplier to parse further.

if [ $# != 2 ]; then
    echo "Usage: dxf2dxf.sh in.dxf out.dxf"
fi

echo "\$fn=50; import(\"$1\");" > tmp.scad
openscad tmp.scad -o $2
rm tmp.scad
