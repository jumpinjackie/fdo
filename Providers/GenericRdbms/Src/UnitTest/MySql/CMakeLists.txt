include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${UNMANAGED_INCLUDE_DIR}
	${CPPUNIT_INCLUDE_DIR}
	${UTILITIES_SCHEMAMGR_INCLUDE_DIR}
	${UTILITIES_COMMON_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../../LockManager
	${CMAKE_CURRENT_SOURCE_DIR}/../../LongTransactionManager
	${CMAKE_CURRENT_SOURCE_DIR}/../../Gdbi
	${CMAKE_CURRENT_SOURCE_DIR}/../../Fdo/Connection
	${CMAKE_CURRENT_SOURCE_DIR}/../../Fdo/SpatialContext
	${CMAKE_CURRENT_SOURCE_DIR}/../../Fdo/Other
	${CMAKE_CURRENT_SOURCE_DIR}/../../Fdo/FeatureCommands
	${UTILITIES_TESTCOMMON_INCLUDE_DIR}
	)

add_definitions( -DRDBI_STATIC )

set( MySqlUnitTests_STAT_SRCS
	MySqlConnectionUtil.cpp
	MySqlConnectTests.cpp
	MySqlDataTypeTests.cpp
	MySqlDescribeSchemaTest.cpp
	MySqlFdoAdvancedSelectTest.cpp
	MySqlFdoApplySchemaTest.cpp
	MySqlFdoAssociationDeleteTest.cpp
	MySqlFdoAssociationInsertTest.cpp
	MySqlFdoAssociationSchemaTest.cpp
	MySqlFdoAssociationSelectTest.cpp
	MySqlFdoAssociationUpdateTest.cpp
	MySqlFdoCapabilityTest.cpp
	MySqlFdoConnectionInfoTest.cpp
	MySqlFdoConnectTest.cpp
	MySqlFdoConstraintsTest.cpp
	MySqlFdoDatastoreTest.cpp
	MySqlFdoDeleteTest.cpp
	MySqlFdoFilterTest.cpp
	MySqlFdoExpressionFunctionTest.cpp
	MySqlFdoForeignSchemaTest.cpp
	MySqlFdoImportExportTest.cpp
	MySqlFdoInsertTest.cpp
	MySqlFdoMultiThreadTest.cpp
	MySqlFdoSchemaRollbackTest.cpp
	MySqlFdoSchemaTest.cpp
	MySqlFdoSelectTest.cpp
	MySqlFdoSqlCmdTest.cpp
	MySqlFdoUpdateTest.cpp
	MySqlGeometryTests.cpp
	MySqlSchemaMgrTests.cpp
	MySqlSchemaOverrideUtil.cpp
	MySqlSelectTests.cpp
)

add_library( MySqlUnitTests STATIC ${MySqlUnitTests_STAT_SRCS} )
