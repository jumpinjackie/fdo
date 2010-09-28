include_directories( BEFORE ${CMAKE_CURRENT_SOURCE_DIR} )

set( FdoRdbms_Custom_STAT_SRCS
   FdoRdbmsCreateSpatialIndex.cpp
   FdoRdbmsGetSpatialIndexes.cpp
   FdoRdbmsDestroySpatialIndex.cpp
   FdoRdbmsSpatialIndexReader.cpp
)

add_library( FdoRdbms_Custom STATIC ${FdoRdbms_Custom_STAT_SRCS} )
