#!/bin/sh

check_for_errors()
{
  # $ 1 is the return code
  # $ 2 is text to display on failure.
  if [ "${1}" -ne "0" ]; then
    echo "ERROR # ${1} : ${2}"
    if [ "${3}" -ne "0" ]; then
      # make our script exit with the right error code.
      exit ${1}
    fi
  fi
}

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

SHOWHELP=no
FDOTARZIPFOLDERRTB=OpenSource_FDO
FDOTARGETFOLDER=/home/OpenSource
FDOBUILDNUMBERRTB=GXXX
TIMECOUNT=0

if test -z "$FDOSOURCETARGZ"; then
   FDOSOURCETARGZ=$PWD
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
        echo "Build number cannot be empty"
	exit 1
     else 
        FDOBUILDNUMBERRTB="$1"
     fi
     shift
    ;;
  -o | --o | --outpath)
     if test -z	 "$1"; then
        echo "Destination build folder can not be empty"
	exit 1
     else 
        FDOTARGETFOLDER="$1"
     fi
     shift
    ;;
  -i | --i | --inpath)
     if test -z	 "$1"; then
        echo "Destination folder can not be empty"
	exit 1
     else 
        FDOSOURCETARGZ="$1"
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
   echo "runtestbuild.sh [--h] [--i InFolder] [--o OutFolder] [--b BuildNumber]"
   echo " "
   echo "Help:           --h[elp]"
   echo "InFolder:       --i[npath]=input source svn checkout folder"
   echo "OutFolder:      --o[utpath]=destination build test folder, default (/home/OpenSource)"
   echo "BuildNumber:    --b[uild]=User-Defined build number appended to the end of the tar.gz files"
   echo "**************************************************************************"
   exit 0
fi
   if test ! -e "$SDEHOME"; then
      if test -e "/home/ESRI/ArcSDEClient91/Linux"; then
        export SDEHOME=/home/ESRI/ArcSDEClient91/Linux
      else
        echo "WARNING: Invalid path for OPTIONAL ArcSDE SDK files. Optionally set environment variable SDEHOME with a valid path."
      fi
   fi
   if test ! -e "$FDOMYSQL"; then
      if test -e "/home/mysql/rhlinux"; then
        export FDOMYSQL=/home/mysql/rhlinux
      else
        echo "WARNING: Invalid path for OPTIONAL MySQL SDK files. Optionally set environment variable FDOMYSQL with a valid path."
      fi
   fi
   if test ! -e "$FDOODBC"; then
      if test -e "/home/unixODBC"; then
        export FDOODBC=/home/unixODBC
      else
        echo "WARNING: Invalid path for OPTIONAL ODBC SDK files. Optionally set environment FDOODBC with a valid path."
      fi
   fi
   if test "$FDOBUILDNUMBERRTB" == GXXX; then
      while test ! -e "$FDOSOURCETARGZ/release.txt";  do
         echo "Release not ready yet. Waiting..."
         sleep 5m
         TIMECOUNT=`expr $TIMECOUNT + 1`
         if test $TIMECOUNT -eq 48; then
            echo "Error: time out waiting for the release..."
            exit 1
         fi
      done
      FDOBUILDNUMBERRTB=`awk -F: '{print $m}' $FDOSOURCETARGZ/release.txt`
   fi
   rm -rf "/usr/local/fdo-3.2.0"
   rm -rf "$FDOTARGETFOLDER"

   mkdir -p "$FDOTARGETFOLDER"
   echo "*start copy zip files"
   cp --force "$FDOSOURCETARGZ/fdo-3.2.0_$FDOBUILDNUMBERRTB.tar.gz" "$FDOTARGETFOLDER"
   cp --force "$FDOSOURCETARGZ/fdoshp-3.2.0_$FDOBUILDNUMBERRTB.tar.gz" "$FDOTARGETFOLDER"
   cp --force "$FDOSOURCETARGZ/fdosdf-3.2.0_$FDOBUILDNUMBERRTB.tar.gz" "$FDOTARGETFOLDER"
   cp --force "$FDOSOURCETARGZ/fdowfs-3.2.0_$FDOBUILDNUMBERRTB.tar.gz" "$FDOTARGETFOLDER"
   cp --force "$FDOSOURCETARGZ/fdowms-3.2.0_$FDOBUILDNUMBERRTB.tar.gz" "$FDOTARGETFOLDER"
   cp --force "$FDOSOURCETARGZ/fdoarcsde-3.2.0_$FDOBUILDNUMBERRTB.tar.gz" "$FDOTARGETFOLDER"
   cp --force "$FDOSOURCETARGZ/fdordbms-3.2.0_$FDOBUILDNUMBERRTB.tar.gz" "$FDOTARGETFOLDER"
   cp --force "$FDOSOURCETARGZ/fdogdal-3.2.0_$FDOBUILDNUMBERRTB.tar.gz" "$FDOTARGETFOLDER"
   echo "copy zip files done..."

   echo "*start unzip files"
   cd "$FDOTARGETFOLDER"
   gzip -dN "fdo-3.2.0_$FDOBUILDNUMBERRTB.tar.gz"
   gzip -dN "fdoshp-3.2.0_$FDOBUILDNUMBERRTB.tar.gz"
   gzip -dN "fdosdf-3.2.0_$FDOBUILDNUMBERRTB.tar.gz"
   gzip -dN "fdowfs-3.2.0_$FDOBUILDNUMBERRTB.tar.gz"
   gzip -dN "fdowms-3.2.0_$FDOBUILDNUMBERRTB.tar.gz"
   gzip -dN "fdoarcsde-3.2.0_$FDOBUILDNUMBERRTB.tar.gz"
   gzip -dN "fdordbms-3.2.0_$FDOBUILDNUMBERRTB.tar.gz"
   gzip -dN "fdogdal-3.2.0_$FDOBUILDNUMBERRTB.tar.gz"

   tar -xf "fdo-3.2.0_$FDOBUILDNUMBERRTB.tar"
   tar -xf "fdoshp-3.2.0_$FDOBUILDNUMBERRTB.tar"
   tar -xf "fdosdf-3.2.0_$FDOBUILDNUMBERRTB.tar"
   tar -xf "fdowfs-3.2.0_$FDOBUILDNUMBERRTB.tar"
   tar -xf "fdowms-3.2.0_$FDOBUILDNUMBERRTB.tar"
   tar -xf "fdoarcsde-3.2.0_$FDOBUILDNUMBERRTB.tar"
   tar -xf "fdordbms-3.2.0_$FDOBUILDNUMBERRTB.tar"
   tar -xf "fdogdal-3.2.0_$FDOBUILDNUMBERRTB.tar"
   popd >& /dev/null
   echo "unzip files done..."
   
   pushd "$FDOTARGETFOLDER/$FDOTARZIPFOLDERRTB" >& /dev/null
   export FDO="$PWD/Fdo"
   export FDOUTILITIES="$PWD/Utilities"
   export FDOTHIRDPARTY="$PWD/Thirdparty"
   mkdir -p "/usr/local/fdo-3.2.0/lib"
   export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/fdo-3.2.0/lib:$SDEHOME/lib
   popd >& /dev/null
   
   echo "*Starting thirdparty build"
   pushd "$FDOTARGETFOLDER/$FDOTARZIPFOLDERRTB" >& /dev/null
   ./build_thirdparty.sh >& thirdparty_build_log.txt
   check_for_errors $? "Thirdparty build returned an error, please check thirdparty_build_log.txt for more information" "1"
   echo "*Starting fdocore release build"
   ./build_linux.sh --c release --d build >& fdocore_build_log.txt
   check_for_errors $? "Fdocore build returned an error, please check fdocore_build_log.txt for more information" "1"
   echo "End release build"
   
   echo "*Starting unit tests"
   pushd Fdo/UnitTest >& /dev/null
   ./UnitTest >& ../../Fdo_unit_test_log.txt
   check_for_errors $? "Fdo unit test returned an error, please check Fdo_unit_test_log.txt for more information" "0"
   popd >& /dev/null
   
   pushd Providers/SHP/Src/UnitTest >& /dev/null
   ./UnitTest >& ../../../../Shp_unit_test_log.txt
   check_for_errors $? "Shp unit test returned an error, please check Shp_unit_test_log.txt for more information" "0"
   popd >& /dev/null
   
   pushd Providers/SDF/Src/UnitTest >& /dev/null
   ./UnitTest >& ../../../../Sdf_unit_test_log.txt
   check_for_errors $? "Sdf unit test returned an error, please check Sdf_unit_test_log.txt for more information" "0"
   popd >& /dev/null
   
   pushd Providers/WMS/Src/UnitTest >& /dev/null
   ./UnitTest >& ../../../../Wms_unit_test_log.txt
   check_for_errors $? "Wms unit test returned an error, please check Wms_unit_test_log.txt for more information" "0"
   popd >& /dev/null
   
   pushd Providers/ArcSDE/Src/UnitTest >& /dev/null
   ./UnitTest server=adfdo.dnsalias.com port_multi=5151/tcp port_single=5151/tcp sdepwd=system dataset=FC4 rdbms=ORACLE >& ../../../../ArcSDE_unit_test_log.txt
   check_for_errors $? "ArcSDE unit test returned an error, please check ArcSDE_unit_test_log.txt for more information" "0"
   popd >& /dev/null
   
   pushd Providers/GDAL/Src/UnitTest >& /dev/null
   ./UnitTest >& ../../../../GDAL_unit_test_log.txt
   check_for_errors $? "GDAL unit test returned an error, please check GDAL_unit_test_log.txt for more information" "0"
   popd >& /dev/null

   popd >& /dev/null
   echo "End unit tests..."
exit 0
