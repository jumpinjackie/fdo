# Local tools
add_subdirectory( linux/mkcatdefs )

# Build local CPPUNIT
if( INTERNAL_CPPUNIT )
	if( UNIX )
		message( STATUS "Internal CppUnit will be compiled and used." )
		add_custom_target( Internal_Cppunit
			DEPENDS ${FDOTHIRDPARTY}/linux/cppunit/lib/libcppunit.a )
		add_custom_command(
			OUTPUT ${FDOTHIRDPARTY}/linux/cppunit/lib/libcppunit.a
			COMMAND ${FDOTHIRDPARTY}/linux/cppunit/build
			WORKING_DIRECTORY ${FDOTHIRDPARTY}/linux/cppunit )
	endif( UNIX )
endif( INTERNAL_CPPUNIT )

# Build local GDAL
if( INTERNAL_GDAL )
	if( UNIX )
		message( STATUS "Internal GDAL will be compiled and used." )
		add_custom_target( Internal_GDAL
			DEPENDS ${FDOTHIRDPARTY}/gdal/lib/libgdal.a )
		add_custom_command(
			OUTPUT ${FDOTHIRDPARTY}/gdal/lib/libgdal.a
			COMMAND mkdir ARGS -p ${FDOTHIRDPARTY}/gdal/lib ${FDOTHIRDPARTY}/gdal/include
			COMMAND chmod ARGS +x ${FDOTHIRDPARTY}/gdal/configure
			COMMAND ${FDOTHIRDPARTY}/gdal/configure ARGS 
				--with-gif=internal 
				--with-jpeg=internal 
				--with-png=internal 
				--with-libtiff=internal 
				--with-geotiff=internal 
				--with-pg=no 
				--with-python=no 
				--with-libz=internal
				--with-netcdf=no
			COMMAND make
			COMMAND cp ARGS -f .libs/libgdal.a ${FDOTHIRDPARTY}/gdal/lib
			COMMAND cp ARGS -f .libs/libgdal.so* ${FDOTHIRDPARTY}/gdal/lib
			COMMAND cp ARGS -f port/*.h ${FDOTHIRDPARTY}/gdal/include/
			COMMAND cp ARGS -f gcore/*.h ${FDOTHIRDPARTY}/gdal/include/
			COMMAND cp ARGS -f alg/*.h ${FDOTHIRDPARTY}/gdal/include/
			COMMAND cp ARGS -f ogr/*.h ${FDOTHIRDPARTY}/gdal/include/
			WORKING_DIRECTORY ${FDOTHIRDPARTY}/gdal )
	endif( UNIX )
endif( INTERNAL_GDAL )

# Build local Sqlite
if( INTERNAL_SQLITE )
	if( UNIX )
		message( STATUS "Internal Sqlite will be compiled and used." )
		add_custom_target( Internal_Sqlite
			DEPENDS ${FDOTHIRDPARTY}/Sqlite3.3.13/linux/lib/sqlite3.a )
		add_custom_command(
			OUTPUT ${FDOTHIRDPARTY}/Sqlite3.3.13/linux/lib/sqlite3.a
			COMMAND make
			WORKING_DIRECTORY ${FDOTHIRDPARTY}/Sqlite3.3.13/Src )
	endif( UNIX )
endif( INTERNAL_SQLITE )

# Build local Curl
if( INTERNAL_CURL )
	if( UNIX )
		message( STATUS "Internal CURL will be compiled and used." )
		add_custom_target( Internal_CURL
			DEPENDS ${FDOTHIRDPARTY}/libcurl/lib/linux/libcurl.a )
		add_custom_command(
			OUTPUT ${FDOTHIRDPARTY}/libcurl/lib/linux/libcurl.a
			COMMAND mkdir ARGS -p ${FDOTHIRDPARTY}/libcurl/lib/linux 
			COMMAND chmod ARGS +x ${FDOTHIRDPARTY}/libcurl/configure
			COMMAND ${FDOTHIRDPARTY}/libcurl/configure ARGS --without-libidn
			COMMAND make
			COMMAND cp ARGS -f ./lib/.libs/libcurl.a ./lib/linux
			WORKING_DIRECTORY ${FDOTHIRDPARTY}/libcurl )
	endif( UNIX )
endif( INTERNAL_CURL )

# Build local boost
if( INTERNAL_BOOST )
	if( UNIX )
		message( STATUS "Internal boost will be compiled and used." )
		add_custom_target( Internal_Boost
			DEPENDS ${FDOTHIRDPARTY}/boost/lib/libboost_iostreams.a )
		execute_process( COMMAND rm -f ${FDOTHIRDPARTY}/boost/tools/jam/src/bin.linux/bjam )
		add_custom_command(
			OUTPUT ${FDOTHIRDPARTY}/boost/tools/jam/src/bin.linux/bjam
			COMMAND chmod ARGS a+x build.sh
			COMMAND ./build.sh
			WORKING_DIRECTORY ${FDOTHIRDPARTY}/boost/tools/jam/src/ )
		add_custom_command(
			OUTPUT ${FDOTHIRDPARTY}/boost/lib/libboost_iostreams.a
			DEPENDS  ${FDOTHIRDPARTY}/boost/tools/jam/src/bin.linux/bjam
			COMMAND ${FDOTHIRDPARTY}/boost/tools/jam/src/bin.linux/bjam ARGS 
				toolset=gcc variant=release threading=multi link=static --layout=system stage
			WORKING_DIRECTORY ${FDOTHIRDPARTY}/boost/ )
	endif( UNIX )
endif( INTERNAL_BOOST )

# Build local openssl
if( INTERNAL_OPENSSL )
	if( UNIX )
		message( STATUS "Internal openssl will be compiled and used." )
		add_custom_target( Internal_OpenSSL 
			DEPENDS ${FDOTHIRDPARTY}/openssl/lib/linux/libssl.a )
		add_custom_command(
			OUTPUT ${FDOTHIRDPARTY}/openssl/lib/linux/libssl.a
			COMMAND mkdir ARGS -p lib/linux 
			COMMAND chmod ARGS +x config
			COMMAND ./config
			COMMAND make
			COMMAND cp ARGS -f libssl.a libcrypto.a lib/linux
			WORKING_DIRECTORY ${FDOTHIRDPARTY}/openssl )
	endif( UNIX )
endif( INTERNAL_OPENSSL )
