include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${UNMANAGED_INCLUDE_DIR}
	${FDO}/Unmanaged
	${UTILITIES_COMMON_INCLUDE_DIR}
	)

set( SmPhCfg_SRCS
	ClassReader.cpp
	PropertyReader.cpp
	SchemaReader.cpp
)

add_library( SmPhCfg STATIC ${SmPhCfg_SRCS} )

