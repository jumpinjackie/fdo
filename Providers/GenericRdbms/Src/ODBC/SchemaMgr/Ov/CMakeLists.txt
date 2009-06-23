include_directories(
	BEFORE
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../Src/Fdo/Other
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../Src/Fdo/Capability
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../Src/Fdo/Connection
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../Src/Fdo/Custom
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../Src/Fdo/FeatureCommands
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../Src/Fdo/Filter
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../Src/Fdo/Lock
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../Src/Fdo/Lt
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../Src/Fdo/Schema
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../Src/Fdo/SpatialContext
	${UTILITIES_SCHEMAMGR_INCLUDE_DIR}/Sm/Lp
	${CMAKE_CURRENT_SOURCE_DIR}
	)

set( FdoRdbmsODBC_SchemaMgr_Ov_STAT_SRCS
   OdbcOvClassCollection.cpp
   OdbcOvClassDefinition.cpp
   OdbcOvColumn.cpp
   OdbcOvDataPropertyDefinition.cpp
   OdbcOvGeometricPropertyDefinition.cpp
   OdbcOvPhysicalSchemaMapping.cpp
   OdbcOvPropertyDefinitionCollection.cpp
   OdbcOvPropertyDefinition.cpp
   OdbcOvTable.cpp
   stdafx.cpp
)

add_library( FdoRdbmsODBC_SchemaMgr_Ov STATIC ${FdoRdbmsODBC_SchemaMgr_Ov_STAT_SRCS} )
