Feature Data Objects (FDO) CMake Experimental Build
===================================================

This ReadMe contains instructions of how to use CMake to compile FDO without using the automake/shell scripts.
Initial support is only Linux and Unix and the goal is use all possible system external libraries,
but code is been prepared for Windows ( possible MacOSX )

Current Status:

- Compiles Whole FDO Core and Utilities, and the following providers:
    - SDF
    - SHP
    - OGR
    - SQLite
    - GDAL
    - GenericRdbms (ODBC, MySQL, PostgreSQL)
    - WFS
    - WMS
    - KingOracle

- When compiling on Ubuntu 14.04 LTS, all thirdparty libraries needed by FDO can be acquired through apt-get. 
  None of the in-tree thirdparty components need to be built.

TODO
- Make internal apache Thirdparty compile ( now only system )
- Finish install part ( not tested yet )
- Create a global instalable FindFDO.cmake to enable providers compile independent of full FDO source
- Compile providers ( independent )


----------------------------------
Unix / Linux Instructions
----------------------------------

I. Requirements
    Base:
        CMake
    For system libraries, development packages for:
        openssl
        curl
        boost
        gdal
        cppunit
        xerces-c
        xalan-c
        postgresql
        mysql
        unixODBC
    For Ubuntu, this is the list of libraries you need installed:
        libssl-dev 
        libcurl4-openssl-dev
        libmysqlclient-dev
        unixODBC-dev
        python-dev
        libpq-dev
        libcppunit-dev
        libxalan-c-dev
        libxerces-c-dev
        libboost-date-time-dev
        libboost-thread-dev
        libboost-system-dev
        libboost-chrono-dev
        libboost-program-options-dev
        libgdal-dev

II. Usage
    Cmake build can be done ( and recomended to do ) outside of the source directory, so a simple run would be
    - mkdir <MY_BUILDDIR>
    - cd <MY_BUILDDIR>
    - cmake <FDO_SOURCEDIR> -DWITH_SDF=TRUE -DWITH_SHP=TRUE -DWITH_SQLITE=TRUE
    - make
    - sudo make install

    The example enables the SDF, SHP and SQlite provider builds. 
        You can pass any definition to compiler through this method.

III. Using internal provided Thirdparty libraries
    Since fdo main source has a Thirdparty internal  dependencies source provided, if you desire
    compile and use internal dependencies, add the follow definitions in the cmake command line call:
    -DINTERNAL_<DEPENDENCY>=TRUE
    Where DEPENDENCY can be one of this ( case sensitive ):
    - OPENSSL - openssl library
    - GDAL - gdal library
    - CPPUNIT - Old cppunit version
    - CURL - curl library
    - BOOST - Boost libraries
    If you desire buils all third party dependencies, use:
    -DALL_THIRDPARTY=TRUE
    
    NOTE: It is *not* recommended to use the internal third party libraries, unless you are using
        an older compiler which requires them.

IV. Debug mode

    To build FDO and its providers in debug mode. Pass in the following switch in addition to other CMake switches:
    
        -DCMAKE_BUILD_TYPE=Debug

    This will set up necessary debug-only compiler flags. The value for this setting is case-sensitive

V.  Compiling Providers
    Providers can be called directly from cmake commandline, no need to compile separated anymore if you
    are doing in source build
    
    If you desire compile some provider(s), add the follow definitions in the cmake command line call:
        -DWITH_<provider>
    Where <provider> is the name of provider you want to compile, be case sensitive, all upper case or all lower case.

    All OpenSource providers are available already. Usage example:
        -DWITH_GDAL=TRUE or -DWITH_Gdal=TRUE or -DWITH_gdal=TRUE

    With some providers:
        -DWITH_GDAL=TRUE -DWITH_WMS=TRUE

    If you desire to build all available providers, use this definition ( case sensitive )
        -DALL_PROVIDERS=TRUE

    There's no automatic provider compilation unless explicit defined.
    The compilation order is same as providers defined in command line.

    When building the King Oracle provider, CMake will try to guess your client version and installation from
    ORACLE_HOME environment variable (implying a proper Oracle client installation). If you are building against
    an extracted archive of the Oracle Instant Client, such information wouldn't exist and you will need to tell
    cmake the version of your Oracle client and where its headers/libs are located

    For example, suppose you have extracted your 11g Oracle Instant Client location to /home/fdobuild/oracle/instantclient_11_2/sdk
    then you would need to pass the following information to CMake:

        -DORACLE_PATH_INCLUDES=/home/fdobuild/oracle/instantclient_11_2/sdk/include
        -DORACLE_PATH_LIB=/home/fdobuild/oracle/instantclient_11_2/sdk/lib
        -DORACLE_OCI_VERSION=110

    NOTE: The following providers do not have CMake configurations and cannot be built:

        ArcSDE
        KingInformix
        KingKML
        KingMsSqlSpatial
        GenericRdbms/SQLServerSpatial

Help and reports to Helio Castro <helio@kde.org>


