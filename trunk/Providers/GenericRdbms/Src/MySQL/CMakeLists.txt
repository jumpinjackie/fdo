include_directories( BEFORE ${CMAKE_CURRENT_SOURCE_DIR} )

add_subdirectory( Driver )
add_subdirectory( Fdo )
add_subdirectory( SchemaMgr )

set( FdoMySQL_LIB_SRCS
   MySql.cpp
   stdafx.cpp
)

add_library( FdoMySQL SHARED ${FdoMySQL_LIB_SRCS} )

if( CMAKE_COMPILER_IS_GNUCXX )
	target_link_libraries( FdoMySQL
		-Wl,-whole-archive
		FdoRdbms
		rdbi
		mysqldr
		Gdbi
		LTManager
		LockManager
		genericrdbms_util
		FdoRdbmsMySQL_Fdo
		FdoRdbmsMySQL_SchemaMgr
		FdoRdbmsMySQL_SchemaMgr_Lp
		FdoRdbmsMySQL_SchemaMgr_Ov
		FdoRdbmsMySQL_SchemaMgr_Ph
		geometry_fgf
		SmLpGrd
		SmGrd
		SmPhCfgGrd
		SmPhGrd
		SmPhRdGrd
		-Wl,-no-whole-archive
 	)
else( CMAKE_COMPILER_IS_GNUCXX )
	target_link_libraries( FdoMySQL
		FdoRdbms
		rdbi
		mysqldr
		Gdbi
		LTManager
		LockManager
		genericrdbms_util
		FdoRdbmsMySQL_Fdo
		FdoRdbmsMySQL_SchemaMgr
		FdoRdbmsMySQL_SchemaMgr_Lp
		FdoRdbmsMySQL_SchemaMgr_Ov
		FdoRdbmsMySQL_SchemaMgr_Ph
		geometry_fgf
		SmLpGrd
		SmGrd
		SmPhCfgGrd
		SmPhGrd
		SmPhRdGrd
 	)
endif( CMAKE_COMPILER_IS_GNUCXX )

target_link_libraries( FdoMySQL
	xalan-c
	xalanMsg
	xerces-c
	${MYSQL_LIBRARIES}
	${ZLIB_LIBRARIES}
	FDO
	ExpressionEngine
	ProvidersCommon
	SchemaMgr_OV
	SchemaMgr
	)

set_target_properties( FdoMySQL PROPERTIES VERSION ${FDO_VERSION} SOVERSION ${FDO_VERSION_MAJOR} )

install( TARGETS FdoMySQL DESTINATION ${LIB_INSTALL_DIR} )

