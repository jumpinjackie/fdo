include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${UNMANAGED_INCLUDE_DIR}
	${UNMANAGED_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../Gdbi
	${UTILITIES_COMMON_INCLUDE_DIR}
	${UTILITIES_SCHEMAMGR_INCLUDE_DIR}/Sm/Lp
	${UTILITIES_SCHEMAMGR_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../Inc
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../Inc/Rdbi
	${CMAKE_CURRENT_SOURCE_DIR}/../../../
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../Inc
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../Src/Fdo/Other
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../Src/Fdo/Capability
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../Src/Fdo/Connection
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../Src/Fdo/Custom
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../Src/Fdo/FeatureCommands
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../Src/Fdo/Filter
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../Src/Fdo/Lock
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../Src/Fdo/Lt
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../Src/Fdo/Schema
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../Src/Fdo/SpatialContext
	${UNMANAGED_INCLUDE_DIR}
	${UNMANAGED_DIR}/Src/Geometry
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Gdbi
	${CMAKE_CURRENT_SOURCE_DIR}/../../../LongTransactionManager
	${CMAKE_CURRENT_SOURCE_DIR}/../../../LockManager
	)

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
