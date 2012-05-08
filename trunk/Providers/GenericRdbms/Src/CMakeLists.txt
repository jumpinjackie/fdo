include_directories( 
	${CMAKE_CURRENT_SOURCE_DIR}
	${UNMANAGED_INCLUDE_DIR}
	${UNMANAGED_DIR}
	${UTILITIES_SCHEMAMGR_INCLUDE_DIR}
	${UTILITIES_COMMON_INCLUDE_DIR}
	${UNMANAGED_DIR}/Src/Geometry
	${UTILITIES_EXPRESSION_INCLUDE_DIR}
	${UTILITIES_SCHEMAMGR_INCLUDE_DIR}
	${UTILITIES_SCHEMAMGR_INCLUDE_DIR}/Sm/Lp
	${UTILITIES_COMMON_INCLUDE_DIR}
	Gdbi
	LockManager
	LongTransactionManager
    Fdo/Connection
	Fdo/Other
	Fdo/FeatureCommands
	Fdo/Custom
	Fdo/Filter
	Fdo/Schema
	Fdo/Lt
	Fdo/Lock
	Fdo/SpatialContext
	)

add_subdirectory( Fdo )
add_subdirectory( Gdbi )
add_subdirectory( LongTransactionManager )
add_subdirectory( LockManager )
add_subdirectory( MySQL )
add_subdirectory( Rdbi )
add_subdirectory( SchemaMgr )
add_subdirectory( Util )
add_subdirectory( Geometry )
add_subdirectory( ODBCDriver )
add_subdirectory( ODBC )
# Later
# add_subdirectory( UnitTest )
