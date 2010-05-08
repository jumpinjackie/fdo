#!/bin/bash
# Build Fdo core libraries on Linux and create install info
#
BUILDDIR=`pwd`
BUILDLIST=${BUILDDIR}/filelist
FDOBUILD=${BUILDDIR}/..
FDOTARGET=/usr/local/fdo-3.6.0

# Clean original directory tree
#
rm -rf $FDOTARGET
mkdir -p ${FDOTARGET}/lib


# Prepare for build
#
pushd $FDOBUILD
source ./setenvironment.sh


# Build thirdparty libraries for core Fdo libraries
#
# usage for build_thirdparty.sh
# build_thirdparty.sh --w fdo/sdf/wfs/wms/gdal/ogr --a build/install --p prefix
#
./build_thirdparty.sh --a build --p $FDOTARGET
./build_thirdparty.sh --a install --p $FDOTARGET


# Build core Fdo libraries
#
# usage for build_linux.sh
# build_linux.sh --w fdocore/fdo/utilities/shp/sdf/wfs/wms/arcsde/rdbms/gdal/ogr/postgis/kingoracle/sqlite --a build/install --p prefix
#
./build_linux.sh --w fdo --a build --p $FDOTARGET
./build_linux.sh --w utilities --a build --p $FDOTARGET
./build_linux.sh --w fdocore --a build --p $FDOTARGET
./build_linux.sh --w fdocore --a install --p $FDOTARGET
popd


# Create file and directory listing for core Fdo libraries and headers
#
pushd $FDOTARGET
mkdir -p $BUILDLIST
find . -type f -print > ${BUILDLIST}/temp.lst
find . -type l -print >> ${BUILDLIST}/temp.lst
sort ${BUILDLIST}/temp.lst > ${BUILDLIST}/fdocore.lst
find . -type d -print | sort > ${BUILDLIST}/fdocoredir.lst
popd

