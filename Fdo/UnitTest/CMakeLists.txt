include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CPPUNIT_INCLUDE_DIR}
	)

set( UnitTest_SRCS
   	app.cpp
    CapabilitiesTest.cpp
    ClientServicesTest.cpp
    CommandTest.cpp
    CommonTest.cpp
    ExpressionParseTest.cpp
    ExpressionTest.cpp
    FilterParseTest.cpp
    FilterTest.cpp
    GeometryTest.cpp
    GeometryMemPerfTest.cpp
    IoTest.cpp
    Pch.cpp
    SchemaTest.cpp
    SpatialContextTest.cpp
    TestProcessor.cpp
    UnitTestUtil.cpp
    XmlFormatter.cpp
    XmlTest.cpp
    GmlTest.cpp
    GmlWriteTest.cpp
)

link_directories( ${CPPUNIT_LIBRARY_DIR} )

add_executable( UnitTest ${UnitTest_SRCS} )
add_dependencies( UnitTest FDOMessage )

if( LOCAL_CPPUNIT )
	add_dependencies( UnitTest Internal_Cppunit )
endif( LOCAL_CPPUNIT )

target_link_libraries( UnitTest
	cppunit
	FDO
	dl
	)
