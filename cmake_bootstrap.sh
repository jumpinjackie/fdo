#!/bin/sh
BUILD_CPU=32
BUILD_CONFIG=Release
INTERNAL_CPPUNIT=FALSE
INTERNAL_GDAL=FALSE
INTERNAL_OPENSSL=FALSE
INTERNAL_MYSQL=FALSE
INTERNAL_POSTGRESQL=FALSE
INTERNAL_CURL=FALSE
INTERNAL_XERCESC=FALSE
INTERNAL_XALANC=FALSE

while [ $# -gt 0 ]; do    # Until you run out of parameters...
    case "$1" in
        --working-dir)
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
            echo "  --working-dir [build directory]"
            echo "  --build [32(default)|64]"
            echo "  --config [Release(default)|Debug]"
            echo "  --internal-cppunit [Use internal cppunit]"
            echo "  --internal-gdal [Use internal gdal]"
            echo "  --internal-openssl [Use internal openssl]"
            #echo "  --internal-mysql [Use internal mysql]"
            #echo "  --internal-postgresql [Use internal postgresql]"
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
    echo "--working-dir must be specified"
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

####################### Begin inline setenvironment.sh ######################

# Fully-qualfied location of the FDO files
export FDO=$SOURCE_DIR/Fdo
if test ! -e "$FDO"; then
   echo ""
   echo "Invalid FDO path provided. "
   echo "The setenvironment script sets the default value to: "
   echo "$FDO"
   echo "Please modify the setenvironment.sh script with a valid path."
   echo ""
fi

# Fully-qualfied location of the FDO Utility files
export FDOUTILITIES=$SOURCE_DIR/Utilities
if test ! -e "$FDOUTILITIES"; then
   echo ""
   echo "Invalid FDO Utilities path provided. "
   echo "The setenvironment script sets the default value to: "
   echo "$FDOUTILITIES" 
   echo "Please modify the setenvironment.sh script with a valid path."
   echo ""
fi

# Fully-qualfied location of the FDO Thirdparty files
export FDOTHIRDPARTY=$THIRDPARTY_WORK_DIR/Thirdparty
if test ! -e "$FDOTHIRDPARTY"; then
   echo ""
   echo "Invalid FDO Thirdparty path provided. "
   echo "The setenvironment script sets the default value to: "
   echo "$FDOTHIRDPARTY"
   echo "Please modify the setenvironment.sh script with a valid path."
   echo ""
fi

# Fully-qualfied location of the ESRI ArcSDE SDK
export SDEHOME=$FDOTHIRDPARTY/ESRI/ArcSDEClient931/Linux
if test ! -e "$SDEHOME"; then
   echo ""
   echo "NOTE: The default location for the ArcSDE client SDK files"
   echo "was not found. The setenvironment script sets the default value to: "
   echo "$FDOTHIRDPARTY/ESRI/ArcSDEClient91/Linux. "
   echo "If you need to build the FDO ArcSDE Provider, modify the "
   echo "setenvironment.sh script with a valid path and set SDEHOME to "
   echo "point to the location of the ArcSDE client installation."
   echo ""
fi

# Fully-qualfied location of the GDAL Installation
export FDOGDAL=$FDOTHIRDPARTY/gdal
   echo ""
   echo "NOTE: The setenvironment.sh script sets the installation location for "
   echo "the GDAL SDK files to $FDOTHIRDPARTY/gdal. "
   echo "If this value remains unchanged, the FDO build process will"
   echo "build the version of GDAL located in Thirdparty/gdal and will "
   echo "install the resulting libraries in /usr/local/fdo-4.2.0. The FDO build"
   echo "process will then use that location when building the GDAL and"
   echo "WMS providers. If you wish to build the FDO GDAL or WMS Providers"
   echo "using a previously installed version of GDAL, modify the setenvironment.sh "
   echo "script and set FDOGDAL to point to the existing GDAL installation."
   echo "For example: /user/local (The default GDAL installation path)."
echo ""

# Fully-qualfied location of the MySQL SDK
export FDOMYSQL=$FDOTHIRDPARTY/mysql/rhlinux
if test ! -e "$FDOMYSQL"; then
   echo ""
   echo "NOTE: The default location for the MySQL SDK files "
   echo "was not found. The setenvironment script sets the default value to: "
   echo "$FDOTHIRDPARTY/mysql/rhlinux "
   echo "If you need to build the FDO MySQL Provider, and have chosen not "
   echo "to install the MySQL SDK using the recommended RPM installtion "
   echo "process, modify the setenvironment.sh script and set FDOMYSQL to point"
   echo "to the location of the MySQL client installation. If you have chosen "
   echo "to install the MySQL client using the RPM install, this environment "
   echo "variable does not need to be set and this warning can be ignored."
   echo ""
fi

# Fully-qualfied location of the PostgreSQL SDK
export FDOPOSTGRESQL=$FDOTHIRDPARTY/pgsql
if test ! -e "$FDOPOSTGRESQL"; then
   echo ""
   echo "NOTE: The default path for the PostgreSQL SDK files was not found. "
   echo "The setenvironment script sets the default value to: "
   echo "$FDOPOSTGRESQL"
   echo "If you need to build the PostgreSQL prvovider, modify the setenvironment.sh "
   echo "script and set FDOPOSTGRESQL to point to the location of the PostgreSQL SDK."
   echo ""
fi

# Fully-qualfied location of the ODBC SDK
export FDOODBC=/usr
if test ! -e "$FDOODBC"; then
   echo ""
   echo "NOTE: The default path for the ODBC SDK files was not found. "
   echo "The setenvironment script sets the default value to: "
   echo "$FDOODBC"
   echo "If you need to build the ODBC prvovider, modify the setenvironment.sh "
   echo "script and set FDOODBC to point to the location of the ODBC SDK."
   echo ""
fi

# Location of the PYTHON lib files. Typically in /usr/lib/pythonXXX
export PYTHON_LIB_PATH=/usr/lib/python2.4
if test ! -e "$PYTHON_LIB_PATH"; then
   echo ""
   echo "NOTE: The default path for the Python SDK lib files was not found. "
   echo "The setenvironment script sets the default value to: "
   echo "$PYTHON_LIB_PATH"
   echo "If you need to build the FDO Python wrappers, modify the setenvironment.sh "
   echo "script and set PYTHON_LIB_PATH to point to the location of the Python "
   echo "lib files."
   echo ""
fi

# Location of the PYTHON include files. Typically in /usr/include/pythonXXX
export PYTHON_INCLUDE_PATH=/usr/include/python2.4
if test ! -e "$PYTHON_INCLUDE_PATH"; then
   echo ""
   echo "NOTE: The default path for the Python SDK header files was not found. "
   echo "The setenvironment script sets the default value to: "
   echo "$PYTHON_INCLUDE_PATH"
   echo "If you need to build the FDO Python wrappers, modify the setenvironment.sh "
   echo "script and set PYTHON_INCLUDE_PATH to point to the location of the Python "
   echo "include files."
   echo ""
fi

export XERCESCROOT=$FDOTHIRDPARTY/apache/xerces
export XALANCROOT=$FDOTHIRDPARTY/apache/xalan
export NLSDIR=$XALANCROOT/src/xalanc/NLS

####################### End inline setenvironment.sh ######################

# Patch various build scripts (to strip sudo)
if test -f "$THIRDPARTY_WORK_DIR/Thirdparty/apache/build2.sh"; then
    sed -i "s#sudo ##g" $THIRDPARTY_WORK_DIR/Thirdparty/apache/build2.sh
fi

CMDEX="-b $BUILD_CPU -a buildinstall"

if test "$CFLAGS" != *"-m$BUILD_CPU"*; then
    CFLAGS="$CFLAGS -m$BUILD_CPU"
    echo "Exporting CFLAGS: "$CFLAGS""
    export CFLAGS
fi

if test "$CPPFLAGS" != *"-m$BUILD_CPU"*; then
    CPPFLAGS="$CPPFLAGS -m$BUILD_CPU"
    echo "Exporting CPPFLAGS: "$CPPFLAGS""
    export CPPFLAGS
fi

if test "$LDFLAGS" != *"-m$BUILD_CPU"*; then
    LDFLAGS="$LDFLAGS -m$BUILD_CPU"
    echo "Exporting LDFLAGS: "$LDFLAGS""
    export LDFLAGS
fi

if test "$BUILD_CPU" = "32" ; then
    if test "$HOSTTYPE" = "i686" ; then
        if test "$CPPFLAGS" != *"-march=i686"*; then
            CPPFLAGS="$CPPFLAGS -march=i686"
            echo "Exporting CPPFLAGS: "$CPPFLAGS""
            export CPPFLAGS
        fi
    fi
fi

if test "$CPPFLAGS" != *"-Wno-write-strings"*; then
    CPPFLAGS="$CPPFLAGS -Wno-write-strings"
    echo "Exporting CPPFLAGS: "$CPPFLAGS""
    export CPPFLAGS
fi

if test "$CPPFLAGS" != *"-Wno-deprecated"*; then
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
else
    echo "Skipping internal xalan/xerces build"
fi
if test "$INTERNAL_CPPUNIT" = "TRUE"; then
    LIB_NAME="cppunit"
    echo "Building internal $LIB_NAME"
    cd $THIRDPARTY_WORK_DIR/Thirdparty/linux/cppunit
    chmod a+x ./build.sh
    sh ./build.sh $CMDEX
    check_build
else
    echo "Skipping internal cppunit build"
fi
if test "$INTERNAL_GDAL" = "TRUE"; then
    LIB_NAME="gdal"
    echo "Building internal $LIB_NAME"
    cd $THIRDPARTY_WORK_DIR/Thirdparty/gdal
    chmod a+x ./build.sh
    sh ./build.sh $CMDEX
    check_build
else
    echo "Skipping internal gdal build"
fi
if test "$INTERNAL_CURL" = "TRUE"; then
    LIB_NAME="libcurl"
    echo "Building internal $LIB_NAME"
    cd $THIRDPARTY_WORK_DIR/Thirdparty/libcurl
    chmod a+x ./build.sh
    sh ./build.sh $CMDEX
    check_build
else
    echo "Skipping internal curl build"
fi
if test "$INTERNAL_OPENSSL" = "TRUE"; then
    LIB_NAME="openssl"
    echo "Building internal $LIB_NAME"
    cd $THIRDPARTY_WORK_DIR/Thirdparty/openssl
    chmod a+x ./build.sh
    sh ./build.sh $CMDEX
    check_build
else
    echo "Skipping internal openssl build"
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
echo "export INTERNAL_CURL=$INTERNAL_CURL" >> $THIRDPARTY_WORK_DIR/env_vars.sh
echo "export INTERNAL_XERCESC=$INTERNAL_XERCESC" >> $THIRDPARTY_WORK_DIR/env_vars.sh
echo "export INTERNAL_XALANC=$INTERNAL_XALANC" >> $THIRDPARTY_WORK_DIR/env_vars.sh
chmod +x $THIRDPARTY_WORK_DIR/env_vars.sh

echo "$THIRDPARTY_WORK_DIR/env_vars.sh written"
echo "Make sure to run cmake_build.sh with --thirdparty-working-dir set to: $THIRDPARTY_WORK_DIR"
