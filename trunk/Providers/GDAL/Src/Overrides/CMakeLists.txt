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

add_library( GRFPOverrides SHARED ${GRFPOverrides_SRCS} )

target_link_libraries( GRFPOverrides
	ProvidersCommon
	FDO
 )

set_target_properties( GRFPOverrides PROPERTIES VERSION ${FDO_VERSION} SOVERSION ${FDO_VERSION_MAJOR} )
install( TARGETS GRFPOverrides DESTINATION ${LIB_INSTALL_DIR} )
