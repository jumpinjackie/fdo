#!/bin/bash
# Build Fdo providers on Linux and extract file listing for packaging
# Note:  buildfdocore.sh must be run before this script to avoid build failures
#
BUILDDIR=`pwd`
BUILDLIST=${BUILDDIR}/filelist
FDOBUILD=${BUILDDIR}/..
FDOTARGET=/usr/local/fdo-3.5.0

# Check for provider argument
#
arg="$1"
case $arg in
sdf)
  PROVIDER=sdf
  THIRDPARTY=sdf
  ;;
shp)
  PROVIDER=shp
  THIRDPARTY=
  ;;
wfs)
  PROVIDER=wfs
  THIRDPARTY=wfs
  ;;
wms)
  PROVIDER=wms
  THIRDPARTY=wms
  ;;
arcsde)
  PROVIDER=arcsde
  THIRDPARTY=
  ;;
rdbms)
  PROVIDER=rdbms
  THIRDPARTY=
  ;;
gdal)
  PROVIDER=gdal
  THIRDPARTY=gdal
  ;;
ogr)
  PROVIDER=ogr
  THIRDPARTY=ogr
  ;;
postgis)
  PROVIDER=postgis
  THIRDPARTY=
  ;;
kingoracle)
  PROVIDER=kingoracle
  THIRDPARTY=
  ;;
sqlite)
  PROVIDER=sqlite
  THIRDPARTY=
  ;;
*)
  echo Usage: buildfdoprovider.sh [provider], where [provider] is one of:
  echo    sdf,shp,wfs,wms,arcsde,rdbms,gdal,ogr,postgis,kingoracle,sqlite
  exit -1
  ;;
esac

# Save existing build tree for later comparison
#
pushd $FDOTARGET
find . -type f -print > ${BUILDLIST}/temp.lst
find . -type l -print >> ${BUILDLIST}/temp.lst
sort ${BUILDLIST}/temp.lst > ${BUILDLIST}/orig.lst
find . -type d -print | sort > ${BUILDLIST}/origdir.lst
popd


# Prepare build environment
#
pushd $FDOBUILD
source ./setenvironment.sh


# Build thirdparty libraries for provider
#
# usage for build_thirdparty.sh
# build_thirdparty.sh --w fdo/sdf/wfs/wms/gdal/ogr --a build/install --p prefix
#
if test -n "$THIRDPARTY"; then
  ./build_thirdparty.sh --w $THIRDPARTY --a build --p $FDOTARGET
  ./build_thirdparty.sh --w $THIRDPARTY --a install --p $FDOTARGET
fi


# Build provider
#
# usage for build_linux.sh
# build_linux.sh --w fdocore/fdo/utilities/shp/sdf/wfs/wms/arcsde/rdbms/gdal/ogr/postgis/kingoracle/sqlite --a build/install --p prefix
#
./build_linux.sh --w $PROVIDER --a build --p $FDOTARGET
./build_linux.sh --w $PROVIDER --a install --p $FDOTARGET

popd


# Create file and directory listings for SDF Provider by
# adding original directory listing to current directory listing
# and then using sort, unique to strip it
#
pushd $FDOTARGET
mkdir -p $BUILDLIST
find . -type f -print > ${BUILDLIST}/temp.lst
find . -type l -print >> ${BUILDLIST}/temp.lst
cat ${BUILDLIST}/orig.lst >> ${BUILDLIST}/temp.lst
sort ${BUILDLIST}/temp.lst | uniq -u > ${BUILDLIST}/${PROVIDER}.lst
find . -type d -print | sort > ${BUILDLIST}/temp.lst
cat ${BUILDLIST}/origdir.lst >> ${BUILDLIST}/temp.lst
sort ${BUILDLIST}/temp.lst | uniq -u > ${BUILDLIST}/${PROVIDER}dir.lst
popd

