include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${UNMANAGED_INCLUDE_DIR}
    ${UTILITIES_TESTCOMMON_INCLUDE_DIR}
    ${UTILITIES_COMMON_INCLUDE_DIR}
    ${UTILITIES_EXPRESSION_INCLUDE_DIR}
    ${CPPUNIT_INCLUDE_DIR}
)

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
    ConstraintsTest.cpp
    DeleteTest.cpp
    ReaderTest.cpp
)

add_executable( SQLiteUnitTest ${SQLiteUnitTest_SRCS} )

set_target_properties( SQLiteUnitTest
    PROPERTIES OUTPUT_NAME "UnitTest" )

target_link_libraries( SQLiteUnitTest
    TestCommon
    ExpressionEngine${FDO_VERSION_SUFFIX}
    ProvidersCommon
    FDO${FDO_VERSION_SUFFIX}
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