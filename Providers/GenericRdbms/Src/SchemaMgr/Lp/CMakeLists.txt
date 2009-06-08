include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${UTILITIES_SCHEMAMGR_INCLUDE_DIR}
	${UTILITIES_COMMON_INCLUDE_DIR}
	)

set( SmLpGrd_STAT_SRCS
   ClassDefinition.cpp
   DataPropertyDefinition.cpp
   GeometricPropertyDefinition.cpp
   ObjectPropertyClass.cpp
   ObjectPropertyDefinition.cpp
   PropertyMappingConcrete.cpp
   PropertyMappingSingle.cpp
   Schema.cpp
)

add_library( SmLpGrd STATIC ${SmLpGrd_STAT_SRCS} )
