include_directories( BEFORE	${CMAKE_CURRENT_SOURCE_DIR} )

set( FdoRdbmsPostGis_SchemaMgr_Ph_Rd_STAT_SRCS
   BaseObjectReader.cpp
   ClassReader.cpp
   ColumnReader.cpp
   ConstraintReader.cpp
   DbObjectReader.cpp
   DbSchemaReader.cpp
   FkeyReader.cpp
   IndexReader.cpp
   OwnerReader.cpp
   PkeyReader.cpp
   SchemaReader.cpp
   SpatialContextReader.cpp
)

add_library( FdoRdbmsPostGis_SchemaMgr_Ph_Rd STATIC ${FdoRdbmsPostGis_SchemaMgr_Ph_Rd_STAT_SRCS} )
add_dependencies( FdoRdbmsPostGis_SchemaMgr_Ph_Rd fdordbmsmsg )
