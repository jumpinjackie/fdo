include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}/Message
	)

add_subdirectory( Message )
add_subdirectory( Utils )
add_subdirectory( Provider )
add_subdirectory( UnitTest )
