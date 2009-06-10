include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${UNMANAGED_INCLUDE_DIR}
	${UNMANAGED_NLS_INCLUDE_DIR}
	${UTILITIES_COMMON_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../Utils
	${UTILITIES_SQLITEINTERFACE_INCLUDE_DIR}
	${UTILITIES_EXPRESSION_INCLUDE_DIR}
	)

set( SDFProvider_SRCS
   RTree.cpp
   BinaryReader.cpp
   BinaryWriter.cpp
   DataDb.cpp
   DataIO.cpp
   ExInfoDb.cpp
   KeyDb.cpp
   PhysName.cpp
   SchemaDb.cpp
   TableReformatter.cpp
   utf8_.cpp
   SdfApplySchema.cpp
   SdfCommandCapabilities.cpp
   SdfCompareHandler.cpp
   SdfConnection.cpp
   SdfConnectionCapabilities.cpp
   SdfConnectionInfo.cpp
   SdfCreateSDFFile.cpp
   SdfCreateSpatialContext.cpp
   SdfDataValidator.cpp
   SdfDelete.cpp
   SdfDeletingFeatureReader.cpp
   SdfDescribeSchema.cpp
   SdfExpressionCapabilities.cpp
   SdfImpExtendedSelect.cpp
   SdfFilterCapabilities.cpp
   SdfGeometryCapabilities.cpp
   SdfGetSpatialContexts.cpp
   SdfInsert.cpp
   SdfRasterCapabilities.cpp
   SdfSchemaCapabilities.cpp
   SdfSchemaMergeContext.cpp
   SdfScrollableFeatureReader.cpp
   SdfSelect.cpp
   SdfSelectAggregates.cpp
   SdfSelectAggregatesCommand.cpp
   SdfSimpleFeatureReader.cpp
   SdfIndexedScrollableFeatureReader.cpp
   SdfSpatialContextReader.cpp
   SdfTopologyCapabilities.cpp
   SdfUpdate.cpp
   SdfUpdatingFeatureReader.cpp
   SdfDataReader.cpp
   SdfDistinctDataReader.cpp
   SdfQueryOptimizer.cpp
   SdfCreateDataStore.cpp
   SdfDeleteDataStore.cpp
   SdfSpatialExtentsAggregateReader.cpp
   SDF.cpp
)

add_library( SDFProvider SHARED ${SDFProvider_SRCS} )

target_link_libraries( SDFProvider
	FDO
	ExpressionEngine
	ProvidersCommon
	xalan-c
	xalanMsg
	xerces-c
 )

if( CMAKE_COMPILER_IS_GNUCXX )
	target_link_libraries( SDFProvider
		-Wl,-whole-archive
		SDFUtils	
		SQLiteInterface
		sqlite3fdo
		-Wl,-no-whole-archive
		)
else( CMAKE_COMPILER_IS_GNUCXX )
	target_link_libraries( SDFProvider
		SDFUtils	
		SQLiteInterface
		sqlite3fdo
		)
endif( CMAKE_COMPILER_IS_GNUCXX )

set_target_properties( SDFProvider PROPERTIES VERSION ${FDO_VERSION} SOVERSION ${FDO_VERSION_MAJOR} )
install( TARGETS SDFProvider DESTINATION ${LIB_INSTALL_DIR} )

