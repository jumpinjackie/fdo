include_directories( 
	${CMAKE_CURRENT_SOURCE_DIR}/Inc 
	${CMAKE_CURRENT_SOURCE_DIR}/SpatialIndex
	${CMAKE_CURRENT_SOURCE_DIR}/Message/Inc
	${CMAKE_CURRENT_SOURCE_DIR}/sqlite )

# Provider uses a special modified version of sqlite
add_subdirectory( sqlite )
add_subdirectory( SpatialIndex )
add_subdirectory( Provider )

