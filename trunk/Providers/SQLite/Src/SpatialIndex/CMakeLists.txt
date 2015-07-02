include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${UNMANAGED_INCLUDE_DIR}
	${UTILITIES_COMMON_INCLUDE_DIR}
	)

set( SQLiteSpatialIndex_SRCS
	SltGeomUtils.cpp  
	stdafx.cpp
)

if( UNIX )
	set( SQLiteSpatialIndex_Platform_SRCS SpatialIndex.cpp )
	add_definitions(-msse -msse2 -mmmx)
else( UNIX )
	set( SQLiteSpatialIndex_Platform_SRCS DiskSpatialIndex.cpp MappedFile.cpp )
endif( UNIX )


add_library( SQLiteSpatialIndex STATIC 
	${SQLiteSpatialIndex_SRCS}  
	${SQLiteSpatialIndex_Platform_SRCS}
	)

