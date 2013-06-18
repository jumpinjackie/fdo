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

add_library( WMSOverrides-${FDO_VERSION} SHARED ${WMSOverrides_SRCS} )

target_link_libraries( WMSOverrides-${FDO_VERSION}
	ProvidersCommon
	FDO-${FDO_VERSION}
 )

add_dependencies( WMSOverrides-${FDO_VERSION} FdoWmsMessage )

install( TARGETS WMSOverrides-${FDO_VERSION} DESTINATION ${LIB_INSTALL_DIR} )

