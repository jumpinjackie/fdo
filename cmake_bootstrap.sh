#!/bin/sh
BUILD_CPU=32
BUILD_CONFIG=Release
INTERNAL_CPPUNIT=FALSE
INTERNAL_GDAL=FALSE
INTERNAL_OPENSSL=FALSE
INTERNAL_MYSQL=FALSE
INTERNAL_POSTGRESQL=FALSE
INTERNAL_BOOST=FALSE
INTERNAL_CURL=FALSE
INTERNAL_XERCESC=FALSE
INTERNAL_XALANC=FALSE

while [ $# -gt 0 ]; do    # Until you run out of parameters...
    case "$1" in
        --working_dir)
            THIRDPARTY_WORK_DIR="$2"
            shift
            ;;
        --build)
            BUILD_CPU=$2
            shift
            ;;
        --config)
            BUILD_CONFIG=$2
            shift
            ;;
        --all-internal)
            INTERNAL_CPPUNIT=TRUE
            INTERNAL_GDAL=TRUE
            INTERNAL_OPENSSL=TRUE
            #INTERNAL_MYSQL=TRUE
            #INTERNAL_POSTGRESQL=TRUE
            INTERNAL_BOOST=TRUE
            INTERNAL_CURL=TRUE
            INTERNAL_XERCESC=TRUE
            INTERNAL_XALANC=TRUE
            ;;
        --internal-cppunit)
            INTERNAL_CPPUNIT=TRUE
            ;;
        --internal-gdal)
            INTERNAL_GDAL=TRUE
            ;;
        --internal-openssl)
            INTERNAL_OPENSSL=TRUE
            ;;
        #--internal-mysql)
        #    INTERNAL_MYSQL=TRUE
        #    ;;
        #--internal-postgresql)
        #    INTERNAL_POSTGRESQL=TRUE
        #    ;;
        --internal-boost)
            INTERNAL_BOOST=TRUE
            ;;
        --internal-curl)
            INTERNAL_CURL=TRUE
            ;;
        --internal-xerces)
            INTERNAL_XERCESC=TRUE
            ;;
        --internal-xalan)
            INTERNAL_XALANC=TRUE
            ;;
        --help)
            echo "Usage: $0 (options)"
            echo "Options:"
            echo "  --working_dir [build directory]"
            echo "  --build [32(default)|64]"
            echo "  --config [Release(default)|Debug]"
            echo "  --internal-cppunit [Use internal cppunit]"
            echo "  --internal-gdal [Use internal gdal]"
            echo "  --internal-openssl [Use internal openssl]"
            #echo "  --internal-mysql [Use internal mysql]"
            #echo "  --internal-postgresql [Use internal postgresql]"
            echo "  --internal-boost [Use internal boost]"
            echo "  --internal-curl [Use internal curl]"
            echo "  --internal-xerces [Use internal xerces]"
            echo "  --internal-xalan [Use internal xalan]"
            echo "  --all-internal [Use all internal thirdparty components]"
            echo "  --help [Display usage]"
            exit
            ;;
    esac
    shift   # Check next set of parameters.
done

SOURCE_DIR=`pwd`

check_build()
{
    error=$?
    if [ $error -ne 0 ]; then
        echo "$LIB_NAME: Error build failed ($error)................."
        # Return back to this dir before bailing
        cd $SOURCE_DIR
        exit $error
    fi
}

# Validate
if test "$BUILD_CPU" != "32" -a "$BUILD_CPU" != "64"; then
    echo "--build must be 32 or 64"
    exit 1;
fi
if test "$BUILD_CONFIG" != "Release" -a "$BUILD_CONFIG" != "Debug"; then
    echo "--config must be Release or Debug"
    exit 1;
fi
if test -z "$THIRDPARTY_WORK_DIR"; then
    echo "--working_dir must be specified"
    exit 1;
fi

echo "Current source dir is: $SOURCE_DIR"
echo "Building for: $BUILD_CPU-bit"
echo "Building in: $BUILD_CONFIG"
echo "Thirdparty Working Directory is: $THIRDPARTY_WORK_DIR"
echo "Internal cppunit: $INTERNAL_CPPUNIT"
echo "Internal gdal: $INTERNAL_GDAL"
echo "Internal openssl: $INTERNAL_OPENSSL"
echo "Internal mysql: $INTERNAL_MYSQL"
echo "Internal postgresql: $INTERNAL_POSTGRESQL"
echo "Internal boost: $INTERNAL_BOOST"
echo "Internal curl: $INTERNAL_CURL"
echo "Internal xerces: $INTERNAL_XERCESC"
echo "Internal xalan: $INTERNAL_XALANC"

echo "Creating thirdparty working dir if required"
if [ ! -d $THIRDPARTY_WORK_DIR ]; then
    mkdir -p $THIRDPARTY_WORK_DIR
    echo "Created: $THIRDPARTY_WORK_DIR"
fi

echo "Copying thirdparty sources to working dir"
cp -fR ${SOURCE_DIR}/Thirdparty $THIRDPARTY_WORK_DIR/Thirdparty

echo "Copying root files to working dir"
cp -f ${SOURCE_DIR}/setenvironment.sh $THIRDPARTY_WORK_DIR

cd $THIRDPARTY_WORK_DIR
# Patch setenvironment.sh before running it
cat setenvironment.sh \
    | sed -e "s#export FDO=\$PWD\/Fdo#export FDO=$SOURCE_DIR/Fdo#g" \
    | sed -e "s#export FDOUTILITIES=\$PWD\/Utilities#export FDOUTILITIES=$SOURCE_DIR/Utilities#g" \
    | sed -e "s#export FDOTHIRDPARTY=\$PWD\/Thirdparty#export FDOTHIRDPARTY=$THIRDPARTY_WORK_DIR/Thirdparty#g" \
    > setenvironment_patched.sh
source ./setenvironment_patched.sh --noinstall

# Patch various build scripts (to strip sudo)
if test -f "$THIRDPARTY_WORK_DIR/Thirdparty/boost/build.sh"; then
    sed -i "s#sudo ##g" $THIRDPARTY_WORK_DIR/Thirdparty/boost/build.sh
fi
if test -f "$THIRDPARTY_WORK_DIR/Thirdparty/apache/build2.sh"; then
    sed -i "s#sudo ##g" $THIRDPARTY_WORK_DIR/Thirdparty/apache/build2.sh
fi

CMDEX="-b $BUILD_CPU -a buildinstall"

if [[ "$CFLAGS" != *"-m$BUILD_CPU"* ]]; then
    CFLAGS="$CFLAGS -m$BUILD_CPU"
    echo "Exporting CFLAGS: "$CFLAGS""
    export CFLAGS
fi

if [[ "$CPPFLAGS" != *"-m$BUILD_CPU"* ]]; then
    CPPFLAGS="$CPPFLAGS -m$BUILD_CPU"
    echo "Exporting CPPFLAGS: "$CPPFLAGS""
    export CPPFLAGS
fi

if [[ "$LDFLAGS" != *"-m$BUILD_CPU"* ]]; then
    LDFLAGS="$LDFLAGS -m$BUILD_CPU"
    echo "Exporting LDFLAGS: "$LDFLAGS""
    export LDFLAGS
fi

if test "$BUILD_CPU" == "32" ; then
    if test "$HOSTTYPE" == "i686" ; then
        if [[ "$CPPFLAGS" != *"-march=i686"* ]]; then
            CPPFLAGS="$CPPFLAGS -march=i686"
            echo "Exporting CPPFLAGS: "$CPPFLAGS""
            export CPPFLAGS
        fi
    fi
fi

if [[ "$CPPFLAGS" != *"-Wno-write-strings"* ]]; then
    CPPFLAGS="$CPPFLAGS -Wno-write-strings"
    echo "Exporting CPPFLAGS: "$CPPFLAGS""
    export CPPFLAGS
fi

if [[ "$CPPFLAGS" != *"-Wno-deprecated"* ]]; then
    CPPFLAGS="$CPPFLAGS -Wno-deprecated"
    echo "Exporting CPPFLAGS: "$CPPFLAGS""
    export CPPFLAGS
fi

if test "$INTERNAL_XALANC" = "TRUE" -o "$INTERNAL_XERCESC" = "TRUE"; then
    LIB_NAME="xalan/xerces"
    echo "Building internal $LIB_NAME"
    cd $THIRDPARTY_WORK_DIR/Thirdparty/apache
    chmod a+x ./build2.sh
    sh ./build2.sh $CMDEX
    check_build
fi
if test "$INTERNAL_CPPUNIT" = "TRUE"; then
    LIB_NAME="cppunit"
    echo "Building internal $LIB_NAME"
    cd $THIRDPARTY_WORK_DIR/Thirdparty/linux/cppunit
    chmod a+x ./build.sh
    sh ./build.sh $CMDEX
    check_build
fi
if test "$INTERNAL_GDAL" = "TRUE"; then
    LIB_NAME="gdal"
    echo "Building internal $LIB_NAME"
    cd $THIRDPARTY_WORK_DIR/Thirdparty/gdal
    chmod a+x ./build.sh
    sh ./build.sh $CMDEX
    check_build
fi
if test "$INTERNAL_BOOST" = "TRUE"; then
    LIB_NAME="boost"
    echo "Building internal $LIB_NAME"
    cd $THIRDPARTY_WORK_DIR/Thirdparty/boost
    chmod a+x ./build.sh
    sh ./build.sh $CMDEX
    check_build
fi
if test "$INTERNAL_CURL" = "TRUE"; then
    LIB_NAME="libcurl"
    echo "Building internal $LIB_NAME"
    cd $THIRDPARTY_WORK_DIR/Thirdparty/libcurl
    chmod a+x ./build.sh
    sh ./build.sh $CMDEX
    check_build
fi
if test "$INTERNAL_OPENSSL" = "TRUE"; then
    LIB_NAME="openssl"
    echo "Building internal $LIB_NAME"
    cd $THIRDPARTY_WORK_DIR/Thirdparty/openssl
    chmod a+x ./build.sh
    sh ./build.sh $CMDEX
    check_build
fi

# dump vars to setup script
echo "#!/bin/sh" > $THIRDPARTY_WORK_DIR/env_vars.sh
echo "export BUILD_CPU=$BUILD_CPU" >> $THIRDPARTY_WORK_DIR/env_vars.sh
echo "export BUILD_CONFIG=$BUILD_CONFIG" >> $THIRDPARTY_WORK_DIR/env_vars.sh
echo "export INTERNAL_CPPUNIT=$INTERNAL_CPPUNIT" >> $THIRDPARTY_WORK_DIR/env_vars.sh
echo "export INTERNAL_GDAL=$INTERNAL_GDAL" >> $THIRDPARTY_WORK_DIR/env_vars.sh
echo "export INTERNAL_OPENSSL=$INTERNAL_OPENSSL" >> $THIRDPARTY_WORK_DIR/env_vars.sh
echo "export INTERNAL_MYSQL=$INTERNAL_MYSQL" >> $THIRDPARTY_WORK_DIR/env_vars.sh
echo "export INTERNAL_POSTGRESQL=$INTERNAL_POSTGRESQL" >> $THIRDPARTY_WORK_DIR/env_vars.sh
echo "export INTERNAL_BOOST=$INTERNAL_BOOST" >> $THIRDPARTY_WORK_DIR/env_vars.sh
echo "export INTERNAL_CURL=$INTERNAL_CURL" >> $THIRDPARTY_WORK_DIR/env_vars.sh
echo "export INTERNAL_XERCESC=$INTERNAL_XERCESC" >> $THIRDPARTY_WORK_DIR/env_vars.sh
echo "export INTERNAL_XALANC=$INTERNAL_XALANC" >> $THIRDPARTY_WORK_DIR/env_vars.sh
chmod +x $THIRDPARTY_WORK_DIR/env_vars.sh

echo "$THIRDPARTY_WORK_DIR/env_vars.sh written"
echo "Make sure to run cmake_build.sh with --thirdparty_working_dir set to: $THIRDPARTY_WORK_DIR"
