include_directories( BEFORE ${CMAKE_CURRENT_SOURCE_DIR} )

add_subdirectory( Driver )
add_subdirectory( Fdo )
add_subdirectory( SchemaMgr )

set( FdoPostGis_LIB_SRCS
   PostGis.cpp
   stdafx.cpp
)

add_library( FdoPostgreSQL SHARED ${FdoPostGis_LIB_SRCS} )

if( CMAKE_COMPILER_IS_GNUCXX )
	target_link_libraries( FdoPostgreSQL
		-Wl,-whole-archive
		FdoRdbms
		rdbi
		PostGisdr
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
	target_link_libraries( FdoPostgreSQL
		FdoRdbms
		rdbi
		PostGisdr
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

target_link_libraries( FdoPostgreSQL
	xalan-c
	xalanMsg
	xerces-c
	${PostGis_LIBRARIES}
	${ZLIB_LIBRARIES}
	FDO
	ExpressionEngine
	ProvidersCommon
	SchemaMgr_OV
	SchemaMgr
	)

set_target_properties( FdoPostgreSQL PROPERTIES VERSION ${FDO_VERSION} SOVERSION ${FDO_VERSION_MAJOR} )

install( TARGETS FdoPostgreSQL DESTINATION ${LIB_INSTALL_DIR} )

