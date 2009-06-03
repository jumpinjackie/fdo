include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../ShpRead
	${CMAKE_CURRENT_SOURCE_DIR}/../ShpSpatialIndex
  	${FDO}/Unmanaged/Inc/ 
	${FDOUTILITIES}/Common/Inc
  	${FDO}/Unmanaged/Inc/Fdo/Expression 
	${FDOUTILITIES}/ExpressionEngine/Inc
	)

set( SHPProvider_SRCS
   stdafx.cpp
   ShpCommandCapabilities.cpp
   ShpConnectionCapabilities.cpp
   ShpConnection.cpp
   ShpConnectionInfo.cpp
   ShpSchemaUtilities.cpp
   ShpDescribeSchemaCommand.cpp
   ShpDescribeSchemaMappingCommand.cpp
   ShpApplySchemaCommand.cpp
   ShpDestroySchemaCommand.cpp
   ShpExpressionCapabilities.cpp
   ShpFeatureReader.cpp
   ShpFileSet.cpp
   ShpFilterCapabilities.cpp
   ShpGeometryCapabilities.cpp
   ShpQueryOptimizer.cpp
   ShpFeatIdQueryTester.cpp
   ShpFeatIdQueryEvaluator.cpp
   ShpRasterCapabilities.cpp
   ShpSchemaCapabilities.cpp
   ShpSelectCommand.cpp
   ShpSelectAggregates.cpp
   ShpSelectAggregatesCommand.cpp
   ShpOptimizedAggregateReader.cpp
   ShpInsertCommand.cpp
   ShpUpdateCommand.cpp
   ShpDeleteCommand.cpp
   ShpTopologyCapabilities.cpp
   ShpGetSpatialContextsCommand.cpp
   ShpCreateSpatialContextCommand.cpp
   ShpSpatialContext.cpp
   ShpSpatialContextReader.cpp
   ShpPhysicalSchema.cpp
   ShpLpClassDefinition.cpp
   ShpLpFeatureSchema.cpp
   ShpLpPropertyDefinition.cpp
   ShpScrollableFeatureReader.cpp
   ShpCompareHandler.cpp
   ShpImpExtendedSelect.cpp
)

add_library( SHPProvider SHARED ${SHPProvider_SRCS} )

target_link_libraries( SHPProvider
	SHPRead
	SHPSpatialIndex
	SHPOverrides
	FDO
	ProvidersCommon
	ExpressionEngine
 )

add_dependencies( SHPProvider SHPMessage )

set_target_properties( SHPProvider PROPERTIES VERSION ${FDO_VERSION} SOVERSION ${FDO_VERSION_MAJOR} )

install( TARGETS SHPProvider DESTINATION ${LIB_INSTALL_DIR} )

