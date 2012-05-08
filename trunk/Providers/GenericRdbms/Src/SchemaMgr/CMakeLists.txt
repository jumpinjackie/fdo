include_directories( BEFORE	${CMAKE_CURRENT_SOURCE_DIR} )

add_subdirectory( Lp )
add_subdirectory( Ov )
add_subdirectory( Ph )

set( SmGrd_STAT_SRCS
   SchemaManager.cpp
   stdafx.cpp
)

add_library( SmGrd STATIC ${SmGrd_STAT_SRCS} )
add_dependencies( SmGrd fdordbmsmsg )
