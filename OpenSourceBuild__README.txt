Feature Data Objects (FDO) OpenSource README
============================================

This ReadMe contains important information required to build FDO open source.
Ensure you read through this document before building.

Contents:
Windows instructions
   - Windows Notes
   - Windows build steps
Linux RedHat 3.0 instructions
   - Linux Notes
   - Linux build steps


-----------------------------

Windows Instructions
-----------------------------

I. Windows Notes:

  1. Ensure that the Windows Environment Variables are set as follows:

   FDO = <FDO OpenSource Location>\Fdo (e.g. D:\OpenSource\Fdo)

   FDOUTILITIES = <FDO OpenSource Location>\Utilities (e.g. D:\OpenSource\Providers\Utilities)

   FDOTHIRDPARTY = <FDO OpenSource Location>\ThirdParty folder (e.g. D:\OpenSource\Thridparty)

   XALANROOT=%FDOTHIRDPARTY%\apache\xml-xalan\c

   XERCESCROOT=%FDOTHIRDPARTY%\apache\xml-xerces\c

   NLSDIR=%XALANROOT%\Src\xalanc\NLS

  2. Ensure that the MSVC Variables are set through the VisualStudio .Net 
     User Interface (Tools>Options>Properties and Solutions>VC++ Directories):

      Add to MSVC Include Paths

          $(XERCESCROOT)\src

      Add to MSVC Library Paths

          $(XERCESCROOT)\Build\Win32\VC8\Release

          $(XERCESCROOT)\Build\Win32\VC8\Debug

      Add to MSVC Executable Paths

          $(XERCESCROOT)\Build\Win32\VC8\Release

          $(XERCESCROOT)\Build\Win32\VC8\Debug

   3. Download and install the GNU Win32 Bison (version 1.875) 
      and Sed (version 4.1.4) self-extracting utilities

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

   
      FDO has been tested using Bison 1.875 and Sed 4.1.4
      The Bison and Sed files MUST be installed in the same location 
      e.g.  C:\Program Files\GnuWin32

   4. OPTIONAL: The FDO build process allows it's documentation to be regenerated. 
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


   5. Ensure that the tools that the FDO build process use are in the build 
      environment's PATH.

        MsBuild    -- e.g. C:\WINDOWS\Microsoft.NET\Framework\v2.0.50727
        devenv     -- e.g. C:\Program Files\Microsoft Visual Studio 8\Common7\IDE
        Doxygen    -- e.g. C:\Program Files\doxygen\bin
        Dot        -- e.g. C:\Program Files\ATT\Graphviz\bin
        Sed/Bison  -- e.g. C:\Program Files\GnuWin32

        e.g.
        
        set PATH=%PATH%;C:\WINDOWS\Microsoft.NET\Framework\v2.0.50727
        set PATH=%PATH%;C:\Program Files\Microsoft Visual Studio 8\Common7\IDE
        set PATH=%PATH%;C:\Program Files\doxygen\bin
        set PATH=%PATH%;C:\Program Files\ATT\Graphviz\bin
        set PATH=%PATH%;C:\Program Files\GnuWin32\bin


   6. The FDO OpenSource Distribution files contains a providers.xml file.
      The providers.xml file is used by FDO to identify where FDO provider
      binaries are installed. 

      The providers.xml file included in the FDO OpenSource distribution 
      has been generated for a Linux install environment and contains 
      references to Linux .so files. 

      In order for the providers.xml file to be used on a Windows environment
      the names and path locations of the provider libraries must be changed
      to be windows specific. This will be done as a part of the Windows build 
      process. 

      This can also be done manually according to the following template:

      <?xml version="1.0" encoding="UTF-8" standalone="no" ?>
      <FeatureProviderRegistry>
        <FeatureProvider>
          <Name>OSGeo.SDF.3.2</Name>
          <DisplayName>OSGeo FDO Provider for SDF</DisplayName>
          <Description>Read/write access to Autodesk's spatial database format, 
          a file-based personal geodatabase that supports multiple features/attributes, 
          spatial indexing, and file-locking.</Description>
          <IsManaged>False</IsManaged>
          <Version>3.2.0.0</Version>
          <FeatureDataObjectsVersion>3.2.0.0</FeatureDataObjectsVersion>
          <LibraryPath>.\SDFProvider.dll</LibraryPath>
        </FeatureProvider>
        <FeatureProvider>
          <Name>OSGeo.SHP.3.2</Name>
          <DisplayName>OSGeo FDO Provider for SHP</DisplayName>
          <Description>Read/write access to spatial and attribute data in an ESRI SHP file.</Description>
          <IsManaged>False</IsManaged>
          <Version>3.2.0.0</Version>
          <FeatureDataObjectsVersion>3.2.0.0</FeatureDataObjectsVersion>
          <LibraryPath>.\SHPProvider.dll</LibraryPath>
        </FeatureProvider>
        <FeatureProvider>
          <Name>OSGeo.ArcSDE.3.2</Name>
          <DisplayName>OSGeo FDO Provider for ArcSDE</DisplayName>
          <Description>Read/write access to an ESRI ArcSDE-based data store, using Oracle and SQL Server.</Description>
          <IsManaged>False</IsManaged>
          <Version>3.2.0.0</Version>
          <FeatureDataObjectsVersion>3.2.0.0</FeatureDataObjectsVersion>
          <LibraryPath>.\ArcSDEProvider.dll</LibraryPath>
        </FeatureProvider>
        <FeatureProvider>
          <Name>OSGeo.WFS.3.2</Name>
          <DisplayName>OSGeo FDO Provider for WFS</DisplayName>
          <Description>Read access to OGC WFS-based data store.</Description>
          <IsManaged>False</IsManaged>
          <Version>3.2.0.0</Version>
          <FeatureDataObjectsVersion>3.2.0.0</FeatureDataObjectsVersion>
          <LibraryPath>.\WFSProvider.dll</LibraryPath>
        </FeatureProvider>
        <FeatureProvider>
          <Name>OSGeo.WMS.3.2</Name>
          <DisplayName>OSGeo FDO Provider for WMS</DisplayName>
          <Description>Read access to OGC WMS-based data store.</Description>
          <IsManaged>False</IsManaged>
          <Version>3.2.0.0</Version>
          <FeatureDataObjectsVersion>3.2.0.0</FeatureDataObjectsVersion>
          <LibraryPath>.\WMSProvider.dll</LibraryPath>
        </FeatureProvider>
        <FeatureProvider>
          <Name>OSGeo.ODBC.3.2</Name> 
          <DisplayName>OSGeo FDO Provider for ODBC</DisplayName> 
          <Description>Autodesk FDO Provider for ODBC</Description> 
          <IsManaged>False</IsManaged> 
          <Version>3.2.0.0</Version> 
          <FeatureDataObjectsVersion>3.2.0.0</FeatureDataObjectsVersion>
          <LibraryPath>.\ODBCProvider.dll</LibraryPath>
        </FeatureProvider>
        <FeatureProvider>
          <Name>OSGeo.MySQL.3.2</Name> 
          <DisplayName>OSGeo FDO Provider for MySQL</DisplayName> 
          <Description>FDO Provider for MySql</Description>
          <IsManaged>False</IsManaged>
          <Version>3.2.0.0</Version>
          <FeatureDataObjectsVersion>3.2.0.0</FeatureDataObjectsVersion>
          <LibraryPath>.\MySQLProvider.dll</LibraryPath>
        </FeatureProvider>
      </FeatureProviderRegistry>

     On Windows the providers.xml file will need to be located where FDO.dll is located. 


   8. In order to build the ArcSDE Provider, you will need to have the
      ArcSDE 9.1 Client SDK installed and the following environment variable set that 
      points to the installation location:

        set SDEHOME==<ArcSDE developer components path> 

      Note that ArcSDE is licensed software and must be obtained from an
      ESRI vendor.


   9. In order to build the MySQL Provider, you will need to download 
   and install the MySQL client (Windows (x86) version 5.0), including
   the developer components.

        Home Page: 
        http://dev.mysql.com/

        Download Location: 
	http://dev.mysql.com/get/Downloads/MySQL-5.0/mysql-standard-5.0.21-linux-i686-glibc23.tar.gz/from/http://mysql.mirrors.pair.com/

   Following the installation, set the following FDO environment variable:

        set FDOMYSQL=<MySQL developer components path> 

   e.g. \Program Files\MySQL\MySQL Server 5.0


II. Windows steps for open source build: 

  1.  Extract the FDO OpenSource files using either gunzip/tar or Winzip.

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

  2.   Use the build.bat file to build the FDO OpenSource binaries. The following 
       is a general guideline on how to use build.bat
  
       **************************************************************************
       build.bat [-h] [-o=OutFolder] [-c=BuildType] [-a=Action] [-w=WithModule] [-d=BuildDocs]
       *
       Help:        -h[elp]
       OutFolder:   -o[utpath]=destination folder for binaries
       BuildType:   -c[onfig]=release(default), debug
       Action:      -a[ction]=buildinstall(default), buildonly, installonly
       WithModule:  -w[ith]=all(default), thirdparty, fdo, providers, shp, sdf, wfs, wms, arcsde, odbc, mysql
       BuildDocs:   -d[ocs]=skip(default), build
       **************************************************************************

            e.g.

            **** Display help for build.bat
            build -h
            **** ReBuild all components excluding documentation
            build -a=buildonly
            **** ReBuild all components excluding documentation 
            **** and install output files to C:\Fdo
            build -o=C:\Fdo
            **** ReBuild all components and documentation
            **** and installoutput and documentation files to C:\Fdo
            build -o=C:\Fdo -d=build
            **** ReBuild FDO library
            build -a=buildonly -w=fdo
            **** ReBuild all FDO Providers
            build -a=buildonly -w=providers
            **** ReBuild FDO Shape Provider
            build -a=buildonly -w=shp
            **** ReBuild FDO SDF Provider
            build -a=buildonly -w=sdf
            **** Build only documentation and install files to C:\Fdo
            build -o=C:\Fdo -d=build -a=installonly

 3.   In order to run the SHP and SDF Provider Unit Tests

        SDF:

            cd <FDO OpenSource Location>\Providers\SDF\Src\UnitTest
            ..\..\bin\win32\Debug\UnitTest
 
        SHP:

            cd <FDO OpenSource Location>\Providers\SHP\Src\UnitTest
            ..\..\bin\win32\Debug\UnitTest


-----------------------------

Linux Redhat 3.0 Instructions
-----------------------------
 
I. Linux Notes:

  1. REQUIRED: Before building FDO and thirdparty libraries, the following 
     directories must be set as writeable by the user, otherwise the user 
     will have to switch to root.

      /usr/local
  
  2.  FDO Libraries will be installed under /usr/local/fdo-3.2.0/lib

  3. Ensure that the FDO Environment Variables are set as follows:

      FDO = <FDO OpenSource Location>/Fdo (e.g. /home/OpenSource/Fdo)

      FDOUTILITIES = <FDO OpenSource Location>/Providers/Utilities (e.g. /home/OpenSource/Providers/Utilities)

      FDOTHIRDPARTY = <FDO OpenSource Location>/ThirdParty (e.g. /home/OpenSource/Thridparty)

 
  4.  To run the unit test, you must set LD_LIBRARY_PATH as follows:

       $ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/fdo-3.2.0/lib:$FDOTHIRDPARTY/linux/cppunit/lib
 
  5.  providers.xml will be installed to /usr/local/fdo-3.2.0/lib.
 
  6.  Several known problems exist if auto-mounted drives are used as locations for the
      FDO Linux builds. It is recommended that users not use auto-mounted drives 
      when building FDO.

      An Example of the auto-mounted build issue:

      If the 'make' process is performed by a non-root user, directories and files 
      will created according to the users permissions. 

      When the user logs in as root and runs 'make install' a problem similar to 
      the following may occur:

          mv: cannot move `libSHPProvider.so.3.2.0' to 
         `libSHPProvider.so.3.2.0': Permission denied.
          libtool: install: error: relink `libSHPProvider.la' 
          with the above command

      mv fails because root has no privilege to mv files on auto-mounted directories 
      created during the make process by another user.

  7.  In order to build the ArcSDE Provider, you will need to have the
      ArcSDE 9.1 Client SDK installed at one of the following location:

        $SDEHOME/

      Note that ArcSDE is licensed software and must be obtained from an
      ESRI vendor.

   8. In order to build the MySQL Provider, you will need to download 
      and extract the Linux (non RPM package - Linux (x86)  Standard verion 5.0)
      files using either gunzip/tar.
 
        Home Page: 
        http://dev.mysql.com/
        
        Download Location: 
 	http://dev.mysql.com/get/Downloads/MySQL-5.0/mysql-standard-5.0.21-linux-i686-glibc23.tar.gz/from/http://mysql.mirrors.pair.com/
 
      Following the installation, set the following FDO environment variable:
  
        FDOMYSQL=<MySQL developer components path>
 
      e.g. /home/mysql-5.0

   10. In order to build the ODBC Provider, you will need to either 

       a) download and install unixODBC-CORE and unixODBC-devel
 
           Home Page: 
           http://www.unixodbc.org/

           Download Location: 
           http://sourceforge.net/project/showfiles.php?group_id=1544

           Download Files: 
           unixODBC-CORE  - unixODBC-2.2.11-1.i386.rpm
           unixODBC-devel - unixODBC-devel-2.2.11-1.i386.rpm

           Such an installation will install the unixODBC driver manager libraries 
           and header files  in

           usr/lib and usr/include

           The FDO ODBC provider build will automatically locate thee files in this liocation.

       b) install an unixODBC driver such as the EasySofy Oracle ODBC driver. This installation 
          will include the driver libraries as well as the unixODBC manager libraries. 
          If such an installation occurs the manager libraries and headers will not be 
          installed under /usr/lib or usr/include but will be installed in a custom location
          as specified by the driver RPM package. If this is the case, you will need to set 
          the following FDO environment variable to point to the location of the unixODBC 
          components

           FDOODBC=<unixODBC developer components path>

II. Linux steps for open source build:

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
       echo "build_linux.sh [--h] [-c BuildType] [-a Action] [--w WithModule] "
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
