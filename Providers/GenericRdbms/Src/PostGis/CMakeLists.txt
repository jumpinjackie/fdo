include_directories( BEFORE ${CMAKE_CURRENT_SOURCE_DIR} )

add_subdirectory( Driver )
add_subdirectory( Fdo )
add_subdirectory( SchemaMgr )

set( FdoPostGis_LIB_SRCS
   PostGis.cpp
   stdafx.cpp
)

add_library( FdoPostgreSQL-${FDO_VERSION} SHARED ${FdoPostGis_LIB_SRCS} )

if( CMAKE_COMPILER_IS_GNUCXX )
	target_link_libraries( FdoPostgreSQL-${FDO_VERSION}
		-Wl,-whole-archive
		FdoRdbms
		rdbi
		postgisdr
		Gdbi
		LTManager
		LockManager
		genericrdbms_util
		FdoRdbmsPostGis_Fdo
		FdoRdbmsPostGis_SchemaMgr
		FdoRdbmsPostGis_SchemaMgr_Lp
		FdoRdbmsPostGis_SchemaMgr_Ov
		FdoRdbmsPostGis_SchemaMgr_Ph
		geometry_fgf
		SmLpGrd
		SmGrd
		SmPhCfgGrd
		SmPhGrd
		SmPhRdGrd
		-Wl,-no-whole-archive
 	)
else( CMAKE_COMPILER_IS_GNUCXX )
	target_link_libraries( FdoPostgreSQL-${FDO_VERSION}
		FdoRdbms
		rdbi
		postgisdr
		Gdbi
		LTManager
		LockManager
		genericrdbms_util
		FdoRdbmsPostGis_Fdo
		FdoRdbmsPostGis_SchemaMgr
		FdoRdbmsPostGis_SchemaMgr_Lp
		FdoRdbmsPostGis_SchemaMgr_Ov
		FdoRdbmsPostGis_SchemaMgr_Ph
		geometry_fgf
		SmLpGrd
		SmGrd
		SmPhCfgGrd
		SmPhGrd
		SmPhRdGrd
 	)
endif( CMAKE_COMPILER_IS_GNUCXX )

target_link_libraries( FdoPostgreSQL-${FDO_VERSION}
	${POSTGRESQL_LIBRARIES}
	${ZLIB_LIBRARIES}
	FDO-${FDO_VERSION}
	ExpressionEngine-${FDO_VERSION}
	ProvidersCommon
	SchemaMgr_OV-${FDO_VERSION}
	SchemaMgr
	)

install( TARGETS FdoPostgreSQL-${FDO_VERSION} DESTINATION ${LIB_INSTALL_DIR} )

