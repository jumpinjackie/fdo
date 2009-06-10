include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${UNMANAGED_INCLUDE_DIR}
	${UTILITIES_TESTCOMMON_INCLUDE_DIR}
	${UTILITIES_COMMON_INCLUDE_DIR}
	${UTILITIES_EXPRESSION_INCLUDE_DIR}
	${CPPUNIT_INCLUDE_DIR}
	)

set( SDFUnitTest_SRCS
   UnitTest.cpp
   glibpatch.cpp
   MasterTest.cpp
   ApplySchemaTest.cpp
   AssociationSchemaTest.cpp
   AssociationInsertTest.cpp
   AssociationSelectTest.cpp
   AssociationUpdateTest.cpp
   AssociationDeleteTest.cpp
   ConnectionInfoTest.cpp
#  ConstraintsTest.cpp
   ExtendedSelectTest.cpp
   FdoExpressionFunctionTest.cpp
   FdoMultiThreadTest.cpp
#  InsertTest.cpp
   SelectTest.cpp
   SchemaTest.cpp
   SpatialContextTest.cpp
   UnitTestUtil.cpp
   UserFunctionsTest.cpp
)

add_executable( SDFUnitTest ${SDFUnitTest_SRCS} )

set_target_properties( SDFUnitTest
	PROPERTIES OUTPUT_NAME "UnitTest" )

target_link_libraries( SDFUnitTest
	TestCommon
	ExpressionEngine
	ProvidersCommon
	FDO
	${CPPUNIT_LIBRARIES}
	dl
	xalan-c
	xalanMsg
	xerces-c
	)
