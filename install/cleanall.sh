#!/bin/bash

BUILDDIR=`pwd`
FDOBUILD=/root/fdo
FDOTARGET=/usr/fdo-3.6

pushd $FDOBUILD
source ./setenvironment.sh
for prov in fdo utilities fdocore sdf shp wfs wms rdbms gdal ogr postgis kingoracle sqlite;
do
  ./build_linux.sh --w $prov --a clean --p $FDOTARGET
done
popd
