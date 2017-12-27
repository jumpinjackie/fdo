include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${UNMANAGED_INCLUDE_DIR}
    ${UTILITIES_COMMON_INCLUDE_DIR}
    ${UTILITIES_EXPRESSION_INCLUDE_DIR}
    ${GDAL_INCLUDE_DIR}
    )

set( OGRProvider_SRCS
    OgrFdoUtil.cpp
    OgrFilterIdentifierExtractor.cpp
    OgrProvider.cpp
    OgrSpatialExtentsDataReader.cpp
    ProjConverter.cpp
)

add_library( OGRProvider${FDO_VERSION_SUFFIX} SHARED ${OGRProvider_SRCS} )

target_link_libraries( OGRProvider${FDO_VERSION_SUFFIX}
    FDO${FDO_VERSION_SUFFIX}
    ExpressionEngine${FDO_VERSION_SUFFIX}
    ProvidersCommon
    ${GDAL_LIBRARY}
)

install( TARGETS OGRProvider${FDO_VERSION_SUFFIX} DESTINATION ${LIB_INSTALL_DIR} )
install_symlink( ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/libOGRProvider${FDO_VERSION_SUFFIX}.so ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/libOGRProvider.so )
