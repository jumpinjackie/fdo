include_directories( BEFORE	${CMAKE_CURRENT_SOURCE_DIR} )

set( genericrdbms_util_STAT_SRCS
   da_.c
   dtoa.c
   dtoa_p.c
   itoa.c
   thread.c
   trace.c
)

set_source_files_properties( ${genericrdbms_util_STAT_SRCS} PROPERTIES LANGUAGE CXX )

add_library( genericrdbms_util STATIC ${genericrdbms_util_STAT_SRCS} )
