include_directories( BEFORE	${CMAKE_CURRENT_SOURCE_DIR} )

set( genericrdbms_util_STAT_SRCS
   da_.cpp
   dtoa.cpp
   dtoa_p.cpp
   itoa.cpp
   thread.cpp
   trace.cpp
)

set_source_files_properties( ${genericrdbms_util_STAT_SRCS} PROPERTIES LANGUAGE CXX )

add_library( genericrdbms_util STATIC ${genericrdbms_util_STAT_SRCS} )
