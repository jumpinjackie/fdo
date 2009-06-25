include_directories( BEFORE	${CMAKE_CURRENT_SOURCE_DIR}	)

set( FdoRdbmsODBC_SchemaMgr_Ph_STAT_SRCS
   ColTypeMapper.cpp
   Column.cpp
   Database.cpp
   DbObject.cpp
   Fkey.cpp
   Index.cpp
   Mgr.cpp
   Owner.cpp
   Table.cpp
   TempObject.cpp
   View.cpp
   Rd/ColumnReader.cpp
   Rd/ConstraintReader.cpp
   Rd/DbObjectReader.cpp
   Rd/OraBaseObjectReader.cpp
   Rd/FkeyReader.cpp
   Rd/IndexReader.cpp
   Rd/OraColumnReader.cpp
   Rd/OraDbObjectReader.cpp
   Rd/OraPkeyReader.cpp
   Rd/OwnerReader.cpp
   Rd/SchemaReader.cpp
   Rd/PkeyReader.cpp
)

add_library( FdoRdbmsODBC_SchemaMgr_Ph STATIC ${FdoRdbmsODBC_SchemaMgr_Ph_STAT_SRCS} )
