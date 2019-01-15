include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CPPUNIT_INCLUDE_DIR}
    ${UTILITIES_TESTCOMMON_INCLUDE_DIR}
    ${MYSQL_INCLUDE_DIR}
    Common
    MySql
    Odbc
    PostGis
)

add_subdirectory( Common )
add_subdirectory( Odbc )
add_subdirectory( MySql )
add_subdirectory( PostGis )

add_definitions( -D__STDC_LIMIT_MACROS -DRDBI_STATIC )

# Ensure test data is copied to where the unit test binary is generated
file(GLOB GenericRdbmsUnitTest_TESTDATA
    "*.txt"
    "*.xml"
    "*.xslt"
    "*.mdb"
    "*.xls"
    "*.DBF"
    "*.INF"
    "*.MDX"
)
file(COPY ${GenericRdbmsUnitTest_TESTDATA} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

################ MySQL Unit Test ################

set( UnitTestMySql_SRCS
    MySqlTestRegister.cpp
)

add_executable( UnitTestMySql ${UnitTestMySql_SRCS} )
add_sanitizers( UnitTestMySql )

if( CMAKE_COMPILER_IS_GNUCXX )
    target_link_libraries( UnitTestMySql
        -Wl,-whole-archive
        ${CPPUNIT_LIBRARIES}
        FdoRdbms
        rdbi
        mysqldr
        Gdbi
        LTManager
        LockManager
        genericrdbms_util
        FdoRdbmsMySQL_Fdo
        FdoRdbmsMySQL_SchemaMgr
        FdoRdbmsMySQL_SchemaMgr_Lp
        FdoRdbmsMySQL_SchemaMgr_Ph
        geometry_fgf
        SmLpGrd
        SmGrd
        SmPhCfgGrd
        SmPhGrd
        SmPhRdGrd
        ProvidersCommon
        SchemaMgr
        TestCommon
        CommonUnitTests
        MySqlUnitTests
        -Wl,-no-whole-archive
    )
else( CMAKE_COMPILER_IS_GNUCXX )
    target_link_libraries( UnitTestMySql
        ${CPPUNIT_LIBRARIES}
        FdoRdbms
        rdbi
        mysqldr
        Gdbi
        LTManager
        LockManager
        genericrdbms_util
        FdoRdbmsMySQL_Fdo
        FdoRdbmsMySQL_SchemaMgr
        FdoRdbmsMySQL_SchemaMgr_Lp
        FdoRdbmsMySQL_SchemaMgr_Ph
        geometry_fgf
        SmLpGrd
        SmGrd
        SmPhCfgGrd
        SmPhGrd
        SmPhRdGrd
        ProvidersCommon
        SchemaMgr
        TestCommon
        CommonUnitTests
        MySqlUnitTests
    )
endif( CMAKE_COMPILER_IS_GNUCXX )
target_link_libraries( UnitTestMySql
    ${ZLIB_LIBRARIES}
    ${MYSQL_LIBRARIES}
    FDO${FDO_VERSION_SUFFIX}
    ExpressionEngine${FDO_VERSION_SUFFIX}
    RdbmsOverrides${FDO_VERSION_SUFFIX}
    MySQLOverrides${FDO_VERSION_SUFFIX}
    MySQLProvider${FDO_VERSION_SUFFIX}
)

################ ODBC Unit Test ################

set( UnitTestOdbc_SRCS
    OdbcTestRegister.cpp
)

add_executable( UnitTestOdbc ${UnitTestOdbc_SRCS} )
add_sanitizers( UnitTestOdbc )

if( CMAKE_COMPILER_IS_GNUCXX )
    target_link_libraries( UnitTestOdbc
        -Wl,-whole-archive
        ${CPPUNIT_LIBRARIES}
        FdoRdbms
        rdbi
        FdoODBCDriver
        Gdbi
        LTManager
        LockManager
        genericrdbms_util
        FdoRdbmsODBC_Fdo
        FdoRdbmsODBC_SchemaMgr
        FdoRdbmsODBC_SchemaMgr_Lp
        FdoRdbmsODBC_SchemaMgr_Ph
        geometry_fgf
        SmLpGrd
        SmGrd
        SmPhCfgGrd
        SmPhGrd
        SmPhRdGrd
        ProvidersCommon
        SchemaMgr
        TestCommon
        CommonUnitTests
        OdbcUnitTests
        -Wl,-no-whole-archive
    )
else( CMAKE_COMPILER_IS_GNUCXX )
    target_link_libraries( UnitTestOdbc
        ${CPPUNIT_LIBRARIES}
        FdoRdbms
        rdbi
        FdoODBCDriver
        Gdbi
        LTManager
        LockManager
        genericrdbms_util
        FdoRdbmsODBC_Fdo
        FdoRdbmsODBC_SchemaMgr
        FdoRdbmsODBC_SchemaMgr_Lp
        FdoRdbmsODBC_SchemaMgr_Ph
        geometry_fgf
        SmLpGrd
        SmGrd
        SmPhCfgGrd
        SmPhGrd
        SmPhRdGrd
        ProvidersCommon
        SchemaMgr
        TestCommon
        CommonUnitTests
        OdbcUnitTests
    )
endif( CMAKE_COMPILER_IS_GNUCXX )
target_link_libraries( UnitTestOdbc
    ${ODBC_LIBRARY}
    ${ODBCINST_LIBRARY}
    FDO${FDO_VERSION_SUFFIX}
    ExpressionEngine${FDO_VERSION_SUFFIX}
    RdbmsOverrides${FDO_VERSION_SUFFIX}
    ODBCOverrides${FDO_VERSION_SUFFIX}
    ODBCProvider${FDO_VERSION_SUFFIX}
)

################ PostGIS Unit Test ################

set( UnitTestPostGis_SRCS
    PostGisTestRegister.cpp
)

add_executable( UnitTestPostGis ${UnitTestPostGis_SRCS} )
add_sanitizers( UnitTestPostGis )

if( CMAKE_COMPILER_IS_GNUCXX )
    target_link_libraries( UnitTestPostGis
        -Wl,-whole-archive
        ${CPPUNIT_LIBRARIES}
        ProvidersCommon
        SchemaMgr
        FdoRdbms
        rdbi
        postgisdr
        Gdbi
        LTManager
        LockManager
        genericrdbms_util
        FdoRdbmsPostGis_Fdo
        FdoRdbmsPostGis_SchemaMgr
        FdoRdbmsPostGis_SchemaMgr_Lp
        FdoRdbmsPostGis_SchemaMgr_Ph
        geometry_fgf
        SmLpGrd
        SmGrd
        SmPhCfgGrd
        SmPhGrd
        SmPhRdGrd
        TestCommon
        CommonUnitTests
        PostGisUnitTests
        -Wl,-no-whole-archive
    )
else( CMAKE_COMPILER_IS_GNUCXX )
    target_link_libraries( UnitTestPostGis
        ${CPPUNIT_LIBRARIES}
        ProvidersCommon
        SchemaMgr
        FdoRdbms
        rdbi
        postgisdr
        Gdbi
        LTManager
        LockManager
        genericrdbms_util
        FdoRdbmsPostGis_Fdo
        FdoRdbmsPostGis_SchemaMgr
        FdoRdbmsPostGis_SchemaMgr_Lp
        FdoRdbmsPostGis_SchemaMgr_Ph
        geometry_fgf
        SmLpGrd
        SmGrd
        SmPhCfgGrd
        SmPhGrd
        SmPhRdGrd
        TestCommon
        CommonUnitTests
        PostGisUnitTests
    )
endif( CMAKE_COMPILER_IS_GNUCXX )
target_link_libraries( UnitTestPostGis
    ${POSTGRESQL_LIBRARIES}
    FDO${FDO_VERSION_SUFFIX}
    ExpressionEngine${FDO_VERSION_SUFFIX}
    RdbmsOverrides${FDO_VERSION_SUFFIX}
    PostgreSQLOverrides${FDO_VERSION_SUFFIX}
    PostgreSQLProvider${FDO_VERSION_SUFFIX}
)

# Remove providers.xml from output as that is windows-only
file(REMOVE "${CMAKE_CURRENT_BINARY_DIR}/providers.xml")

# Replace with our own configured version
set(FDO_MYSQL_PROVIDER_NAME "OSGeo.MySQL")
set(FDO_MYSQL_PROVIDER_LIBRARY "../MySQL/libMySQLProvider${FDO_VERSION_SUFFIX}.so")
set(FDO_ODBC_PROVIDER_NAME "OSGeo.ODBC")
set(FDO_ODBC_PROVIDER_LIBRARY "../ODBC/libODBCProvider${FDO_VERSION_SUFFIX}.so")
set(FDO_POSTGRESQL_PROVIDER_NAME "OSGeo.PostgreSQL")
set(FDO_POSTGRESQL_PROVIDER_LIBRARY "../PostGis/libPostgreSQLProvider${FDO_VERSION_SUFFIX}.so")
# This provider doesn't exist on Linux, but if/when it does ...
set(FDO_SQLSERVER_PROVIDER_NAME "OSGeo.SQLServerSpatial")
set(FDO_SQLSERVER_PROVIDER_LIBRARY "../SQLServerSpatial/libSQLServerSpatialProvider${FDO_VERSION_SUFFIX}.so")
configure_file(${CMAKE_MODULE_PATH}/../configs/providers.rdbms.xml.in ${CMAKE_CURRENT_BINARY_DIR}/providers.xml)