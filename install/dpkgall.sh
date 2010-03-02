#!/bin/bash
ARCH=i386
BUILDNUM=5259
./dpkgfdocore.sh $ARCH $BUILDNUM
./dpkgfdoprovider.sh $ARCH $BUILDNUM sdf
./dpkgfdoprovider.sh $ARCH $BUILDNUM shp
./dpkgfdoprovider.sh $ARCH $BUILDNUM wfs
./dpkgfdoprovider.sh $ARCH $BUILDNUM wms
# ArcSDE provider currently disabled due to missing libraries
#./dpkgfdoprovider.sh arcsde
./dpkgfdoprovider.sh $ARCH $BUILDNUM rdbms
./dpkgfdoprovider.sh $ARCH $BUILDNUM gdal
./dpkgfdoprovider.sh $ARCH $BUILDNUM ogr
./dpkgfdoprovider.sh $ARCH $BUILDNUM postgis
./dpkgfdoprovider.sh $ARCH $BUILDNUM kingoracle
./dpkgfdoprovider.sh $ARCH $BUILDNUM sqlite
