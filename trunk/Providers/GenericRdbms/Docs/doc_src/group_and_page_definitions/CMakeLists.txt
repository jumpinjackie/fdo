include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	)

add_subdirectory( MySQL )
add_subdirectory( ODBC )
add_subdirectory( SQLServerSpatial )
