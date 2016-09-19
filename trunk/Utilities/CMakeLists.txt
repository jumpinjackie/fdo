find_package( ZLIB REQUIRED )

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
add_subdirectory( OWS )
add_subdirectory( SchemaMgr )
add_subdirectory( SQLiteInterface )
