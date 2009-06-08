include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${UNMANAGED_INCLUDE_DIR}
	${UNMANAGED_DIR}
	${UTILITIES_COMMON_INCLUDE_DIR}
	)

set( Gdbi_STAT_SRCS
   GdbiCommands.cpp
   GdbiConnection.cpp
   GdbiException.cpp
   GdbiQueryResult.cpp
   GdbiStatement.cpp
   stdafx.cpp
)

add_library( Gdbi STATIC ${Gdbi_STAT_SRCS} )
