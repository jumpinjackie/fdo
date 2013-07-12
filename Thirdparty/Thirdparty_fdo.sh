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

##########################################
## build third party libraries for fdo: ##
## apache, linux/cppunit                ##
##########################################


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
   echo "Thirdparty_fdo.sh   [--h]"
   echo "                    [--a Action]"
   echo "                    [--b BuildArchicture]"
   echo " "
   echo "Help:                  --h[elp]"
   echo "Action:                --a[ction] buildinstall(default), build, install, uninstall, clean"
   echo "ConfigArchitecture:    --b[uild] 32(default), 64"
   echo "*******************************************************************"

   exit 0
fi

CMDEX="-b $TYPEARCHITECTURE -a $TYPEACTION"

## mkcatdefs
pushd . >& /dev/null
cd linux/mkcatdefs
echo Building mkcatdefs
chmod a+x ./build.sh
sh ./build.sh $CMDEX
popd >& /dev/null


## Apache Xalan/Xerces
pushd . >& /dev/null
cd apache
echo Building apche
chmod a+x ./build2.sh
sh ./build2.sh $CMDEX
popd >& /dev/null


## CPPUnit
pushd . >& /dev/null
cd linux/cppunit
echo Building linux/cppunit
chmod a+x ./build.sh
sh ./build.sh $CMDEX
popd >& /dev/null


## GDAL
pushd . >& /dev/null
cd gdal
echo Building gdal
chmod a+x ./build.sh
sh ./build.sh $CMDEX
popd >& /dev/null



