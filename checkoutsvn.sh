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
FDOALLENABLECHK=yes
FDOCOREENABLECHK=no
FDOPROVIDERSENABLECHK=no
THRPENABLECHK=no
FDOENABLECHK=no
UTILENABLECHK=no
SHPENABLECHK=no
SDFENABLECHK=no
WFSENABLECHK=no
WMSENABLECHK=no
ARCENABLECHK=no
RDBMSENABLECHK=no
GDALENABLECHK=no
KINGENABLECHK=no
OGRENABLECHK=no
SHOWHELP=no

FDO_SVN_USERNAME=
FDO_SVN_PASSWORD=
FDO_SVN_SOURCEDIR=

if test -z "$FDO_SVN_SOURCEDIR"; then
   FDO_SVN_SOURCEDIR=trunk
fi

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
        FDOALLENABLECHK=no
        FDOCOREENABLECHK=no
        FDOPROVIDERSENABLECHK=no
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
        KINGENABLECHK=no
        OGRENABLECHK=no
     fi
     if test -z "$1"; then
        echo "Invalid parameter"
	    exit 1
     elif test "$1" == providers; then
        FDOPROVIDERSENABLECHK=yes
     elif test "$1" == all; then
        FDOALLENABLECHK=yes
        FDOPROVIDERSENABLECHK=no
        FDOCOREENABLECHK=no
        THRPENABLECHK=no
        FDOENABLECHK=no
        UTILENABLECHK=no
        SHPENABLECHK=no
        SDFENABLECHK=no
        WFSENABLECHK=no
        WMSENABLECHK=no
        ARCENABLECHK=no
        RDBMSENABLECHK=no
        GDALENABLECHK=no
        KINGENABLECHK=no
        OGRENABLECHK=no
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
     elif test "$1" == king; then
        KINGENABLECHK=yes
     elif test "$1" == ogr; then
        OGRENABLECHK=yes
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
   echo "                         fdocore"
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
   echo "                         king"
   echo "                         ogr"
   echo "User:           --u[ser]=user id"
   echo "Password:       --p[assword]=user password"
   echo "**********************************************************"
   exit 0
fi

if test ! -e "$FDO_SVN_DESTDIR"; then
   mkdir "$FDO_SVN_DESTDIR"
fi

# Check out All source files
if test "$FDOALLENABLECHK" == "yes"; then
   echo "Checking out https://osgeo.org/svn/fdo/$FDO_SVN_SOURCEDIR"
   svn checkout https://osgeo.org/svn/fdo/$FDO_SVN_SOURCEDIR "$FDO_SVN_DESTDIR" --username "$FDO_SVN_USERNAME" --password "$FDO_SVN_PASSWORD"
fi
# Check out All Provider source files
if test "$FDOPROVIDERSENABLECHK" == yes; then
   echo "Checking out https://osgeo.org/svn/fdo/$FDO_SVN_SOURCEDIR/Providers"
   svn checkout https://osgeo.org/svn/fdo/$FDO_SVN_SOURCEDIR/Providers "$FDO_SVN_DESTDIR/Providers" --username "$FDO_SVN_USERNAME" --password "$FDO_SVN_PASSWORD"
fi
# Check out fdocore source files: FDO, Thirdparty, Utilities
if test "$FDOCOREENABLECHK" == yes; then
   echo "Checking out https://osgeo.org/svn/fdo/$FDO_SVN_SOURCEDIR/Fdo"
   svn checkout https://osgeo.org/svn/fdo/$FDO_SVN_SOURCEDIR/Fdo "$FDO_SVN_DESTDIR/Fdo" --username "$FDO_SVN_USERNAME" --password "$FDO_SVN_PASSWORD"
   echo "Checking out https://osgeo.org/svn/fdo/$FDO_SVN_SOURCEDIR/Thirdparty"
   svn checkout https://osgeo.org/svn/fdo/$FDO_SVN_SOURCEDIR/Thirdparty "$FDO_SVN_DESTDIR/Thirdparty" --username "$FDO_SVN_USERNAME" --password "$FDO_SVN_PASSWORD"
   echo "Checking out https://osgeo.org/svn/fdo/$FDO_SVN_SOURCEDIR/Utilities"
   svn checkout https://osgeo.org/svn/fdo/$FDO_SVN_SOURCEDIR/Utilities "$FDO_SVN_DESTDIR/Utilities" --username "$FDO_SVN_USERNAME" --password "$FDO_SVN_PASSWORD"
fi
# Check out Fdo source files
if test "$FDOCOREENABLECHK" == no && test "$FDOENABLECHK" == yes; then
   echo "Checking out https://osgeo.org/svn/fdo/$FDO_SVN_SOURCEDIR/Fdo"
   svn checkout https://osgeo.org/svn/fdo/$FDO_SVN_SOURCEDIR/Fdo "$FDO_SVN_DESTDIR/Fdo" --username "$FDO_SVN_USERNAME" --password "$FDO_SVN_PASSWORD"
fi
# Check out Thirdparty source files
if test "$FDOCOREENABLECHK" == no && test "$THRPENABLECHK" == yes; then
   echo "Checking out https://osgeo.org/svn/fdo/$FDO_SVN_SOURCEDIR/Thirdparty"
   svn checkout https://osgeo.org/svn/fdo/$FDO_SVN_SOURCEDIR/Thirdparty "$FDO_SVN_DESTDIR/Thirdparty" --username "$FDO_SVN_USERNAME" --password "$FDO_SVN_PASSWORD"
fi
# Check out Utility source files
if test "$FDOCOREENABLECHK" == no && test "$UTILENABLECHK" == yes; then
   echo "Checking out https://osgeo.org/svn/fdo/$FDO_SVN_SOURCEDIR/Utilities"
   svn checkout https://osgeo.org/svn/fdo/$FDO_SVN_SOURCEDIR/Utilities "$FDO_SVN_DESTDIR/Utilities" --username "$FDO_SVN_USERNAME" --password "$FDO_SVN_PASSWORD"
fi
# Check out SHP Provider source files
if test "$SHPENABLECHK" == yes; then
   echo "Checking out https://osgeo.org/svn/fdo/$FDO_SVN_SOURCEDIR/Providers/SHP"
   svn checkout https://osgeo.org/svn/fdo/$FDO_SVN_SOURCEDIR/Providers/SHP "$FDO_SVN_DESTDIR/Providers/SHP" --username "$FDO_SVN_USERNAME" --password "$FDO_SVN_PASSWORD"
fi
# Check out SDF Provider source files
if test "$SDFENABLECHK" == yes; then
   echo "Checking out https://osgeo.org/svn/fdo/$FDO_SVN_SOURCEDIR/Providers/SDF"
   svn checkout https://osgeo.org/svn/fdo/$FDO_SVN_SOURCEDIR/Providers/SDF "$FDO_SVN_DESTDIR/Providers/SDF" --username "$FDO_SVN_USERNAME" --password "$FDO_SVN_PASSWORD"
fi
# Check out WFS Provider source files
if test "$WFSENABLECHK" == yes; then
   echo "Checking out https://osgeo.org/svn/fdo/$FDO_SVN_SOURCEDIR/Providers/WFS"
   svn checkout https://osgeo.org/svn/fdo/$FDO_SVN_SOURCEDIR/Providers/WFS "$FDO_SVN_DESTDIR/Providers/WFS" --username "$FDO_SVN_USERNAME" --password "$FDO_SVN_PASSWORD"
fi
# Check out WMS Provider source files
if test "$WMSENABLECHK" == yes; then
   echo "Checking out https://osgeo.org/svn/fdo/$FDO_SVN_SOURCEDIR/Providers/WMS"
   svn checkout https://osgeo.org/svn/fdo/$FDO_SVN_SOURCEDIR/Providers/WMS "$FDO_SVN_DESTDIR/Providers/WMS" --username "$FDO_SVN_USERNAME" --password "$FDO_SVN_PASSWORD"
fi
# Check out ArcSDE Provider source files
if test "$ARCENABLECHK" == yes; then
   echo "Checking out https://osgeo.org/svn/fdo/$FDO_SVN_SOURCEDIR/Providers/ArcSDE"
   svn checkout https://osgeo.org/svn/fdo/$FDO_SVN_SOURCEDIR/Providers/ArcSDE "$FDO_SVN_DESTDIR/Providers/ArcSDE" --username "$FDO_SVN_USERNAME" --password "$FDO_SVN_PASSWORD"
fi
# Check out GenericRDBMS Providers source files
if test "$RDBMSENABLECHK" == yes; then
   echo "Checking out https://osgeo.org/svn/fdo/$FDO_SVN_SOURCEDIR/Providers/GenericRdbms"
   svn checkout https://osgeo.org/svn/fdo/$FDO_SVN_SOURCEDIR/Providers/GenericRdbms "$FDO_SVN_DESTDIR/Providers/GenericRdbms" --username "$FDO_SVN_USERNAME" --password "$FDO_SVN_PASSWORD"
fi
# Check out GDAL Provider source files
if test "$GDALENABLECHK" == yes; then
   echo "Checking out https://osgeo.org/svn/fdo/$FDO_SVN_SOURCEDIR/Providers/GDAL"
   svn checkout https://osgeo.org/svn/fdo/$FDO_SVN_SOURCEDIR/Providers/GDAL "$FDO_SVN_DESTDIR/Providers/GDAL" --username "$FDO_SVN_USERNAME" --password "$FDO_SVN_PASSWORD"
fi
# Check out King Oracle Provider source files
if test "$KINGENABLECHK" == yes; then
   echo "Checking out https://osgeo.org/svn/fdo/$FDO_SVN_SOURCEDIR/Providers/KingOracle"
   svn checkout https://osgeo.org/svn/fdo/$FDO_SVN_SOURCEDIR/Providers/KingOracle "$FDO_SVN_DESTDIR/Providers/KingOracle" --username "$FDO_SVN_USERNAME" --password "$FDO_SVN_PASSWORD"
fi
# Check out OGR Provider source files
if test "$OGRENABLECHK" == yes; then
   echo "Checking out https://osgeo.org/svn/fdo/$FDO_SVN_SOURCEDIR/Providers/OGR"
   svn checkout https://osgeo.org/svn/fdo/$FDO_SVN_SOURCEDIR/Providers/OGR "$FDO_SVN_DESTDIR/Providers/OGR" --username "$FDO_SVN_USERNAME" --password "$FDO_SVN_PASSWORD"
fi
exit 0
