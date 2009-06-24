include_directories( BEFORE	${CMAKE_CURRENT_SOURCE_DIR} )

add_subdirectory( Lp )
add_subdirectory( Ov )
add_subdirectory( Ph )

set( FdoRdbmsODBC_SchemaMgr_STAT_SRCS
   SchemaManager.cpp
   stdafx.cpp
)

add_library( FdoRdbmsODBC_SchemaMgr STATIC ${FdoRdbmsODBC_SchemaMgr_STAT_SRCS} )
