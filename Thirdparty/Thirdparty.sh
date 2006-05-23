#!/bin/csh

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
./configure
make
mkdir -p lib/linux
rm -f ./lib/linux/*.*
cp -f ./lib/.libs/libcurl.a ./lib/linux
cp -f ./lib/.libs/libcurl.so ./lib/linux
cp -f ./lib/.libs/libcurl.so.3 ./lib/linux
cp -f ./lib/.libs/libcurl.so.3.0.0 ./lib/linux
popd >& /dev/null

pushd . >& /dev/null
cd openssl
echo Building openssl
mkdir -p lib/linux
rm -f ./lib/linux/*.*
chmod a+x ./config
./config shared
make
mv -f ./libssl.a ./lib/linux/libssl.a
mv -f ./libssl.so ./lib/linux/libssl.so
mv -f ./libssl.so.0 ./lib/linux/libssl.so.0
mv -f ./libssl.so.0.9.7 ./lib/linux/libssl.so.0.9.7
mv -f ./libcrypto.a ./lib/linux/libcrypto.a
mv -f ./libcrypto.so ./lib/linux/libcrypto.so
mv -f ./libcrypto.so.0 ./lib/linux/libcrypto.so.0
mv -f ./libcrypto.so.0.9.7 ./lib/linux/libcrypto.so.0.9.7
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

