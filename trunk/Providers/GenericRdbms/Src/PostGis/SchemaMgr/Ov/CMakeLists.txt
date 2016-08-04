include_directories( BEFORE	${CMAKE_CURRENT_SOURCE_DIR} )

set( FdoRdbmsPostGis_SchemaMgr_Ov_STAT_SRCS
   PostGisOvAssociationPropertyDefinition.cpp
   PostGisOvClassCollection.cpp
   PostGisOvClassDefinition.cpp
   PostGisOvColumn.cpp
   PostGisOvDataPropertyDefinitionCollection.cpp
   PostGisOvDataPropertyDefinition.cpp
   PostGisOvGeometricColumn.cpp
   PostGisOvGeometricPropertyDefinition.cpp
   PostGisOvObjectPropertyDefinition.cpp
   PostGisOvPhysicalSchemaMapping.cpp
   PostGisOvPropertyDefinitionCollection.cpp
   PostGisOvPropertyDefinition.cpp
   PostGisOvPropertyMappingClass.cpp
   PostGisOvPropertyMappingConcrete.cpp
   PostGisOvPropertyMappingDefinition.cpp
   PostGisOvPropertyMappingRelation.cpp
   PostGisOvPropertyMappingSingle.cpp
   PostGisOvTable.cpp
   stdafx.cpp
)

add_library( FdoRdbmsPostGis_SchemaMgr_Ov STATIC ${FdoRdbmsPostGis_SchemaMgr_Ov_STAT_SRCS} )
