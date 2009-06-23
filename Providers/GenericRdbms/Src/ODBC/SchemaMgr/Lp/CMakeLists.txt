include_directories(
	BEFORE
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../Inc/Rdbi
	${CMAKE_CURRENT_SOURCE_DIR}/../../../
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../
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
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Gdbi
	${CMAKE_CURRENT_SOURCE_DIR}/../../../LongTransactionManager
	${CMAKE_CURRENT_SOURCE_DIR}/../../../LockManager
	${UTILITIES_COMMON_INCLUDE_DIR}
	${UTILITIES_SCHEMAMGR_INCLUDE_DIR}
	${UTILITIES_SCHEMAMGR_INCLUDE_DIR}/Sm/Lp
	${UNMANAGED_INCLUDE_DIR}
	${UNMANAGED_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	)

set( FdoRdbmsODBC_SchemaMgr_Lp_STAT_SRCS
   Class.cpp
   ClassDefinition.cpp
   DataPropertyDefinition.cpp
   FeatureClass.cpp
   GeometricPropertyDefinition.cpp
   SchemaCollection.cpp
   Schema.cpp
)

add_library( FdoRdbmsODBC_SchemaMgr_Lp STATIC ${FdoRdbmsODBC_SchemaMgr_Lp_STAT_SRCS} )
