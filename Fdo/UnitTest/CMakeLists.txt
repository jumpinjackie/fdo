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

# HACK: Workaround a bug in system-installed xalan
#
# https://bugs.debian.org/cgi-bin/bugreport.cgi?bug=718303
# https://issues.apache.org/jira/browse/XALANC-751
#
# TODO: We ideally should version check known distros that have
# the fix. Also if/when we merge the fix into our internal copy
# of xalan, we should also check if we're building against system
# or internal copy of xalan
add_definitions(-DHAS_XALANC_751_BUG)

link_directories( ${CPPUNIT_LIBRARY_DIR} )

add_executable( UnitTest ${UnitTest_SRCS} )
add_sanitizers( UnitTest )
add_dependencies( UnitTest FDOMessage )

if( LOCAL_CPPUNIT )
    add_dependencies( UnitTest Internal_Cppunit )
endif( LOCAL_CPPUNIT )

target_link_libraries( UnitTest
    cppunit
    FDO${FDO_VERSION_SUFFIX}
    dl
)

# Ensure test data is copied to where the unit test binary is generated
file(GLOB UnitTestFdo_TESTDATA
    "*.txt"
    "*.xml"
)
file(COPY ${UnitTestFdo_TESTDATA} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})