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
    FDO-${FDO_VERSION}
    dl
)

# Ensure test data is copied to where the unit test binary is generated
file(GLOB UnitTestFdo_TESTDATA
    "*.txt"
    "*.xml"
)
file(COPY ${UnitTestFdo_TESTDATA} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# It so happens that by convention all unit test implementation sources end with "Test"
# and the cppunit host executable can selectively run certain test fixtures by passing the
# name of the fixtures to run, which happens to be the same name as the source file, making
# CTest integration really simple.
#
# The only annoyance is that the true test case count is not shown in CTest as it counts
# based on the number of add_test() calls we make (ie. The number of fixtures discovered and
# registered)
foreach(test ${UnitTest_SRCS})
    if (test MATCHES "Test.cpp$")
        get_filename_component(TestName ${test} NAME_WE)
        message("  ######  adding FDO unit test fixture ${TestName} (${test})")
        add_test(NAME UnitTestFdo_${TestName} COMMAND UnitTest ${TestName})
    endif (test MATCHES "Test.cpp$")
endforeach(test)