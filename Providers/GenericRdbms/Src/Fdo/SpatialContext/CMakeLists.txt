include_directories( BEFORE ${CMAKE_CURRENT_SOURCE_DIR} )

set( FdoRdbms_SpatialContext_STAT_SRCS
   FdoRdbmsActivateSpatialContext.cpp
   FdoRdbmsGetSpatialContexts.cpp
   FdoRdbmsCreateSpatialContext.cpp
   FdoRdbmsSpatialContextReader.cpp
   FdoRdbmsDestroySpatialContext.cpp
)

add_library( FdoRdbms_SpatialContext STATIC ${FdoRdbms_SpatialContext_STAT_SRCS} )
add_dependencies( FdoRdbms_SpatialContext fdordbmsmsg )
