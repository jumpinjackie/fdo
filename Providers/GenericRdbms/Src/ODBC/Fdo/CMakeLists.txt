include_directories( BEFORE	${CMAKE_CURRENT_SOURCE_DIR} )

set( FdoRdbmsODBC_Fdo_STAT_SRCS
   FdoRdbmsOdbcCommandCapabilities.cpp
   FdoRdbmsOdbcConnectionCapabilities.cpp
   FdoRdbmsOdbcConnection.cpp
   FdoRdbmsOdbcConnectionInfo.cpp
   FdoRdbmsOdbcExpressionCapabilities.cpp
   FdoRdbmsOdbcFilterCapabilities.cpp
   FdoRdbmsOdbcFilterProcessor.cpp
   FdoRdbmsOdbcSchemaCapabilities.cpp
   FdoRdbmsOdbcSpatialManager.cpp
   FdoRdbmsOdbcSpatialSecondaryFilterLocal.cpp
   FdoRdbmsOdbcSpatialSqlFilter.cpp
)

add_library( FdoRdbmsODBC_Fdo STATIC ${FdoRdbmsODBC_Fdo_STAT_SRCS} )
add_dependencies( FdoRdbmsODBC_Fdo fdordbmsmsg )
