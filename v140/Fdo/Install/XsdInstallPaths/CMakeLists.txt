include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	)

set( XsdInstallPaths_SRCS
   XsdInstallPaths.cpp
)

add_executable( XsdInstallPaths ${XsdInstallPaths_SRCS} )
