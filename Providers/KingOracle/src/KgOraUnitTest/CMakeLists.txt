include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../Provider
    ${CMAKE_CURRENT_SOURCE_DIR}/../Overrides
    ${CMAKE_CURRENT_SOURCE_DIR}/../../inc
    ${CMAKE_CURRENT_BINARY_DIR}
    ${UNMANAGED_INCLUDE_DIR}
    ${UTILITIES_TESTCOMMON_INCLUDE_DIR}
    ${UTILITIES_COMMON_INCLUDE_DIR}
    ${UTILITIES_EXPRESSION_INCLUDE_DIR}
    ${CPPUNIT_INCLUDE_DIR}
    ${ORACLE_INCLUDES}
)

set( KgOraUnitTest_SRCS
    main.cpp
    glibpatch.cpp
    UnitTestUtil.cpp
    DataTypeTests.cpp
    FilterProcessorTests.cpp
    GeometryTests.cpp
    InsertUpdateDeleteTests.cpp
    OCITests.cpp
    SelectOrderByTests.cpp
)

add_executable( KgOraUnitTest ${KgOraUnitTest_SRCS} )
add_sanitizers( KgOraUnitTest )
set_target_properties( KgOraUnitTest
    PROPERTIES OUTPUT_NAME "KgOraUnitTest" )

target_link_libraries( KgOraUnitTest
    TestCommon
    ExpressionEngine${FDO_VERSION_SUFFIX}
    ProvidersCommon
    FDO${FDO_VERSION_SUFFIX}
    KingOracleProvider${FDO_VERSION_SUFFIX}
    KingOracleOverrides${FDO_VERSION_SUFFIX}
    ${CPPUNIT_LIBRARIES}
    ${ORACLE_LIBRARIES}
    dl
)