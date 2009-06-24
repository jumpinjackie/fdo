Feature Data Objects (FDO) CMake Experimental Build
===================================================

Thie ReadMe contains instructions of how to use CMake to compile FDO wihtout needing rely on any
of the external scripts.
Initial support is only Linux and Unix and the goal is use all possible system external libraries,
but code is been prepared for Windows ( possible MacOSX )

Current Status:

- Compiles Whole FDO Core and Utilities, except SQLiteInterface ( SQLiteInterface can't be build yet
	since relies on non public headers. A code tu use standard sqlite can be made replacing explicit
	BTree code.
- Test for SYSTEM third party dependencies as default and use it if available

TODO
- Make internal apache Thirdparty compile ( now only system )
- Finish install part ( not tested yet )
- Create a global instalable FindFDO.cmake to enable providers compile independent of full FDO source
- Compile providers ( independent )

ISSUES
- SQlite have two different internal versions on FDO source, one in ThirdParty, other in SQL provider
- Make SQLite provider CMake build been common among all providers following new system
- Need to use internal CPPUNIT since Unittest is compiling only with specific included version


----------------------------------
Unix / Linux Instructions
----------------------------------

I. Requiremnents
	Base:
		CMake
	For system libraries, developemnet packages for:
		openssl
		curl
		boost
		gdal
		cppunit ( not recomended yet )
		sqlite3
		xerces-c
		xalan-c

II. Usage
	Cmake build can be done ( and recomended to ) in out of source directory, so a simple run would be
	- mkdir <MY_BUILDDIR>
	- cd <MY_BUILDDIR>
	- cmake <FDO_SOURCEDIR> -DINTERNAL_CPPUNIT=TRUE
	- make

	You can pass any definition to compiler through this method.

III. Using internal provided Thirdparty libraries
	Since fdo main source has a Thirdparty internal  dependencies source provided, if you desire
	compile and use internal dependencies, add the follow definitions in the cmake command line call:
	-DINTERNAL_<DEPENDENCY>=TRUE
	Where DEPENDENCY can be one of this ( case sensitive ):
	- OPENSSL - openssl library
	- GDAL - gdal library
	- CPPUNIT - Olf cppunit version
	- CURL - curl library
	- SQLITE - Sqlite 3
	- BOOST - Boost libraries
	- POSTGRESQL - postgresql ( for PostGis provider )

IV. Compiling Providers
	Providers can be called directly from cmake commandline, no need to compile separated anymore if you
	are doing in source build
	
	If you desire compile some provider(s), add the follow definitions in the cmake command line call:
	-DWITH_<provider>
	Where <provider> is the name of provider you want to compile, be case sensitive, all upper case or all lower case.

	All OpenSource providers are available already.Usage example:
	-DWITH_POSTGIS=1 or -DWITH_PostGIS=1 or -DWITH_postgis=1

	With some providers:
	-DWITH_POSTGIS=1 -DWITH_GDAL=1 -DWITH_WMS=1

	If you desire to build all available providers, use this definition ( case sensitive )
	-DWITH_ALLPROVIDERS=1

	There's no automatic provider compilation unless explicit defined.
	The compilation order is same as providers defined in command line.

Help and reports to Helio Castro <helio@kde.org>


