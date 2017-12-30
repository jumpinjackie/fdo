include_directories( BEFORE	${CMAKE_CURRENT_SOURCE_DIR} )

set( RdbmsOverrides_SRCS
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

add_library( RdbmsOverrides${FDO_VERSION_SUFFIX} SHARED ${RdbmsOverrides_SRCS} )
add_dependencies( RdbmsOverrides${FDO_VERSION_SUFFIX} fdordbmsmsg )

target_link_libraries( RdbmsOverrides${FDO_VERSION_SUFFIX} 
    FDO${FDO_VERSION_SUFFIX} 
)

if( CMAKE_COMPILER_IS_GNUCXX )
    target_link_libraries( RdbmsOverrides${FDO_VERSION_SUFFIX}
        -Wl,-whole-archive
        SmOverrides
        -Wl,-no-whole-archive
    )
else( CMAKE_COMPILER_IS_GNUCXX )
    target_link_libraries( RdbmsOverrides${FDO_VERSION_SUFFIX}
        -Wl,-whole-archive
        SmOverrides
    )
endif( CMAKE_COMPILER_IS_GNUCXX )

install( TARGETS RdbmsOverrides${FDO_VERSION_SUFFIX} DESTINATION ${LIB_INSTALL_DIR} )
install_symlink( ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/libRdbmsOverrides${FDO_VERSION_SUFFIX}.so libRdbmsOverrides.so )