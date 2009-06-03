include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../ShpRead
	${CMAKE_CURRENT_SOURCE_DIR}/../ShpSpatialIndex
	${CPPUNIT_INCLUDE_DIR}
	${FDO}/Unmanaged/Inc
	${FDOUTILITIES}/Common/Inc
	${FDOUTILITIES}/TestCommon/Inc
	${FDOUTILITIES}/OWS/Inc
	${CMAKE_CURRENT_SOURCE_DIR}/../Provider
	)

add_definitions( -D__STDC_LIMIT_MACROS )

set( UnitTest_SRCS
   UnitTest.cpp
   glibpatch.cpp
   ShpTests.cpp
   FileAccessTests.cpp
   ConnectTests.cpp
   ConnectionInfoTests.cpp
   IndexTests.cpp
   SelectTests.cpp
   SelectAggregatesTests.cpp
   InsertTests.cpp
   # InsertTests2.cpp
   DeleteTests.cpp
   SchemaTests.cpp
   OverridesTest.cpp
   FilterTests.cpp
   SpatialFilterTests.cpp
   XmlFormatter.cpp
   PerformanceTests.cpp
   UpdateTests.cpp
   FidelityTests.cpp
   BigPerformanceTests.cpp
   ExtendedSelectTests.cpp
   FdoExpressionFunctionTest.cpp
)

add_executable( SHPUnitTest ${UnitTest_SRCS} )

set_target_properties( SHPUnitTest
	PROPERTIES OUTPUT_NAME "UnitTest" )

target_link_libraries( SHPUnitTest
	FDO
	xerces-c
	xalan-c
	xalanMsg
	${CPPUNIT_LIBRARIES}
	ProvidersCommon
	TestCommon
	SHPOverrides
	SHPRead
	SHPSpatialIndex
	)

