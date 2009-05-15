include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

set( SmPhRb_STAT_SRCS
   Cache.cpp
   Column.cpp
   Table.cpp
)

add_library( SmPhRb STATIC ${SmPhRb_STAT_SRCS} )
