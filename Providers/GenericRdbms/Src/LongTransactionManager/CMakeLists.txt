include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${UTILITIES_COMMON_INCLUDE_DIR}
	${UTILITIES_SCHEMAMGR_INCLUDE_DIR}
	${UTILITIES_SCHEMAMGR_INCLUDE_DIR}/Sm/Lp
	${CMAKE_CURRENT_SOURCE_DIR}/../../Inc/Rdbi
	${CMAKE_CURRENT_SOURCE_DIR}/../Fdo/Other
	${CMAKE_CURRENT_SOURCE_DIR}/../Fdo/Capability
	${CMAKE_CURRENT_SOURCE_DIR}/../Fdo/Connection
	${CMAKE_CURRENT_SOURCE_DIR}/../Fdo/Custom
	${CMAKE_CURRENT_SOURCE_DIR}/../Fdo/FeatureCommands
	${CMAKE_CURRENT_SOURCE_DIR}/../Fdo/Filter
	${CMAKE_CURRENT_SOURCE_DIR}/../Fdo/Lock
	${CMAKE_CURRENT_SOURCE_DIR}/../Fdo/Lt
	${CMAKE_CURRENT_SOURCE_DIR}/../Fdo/Schema
	${CMAKE_CURRENT_SOURCE_DIR}/../Fdo/SpatialContext
	${CMAKE_CURRENT_SOURCE_DIR}/../Gdbi
	${CMAKE_CURRENT_SOURCE_DIR}/../LongTransactionManager
	${CMAKE_CURRENT_SOURCE_DIR}/../LockManager
	${UNMANAGED_DIR}
	${UNMANAGED_INCLUDE_DIR}
	)

set( LTManager_STAT_SRCS
   Disposable.cpp
   LongTransactionEnvInfo.cpp
   LongTransactionConflict.cpp
   LongTransactionInfo.cpp
   LongTransactionConflictInfo.cpp
   LongTransactionInfos.cpp
   LongTransactionConflictInfos.cpp
   stdafx.cpp
   LongTransactionConflicts.cpp
)

add_library( LTManager STATIC ${LTManager_STAT_SRCS} )
