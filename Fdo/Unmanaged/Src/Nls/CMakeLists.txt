include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	)

set( FDONLS_STAT_SRCS
   msg_get.c
   cat_open.c
   locale_s.c
)

add_library( FDONLS STATIC ${FDONLS_STAT_SRCS} )
