include_directories( BEFORE ${CMAKE_CURRENT_SOURCE_DIR} )

set( FdoRdbms_DataStore_STAT_SRCS
   FdoRdbmsGetDataStores.cpp
   FdoRdbmsDataStoreReader.cpp
)

add_library( FdoRdbms_DataStore STATIC ${FdoRdbms_DataStore_STAT_SRCS} )
