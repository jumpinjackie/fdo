# This file is strictly to check and enable system or internal Thirdparty
# libraries

if( ALL_THIRDPARTY )
	set( INTERNAL_CPPUNIT true )
	set( INTERNAL_GDAL true )
	set( INTERNAL_OPENSSL true )
	set( INTERNAL_MYSQL true )
	set( INTERNAL_POSTGRESQL true )
	set( INTERNAL_CURL true )
	set( INTERNAL_XERCESC true )
	set( INTERNAL_XALANC true )
endif( ALL_THIRDPARTY )

# Check GDAL or add ThirdParty
if( NOT INTERNAL_GDAL )
    find_package(GDAL REQUIRED)
else( NOT INTERNAL_GDAL )
    set( LOCAL_GDAL TRUE )
    set( GDAL_LIBRARIES  ${FDOTHIRDPARTY}/gdal/lib )
    set( GDAL_INCLUDE_DIR  ${FDOTHIRDPARTY}/gdal 
        ${FDOTHIRDPARTY}/gdal/include
        ${FDOTHIRDPARTY}/gdal/ogr/ogrsf_frmts
    )
    message (STATUS "GDAL Include Dir: ${GDAL_INCLUDE_DIR}")
    find_library( GDAL_LIBRARY NAMES gdal
        PATHS ${GDAL_LIBRARIES}
        NO_DEFAULT_PATH
    )
    message (STATUS "Using internal gdal: ${GDAL_LIBRARY}")
endif( NOT INTERNAL_GDAL )

# Check MySQL or add ThirdParty
if( NOT INTERNAL_MYSQL )
    find_package(MySQL REQUIRED)
else( NOT INTERNAL_MYSQL )
    set( LOCAL_MYSQL TRUE )
endif( NOT INTERNAL_MYSQL )

# Check PostgreSQL or add Thirdparty
if (NOT INTERNAL_POSTGRESQL)
    find_package(PostgreSQL REQUIRED)
else (NOT INTERNAL_POSTGRESQL)
    set ( LOCAL_POSTGRESQL TRUE )
    set ( POSTGRESQL_INCLUDE_DIR ${FDOTHIRDPARTY}/pgsql/include/ )
    set ( POSTGRESQL_LIBRARIES ${FDOTHIRDPARTY}/pgsql/lib/libpq.a )
    set ( POSTGRESQL_FOUND TRUE )
    mark_as_advanced ( POSTGRESQL_INCLUDE_DIR POSTGRESQL_LIBRARIES )
endif (NOT INTERNAL_POSTGRESQL)

# Check for Apache or ThirdParty
if( NOT INTERNAL_APACHE )
else( NOT INTERNAL_APACHE )
    set( LOCAL_APACHE TRUE )
endif( NOT INTERNAL_APACHE )

# Check for cppunit or ThirdParty
if( NOT INTERNAL_CPPUNIT )
    find_package( CppUnit )
else( NOT INTERNAL_CPPUNIT )
    set( LOCAL_CPPUNIT TRUE )
    set( CPPUNIT_LIBRARY_DIR  ${FDOTHIRDPARTY}/linux/cppunit/lib )
    set( CPPUNIT_INCLUDE_DIR  ${FDOTHIRDPARTY}/linux/cppunit/include )
    find_library ( CPPUNIT_LIBRARIES NAMES libcppunit.a
        PATHS ${CPPUNIT_LIBRARY_DIR}
        NO_DEFAULT_PATH
    )
    set( CPPUNIT_FOUND TRUE )
    message (STATUS "Using internal cppunit: ${CPPUNIT_INCLUDE_DIR}, ${CPPUNIT_LIBRARIES}")
    mark_as_advanced( CPPUNIT_LIBRARY_DIR CPPUNIT_INCLUDE_DIR )
endif( NOT INTERNAL_CPPUNIT )

# Check for xerces-c or ThirdParty
if( NOT INTERNAL_XERCESC )
    find_package( XercesC REQUIRED )
else( NOT INTERNAL_XERCESC )
    set( LOCAL_XERCESC TRUE )
    set( XERCESC_LIBRARY_DIR  ${FDOTHIRDPARTY}/apache/xerces/lib/ )
    set( XERCESC_INCLUDE_DIR  ${FDOTHIRDPARTY}/apache/xerces/src/ )
    find_library( XERCESC_LIBRARIES NAMES xerces-c
        PATHS ${XERCESC_LIBRARY_DIR}
        NO_DEFAULT_PATH
    )
    set( XERCESC_FOUND TRUE )
    message (STATUS "Using internal xerces-c: ${XERCESC_LIBRARIES}")
    mark_as_advanced( XERCESC_LIBRARY_DIR XERCESC_INCLUDE_DIR )
endif( NOT INTERNAL_XERCESC )

# Check for xalan-c or ThirdParty
if( NOT INTERNAL_XALANC )
    find_package( XalanC REQUIRED )
else( NOT INTERNAL_XALANC )
    set( LOCAL_XALANC TRUE )
    set( XALANC_LIBRARY_DIR  ${FDOTHIRDPARTY}/apache/xalan/lib/ )
    set( XALANC_INCLUDE_DIR  ${FDOTHIRDPARTY}/apache/xalan/src/ 
        ${FDOTHIRDPARTY}/apache/xalan/nls/include/
    )
    find_library( XALANC_LIBRARIES NAMES xalan-c xalanMsg
        PATHS ${XALANC_LIBRARY_DIR}
        NO_DEFAULT_PATH
    )
    set( XALANC_FOUND TRUE )
    message (STATUS "Using internal xalan-c: ${XALANC_LIBRARIES}")
    mark_as_advanced( XALANC_LIBRARY_DIR XALANC_INCLUDE_DIR )
endif( NOT INTERNAL_XALANC )

# SqLite ThirdParty
set( SQLITE_INCLUDE_DIR ${FDOTHIRDPARTY}/Sqlite3.3.13/Src )
set( SQLITE_DIR ${FDOTHIRDPARTY}/Sqlite3.3.13/Src )
mark_as_advanced( SQLITE_INCLUDE_DIR SQLITE_DIR )

# Check for Curl or ThirdParty
if( NOT INTERNAL_CURL )
    find_package( CURL REQUIRED )
else( NOT INTERNAL_CURL )
    set( LOCAL_CURL TRUE )
    set( CURL_INCLUDE_DIRS ${FDOTHIRDPARTY}/libcurl/include/ )
    find_library( CURL_LIBRARIES NAMES curl
        PATHS ${FDOTHIRDPARTY}/libcurl/lib/linux
        NO_DEFAULT_PATH
    )
    message (STATUS "Using internal curl: ${CURL_LIBRARIES}")
    mark_as_advanced( CURL_INCLUDE_DIRS CURL_LIBRARIES )
endif( NOT INTERNAL_CURL )

# Check for OpenSSL or ThirdParty
if( NOT INTERNAL_OPENSSL )
    find_package( OpenSSL REQUIRED )
else( NOT INTERNAL_OPENSSL )
    set( LOCAL_OPENSSL TRUE )
    set( OPENSSL_INCLUDE_DIR ${FDOTHIRDPARTY}/openssl/ )
    set( OPENSSL_LIBRARY ${FDOTHIRDPARTY}/openssl/lib/linux/libssl.a )
    set( OPENSSL_CRYPTO_LIBRARY ${FDOTHIRDPARTY}/openssl/lib/linux/libcrypto.a )
    set( OPENSSL_LIBRARIES ${OPENSSL_LIBRARY}
        ${OPENSSL_CRYPTO_LIBRARY} )
    message (STATUS "Using internal OpenSSL: ${OPENSSL_LIBRARIES}")
    mark_as_advanced( OPENSSL_INCLUDE_DIR OPENSSL_LIBRARIES )
endif( NOT INTERNAL_OPENSSL )

# TODO
# Check UnixODBC
