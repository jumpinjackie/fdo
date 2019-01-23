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
- Create a global instalable FindFDO.cmake to enable providers compile independent of full FDO source
- Compile providers ( independent )


----------------------------------
Unix / Linux Instructions
----------------------------------

I. Requirements
    Base:
        CMake (2.8 or higher)
        A version of gcc with C++11 support (4.8 or higher)
            For CentOS 6, you can install devtoolset for modern gcc (https://www.softwarecollections.org/en/scls/rhscl/devtoolset-7/)
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
    - cmake --build . [or: make]
    - sudo cmake --build . --target install [or: sudo make install]

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
        an older compiler which requires them or you are attempting a distro-agnostic generic build

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

VII. Optional extras

    ccache will be used if found

        If you have ccache installed it will be used for caching compilations

    Using ninja instead of make

        ninja is a lightweight alternative to make that can build FDO up to 2x faster than make

        You can use ninja over standard make by passing USE_NINJA=1 for the main FDO build or passing --ninja 
        to ./cmake_build.sh

    Using gold linker instead of ld

        gold is an alternative linker to ld that may link faster in some cases

        You may also choose to use the gold linker over ld for linking by passing USE_LD_GOLD=ON for the main FDO build or
        passing --use-ld-gold to ./cmake_build.sh

VIII. Running unit tests

    The CMake build builds unit test runner executables for every provider that provides unit tests.

    The CMake build will generate all message catalogs to a local [MY_BUILDDIR]/nls/linux/en_US directory.

    Before running any of the unit tests below, you should set the NLSPATH environment variable to the above
    path so that tests that check for certain FDO/Provider error messages do not fail.

        FDO:

            cd [MY_BUILDDIR]/Fdo/UnitTest
            ./UnitTest

        WMS:

            cd [MY_BUILDDIR]/Providers/WMS/Src/UnitTest
            ./UnitTest

        SDF:

            cd [MY_BUILDDIR]/Providers/SDF/Src/UnitTest
            ./UnitTest
 
        SHP:

            export LD_LIBRARY_PATH=/usr/local/fdo-x.y.z/lib[64]
            cd [MY_BUILDDIR]/Providers/SHP/Src/UnitTest
            rm providers.xml
            [../../TestData/clean &&] ./UnitTest

            Where x.y.z matches the installed version of FDO

            Please note, on subsequent test runs, you should run [MY_BUILDDIR]/Providers/SHP/TestData/clean
            first before running ./UnitTest to clean out intermediate data files produced from the previous
            test run. Failure to do this will generate false positive test failures

            Also make sure that Japanese locales are installed as they are required by certain unit tests. An example
            of how to set this up on Ubuntu is as follows:

               sudo locale-gen ja_JP.EUC-JP
               sudo update-locale

            Also note: Due to a working directory bug in the provider and/or the test executable, the SHP unit test
            executable cannot be run directly from where the executable is built. You need to install FDO after building,
            export this path out to LD_LIBRARY_PATH and delete the local providers.xml before running the test executable

        GDAL:

            export LD_LIBRARY_PATH=/usr/local/fdo-x.y.z/lib[64]
            cd [MY_BUILDDIR]/Providers/GDAL/Src/UnitTest
            rm providers.xml
            ./UnitTest
            
            Where x.y.z matches the installed version of FDO
            
            Note: Due to a working directory bug in the provider and/or the test executable, the GDAL unit test
            executable cannot be run directly from where the executable is built. You need to install FDO after building,
            export this path out to LD_LIBRARY_PATH and delete the local providers.xml before running the test executable
          
        MySql:

            cd [MY_BUILDDIR]/Providers/GenericRdbms/Src/UnitTest

            If you wish to run the MySql unit tests without specifying 
            an initialization file, update the default initialization file 
            "MySqlInit.txt" file with valid values for username, password and 
            service for the service against which the unit tests should be  
            executed and run ./UnitTestMySql with no additional parameters.

            NOTE: Do not drop MySqlInit.txt in subversion if you choose to 
            modify it

            If you wish to run the unit test and specify your own 
            initialization file, create your the file with valid values for 
            username, password and service and run the unit test by specifying
            the initialization file on the command line when executing the unit
            tests. 

            e.g. ./UnitTestMySql initfiletest=MySqlInitEx.txt

            NOTE: The initialization file must contain values for service, 
            username and password.

            e.g.: service=mysqlserver;username=root;password=xxxx;

        Odbc:

            cd [MY_BUILDDIR]/Providers/GenericRdbms/Src/UnitTest

            you must have the DSN created before you will run the unit tests.
            DSN name can be specified using DSNOracle, DSNMySql, DSNSqlServer.

            If you wish to run the ODBC unit tests without specifying 
            an initialization file, update the default initialization file 
            "OdbcInit.txt" file with valid values for username, password and 
            service for the services against which the unit tests should  
            be executed and run ./UnitTestOdbc with no additional parameters.

            NOTE: Do not drop OdbcInit.txt in subversion if you choose to 
            modify it.

            If you wish to run the unit test and specify your own 
            initialization file, create your the file with valid values for  
            username, password and the service names. Run the unit test by  
            specifying the initialization file on the command line when 
            executing the unit tests. 

            e.g. ./UnitTestOdbc initfiletest=OdbcInitEx.txt

            NOTE: The initialization file must contain values for service, 
            username and password.

            The initialization file must contain service, username and 
            password for each server type.

            e.g.: 

            serviceOracle=oraserver;usernameOracle=xxxx;passwordOracle=xxxx;DSNOracle=ORACLE;
            serviceMySql=mysqlserver;usernameMySql=root;passwordMySql=xxxx;DSNMySql=MySQL;

            NOTE: You can also run the unit tests separately for each ODBC subtype:

            MySql:   ./UnitTestOdbc OdbcMySqlTests
            Oracle:  ./UnitTestOdbc OdbcOracleTests

        King Oracle:

            cd [MY_BUILDDIR]/Providers/KingOracle/src/KgOraUnitTest
            ./UnitTest

            Before running ./UnitTest make sure that you first add the Oracle client library 
            path to the LD_LIBRARY_PATH environment variable.
            
            The test suite requires a running Oracle 11g XE instance on localhost listening on port 1521. If you 
            have docker installed, you can spin up an 11g XE docker container with the provided docker-env-11g.sh script.
            
            If your oracle instance resides elsewhere you may set the following environment variables to tell the unit test
            runner where your oracle instance is:
            
             KG_DEFAULT_ORA_CONNECTION - The FDO connection string to your custom oracle instance (eg. Username=fdounittest;Password=fdounittest;Service=//localhost:1521/xe;OracleSchema=fdounittest)
             KG_ORA_USERNAME - The oracle username (eg. fdounittest)
             KG_ORA_PASSWORD - The password for your oracle username (eg. fdounittest)
             KG_ORA_SERVICE - The oracle service (eg. //localhost:1521/xe)
             
            The unit test runner will run and set up connections via both connection string and user/pass/service triplets, so if you do set one of these environment
            variables above, you should set it for all of them too

Please send any help questions, bug reports, enhancements, etc to the fdo-users mailing list <fdo-users@lists.osgeo.org>
