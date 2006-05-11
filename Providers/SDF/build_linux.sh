
TYPEACTION=buildinstall
TYPEBUILD=release

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
    else
        echo "Invalid parameter"
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
    echo "The command is not recognized."
    echo "Please use the format:"
    SHOWHELP=yes
    break
    ;;
  esac
done
### end of study parameters ###

if test "$SHOWHELP" == yes; then
   echo "**************************************************************************"
   echo "build_linux.sh [--h] [-c=BuildType] [-a=Action] "
   echo "*"
   echo "Help:           --h[elp]"
   echo "BuildType:      --c[onfig] release(default), debug"
   echo "Action:         --a[ction] buildinstall(default), buildonly, installonly"
   echo "**************************************************************************"
   exit 0
fi

### start build ###
if test "$TYPEACTION" == buildinstall && test "$TYPEACTION" == buildonly ; then
   aclocal
   libtoolize --force
   automake --add-missing --copy
   autoconf

   if test "$TYPEBUILD" == release; then
      ./configure
   else
      ./configure --enable-debug=yes
   fi
   
   make
fi

if test "$TYPEACTION" == buildinstall && test "$TYPEACTION" == installonly ; then
   make install
fi

exit 0

