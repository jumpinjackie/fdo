include_directories( BEFORE ${CMAKE_CURRENT_SOURCE_DIR} )

add_subdirectory( Driver )
add_subdirectory( Fdo )
add_subdirectory( SchemaMgr )

set( FdoMySQL_LIB_SRCS
   MySql.cpp
   stdafx.cpp
)

add_library( MySQLProvider${FDO_VERSION_SUFFIX} SHARED ${FdoMySQL_LIB_SRCS} )

if( CMAKE_COMPILER_IS_GNUCXX )
    target_link_libraries( MySQLProvider${FDO_VERSION_SUFFIX}
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
    target_link_libraries( MySQLProvider${FDO_VERSION_SUFFIX}
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

target_link_libraries( MySQLProvider${FDO_VERSION_SUFFIX}
    ${MYSQL_LIBRARIES}
    ${ZLIB_LIBRARIES}
    FDO${FDO_VERSION_SUFFIX}
    ExpressionEngine${FDO_VERSION_SUFFIX}
    RdbmsOverrides${FDO_VERSION_SUFFIX}
    MySQLOverrides${FDO_VERSION_SUFFIX}
)

install( TARGETS MySQLProvider${FDO_VERSION_SUFFIX} DESTINATION ${LIB_INSTALL_DIR} COMPONENT ${FDO_COMPONENT} )
install_symlink( ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/libMySQLProvider${FDO_VERSION_SUFFIX}.so libMySQLProvider.so ${FDO_COMPONENT} )
