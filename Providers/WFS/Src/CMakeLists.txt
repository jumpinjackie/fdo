include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/Provider
	${CMAKE_CURRENT_BINARY_DIR}/Provider
	${FDOUTILITIES}/Common/Inc
	${FDOUTILITIES}/OWS/Inc
	)

add_subdirectory( Message )

set( WFSProviderSrc_SRCS
    Provider/FdoWfsCommandCapabilities.cpp
    Provider/FdoWfsConnectionCapabilities.cpp
    Provider/FdoWfsConnection.cpp
    Provider/FdoWfsConnectionInfo.cpp
    Provider/FdoWfsDataReader.cpp
    Provider/FdoWfsDelegate.cpp
    Provider/FdoWfsDescribeFeatureType.cpp
    Provider/FdoWfsDescribeSchemaCommand.cpp
    Provider/FdoWfsExpressionCapabilities.cpp
    Provider/FdoWfsFeatureReader.cpp
    Provider/FdoWfsFeatureType.cpp
    Provider/FdoWfsFeatureTypeList.cpp
    Provider/FdoWfsFilterCapabilities.cpp
    Provider/FdoWfsGeometryCapabilities.cpp
    Provider/FdoWfsGetFeature.cpp
    Provider/FdoWfsGetSpatialContexts.cpp
    Provider/FdoWfsOgcFilterCapabilities.cpp
    Provider/FdoWfsOgcScalarCapabilities.cpp
    Provider/FdoWfsOgcSpatialCapabilities.cpp
    Provider/FdoWfsRasterCapabilities.cpp
    Provider/FdoWfsSchemaCapabilities.cpp
    Provider/FdoWfsSchemaCopyHandler.cpp
    Provider/FdoWfsSchemaMerger.cpp
    Provider/FdoWfsSchemaRefHandler.cpp
    Provider/FdoWfsSelectAggregatesCommand.cpp
    Provider/FdoWfsSelectCommand.cpp
    Provider/FdoWfsServiceMetadata.cpp
    Provider/FdoWfsSpatialContextReader.cpp
    Provider/FdoWfsTopologyCapabilities.cpp
    Provider/FdoWfsUtils.cpp
    Provider/stdafx.cpp
    Provider/FdoWfsSpatialExtentsAggregateReader.cpp
)


set( WFSProvider_LIB_SRCS
	${WFSProviderSrc_SRCS}
)

add_library( WFSProvider SHARED ${WFSProvider_LIB_SRCS} )
add_dependencies( WFSProvider WFSMessage )

target_link_libraries( WFSProvider
	FDO
	ProvidersCommon
	FdoOws
	pthread
 )

set_target_properties( WFSProvider PROPERTIES VERSION ${FDO_VERSION} SOVERSION ${FDO_VERSION_MAJOR} )
install( TARGETS WFSProvider DESTINATION ${LIB_INSTALL_DIR} )
