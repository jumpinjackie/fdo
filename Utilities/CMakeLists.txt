find_package( ZLIB )

# Make global include directories, even binary build dir 
# to match build generated files
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/Inc 
	)

add_subdirectory( Common )
add_subdirectory( TestCommon )
add_subdirectory( ExpressionEngine )
if( ZLIB_FOUND )
	add_subdirectory( OWS )
endif( ZLIB_FOUND )
add_subdirectory( SchemaMgr )
# Disabled until find a better code that not use internal headers
#add_subdirectory( SQLiteInterface )
