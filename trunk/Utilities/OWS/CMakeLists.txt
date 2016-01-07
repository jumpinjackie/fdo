include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/Inc
	${UNMANAGED_INCLUDE_DIR}
	${UTILITIES_COMMON_INCLUDE_DIR}
	)

if( LOCAL_BOOST )
	include_directories( ${FDOTHIRDPARTY}/boost )
endif( LOCAL_BOOST )

if( LOCAL_CURL )
	include_directories( ${FDOTHIRDPARTY}/libcurl/include )
endif( LOCAL_CURL )

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

if( NOT LOCAL_BOOST )
	link_directories( ${Boost_LIBRARY_DIRS} )
endif( NOT LOCAL_BOOST )

add_library( FdoOws-${FDO_VERSION} SHARED ${FdoOws_LIB_SRCS} )

if( LOCAL_OPENSSL )
target_link_libraries( FdoOws-${FDO_VERSION}
	FDO-${FDO_VERSION}
	ProvidersCommon
	${Boost_THREAD_LIBRARY}
	${CURL_LIBRARIES}
	${OPENSSL_LIBRARIES}
	${ZLIB_LIBRARIES}
	pthread
 )
else ( LOCAL_OPENSSL )
target_link_libraries( FdoOws-${FDO_VERSION}
	FDO-${FDO_VERSION}
	ProvidersCommon
	${Boost_THREAD_LIBRARY}
	${CURL_LIBRARIES}
	${OPENSSL_LIBRARIES}
	${ZLIB_LIBRARIES}
	pthread
 )
endif ( LOCAL_OPENSSL )

install( TARGETS FdoOws-${FDO_VERSION} DESTINATION ${LIB_INSTALL_DIR} )

