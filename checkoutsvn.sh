#!/bin/bash

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

# Check that we have a working $echo.
if test "X$1" = X--no-reexec; then
  # Discard the --no-reexec flag, and continue.
  shift
elif test "X$1" = X--fallback-echo; then
  # Avoid inline document here, it may be left over
  :
elif test "X`($echo '\t') 2>/dev/null`" = 'X\t'; then
  # Yippee, $echo works!
  :
else
  # Restart under the correct shell, and then maybe $echo will work.
  exec $SHELL "$0" --no-reexec ${1+"$@"}
fi

if test "X$1" = X--fallback-echo; then
  # used as fallback echo
  shift
  cat <<EOF
$*
EOF
  exit 0
fi

DEFMODIFYCHK=no
FDOCOREENABLECHK=yes
THRPENABLECHK=no
FDOENABLECHK=no
UTILENABLECHK=no
SHPENABLECHK=yes
SDFENABLECHK=yes
WFSENABLECHK=yes
WMSENABLECHK=yes
ARCENABLECHK=yes
RDBMSENABLECHK=yes
GDALENABLECHK=yes
SHOWHELP=no
FDO_SVN_USERNAME=
FDO_SVN_PASSWORD=
FDO_SVN_SOURCEDIR=

if test -z "$FDO_SVN_DESTDIR"; then
   FDO_SVN_DESTDIR=$PWD
fi

while test $# -gt 0
do
  arg="$1"
  shift
  case $arg in
  -h | --h | --help)
    SHOWHELP=yes
    break
    ;;
  -s | --s | --source)
     if test -z	 "$1"; then
        echo "Source location can not be empty"
	    exit 1
     else 
        FDO_SVN_SOURCEDIR="$1"
     fi
     shift
    ;;
  -o | --o | --outpath)
     if test -z	 "$1"; then
        echo "Destination folder can not be empty"
	    exit 1
     else 
        FDO_SVN_DESTDIR="$1"
     fi
     shift
    ;;
  -u | --u | --user)
     if test -z	 "$1"; then
        echo "User id can not be empty"
	    exit 1
     else 
        FDO_SVN_USERNAME="$1"
     fi
     shift
    ;;
  -p | --p | --password)
     if test -z	 "$1"; then
        echo "Password can not be empty"
        exit 1
     else 
        FDO_SVN_PASSWORD="$1"
     fi
     shift
    ;;
  -w | --w | --with)
     if test "$DEFMODIFYCHK" == no; then
        DEFMODIFYCHK=yes
        FDOCOREENABLECHK=no
        THRPENABLECHK=no
        FDOENABLECHK=no
        SHPENABLECHK=no
        SDFENABLECHK=no
        WFSENABLECHK=no
        WMSENABLECHK=no
        ARCENABLECHK=no
        RDBMSENABLECHK=no
        UTILENABLECHK=no
        GDALENABLECHK=no
     fi
     if test -z "$1"; then
        echo "Invalid parameter"
	    exit 1
     elif test "$1" == providers; then
        SHPENABLECHK=yes
        SDFENABLECHK=yes
        WFSENABLECHK=yes
        WMSENABLECHK=yes
        ARCENABLECHK=yes
        RDBMSENABLECHK=yes
        GDALENABLECHK=yes
     elif test "$1" == all; then
        FDOCOREENABLECHK=yes
        THRPENABLECHK=no
        FDOENABLECHK=no
        UTILENABLECHK=no
        SHPENABLECHK=yes
        SDFENABLECHK=yes
        WFSENABLECHK=yes
        WMSENABLECHK=yes
        ARCENABLECHK=yes
        RDBMSENABLECHK=yes
        GDALENABLECHK=yes
     elif test "$1" == fdocore; then
        FDOCOREENABLECHK=yes
        THRPENABLECHK=no
        FDOENABLECHK=no
        UTILENABLECHK=no
     elif test "$1" == thirdparty; then
        THRPENABLECHK=yes
     elif test "$1" == utilities; then
        UTILENABLECHK=yes
     elif test "$1" == fdo; then
        FDOENABLECHK=yes
     elif test "$1" == shp; then
        SHPENABLECHK=yes
     elif test "$1" == sdf; then
        SDFENABLECHK=yes
     elif test "$1" == wfs; then
        WFSENABLECHK=yes
     elif test "$1" == wms; then
        WMSENABLECHK=yes
     elif test "$1" == arcsde; then
        ARCENABLECHK=yes
     elif test "$1" == rdbms; then
        RDBMSENABLECHK=yes
     elif test "$1" == gdal; then
        GDALENABLECHK=yes
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
    echo "The command is not recognized: $arg"
    echo "Please use the format:"
    SHOWHELP=yes
    break
    ;;
  esac
done

if test "$FDO_SVN_USERNAME" == ""; then 
    SHOWHELP=yes 
fi
if test "$FDO_SVN_PASSWORD" == ""; then 
    SHOWHELP=yes 
fi
if test "$FDO_SVN_SOURCEDIR" == ""; then 
    SHOWHELP=yes 
fi

if test "$SHOWHELP" == yes; then
   echo "**********************************************************"
   echo "checkoutsvn.sh [--h]"
   echo "               [--o OutFolder]"
   echo "               [--w WithModule]"
   echo "               --s Source"
   echo "               --u UserId"
   echo "               --p UserPassword"
   echo " "
   echo "Help:           --h[elp]"
   echo "Source:         --s[ource]=location of source file, either"
   echo "                           branch location, tag location or trunk"
   echo "OutFolder:      --o[utpath]=destination folder for files"
   echo "WithModule:     --w[ith]=all(default)"
   echo "                         fdo"
   echo "                         thirdparty"
   echo "                         providers"
   echo "                         utilities"
   echo "                         shp"
   echo "                         sdf"
   echo "                         wfs"
   echo "                         wms"
   echo "                         arcsde"
   echo "                         rdbms"
   echo "                         gdal"
   echo "User:           --u[ser]=user id"
   echo "Password:       --p[assword]=user password"
   echo "**********************************************************"
   exit 0
fi

if test ! -e "$FDO_SVN_DESTDIR"; then
   mkdir "$FDO_SVN_DESTDIR"
fi

if test "$FDOCOREENABLECHK" == no && test "$FDOENABLECHK" == yes; then
   echo "Checking out https://fdocore.osgeo.org/svn/fdocore/$FDO_SVN_SOURCEDIR/Fdo"
   svn checkout https://fdocore.osgeo.org/svn/fdocore/$FDO_SVN_SOURCEDIR/Fdo "$FDO_SVN_DESTDIR/Fdo" --username "$FDO_SVN_USERNAME" --password "$FDO_SVN_PASSWORD"
fi
if test "$FDOCOREENABLECHK" == no && test "$THRPENABLECHK" == yes; then
   echo "Checking out https://fdocore.osgeo.org/svn/fdocore/$FDO_SVN_SOURCEDIR/Thirdparty"
   svn checkout https://fdocore.osgeo.org/svn/fdocore/$FDO_SVN_SOURCEDIR/Thirdparty "$FDO_SVN_DESTDIR/Thirdparty" --username "$FDO_SVN_USERNAME" --password "$FDO_SVN_PASSWORD"
fi
if test "$FDOCOREENABLECHK" == no && test "$UTILENABLECHK" == yes; then
   echo "Checking out https://fdocore.osgeo.org/svn/fdocore/$FDO_SVN_SOURCEDIR/Utilities"
   svn checkout https://fdocore.osgeo.org/svn/fdocore/$FDO_SVN_SOURCEDIR/Utilities "$FDO_SVN_DESTDIR/Utilities" --username "$FDO_SVN_USERNAME" --password "$FDO_SVN_PASSWORD"
fi
if test "$FDOCOREENABLECHK" == yes; then
   echo "Recursively checking out https://fdocore.osgeo.org/svn/fdocore/$FDO_SVN_SOURCEDIR"
   svn checkout https://fdocore.osgeo.org/svn/fdocore/$FDO_SVN_SOURCEDIR "$FDO_SVN_DESTDIR" --username "$FDO_SVN_USERNAME" --password "$FDO_SVN_PASSWORD"
fi
if test "$SHPENABLECHK" == yes; then
   echo "Checking out https://fdoshp.osgeo.org/svn/fdoshp/$FDO_SVN_SOURCEDIR/Providers/SHP"
   svn checkout https://fdoshp.osgeo.org/svn/fdoshp/$FDO_SVN_SOURCEDIR/Providers/SHP "$FDO_SVN_DESTDIR/Providers/SHP" --username "$FDO_SVN_USERNAME" --password "$FDO_SVN_PASSWORD"
fi
if test "$SDFENABLECHK" == yes; then
   echo "Checking out https://fdosdf.osgeo.org/svn/fdosdf/$FDO_SVN_SOURCEDIR/Providers/SDF"
   svn checkout https://fdosdf.osgeo.org/svn/fdosdf/$FDO_SVN_SOURCEDIR/Providers/SDF "$FDO_SVN_DESTDIR/Providers/SDF" --username "$FDO_SVN_USERNAME" --password "$FDO_SVN_PASSWORD"
fi
if test "$WFSENABLECHK" == yes; then
   echo "Checking out https://fdowfs.osgeo.org/svn/fdowfs/$FDO_SVN_SOURCEDIR/Providers/WFS"
   svn checkout https://fdowfs.osgeo.org/svn/fdowfs/$FDO_SVN_SOURCEDIR/Providers/WFS "$FDO_SVN_DESTDIR/Providers/WFS" --username "$FDO_SVN_USERNAME" --password "$FDO_SVN_PASSWORD"
fi
if test "$WMSENABLECHK" == yes; then
   echo "Checking out https://fdowms.osgeo.org/svn/fdowms/$FDO_SVN_SOURCEDIR/Providers/WMS"
   svn checkout https://fdowms.osgeo.org/svn/fdowms/$FDO_SVN_SOURCEDIR/Providers/WMS "$FDO_SVN_DESTDIR/Providers/WMS" --username "$FDO_SVN_USERNAME" --password "$FDO_SVN_PASSWORD"
fi
if test "$ARCENABLECHK" == yes; then
   echo "Checking out https://fdoarcsde.osgeo.org/svn/fdoarcsde/$FDO_SVN_SOURCEDIR/Providers/ArcSDE"
   svn checkout https://fdoarcsde.osgeo.org/svn/fdoarcsde/$FDO_SVN_SOURCEDIR/Providers/ArcSDE "$FDO_SVN_DESTDIR/Providers/ArcSDE" --username "$FDO_SVN_USERNAME" --password "$FDO_SVN_PASSWORD"
fi
if test "$RDBMSENABLECHK" == yes; then
   echo "Checking out https://fdordbms.osgeo.exit 0org/svn/fdordbms/$FDO_SVN_SOURCEDIR/Providers/GenericRdbms"
   svn checkout https://fdordbms.osgeo.org/svn/fdordbms/$FDO_SVN_SOURCEDIR/Providers/GenericRdbms "$FDO_SVN_DESTDIR/Providers/GenericRdbms" --username "$FDO_SVN_USERNAME" --password "$FDO_SVN_PASSWORD"
fi
if test "$GDALENABLECHK" == yes; then
   echo "Checking out https://fdogdal.osgeo.org/svn/fdogdal/$FDO_SVN_SOURCEDIR/Providers/GDAL"
   svn checkout https://fdogdal.osgeo.org/svn/fdogdal/$FDO_SVN_SOURCEDIR/Providers/GDAL "$FDO_SVN_DESTDIR/Providers/GDAL" --username "$FDO_SVN_USERNAME" --password "$FDO_SVN_PASSWORD"
fi
echo "End checkout" 
exit 0
