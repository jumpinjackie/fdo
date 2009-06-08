include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${UNMANAGED_INCLUDE_DIR}
	${UNMANAGED_DIR}
	${UNMANAGED_DIR}/Src/Geometry
	${UTILITIES_COMMON_INCLUDE_DIR}
	${UTILITIES_SCHEMAMGR_INCLUDE_DIR}/Sm/Lp
	${UTILITIES_SCHEMAMGR_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Inc
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Inc/Rdbi
	${CMAKE_CURRENT_SOURCE_DIR}/../../
	${CMAKE_CURRENT_SOURCE_DIR}/../../../
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Inc
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Src/Fdo/Other
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Src/Fdo/Capability
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Src/Fdo/Connection
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Src/Fdo/Custom
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Src/Fdo/FeatureCommands
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Src/Fdo/Filter
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Src/Fdo/Lock
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Src/Fdo/Lt
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Src/Fdo/Schema
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Src/Fdo/SpatialContext
	${CMAKE_CURRENT_SOURCE_DIR}/../../Gdbi
	${CMAKE_CURRENT_SOURCE_DIR}/../../LongTransactionManager
	${CMAKE_CURRENT_SOURCE_DIR}/../../LockManager
	)

set( FdoRdbmsMySQL_Fdo_STAT_SRCS
   FdoRdbmsMySqlConnectionCapabilities.cpp
   FdoRdbmsMySqlFilterCapabilities.cpp
   FdoRdbmsMySqlFilterProcessor.cpp
   FdoRdbmsMySqlGeometryCapabilities.cpp
   FdoRdbmsMySqlConnection.cpp
   FdoRdbmsMySqlCommandCapabilities.cpp
   FdoRdbmsMySqlConnectionInfo.cpp
)

add_library( FdoRdbmsMySQL_Fdo STATIC ${FdoRdbmsMySQL_Fdo_STAT_SRCS} )
