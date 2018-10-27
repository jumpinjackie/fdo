include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Inc
    ${UNMANAGED_INCLUDE_DIR}
    ${UTILITIES_COMMON_INCLUDE_DIR}
)

if( LOCAL_CURL )
    include_directories( ${FDOTHIRDPARTY}/libcurl/include )
endif( LOCAL_CURL )

if (CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-pthread)
endif (CMAKE_COMPILER_IS_GNUCXX)

set( FdoOwsSrc_SRCS
    Src/FdoOwsCapabilities.cpp
    Src/FdoOwsRequest.cpp
    Src/FdoOwsDelegate.cpp
    Src/FdoOwsRequestMetadata.cpp
    Src/FdoOwsExceptionReport.cpp
    Src/FdoOwsResponse.cpp
    Src/FdoOwsServiceIdentification.cpp
    Src/FdoOwsGetCapabilities.cpp
    Src/FdoOwsServiceMetadata.cpp
    Src/FdoOwsHttpHandler.cpp
    Src/FdoOwsUrlResolver.cpp
    Src/FdoOwsIHttpHandler.cpp
    Src/FdoOwsOgcFilterSerializer.cpp
    Src/FdoOwsGeographicBoundingBox.cpp
    Src/FdoOwsGeographicBoundingBoxCollection.cpp
    Src/FdoOwsOgcGeometrySerializer.cpp
    Src/FdoOwsOperationsMetadata.cpp
    Src/FdoOwsServiceProvider.cpp
)

set( FdoOws_LIB_SRCS
    ${FdoOwsSrc_SRCS}
)

add_library( OWS${FDO_VERSION_SUFFIX} SHARED ${FdoOws_LIB_SRCS} )

target_link_libraries( OWS${FDO_VERSION_SUFFIX}
    FDO${FDO_VERSION_SUFFIX}
    ProvidersCommon
    ${CURL_LIBRARIES}
    ${OPENSSL_LIBRARIES}
    ${ZLIB_LIBRARIES}
    rt
)

install( TARGETS OWS${FDO_VERSION_SUFFIX} DESTINATION ${LIB_INSTALL_DIR} COMPONENT ${FDO_COMPONENT} )
install_symlink( ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/libOWS${FDO_VERSION_SUFFIX}.so libOWS.so COMPONENT ${FDO_COMPONENT} )
