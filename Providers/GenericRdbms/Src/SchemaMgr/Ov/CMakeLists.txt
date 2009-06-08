include_directories(
	BEFORE
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${UTILITIES_SCHEMAMGR_INCLUDE_DIR}
	)

set( SchemaMgr_OV_STAT_SRCS
   RdbmsOvAssociationPropertyDefinition.cpp
   RdbmsOvClassCollection.cpp
   RdbmsOvClassDefinition.cpp
   RdbmsOvColumn.cpp
   RdbmsOvDataPropertyDefinition.cpp
   RdbmsOvGeometricColumn.cpp
   RdbmsOvGeometricPropertyDefinition.cpp
   RdbmsOvObjectPropertyDefinition.cpp
   RdbmsOvPhysicalSchemaMapping.cpp
   RdbmsOvPropertyDefinition.cpp
   RdbmsOvPropertyDefinitionCollection.cpp
   RdbmsOvPropertyMappingClass.cpp
   RdbmsOvPropertyMappingConcrete.cpp
   RdbmsOvPropertyMappingDefinition.cpp
   RdbmsOvPropertyMappingRelation.cpp
   RdbmsOvPropertyMappingSingle.cpp
   RdbmsOvSchemaAutoGeneration.cpp
   RdbmsOvTable.cpp
   stdafx.cpp
)

add_library( RdbmsSchemaMgr_OV STATIC ${SchemaMgr_OV_STAT_SRCS} )

target_link_libraries( RdbmsSchemaMgr_OV FDO )
