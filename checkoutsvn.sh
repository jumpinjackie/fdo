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
THRPENABLECHK=no
FDOENABLECHK=no
UTILENABLECHK=no
SHPENABLECHK=yes
SDFENABLECHK=yes
WFSENABLECHK=yes
WMSENABLECHK=yes
ARCENABLECHK=yes
RDBMSENABLECHK=yes
SHOWHELP=no

if test -z "$FDO_SVN_ROOT"; then
   FDO_SVN_ROOT=$PWD
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
  --o | --outpath)
     if test -z	 "$1"; then
        echo "Destination folder can not be empty"
	exit 1
     else 
        FDO_SVN_ROOT="$1"
     fi
     shift
    ;;
  --u | --user)
     if test -z	 "$1"; then
        echo "User id can not be empty"
	exit 1
     else 
        FDO_SVN_USERNAME="$1"
     fi
     shift
    ;;
  --p | --password)
     if test -z	 "$1"; then
        echo "Password can not be empty"
	exit 1
     else 
        FDO_SVN_PASSWORD="$1"
     fi
     shift
    ;;
  --w | --with)
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
	THRPENABLECHK=no
	FDOENABLECHK=no
	UTILENABLECHK=no
	SHPENABLECHK=yes
	SDFENABLECHK=yes
	WFSENABLECHK=yes
	WMSENABLECHK=yes
	ARCENABLECHK=yes
	RDBMSENABLECHK=yes
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
   echo "checkoutsvn.sh [--h] [--o OutFolder] [--w WithModule] [--u UserId] [--p UserPassword]"
   echo "*"
   echo "Help:           --h[elp]"
   echo "OutFolder:      --o[utpath]=destination folder for files"
   echo "WithModule:     --w[ith]=all(default), fdo, thirdparty, providers, utilities, shp, sdf, wfs, wms, arcsde, rdbms"
   echo "User:           --u[ser]=user id"
   echo "Password:       --p[assword]=user password"
   echo "**************************************************************************"
   exit 0
fi

if test ! -e "$FDO_SVN_ROOT"; then
   mkdir "$FDO_SVN_ROOT"
fi

if test "$FDOCOREENABLECHK" == no && test "$FDOENABLECHK" == yes; then
   svn checkout https://fdocore.osgeo.org/svn/fdocore/trunk/Fdo "$FDO_SVN_ROOT/Fdo" --username "$FDO_SVN_USERNAME" --password "$FDO_SVN_PASSWORD"
fi
if test "$FDOCOREENABLECHK" == no && test "$THRPENABLECHK" == yes; then
   svn checkout https://fdocore.osgeo.org/svn/fdocore/trunk/Thirdparty "$FDO_SVN_ROOT/Thirdparty" --username "$FDO_SVN_USERNAME" --password "$FDO_SVN_PASSWORD"
fi
if test "$FDOCOREENABLECHK" == no && test "$UTILENABLECHK" == yes; then
   svn checkout https://fdocore.osgeo.org/svn/fdocore/trunk/Utilities "$FDO_SVN_ROOT/Utilities" --username "$FDO_SVN_USERNAME" --password "$FDO_SVN_PASSWORD"
fi
if test "$FDOCOREENABLECHK" == yes; then
   svn checkout https://fdocore.osgeo.org/svn/fdocore/trunk "$FDO_SVN_ROOT" --username "$FDO_SVN_USERNAME" --password "$FDO_SVN_PASSWORD"
fi
if test "$SHPENABLECHK" == yes; then
   svn checkout https://fdoshp.osgeo.org/svn/fdoshp/trunk/Providers/SHP "$FDO_SVN_ROOT/Providers/SHP" --username "$FDO_SVN_USERNAME" --password "$FDO_SVN_PASSWORD"
fi
if test "$SDFENABLECHK" == yes; then
   svn checkout https://fdosdf.osgeo.org/svn/fdosdf/trunk/Providers/SDF "$FDO_SVN_ROOT/Providers/SDF" --username "$FDO_SVN_USERNAME" --password "$FDO_SVN_PASSWORD"
fi
if test "$WFSENABLECHK" == yes; then
   svn checkout https://fdowfs.osgeo.org/svn/fdowfs/trunk/Providers/WFS "$FDO_SVN_ROOT/Providers/WFS" --username "$FDO_SVN_USERNAME" --password "$FDO_SVN_PASSWORD"
fi
if test "$WMSENABLECHK" == yes; then
   svn checkout https://fdowms.osgeo.org/svn/fdowms/trunk/Providers/WMS "$FDO_SVN_ROOT/Providers/WMS" --username "$FDO_SVN_USERNAME" --password "$FDO_SVN_PASSWORD"
fi
if test "$ARCENABLECHK" == yes; then
   svn checkout https://fdoarcsde.osgeo.org/svn/fdoarcsde/trunk/Providers/ArcSDE "$FDO_SVN_ROOT/Providers/ArcSDE" --username "$FDO_SVN_USERNAME" --password "$FDO_SVN_PASSWORD"
fi
if test "$RDBMSENABLECHK" == yes; then
   svn checkout https://fdordbms.osgeo.org/svn/fdordbms/trunk/Providers/GenericRdbms "$FDO_SVN_ROOT/Providers/GenericRdbms" --username "$FDO_SVN_USERNAME" --password "$FDO_SVN_PASSWORD"
fi
echo "End checkout"

exit 0

