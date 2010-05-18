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
	SpatialIndexDescriptor.cpp
	SQLiteSchemaMergeContext.cpp
	SltBLOBStreamReader.cpp
	) 

if( UNIX )
	add_definitions(-msse -msse2 -mmmx -DSQLITE_OMIT_AUTHORIZATION -DSQLITE_OMIT_PROGRESS_CALLBACK)
endif( UNIX )

add_library (SQLiteProvider SHARED ${SQLiteProvider_SRCS} )

target_link_libraries ( SQLiteProvider 
	ProvidersCommon 
	ExpressionEngine
	FDO 
	) 

if( CMAKE_COMPILER_IS_GNUCXX )
	target_link_libraries( SQLiteProvider
		-Wl,-whole-archive
		SQLiteSpatialIndex 
		sqlitefdoprovider3
		-Wl,-no-whole-archive
		)
else( CMAKE_COMPILER_IS_GNUCXX )
	target_link_libraries( SQLiteProvider
		SQLiteSpatialIndex 
		sqlitefdoprovider3
		)
endif( CMAKE_COMPILER_IS_GNUCXX )

set_target_properties( SQLiteProvider PROPERTIES VERSION ${FDO_VERSION} SOVERSION ${FDO_VERSION_MAJOR} )

install( TARGETS SQLiteProvider DESTINATION ${LIB_INSTALL_DIR} )
