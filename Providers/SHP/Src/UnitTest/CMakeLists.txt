include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../ShpRead
    ${CMAKE_CURRENT_SOURCE_DIR}/../ShpSpatialIndex
    ${CPPUNIT_INCLUDE_DIR}
    ${UNMANAGED_INCLUDE_DIR}
    ${UTILITIES_COMMON_INCLUDE_DIR}
    ${UTILITIES_TESTCOMMON_INCLUDE_DIR}
    ${UTILITIES_OWS_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../Provider
)

add_definitions( -D__STDC_LIMIT_MACROS )

# FIXME: Commented out sources files are direct subclasses of TestCommon implementations
# that are failing to build here
#
# Error is basically of the form: ‘CppUnit::TestFixture’ is an inaccessible base of $CLASSNAME
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
    FDO-${FDO_VERSION}
    ${CPPUNIT_LIBRARIES}
    ProvidersCommon
    TestCommon
    SHPOverrides-${FDO_VERSION}
    SHPRead
    SHPSpatialIndex
)

# Ensure test data is copied to where the unit test binary is generated
file(GLOB UnitTestSHP_TESTDATA
    "*.txt"
    "*.xml"
)
file(COPY ${UnitTestSHP_TESTDATA} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# Remove providers.xml from output as that is windows-only
file(REMOVE "${CMAKE_CURRENT_BINARY_DIR}/providers.xml")

# It so happens that by convention all unit test implementation sources end with "Test"
# and the cppunit host executable can selectively run certain test fixtures by passing the
# name of the fixtures to run, which happens to be the same name as the source file, making
# CTest integration really simple.
#
# The only annoyance is that the true test case count is not shown in CTest as it counts
# based on the number of add_test() calls we make (ie. The number of fixtures discovered and
# registered)
foreach(test ${UnitTest_SRCS})
    if (test MATCHES "Test[s]*.cpp$")
        get_filename_component(TestName ${test} NAME_WE)
        message("  ######  adding SHP Provider unit test fixture ${TestName} (${test})")
        add_test(NAME UnitTestSHP_${TestName} COMMAND UnitTest ${TestName})
    endif (test MATCHES "Test[s]*.cpp$")
endforeach(test)