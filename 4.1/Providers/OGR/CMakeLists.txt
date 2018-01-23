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

add_library( OGRProvider-${FDO_VERSION} SHARED ${OGRProvider_SRCS} )

target_link_libraries( OGRProvider-${FDO_VERSION}
	FDO-${FDO_VERSION}
	ProvidersCommon
	${GDAL_LIBRARY}
 )

install( TARGETS OGRProvider-${FDO_VERSION} DESTINATION ${LIB_INSTALL_DIR} )


