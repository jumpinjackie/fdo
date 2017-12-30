include_directories( BEFORE	${CMAKE_CURRENT_SOURCE_DIR} )

set( PostgreSQLOverrides_SRCS
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

add_library( PostgreSQLOverrides${FDO_VERSION_SUFFIX} SHARED ${PostgreSQLOverrides_SRCS} )
target_link_libraries(PostgreSQLOverrides${FDO_VERSION_SUFFIX}
    FDO${FDO_VERSION_SUFFIX}
    RdbmsOverrides${FDO_VERSION_SUFFIX}
)
install( TARGETS PostgreSQLOverrides${FDO_VERSION_SUFFIX} DESTINATION ${LIB_INSTALL_DIR} )
install_symlink( ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/libPostgreSQLOverrides${FDO_VERSION_SUFFIX}.so libPostgreSQLOverrides.so )