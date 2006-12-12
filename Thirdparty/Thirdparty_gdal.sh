#!/bin/sh

##
## Copyright (C) 2004-2006  Autodesk, Inc.
## 
## This library is free software; you can redistribute it and/or
## modify it under the terms of version 2.1 of the GNU Lesser
## General Public License as published by the Free Software Foundation.
## 
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## Lesser General Public License for more details.
## 
## You should have received a copy of the GNU Lesser General Public
## License along with this library; if not, write to the Free Software
## Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
##

#############################################
## build third party libraries for GDAL:   ##
## GDAL1.3                                 ##
#############################################

pushd . >& /dev/null
cd GDAL1.3
echo Building GDAL1.3
mkdir -p linux/lib/optimized
rm -f linux/lib/optimized/*.*
cd src
chmod a+x ./configure
echo Build GDAL library with the following settings:
echo     gif support         - internal
echo     jpeg support        - internal
echo     png support         - internal
echo     tiff support        - internal
echo     geotiff support     - internal
echo     libz support        - internal
echo     python support      - no
echo     OGR support         - no
echo     postgreSQL support  - no
./configure --with-gif=internal --with-jpeg=internal --with-png=internal --with-libtiff=internal --with-geotiff=internal --without-ogr --with-pg=no --with-python=no --with-libz=internal
make
cp -f ./.libs/libgdal.a ../linux/lib/optimized
cp -f ./.libs/libgdal.so ../linux/lib/optimized
cp -f ./.libs/libgdal.so.1 ../linux/lib/optimized
cp -f ./.libs/libgdal.so.1.9.0 ../linux/lib/optimized
popd >& /dev/null
