include_directories( BEFORE	${CMAKE_CURRENT_SOURCE_DIR} )

set( FdoRdbmsMySQL_SchemaMgr_Ph_Rd_STAT_SRCS
   CharacterSetReader.cpp
   CollationReader.cpp
   ColumnReader.cpp
   ConstraintReader.cpp
   DbObjectReader.cpp
   FkeyReader.cpp
   IndexReader.cpp
   OwnerReader.cpp
   PkeyReader.cpp
)

add_library( FdoRdbmsMySQL_SchemaMgr_Ph_Rd STATIC ${FdoRdbmsMySQL_SchemaMgr_Ph_Rd_STAT_SRCS} )
add_dependencies( FdoRdbmsMySQL_SchemaMgr_Ph_Rd fdordbmsmsg )
