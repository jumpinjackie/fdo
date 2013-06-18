include_directories(
	BEFORE
	${UTILITIES_COMMON_INCLUDE_DIR}
	${UNMANAGED_INCLUDE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	)

set( GRFPOverrides_SRCS
   RFPOverrides.cpp
   FdoGrfpClassDefinition.cpp
   FdoGrfpPhysicalSchemaMapping.cpp
   FdoGrfpRasterDefinition.cpp
   FdoGrfpRasterImageDefinition.cpp
   FdoGrfpRasterLocation.cpp
   FdoGrfpRasterFeatureDefinition.cpp
   FdoGrfpRasterBandDefinition.cpp
   FdoGrfpRasterGeoreferenceLocation.cpp
)

add_library( GRFPOverrides-${FDO_VERSION} SHARED ${GRFPOverrides_SRCS} )

target_link_libraries( GRFPOverrides-${FDO_VERSION}
	ProvidersCommon
	FDO-${FDO_VERSION}
 )

install( TARGETS GRFPOverrides-${FDO_VERSION} DESTINATION ${LIB_INSTALL_DIR} )
