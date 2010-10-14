include_directories( BEFORE ${CMAKE_CURRENT_SOURCE_DIR} )

set( FdoRdbms_Filter_STAT_SRCS
   FdoRdbmsFilterProcessor.cpp
   FdoRdbmsFilterUtil.cpp
   FdoRdbmsFunctionIsValid.cpp
)

add_library( FdoRdbms_Filter STATIC ${FdoRdbms_Filter_STAT_SRCS} )

