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
    - RDBMS
    - WFS
    - WMS
    - PostGIS
 

TODO
- Make internal apache Thirdparty compile ( now only system )
- Finish install part ( not tested yet )
- Create a global instalable FindFDO.cmake to enable providers compile independent of full FDO source
- Compile providers ( independent )


----------------------------------
Unix / Linux Instructions
----------------------------------

I. Requiremnents
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

II. Usage
	Cmake build can be done ( and recomended to do ) outside of the source directory, so a simple run would be
	- mkdir <MY_BUILDDIR>
	- cd <MY_BUILDDIR>
	- cmake <FDO_SOURCEDIR> -DWITH_SDF=TRUE -DWITH_SHP=TRUE -DWITH_SQLITE=TRUE
	- make

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


IV. Compiling Providers
	Providers can be called directly from cmake commandline, no need to compile separated anymore if you
	are doing in source build
	
	If you desire compile some provider(s), add the follow definitions in the cmake command line call:
	-DWITH_<provider>
	Where <provider> is the name of provider you want to compile, be case sensitive, all upper case or all lower case.

	All OpenSource providers are available already. Usage example:
	-DWITH_POSTGIS=TRUE or -DWITH_PostGIS=TRUE or -DWITH_postgis=TRUE

	With some providers:
	-DWITH_POSTGIS=TRUE -DWITH_GDAL=TRUE -DWITH_WMS=TRUE

	If you desire to build all available providers, use this definition ( case sensitive )
	-DALL_PROVIDERS=TRUE

	There's no automatic provider compilation unless explicit defined.
	The compilation order is same as providers defined in command line.

Help and reports to Helio Castro <helio@kde.org>


