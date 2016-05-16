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
	SltTransaction.cpp
	) 

if( UNIX )
	add_definitions(-msse -msse2 -mmmx -DSQLITE_OMIT_AUTHORIZATION -DSQLITE_OMIT_PROGRESS_CALLBACK -DSQLITE -D_FILE_OFFSET_BITS=64)
endif( UNIX )

add_library (SQLiteProvider-${FDO_VERSION} SHARED ${SQLiteProvider_SRCS} )

target_link_libraries ( SQLiteProvider-${FDO_VERSION} 
	ProvidersCommon 
	ExpressionEngine-${FDO_VERSION}
	FDO-${FDO_VERSION} 
	) 

if( CMAKE_COMPILER_IS_GNUCXX )
	target_link_libraries( SQLiteProvider-${FDO_VERSION}
		-Wl,-whole-archive
		SQLiteSpatialIndex 
		sqlitefdoprovider3
		-Wl,-no-whole-archive
		)
else( CMAKE_COMPILER_IS_GNUCXX )
	target_link_libraries( SQLiteProvider-${FDO_VERSION}
		SQLiteSpatialIndex 
		sqlitefdoprovider3
		)
endif( CMAKE_COMPILER_IS_GNUCXX )

install( TARGETS SQLiteProvider-${FDO_VERSION} DESTINATION ${LIB_INSTALL_DIR} )
