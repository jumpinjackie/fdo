include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )


set( FdoRdbmsMySQL_Fdo_STAT_SRCS
   FdoRdbmsMySqlConnectionCapabilities.cpp
   FdoRdbmsMySqlFilterCapabilities.cpp
   FdoRdbmsMySqlFilterProcessor.cpp
   FdoRdbmsMySqlGeometryCapabilities.cpp
   FdoRdbmsMySqlConnection.cpp
   FdoRdbmsMySqlCommandCapabilities.cpp
   FdoRdbmsMySqlConnectionInfo.cpp
)

add_library( FdoRdbmsMySQL_Fdo STATIC ${FdoRdbmsMySQL_Fdo_STAT_SRCS} )
