include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${UNMANAGED_INCLUDE_DIR}
    ${UTILITIES_COMMON_INCLUDE_DIR}
)

set( SHPOverrides_SRCS
    FdoShpPropertyDefinition.cpp
    FdoShpColumnDefinition.cpp
    FdoShpPhysicalSchemaMapping.cpp
    FdoShpClassDefinition.cpp
    FdoShpClassCollection.cpp
    FdoShpPropertyDefinitionCollection.cpp
)

add_library( SHPOverrides${FDO_VERSION_SUFFIX} SHARED ${SHPOverrides_SRCS} )
target_link_libraries( SHPOverrides${FDO_VERSION_SUFFIX}
    ExpressionEngine${FDO_VERSION_SUFFIX}
    ProvidersCommon
    FDO${FDO_VERSION_SUFFIX}
)
add_dependencies( SHPOverrides${FDO_VERSION_SUFFIX} ShpMessage )

install( TARGETS SHPOverrides${FDO_VERSION_SUFFIX} DESTINATION ${LIB_INSTALL_DIR} )
install_symlink( ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/libSHPOverrides${FDO_VERSION_SUFFIX}.so ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/libSHPOverrides.so )