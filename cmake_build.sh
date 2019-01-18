#!/bin/sh

CMAKE_BUILD_DIR=
THIRDPARTY_WORK_DIR=
USE_NINJA=0
SANITIZE_ADDRESS=OFF
USE_LD_GOLD=OFF
WITH_SDF=FALSE
WITH_SHP=FALSE
WITH_SQLITE=FALSE
WITH_OGR=FALSE
WITH_GDAL=FALSE
WITH_WFS=FALSE
WITH_WMS=FALSE
WITH_GENERICRDBMS=FALSE
WITH_KINGORACLE=FALSE
ORACLE_INCLUDE=
ORACLE_LIB=
ORACLE_OCI_VERSION=110
while [ $# -gt 0 ]; do    # Until you run out of parameters...
    case "$1" in
        --cmake-build-dir)
            CMAKE_BUILD_DIR="$2"
            shift
            ;;
        --thirdparty-working-dir)
            THIRDPARTY_WORK_DIR="$2"
            shift
            ;;
        --ninja)
            USE_NINJA=1
            ;;
        --use-ld-gold)
            USE_LD_GOLD=ON
            ;;
        --with-oracle-include)
            ORACLE_INCLUDE="$2"
            shift
            ;;
        --with-oracle-lib)
            ORACLE_LIB="$2"
            shift
            ;;
        --with-oci-version)
            ORACLE_OCI_VERSION="$2"
            shift
            ;;
        --with-asan)
            SANITIZE_ADDRESS=ON
            ;;
        --with-all-providers)
            WITH_SDF=TRUE
            WITH_SHP=TRUE
            WITH_SQLITE=TRUE
            WITH_OGR=TRUE
            WITH_GDAL=TRUE
            WITH_WFS=TRUE
            WITH_WMS=TRUE
            WITH_GENERICRDBMS=TRUE
            WITH_KINGORACLE=TRUE
            ;;
        --with-sdf)
            WITH_SDF=TRUE
            ;;
        --with-shp)
            WITH_SHP=TRUE
            ;;
        --with-sqlite)
            WITH_SQLITE=TRUE
            ;;
        --with-ogr)
            WITH_OGR=TRUE
            ;;
        --with-gdal)
            WITH_GDAL=TRUE
            ;;
        --with-wfs)
            WITH_WFS=TRUE
            ;;
        --with-wms)
            WITH_WMS=TRUE
            ;;
        --with-genericrdbms)
            WITH_GENERICRDBMS=TRUE
            ;;
        --with-kingoracle)
            WITH_KINGORACLE=TRUE
            ;;
        --help)
            echo "Usage: $0 (options)"
            echo "Options:"
            echo "  --cmake-build-dir [CMake build directory]"
            echo "  --thirdparty-working-dir [thirdparty working directory]"
            echo "  --with-asan [build with ASAN]"
            echo "  --ninja [Use ninja build system]"
            echo "  --use-ld-gold [Link with gold instead of ld (if available)]"
            echo "  --with-sdf [Enable SDF Provider]"
            echo "  --with-shp [Enable SHP Provider]"
            echo "  --with-sqlite [Enable SQLite Provider]"
            echo "  --with-ogr [Enable OGR Provider]"
            echo "  --with-gdal [Enable GDAL Provider]"
            echo "  --with-wfs [Enable WFS Provider]"
            echo "  --with-wms [Enable WMS Provider]"
            echo "  --with-genericrdbms [Enable GenericRdbms Providers]"
            echo "  --with-kingoracle [Enable King Oracle Provider]"
            echo "  --with-all-providers [Enable all providers]"
            echo "  --with-oracle-include [Oracle client include path]"
            echo "  --with-oracle-lib [Oracle client lib path]"
            echo "  --with-oci-version [Oracle client version]"
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

if [ ! -f $THIRDPARTY_WORK_DIR/env_vars.sh ]; then
    echo "ERROR: Could not find env_vars.sh in $THIRDPARTY_WORK_DIR"
    echo "       This file should exist if you ran cmake_bootstrap.sh with (--thirdparty-working-dir $THIRDPARTY_WORK_DIR)"
    exit 1;
fi

echo "Reading variables from thirdparty build"
. $THIRDPARTY_WORK_DIR/env_vars.sh

# Validate
if test "$BUILD_CPU" != "32" -a "$BUILD_CPU" != "64"; then
    echo "--build must be 32 or 64"
    exit 1;
fi
if test "$BUILD_CONFIG" != "Release" -a "$BUILD_CONFIG" != "Debug"; then
    echo "--config must be Release or Debug"
    exit 1;
fi

echo "Current source dir is: $SOURCE_DIR"
echo "Building for: $BUILD_CPU-bit"
echo "Building in: $BUILD_CONFIG"
echo "Using Ninja build: $USE_NINJA"
echo "Linking with gold instead of ld (if possible): $USE_LD_GOLD"
echo "CMake build directory is: $CMAKE_BUILD_DIR" 
echo "Thirdparty Working Directory is: $THIRDPARTY_WORK_DIR"

echo "Creating CMake build directory"
mkdir -p $CMAKE_BUILD_DIR

CMAKE_INTERNAL_ARGS="-DINTERNAL_CPPUNIT=$INTERNAL_CPPUNIT -DINTERNAL_GDAL=$INTERNAL_GDAL -DINTERNAL_OPENSSL=$INTERNAL_OPENSSL -DINTERNAL_MYSQL=$INTERNAL_MYSQL -DINTERNAL_POSTGRESQL=$INTERNAL_POSTGRESQL -DINTERNAL_CURL=$INTERNAL_CURL -DINTERNAL_XERCESC=$INTERNAL_XERCESC -DINTERNAL_XALANC=$INTERNAL_XALANC -DINTERNAL_THIRDPARTY_DIR=$THIRDPARTY_WORK_DIR"
CMAKE_ARGS="-DFDO_CPU=$BUILD_CPU -DCMAKE_BUILD_TYPE=$BUILD_CONFIG -DUSE_LD_GOLD=$USE_LD_GOLD -DSANITIZE_ADDRESS=$SANITIZE_ADDRESS -DWITH_SDF=$WITH_SDF -DWITH_SHP=$WITH_SHP -DWITH_SQLITE=$WITH_SQLITE -DWITH_WFS=$WITH_WFS -DWITH_WMS=$WITH_WMS -DWITH_OGR=$WITH_OGR -DWITH_GDAL=$WITH_GDAL -DWITH_GENERICRDBMS=$WITH_GENERICRDBMS -DWITH_KINGORACLE=$WITH_KINGORACLE -DORACLE_PATH_INCLUDES=$ORACLE_INCLUDE -DORACLE_PATH_LIB=$ORACLE_LIB -DORACLE_OCI_VERSION=$ORACLE_OCI_VERSION $CMAKE_INTERNAL_ARGS"

echo "CMake args: $CMAKE_ARGS"

# Now for the main event
echo "Setting up CMake in: $CMAKE_BUILD_DIR"
cd $CMAKE_BUILD_DIR
if test $USE_NINJA -eq 1; then
    cmake -G Ninja $SOURCE_DIR $CMAKE_ARGS
    check_build
else
    cmake $SOURCE_DIR $CMAKE_ARGS
    check_build
fi
cmake --build .
check_build