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
    ConstraintsTest.cpp
    ExtendedSelectTest.cpp
    FdoExpressionFunctionTest.cpp
    FdoMultiThreadTest.cpp
    InsertTest.cpp
    SelectTest.cpp
    SchemaTest.cpp
    SpatialContextTest.cpp
    UnitTestUtil.cpp
    UserFunctionsTest.cpp
)

add_executable( SDFUnitTest ${SDFUnitTest_SRCS} )
add_sanitizers( SDFUnitTest )
set_target_properties( SDFUnitTest
    PROPERTIES OUTPUT_NAME "UnitTest" )

target_link_libraries( SDFUnitTest
    TestCommon
    ExpressionEngine${FDO_VERSION_SUFFIX}
    ProvidersCommon
    FDO${FDO_VERSION_SUFFIX}
    ${CPPUNIT_LIBRARIES}
    dl
)

# Ensure test data is copied to where the unit test binary is generated
file(GLOB UnitTestSDF_TESTDATA
    "*.txt"
    "*.xml"
)
file(COPY ${UnitTestSDF_TESTDATA} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# Remove providers.xml from output as that is windows-only
file(REMOVE "${CMAKE_CURRENT_BINARY_DIR}/providers.xml")

# Replace with our own configured version
set(FDO_PROVIDER_NAME "OSGeo.SDF")
set(FDO_PROVIDER_LIBRARY "../Provider/libSDFProvider${FDO_VERSION_SUFFIX}.so")
configure_file(${CMAKE_MODULE_PATH}/../configs/providers.xml.in ${CMAKE_CURRENT_BINARY_DIR}/providers.xml)