#!/bin/sh

TYPEACTION=buildinstall
TYPEBUILD=release
TYPECONFIGURE=

DEFMODIFY=no
THRPENABLE=yes
FDOENABLE=yes
UTILENABLE=yes
SHPENABLE=no
SDFENABLE=no
WFSENABLE=no
WMSENABLE=no
ARCENABLE=no
RDBMSENABLE=no
SHOWHELP=no


### study parameters ###
while test $# -gt 0
do
  arg="$1"
  shift
  case $arg in
  --h | --help)
    SHOWHELP=yes
    break
    ;;
  --a | --action)
    if test "$1" == buildinstall; then
        TYPEACTION=buildinstall
    elif test "$1" == buildonly; then
        TYPEACTION=buildonly
    elif test "$1" == installonly; then
        TYPEACTION=installonly
    elif test "$1" == configure; then
        TYPECONFIGURE=configure
    else
        echo "Invalid parameter"
	exit 1
    fi
    shift
    ;;
  --c | --config)
    if test "$1" == debug; then
        TYPEBUILD=debug
    elif test "$1" == release; then
        TYPEBUILD=release
    else
        echo "Invalid parameter"
	exit 1
    fi
    shift
    ;;
  --w | --with)
     if test "$DEFMODIFY" == no; then
	DEFMODIFY=yes
	THRPENABLE=no
	FDOENABLE=no
	SHPENABLE=no
	SDFENABLE=no
	WFSENABLE=no
	WMSENABLE=no
	ARCENABLE=no
	RDBMSENABLE=no
	UTILENABLE=no
     fi
     if test -z "$1"; then
        echo "Invalid parameter"
	exit 1
     elif test "$1" == fdocore; then
	THRPENABLE=yes
	FDOENABLE=yes
	UTILENABLE=yes
     elif test "$1" == providers; then
	SHPENABLE=yes
	SDFENABLE=yes
	WFSENABLE=yes
	WMSENABLE=yes
	ARCENABLE=yes
	RDBMSENABLE=yes
     elif test "$1" == thirdparty; then
        THRPENABLE=yes
     elif test "$1" == utilities; then
        UTILENABLE=yes
     elif test "$1" == fdo; then
        FDOENABLE=yes
     elif test "$1" == shp; then
        SHPENABLE=yes
     elif test "$1" == sdf; then
        SDFENABLE=yes
     elif test "$1" == wfs; then
        WFSENABLE=yes
     elif test "$1" == wms; then
        WMSENABLE=yes
     elif test "$1" == arcsde; then
        ARCENABLE=yes
     elif test "$1" == rdbms; then
        RDBMSENABLE=yes
     else
        echo "Invalid parameter"
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
    echo "The command is not recognized."
    echo "Please use the format:"
    SHOWHELP=yes
    break
    ;;
  esac
done
### end of study parameters ###

if test "$SHOWHELP" == yes; then

   echo "**************************************************************************"
   echo "build_linux.sh [--h] [-c=BuildType] [-a=Action] [--w WithModule] "
   echo "*"
   echo "Help:           --h[elp]"
   echo "BuildType:      --c[onfig] release(default), debug"
   echo "Action:         --a[ction] buildinstall(default), buildonly, installonly, configure"

   HELPSTRINGWITH= WithModule: --w[ith] fdocore(default), fdo, thirdparty, providers
   if test ! -e "Providers/SHP/build_linux.sh"; then
   HELPSTRINGWITH=$HELPSTRINGWITH, shp
   fi
   if test ! -e "Providers/SDF/build_linux.sh"; then
   HELPSTRINGWITH=$HELPSTRINGWITH, sdf
   fi
   if test ! -e "Providers/WFS/build_linux.sh"; then
   HELPSTRINGWITH=$HELPSTRINGWITH, wfs
   fi
   if test ! -e "Providers/WMS/build_linux.sh"; then
   HELPSTRINGWITH=$HELPSTRINGWITH, wms
   fi
   if test ! -e "Providers/ArcSDE/build_linux.sh"; then
   HELPSTRINGWITH=$HELPSTRINGWITH, arcsde
   fi
   if test ! -e "Providers/GenericRdbms/build_linux.sh"; then
   HELPSTRINGWITH=$HELPSTRINGWITH, rdbms
   fi
   
   echo "$HELPSTRINGWITH"
   echo "**************************************************************************"

   exit 0
fi

### configure build ###
if test "$TYPECONFIGURE" == configure ; then
   aclocal
   libtoolize --force
   automake --add-missing --copy
   autoconf

   if test "$TYPEBUILD" == release; then
      ./configure
   else
      ./configure --enable-debug=yes
   fi
fi

### start build ###

CMDEX= --c $TYPEBUILD --a $TYPEACTION
if test "$TYPECONFIGURE" == configure ; then
   CMDEX=$CMDEX --a $TYPECONFIGURE
fi

#build Thirdparty
if test "$THRPENABLE" == yes; then
   if test ! -e "Thirdparty/Thirdparty.sh"; then
      echo "Thirdparty/Thirdparty.sh is missing!"
      exit 1;
   fi

   pushd "Thirdparty" >& /dev/null
   if test "$TYPEACTION" == buildinstall || test "$TYPEACTION" == buildonly ; then
      ./Thirdparty.sh
   fi
   if test "$TYPEACTION" == buildinstall || test "$TYPEACTION" == installonly ; then
      make install
   fi
   popd >& /dev/null
fi

#build Fdo
if test "$FDOENABLE" == yes; then
   pushd Fdo >& /dev/null
   if test "$TYPEACTION" == buildinstall || test "$TYPEACTION" == buildonly ; then
      make
   fi
   if test "$TYPEACTION" == buildinstall || test "$TYPEACTION" == installonly ; then
      make install
   fi
   popd >& /dev/null
fi

#build Utilities
if test "$UTILENABLE" == yes; then
   pushd Utilities >& /dev/null
   if test "$TYPEACTION" == buildinstall || test "$TYPEACTION" == buildonly ; then
      make
   fi
   if test "$TYPEACTION" == buildinstall || test "$TYPEACTION" == installonly ; then
      make install
   fi
   popd >& /dev/null
fi

#build SHP Provider
if test "$SHPENABLE" == yes; then
   if test ! -e "Providers/SHP/build_linux.sh"; then
      echo "Providers/SHP/build_linux.sh is missing!"
      exit 1;
   fi

   pushd Providers/SHP >& /dev/null
   ./build_linux.sh $CMDEX
   popd >& /dev/null
fi

#build SDF Provider
if test "$SDFENABLE" == yes; then
   if test ! -e "Providers/SDF/build_linux.sh"; then
      echo "Providers/SDF/build_linux.sh is missing!"
      exit 1;
   fi

   pushd Providers/SDF >& /dev/null
   ./build_linux.sh $CMDEX
   popd >& /dev/null
fi

#build WFS Provider
if test "$WFSENABLE" == yes; then
   if test ! -e "Providers/WFS/build_linux.sh"; then
      echo "Providers/WFS/build_linux.sh is missing!"
      exit 1;
   fi

   pushd Providers/WFS >& /dev/null
   ./build_linux.sh $CMDEX
   popd >& /dev/null
fi

#build WMS Provider
if test "$WMSENABLE" == yes; then
   if test ! -e "Providers/WMS/build_linux.sh"; then
      echo "Providers/WMS/build_linux.sh is missing!"
      exit 1;
   fi

   pushd Providers/WMS >& /dev/null
   ./build_linux.sh $CMDEX
   popd >& /dev/null
fi

#build ArcSDE Provider
if test "$ARCENABLE" == yes; then
   if test ! -e "Providers/ArcSDE/build_linux.sh"; then
      echo "Providers/ArcSDE/build_linux.sh is missing!"
      exit 1;
   fi

   pushd Providers/ArcSDE >& /dev/null
   ./build_linux.sh $CMDEX
   popd >& /dev/null
fi

#build GenericRdbms Providers
if test "$RDBMSENABLE" == yes; then
   if test ! -e "Providers/GenericRdbms/build_linux.sh"; then
      echo "Providers/GenericRdbms/build_linux.sh is missing!"
      exit 1;
   fi

   pushd Providers/GenericRdbms >& /dev/null
   ./build_linux.sh $CMDEX
   popd >& /dev/null
fi

exit 0

