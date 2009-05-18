project(fdopostgis)

# Look first in internal modules
set( CMAKE_MODULE_PATH 
	${CMAKE_MODULE_PATH}
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules )

# Make CPack available to easy generate binary packages
include(CPack)

if( NOT FDO_CORE_BUILD )
	find_package( FDO REQUIRED )
endif( NOT FDO_CORE_BUILD )

if( UNIX )
	add_definitions( -DFULLPROTO -D__USE_GNU -DLINUX -fPIC )
endif( UNIX )

find_package( FDOPostGIS REQUIRED )

include_directories( 
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/Inc
	${CMAKE_CURRENT_BINARY_DIR}/Inc
	)

add_subdirectory( Src )
add_subdirectory( Doc )
add_subdirectory( Inc )
