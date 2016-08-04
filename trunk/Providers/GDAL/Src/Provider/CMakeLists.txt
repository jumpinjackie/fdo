include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}/../Message/Inc/
	${UNMANAGED_INCLUDE_DIR}
	${UTILITIES_COMMON_INCLUDE_DIR}
	${GDAL_INCLUDE_DIR}
	)

set( GRFPProvider_SRCS
   FdoRfpClassData.cpp
   FdoRfpCommandCapabilities.cpp
   FdoRfpConnection.cpp
   FdoRfpConnectionCapabilities.cpp
   FdoRfpConnectionInfo.cpp
   FdoRfpDataReader.cpp
   FdoRfpDatasetCache.cpp
   FdoRfpDescribeSchemaCommand.cpp
   FdoRfpDescribeSchemaMapping.cpp
   FdoRfpExpressionCapabilities.cpp
   FdoRfpFeatureReader.cpp
   FdoRfpFilterCapabilities.cpp
   FdoRfpFilterEvaluator.cpp
   FdoRfpGeoRasterExtractor.cpp
   FdoRfpGetSpatialContexts.cpp
   FdoRfpImage.cpp
   FdoRfpImageFactory.cpp
   FdoRfpQueryExecutor.cpp
   FdoRfpRasterCapabilities.cpp
   FdoRfpRasterPropertyDictionary.cpp
   FdoRfpRasterUtil.cpp
   FdoRfpStreamReaderByTile.cpp
   FdoRfpStreamReaderByTileResample.cpp
   FdoRfpRect.cpp
   FdoRfpSchemaCapabilities.cpp
   FdoRfpSchemaData.cpp
   FdoRfpSelectAggregate.cpp
   FdoRfpSelectCommand.cpp
   FdoRfpSpatialContext.cpp
   FdoRfpSpatialContextReader.cpp
   FdoRfpStreamReaderRGB24.cpp
   FdoRfpVariant.cpp
   FdoRfpLinux.cpp
   FdoRfpBandRaster.cpp
   FdoRfpGeoBandRaster.cpp
   FdoRfpGeoBandRasterImp.cpp
   FdoRfpGeoRaster.cpp
   FdoRfpRaster.cpp
   FdoRfpGeoreference.cpp
   FdoRfpKeyColor.cpp
   stdafx.cpp
)

add_library( GRFPProvider-${FDO_VERSION} SHARED ${GRFPProvider_SRCS} )

target_link_libraries( GRFPProvider-${FDO_VERSION}
	GRFPOverrides-${FDO_VERSION}
	ProvidersCommon
	FDO-${FDO_VERSION}
	${GDAL_LIBRARY}
 )

install( TARGETS GRFPProvider-${FDO_VERSION} DESTINATION ${LIB_INSTALL_DIR} )
