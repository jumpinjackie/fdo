project(fdopostgis)

# Look first in internal modules
set( CMAKE_MODULE_PATH 
	${CMAKE_MODULE_PATH}
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules )

# Make CPack available to easy generate binary packages
include(CPack)

find_package( FDOPostGIS REQUIRED )

if( NOT FDO_CORE_BUILD )
	find_package( FDO REQUIRED )
endif( NOT FDO_CORE_BUILD )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/Inc )

add_subdirectory( Src )
#add_subdirectory( Doc )
#add_subdirectory( Inc )
