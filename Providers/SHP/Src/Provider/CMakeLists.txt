include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../ShpRead
	${CMAKE_CURRENT_SOURCE_DIR}/../ShpSpatialIndex
	${UNMANAGED_INCLUDE_DIR}
	${UNMANAGED_INCLUDE_DIR}/Fdo/Expression
	${UTILITIES_COMMON_INCLUDE_DIR}
	${UTILITIES_EXPRESSION_INCLUDE_DIR}
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

add_library( SHPProvider-${FDO_VERSION} SHARED ${SHPProvider_SRCS} )

target_link_libraries( SHPProvider-${FDO_VERSION}
	SHPOverrides-${FDO_VERSION}
	FDO-${FDO_VERSION}
	ProvidersCommon
	ExpressionEngine-${FDO_VERSION}
 )

if( CMAKE_COMPILER_IS_GNUCXX )
	target_link_libraries( SHPProvider-${FDO_VERSION}
		-Wl,-whole-archive
		SHPRead
		SHPSpatialIndex
		-Wl,-no-whole-archive
		)
else( CMAKE_COMPILER_IS_GNUCXX )
	target_link_libraries( SHPProvider-${FDO_VERSION}
		SHPRead
		SHPSpatialIndex
		)
endif( CMAKE_COMPILER_IS_GNUCXX )

	
add_dependencies( SHPProvider-${FDO_VERSION} SHPMessage )

install( TARGETS SHPProvider-${FDO_VERSION} DESTINATION ${LIB_INSTALL_DIR} )

