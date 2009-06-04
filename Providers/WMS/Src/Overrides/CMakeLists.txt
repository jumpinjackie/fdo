include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${UNMANAGED_INCLUDE_DIR}
	${UTILITIES_COMMON_INCLUDE_DIR}
	)

set( WMSOverrides_SRCS
   FdoWmsOvPhysicalSchemaMapping.cpp
   FdoWmsOvClassCollection.cpp
   FdoWmsOvClassDefinition.cpp
   FdoWmsOvLayerCollection.cpp
   FdoWmsOvLayerDefinition.cpp
   FdoWmsOvStyleDefinition.cpp
   FdoWmsOvRasterDefinition.cpp
   FdoWmsOverrides.cpp
)

add_library( WMSOverrides SHARED ${WMSOverrides_SRCS} )

target_link_libraries( WMSOverrides
	ProvidersCommon
	FDO
 )

add_dependencies( WMSOverrides FdoWmsMessage )

set_target_properties( WMSOverrides PROPERTIES VERSION ${FDO_VERSION} SOVERSION ${FDO_VERSION_MAJOR} )
install( TARGETS WMSOverrides DESTINATION ${LIB_INSTALL_DIR} )

