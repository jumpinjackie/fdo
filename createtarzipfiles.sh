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
SHPENABLECHK=yes
SDFENABLECHK=yes
WFSENABLECHK=yes
WMSENABLECHK=yes
ARCENABLECHK=yes
RDBMSENABLECHK=yes
GDALENABLECHK=yes
KINGENABLECHK=yes
OGRENABLECHK=yes

SHOWHELP=no

FDOTARZIPFOLDER=/OpenSource_FDO
FDOBUILDNUMBER=GXXX

if test -z "$FDOSVNROOT"; then
   FDOSVNROOT=$PWD
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
  -b | --b | --build)
     if test -z	 "$1"; then
        echo "build number cannot be empty"
	    exit 1
     else 
        FDOBUILDNUMBER="$1"
     fi
     shift
    ;;
  -o | --o | --outpath)
     if test -z	 "$1"; then
        echo "Destination folder can not be empty"
	    exit 1
     else 
        FDOTARZIPFOLDER="$1"
     fi
     shift
    ;;
  -i | --i | --inpath)
     if test -z	 "$1"; then
        echo "Destination folder can not be empty"
	    exit 1
     else 
        FDOSVNROOT="$1"
     fi
     shift
    ;;
  -w | --w | --with)
     if test "$DEFMODIFYCHK" == no; then
	    DEFMODIFYCHK=yes
	    FDOCOREENABLECHK=no
	    SHPENABLECHK=no
	    SDFENABLECHK=no
	    WFSENABLECHK=no
	    WMSENABLECHK=no
	    ARCENABLECHK=no
	    RDBMSENABLECHK=no
	    GDALENABLECHK=no
	    KINGENABLECHK=no
	    OGRENABLECHK=no
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
	    KINGENABLECHK=yes
	    OGRENABLECHK=yes
     elif test "$1" == all; then
	    FDOCOREENABLECHK=yes
	    SHPENABLECHK=yes
	    SDFENABLECHK=yes
	    WFSENABLECHK=yes
	    WMSENABLECHK=yes
	    ARCENABLECHK=yes
	    RDBMSENABLECHK=yes
	    GDALENABLECHK=yes
	    KINGENABLECHK=yes
	    OGRENABLECHK=yes
     elif test "$1" == fdo; then
	    FDOCOREENABLECHK=yes
	    THRPENABLECHK=no
	    FDOENABLECHK=no
	    UTILENABLECHK=no
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


if test "$SHOWHELP" == yes; then
   echo "**************************************************************************"
   echo "createtarzipfiles.sh [--h]"
   echo "                     [--i InFolder]"
   echo "                     [--o OutFolder]" 
   echo "                     [--w WithModule]" 
   echo "                     [--b BuildNumber]"
   echo " "
   echo "Help:           --h[elp]"
   echo "InFolder:       --i[npath]=input source svn checkout folder"
   echo "OutFolder:      --o[utpath]=destination folder for exported svn files"
   echo "WithModule:     --w[ith]=all(default)"
   echo "                         fdo"
   echo "                         providers"
   echo "                         shp"
   echo "                         sdf"
   echo "                         wfs"
   echo "                         wms"
   echo "                         arcsde"
   echo "                         rdbms"
   echo "                         gdal"
   echo "                         king"
   echo "                         ogr"
   echo "BuildNumber:    --b[uild]=User-Defined build number appended"
   echo "                          to the end of the tar.gz files"
   echo "**************************************************************************"
   exit 0
fi

if test "$FDOCOREENABLECHK" == yes; then
   mkdir -p "$FDOTARZIPFOLDER"
   svn export "$FDOSVNROOT" "$FDOTARZIPFOLDER" --non-recursive --force
   svn export "$FDOSVNROOT"/Fdo "$FDOTARZIPFOLDER"/Fdo --force
   svn export "$FDOSVNROOT"/Utilities "$FDOTARZIPFOLDER"/Utilities --force
   svn export "$FDOSVNROOT"/Thirdparty "$FDOTARZIPFOLDER"/Thirdparty --force
   find "$FDOTARZIPFOLDER" -name .svn | xargs rm -rf
   rm -f fdo-3.3.0_"$FDOBUILDNUMBER".tar
   tar -cf fdo-3.3.0_"$FDOBUILDNUMBER".tar "$FDOTARZIPFOLDER"
   rm -f fdo-3.3.0_"$FDOBUILDNUMBER".tar.gz
   gzip -9 fdo-3.3.0_"$FDOBUILDNUMBER".tar
   rm -rf "$FDOTARZIPFOLDER"
fi
if test "$SHPENABLECHK" == yes; then
   mkdir -p "$FDOTARZIPFOLDER"/Providers/SHP
   svn export "$FDOSVNROOT"/Providers/SHP "$FDOTARZIPFOLDER"/Providers/SHP --force
   find "$FDOTARZIPFOLDER" -name .svn | xargs rm -rf
   rm -f fdoshp-3.3.0_"$FDOBUILDNUMBER".tar
   tar -cf fdoshp-3.3.0_"$FDOBUILDNUMBER".tar "$FDOTARZIPFOLDER"
   rm -f fdoshp-3.3.0_"$FDOBUILDNUMBER".tar.gz
   gzip -9 fdoshp-3.3.0_"$FDOBUILDNUMBER".tar
   rm -rf "$FDOTARZIPFOLDER"
fi
if test "$SDFENABLECHK" == yes; then
   mkdir -p "$FDOTARZIPFOLDER"/Providers/SDF
   svn export "$FDOSVNROOT"/Providers/SDF "$FDOTARZIPFOLDER"/Providers/SDF --force
   find "$FDOTARZIPFOLDER" -name .svn | xargs rm -rf
   rm -f fdosdf-3.3.0_"$FDOBUILDNUMBER".tar
   tar -cf fdosdf-3.3.0_"$FDOBUILDNUMBER".tar "$FDOTARZIPFOLDER"
   rm -f fdosdf-3.3.0_"$FDOBUILDNUMBER".tar.gz
   gzip -9 fdosdf-3.3.0_"$FDOBUILDNUMBER".tar
   rm -rf "$FDOTARZIPFOLDER"
fi
if test "$WFSENABLECHK" == yes; then
   mkdir -p "$FDOTARZIPFOLDER"/Providers/WFS
   svn export "$FDOSVNROOT"/Providers/WFS "$FDOTARZIPFOLDER"/Providers/WFS --force
   find "$FDOTARZIPFOLDER" -name .svn | xargs rm -rf
   rm -f fdowfs-3.3.0_"$FDOBUILDNUMBER".tar
   tar -cf fdowfs-3.3.0_"$FDOBUILDNUMBER".tar "$FDOTARZIPFOLDER"
   rm -f fdowfs-3.3.0_"$FDOBUILDNUMBER".tar.gz
   gzip -9 fdowfs-3.3.0_"$FDOBUILDNUMBER".tar
   rm -rf "$FDOTARZIPFOLDER"
fi
if test "$WMSENABLECHK" == yes; then
   mkdir -p "$FDOTARZIPFOLDER"/Providers/WMS
   svn export "$FDOSVNROOT"/Providers/WMS "$FDOTARZIPFOLDER"/Providers/WMS --force
   find "$FDOTARZIPFOLDER" -name .svn | xargs rm -rf
   rm -f fdowms-3.3.0_"$FDOBUILDNUMBER".tar
   tar -cf fdowms-3.3.0_"$FDOBUILDNUMBER".tar "$FDOTARZIPFOLDER"
   rm -f fdowms-3.3.0_"$FDOBUILDNUMBER".tar.gz
   gzip -9 fdowms-3.3.0_"$FDOBUILDNUMBER".tar
   rm -rf "$FDOTARZIPFOLDER"
fi
if test "$ARCENABLECHK" == yes; then
   mkdir -p "$FDOTARZIPFOLDER"/Providers/ArcSDE
   svn export "$FDOSVNROOT"/Providers/ArcSDE "$FDOTARZIPFOLDER"/Providers/ArcSDE --force
   find "$FDOTARZIPFOLDER" -name .svn | xargs rm -rf
   rm -f fdoarcsde-3.3.0_"$FDOBUILDNUMBER".tar
   tar -cf fdoarcsde-3.3.0_"$FDOBUILDNUMBER".tar "$FDOTARZIPFOLDER"
   rm -f fdoarcsde-3.3.0_"$FDOBUILDNUMBER".tar.gz
   gzip -9 fdoarcsde-3.3.0_"$FDOBUILDNUMBER".tar
   rm -rf "$FDOTARZIPFOLDER"
fi
if test "$RDBMSENABLECHK" == yes; then
   mkdir -p "$FDOTARZIPFOLDER"/Providers/GenericRdbms
   svn export "$FDOSVNROOT"/Providers/GenericRdbms "$FDOTARZIPFOLDER"/Providers/GenericRdbms --force
   find "$FDOTARZIPFOLDER" -name .svn | xargs rm -rf
   rm -f fdordbms-3.3.0_"$FDOBUILDNUMBER".tar
   tar -cf fdordbms-3.3.0_"$FDOBUILDNUMBER".tar "$FDOTARZIPFOLDER"
   rm -f fdordbms-3.3.0_"$FDOBUILDNUMBER".tar.gz
   gzip -9 fdordbms-3.3.0_"$FDOBUILDNUMBER".tar
   rm -rf "$FDOTARZIPFOLDER"
fi
if test "$GDALENABLECHK" == yes; then
   mkdir -p "$FDOTARZIPFOLDER"/Providers/GDAL
   svn export "$FDOSVNROOT"/Providers/GDAL "$FDOTARZIPFOLDER"/Providers/GDAL --force
   find "$FDOTARZIPFOLDER" -name .svn | xargs rm -rf
   rm -f fdogdal-3.3.0_"$FDOBUILDNUMBER".tar
   tar -cf fdogdal-3.3.0_"$FDOBUILDNUMBER".tar "$FDOTARZIPFOLDER"
   rm -f fdogdal-3.3.0_"$FDOBUILDNUMBER".tar.gz
   gzip -9 fdogdal-3.3.0_"$FDOBUILDNUMBER".tar
   rm -rf "$FDOTARZIPFOLDER"
fi
if test "$KINGENABLECHK" == yes; then
   mkdir -p "$FDOTARZIPFOLDER"/Providers/KingOracle
   svn export "$FDOSVNROOT"/Providers/KingOracle "$FDOTARZIPFOLDER"/Providers/KingOracle --force
   find "$FDOTARZIPFOLDER" -name .svn | xargs rm -rf
   rm -f fdoking-3.3.0_"$FDOBUILDNUMBER".tar
   tar -cf fdoking-3.3.0_"$FDOBUILDNUMBER".tar "$FDOTARZIPFOLDER"
   rm -f fdoking-3.3.0_"$FDOBUILDNUMBER".tar.gz
   gzip -9 fdoking-3.3.0_"$FDOBUILDNUMBER".tar
   rm -rf "$FDOTARZIPFOLDER"
fi
if test "$OGRENABLECHK" == yes; then
   mkdir -p "$FDOTARZIPFOLDER"/Providers/OGR
   svn export "$FDOSVNROOT"/Providers/OGR "$FDOTARZIPFOLDER"/Providers/OGR --force
   find "$FDOTARZIPFOLDER" -name .svn | xargs rm -rf
   rm -f fdoogr-3.3.0_"$FDOBUILDNUMBER".tar
   tar -cf fdoogr-3.3.0_"$FDOBUILDNUMBER".tar "$FDOTARZIPFOLDER"
   rm -f fdoogr-3.3.0_"$FDOBUILDNUMBER".tar.gz
   gzip -9 fdoogr-3.3.0_"$FDOBUILDNUMBER".tar
   rm -rf "$FDOTARZIPFOLDER"
fi

echo "End creation of tar files"

exit 0
