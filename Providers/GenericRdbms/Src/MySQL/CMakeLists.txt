include_directories( BEFORE ${CMAKE_CURRENT_SOURCE_DIR} )

add_subdirectory( Driver )
add_subdirectory( Fdo )
add_subdirectory( SchemaMgr )

set( FdoMySQL_LIB_SRCS
   MySql.cpp
   stdafx.cpp
)

add_library( FdoMySQL-${FDO_VERSION} SHARED ${FdoMySQL_LIB_SRCS} )

if( CMAKE_COMPILER_IS_GNUCXX )
	target_link_libraries( FdoMySQL-${FDO_VERSION}
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
        ProvidersCommon
    	SchemaMgr
		-Wl,-no-whole-archive
 	)
else( CMAKE_COMPILER_IS_GNUCXX )
	target_link_libraries( FdoMySQL-${FDO_VERSION}
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
        ProvidersCommon
    	SchemaMgr
 	)
endif( CMAKE_COMPILER_IS_GNUCXX )

target_link_libraries( FdoMySQL-${FDO_VERSION}
	${MYSQL_LIBRARIES}
	${ZLIB_LIBRARIES}
	FDO-${FDO_VERSION}
	ExpressionEngine-${FDO_VERSION}
	SchemaMgr_OV-${FDO_VERSION}
)

install( TARGETS FdoMySQL-${FDO_VERSION} DESTINATION ${LIB_INSTALL_DIR} )

