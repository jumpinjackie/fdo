include_directories( BEFORE	${CMAKE_CURRENT_SOURCE_DIR} )

set( ODBCOverrides_SRCS
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

add_library( ODBCOverrides${FDO_VERSION_SUFFIX} SHARED ${ODBCOverrides_SRCS} )
target_link_libraries(ODBCOverrides${FDO_VERSION_SUFFIX}
    FDO${FDO_VERSION_SUFFIX}
    RdbmsOverrides${FDO_VERSION_SUFFIX}
)
install( TARGETS ODBCOverrides${FDO_VERSION_SUFFIX} DESTINATION ${LIB_INSTALL_DIR} COMPONENT ${FDO_COMPONENT} )
install_symlink( ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/libODBCOverrides${FDO_VERSION_SUFFIX}.so libODBCOverrides.so ${FDO_COMPONENT} )