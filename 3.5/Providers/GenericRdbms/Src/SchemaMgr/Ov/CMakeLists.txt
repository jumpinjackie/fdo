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

add_library( SchemaMgr_OV SHARED ${SchemaMgr_OV_LIB_SRCS} )
add_dependencies( SchemaMgr_OV fdordbmsmsg )

target_link_libraries( SchemaMgr_OV 
	FDO 
	)

if( CMAKE_COMPILER_IS_GNUCXX )
	target_link_libraries( SchemaMgr_OV
		-Wl,-whole-archive
		SmOverrides
		-Wl,-no-whole-archive
		)
else( CMAKE_COMPILER_IS_GNUCXX )
	target_link_libraries( SchemaMgr_OV
		-Wl,-whole-archive
		SmOverrides
		)
endif( CMAKE_COMPILER_IS_GNUCXX )

set_target_properties( SchemaMgr_OV PROPERTIES VERSION ${FDO_VERSION} SOVERSION ${FDO_VERSION_MAJOR}  )
install( TARGETS SchemaMgr_OV DESTINATION ${LIB_INSTALL_DIR} )

