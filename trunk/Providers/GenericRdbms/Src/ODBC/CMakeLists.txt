include_directories( BEFORE
	Fdo
	${CMAKE_CURRENT_SOURCE_DIR}
	)

add_subdirectory( Fdo )
add_subdirectory( SchemaMgr )

set( FdoODBC_LIB_SRCS
   Odbc.cpp
   stdafx.cpp
)

add_library( FdoODBC-${FDO_VERSION} SHARED ${FdoODBC_LIB_SRCS} )

if( CMAKE_COMPILER_IS_GNUCXX )
	target_link_libraries( FdoODBC-${FDO_VERSION}
		-Wl,-whole-archive
		genericrdbms_util
		rdbi
		FdoRdbms
		FdoRdbmsODBC_Fdo
		FdoRdbmsODBC_SchemaMgr
		FdoRdbmsODBC_SchemaMgr_Lp
		FdoRdbmsODBC_SchemaMgr_Ov
		FdoRdbmsODBC_SchemaMgr_Ph
		FdoODBCDriver
		Gdbi
		LTManager
		LockManager
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
	target_link_libraries( FdoODBC-${FDO_VERSION}
		FdoRdbms
		FdoRdbmsODBC_Fdo
		FdoRdbmsODBC_SchemaMgr_Lp
		FdoRdbmsODBC_SchemaMgr_Ov
		FdoRdbmsODBC_SchemaMgr_Ph
		rdbi
		FdoODBCDriver
		Gdbi
		LTManager
		LockManager
		genericrdbms_util
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

target_link_libraries( FdoODBC-${FDO_VERSION}
	${ODBC_LIBRARY}
	${ODBCINST_LIBRARY}
	FDO-${FDO_VERSION}
	ExpressionEngine-${FDO_VERSION}
	SchemaMgr_OV-${FDO_VERSION}
	rdbi
	Gdbi
	FdoRdbms
)

install( TARGETS FdoODBC-${FDO_VERSION} DESTINATION ${LIB_INSTALL_DIR} )


