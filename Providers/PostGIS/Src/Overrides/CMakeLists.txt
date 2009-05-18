include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${FDO}/Unmanaged/Inc
	${FDOUTILITIES}/Common/Inc
	${POSTGRESQL_INCLUDE_DIR}
	${Boost_INCLUDE_DIRS}
	)

set( PostGISOverridesSrc_STAT_SRCS
   ClassCollection.cpp
   ClassDefinition.cpp
   ColumnDefinition.cpp
   PhysicalSchemaMapping.cpp
   PostGisOverrides.cpp
   PropertyDefinitionCollection.cpp
   PropertyDefinition.cpp
   XmlGlobals.cpp
)

add_library( PostGISOverridesSrc STATIC ${PostGISOverridesSrc_STAT_SRCS} )
add_dependencies( PostGISOverridesSrc PostGisMessage )

