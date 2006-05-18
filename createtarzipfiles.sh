#!/bin/bash

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
  --h | --help)
    SHOWHELP=yes
    break
    ;;
  --b | --build)
     if test -z	 "$1"; then
        echo "build number cannot be empty"
	exit 1
     else 
        FDOBUILDNUMBER="$1"
     fi
     shift
    ;;
  --o | --outpath)
     if test -z	 "$1"; then
        echo "Destination folder can not be empty"
	exit 1
     else 
        FDOTARZIPFOLDER="$1"
     fi
     shift
    ;;
  --i | --inpath)
     if test -z	 "$1"; then
        echo "Destination folder can not be empty"
	exit 1
     else 
        FDOSVNROOT="$1"
     fi
     shift
    ;;
  --w | --with)
     if test "$DEFMODIFYCHK" == no; then
	DEFMODIFYCHK=yes
	FDOCOREENABLECHK=no
	SHPENABLECHK=no
	SDFENABLECHK=no
	WFSENABLECHK=no
	WMSENABLECHK=no
	ARCENABLECHK=no
	RDBMSENABLECHK=no
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
     elif test "$1" == all; then
	FDOCOREENABLECHK=yes
	SHPENABLECHK=yes
	SDFENABLECHK=yes
	WFSENABLECHK=yes
	WMSENABLECHK=yes
	ARCENABLECHK=yes
	RDBMSENABLECHK=yes
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
   echo "createtarzipfiles.sh [--h] [--i InFolder] [--o OutFolder] [--w WithModule] [--b BuildNumber]"
   echo " "
   echo "Help:           --h[elp]"
   echo "InFolder:       --i[npath]=input source svn checkout folder"
   echo "OutFolder:      --o[utpath]=destination folder for exported svn files"
   echo "WithModule:     --w[ith]=all(default), fdo, providers, shp, sdf, wfs, wms, arcsde, rdbms"
   echo "BuildNumber:    --b[uild]=User-Defined build number appended to the end of the tar.gz files"
   echo "**************************************************************************"
   exit 0
fi

if test "$FDOCOREENABLECHK" == yes; then
   mkdir -p "$FDOTARZIPFOLDER"
   svn export "$FDOSVNROOT" "$FDOTARZIPFOLDER" --non-recursive --force
   svn export "$FDOSVNROOT"/Providers "$FDOTARZIPFOLDER"/Providers --non-recursive --force
   svn export "$FDOSVNROOT"/Fdo "$FDOTARZIPFOLDER"/Fdo --force
   svn export "$FDOSVNROOT"/Utilities "$FDOTARZIPFOLDER"/Utilities --force
   svn export "$FDOSVNROOT"/Thirdparty "$FDOTARZIPFOLDER"/Thirdparty --force
   find "$FDOTARZIPFOLDER" -name .svn | xargs rm -rf
   rm -f fdo-3.2.0_"$FDOBUILDNUMBER".tar
   tar -cvf fdo-3.2.0_"$FDOBUILDNUMBER".tar "$FDOTARZIPFOLDER"
   rm -f fdo-3.2.0_"$FDOBUILDNUMBER".tar.gz
   gzip -9 fdo-3.2.0_"$FDOBUILDNUMBER".tar
   rm -rf "$FDOTARZIPFOLDER"
fi
if test "$SHPENABLECHK" == yes; then
   mkdir -p "$FDOTARZIPFOLDER"/Providers/SHP
   svn export "$FDOSVNROOT"/Providers/SHP "$FDOTARZIPFOLDER"/Providers/SHP --force
   find "$FDOTARZIPFOLDER" -name .svn | xargs rm -rf
   rm -f fdoshp-3.2.0_"$FDOBUILDNUMBER".tar
   tar -cvf fdoshp-3.2.0_"$FDOBUILDNUMBER".tar "$FDOTARZIPFOLDER"
   rm -f fdoshp-3.2.0_"$FDOBUILDNUMBER".tar.gz
   gzip -9 fdoshp-3.2.0_"$FDOBUILDNUMBER".tar
   rm -rf "$FDOTARZIPFOLDER"
fi
if test "$SDFENABLECHK" == yes; then
   mkdir -p "$FDOTARZIPFOLDER"/Providers/SDF
   svn export "$FDOSVNROOT"/Providers/SDF "$FDOTARZIPFOLDER"/Providers/SDF --force
   find "$FDOTARZIPFOLDER" -name .svn | xargs rm -rf
   rm -f fdosdf-3.2.0_"$FDOBUILDNUMBER".tar
   tar -cvf fdosdf-3.2.0_"$FDOBUILDNUMBER".tar "$FDOTARZIPFOLDER"
   rm -f fdosdf-3.2.0_"$FDOBUILDNUMBER".tar.gz
   gzip -9 fdosdf-3.2.0_"$FDOBUILDNUMBER".tar
   rm -rf "$FDOTARZIPFOLDER"
fi
if test "$WFSENABLECHK" == yes; then
   mkdir -p "$FDOTARZIPFOLDER"/Providers/WFS
   svn export "$FDOSVNROOT"/Providers/WFS "$FDOTARZIPFOLDER"/Providers/WFS --force
   find "$FDOTARZIPFOLDER" -name .svn | xargs rm -rf
   rm -f fdowfs-3.2.0_"$FDOBUILDNUMBER".tar
   tar -cvf fdowfs-3.2.0_"$FDOBUILDNUMBER".tar "$FDOTARZIPFOLDER"
   rm -f fdowfs-3.2.0_"$FDOBUILDNUMBER".tar.gz
   gzip -9 fdowfs-3.2.0_"$FDOBUILDNUMBER".tar
   rm -rf "$FDOTARZIPFOLDER"
fi
if test "$WMSENABLECHK" == yes; then
   mkdir -p "$FDOTARZIPFOLDER"/Providers/WMS
   svn export "$FDOSVNROOT"/Providers/WMS "$FDOTARZIPFOLDER"/Providers/WMS --force
   find "$FDOTARZIPFOLDER" -name .svn | xargs rm -rf
   rm -f fdowms-3.2.0_"$FDOBUILDNUMBER".tar
   tar -cvf fdowms-3.2.0_"$FDOBUILDNUMBER".tar "$FDOTARZIPFOLDER"
   rm -f fdowms-3.2.0_"$FDOBUILDNUMBER".tar.gz
   gzip -9 fdowms-3.2.0_"$FDOBUILDNUMBER".tar
   rm -rf "$FDOTARZIPFOLDER"
fi
if test "$ARCENABLECHK" == yes; then
   mkdir -p "$FDOTARZIPFOLDER"/Providers/ArcSDE
   svn export "$FDOSVNROOT"/Providers/ArcSDE "$FDOTARZIPFOLDER"/Providers/ArcSDE --force
   find "$FDOTARZIPFOLDER" -name .svn | xargs rm -rf
   rm -f fdoarcsde-3.2.0_"$FDOBUILDNUMBER".tar
   tar -cvf fdoarcsde-3.2.0_"$FDOBUILDNUMBER".tar "$FDOTARZIPFOLDER"
   rm -f fdoarcsde-3.2.0_"$FDOBUILDNUMBER".tar.gz
   gzip -9 fdoarcsde-3.2.0_"$FDOBUILDNUMBER".tar
   rm -rf "$FDOTARZIPFOLDER"
fi
if test "$RDBMSENABLECHK" == yes; then
   mkdir -p "$FDOTARZIPFOLDER"/Providers/GenericRdbms
   svn export "$FDOSVNROOT"/Providers/GenericRdbms "$FDOTARZIPFOLDER"/Providers/GenericRdbms --force
   find "$FDOTARZIPFOLDER" -name .svn | xargs rm -rf
   rm -f fdordbms-3.2.0_"$FDOBUILDNUMBER".tar
   tar -cvf fdordbms-3.2.0_"$FDOBUILDNUMBER".tar "$FDOTARZIPFOLDER"
   rm -f fdordbms-3.2.0_"$FDOBUILDNUMBER".tar.gz
   gzip -9 fdordbms-3.2.0_"$FDOBUILDNUMBER".tar
   rm -rf "$FDOTARZIPFOLDER"
fi

echo "End creation of tar files"

exit 0
