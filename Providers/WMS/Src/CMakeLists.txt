include_directories(
	${CMAKE_CURRENT_BINARY_DIR}/Message
	${CMAKE_CURRENT_BINARY_DIR}/Message/Inc
	)

add_subdirectory( Message )
add_subdirectory( Overrides )
add_subdirectory( Provider )
add_subdirectory( UnitTest )

