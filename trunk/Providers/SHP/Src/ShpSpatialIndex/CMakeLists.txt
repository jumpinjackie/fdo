include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${UNMANAGED_INCLUDE_DIR}
	${UTILITIES_COMMON_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../ShpRead
	)

set( SHPSpatialIndex_SRCS
   ShpSpatialIndexFile.cpp
   ShpSpatialIndexCallBack.cpp
   ShpSpatialIndexHeader.cpp
   ShpSpatialIndexNode.cpp
   ShpSpatialIndexHandle.cpp
   ShpSpatialIndex.cpp
   stdafx.cpp
)

add_library( SHPSpatialIndex STATIC ${SHPSpatialIndex_SRCS} )
