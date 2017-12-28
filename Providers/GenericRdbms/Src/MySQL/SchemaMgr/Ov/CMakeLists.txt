include_directories( BEFORE	${CMAKE_CURRENT_SOURCE_DIR} )

set( MySQLOverrides_SRCS
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

add_library( MySQLOverrides${FDO_VERSION_SUFFIX} SHARED ${MySQLOverrides_SRCS} )
target_link_libraries(MySQLOverrides${FDO_VERSION_SUFFIX}
    FDO${FDO_VERSION_SUFFIX}
    RdbmsOverrides${FDO_VERSION_SUFFIX}
)
install( TARGETS MySQLOverrides${FDO_VERSION_SUFFIX} DESTINATION ${LIB_INSTALL_DIR} )
install_symlink( ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/libMySQLOverrides${FDO_VERSION_SUFFIX}.so ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/libMySQLOverrides.so )