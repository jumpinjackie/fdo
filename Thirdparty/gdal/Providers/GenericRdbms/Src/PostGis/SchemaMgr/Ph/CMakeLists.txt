include_directories( BEFORE ${CMAKE_CURRENT_SOURCE_DIR} )

add_subdirectory( Rd )

set( FdoRdbmsMySQL_SchemaMgr_Ph_STAT_SRCS
   ColTypeMapper.cpp
  Column.cpp
  ColumnBLOB.cpp
  ColumnBool.cpp
  ColumnByte.cpp
  ColumnChar.cpp
  ColumnDate.cpp
  ColumnDecimal.cpp
  ColumnDouble.cpp
  ColumnGeom.cpp
  ColumnInt16.cpp
  ColumnInt32.cpp
  ColumnInt64.cpp
  ColumnSingle.cpp
  ColumnUnknown.cpp
  Database.cpp
  DbObject.cpp
  Fkey.cpp
  Index.cpp
  Mgr.cpp
  Owner.cpp
  Schema.cpp
  SpatialContextGroupWriter.cpp
  SpatialIndex.cpp
  Table.cpp
  TempObject.cpp
  View.cpp
)

add_library( FdoRdbmsMySQL_SchemaMgr_Ph STATIC ${FdoRdbmsMySQL_SchemaMgr_Ph_STAT_SRCS} )

target_link_libraries( FdoRdbmsMySQL_SchemaMgr_Ph
	FdoRdbmsMySQL_SchemaMgr_Ph_Rd
)
