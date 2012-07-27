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

add_library( SHPOverrides SHARED ${SHPOverrides_SRCS} )
target_link_libraries( SHPOverrides
	ExpressionEngine
	ProvidersCommon
	FDO
 )
add_dependencies( SHPOverrides ShpMessage )

set_target_properties( SHPOverrides PROPERTIES VERSION ${FDO_VERSION} SOVERSION ${FDO_VERSION_MAJOR} )
install( TARGETS SHPOverrides DESTINATION ${LIB_INSTALL_DIR} )



