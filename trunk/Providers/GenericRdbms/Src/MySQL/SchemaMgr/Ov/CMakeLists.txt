include_directories( BEFORE	${CMAKE_CURRENT_SOURCE_DIR} )

set( FdoRdbmsMySQL_SchemaMgr_Ov_STAT_SRCS
   MySqlOvAssociationPropertyDefinition.cpp
   MySqlOvClassCollection.cpp
   MySqlOvClassDefinition.cpp
   MySqlOvColumn.cpp
   MySqlOvDataPropertyDefinitionCollection.cpp
   MySqlOvDataPropertyDefinition.cpp
   MySqlOvGeometricColumn.cpp
   MySqlOvGeometricPropertyDefinition.cpp
   MySqlOvObjectPropertyDefinition.cpp
   MySqlOvPhysicalSchemaMapping.cpp
   MySqlOvPropertyDefinitionCollection.cpp
   MySqlOvPropertyDefinition.cpp
   MySqlOvPropertyMappingClass.cpp
   MySqlOvPropertyMappingConcrete.cpp
   MySqlOvPropertyMappingDefinition.cpp
   MySqlOvPropertyMappingRelation.cpp
   MySqlOvPropertyMappingSingle.cpp
   MySqlOvTable.cpp
   stdafx.cpp
)

add_library( FdoRdbmsMySQL_SchemaMgr_Ov STATIC ${FdoRdbmsMySQL_SchemaMgr_Ov_STAT_SRCS} )
