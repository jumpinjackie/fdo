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

add_library( SchemaMgr_OV${FDO_VERSION_SUFFIX} STATIC ${SchemaMgr_OV_LIB_SRCS} )
add_dependencies( SchemaMgr_OV${FDO_VERSION_SUFFIX} fdordbmsmsg )

target_link_libraries( SchemaMgr_OV${FDO_VERSION_SUFFIX} 
    FDO${FDO_VERSION_SUFFIX} 
)

if( CMAKE_COMPILER_IS_GNUCXX )
    target_link_libraries( SchemaMgr_OV${FDO_VERSION_SUFFIX}
        -Wl,-whole-archive
        SmOverrides
        -Wl,-no-whole-archive
    )
else( CMAKE_COMPILER_IS_GNUCXX )
    target_link_libraries( SchemaMgr_OV${FDO_VERSION_SUFFIX}
        -Wl,-whole-archive
        SmOverrides
    )
endif( CMAKE_COMPILER_IS_GNUCXX )