include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${UNMANAGED_INCLUDE_DIR}
    ${UTILITIES_TESTCOMMON_INCLUDE_DIR}
    ${UTILITIES_COMMON_INCLUDE_DIR}
    ${UTILITIES_EXPRESSION_INCLUDE_DIR}
    ${CPPUNIT_INCLUDE_DIR}
)

# FIXME: Commented out sources files are direct subclasses of TestCommon implementations
# that are failing to build here
#
# Error is basically of the form: ‘CppUnit::TestFixture’ is an inaccessible base of $CLASSNAME
set( SQLiteUnitTest_SRCS
    UnitTest.cpp
    glibpatch.cpp
    FdoExpressionFunctionTest.cpp
    FdoMultiThreadTest.cpp
    InsertTest.cpp
    SelectTest.cpp
    SpatialContextTest.cpp
    SQLCmdTest.cpp
    TransactionTest.cpp
    UnitTestUtil.cpp
    ApplySchemaTest.cpp
    #ConstraintsTest.cpp
    DeleteTest.cpp
    ReaderTest.cpp
)

add_executable( SQLiteUnitTest ${SQLiteUnitTest_SRCS} )

set_target_properties( SQLiteUnitTest
    PROPERTIES OUTPUT_NAME "UnitTest" )

target_link_libraries( SQLiteUnitTest
    TestCommon
    ExpressionEngine-${FDO_VERSION}
    ProvidersCommon
    FDO-${FDO_VERSION}
    ${CPPUNIT_LIBRARIES}
    dl
)

# Ensure test data is copied to where the unit test binary is generated
file(GLOB UnitTestSQLite_TESTDATA
    "*.txt"
    "*.xml"
)
file(COPY ${UnitTestSQLite_TESTDATA} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
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
foreach(test ${SDFUnitTest_SRCS})
    if (test MATCHES "Test.cpp$")
        get_filename_component(TestName ${test} NAME_WE)
        message("  ######  adding SQLite Provider unit test fixture ${TestName} (${test})")
        add_test(NAME UnitTestSDF_${TestName} COMMAND UnitTest ${TestName})
    endif (test MATCHES "Test.cpp$")
endforeach(test)