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

add_definitions( -D__STDC_LIMIT_MACROS -DRDBI_STATIC )

set( CommonUnitTests_STAT_SRCS
	UnitTest.cpp
	glibpatch.cpp
	ConnectionUtil.cpp
	ConnectTests.cpp
	DataTypeTests.cpp
	DescribeSchemaTest.cpp
	FdoAdvancedSelectTest.cpp
	FdoApplySchemaTest.cpp
	FdoAssociationDeleteTest.cpp
	FdoAssociationInsertTest.cpp
	FdoAssociationSchemaTest.cpp
	FdoAssociationSelectTest.cpp
	FdoAssociationUpdateTest.cpp
	FdoCapabilityTest.cpp
	FdoConnectTest.cpp
	FdoConstraintsTest.cpp
	FdoDatastoreTest.cpp
	FdoDeleteTest.cpp
	FdoFilterTest.cpp
	FdoExpressionFunctionTest.cpp
	FdoForeignSchemaTest.cpp
	FdoImportExportTest.cpp
	FdoInsertTest.cpp
	FdoMultiThreadTest.cpp
	FdoSchemaRollbackTest.cpp
	FdoSchemaTest.cpp
	FdoSelectTest.cpp
	FdoSqlCmdTest.cpp
	FdoUpdateTest.cpp
	GeometryTests.cpp
	MessageTest.cpp
	SchemaMgrTests.cpp
	SchemaOverrideUtil.cpp
	SelectTests.cpp
	XmlFormatter.cpp
	UnitTestUtil.cpp
)

add_library( CommonUnitTests STATIC ${CommonUnitTests_STAT_SRCS} )
