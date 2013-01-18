project(fdoogr)

# Make CPack available to easy generate binary packages
include(CPack)

if( NOT INTERNAL_GDAL )
	find_package( GDAL REQUIRED )
endif( NOT INTERNAL_GDAL )

if( NOT FDO_CORE_BUILD )
	find_package( FDO REQUIRED )
endif( NOT FDO_CORE_BUILD )

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${UNMANAGED_INCLUDE_DIR}
	${GDAL_INCLUDE_DIR}
	${UTILITIES_COMMON_INCLUDE_DIR}
	)

set( OGRProvider_SRCS
	OgrFdoUtil.cpp
	OgrProvider.cpp
	ProjConverter.cpp
)

add_library( OGRProvider SHARED ${OGRProvider_SRCS} )

target_link_libraries( OGRProvider
	FDO
	ProvidersCommon
	${GDAL_LIBRARY}
 )

set_target_properties( OGRProvider PROPERTIES VERSION ${FDO_VERSION} SOVERSION ${FDO_VERSION_MAJOR} )

install( TARGETS OGRProvider DESTINATION ${LIB_INSTALL_DIR} )


