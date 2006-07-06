#!/bin/sh

TYPEACTION=buildinstall
TYPECONFIGURE=configure

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

   echo "************************************************************************************************************"
   echo "build_thirdparty.sh [--h] [--a Action]"
   echo "*"
   echo "Help:            --h[elp]"
   echo "Action:          --a[ction] buildinstall(default), build, install, uninstall, clean"
   echo "ConfigMakefiles: --m[akefile] configure(default), noconfigure"
   echo "************************************************************************************************************"

   exit 0
fi

if test "$TYPECONFIGURE" == configure ; then
  echo "Configuring FDO Makefiles"
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

if test ! -e "Thirdparty/Thirdparty.sh"; then
  echo "Thirdparty/Thirdparty.sh is missing!"
  exit 1;
fi

pushd "Thirdparty" >& /dev/null

if test "$TYPEACTION" == clean ; then
  make clean
fi
if test "$TYPEACTION" == buildinstall || test "$TYPEACTION" == build ; then
  ./Thirdparty.sh
fi
if test "$TYPEACTION" == buildinstall || test "$TYPEACTION" == install ; then
  make install
fi
if test "$TYPEACTION" == uninstall ; then
  make uninstall
fi

popd >& /dev/null

exit 0

