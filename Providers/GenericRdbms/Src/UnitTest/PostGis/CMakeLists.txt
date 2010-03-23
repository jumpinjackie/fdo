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

set( PostGisUnitTests_STAT_SRCS
	PostGisConnectionUtil.cpp
	PostGisConnectTests.cpp
	PostGisDescribeSchemaTest.cpp
	PostGisFdoApplySchemaTest.cpp
	PostGisFdoConnectionInfoTest.cpp
	PostGisFdoConnectTest.cpp
	PostGisFdoConstraintsTest.cpp
	PostGisFdoDatastoreTest.cpp
	PostGisFdoDeleteTest.cpp
	PostGisFdoImportExportTest.cpp
	PostGisFdoInsertTest.cpp
	PostGisFdoMultiThreadTest.cpp
	PostGisFdoSelectTest.cpp
	PostGisFdoSqlCmdTest.cpp
	PostGisFdoUpdateTest.cpp
	PostGisSchemaMgrTests.cpp
	PostGisSchemaOverrideUtil.cpp
)

add_library( PostGisUnitTests STATIC ${PostGisUnitTests_STAT_SRCS} )
