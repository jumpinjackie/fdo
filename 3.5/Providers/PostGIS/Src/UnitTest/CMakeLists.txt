include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CPPUNIT_INCLUDE_DIR}
	${UNMANAGED_INCLUDE_DIR}
	${UTILITIES_COMMON_INCLUDE_DIR}
	${UTILITIES_TESTCOMMON_INCLUDE_DIR}
	${UTILITIES_OWS_INCLUDE_DIR}
	)

set( PostUnitTest_SRCS
	BaseTestCase.cpp
	CapabilitiesTest.cpp
	ConnectionInfoTest.cpp
	ConnectionTest.cpp
	Conv.cpp
	Pch.cpp
	PgGeometryTest.cpp
	StdAfx.cpp
	TestConfig.cpp
	UnitTest.cpp
)

add_executable( PostUnitTest ${PostUnitTest_SRCS} )

set_target_properties( PostUnitTest
	PROPERTIES OUTPUT_NAME "UnitTest" )

target_link_libraries( PostUnitTest
	FDO
	${CPPUNIT_LIBRARIES}
	ProvidersCommon
	TestCommon
	PostGISOverrides
	)

