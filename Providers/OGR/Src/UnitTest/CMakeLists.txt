include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${UNMANAGED_INCLUDE_DIR}
    ${UTILITIES_TESTCOMMON_INCLUDE_DIR}
    ${UTILITIES_COMMON_INCLUDE_DIR}
    ${UTILITIES_EXPRESSION_INCLUDE_DIR}
    ${CPPUNIT_INCLUDE_DIR}
)

add_definitions( -D__STDC_LIMIT_MACROS )

set( UnitTest_SRCS
    ConnectTests.cpp
    DeleteTests.cpp
    FdoExpressionFunctionTest.cpp
    glibpatch.cpp
    InsertTests.cpp
    SelectTests.cpp
    UnitTest.cpp
    UpdateTests.cpp
    VrtTests.cpp
)

add_executable( OGRUnitTest ${UnitTest_SRCS} )
set_target_properties( OGRUnitTest 
    PROPERTIES OUTPUT_NAME UnitTest )

target_link_libraries( OGRUnitTest
    OGRProvider-${FDO_VERSION}
    TestCommon
    ProvidersCommon
    ${CPPUNIT_LIBRARIES}
    FDO-${FDO_VERSION}
)

# It so happens that by convention all unit test implementation sources end with "Test"
# and the cppunit host executable can selectively run certain test fixtures by passing the
# name of the fixtures to run, which happens to be the same name as the source file, making
# CTest integration really simple.
#
# The only annoyance is that the true test case count is not shown in CTest as it counts
# based on the number of add_test() calls we make (ie. The number of fixtures discovered and
# registered)
foreach(test ${SDFUnitTest_SRCS})
    if (test MATCHES "Test[s]*.cpp$")
        get_filename_component(TestName ${test} NAME_WE)
        message("  ######  adding OGR Provider unit test fixture ${TestName} (${test})")
        add_test(NAME UnitTestSDF_${TestName} COMMAND UnitTest ${TestName})
    endif (test MATCHES "Test[s]*.cpp$")
endforeach(test)