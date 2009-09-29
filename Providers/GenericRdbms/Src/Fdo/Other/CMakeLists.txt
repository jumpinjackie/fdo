include_directories( BEFORE ${CMAKE_CURRENT_SOURCE_DIR} )

set( FdoRdbms_Other_STAT_SRCS
   FdoRdbmsBLOBStreamReader.cpp
   FdoRdbmsPrimaryKey.cpp
   FdoRdbmsException.cpp
   FdoRdbmsSQLCommand.cpp
   FdoRdbmsFeatureTransaction.cpp
   FdoRdbmsSQLDataReader.cpp
   FdoRdbmsLobUtility.cpp
   FdoRdbmsUtil.cpp
   FdoRdbmsPrimaryKeyColumn.cpp
)

add_library( FdoRdbms_Other STATIC ${FdoRdbms_Other_STAT_SRCS} )
add_dependencies( FdoRdbms_Other fdordbmsmsg )
