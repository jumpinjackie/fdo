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

add_definitions( -D__USE_GNU -DLINUX -DLINUX_IA32 -DRDBI_STATIC  )

set( OdbcUnitTests_STAT_SRCS
	OdbcBaseSetup.cpp
	OdbcConnectionUtil.cpp
	OdbcDescribeSchemaTest.cpp
	OdbcFdoAdvancedSelectTest.cpp
	OdbcFdoConnectionInfoTest.cpp
	OdbcFdoConnectTest.cpp
	OdbcFdoDeleteTest.cpp
	OdbcFdoInsertTest.cpp
	OdbcFdoMultiThreadTest.cpp
	OdbcFdoSchemaTest.cpp
	OdbcFdoSelectTest.cpp
	OdbcFdoSqlCmdTest.cpp
	OdbcFdoUpdateTest.cpp
)

add_library( OdbcUnitTests STATIC ${OdbcUnitTests_STAT_SRCS} )
