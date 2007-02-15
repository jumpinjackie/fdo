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
TYPECONFIGURE=configure
BUILDDOCS=no

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
   echo "build_linux.sh [--h] [--c BuildType] [--a Action] [--d BuildDocs] [--m ConfigMakefiles]"
   echo "*"
   echo "Help:            --h[elp]"
   echo "BuildType:       --c[onfig] release(default), debug"
   echo "Action:          --a[ction] buildinstall(default), build, install, uninstall, clean"
   echo "BuildDocs:       --d[ocs] skip(default), build"
   echo "ConfigMakefiles: --m[akefile] configure(default), noconfigure"
   echo "************************************************************************************************************"
   exit 0
fi

### start build ###
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
   
if test "$TYPEACTION" == clean ; then
  make clean
fi

if test "$TYPEACTION" == buildinstall || test "$TYPEACTION" == build ; then
   make
fi

if test "$TYPEACTION" == buildinstall || test "$TYPEACTION" == install ; then
   make install
fi

if test "$TYPEACTION" == uninstall ; then
   make uninstall
fi

if test "$BUILDDOCS" == yes ; then
   rm -rf Docs/HTML/MySQL
   rm -rf Docs/HTML/ODBC
   mkdir -p Docs/HTML/MySQL
   mkdir -p Docs/HTML/ODBC

   echo Creating MySQL Provider HTML documentation
   pushd Docs/doc_src >& /dev/null
   doxygen Doxyfile_MySQL >& /dev/null
   doxygen Doxyfile_MySQL_managed >& /dev/null
   popd >& /dev/null
   
   echo Creating ODBC Provider HTML documentation
   pushd Docs/doc_src >& /dev/null
   doxygen Doxyfile_ODBC >& /dev/null
   doxygen Doxyfile_ODBC_managed >& /dev/null
   popd >& /dev/null
fi

if test "$TYPEACTION" == buildinstall || test "$TYPEACTION" == install ; then
   rm -rf "/usr/local/fdo-3.3.0/docs/HTML/Providers/MySQL"
   rm -rf "/usr/local/fdo-3.3.0/docs/HTML/Providers/ODBC"
   mkdir -p "/usr/local/fdo-3.3.0/docs/HTML/Providers"
   if test -e "Docs/HTML/MySQL"; then
      cp --force --recursive "Docs/HTML/MySQL" "/usr/local/fdo-3.3.0/docs/HTML/Providers"
      cp --force --recursive "Docs/HTML/MySQL_managed" "/usr/local/fdo-3.2.0/docs/HTML/Providers"
   fi
   if test -e "Docs/HTML/ODBC"; then
      cp --force --recursive "Docs/HTML/ODBC" "/usr/local/fdo-3.3.0/docs/HTML/Providers"
      cp --force --recursive "Docs/HTML/ODBC_managed" "/usr/local/fdo-3.2.0/docs/HTML/Providers"
   fi
fi

exit 0

