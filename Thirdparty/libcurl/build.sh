#!/bin/sh

##
## Copyright (C) 2013  Autodesk, Inc.
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

TYPEACTION=buildinstall
TYPEARCHITECTURE=32

### study parameters ###
while test $# -gt 0
do
  arg="$1"
  shift
  case $arg in
  -h |--h | --help)
    SHOWHELP=yes
    break
    ;;
  -b | --b | --build)
    if test "$1" == ""; then
        echo "$arg Invalid parameter $1"
        exit 1
    else
        TYPEARCHITECTURE="$1"
    fi
    shift
    ;;
  -a | --a | --action)
    if test "$1" == buildinstall; then
        TYPEACTION=buildinstall
    elif test "$1" == build; then
        TYPEACTION=build
    elif test "$1" == install; then
        TYPEACTION=install
    elif test "$1" == uninstall; then
        TYPEACTION=uninstall
    elif test "$1" == clean; then
        TYPEACTION=clean
    else
        echo "$arg Invalid parameter $1"
	exit 1
    fi
    shift
    ;;
  -*)
    echo "The command option is not recognized: $arg"
    echo "Please use the format:"
    SHOWHELP=yes
    break
    ;;

  *)
    echo "The command is not recognized: $arg"
    echo "Please use the format:"
    SHOWHELP=yes
    break
    ;;
  esac
done
### end of study parameters ###


if test "$SHOWHELP" == yes; then

   echo "*******************************************************************"
   echo "build.sh   [--h]"
   echo "           [--a Action]"
   echo "           [--b BuildArchicture]"
   echo " "
   echo "Help:                  --h[elp]"
   echo "Action:                --a[ction] buildinstall(default), build, install, uninstall, clean"
   echo "ConfigArchitecture:    --b[build] 32(default), 64"
   echo "*******************************************************************"

   exit 0
fi

# If you're building this copy of libcurl, chances are very high you also built the internal 
# openssl, and want to build/link against that internal copy.
#
# If you have a system-provided OpenSSL (chances are very high that this is the case) then
# libcurl will build against it by default even if you built the internal copy of openssl. Per 
# curl's build docs, if we want to *force* it to build/link against our internal openssl we have to
# append the internal openssl include/lib paths to CPPFLAGS and LDFLAGS.
CPPFLAGSEX="-I$PWD/../openssl/include"
LDFLAGSEX="-L$PWD/../openssl/lib/linux"

if [[ "$CFLAGS" != *"-m$TYPEARCHITECTURE"* ]]; then
CFLAGS="$CFLAGS -m$TYPEARCHITECTURE"
echo "Exporting CFLAGS: "$CFLAGS""
export CFLAGS
fi

if [[ "$CPPFLAGS" != *"-m$TYPEARCHITECTURE"* ]]; then
CPPFLAGS="$CPPFLAGS -m$TYPEARCHITECTURE $CPPFLAGSEX"
echo "Exporting CPPFLAGS: "$CPPFLAGS""
export CPPFLAGS
fi

if [[ "$LDFLAGS" != *"-m$TYPEARCHITECTURE"* ]]; then
LDFLAGS="$LDFLAGS -m$TYPEARCHITECTURE $LDFLAGSEX"
echo "Exporting LDFLAGS: "$LDFLAGS""
export LDFLAGS
fi

if test "$TYPEARCHITECTURE" == "32" ; then
if test "$HOSTTYPE" == "i686" ; then
if [[ "$CPPFLAGS" != *"-march=i686"* ]]; then
CPPFLAGS="$CPPFLAGS -march=i686"
echo "Exporting CPPFLAGS: "$CPPFLAGS""
export CPPFLAGS
fi
fi
fi

chmod a+x ./configure

# In conjunction with appending the internal openssl paths to CPPFLAGS/LDFLAGS, it also needs to be
# passed to --with-ssl so that the configure script takes the hint that we want to build/link against
# our internal copy of openssl
./configure --enable-silent-rules --without-libidn --without-librtmp --disable-ldap --with-ssl=$PWD/../openssl

mkdir -p lib/linux

if test "$TYPEACTION" == clean ; then
   rm -f ./lib/linux/*.*
   make clean
   if [ "$?" -ne 0 ] ; then
      exit 1
   fi
fi

if test "$TYPEACTION" == buildinstall || test "$TYPEACTION" == build ; then
   make
   if [ "$?" -ne 0 ] ; then
      exit 1
   fi
   rm -f ./lib/linux/*.*
   cp -f ./lib/.libs/libcurl.a ./lib/linux
fi





