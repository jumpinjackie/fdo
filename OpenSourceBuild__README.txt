Feature Data Objects (FDO) OpenSource README
============================================

This ReadMe contains important information required to build FDO open source.
Ensure you read through this document before building.

Contents:
Windows Instructions
   - Windows Notes
   - Windows Build Instructions
Linux Instructions
   - Linux Notes
   - Linux Build Instructions

-----------------------------
Windows Instructions
-----------------------------

I. Windows Notes: 

  In preparation for building the Windows Open Source FDO Binaries

  1. Ensure that the Windows Environment Variables are set as follows:

        SET FDO=<FDO OpenSource>\Fdo
        SET FDOUTILITIES=<FDO OpenSource>\Utilities
        SET FDOTHIRDPARTY=<FDO OpenSource>\ThirdParty
        SET XALANROOT=%FDOTHIRDPARTY%\apache\xml-xalan\c
        SET XERCESCROOT=%FDOTHIRDPARTY%\apache\xml-xerces\c
        SET NLSDIR=%XALANROOT%\Src\xalanc\NLS

  2. In order to build The FDO Thirdparty OpenSSL components, be sure to install ActiveState Perl, 
     available from:
     
        http://www.activestate.com/ActivePerl

  3. In order to build all FDO Windows components, ensure that the Microsoft
     MsBuild tool that is used by the the FDO build process is included in the Windows system PATH.

        set PATH=%PATH%;C:\WINDOWS\Microsoft.NET\Framework\v2.0.50727
 
  4. In order to build the ArcSDE Provider, you will need to have the
     ArcSDE 9.1 Client SDK installed and the following environment variable set that 
     points to the installation location:

        set SDEHOME=<ArcSDE developer components path> 

     Note that ArcSDE is licensed software and must be obtained from an ESRI vendor.

  5. In order to build the MySQL Provider, you will need to download 
     and install the MySQL 5.0 Windows client and developer components from:

        http://dev.mysql.com/downloads/mysql/5.0.html

     Following the installation, set the following FDO Windows environment variable:

        set FDOMYSQL=<MySQL developer components path> 

     e.g. set FDOMYSQL=c:\Program Files\MySQL\MySQL Server 5.0

  6. OPTIONAL: The FDO build includes several generated .cpp files that were 
     generated from source .y files by the Bision and Sed utilities. These 
     files are fairly static therefore they are not automatically regenerated 
     as a part of the standard FDO build process. If changes are made 
     to the .y files they will need to be recompiled into their respective .cpp and 
     .h files. 

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
 
  7. OPTIONAL: The FDO build process allows it's documentation to be regenerated. 
     If this is required, install the OpenSource Doxygen documentation generation
     software.  Doxygen is a documentation system for C++, C, Java, Objective-C, 
     Python, IDL (Corba and Microsoft flavors) and to some extent PHP, C#, and D.
  
     To install Doxygen, refer to: http://www.stack.nl/~dimitri/doxygen/
     
     The FDO Doxygen documentation generation process will also use the "dot" tool  
     from graphviz 1.5 to generate more advanced diagrams and graphs. Graphviz is an 
     "open-sourced", cross-platform graph drawing toolkit from AT&T and Lucent 
     Bell Labs.
     
     To install graphviz, refer to: http://www.research.att.com/sw/tools/graphviz/ 

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
 
II. Windows Build Instructions:

  1. Extract the FDO OpenSource files using either gunzip/tar or Winzip.

       Use gunzip and tar utilities as follows:

            gunzip fdo-3.2.0.tar.gz
            tar -xvf fdo-3.2.0.tar

            gunzip fdosdf-3.2.0.tar.gz
            tar -xvf fdosdf-3.2.0.tar

        etc...

       - or -

       Use Windows Winzip utility as follows:

            WINZIP32.EXE fdo-3.2.0.tar.gz
            WINZIP32.EXE fdosdf-3.2.0.tar.gz
        etc...

  2. Use the build_thirdparty.bat file to build the FDO Thirdparty binaries. The following 
     is a general guideline on how to use the build_thirdparty.bat build script.
  
           **************************************************************************
           build_thirdparty.bat [-h] [-o=OutFolder] [-c=BuildType] [-a=Action] [-w=WithModule]

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
            build -o=C:\Fdo -a=buildinstall
            **** Build all Thirdparty components and install files to C:\Fdo
            build_thirdparty -o=C:\Fdo -a=buildinstall -d=build
            **** Build Thirdparty components for FDO libraries
            build_thirdparty -w=fdo
            **** Build Thirdparty components for all FDO Providers
            build_thirdparty -w=providers
            **** Build Thirdparty components for for SDF Provider
            build_thirdparty -w=sdf
            **** Build Thirdparty components for for WMS Provider
            build_thirdparty -w=wms


  3. Use the build.bat file to build the FDO binaries. The following 
     is a general guideline on how to use the build.bat build script.
  
           **************************************************************************
           build.bat [-h] [-o=OutFolder] [-c=BuildType] [-a=Action] [-w=WithModule] [-d=BuildDocs]

           Help:           -h[elp]
           OutFolder:      -o[utpath]=destination folder for binaries '
           BuildType:      -c[onfig]=release(default), debug
           Action:         -a[ction]=build(default), buildinstall, install, clean
           WithModule:     -w[ith]=all(default), fdo, providers, shp, sdf, wfs, wms, arcsde, odbc, mysql
           BuildDocs:      -d[ocs]=skip(default), build
           **************************************************************************
            
            e.g.

            **** Display help for build.bat
            build -h
            **** Build all components excluding documentation
            build
            **** Build all components and install output files to C:\Fdo
            build -o=C:\Fdo -a=buildinstall
            **** Build all components and documentation and install files to C:\Fdo
            build -o=C:\Fdo -a=buildinstall -d=build
            **** Build only FDO libraries
            build -w=fdo
            **** Build all FDO Providers
            build -w=providers
            **** Build FDO Shape Provider
            build -w=shp
            **** Build FDO SDF Provider
            build -w=sdf
            **** Build documentation and install files to C:\Fdo
            build -o=C:\Fdo -d=build -a=install

   4. In order to run the SHP and SDF Provider Unit Tests

        SDF:

            cd <FDO OpenSource Location>\Providers\SDF\Src\UnitTest
            ..\..\bin\win32\Debug\UnitTest
 
        SHP:

            cd <FDO OpenSource Location>\Providers\SHP\Src\UnitTest
            ..\..\bin\win32\Debug\UnitTest


-----------------------------
Linux Instructions
-----------------------------
 
I. Linux Notes:

  In preparation for building the Linux Open Source FDO Binaries

  1. Before building FDO and thirdparty libraries, the /usr/local
     directory must be set as writeable by the user, otherwise the user 
     will have to log in and build FDO as the ROOT user. This requirement
     is due to the fact that the FDO Libraries will be built and installed 
     in /usr/local/fdo-3.2.0/lib

  2. Ensure that the following FDO Environment Variables are set as follows:

       FDO = <FDO OpenSource>/Fdo
       FDOUTILITIES = <FDO OpenSource>/Providers/Utilities
       FDOTHIRDPARTY = <FDO OpenSource>/ThirdParty
 
  3. In order to build the ArcSDE Provider, you will need to have the
     ArcSDE 9.1 Client SDK installed and the following environment variable set that 
     points to the installation location:

       SDEHOME=<ArcSDE developer components path> 

     Note that ArcSDE is licensed software and must be obtained from an ESRI vendor.

  4. In order to build the MySQL Provider, you will need to download and extract 
     the MySQL client and MySQL 5.0 devloper components from MySQL. These components are 
     located at:
 
       http://dev.mysql.com/downloads/mysql/5.0.html
        
     For example, for Red Hat Enterprise Linux 3 RPM (x86)
       
       Client:                      MySQL-client-standard-5.0.22-0.rhel3.i386.rpm
       Headers and Libraries:       MySQL-devel-standard-5.0.22-0.rhel3.i386.rpm

     NOTE: Choosing the RPM install of the MySQL components is *strongly* recommended. 
     However, if a non-RPM installation of MySQL is chosen, following the installation, 
     set the following FDO environment variable:
  
       FDOMYSQL=<MySQL developer components path>
 
  5. In order to build the ODBC Provider, you will need to either 
     
     a) download and install the Linux ODBC driver Manager, specifically 
        the unixODBC-CORE and unixODBC-devel packages relased by www.unixodbc.org
 
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

           The FDO ODBC provider build will automatically locate thee files in this liocation.

     b) install an unixODBC driver such as the EasySofy Oracle ODBC driver. This installation 
        will include the driver libraries as well as the unixODBC manager libraries. 
        If such an installation occurs the manager libraries and headers will not be 
        installed under /usr/lib or usr/include but will be installed in a custom location
        as specified by the driver RPM package. If this is the case, you will need to set 
        the following FDO environment variable to point to the location of the unixODBC 
        components:

           FDOODBC=<unixODBC developer components path>

  6. NOTE: To run the unit test, you must set LD_LIBRARY_PATH as follows:

       export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/fdo-3.2.0/lib:$FDOTHIRDPARTY/linux/cppunit/lib
 
  7. NOTE: Several known problems exist if auto-mounted drives are used as locations 
     for the FDO Linux builds. It is recommended that users not use auto-mounted drives 
     when building FDO.


II. Linux Build Instructions:

   1.  Extract the FDO OpenSource files using gunzip/tar.

       Use gunzip and tar utilities as follows:

            gunzip fdo-3.2.0.tar.gz
            tar -xvf fdo-3.2.0.tar

            gunzip fdosdf-3.2.0.tar.gz
            tar -xvf fdosdf-3.2.0.tar

        etc...

  2.   cd <FDO OpenSource Location>

  3.   Use the build_linux.sh script to build and install the FDO OpenSource libraries. 
       The following is a general guideline on how to use build_linux.sh

       echo "**************************************************************************"
       echo "build_linux.sh [--h] [-c BuildType] [--a Action] [--w WithModule] "
       echo "*"
       echo "Help:       --h[elp]"
       echo "BuildType:  --c[onfig] release(default), debug"
       echo "Action:     --a[ction] buildinstall(default), buildonly, installonly, configure"
       echo "WithModule: --w[ith] fdocore(default), fdo, thirdparty, providers, shp, sdf, wfs, wms, arcsde, rdbms"
       echo "**************************************************************************"

           e.g.

            **** Display help for build.bat
            build_linux.sh --h
            **** ReBuild and Install all components without running configure
            build_linux.sh
            **** Configure and ReBuild all components
            build_linux.sh --a buildonly --a configure
            **** Configure, ReBuild and Install all components
            build_linux.sh --a buildinstall --a configure
            **** ReBuild all components
            build_linux.sh --a buildonly
            **** Install all components
            build_linux.sh --a installonly
            **** ReBuild FDO library
            build_linux.sh --a buildonly -w fdo
            **** ReBuild all FDO Providers
            build_linux.sh --a buildonly --w providers
            **** ReBuild FDO Shape Provider
            build_linux.sh --a buildonly --w shp
            **** ReBuild FDO SDF Provider
            build_linux.sh --a buildonly --w sdf


 4.   In order to run the SHP and SDF Provider Unit Tests:

        SDF:

            cd <FDO OpenSource Location>/Providers/SDF/Src/UnitTest
            ./UnitTest
 
        SHP:

            cd <FDO OpenSource Location>/Providers/SHP/Src/UnitTest
            ./UnitTest
 
<end>
