#!/bin/sh

###########################################
## build third party libraries:          ##
## apache, linux/cppunit, Sqlite3.1.5,   ##
## boost_1_32_0, libcurl, openssl        ##
###########################################

## Thirdparty_fdo
pushd . >& /dev/null
cd apache
echo Building apche
./build
popd >& /dev/null

pushd . >& /dev/null
cd linux/cppunit
echo Building linux/cppunit
./build
popd >& /dev/null


## Thirdparty_SDF
pushd . >& /dev/null
cd Sqlite3.1.5
echo Building Sqlite3.1.5
cd Src
make
popd >& /dev/null


## Thirdparty_OWS
pushd . >& /dev/null
cd boost_1_32_0
echo Building boost_1_32_0
cd tools/build/jam_src
chmod a+x ./build.sh
./build.sh
cd ../../../
./tools/build/jam_src/bin.linuxx86/bjam "-sTOOLS=gcc" stage
popd >& /dev/null

pushd . >& /dev/null
cd libcurl
echo Building libcurl
chmod a+x ./configure
./configure --without-libidn
make
mkdir -p lib/linux
rm -f ./lib/linux/*.*
cp -f ./lib/.libs/libcurl.a ./lib/linux
popd >& /dev/null

pushd . >& /dev/null
cd openssl
echo Building openssl
mkdir -p lib/linux
rm -f ./lib/linux/*.*
chmod a+x ./config
./config
make
mv -f ./libssl.a ./lib/linux/libssl.a
mv -f ./libcrypto.a ./lib/linux/libcrypto.a
popd >& /dev/null


## Thirdparty_WMS
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
