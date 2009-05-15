include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
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

set( FdoOwsSrc_STAT_SRCS
   FdoOwsCapabilities.cpp
   FdoOwsRequest.cpp
   FdoOwsDelegate.cpp
   FdoOwsRequestMetadata.cpp
   FdoOwsExceptionReport.cpp
   FdoOwsResponse.cpp
   FdoOwsServiceIdentification.cpp
   FdoOwsGetCapabilities.cpp
   FdoOwsServiceMetadata.cpp
   FdoOwsHttpHandler.cpp
   FdoOwsUrlResolver.cpp
   FdoOwsIHttpHandler.cpp
   FdoOwsOgcFilterSerializer.cpp
   FdoOwsGeographicBoundingBox.cpp
   FdoOwsGeographicBoundingBoxCollection.cpp
   FdoOwsOgcGeometrySerializer.cpp
)

add_library( FdoOwsSrc STATIC ${FdoOwsSrc_STAT_SRCS} )

