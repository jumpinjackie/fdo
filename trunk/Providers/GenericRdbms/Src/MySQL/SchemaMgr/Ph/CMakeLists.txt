include_directories( BEFORE ${CMAKE_CURRENT_SOURCE_DIR} )

add_subdirectory( Rd )

set( FdoRdbmsMySQL_SchemaMgr_Ph_STAT_SRCS
   CharacterSet.cpp
   Collation.cpp
   ColTypeMapper.cpp
   ColumnChar.cpp
   ColumnGeom.cpp
   Column.cpp
   ColumnDecimal.cpp
   Database.cpp
   DbObject.cpp
   Fkey.cpp
   Index.cpp
   Mgr.cpp
   Owner.cpp
   Table.cpp
   TempObject.cpp
   View.cpp
)

add_library( FdoRdbmsMySQL_SchemaMgr_Ph STATIC ${FdoRdbmsMySQL_SchemaMgr_Ph_STAT_SRCS} )

target_link_libraries( FdoRdbmsMySQL_SchemaMgr_Ph
	FdoRdbmsMySQL_SchemaMgr_Ph_Rd
)
