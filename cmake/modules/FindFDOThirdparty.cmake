# This file is strictly to check and enable syetm or internal Thridparty
# libraries

# Check GDAL or add ThirdParty
if( NOT INTERNAL_GDAL )
	find_package(GDAL REQUIRED)
else( NOT INTERNAL_GDAL )
	set( LOCAL_GDAL TRUE )
	set( GDAL_LIBRARIES  ${FDOTHIRDPARTY}/gdal/lib )
	set( GDAL_LIBRARIES_INCLUDE_DIRS  ${FDOTHIRDPARTY}/gdal )
endif( NOT INTERNAL_GDAL )

# Check MySQL or add ThirdParty
if( NOT INTERNAL_MYSQL )
	find_package(MySQL REQUIRED)
else( NOT INTERNAL_MYSQL )
	set( LOCAL_MYSQL TRUE )
endif( NOT INTERNAL_MYSQL )

# Check for Apache or ThirdParty
if( NOT INTERNAL_APACHE )
else( NOT INTERNAL_APACHE )
	set( LOCAL_APACHE TRUE )
endif( NOT INTERNAL_APACHE )

# Check for cppunit or ThirdParty
if( NOT INTERNAL_CPPUNIT )
else( NOT INTERNAL_CPPUNIT )
	set( LOCAL_CPPUNIT TRUE )
	set( CPPUNIT_LIBRARY_DIR  ${FDOTHIRDPARTY}/linux/cppunit/lib )
	set( CPPUNIT_INCLUDE_DIR  ${FDOTHIRDPARTY}/linux/cppunit/include )
	mark_as_advanced( CPPUNIT_LIBRARY_DIR CPPUNIT_INCLUDE_DIR )
endif( NOT INTERNAL_CPPUNIT )

# Check for SqLite or ThirdParty
if( NOT INTERNAL_SQLITE )
	find_package( Sqlite REQUIRED )
else( NOT INTERNAL_SQLITE )
	set( LOCAL_SQLITE TRUE )
	set( SQLITE_INCLUDE_DIR ${FDOTHIRDPARTY}/Sqlite3.3.13/Src )
	mark_as_advanced( SQLITE_INCLUDE_DIR )
endif( NOT INTERNAL_SQLITE )

# Check for Boost or ThirdParty
if( NOT INTERNAL_BOOST )
	find_package( Boost REQUIRED )
else( NOT INTERNAL_BOOST )
	set( LOCAL_BOOST TRUE )
endif( NOT INTERNAL_BOOST )

# Check for Curl or ThirdParty
if( NOT INTERNAL_CURL )
	find_package( CURL REQUIRED )
else( NOT INTERNAL_CURL )
	set( LOCAL_CURL TRUE )
	set( CURL_INCLUDE_DIRS ${FDOTHIRDPARTY}/libcurl/include )
	if( UNIX )
		set( CURL_LIBRARIES ${FDOTHIRDPARTY}/libcurl/lib/linux )
	endif( UNIX )
	mark_as_advanced( CURL_INCLUDE_DIRS CURL_LIBRARIES )
endif( NOT INTERNAL_CURL )

# Check for OpenSSL or ThirdParty
if( NOT INTERNAL_OPENSSL )
	find_package( OpenSSL REQUIRED )
else( NOT INTERNAL_OPENSSL )
	set( LOCAL_OPENSSL TRUE )
	set( OPENSSL_INCLUDE_DIR ${FDOTHIRDPARTY}/openssl )
	if( UNIX )
		set( OPENSSL_LIBRARIES ${FDOTHIRDPARTY}/openssl/lib/linux )
	endif( UNIX )
	mark_as_advanced( OPENSSL_INCLUDE_DIR OPENSSL_LIBRARIES )
endif( NOT INTERNAL_OPENSSL )

# TODO
# Check UnixODBC
