include_directories( BEFORE	${CMAKE_CURRENT_SOURCE_DIR} )

set( FdoRdbmsPostGis_Fdo_STAT_SRCS
   FdoRdbmsPostGisCommandCapabilities.cpp
   FdoRdbmsPostGisConnectionCapabilities.cpp
   FdoRdbmsPostGisConnection.cpp
   FdoRdbmsPostGisConnectionInfo.cpp
   FdoRdbmsPostGisCreateDataStore.cpp
   FdoRdbmsPostGisDeleteDataStore.cpp
   FdoRdbmsPostGisExpressionCapabilities.cpp
   FdoRdbmsPostGisFilterCapabilities.cpp
   FdoRdbmsPostGisFilterProcessor.cpp
   FdoRdbmsPostGisGeometryCapabilities.cpp
   FdoRdbmsPostGisGetDataStores.cpp
   FdoRdbmsPostGisSchemaCapabilities.cpp
)

add_library( FdoRdbmsPostGis_Fdo STATIC ${FdoRdbmsPostGis_Fdo_STAT_SRCS} )
add_dependencies( FdoRdbmsPostGis_Fdo fdordbmsmsg )
