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
    If you desire buils all third party dependencies, use:
    -DALL_THIRDPARTY=TRUE
    
    NOTE: It is *not* recommended to use the internal third party libraries, unless you are using
        an older compiler which requires them.

IV. Debug mode

    To build FDO and its providers in debug mode. Pass in the following switch in addition to other CMake switches:
    
        -DCMAKE_BUILD_TYPE=Debug

    This will set up necessary debug-only compiler flags. The value for this setting is case-sensitive

    If your compiler is gcc 4.8 or higher you can also compile with Address Sanitizer (ASAN) support by passing in:
    
        -DSANITIZE_ADDRESS=ON

    If compiling in debug mode, it is also recommended to compile with ASAN support if your compiler supports it as this
    can detect a certain class of errors and bugs without needing to use an external tool like valgrind

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

VI. Using the wrapper scripts

    For convenience, the following wrapper scripts are provided to automate the setup and invocation of the CMake build

        cmake_bootstrap.sh
        cmake_build.sh
    
    The cmake_bootstrap.sh script will build thirdparty components in an external source tree and output an env_vars.sh script 
    to the root of this external source tree.

    The cmake_build.sh script will then use the vars from this env_vars.sh (along with any switches) to then set up the main 
    CMake build and start it. The env_vars.sh script generated by cmake_bootstrap.sh will inform the main CMake build as to 
    whether to use an internal or system-installed thirdparty component and the appropriate configuration/bitness so that the
    main FDO build lines up with the same configuration/bitness used to build the thirdparty components.

    For example, this will build all thirdparty components and the main CMake build will use all internal thirdparty components 
    instead of looking for system-installed counterparts:

        ./cmake_bootstrap.sh --working-dir ~/fdo_thirdparty_build --all-internal
        ./cmake_build.sh --thirdparty-working-dir ~/fdo_thirdparty_build \
            --cmake-build-dir ~/fdo_build \
            --with-all-providers \
            --with-oracle-include ~/oracle/instantclient_11_2/sdk/include \
            --with-oracle-lib ~/oracle/instantclient_11_2/sdk/lib \
            --with-oci-version 110

    From here you can install as follows:

        cd ~/fdo_build
        sudo make install

    Or create a tarball package:

        cd ~/fdo_build
        make package

    You can pass the --help switch to both scripts to find out all available parameters that are supported.

Please send any help questions, bug reports, enhancements, etc to the fdo-users mailing list <fdo-users@lists.osgeo.org>