include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/Inc
	${FDOUTILITIES}/Common/Inc
	${FDO}/Unmanaged/Inc
	)

if( LOCAL_BOOST )
	include_directories( ${FDOTHIRDPARTY}/boost )
endif( LOCAL_BOOST )

if( LOCAL_CURL )
	include_directories( ${FDOTHIRDPARTY}/libcurl/include )
endif( LOCAL_CURL )

if( UNIX )
	add_definitions( -DLINUX_IA32 -pthread )
endif( UNIX )

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
)


set( FdoOws_LIB_SRCS
	${FdoOwsSrc_SRCS}
)

link_directories( ${Boost_LIBRARY_DIRS} )

add_library( FdoOws SHARED ${FdoOws_LIB_SRCS} )

target_link_libraries( FdoOws
	FDO
	ProvidersCommon
	boost_thread-mt
	${CURL_LIBRARIES}
	${OPENSSL_LIBRARIES}
	${ZLIB_LIBRARIES}
	pthread
 )

set_target_properties( FdoOws PROPERTIES VERSION ${FDO_VERSION} SOVERSION ${FDO_VERSION_MAJOR} )
install( TARGETS FdoOws DESTINATION ${LIB_INSTALL_DIR} )

