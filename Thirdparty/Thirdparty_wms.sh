#!/bin/csh

#############################################
## build third party libraries for WMS:    ##
## boost_1_32_0, libcurl, openssl, GDAL1.3 ##
#############################################

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
./configure
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
make clean
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
./configure
make
cp -f ./.libs/libgdal.a ../linux/lib/optimized
cp -f ./.libs/libgdal.so ../linux/lib/optimized
cp -f ./.libs/libgdal.so.1 ../linux/lib/optimized
cp -f ./.libs/libgdal.so.1.9.0 ../linux/lib/optimized
popd >& /dev/null
