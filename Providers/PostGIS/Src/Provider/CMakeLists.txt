include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${FDO}/Unmanaged/Inc
	${FDOUTILITIES}/Common/Inc
	${FDOUTILITIES}/ExpressionEngine/Inc
	${POSTGRESQL_INCLUDE_DIR}
	${Boost_INCLUDE_DIRS}
	)

set( PostGISProviderSrc_STAT_SRCS
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

add_library( PostGISProviderSrc STATIC ${PostGISProviderSrc_STAT_SRCS} )
add_dependencies( PostGISProviderSrc PostGisMessage )

