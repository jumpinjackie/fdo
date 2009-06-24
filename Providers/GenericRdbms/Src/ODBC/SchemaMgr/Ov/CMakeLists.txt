include_directories( BEFORE	${CMAKE_CURRENT_SOURCE_DIR} )

set( FdoRdbmsODBC_SchemaMgr_Ov_STAT_SRCS
   OdbcOvClassCollection.cpp
   OdbcOvClassDefinition.cpp
   OdbcOvColumn.cpp
   OdbcOvDataPropertyDefinition.cpp
   OdbcOvGeometricPropertyDefinition.cpp
   OdbcOvPhysicalSchemaMapping.cpp
   OdbcOvPropertyDefinitionCollection.cpp
   OdbcOvPropertyDefinition.cpp
   OdbcOvTable.cpp
   stdafx.cpp
)

add_library( FdoRdbmsODBC_SchemaMgr_Ov STATIC ${FdoRdbmsODBC_SchemaMgr_Ov_STAT_SRCS} )
