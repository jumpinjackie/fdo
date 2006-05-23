#!/bin/sh

TYPEACTION=buildinstall
TYPEBUILD=release
TYPECONFIGURE=no
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
    elif test "$1" == buildonly; then
        TYPEACTION=buildonly
    elif test "$1" == installonly; then
        TYPEACTION=installonly
    elif test "$1" == clean; then
        TYPEACTION=clean
    elif test "$1" == configure; then
        TYPECONFIGURE=yes
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
   echo "**************************************************************************"***********
   echo "build_linux.sh [--h] [-c BuildType] [-a Action] [--d BuildDocs]"
   echo "*"
   echo "Help:           --h[elp]"
   echo "BuildType:      --c[onfig] release(default), debug"
   echo "Action:         --a[ction] buildinstall(default), buildonly, installonly, clean, configure"
   echo "BuildDocs:      --d[ocs] skip(default), build"
   echo "**************************************************************************"***********
   exit 0
fi

### start build ###
if test "$TYPECONFIGURE" == yes ; then
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

if test "$TYPEACTION" == buildinstall || test "$TYPEACTION" == buildonly ; then
   make
fi

if test "$TYPEACTION" == buildinstall || test "$TYPEACTION" == installonly ; then
   make install
fi

if test "$BUILDDOCS" == yes ; then
   echo Creating ArcSDE provider html documentation
   rm -rf ../Docs/HTML/Providers/ArcSDE
   mkdir -p ../Docs/HTML/Providers/ArcSDE

   pushd Docs/doc_src >& /dev/null
   doxygen Doxyfile_ArcSDE >& /dev/null
   popd >& /dev/null   
fi

if test "$TYPEACTION" == buildinstall || test "$TYPEACTION" == installonly ; then
   rm -rf "/usr/local/fdo-3.2.0/docs/HTML/Providers/ArcSDE"
   mkdir -p "/usr/local/fdo-3.2.0/docs/HTML/Providers"
   if test -e "../Docs/HTML/Providers/ArcSDE"; then
      cp --force --recursive "../Docs/HTML/Providers/ArcSDE" "/usr/local/fdo-3.2.0/docs/HTML/Providers"
   fi
fi

exit 0

