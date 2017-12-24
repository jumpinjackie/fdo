include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../../inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../Message/Inc
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/Inc
    ${UNMANAGED_INCLUDE_DIR}
    ${UTILITIES_COMMON_INCLUDE_DIR}
    ${ORACLE_INCLUDES}
)

set (KingOracleProvider_SRCS
    c_FdoOra_API3.cpp
    c_FdoOra_API2.cpp
    c_KgOraRasterCapabilities.cpp
    c_FgfToSdoGeom.cpp
    c_KgOraReader.cpp
    c_KgOraDataReader.cpp
    c_FilterStringBuffer.cpp
    c_KgOraSchemaCapabilities.cpp
    c_KgOraApplySchema.cpp
    c_KgOraSchemaDesc.cpp
    c_KgOraCommandCapabilities.cpp
    c_KgOraSchemaPool.cpp
    c_KgOraConnectionCapabilities.cpp
    c_KgOraSelectCommand.cpp
    c_KgOraConnection.cpp
    c_KgOraSpatialContext.cpp
    c_KgOraConnectionInfo.cpp
    c_KgOraSpatialContextReader.cpp
    c_KgOraCreateDataStore.cpp
    c_KgOraSQLCommand.cpp
    c_KgOraCreateSpatialContext.cpp
    c_KgOraSQLDataReader.cpp
    c_KgOraDelete.cpp
    c_KgOraSqlParamDesc.cpp
    c_KgOraDescribeSchemaCommand.cpp
    c_KgOraSridDesc.cpp
    c_KgOraExpressionCapabilities.cpp
    c_KgOraTopologyCapabilities.cpp
    c_KgOraExpressionProcessor.cpp
    c_KgOraUpdate.cpp
    c_KgOraFeatureReader.cpp
    c_LogAPI.cpp
    c_KgOraFeatureReaderInsert.cpp
    c_OCI_API.cpp
    c_Oci_ColumnData.cpp
    c_Oci_Statement.cpp
    c_KgOraSelectAggregates.cpp
    c_KgOraFilterCapabilities.cpp
    c_Ora_API2.cpp
    c_KgOraFilterProcessor.cpp
    c_SdoGeomToAGF2.cpp
    c_KgOraGeometryCapabilities.cpp
    c_KgOraInsert.cpp
    c_KgOraGetSpatialContextsCommand.cpp
    c_SDO_DIM_ELEMENT.cpp
    c_SDO_GEOMETRY.cpp
    c_SdeGeom2AGF.cpp
)

if( UNIX )
    add_definitions(-D_FDO_3_2 -D_FDO_3_5)
endif( UNIX )

add_library( KingOracleProvider-${FDO_VERSION} SHARED ${KingOracleProvider_SRCS} )

target_link_libraries( KingOracleProvider-${FDO_VERSION} 
    KingOracleOverridesSrc-${FDO_VERSION}    
    FDO-${FDO_VERSION}
    ProvidersCommon
    ${ORACLE_LIBRARIES}
)

install( TARGETS KingOracleProvider-${FDO_VERSION} DESTINATION ${LIB_INSTALL_DIR} )
install_symlink( ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/libKingOracleProvider-${FDO_VERSION}.so ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/libKingOracleProvider.so )