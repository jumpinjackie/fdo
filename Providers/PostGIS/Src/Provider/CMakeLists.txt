include_directories(
	BEFORE
	${POSTGRESQL_INCLUDE_DIR}
	${Boost_INCLUDE_DIRS}
	${UTILITIES_EXPRESSION_INCLUDE_DIR}
	${UTILITIES_COMMON_INCLUDE_DIR}
	${UNMANAGED_INCLUDE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	)

set( PostGISProvider_LIB_SRCS
	ApplySchemaCommand.cpp
	FilterProcessor.cpp
	SchemaCapabilities.cpp
	CommandCapabilities.cpp
	GeometryCapabilities.cpp
	SchemaDescription.cpp
	ConnectionCapabilities.cpp
	GetSpatialContextsCommand.cpp
	SelectAggregatesCommand.cpp
	Connection.cpp
	InsertCommand.cpp
	InsertFeatureReader.cpp
	SelectCommand.cpp
	ConnectionInfo.cpp
	ListDataStores.cpp
	SpatialContextCollection.cpp
	CreateDataStore.cpp
	md5.cpp
	SpatialContext.cpp
	CreateSpatialContextCommand.cpp
	DataReader.cpp
	PgCursor.cpp
	SpatialContextReader.cpp
	DataStoreReader.cpp
	PgGeometryColumn.cpp
	SQLCommand.cpp
	DeleteCommand.cpp
	PgGeometry.cpp
	SQLDataReader.cpp
	DescribeSchemaCommand.cpp
	PgSpatialRefSysReader.cpp
	DestroyDataStore.cpp
	PgTablesReader.cpp
	TopologyCapabilities.cpp
	ExpressionCapabilities.cpp
	PgTableColumnsReader.cpp
	Transaction.cpp
	ExpressionProcessor.cpp
	PgUtility.cpp
	UpdateCommand.cpp
	FeatureReader.cpp
	PostGisProvider.cpp
	FilterCapabilities.cpp
	RasterCapabilities.cpp
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
