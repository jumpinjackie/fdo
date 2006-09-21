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
make clean
make
mv -f ./libssl.a ./lib/linux/libssl.a
mv -f ./libcrypto.a ./lib/linux/libcrypto.a
popd >& /dev/null
