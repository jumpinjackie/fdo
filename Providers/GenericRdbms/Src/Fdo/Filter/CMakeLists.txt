include_directories(
	BEFORE
	${UNMANAGED_INCLUDE_DIR}
	${UNMANAGED_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../GenericRdbms/Inc
	${UTILITIES_COMMON_INCLUDE_DIR}
	${UTILITIES_SCHEMAMGR_INCLUDE_DIR}/Sm/Lp
	${UTILITIES_SCHEMAMGR_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Inc
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Inc/Rdbi
	${CMAKE_CURRENT_SOURCE_DIR}/../../
	${CMAKE_CURRENT_SOURCE_DIR}/../../../
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Inc
	${CMAKE_CURRENT_SOURCE_DIR}/.././Other
	${CMAKE_CURRENT_SOURCE_DIR}/.././Capability
	${CMAKE_CURRENT_SOURCE_DIR}/.././Connection
	${CMAKE_CURRENT_SOURCE_DIR}/.././Custom
	${CMAKE_CURRENT_SOURCE_DIR}/.././FeatureCommands
	${CMAKE_CURRENT_SOURCE_DIR}/.././Filter
	${CMAKE_CURRENT_SOURCE_DIR}/.././Lock
	${CMAKE_CURRENT_SOURCE_DIR}/.././Lt
	${CMAKE_CURRENT_SOURCE_DIR}/.././Schema
	${CMAKE_CURRENT_SOURCE_DIR}/.././SpatialContext
	${UNMANAGED_DIR}/src/geometry
	${CMAKE_CURRENT_SOURCE_DIR}/../../Gdbi
	${CMAKE_CURRENT_SOURCE_DIR}/../../LongTransactionManager
	${CMAKE_CURRENT_SOURCE_DIR}/../../LockManager
	${CMAKE_CURRENT_SOURCE_DIR}/../
	${CMAKE_CURRENT_SOURCE_DIR}
	)

set( FdoRdbms_Filter_STAT_SRCS
   FdoRdbmsFilterProcessor.cpp
   FdoRdbmsFilterUtil.cpp
)

add_library( FdoRdbms_Filter STATIC ${FdoRdbms_Filter_STAT_SRCS} )

