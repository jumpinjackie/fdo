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

add_library( FdoODBC SHARED ${FdoODBC_LIB_SRCS} )

if( CMAKE_COMPILER_IS_GNUCXX )
	target_link_libraries( FdoODBC
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
		-Wl,-no-whole-archive
 )
else( CMAKE_COMPILER_IS_GNUCXX )
	target_link_libraries( FdoODBC
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
 )
endif( CMAKE_COMPILER_IS_GNUCXX )

target_link_libraries( FdoODBC
	xalan-c
	xalanMsg
	xerces-c
	odbc
	odbcinst
	odbcpsql
	FDO
	ExpressionEngine
	ProvidersCommon
	SchemaMgr_OV
	SchemaMgr
	)



set_target_properties( FdoODBC PROPERTIES VERSION ${FDO_VERSION} SOVERSION ${FDO_VERSION_MAJOR} )
install( TARGETS FdoODBC DESTINATION ${LIB_INSTALL_DIR} )


