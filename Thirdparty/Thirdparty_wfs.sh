#!/bin/csh

##########################################
## build third party libraries for WFS: ##
## boost_1_32_0, libcurl, openssl       ##
##########################################

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
make clean
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
