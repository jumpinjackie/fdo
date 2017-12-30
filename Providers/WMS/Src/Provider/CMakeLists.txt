include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${UNMANAGED_INCLUDE_DIR}
    ${UTILITIES_COMMON_INCLUDE_DIR}
    ${UTILITIES_OWS_INCLUDE_DIR}
    ${GDAL_INCLUDE_DIR}
)

set( WMSProvider_SRCS
    FdoWmsCommandCapabilities.cpp
    FdoWmsConnectionCapabilities.cpp
    FdoWmsConnection.cpp
    FdoWmsConnectionInfo.cpp
    FdoWmsDataReader.cpp
    FdoWmsDescribeSchemaCommand.cpp
    FdoWmsExpressionCapabilities.cpp
    FdoWmsFeatureReader.cpp
    FdoWmsFilterCapabilities.cpp
    FdoWmsGeometryCapabilities.cpp
    FdoWmsRasterCapabilities.cpp
    FdoWmsSchemaCapabilities.cpp
    FdoWmsSelectCommand.cpp
    FdoWmsTopologyCapabilities.cpp
    FdoWmsUtils.cpp
    FdoWmsActivateSpatialContextCommand.cpp
    FdoWmsGetSpatialContextsCommand.cpp
    FdoWmsSpatialContextReader.cpp
    FdoWmsBandRasterGdal.cpp
    FdoWmsRasterGdal.cpp
    FdoWmsImage.cpp
    FdoWmsRasterPropertyDictionaryGdal.cpp
    FdoWmsStreamReaderGdalByRow.cpp
    FdoWmsDescribeSchemaMappingCommand.cpp
    FdoWmsDimension.cpp
    FdoWmsLayerCollection.cpp
    FdoWmsBoundingBoxCollection.cpp
    FdoWmsLayer.cpp
    FdoWmsBoundingBox.cpp
    FdoWmsServiceMetadata.cpp
    FdoWmsCapabilities.cpp
    FdoWmsStyleCollection.cpp
    FdoWmsDelegate.cpp
    FdoWmsStyle.cpp
    FdoWmsDimensionCollection.cpp
    FdoWmsRequestMetadata.cpp
    FdoWmsGetMap.cpp
    FdoWmsSelectAggregatesCommand.cpp
    FdoWmsSpatialExtentsAggregateReader.cpp
    FdoWmsGetFeatureClassCRSNames.cpp
    FdoWmsGetFeatureClassStyles.cpp
    FdoWmsGetImageFormats.cpp
    FdoWmsGetFeatureInfo.cpp
    FdoWmsGetFeatureInfoRequest.cpp
    FdoWmsGetFeatureInfoFormats.cpp
)

add_library( WMSProvider${FDO_VERSION_SUFFIX} SHARED ${WMSProvider_SRCS} )

target_link_libraries( WMSProvider${FDO_VERSION_SUFFIX}
    FDO${FDO_VERSION_SUFFIX}
    ProvidersCommon
    OWS${FDO_VERSION_SUFFIX}
    WMSOverrides${FDO_VERSION_SUFFIX}
    ${GDAL_LIBRARY}
    pthread
)

install( TARGETS WMSProvider${FDO_VERSION_SUFFIX} DESTINATION ${LIB_INSTALL_DIR} )
install_symlink( ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/libWMSProvider${FDO_VERSION_SUFFIX}.so libWMSProvider.so )
