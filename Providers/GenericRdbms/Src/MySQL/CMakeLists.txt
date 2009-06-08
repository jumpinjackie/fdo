include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${UNMANAGED_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../../Inc/Rdbi
	${CMAKE_CURRENT_SOURCE_DIR}/../Fdo/Connection
	${CMAKE_CURRENT_SOURCE_DIR}/../Fdo/Other
	${CMAKE_CURRENT_SOURCE_DIR}/../Fdo/FeatureCommands
	${CMAKE_CURRENT_SOURCE_DIR}/../Fdo/Custom
	${CMAKE_CURRENT_SOURCE_DIR}/../Fdo/Schema
	${CMAKE_CURRENT_SOURCE_DIR}/../Fdo/Lt
	${CMAKE_CURRENT_SOURCE_DIR}/../Fdo/Lock
	${CMAKE_CURRENT_SOURCE_DIR}/../Fdo/SpatialContext
	${UNMANAGED_DIR}
	${UTILITIES_SCHEMAMGR_INCLUDE_DIR}
	${UTILITIES_COMMON_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../Gdbi
	${CMAKE_CURRENT_SOURCE_DIR}/../LongTransactionManager
	${CMAKE_CURRENT_SOURCE_DIR}/../LockManager
	${MYSQL_INCLUDE_DIR}
	)

add_subdirectory( Driver )
add_subdirectory( Fdo )
add_subdirectory( SchemaMgr )

add_definitions( -DFULLPROTO -D__USE_GNU -DLINUX )

set( FdoRdbmsMySQL_STAT_SRCS
   MySql.cpp
   stdafx.cpp
)

add_library( FdoRdbmsMySQL STATIC ${FdoRdbmsMySQL_STAT_SRCS} )

target_link_libraries( FdoRdbmsMySQL 
	FdoRdbmsMySQL_SchemaMgr
	FdoRdbmsMySQL_SchemaMgr_Lp
	FdoRdbmsMySQL_SchemaMgr_Ov
	FdoRdbmsMySQL_SchemaMgr_Ph
	)
