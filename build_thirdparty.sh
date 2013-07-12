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
TYPEBUILD=release
TYPECONFIGURE=configure
TYPEARCHITECTURE=32
PREFIXVAL=/usr/local/fdo-3.8.1

DEFMODIFY=no
ALLENABLE=yes
FDOENABLE=no
SDFENABLE=no
WFSENABLE=no
WMSENABLE=no
GDALENABLE=no
OGRENABLE=no

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
  -p | --p | --prefix)
    if test "$1" == ""; then
        echo "$arg Invalid parameter $1"
        exit 1
    else
        PREFIXVAL="$1"
    fi
    shift
    ;;
  -m | --m | --makefile)
    if test "$1" == configure; then
        TYPECONFIGURE=configure
    elif test "$1" == noconfigure; then
        TYPECONFIGURE=noconfigure
    else
        echo "$arg Invalid parameter $1"
	exit 1
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
  -c | --c | --config)
    if test "$1" == debug; then
        TYPEBUILD=debug
    elif test "$1" == release; then
        TYPEBUILD=release
    else
        echo "$arg Invalid parameter $1"
    exit 1
    fi
    shift
    ;;
  -w | --w | --with)
    if test "$DEFMODIFY" == no; then
       DEFMODIFY=yes
       ALLENABLE=no
       FDOENABLE=no
       SDFENABLE=no
       WFSENABLE=no
       WMSENABLE=no
       GDALENABLE=no
       OGRENABLE=no
    fi
    if test -z "$1"; then
       echo "$arg Invalid parameter $1"
       exit 1
    elif test "$1" == all; then
       ALLENABLE=yes
       FDOENABLE=no
       SDFENABLE=no
       WFSENABLE=no
       WMSENABLE=no
       GDALENABLE=no
       OGRENABLE=no
    elif test "$1" == fdo; then
        FDOENABLE=yes
    elif test "$1" == sdf; then
        SDFENABLE=yes
    elif test "$1" == wfs; then
        WFSENABLE=yes
    elif test "$1" == wms; then
        WMSENABLE=yes
    elif test "$1" == gdal; then
        GDALENABLE=yes
    elif test "$1" == ogr; then
        OGRENABLE=yes
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
   echo "build_thirdparty.sh [--h]"
   echo "                    [--a Action]"
   echo "                    [--c BuildType]" 
   echo "                    [--w WithModule]"
   echo "                    [--m ConfigMakefiles]"
   echo "                    [--p Prefix]"
   echo "                    [--b BuildArchicture]"
   echo " "
   echo "Help:                  --h[elp]"
   echo "BuildType:             --c[onfig] release(default), debug"
   echo "Action:                --a[ction] buildinstall(default), "
   echo "                                  build,"
   echo "                                  install,"
   echo "                                  uninstall,"
   echo "                                  clean"
   echo "WithModule:            --w[ith] all(default),"
   echo "                                fdo,"
   echo "                                sdf,"
   echo "                                wms,"
   echo "                                wfs,"
   echo "                                gdal"
   echo "                                ogr"
   echo "ConfigMakefiles:       --m[akefile] configure(default), noconfigure"
   echo "BuildArchitecture:     --b[uild] 32(default), 64"
   echo "Prefix:                --p[refix] <fdo install location>"
   echo "*******************************************************************"

   exit 0
fi


if [[ "$CFLAGS" != *"-m$TYPEARCHITECTURE"* ]]; then
CFLAGS="$CFLAGS -m$TYPEARCHITECTURE"
echo "Exporting CFLAGS: "$CFLAGS""
export CFLAGS
fi

if [[ "$CPPFLAGS" != *"-m$TYPEARCHITECTURE"* ]]; then
CPPFLAGS="$CPPFLAGS -m$TYPEARCHITECTURE"
echo "Exporting CPPFLAGS: "$CPPFLAGS""
export CPPFLAGS
fi

if [[ "$LDFLAGS" != *"-m$TYPEARCHITECTURE"* ]]; then
LDFLAGS="$LDFLAGS -m$TYPEARCHITECTURE"
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


if test "$TYPECONFIGURE" == configure ; then
  echo "Configuring FDO Makefiles"
  aclocal
  libtoolize --force
  automake --add-missing --copy
  autoconf

  chmod a+x ./configure

  if test "$TYPEBUILD" == release; then
     ./configure --prefix="$PREFIXVAL"
  else
     ./configure --enable-debug=yes --prefix="$PREFIXVAL"
  fi
fi

if test ! -e "Thirdparty/Thirdparty.sh"; then
  echo "Thirdparty/Thirdparty.sh is missing!"
  exit 1;
fi

CMDEX="-b $TYPEARCHITECTURE -a $TYPEACTION"

pushd "Thirdparty" >& /dev/null

if test "$TYPEACTION" == clean ; then
  make clean
fi

if test "$ALLENABLE" == yes ; then
  chmod a+x ./Thirdparty.sh
  sh ./Thirdparty.sh $CMDEX
elif test "$FDOENABLE" == yes ; then
  chmod a+x ./Thirdparty_fdo.sh
  sh ./Thirdparty_fdo.sh $CMDEX
elif test "$SDFENABLE" == yes ; then
  chmod a+x ./Thirdparty_sdf.sh
  sh ./Thirdparty_sdf.sh $CMDEX
elif test "$WMSENABLE" == yes ; then
  chmod a+x ./Thirdparty_wms.sh
  sh ./Thirdparty_wms.sh $CMDEX
elif test "$WFSENABLE" == yes ; then
  chmod a+x ./Thirdparty_wfs.sh
  sh ./Thirdparty_wfs.sh $CMDEX
elif test "$GDALENABLE" == yes ; then
  chmod a+x ./Thirdparty_gdal.sh
  sh ./Thirdparty_gdal.sh $CMDEX
elif test "$OGRENABLE" == yes ; then
  chmod a+x ./Thirdparty_ogr.sh
  sh ./Thirdparty_ogr.sh $CMDEX
fi

if test "$TYPEACTION" == buildinstall || test "$TYPEACTION" == install ; then
  sudo make install
fi

if test "$TYPEACTION" == uninstall ; then
  sudo make uninstall
fi

popd >& /dev/null

exit 0

