include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${UNMANAGED_INCLUDE_DIR}
    ${UTILITIES_COMMON_INCLUDE_DIR}
    ${CPPUNIT_INCLUDE_DIR}
)

add_definitions( -D__STDC_LIMIT_MACROS )

set( UnitTest_SRCS
   WmsTests.cpp
   glibpatch.cpp
   ConnectTests.cpp
   CapabilityTest.cpp
   UnitTest.cpp
   OverridesTest.cpp
   XmlFormatter.cpp
   UnitTestUtil.cpp
   WmsTestSelect.cpp
   WmsTestDescribeSchema.cpp
   WmsTestSpatialContexts.cpp
   WmsTestCustomCommands.cpp
)

add_executable( WMSUnitTest ${UnitTest_SRCS} )
add_sanitizers( WMSUnitTest )
set_target_properties( WMSUnitTest 
    PROPERTIES OUTPUT_NAME UnitTest )

target_link_libraries( WMSUnitTest
    WMSProvider${FDO_VERSION_SUFFIX}
    WMSOverrides${FDO_VERSION_SUFFIX}
    ProvidersCommon
    ${CPPUNIT_LIBRARIES}
    FDO${FDO_VERSION_SUFFIX}
    pthread_nonshared
    pthread
)

# Ensure test data is copied to where the unit test binary is generated
file(GLOB UnitTestWMS_TESTDATA
    "*.txt"
    "*.xml"
)
file(COPY ${UnitTestWMS_TESTDATA} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# Remove providers.xml from output as that is windows-only
file(REMOVE "${CMAKE_CURRENT_BINARY_DIR}/providers.xml")