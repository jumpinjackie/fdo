include_directories( BEFORE ${CMAKE_CURRENT_SOURCE_DIR} )

set( FdoRdbms_Connection_STAT_SRCS
   DbiConnection.cpp
   FdoRdbmsConnectionPropertyDictionary.cpp
   FdoRdbmsConnection.cpp
)

add_library( FdoRdbms_Connection STATIC ${FdoRdbms_Connection_STAT_SRCS} )
add_dependencies( FdoRdbms_Connection fdordbmsmsg )
