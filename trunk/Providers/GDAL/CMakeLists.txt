project(fdogdal)

# Make CPack available to easy generate binary packages
include(CPack)

if( NOT INTERNAL_GDAL )
	find_package( GDAL REQUIRED )
endif( NOT INTERNAL_GDAL )

if( NOT FDO_CORE_BUILD )
	find_package( FDO REQUIRED )
endif( NOT FDO_CORE_BUILD )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/Inc )

add_subdirectory( Src )
add_subdirectory( Inc )
add_subdirectory( Docs )
