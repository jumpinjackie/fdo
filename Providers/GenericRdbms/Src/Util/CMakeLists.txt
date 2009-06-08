include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${UNMANAGED_INCLUDE_DIR}
	)

add_definitions( -pthread )

set( util_STAT_SRCS
   da_.c
   dtoa.c
   dtoa_p.c
   itoa.c
   thread.c
   trace.c
)

add_library( rdbmsutil STATIC ${util_STAT_SRCS} )
