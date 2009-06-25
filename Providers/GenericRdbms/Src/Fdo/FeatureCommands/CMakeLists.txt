include_directories( BEFORE ${CMAKE_CURRENT_SOURCE_DIR} )

set( FdoRdbms_FeatureCommands_STAT_SRCS
   FdoRdbmsDeleteCommand.cpp
   FdoRdbmsInsertCommand.cpp
   FdoRdbmsFeatureInfoReader.cpp
   FdoRdbmsSelectAggregates.cpp
   FdoRdbmsSelectCommand.cpp
   FdoRdbmsFeatureReader.cpp
   FdoRdbmsUpdateCommand.cpp
   FdoRdbmsFeatureSubsetReader.cpp
)

add_library( FdoRdbms_FeatureCommands STATIC ${FdoRdbms_FeatureCommands_STAT_SRCS} )
