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

add_library( ODBCProvider${FDO_VERSION_SUFFIX} SHARED ${FdoODBC_LIB_SRCS} )

if (UNIX)
    add_definitions(-DCMAKE_BUILD)
endif (UNIX)

if( CMAKE_COMPILER_IS_GNUCXX )
    target_link_libraries( ODBCProvider${FDO_VERSION_SUFFIX}
        -Wl,-whole-archive
        genericrdbms_util
        rdbi
        FdoRdbms
        FdoRdbmsODBC_Fdo
        FdoRdbmsODBC_SchemaMgr
        FdoRdbmsODBC_SchemaMgr_Lp
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
    target_link_libraries( ODBCProvider${FDO_VERSION_SUFFIX}
        FdoRdbms
        FdoRdbmsODBC_Fdo
        FdoRdbmsODBC_SchemaMgr_Lp
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

target_link_libraries( ODBCProvider${FDO_VERSION_SUFFIX}
    ${ODBC_LIBRARY}
    ${ODBCINST_LIBRARY}
    FDO${FDO_VERSION_SUFFIX}
    ExpressionEngine${FDO_VERSION_SUFFIX}
    RdbmsOverrides${FDO_VERSION_SUFFIX}
    ODBCOverrides${FDO_VERSION_SUFFIX}
    rdbi
    Gdbi
    FdoRdbms
)

install( TARGETS ODBCProvider${FDO_VERSION_SUFFIX} DESTINATION ${LIB_INSTALL_DIR} COMPONENT ${FDO_COMPONENT} )
install_symlink( ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/libODBCProvider${FDO_VERSION_SUFFIX}.so libODBCProvider.so ${FDO_COMPONENT} )

# Needed for unit tests
file(COPY "../../com" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})