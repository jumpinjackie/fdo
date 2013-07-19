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


TYPEACTION=buildinstall
TYPEBUILD=release
TYPEARCHITECTURE=32
TYPECONFIGURE=configure
BUILDDOCS=no
PREFIXVAL=/usr/local/fdo-3.8.1

### study parameters ###
while test $# -gt 0
do
  arg="$1"
  shift
  case $arg in
  -h | --h | --help)
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
  -d | --d | --docs)
    if test "$1" == skip; then
        BUILDDOCS=no
    elif test "$1" == build; then
        BUILDDOCS=yes
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
   echo "************************************************************************************************************"
   echo "build_linux.sh [--h]"
   echo "               [--c BuildType]"
   echo "               [--a Action]"
   echo "               [--d BuildDocs]"
   echo "               [--m ConfigMakefiles]"
   echo "               [--p Prefix]"
   echo "               [--b BuildArchicture]"
   echo " "
   echo "Help:                  --h[elp]"
   echo "BuildType:             --c[onfig] release(default), debug"
   echo "Action:                --a[ction] buildinstall(default), build, install, uninstall, clean"
   echo "BuildDocs:             --d[ocs] skip(default), build"
   echo "ConfigMakefiles:       --m[akefile] configure(default), noconfigure"
   echo "BuildArchitecture:     --b[uild] 32(default), 64"
   echo "Prefix:                --p[refix] <fdo install location>"
   echo "************************************************************************************************************"
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

### start build ###
if test "$TYPECONFIGURE" == configure ; then
   if test -e "m4"; then
      echo "m4 directory exists"
   else
      mkdir m4
   fi

   aclocal
   if [ "$?" -ne 0 ] ; then
     exit 1
   fi

   libtoolize --force
   if [ "$?" -ne 0 ] ; then
     exit 1
   fi

   automake --add-missing --copy
   if [ "$?" -ne 0 ] ; then
     exit 1
   fi

   autoconf
   if [ "$?" -ne 0 ] ; then
     exit 1
   fi

   if test "$TYPEBUILD" == release; then
      ./configure --prefix="$PREFIXVAL"
      if [ "$?" -ne 0 ] ; then
        exit 1
      fi
   else
      ./configure --enable-debug=yes --prefix="$PREFIXVAL"
      if [ "$?" -ne 0 ] ; then
        exit 1
      fi
   fi
fi
   
if test "$TYPEACTION" == clean ; then
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
fi

if test "$TYPEACTION" == buildinstall || test "$TYPEACTION" == install ; then
   sudo make install
   if [ "$?" -ne 0 ] ; then
     exit 1
   fi
fi

if test "$TYPEACTION" == uninstall ; then
   sudo make uninstall
   if [ "$?" -ne 0 ] ; then
     exit 1
   fi
fi

if test "$BUILDDOCS" == yes ; then
   echo Creating SHP provider html documentation
   rm -rf Docs/HTML/SHP
   mkdir -p Docs/HTML/SHP
   cp ../../DocResources/geospatial.js Docs/HTML/SHP
   cp ../../DocResources/osgeo.css Docs/HTML/SHP

   rm -rf Docs/HTML/SHP_managed
   mkdir -p Docs/HTML/SHP_managed
   cp ../../DocResources/geospatial.js Docs/HTML/SHP_managed
   cp ../../DocResources/osgeo.css Docs/HTML/SHP_managed

   pushd Docs/doc_src >& /dev/null
   doxygen Doxyfile_SHP >& /dev/null
   doxygen Doxyfile_SHP_managed >& /dev/null
   popd >& /dev/null
fi

if test "$TYPEACTION" == buildinstall || test "$TYPEACTION" == install ; then
   sudo rm -rf "$PREFIXVAL/docs/HTML/Providers/SHP"
   sudo mkdir -p "$PREFIXVAL/docs/HTML/Providers"
   if test -e "Docs/HTML/SHP"; then
      sudo cp --force --recursive "Docs/HTML/SHP" "$PREFIXVAL/docs/HTML/Providers"
      sudo cp --force --recursive "Docs/HTML/SHP_managed" "$PREFIXVAL/docs/HTML/Providers"
   fi
fi

exit 0

