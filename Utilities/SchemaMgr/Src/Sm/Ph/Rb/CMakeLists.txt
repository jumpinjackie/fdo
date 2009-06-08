include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${UNMANAGED_INCLUDE_DIR}
	${FDO}/Unmanaged
	${UTILITIES_COMMON_INCLUDE_DIR}
	)

set( SmPhRb_SRCS
	Cache.cpp
	Column.cpp
	Table.cpp
)

add_library( SmPhRb STATIC ${SmPhRb_SRCS} )
