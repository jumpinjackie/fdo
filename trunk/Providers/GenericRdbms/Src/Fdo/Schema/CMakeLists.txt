include_directories( BEFORE ${CMAKE_CURRENT_SOURCE_DIR} )

set( FdoRdbms_Schema_STAT_SRCS
   FdoRdbmsApplySchemaCommand.cpp
   FdoRdbmsDestroySchemaCommand.cpp
   FdoRdbmsDescribeSchemaCommand.cpp
   FdoRdbmsGetClassNamesCommand.cpp
   FdoRdbmsGetSchemaNamesCommand.cpp
   FdoRdbmsSchemaUtil.cpp
   FdoRdbmsDescribeSchemaMappingCommand.cpp
)

add_library( FdoRdbms_Schema STATIC ${FdoRdbms_Schema_STAT_SRCS} )
add_dependencies( FdoRdbms_Schema fdordbmsmsg )
