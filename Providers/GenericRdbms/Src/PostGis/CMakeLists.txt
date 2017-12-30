include_directories( BEFORE ${CMAKE_CURRENT_SOURCE_DIR} )

add_subdirectory( Driver )
add_subdirectory( Fdo )
add_subdirectory( SchemaMgr )

set( FdoPostGis_LIB_SRCS
   PostGis.cpp
   stdafx.cpp
)

add_library( PostgreSQLProvider${FDO_VERSION_SUFFIX} SHARED ${FdoPostGis_LIB_SRCS} )

if( CMAKE_COMPILER_IS_GNUCXX )
    target_link_libraries( PostgreSQLProvider${FDO_VERSION_SUFFIX}
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
    target_link_libraries( PostgreSQLProvider${FDO_VERSION_SUFFIX}
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
        FdoRdbmsPostGis_SchemaMgr_Ph
        geometry_fgf
        SmLpGrd
        SmGrd
        SmPhCfgGrd
        SmPhGrd
        SmPhRdGrd
    )
endif( CMAKE_COMPILER_IS_GNUCXX )

target_link_libraries( PostgreSQLProvider${FDO_VERSION_SUFFIX}
    ${POSTGRESQL_LIBRARIES}
    ${ZLIB_LIBRARIES}
    FDO${FDO_VERSION_SUFFIX}
    ExpressionEngine${FDO_VERSION_SUFFIX}
    ProvidersCommon
    RdbmsOverrides${FDO_VERSION_SUFFIX}
    PostgreSQLOverrides${FDO_VERSION_SUFFIX}
    SchemaMgr
)

install( TARGETS PostgreSQLProvider${FDO_VERSION_SUFFIX} DESTINATION ${LIB_INSTALL_DIR} COMPONENT ${FDO_COMPONENT} )
install_symlink( ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/libPostgreSQLProvider${FDO_VERSION_SUFFIX}.so libPostgreSQLProvider.so ${FDO_COMPONENT} )
