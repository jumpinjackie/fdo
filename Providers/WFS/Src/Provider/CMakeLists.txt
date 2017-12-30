include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${UNMANAGED_INCLUDE_DIR}
    ${UTILITIES_COMMON_INCLUDE_DIR}
    ${UTILITIES_OWS_INCLUDE_DIR}
    ${UTILITIES_EXPRESSION_INCLUDE_DIR}
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

add_library( WFSProvider${FDO_VERSION_SUFFIX} SHARED ${WFSProvider_SRCS} )
add_dependencies( WFSProvider${FDO_VERSION_SUFFIX} WFSMessage )

target_link_libraries( WFSProvider${FDO_VERSION_SUFFIX}
    FDO${FDO_VERSION_SUFFIX}
    ProvidersCommon
    ExpressionEngine${FDO_VERSION_SUFFIX}
    OWS${FDO_VERSION_SUFFIX}
    pthread
)

install( TARGETS WFSProvider${FDO_VERSION_SUFFIX} DESTINATION ${LIB_INSTALL_DIR} COMPONENT ${FDO_COMPONENT} )
install_symlink( ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/libWFSProvider${FDO_VERSION_SUFFIX}.so libWFSProvider.so ${FDO_COMPONENT} )
