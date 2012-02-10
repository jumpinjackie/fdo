include_directories( BEFORE	${CMAKE_CURRENT_SOURCE_DIR} )

add_subdirectory( Lp )
add_subdirectory( Ov )
add_subdirectory( Ph )

set( FdoRdbmsPostGis_SchemaMgr_STAT_SRCS
   SchemaManager.cpp
   stdafx.cpp
)

add_library( FdoRdbmsPostGis_SchemaMgr STATIC ${FdoRdbmsPostGis_SchemaMgr_STAT_SRCS} )
add_dependencies( FdoRdbmsPostGis_SchemaMgr fdordbmsmsg )
