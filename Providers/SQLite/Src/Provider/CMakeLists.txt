include_directories (
	${CMAKE_CURRENT_SOURCE_DIR}
	${UNMANAGED_INCLUDE_DIR}
	${UTILITIES_COMMON_INCLUDE_DIR}
	${UTILITIES_EXPRESSION_INCLUDE_DIR}
	) 

set( SQLiteProvider_SRCS
	SltConversionUtils.cpp
	slt.cpp
	SltExprExtensions.cpp
	SltMetadata.cpp
	SltProvider.cpp
	SltQueryTranslator.cpp
	SltReader.cpp
	SltSpatialContextReader.cpp
	) 

add_library (SQLiteProvider SHARED ${SQLiteProvider_SRCS} )

target_link_libraries ( SQLiteProvider 
	SQLiteSpatialIndex 
	sqlitefdo3
	ProvidersCommon 
	FDO 
	) 

set_target_properties( SQLiteProvider PROPERTIES VERSION ${FDO_VERSION} SOVERSION ${FDO_VERSION_MAJOR} )

install( TARGETS SQLiteProvider DESTINATION ${LIB_INSTALL_DIR} )
