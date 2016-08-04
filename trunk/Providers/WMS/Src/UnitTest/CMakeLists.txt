include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${UNMANAGED_INCLUDE_DIR}
	${UTILITIES_COMMON_INCLUDE_DIR}
	${CPPUNIT_INCLUDE_DIR}
	)

add_definitions( -D__STDC_LIMIT_MACROS )

set( UnitTest_SRCS
   WmsTests.cpp
   glibpatch.cpp
   ConnectTests.cpp
   CapabilityTest.cpp
   UnitTest.cpp
   OverridesTest.cpp
   XmlFormatter.cpp
   UnitTestUtil.cpp
   WmsTestSelect.cpp
   WmsTestDescribeSchema.cpp
   WmsTestSpatialContexts.cpp
   WmsTestCustomCommands.cpp
)

add_executable( WMSUnitTest ${UnitTest_SRCS} )
set_target_properties( WMSUnitTest 
	PROPERTIES OUTPUT_NAME UnitTest )

target_link_libraries( WMSUnitTest
	WMSProvider-${FDO_VERSION}
	WMSOverrides-${FDO_VERSION}
	ProvidersCommon
	${CPPUNIT_LIBRARIES}
	FDO-${FDO_VERSION}
	pthread_nonshared
	pthread
	)
