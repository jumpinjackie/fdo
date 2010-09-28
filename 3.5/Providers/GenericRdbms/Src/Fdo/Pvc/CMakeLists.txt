include_directories( BEFORE ${CMAKE_CURRENT_SOURCE_DIR} )

set( FdoRdbms_Pvc_STAT_SRCS
   FdoRdbmsPvcHandler.cpp
   FdoRdbmsPvcInsertHandler.cpp
   FdoRdbmsPvcProcessor.cpp
   FdoRdbmsPvcOperation.cpp
   FdoRdbmsPvcUpdateHandler.cpp
)

add_library( FdoRdbms_Pvc STATIC ${FdoRdbms_Pvc_STAT_SRCS} )
