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
add_sanitizers( OGRUnitTest )
set_target_properties( OGRUnitTest 
    PROPERTIES OUTPUT_NAME UnitTest )

target_link_libraries( OGRUnitTest
    OGRProvider${FDO_VERSION_SUFFIX}
    TestCommon
    ProvidersCommon
    ${CPPUNIT_LIBRARIES}
    FDO${FDO_VERSION_SUFFIX}
)

# Remove providers.xml from output as that is windows-only
file(REMOVE "${CMAKE_CURRENT_BINARY_DIR}/providers.xml")

# Replace with our own configured version
set(FDO_PROVIDER_NAME "OSGeo.OGR")
set(FDO_PROVIDER_LIBRARY "../Provider/libOGRProvider${FDO_VERSION_SUFFIX}.so")
configure_file(${CMAKE_MODULE_PATH}/../configs/providers.xml.in ${CMAKE_CURRENT_BINARY_DIR}/providers.xml)