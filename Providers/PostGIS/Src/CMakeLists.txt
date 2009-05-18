include_directories( 
	${CMAKE_CURRENT_SOURCE_DIR} 
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/Overrides 
	${CMAKE_CURRENT_BINARY_DIR}/Overrides
	${CMAKE_CURRENT_SOURCE_DIR}/Provider 
	${CMAKE_CURRENT_BINARY_DIR}/Provider
	${FDO}/Unmanaged/Inc
	${FDOUTILITIES}/Common/Inc
	${FDOUTILITIES}/ExpressionEngine/Inc
	${POSTGRESQL_INCLUDE_DIR}
	${Boost_INCLUDE_DIRS}
	)

add_subdirectory( Message )
add_subdirectory( Overrides )

#----------- Overrides

set( PostGISOverrides_LIB_SRCS
	Overrides/ClassCollection.cpp
	Overrides/ClassDefinition.cpp
	Overrides/ColumnDefinition.cpp
	Overrides/PhysicalSchemaMapping.cpp
	Overrides/PostGisOverrides.cpp
	Overrides/PropertyDefinitionCollection.cpp
	Overrides/PropertyDefinition.cpp
	Overrides/XmlGlobals.cpp
)

add_library( PostGISOverrides SHARED ${PostGISOverrides_LIB_SRCS} )
add_dependencies( PostGISOverrides PostGisMessage )

target_link_libraries( PostGISOverrides
	ProvidersCommon
	ExpressionEngine
	FDO
 )

set_target_properties( PostGISOverrides PROPERTIES VERSION ${FDO_VERSION} SOVERSION ${FDO_VERSION_MAJOR} )
install( TARGETS PostGISOverrides DESTINATION ${LIB_INSTALL_DIR} )

#----------- Provider

set( PostGISProvider_LIB_SRCS
	Provider/ApplySchemaCommand.cpp
	Provider/FilterProcessor.cpp
	Provider/SchemaCapabilities.cpp
	Provider/CommandCapabilities.cpp
	Provider/GeometryCapabilities.cpp
	Provider/SchemaDescription.cpp
	Provider/ConnectionCapabilities.cpp
	Provider/GetSpatialContextsCommand.cpp
	Provider/SelectAggregatesCommand.cpp
	Provider/Connection.cpp
	Provider/InsertCommand.cpp
	Provider/InsertFeatureReader.cpp
	Provider/SelectCommand.cpp
	Provider/ConnectionInfo.cpp
	Provider/ListDataStores.cpp
	Provider/SpatialContextCollection.cpp
	Provider/CreateDataStore.cpp
	Provider/md5.cpp
	Provider/SpatialContext.cpp
	Provider/DataReader.cpp
	Provider/PgCursor.cpp
	Provider/SpatialContextReader.cpp
	Provider/DataStoreReader.cpp
	Provider/PgGeometryColumn.cpp
	Provider/SQLCommand.cpp
	Provider/DeleteCommand.cpp
	Provider/PgGeometry.cpp
	Provider/SQLDataReader.cpp
	Provider/DescribeSchemaCommand.cpp
	Provider/PgSpatialRefSysReader.cpp
	Provider/DestroyDataStore.cpp
	Provider/PgTablesReader.cpp
	Provider/TopologyCapabilities.cpp
	Provider/ExpressionCapabilities.cpp
	Provider/PgTableColumnsReader.cpp
	Provider/Transaction.cpp
	Provider/ExpressionProcessor.cpp
	Provider/PgUtility.cpp
	Provider/UpdateCommand.cpp
	Provider/FeatureReader.cpp
	Provider/PostGisProvider.cpp
	Provider/FilterCapabilities.cpp
	Provider/RasterCapabilities.cpp
)

add_library( PostGISProvider SHARED ${PostGISProvider_LIB_SRCS} )
add_dependencies( PostGISProvider PostGisMessage )

target_link_libraries( PostGISProvider
	PostGISOverrides
	ProvidersCommon
	FDO
	pq
 )


set_target_properties( PostGISProvider PROPERTIES VERSION ${FDO_VERSION} SOVERSION ${FDO_VERSION_MAJOR} )
install( TARGETS PostGISProvider DESTINATION ${LIB_INSTALL_DIR} )
