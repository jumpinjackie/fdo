include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${UNMANAGED_INCLUDE_DIR}
	${UNMANAGED_DIR}
	${UTILITIES_SCHEMAMGR_INCLUDE_DIR}
	${UTILITIES_COMMON_INCLUDE_DIR}
	${FDO}/Unmanaged/Src/Geometry
	${CMAKE_CURRENT_SOURCE_DIR}/../Fdo/Lock
	${CMAKE_CURRENT_SOURCE_DIR}/../Fdo/Other
	${CMAKE_CURRENT_SOURCE_DIR}/../Fdo/Connection
	${CMAKE_CURRENT_SOURCE_DIR}/../Fdo/Capability
	${CMAKE_CURRENT_SOURCE_DIR}/../Fdo/Custom
	${CMAKE_CURRENT_SOURCE_DIR}/../Fdo/FeatureCommands
	${CMAKE_CURRENT_SOURCE_DIR}/../Fdo/Filter
	${CMAKE_CURRENT_SOURCE_DIR}/../Fdo/Lt
	${CMAKE_CURRENT_SOURCE_DIR}/../Fdo/Schema
	${CMAKE_CURRENT_SOURCE_DIR}/../Fdo/SpatialContext
	${CMAKE_CURRENT_SOURCE_DIR}/../Gdbi
	${CMAKE_CURRENT_SOURCE_DIR}/../LongTransactionManager
	${CMAKE_CURRENT_SOURCE_DIR}/../LockManager
	)

set( LockManager_STAT_SRCS
	Disposable.cpp
	LockConflict.cpp
	LockInfo.cpp
	LockType.cpp
	LockTypes.cpp
	OwnerInfo.cpp
	stdafx.cpp
	TableInfo.cpp
)

add_library( LockManager STATIC ${LockManager_STAT_SRCS} )
