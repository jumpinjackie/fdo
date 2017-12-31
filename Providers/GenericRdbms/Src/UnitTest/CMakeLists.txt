include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
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
    ${CPPUNIT_LIBRARIES}
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
        FdoRdbms
        FdoRdbmsODBC_Fdo
        FdoRdbmsODBC_SchemaMgr_Lp
        FdoRdbmsODBC_SchemaMgr_Ph
        rdbi
        FdoODBCDriver
        Gdbi
        LTManager
        LockManager
        genericrdbms_util
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
        FdoRdbms
        FdoRdbmsODBC_Fdo
        FdoRdbmsODBC_SchemaMgr_Lp
        FdoRdbmsODBC_SchemaMgr_Ph
        rdbi
        FdoODBCDriver
        Gdbi
        LTManager
        LockManager
        genericrdbms_util
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
    ${CPPUNIT_LIBRARIES}
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
    ${CPPUNIT_LIBRARIES}
    FDO${FDO_VERSION_SUFFIX}
    ExpressionEngine${FDO_VERSION_SUFFIX}
    RdbmsOverrides${FDO_VERSION_SUFFIX}
    PostgreSQLOverrides${FDO_VERSION_SUFFIX}
    PostgreSQLProvider${FDO_VERSION_SUFFIX}
)