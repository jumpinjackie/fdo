include_directories(
	BEFORE
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${UNMANAGED_INCLUDE_DIR}
	${UNMANAGED_DIR}
	${UTILITIES_SCHEMAMGR_INCLUDE_DIR}
	${UTILITIES_COMMON_INCLUDE_DIR}
	)

set( SmPhCfgGrd_STAT_SRCS
   ClassReader.cpp
   PropertyReader.cpp
   SchemaReader.cpp
)

add_library( SmPhCfgGrd STATIC ${SmPhCfgGrd_STAT_SRCS} )
add_dependencies( SmPhCfgGrd fdordbmsmsg )
