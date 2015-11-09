include_directories( BEFORE	${CMAKE_CURRENT_SOURCE_DIR} )

set( SchemaMgr_OV_LIB_SRCS
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

add_library( SchemaMgr_OV-${FDO_VERSION} SHARED ${SchemaMgr_OV_LIB_SRCS} )
add_dependencies( SchemaMgr_OV-${FDO_VERSION} fdordbmsmsg )

target_link_libraries( SchemaMgr_OV-${FDO_VERSION} 
	FDO-${FDO_VERSION} 
	)

if( CMAKE_COMPILER_IS_GNUCXX )
	target_link_libraries( SchemaMgr_OV-${FDO_VERSION}
		-Wl,-whole-archive
		SmOverrides
		-Wl,-no-whole-archive
		)
else( CMAKE_COMPILER_IS_GNUCXX )
	target_link_libraries( SchemaMgr_OV-${FDO_VERSION}
		-Wl,-whole-archive
		SmOverrides
		)
endif( CMAKE_COMPILER_IS_GNUCXX )

install( TARGETS SchemaMgr_OV-${FDO_VERSION} DESTINATION ${LIB_INSTALL_DIR} )

