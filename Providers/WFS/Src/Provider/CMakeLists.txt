include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/inc
	${UNMANAGED_INCLUDE_DIR}
	${UTILITIES_COMMON_INCLUDE_DIR}
	${UTILITIES_OWS_INCLUDE_DIR}
	)

set( WFSProvider_SRCS
    FdoWfsCommandCapabilities.cpp
    FdoWfsConnectionCapabilities.cpp
    FdoWfsConnection.cpp
    FdoWfsConnectionInfo.cpp
    FdoWfsDataReader.cpp
    FdoWfsDelegate.cpp
    FdoWfsDescribeFeatureType.cpp
    FdoWfsDescribeSchemaCommand.cpp
    FdoWfsExpressionCapabilities.cpp
    FdoWfsFeatureReader.cpp
    FdoWfsFeatureType.cpp
    FdoWfsFeatureTypeList.cpp
    FdoWfsFilterCapabilities.cpp
    FdoWfsGeometryCapabilities.cpp
    FdoWfsGetFeature.cpp
    FdoWfsGetSpatialContexts.cpp
    FdoWfsOgcFilterCapabilities.cpp
    FdoWfsOgcScalarCapabilities.cpp
    FdoWfsOgcSpatialCapabilities.cpp
    FdoWfsRasterCapabilities.cpp
    FdoWfsSchemaCapabilities.cpp
    FdoWfsSchemaCopyHandler.cpp
    FdoWfsSchemaMerger.cpp
    FdoWfsSchemaRefHandler.cpp
    FdoWfsSelectAggregatesCommand.cpp
    FdoWfsSelectCommand.cpp
    FdoWfsServiceMetadata.cpp
    FdoWfsSpatialContextReader.cpp
    FdoWfsTopologyCapabilities.cpp
    FdoWfsUtils.cpp
    stdafx.cpp
    FdoWfsSpatialExtentsAggregateReader.cpp
)

add_library( WFSProvider SHARED ${WFSProvider_SRCS} )
add_dependencies( WFSProvider WFSMessage )

target_link_libraries( WFSProvider
	FDO
	ProvidersCommon
	FdoOws
	pthread
 )

set_target_properties( WFSProvider PROPERTIES VERSION ${FDO_VERSION} SOVERSION ${FDO_VERSION_MAJOR} )
install( TARGETS WFSProvider DESTINATION ${LIB_INSTALL_DIR} )
