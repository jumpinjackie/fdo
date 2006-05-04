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

   FDOUTILITIES = <FDO OpenSource Location>\Providers\Utilities (e.g. D:\OpenSource\Providers\Utilities)

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

      The Bison and Sed files MUST be extracted to the following FDO build location: 

        %FDOTHIRDPARTY%\util\GnuWin32

      The FDO build expects the following list of files:

        thirdparty/util/GnuWin32/bin/bison.exe
        thirdparty/util/GnuWin32/bin/libiconv2.dll
        thirdparty/util/GnuWin32/bin/libintl3.dll
        thirdparty/util/GnuWin32/bin/m4.exe
        thirdparty/util/GnuWin32/bin/sed.exe
        thirdparty/util/GnuWin32/share/bison/c.m4
        thirdparty/util/GnuWin32/share/bison/glr.c
        thirdparty/util/GnuWin32/share/bison/lalr1.cc
        thirdparty/util/GnuWin32/share/bison/m4sugar/m4sugar.m4
        thirdparty/util/GnuWin32/share/bison/yacc.c
        thirdparty/util/GnuWin32/share/locale/af/LC_MESSAGES/sed.mo
        thirdparty/util/GnuWin32/share/locale/ca/LC_MESSAGES/sed.mo
        thirdparty/util/GnuWin32/share/locale/cs/LC_MESSAGES/sed.mo
        thirdparty/util/GnuWin32/share/locale/da/LC_MESSAGES/bison.mo
        thirdparty/util/GnuWin32/share/locale/da/LC_MESSAGES/sed.mo
        thirdparty/util/GnuWin32/share/locale/de/LC_MESSAGES/bison.mo
        thirdparty/util/GnuWin32/share/locale/de/LC_MESSAGES/sed.mo
        thirdparty/util/GnuWin32/share/locale/el/LC_MESSAGES/sed.mo
        thirdparty/util/GnuWin32/share/locale/eo/LC_MESSAGES/sed.mo
        thirdparty/util/GnuWin32/share/locale/es/LC_MESSAGES/bison.mo
        thirdparty/util/GnuWin32/share/locale/es/LC_MESSAGES/sed.mo
        thirdparty/util/GnuWin32/share/locale/et/LC_MESSAGES/bison.mo
        thirdparty/util/GnuWin32/share/locale/et/LC_MESSAGES/sed.mo
        thirdparty/util/GnuWin32/share/locale/fi/LC_MESSAGES/sed.mo
        thirdparty/util/GnuWin32/share/locale/fr/LC_MESSAGES/bison.mo
        thirdparty/util/GnuWin32/share/locale/fr/LC_MESSAGES/sed.mo
        thirdparty/util/GnuWin32/share/locale/ga/LC_MESSAGES/sed.mo
        thirdparty/util/GnuWin32/share/locale/gl/LC_MESSAGES/sed.mo
        thirdparty/util/GnuWin32/share/locale/he/LC_MESSAGES/sed.mo
        thirdparty/util/GnuWin32/share/locale/hr/LC_MESSAGES/bison.mo
        thirdparty/util/GnuWin32/share/locale/hr/LC_MESSAGES/sed.mo
        thirdparty/util/GnuWin32/share/locale/hu/LC_MESSAGES/sed.mo
        thirdparty/util/GnuWin32/share/locale/id/LC_MESSAGES/bison.mo
        thirdparty/util/GnuWin32/share/locale/id/LC_MESSAGES/sed.mo
        thirdparty/util/GnuWin32/share/locale/it/LC_MESSAGES/bison.mo
        thirdparty/util/GnuWin32/share/locale/it/LC_MESSAGES/sed.mo
        thirdparty/util/GnuWin32/share/locale/ja/LC_MESSAGES/bison.mo
        thirdparty/util/GnuWin32/share/locale/ja/LC_MESSAGES/sed.mo
        thirdparty/util/GnuWin32/share/locale/ko/LC_MESSAGES/sed.mo
        thirdparty/util/GnuWin32/share/locale/nl/LC_MESSAGES/bison.mo
        thirdparty/util/GnuWin32/share/locale/nl/LC_MESSAGES/sed.mo
        thirdparty/util/GnuWin32/share/locale/pl/LC_MESSAGES/sed.mo
        thirdparty/util/GnuWin32/share/locale/pt_BR/LC_MESSAGES/bison.mo
        thirdparty/util/GnuWin32/share/locale/pt_BR/LC_MESSAGES/sed.mo
        thirdparty/util/GnuWin32/share/locale/ro/LC_MESSAGES/sed.mo
        thirdparty/util/GnuWin32/share/locale/ru/LC_MESSAGES/bison.mo
        thirdparty/util/GnuWin32/share/locale/ru/LC_MESSAGES/sed.mo
        thirdparty/util/GnuWin32/share/locale/sk/LC_MESSAGES/sed.mo
        thirdparty/util/GnuWin32/share/locale/sl/LC_MESSAGES/sed.mo
        thirdparty/util/GnuWin32/share/locale/sr/LC_MESSAGES/sed.mo
        thirdparty/util/GnuWin32/share/locale/sv/LC_MESSAGES/bison.mo
        thirdparty/util/GnuWin32/share/locale/sv/LC_MESSAGES/sed.mo
        thirdparty/util/GnuWin32/share/locale/tr/LC_MESSAGES/bison.mo
        thirdparty/util/GnuWin32/share/locale/tr/LC_MESSAGES/sed.mo
        thirdparty/util/GnuWin32/share/locale/zh_CN/LC_MESSAGES/sed.mo

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

        MsBuild -- e.g. C:\WINDOWS\Microsoft.NET\Framework\v2.0.50727
        devenv  -- e.g. C:\Program Files\Microsoft Visual Studio 8\Common7\IDE
        Doxygen -- e.g. C:\Program Files\doxygen\bin
        Dot     -- e.g. C:\Program Files\ATT\Graphviz\bin
        
        e.g.
        
        set PATH=%PATH%;C:\WINDOWS\Microsoft.NET\Framework\v2.0.50727
        set PATH=%PATH%;C:\Program Files\Microsoft Visual Studio 8\Common7\IDE
        set PATH=%PATH%;C:\Program Files\doxygen\bin
        set PATH=%PATH%;C:\Program Files\ATT\Graphviz\bin


   6. The expected list of release binaries that are expected from the  
      FDO build are as follows:

      %FDOTHIRDPARTY%\apache\xml-xalan\c\Build\Win32\VC8\Release
           Xalan-C_1_7_0.dll
           XalanMessages_1_7_0.dll

      %FDOTHIRDPARTY%\apache\xml-xerces\c\Build\Win32\VC8\Release
           xerces-c_2_5_0.dll

      %FDO%\Unmanaged\Bin\Win32\Release
           FDO.dll
           FDOMessage.dll
           FDOCommon.dll
           FDOGeometry.dll
           FDOSpatial.dll

      <FDO OpenSource Location>\Providers\SHP\Bin\Win32\Release
           SHPMessage.dll
           SHPOverrides.dll
           SHPProvider.dll

      <FDO OpenSource Location>\Providers\SDF\Bin\Win32\Release
           SDFProvider.dll
           SDFMessage.dll
 
     The expected list of debug binaries that are expected from the  
      FDO build are as follows:

      %FDOTHIRDPARTY%\apache\xml-xalan\c\Build\Win32\VC8\Debug
           Xalan-C_1_7_0D.dll
           XalanMessages_1_7_0D.dll

      %FDOTHIRDPARTY%\apache\xml-xerces\c\Build\Win32\VC8\Debug
           xerces-c_2_5_0D.dll

      %FDO%\Unmanaged\Bin\Win32\Debug
           FDO.dll
           FDOMessage.dll
           FDOCommon.dll
           FDOGeometry.dll
           FDOSpatial.dll

      <FDO OpenSource Location>\Providers\SHP\Bin\Win32\Debug
           SHPMessage.dll
           SHPOverrides.dll
           SHPProvider.dll

      <FDO OpenSource Location>\Providers\SDF\Bin\Win32\Debug
           SDFProvider.dll
           SDFMessage.dll
 
      The above binaries can be installed in any user-defined location as long
      as FDO.dll, FDOMessage.dll, FDOCommon.dll, FDOGeometry.dll
      and FDOSpatial.dll location are referenced by the windows environment PATH 
      location.


   7. The FDO OpenSource Distribution files contains a providers.xml file.
      The providers.xml file is used by FDO to identify where FDO provider
      binaries are installed. 

      The providers.xml file included in the FDO OpenSource distribution 
      has been generated for a Linux install environment and contains 
      references to Linux .so files. 

      In order for the providers.xml file to be used on a Windows environment
      the names and path locations of the provider libraries must be changed
      to be windows specific. This can be done according to the following  
      template:

      <?xml version="1.0" encoding="UTF-8" standalone="no" ?> 
      <FeatureProviderRegistry>
          <FeatureProvider>
              <Name>OSGeo.SDF.3.2</Name>
              <DisplayName>OSGeo FDO Provider for SDF</DisplayName>
              <Description>FDO Provider for SDF</Description>
              <IsManaged>False</IsManaged>
              <Version>3.2.0.0</Version>
              <FeatureDataObjectsVersion>3.2.0.0</FeatureDataObjectsVersion>
              <LibraryPath>.\SDFProvider.dll</LibraryPath>
          </FeatureProvider>
          <FeatureProvider>
          <Name>OSGeo.SHP.3.2</Name>
              <DisplayName>OSGeo FDO Provider for SHP</DisplayName>
              <Description>OSGeo FDO Provider for SHP</Description>
              <IsManaged>False</IsManaged>
              <Version>3.2.0.0</Version>
              <FeatureDataObjectsVersion>3.2.0.0</FeatureDataObjectsVersion>
              <LibraryPath>.\ShpProvider.dll</LibraryPath>
          </FeatureProvider>
      </FeatureProviderRegistry>

      NOTE: In the above xml content, replace [FDOLIBS] with the FDO install 
      location. e.g. \Program Files\Common Files\OSGeo\FDO\3.2\

      On Windows the providers.xml file will need to be located where the
      the FDO.dll is located. If the providers.xml file is not found in 
      the same directory as FDO.dll, FDO will search providers.xml under:
      \Program Files\Common Files\OSGeo\FDO\3.2\


   8. In order to build the ArcSDE Provider, you will need to have the
      ArcSDE 9.1 Client SDK installed at one of the following location:

        %SDEHOME%/

      Note that ArcSDE is licensed software and must be obtained from an
      ESRI vendor.

      The list of files expected under these locations is as follows:

        .../lib/icuuc.lib
        .../lib/pe91.lib
        .../lib/sde91.lib
        .../lib/sg91.lib
        .../include/sg.h
        .../include/sgerr.h
        .../include/pe.h
        .../include/pe_coordsys_from_prj.h
        .../include/pedef.h
        .../include/pef.h
        .../include/sdeerno.h
        .../include/sderaster.h
        .../include/sdetype.h
        .../bin/edgemt.dll
        .../bin/gsrvrdb291.dll
        .../bin/gsrvrinf91.dll
        .../bin/gsrvrora8i91.dll
        .../bin/gsrvrora9i91.dll
        .../bin/gsrvrsql91.dll
        .../bin/icudt22l.dll
        .../bin/icuuc22.dll
        .../bin/libtiff.dll
        .../bin/loceng.dll
        .../bin/locssa.dll
        .../bin/mtchloc.dll
        .../bin/mtchmt.dll
        .../bin/pe91.dll
        .../bin/sde91.dll
        .../bin/sdedb2srvr91.dll
        .../bin/sdeinfsrvr91.dll
        .../bin/sdeora8isrvr91.dll
        .../bin/sdeora9isrvr91.dll
        .../bin/sdesqlsrvr91.dll
        .../bin/sg91.dll
        .../bin/xerces-c_2_1_0.dll
        
   9. In order to build the MySQL Provider, you will need to download 
   and install the MySQL client (Windows (x86) version 5.0), including
   the developer components.

        Home Page: 
        http://dev.mysql.com/

        Download Location: 
        http://dev.mysql.com/downloads/mysql/5.0.html

   Following the installation, set the following FDO environment variable:

        FDOMYSQL=<MySQL developer components path> 

   e.g. \Program Files\MySQL\MySQL Server 5.0

   In the MySQL developer components path you will need to have two folders 
   used by MySQL provider: include and lib.        

II. Windows steps for open source build: 

  1.  Extract the FDO OpenSource files using either gunzip/tar or Winzip.

       Use gunzip and tar utilities as follows:

            gunzip fdo-3.2.0.tar.gz
            tar -xvf fdo-3.2.0.tar

       - or -

       Use Windows Winzip utility as follows:

            WINZIP32.EXE fdo-3.2.0.tar.gz

  2.   Use the build.bat file to build the FDO OpenSource binaries. The following 
       is a general guideline on how to use build.bat
  
**************************************************************************
build.bat [-h] [-o=OutFolder] [-c=BuildType] [-a=Action] [-w=WithModule] [-d=BuildDocs]
*
Help:               -h[elp]
OutFolder:      -o[utpath]=destination folder for binaries
BuildType:      -c[onfig]=release(default), debug
Action:            -a[ction]=buildinstall(default), buildonly, installonly
WithModule:   -w[ith]=all(default), thirdparty, fdo, providers, shp, sdf, wfs, wms, arcsde, odbc, mysql
BuildDocs:      -d[ocs]=skip(default), build
**************************************************************************
            e.g.

            **** Display help for build.bat
            build -h
            **** ReBuild all components excluding documentation
            build -a=buildonly
            **** ReBuild all components excluding documentation 
            **** and copy output files to C:\Fdo
            build -o=C:\Fdo
            **** ReBuild all components and documentation
            **** and copy output and documentation files to C:\Fdo
            build -o=C:\Fdo -d=build
            **** ReBuild FDO library
            build -a=buildonly -w=fdo
            **** ReBuild all FDO Providers
            build -a=buildonly -w=providers
            **** ReBuild FDO Shape Provider
            build -a=buildonly -w=shp
            **** ReBuild FDO SDF Provider
            build -a=buildonly -w=sdf
            **** Build only documentation and copy files to C:\Fdo
            build -o=C:\Fdo -d=build -a=installonly

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

 
 4.  If any Makefile.am or configure.in file is modified, before "./configure" 
      you must run:

       autoconf

       automake


  5.  To run the unit test, you must set LD_LIBRARY_PATH as follows:

       $ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/fdo-3.2.0/lib:
         $FDOTHIRDPARTY/linux/cppunit/lib
      (Note: The path should be entered all on one line.)

  6.  providers.xml will be installed to /usr/local/fdo-3.2.0/lib.
 
  7.  Several known problems exist if auto-mounted drives are used as locations for the
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

  8.  In order to build the ArcSDE Provider, you will need to have the
      ArcSDE 9.1 Client SDK installed at one of the following location:

        $SDEHOME/

      Note that ArcSDE is licensed software and must be obtained from an
      ESRI vendor.

      The list of files expected under these locations is as follows:

        .../lib/libgsrvrdb291.so
        .../lib/libgsrvrora9i91.so
        .../lib/libicudata.so.22.0
        .../lib/libicuuc.so.22.0
        .../lib/libloceng.so
        .../lib/liblocssa.so
        .../lib/libmtchloc.so
        .../lib/libmtchmt.so
        .../lib/libpe91.a
        .../lib/libpe91.so
        .../lib/libsde91.a
        .../lib/libsde91.so
        .../lib/libsdedb2srvr91.so
        .../lib/libsdejavautil.so
        .../lib/libsdeora9isrvr91.so
        .../lib/libsg91.a
        .../lib/libsg91.so
        .../lib/libxerces-c.so.21.0
        .../lib/concurrent.jar
        .../lib/jpe91_sdk.jar
        .../lib/jsde91_sdk.jar
        .../lib/libedgemt.so
        .../include/pe.h
        .../include/pe_coordsys_from_prj.h
        .../include/pedef.h
        .../include/pef.h
        .../include/sdeerno.h
        .../include/sderaster.h
        .../include/sdetype.h
        .../include/sg.h
        .../include/sgerr.h

   9. In order to build the MySQL Provider, you will need to download 
   and extract the Linux (non RPM package - Linux (x86)  Standard verion 5.0)
   files using either gunzip/tar.
 
        Home Page: 
        http://dev.mysql.com/
        
        Download Location: 
        http://dev.mysql.com/downloads/mysql/5.0.html
 
   Following the installation, set the following FDO environment variable:
  
        FDOMYSQL=<MySQL developer components path>
 
   e.g. /home/mysql-standard-5.0-linux-i686-glibc23

   In the MySQL developer components path you will need to have two folders 
   used by MySQL provider: include and lib.


II. Linux steps for open source build:

  1.  gunzip fdo-3.2.0.tar.gz

  2.  tar -xvf fdo-3.2.0.tar

  3.  cd OpenSource_FDO 

  4.  sh build_linux.sh

  5.  make install
<end>
