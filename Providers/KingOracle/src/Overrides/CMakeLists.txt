include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../inc
    ${CMAKE_CURRENT_BINARY_DIR}
    ${UNMANAGED_INCLUDE_DIR}
    ${UTILITIES_COMMON_INCLUDE_DIR}
)

set (KingOracleOverridesSrc_SRCS
    FdoKgOraClassCollection.cpp
    FdoKgOraPhysicalSchemaMapping.cpp
    FdoKgOraClassDefinition.cpp
    FdoKgOraPropertyDefinitionCollection.cpp
    FdoKgOraColumnDefinition.cpp
    FdoKgOraPropertyDefinition.cpp
)

add_library( KingOracleOverrides${FDO_VERSION_SUFFIX} SHARED ${KingOracleOverridesSrc_SRCS} )
add_dependencies( KingOracleOverrides${FDO_VERSION_SUFFIX} KgOraMessage )

target_link_libraries( KingOracleOverrides${FDO_VERSION_SUFFIX} 
    FDO${FDO_VERSION_SUFFIX}
    ProvidersCommon
)

install( TARGETS KingOracleOverrides${FDO_VERSION_SUFFIX} DESTINATION ${LIB_INSTALL_DIR} )
install_symlink( ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/libKingOracleOverrides${FDO_VERSION_SUFFIX}.so ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/libKingOracleOverrides.so )