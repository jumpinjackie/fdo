include_directories(
	BEFORE
	${CPPUNIT_INCLUDE_DIR}
	${UTILITIES_COMMON_INCLUDE_DIR}
	${UNMANAGED_INCLUDE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	)

add_definitions( -D__STDC_LIMIT_MACROS )

set( GDALUnitTest_SRCS
   XmlFormatter.cpp
   UnitTestUtil.cpp
   RfpNoConfigTest.cpp
   RfpOverridesSerializeTest.cpp
   RfpRasterConversion.cpp
   RfpTestExample1.cpp
   RfpTestGeotiff.cpp
   RfpTestGetRaster.cpp
   RfpTestMosaic.cpp
   RfpTestSpatialContext.cpp
   RfpTestThorough.cpp
   RfpUnitTest.cpp
   UnitTest.cpp
   UnitTestLinux.cpp
   RfpTestMrSID.cpp
   RfpTestBand.cpp
   RfpTestJp2.cpp
   RfpTestEcw.cpp
   RfpTestDem.cpp
   RfpTestBandConfig.cpp
   RfpTestPng.cpp
   RfpTestJpg.cpp
   RfpTestAAIGrid.cpp
   RfpTestSpatialQuery.cpp
)

add_executable( GDALUnitTest ${GDALUnitTest_SRCS} )

set_target_properties( GDALUnitTest
	PROPERTIES OUTPUT_NAME "UnitTest" )

target_link_libraries( GDALUnitTest
	GRFPOverrides
	ProvidersCommon
	${CPPUNIT_LIBRARIES}
	FDO
	xerces-c
	xalan-c
	xalanMsg
	)
