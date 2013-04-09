project(fdogenericrdbms)

# Look first in internal modules
set( CMAKE_MODULE_PATH 
	${CMAKE_MODULE_PATH}
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules )

# Make CPack available to easy generate binary packages
include(CPack)

find_package( MySQL REQUIRED )
find_package( UnixODBC REQUIRED )

if( NOT FDO_CORE_BUILD )
	find_package( FDO REQUIRED )
endif( NOT FDO_CORE_BUILD )

include_directories( 
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/Inc 
	${CMAKE_CURRENT_SOURCE_DIR}/Inc/Rdbi 
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_BINARY_DIR}/Inc )

add_definitions( ${ODBC_DEFINITIONS} )


add_subdirectory( Nls )
add_subdirectory( Src )
add_subdirectory( Inc )
add_subdirectory( com )
add_subdirectory( Docs )

