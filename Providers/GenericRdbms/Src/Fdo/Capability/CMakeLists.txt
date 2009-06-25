include_directories( BEFORE ${CMAKE_CURRENT_SOURCE_DIR} )

set( FdoRdbms_Capability_STAT_SRCS
   FdoRdbmsCommandCapabilities.cpp
   FdoRdbmsFilterCapabilities.cpp
   FdoRdbmsConnectionCapabilities.cpp
   FdoRdbmsSchemaCapabilities.cpp
   FdoRdbmsExpressionCapabilities.cpp
   FdoRdbmsTopologyCapabilities.cpp
)

add_library( FdoRdbms_Capability STATIC ${FdoRdbms_Capability_STAT_SRCS} )
