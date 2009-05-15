include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

set( SmOverrides_STAT_SRCS
   TableMappingTypeMapper.cpp
   GeometricColumnTypeMapper.cpp
   GeometricContentTypeMapper.cpp
   stdafx.cpp
)

add_library( SmOverrides STATIC ${SmOverrides_STAT_SRCS} )
