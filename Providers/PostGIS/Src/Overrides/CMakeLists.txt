include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${UNMANAGED_INCLUDE_DIR}
	${UTILITIES_COMMON_INCLUDE_DIR}
	${POSTGRESQL_INCLUDE_DIR}
	${Boost_INCLUDE_DIRS}
	)

set( PostGISOverrides_LIB_SRCS
	ClassCollection.cpp
	ClassDefinition.cpp
	ColumnDefinition.cpp
	PhysicalSchemaMapping.cpp
	PostGisOverrides.cpp
	PropertyDefinitionCollection.cpp
	PropertyDefinition.cpp
	XmlGlobals.cpp
)

add_library( PostGISOverrides SHARED ${PostGISOverrides_LIB_SRCS} )
add_dependencies( PostGISOverrides PostGisMessage )

set_target_properties( PostGISOverrides PROPERTIES VERSION ${FDO_VERSION} SOVERSION ${FDO_VERSION_MAJOR} )
install( TARGETS PostGISOverrides DESTINATION ${LIB_INSTALL_DIR} )

target_link_libraries( PostGISOverrides
	ProvidersCommon
	ExpressionEngine
	FDO
 )


