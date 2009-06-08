include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${UNMANAGED_INCLUDE_DIR}
	${UNMANAGED_DIR}
	${UTILITIES_SCHEMAMGR_INCLUDE_DIR}
	${UTILITIES_COMMON_INCLUDE_DIR}
	)

add_subdirectory( Lp )
add_subdirectory( Ov )
add_subdirectory( Ph )

set( SmGrd_STAT_SRCS
   SchemaManager.cpp
   stdafx.cpp
)

add_library( SmGrd STATIC ${SmGrd_STAT_SRCS} )
