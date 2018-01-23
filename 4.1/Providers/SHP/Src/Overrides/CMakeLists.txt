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

add_library( SHPOverrides-${FDO_VERSION} SHARED ${SHPOverrides_SRCS} )
target_link_libraries( SHPOverrides-${FDO_VERSION}
	ExpressionEngine-${FDO_VERSION}
	ProvidersCommon
	FDO-${FDO_VERSION}
 )
add_dependencies( SHPOverrides-${FDO_VERSION} ShpMessage )

install( TARGETS SHPOverrides-${FDO_VERSION} DESTINATION ${LIB_INSTALL_DIR} )



