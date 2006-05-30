#!/bin/sh

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
  --h | --help)
    SHOWHELP=yes
    break
    ;;
  --a | --action)
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
  --m | --makefile)
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
  --d | --docs)
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
  --c | --config)
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
   echo "build_linux.sh [--h] [--c BuildType] [--a Action] [--w WithModule] [--d BuildDocs] [--m ConfigMakefiles]"
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
   echo Creating Rdbms providers html documentation
   rm -rf ../Docs/HTML/Providers/MySQL
   rm -rf ../Docs/HTML/Providers/ODBC
   mkdir -p ../Docs/HTML/Providers/MySQL
   mkdir -p ../Docs/HTML/Providers/ODBC

   pushd Docs/doc_src >& /dev/null
   doxygen Doxyfile_MySQL >& /dev/null
   popd >& /dev/null
   
   pushd Docs/doc_src >& /dev/null
   doxygen Doxyfile_ODBC >& /dev/null
   popd >& /dev/null
fi

if test "$TYPEACTION" == buildinstall || test "$TYPEACTION" == install ; then
   rm -rf "/usr/local/fdo-3.2.0/docs/HTML/Providers/MySQL"
   rm -rf "/usr/local/fdo-3.2.0/docs/HTML/Providers/ODBC"
   mkdir -p "/usr/local/fdo-3.2.0/docs/HTML/Providers"
   if test -e "../Docs/HTML/Providers/MySQL"; then
      cp --force --recursive "../Docs/HTML/Providers/MySQL" "/usr/local/fdo-3.2.0/docs/HTML/Providers"
   fi
   if test -e "../Docs/HTML/Providers/ODBC"; then
      cp --force --recursive "../Docs/HTML/Providers/ODBC" "/usr/local/fdo-3.2.0/docs/HTML/Providers"
   fi
fi

exit 0

