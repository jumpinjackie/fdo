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

add_library( KingOracleOverridesSrc-${FDO_VERSION} SHARED ${KingOracleOverridesSrc_SRCS} )
add_dependencies( KingOracleOverridesSrc-${FDO_VERSION} KgOraMessage )

target_link_libraries( KingOracleOverridesSrc-${FDO_VERSION} 
    FDO-${FDO_VERSION}
    ProvidersCommon
)