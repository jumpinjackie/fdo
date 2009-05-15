include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

set( SmPhCfg_STAT_SRCS
   ClassReader.cpp
   PropertyReader.cpp
   SchemaReader.cpp
)

add_library( SmPhCfg STATIC ${SmPhCfg_STAT_SRCS} )
