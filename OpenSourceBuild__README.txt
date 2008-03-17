Feature Data Objects (FDO) OpenSource README
============================================

This ReadMe contains important information required to build FDO open source.
Ensure you read through this document before building.

Contents:

Windows Instructions
   - Build Instructions
   - Build Notes
   - Running the FDO Unit Tests
Linux Instructions
   - Build Instructions
   - Build Notes
   - Running the FDO Unit Tests

-----------------------------
Windows Instructions
-----------------------------

I. Build Instructions

  1. cd [FDO OpenSource]

  2. Open and modify setenvironment.bat to specify the locations of the Thirdparty SDK 
     components and build tools. (Refer to the Build Notes below)

  3. call setenvironment.bat

  4. call build_thirdparty.bat

  5. call build.bat


II. Build Notes

  In preparation for building the Windows Open Source FDO Binaries:

  1. The FDO open source build process supports the usage of either the Visual 
     Studio Express, Standard or Professional editions. 

     The Visual Studio Express software can be download and installed for 
     free from the following sites.

     Visual Studio C# Express Edition:
         
        http://msdn.microsoft.com/vstudio/express/visualcsharp/download/
     
     Visual Studio C++ Express Edition: 
 
        http://msdn.microsoft.com/vstudio/express/visualc/download/

     In order to build the FDO Components using Visual Studio Express, you are also
     required to download and install the Windows® Server 2003 R2 Platform SDK.

     Download the Windows® Server 2003 R2 Platform SDK from: 

        http://www.microsoft.com/downloads/details.aspx?FamilyId=0BAF2B35-C656-4969-ACE8-E4C0C0716ADB&displaylang=en

     NOTE: The Windows® Server 2003 R2 Platform SDK is composed of many components. 
     The only components required for building FDO are:

        - Microsoft Windows Core SDK
        - Microsoft Data Access Services (MDAC) SDK
        - Microsoft Web Workshop (IE) SDK
        
     You can limit you installation to these components only. 

     The easiest procedure to use the PSDK with VC2005 Express is:

        a) Create a directory "PlatformSDK" under "%ProgramFiles%\Microsoft Visual Studio 8\VC"
        b) Copy the "bin", "include" and "lib" directory from the PSDK-installation into this directory. 

  2. Install ActiveState Perl, available from:
     
        http://www.activestate.com/ActivePerl

  3. Ensure that the Windows Environment Variables are set as follows:

        SET FDO=[FDO OpenSource]\Fdo
        SET FDOUTILITIES=[FDO OpenSource]\Utilities
        SET FDOTHIRDPARTY=[FDO OpenSource]\ThirdParty
        SET XALANROOT=%FDOTHIRDPARTY%\apache\xml-xalan\c
        SET XERCESCROOT=%FDOTHIRDPARTY%\apache\xml-xerces\c
        SET NLSDIR=%XALANROOT%\Src\xalanc\NLS

  4. In order to build all FDO Windows components, ensure that the Microsoft
     MsBuild tool that is used by the the FDO build process is included in 
     the Windows system PATH.

        set PATH=%PATH%;C:\WINDOWS\Microsoft.NET\Framework\v2.0.50727
 
  5. In order to build the ArcSDE Provider, you will need to have the
     ArcSDE 9.1 Client SDK and/or ArcSDE 9.2 Client SDK installed 
     and the following environment variable set that points to the installation location:

        set SDEHOME=[ArcSDE developer components path]  - used by unit test 
        set SDEVER_ARCUNITTEST=[91]/[92] 

     SDEVER_ARCUNITTEST=91   -will build unit test using ArcSDE 9.1 Client
     SDEVER_ARCUNITTEST=92   -will build unit test using ArcSDE 9.2 Client
     
     In case user has ArcSDE 9.1 Client SDK and ArcSDE 9.2 Client SDK installed:
     Copy the "bin", "include" and "lib" directory from the ArcSDE 9.1 Client SDK into:
     %FDOTHIRDPARTY%\ESRI\ArcSDEClient91\Windows
     
     Copy the "bin", "include" and "lib" directory from the ArcSDE 9.2 Client SDK into:
     %FDOTHIRDPARTY%\ESRI\ArcSDEClient92\Windows
     
     The script will build the support for both ArcSDE clients.
     In case user has only one version of the ArcSDE client installed, the location must be
     specified using %SDEHOME%
     
     The ArcSDE unit test project will only search for include and lib files in %SDEHOME%
     Note that ArcSDE is licensed software and must be obtained from an ESRI vendor.

  6. In order to build the MySQL Provider, you will need to download 
     and install the MySQL 5.0 Windows client and developer components from:

        http://dev.mysql.com/downloads/mysql/5.0.html

     Following the installation, set the following FDO Windows environment variable:

        set FDOMYSQL=[MySQL developer components path] 

     e.g. set FDOMYSQL=c:\Program Files\MySQL\MySQL Server 5.0

  7. In order to build the King Oracle Provider, you will need to download 
     and install the 'Oracle 10.2.0.3 Instant Client' for Windows from:

        http://www.oracle.com/technology/software/tech/oci/instantclient/htdocs/winsoft.html

     The name of the installation file is 'instantclient-sdk-win32-10.2.0.3-20061115.zip'
	 
	 Following the installation, set the following FDO Windows environment variable:

        set FDOORACLE=[Oracle instant client path] 

     e.g. set FDOORACLE=C:\oracle\instantclient\10.2\sdk

  8. OPTIONAL: The FDO build includes several generated .cpp files that were 
     generated from source .y files by the Bision and Sed utilities. These 
     files are fairly static therefore they are not automatically regenerated 
     as a part of the standard FDO build process. If changes are made 
     to the .y files they will need to be recompiled into their respective 
     .cpp and .h files. 

     In order to rebuild these files download and install the GNU 
     Win32 Bison (version 1.875) and Sed (version 4.1.4) self-extracting utilities.

        "Bison"

        Bison is a general-purpose parser generator that converts a grammar 
        description for an LALR context-free grammar into a C program to parse 
        that grammar.

        Home Page: 
        http://gnuwin32.sourceforge.net/packages/bison.htm
        
        Download Location: 
        http://prdownloads.sourceforge.net/gnuwin32/bison-1.875-1.exe?download
         
        "Sed"

        Sed (streams editor) isn't really a true text editor or text processor. 
        Instead, it is used to filter text, i.e., it takes text input and performs 
        some operation (or set of operations) on it and outputs the modified text. 
        Sed is typically used for extracting part of a file using pattern matching 
        or substituting multiple occurrences of a string within a file. 

        Home Page: 
        http://gnuwin32.sourceforge.net/packages/sed.htm
        
        Download Location: 
        http://prdownloads.sourceforge.net/gnuwin32/sed-4.1.4.exe?download

   
     FDO has been tested using Bison 1.875 and Sed 4.1.4 and will not work with 
     other versions of these binaries. The Bison and Sed executable files MUST be 
     included in the Windows PATH in order to be used by the FDO build process. 

        e.g. set PATH=%PATH%;C:\Program Files\GnuWin32\bin

     The optional FDO build script that will need to be executed to rebuild the 
     generated cpp source files is:

        build_parse.bat
 
  9. OPTIONAL: The FDO build process allows it's documentation to be regenerated. 
     If this is required, install the OpenSource Doxygen documentation generation
     software.  Doxygen is a documentation system for C++, C, Java, Objective-C, 
     Python, IDL (Corba and Microsoft flavors) and to some extent PHP, C#, and D.
  
     To install Doxygen, refer to: http://www.stack.nl/~dimitri/doxygen/
     
     The FDO Doxygen documentation generation process will also use the "dot" tool  
     from graphviz 1.5 to generate more advanced diagrams and graphs. Graphviz is an 
     "open-sourced", cross-platform graph drawing toolkit from AT&T and Lucent 
     Bell Labs.
     
     To install graphviz, refer to: http://www.graphviz.org/ 

     The FDO Doxygen documentation generation process also uses the Microsoft 
     HTML Help Workshop. Microsoft HTML Help is the standard help system for the 
     Windows platform. Authors can use HTML Help to create online help for a 
     software application or to create content for a multimedia title or Web site.

     To install the Microsoft HTML Help Workshop, refer to: 

     http://msdn.microsoft.com/library/default.asp?url=/library/en-us/htmlhelp/html/hwMicrosoftHTMLHelpDownloads.asp

     Ensure that the tools are in the build environment's PATH.

        Doxygen    -- e.g. C:\Program Files\doxygen\bin
        Dot        -- e.g. C:\Program Files\ATT\Graphviz\bin

        e.g.
        
        set PATH=%PATH%;C:\Program Files\doxygen\bin
        set PATH=%PATH%;C:\Program Files\ATT\Graphviz\bin

 10. OPTIONAL: The FDO build process supports the generation of a set of python 
     wrapper scripts for the FDO API. The python scripts are generated using SWIG. To
     generate the python scripts, you must have Python and SWIG installed.
  
     To install SWIG, refer to: http://www.swig.org/
     To install Python, refer to: http://www.python.org/
     
     The FDO python scripts have been tested using SWIG 1.3.31 and Python 2.4.4
     
     Once SWIG and python have been installed set the PYTHON_HOME and SWIG_HOME environment
     variables to point to the respective installation locations.
     
        e.g.
          
        SET PYTHON_HOME=C:\progra~1\Python24
        SET SWIG_HOME=C:\progra~1\swigwin-1.3.31
        
     ** NOTE: These variables are maintained in the setenvironment.bat script. 

 11.Use the [FDO OpenSource]\setenvironment.bat script to assist in setting and
    maintaing the correct environment settings for the FDO build process. 
    This script can be modifed and used to set the correct environment variables 
    and PATH settings.

 12. The build_thirdparty.bat file is used to build the FDO Thirdparty binaries. The following 
     is a general guideline on how to use the build_thirdparty.bat build script.
  
           **************************************************************************
           build_thirdparty.bat [-h] 
                                [-o=OutFolder] 
                                [-c=BuildType] 
                                [-a=Action] 
                                [-w=WithModule]

           Help:           -h[elp]
           OutFolder:      -o[utpath]=destination folder for binaries
           BuildType:      -c[onfig]=release(default), debug
           Action:         -a[ction]=build(default), buildinstall, install, clean
           WithModule:     -w[ith]=all(default), fdo, providers, sdf, wfs, wms
           **************************************************************************            

            e.g.

            **** Display help for build.bat
            build_thirdparty -h
            **** Build all Thirdparty components
            build_thirdparty
            **** Build all Thirdparty components and install output files to C:\Fdo
            build_thirdparty -a=buildinstall -o=C:\Fdo 
            **** Build Thirdparty components required to build the FDO API libraries
            build_thirdparty -w=fdo
            **** Build Thirdparty components for all FDO Providers
            build_thirdparty -w=providers
            **** Build Thirdparty components for for the SDF Provider
            build_thirdparty -w=sdf
            **** Build Thirdparty components for for the WMS and WFS Providers
            build_thirdparty -w=wms -w=wfs

 13. The build.bat file is used to build the FDO binaries. The following 
     is a general guideline on how to use the build.bat build script.
  
           **************************************************************************
           build.bat [-h]
                     [-o=OutFolder]
                     [-c=BuildType]
                     [-p=PlatformType]
                     [-a=Action]
                     [-w=WithModule]
                     [-d=BuildDocs]
                     [-py=BuildPythonWrappers]
           
           Help:                  -h[elp]
           OutFolder:             -o[utpath]=destination folder for binaries
           BuildType:             -c[onfig]=release(default), debug
		   PlatformType:          -p[latform]=Win32(default), x64
           Action:                -a[ction]=build(default), 
                                            buildinstall, 
                                            install, 
                                            clean
           WithModule:            -w[ith]=all(default), 
                                          fdo, 
                                          providers, 
                                          shp, 
                                          sdf, 
                                          wfs, 
                                          wms, 
                                          arcsde, 
                                          odbc, 
                                          mysql,
                                          gdal,
                                          ogr
                                          postgis
                                          kingoracle
           BuildDocs:             -d[ocs]=skip(default), build
           BuildPythonWrappers:   -py[thon]=skip(default), build
           **************************************************************************
            
            e.g.

            **** Display help for build.bat
            build -h
            **** Build all components excluding documentation
            build
            **** Build all components and install output files to C:\Fdo
            build -a=buildinstall -o=C:\Fdo
            **** Build all components and documentation and install files to C:\Fdo
            build -a=buildinstall -o=C:\Fdo -d=build
            **** Build only FDO libraries
            build -w=fdo
            **** Build all FDO Providers
            build -w=providers
            **** Build the FDO Shape Provider
            build -w=shp
            **** Build the FDO SDF Provider
            build -w=sdf
            **** Build the FDO WMS and WFS Providers
            build -w=wms -w=wfs
            **** Build the FDO Python Wrapper scripts
            build -py=build
            **** Build documentation and install all files to C:\Fdo
            build -a=install -o=C:\Fdo -d=build 

 
III. Running the FDO Unit Tests

    The FDO API and the FDO providers come with unit tests that will allow the various 
    products to be tested once the build is complete. In order to run the FDO API, 
    WMS, SHP, SDF, ArcSDE, GDAL, MySql and Odbc Provider Unit Tests, follow the following
    instructions once the build has completed

        FDO:

            cd [FDO OpenSource]\Fdo\Unmanaged\bin\win32\Debug
            Run UnitTest.exe

        WMS:

            cd [FDO OpenSource]\Providers\WMS\bin\win32\debug
            Run UnitTest.exe

        SDF:

            cd [FDO OpenSource]\Providers\SDF\Src\UnitTest
            Run ..\..\bin\win32\Debug\UnitTest.exe
 
        SHP:

            cd [FDO OpenSource]\Providers\SHP\Src\UnitTest
            Run ..\..\bin\win32\Debug\UnitTest.exe

        ArcSDE:

            NOTE: Please refer to [FDO OpenSource]\Providers\ArcSDE\UnitTest_README.txt
            for details on how to create test users and test data, and which command-line 
            arguments to pass to the ArcSDE Provider UnitTest executable in order to 
            point to your test server and data.

            cd [FDO OpenSource]\Providers\ArcSDE\Src\UnitTest
            Run ..\..\bin\win32\Debug\UnitTest.exe [mandatory command-line arguments]

        GDAL:

            cd [FDO OpenSource]\Providers\GDAL\Src\UnitTest
            Run ..\..\Bin\Win32\Debug\UnitTest.exe

        MySql:

            cd [FDO OpenSource]\Providers\GenericRdbms\Src\UnitTest

            If you wish to run the MySql unit tests without specifying an 
            initialization file, update the default initialization file 
            "MySqlInit.txt" file with valid values for username, password 
            and service for the service against which the unit tests should 
            be executed and run "Dbg\UnitTestMySql.exe" with no additional 
            parameters.

            NOTE: Do not drop MySqlInit.txt in subversion if you choose to
            modify it

            If you wish to run the unit test and specify your own initialization 
            file, create your the file with valid values for username, password 
            and service and run the unit test by specifying the initialization 
            file on the command line when executing the unit tests.

            e.g. Dbg\UnitTestMySql.exe initfiletest=MySqlInitEx.txt

            NOTE: The initialization file must contain values for service, 
            username and password.

            e.g.: service=mysqlserver;username=root;password=xxxx;

        Odbc:

            cd [FDO OpenSource]\Providers\GenericRdbms\Src\UnitTest

            If you wish to run the ODBC unit tests without specifying 
            an initialization file, update the default initialization file 
            "OdbcInit.txt" file with valid values for username, password and 
            service for the services against which the unit tests should be  
            executed and run "Dbg\UnitTestOdbc.exe" with no additional parameters.
                
            NOTE: Do not drop OdbcInit.txt in subversion if you choose to modify it
                
            If you wish to run the unit test and specify your own initialization 
            file, create your the file with valid values for username, 
            password and the service names. Run the unit test by specifying the 
            initialization file on the command line when executing the unit tests. 
               
            e.g. Dbg\UnitTestOdbc.exe initfiletest=OdbcInitEx.txt
               
            NOTE: The initialization file must contain values for service, 
            username and password.
               
            The initialization file must contain service, username and password 
            for each server type DSN name can be specified using DSNOracle, 
            DSNMySql, DSNSqlServer

            e.g.: 

            serviceOracle=oraserver;usernameOracle=xxxx;passwordOracle=xxxx;DSNOracle=ORACLExxx;
            serviceMySql=mysqlserver;usernameMySql=root;passwordMySql=xxxx;DSNMySql=MySQLxxx;
            serviceSqlServer=sqlserver;usernameSqlServer=xxxx;passwordSqlServer=xxxx;DSNSqlServer=SqlServerxxx;

            NOTE: You can also run the unit tests separately for each ODBC subtype:

            MySql:       Dbg\UnitTestOdbc.exe OdbcMySqlTests
            Oracle:      Dbg\UnitTestOdbc.exe OdbcOracleTests
            SqlServer:   Dbg\UnitTestOdbc.exe OdbcSqlServerTests
            Access:      Dbg\UnitTestOdbc.exe OdbcAccessTests
            Excel:       Dbg\UnitTestOdbc.exe OdbcExcelTests
            Text:        Dbg\UnitTestOdbc.exe OdbcTextTests
                        
        Python Scripts:

            cd [FDO OpenSource]\Fdo\Python\UnitTest
            call UnitTests.cmd

-----------------------------
Linux Instructions
-----------------------------
 
I. Build Instructions

  1. cd [FDO OpenSource]

  2. Open and modify setenvironment.sh to specify the locations of the 
     Thirdparty SDK components and build tools. (Refer to the Build Notes 
     below)

  3.  Source the setenvironment.sh script (bash or bourne compatible shells)

      source ./setenvironment.sh

  4. ./build_thirdparty.sh

  5. ./build_linux.sh


  NOTE: build_linux.sh is a simple helper script that assists developers to 
        build the FDO components. These scripts supplement standard support of 
        configure and make. 

        The implementation of the above build_linux.sh script in essence 
        calls ...
 
        aclocal
        libtoolize --force
        automake --add-missing --copy
        autoconf
        ./configure
        make
        make install

        for the fdo core API components as well as the set of FDO providers 
        that are currently supported in the automated build process. Users are 
        free to call configure and make without using the build_linux.sh helper
        script.

II. Build Notes

  In preparation for building the Linux Open Source FDO Binaries

  1. Before building FDO and thirdparty libraries, the /usr/local
     directory must be set as writeable by the user, otherwise the user 
     will have to log in and build FDO as the ROOT user. This requirement
     is due to the fact that the FDO Libraries will be built and installed 
     in /usr/local/fdo-3.3.0/lib

  2. Ensure that the following FDO Environment Variables are set as follows:

       FDO = [FDO OpenSource]/Fdo
       FDOUTILITIES = [FDO OpenSource]/Providers/Utilities
       FDOTHIRDPARTY = [FDO OpenSource]/ThirdParty
 
  3. In order to build the ArcSDE Provider, you will need to have the
     ArcSDE 9.1 Client SDK installed, ArcSDE 9.2 Client SDK installed
     and the following environment variable set that points to the installation location:

       SDEHOME=[ArcSDE 9.2 developer components path] - used by unit test 

     In case user has ArcSDE 9.1 Client SDK and ArcSDE 9.2 Client SDK installed: 
     Copy the "include" and "lib" directory from the ArcSDE 9.1 Client SDK into: 
     $FDOTHIRDPARTY\ESRI\ArcSDEClient91\Linux
      
     Copy the "include" and "lib" directory from the ArcSDE 9.2 Client SDK into: 
     $FDOTHIRDPARTY\ESRI\ArcSDEClient92\Linux 
     
     The script will build the support for both ArcSDE clients. 
     In case user has only one version of the ArcSDE client installed, the location must be 
     specified using $SDEHOME 
     
     The ArcSDE unit test project will only search for include and lib files in $SDEHOME
     Note that ArcSDE is licensed software and must be obtained from an ESRI vendor.

  4. In order to build the MySQL Provider, you will need to download and 
     extract the MySQL client and MySQL 5.0 devloper components from MySQL. 
     These components are located at:
 
       http://dev.mysql.com/downloads/mysql/5.0.html
        
     For example, for Red Hat Enterprise Linux 3 RPM (x86)
       
       Client:                   MySQL-client-standard-5.0.22-0.rhel3.i386.rpm
       Headers and Libraries:    MySQL-devel-standard-5.0.22-0.rhel3.i386.rpm

     NOTE: Choosing the RPM install of the MySQL components is *strongly* 
     recommended. However, if a non-RPM installation of MySQL is chosen, 
     following the installation, set the following FDO environment variable:
  
       FDOMYSQL=[MySQL developer components path]
 
  5. In order to build the ODBC Provider, you will need to either 
     
     a) download and install the Linux ODBC driver Manager, specifically the 
        unixODBC-CORE and unixODBC-devel packages relased by www.unixodbc.org
 
           Home Page: 
           http://www.unixodbc.org/

           Download Location: 
           http://sourceforge.net/project/showfiles.php?group_id=1544

           Download Files: 
           unixODBC-CORE  - unixODBC-2.2.11-1.i386.rpm
           unixODBC-devel - unixODBC-devel-2.2.11-1.i386.rpm

           Such an installation will install the unixODBC driver manager libraries 
           and header files in

           usr/lib and usr/include

           The FDO ODBC provider build will automatically locate thee files in 
           this liocation.

     b) install an unixODBC driver such as the EasySofy Oracle ODBC driver. 
        This installation will include the driver libraries as well as the 
        unixODBC manager libraries. 

        If such an installation occurs the manager libraries and headers will 
        not be installed under /usr/lib or usr/include but will be installed 
        in a custom location as specified by the driver RPM package. If this 
        is the case, you will need to set the following FDO environment 
        variable to point to the location of the unixODBC components:

           FDOODBC=[unixODBC developer components path]

  6. In order to build the King Oracle Provider, you will need to download 
     and install the 'Oracle 10.2.0.3 Instant Client' for Linux from:

        http://www.oracle.com/technology/software/tech/oci/instantclient/htdocs/linuxsoft.html

     The name of the installation files are:

         'oracle-instantclient-basic-10.2.0.3-1.i386.rpm'
         'oracle-instantclient-devel-10.2.0.3-1.i386.rpm'
        
     Install the files to the default installation location in /usr/lib and usr/include

 
  7. OPTIONAL: The FDO build process allows it's documentation to be 
     regenerated.  If this is required, install the OpenSource Doxygen 
     documentation generation software.  Doxygen is a documentation system for 
     C++, C, Java, Objective-C, Python, IDL (Corba and Microsoft flavors) and 
     to some extent PHP, C#, and D.  Doxygen is often automataically installed 
     on Fedora Core and RedHat systems.
  
     To install Doxygen, refer to: http://www.stack.nl/~dimitri/doxygen/
     
     The FDO Doxygen documentation generation process will also use the "dot" 
     tool from graphviz 1.5 to generate more advanced diagrams and graphs. 
     Graphviz is an "open-sourced", cross-platform graph drawing toolkit from 
     AT&T and Lucent Bell Labs.
     
     To install graphviz, refer to: http://www.graphviz.org/ 

  8. OPTIONAL: The FDO build process supports the generation of a set of python 
     wrapper scripts for the FDO API. The python scripts are generated using 
     SWIG. To generate the python scripts, you must have Python and SWIG installed.
  
     To install SWIG, refer to: http://www.swig.org/
     To install Python, refer to: http://www.python.org/
     
     The FDO python scripts have been tested using SWIG 1.3.31 and Python 2.4.
     It is recommended that both SWIG and Python be installed in the /usr 
     directory.
     
     Once SWIG and python have been installed, set the PYTHON_LIB_PATH and 
     PYTHON_INCLUDE_PATH environment variables to point to the locations of
     the Python Include and Library files.
     
        e.g.
          
        export PYTHON_INCLUDE_PATH=/usr/include/python2.4
        export PYTHON_LIB_PATH=/usr/lib/python2.4
        
     ** These variables are maintained in the setenvironment.sh script. 
     
     In order to build the FDO Python Wrappers, build and install the FDO 
     libraries. Once the install is complete, run the 
     [FDO OpenSource]/Fdo/Python/build_linux.sh script. 
     The Python components will be installed in /usr/local/fdo-3.3.0/lib

  9. NOTE: To run the unit test, you must set LD_LIBRARY_PATH as follows:

       export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/fdo-3.3.0/lib
       export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$SDEHOME/lib:$FDOTHIRDPARTY/ESRI/ArcSDEClient91/Linux/lib
 
 10. NOTE: Several known problems exist if auto-mounted drives are used as 
     locations for the FDO Linux builds. It is recommended that users not use 
     auto-mounted drives when building FDO.

 11. NOTE: The FDO expression scripts (.y) are not automatially recompiled as a
     part of the FDO build process. In order to recompile the .y files into 
     their .cpp and .h counterparts, execute the build_parse.sh script.

 12. NOTE: Use the [FDO OpenSource]\setenvironment.sh script to assist in 
     setting and maintaing the correct environment settings for the FDO build 
     process.  This script can be modifed and used to set the correct 
     environment variables and PATH settings.

 13. The build_thirdparty.sh script is used to build the FDO Thirdparty 
     open source components used by the FDO API and the FDO providers.  
     The following is a general guideline on how to use the 
     build_thirdparty.sh build script.

            *******************************************************
            build_thirdparty.sh [--h]
                                [--a Action]
                                [--w WithModule]
                                [--m ConfigMakefiles]
                 
            Help:            --h[elp]
            Action:          --a[ction] buildinstall(default),
                                        build, 
                                        install,
                                        uninstall,
                                        clean
            WithModule:      --w[ith] all(default),
                                      fdo,
                                      sdf,
                                      wms,
                                      wfs,
                                      gdal
            ConfigMakefiles: --m[akefile] configure(default), 
                                          noconfigure
            *******************************************************

     
            e.g.

            **** Display help for build_thirdparty.sh 
            build_thirdparty --h
            **** Configure, Build and Install all Thirdparty components
            build_thirdparty
            **** Configure, Build and Install Thirdparty components
            **** related to the FDO API
            build_thirdparty --w fdo
            **** Build and Install Thirdparty components related to 
            **** the FDO API without running the configure script
            build_thirdparty --w fdo --m noconfigure
            **** Configure, Build and Install Thirdparty components
            **** related to the SDF and gdal providers
            build_thirdparty --w sdf --w gdal --m noconfigure
            **** Uninstall all Thirdparty components
            build_thirdparty --a uninstall
            **** Clean up all Thirdparty components
            build_thirdparty --a clean


 13. The build_linux.sh script is used to build and install the FDO OpenSource 
     libraries. The following is a general guideline on how to use 
     build_linux.sh

           **************************************************************
           build_linux.sh [--h] 
                          [--c BuildType] 
                          [--a Action] 
                          [--w WithModule] 
                          [--d BuildDocs] 
                          [--m ConfigMakefiles]

           Help:            --h[elp]
           BuildType:       --c[onfig] release(default), debug
           Action:          --a[ction] buildinstall(default), 
                                       build, 
                                       install, 
                                       uninstall, 
                                       clean
           BuildDocs:       --d[ocs] skip(default), build
           ConfigMakefiles: --m[akefile] configure(default), noconfigure
           WithModule:      --w[ith] all(default), 
                                     fdocore, 
                                     fdo, 
                                     thirdparty, 
                                     providers, 
                                     shp, 
                                     sdf, 
                                     wfs, 
                                     wms, 
                                     arcsde, 
                                     rdbms,
                                     gdal,
                                     ogr.
                                     postgis
                                     kingoracle
           **************************************************************

           e.g.

            **** Display help for build_linux.sh
            build_linux.sh --h
            **** Configure, Build and Install all components
            build_linux.sh
            **** Configure and Build all components
            build_linux.sh --a build
            **** Build all components without running configure
            build_linux.sh --a build --m noconfigure
            **** Configure and Install all components
            build_linux.sh --a install
            **** Configure and Build only the FDO library
            build_linux.sh --a build -w fdo
            **** Configure and Build all FDO Providers
            build_linux.sh --a build --w providers
            **** Configure and Build only the SHP Provider
            build_linux.sh --a build --w shp
            **** Configure and Build only the SDF Provider
            build_linux.sh --a build --w sdf
            **** Configure and Build both the WMS and WFS Providers
            build_linux.sh --a build --w wms --w wfs


III. Running the FDO Unit Tests

    The FDO API and the FDO providers come with unit tests that will allow the  
    various products to be tested once the build is complete. In order to run 
    the FDO API, WMS, SHP, SDF, ArcSDE, GDAL, MySql and Odbc Provider Unit 
    Tests, follow the following instructions once the build has completed
        
        FDO:

            cd [FDO OpenSource]/Fdo/UnitTest
            ./UnitTest

        WMS:

            cd [FDO OpenSource]/Providers/WMS/Src/UnitTest
            ./UnitTest

        SDF:

            cd [FDO OpenSource]/Providers/SDF/Src/UnitTest
            ./UnitTest
 
        SHP:

            cd [FDO OpenSource]/Providers/SHP/Src/UnitTest
            ./UnitTest
 
        ArcSDE:

            Please refer to [FDO OpenSource]/Providers/ArcSDE/UnitTest_README.txt
            for details on how to create test users and test data, and which 
            command-line arguments to pass to the ArcSDE Provider UnitTest 
            executable in order to point to your test server and data.

            cd [FDO OpenSource]/Providers/ArcSDE/Src/UnitTest
            ./UnitTest [mandatory command-line arguments] 
        
            e.g. 

            ./UnitTest server=somesite.com port_multi=5151/tcp port_single=5151/tcp sdepwd=test dataset=FC4 rdbms=ORACLE

        GDAL:

            cd [FDO OpenSource]/Providers/GDAL/Src/UnitTest
            ./UnitTest
          
        MySql:

            cd [FDO OpenSource]/Providers/GenericRdbms/Src/UnitTest

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

            cd [FDO OpenSource]/Providers/GenericRdbms/Src/UnitTest

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

        Python Scripts:
    
            cd [FDO OpenSource]/Fdo/Python/UnitTest
            ./UnitTests.sh

[end]
