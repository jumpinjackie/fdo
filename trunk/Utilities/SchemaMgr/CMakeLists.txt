include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/Inc
	${CMAKE_CURRENT_BINARY_DIR}/Inc
        ${UNMANAGED_INCLUDE_DIRS}
	)

add_subdirectory( Inc )
add_subdirectory( Nls )
add_subdirectory( Src )
