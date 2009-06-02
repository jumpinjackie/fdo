include_directories( 
	${CMAKE_CURRENT_SOURCE_DIR} 
	${CMAKE_CURRENT_BINARY_DIR} 
	)

set( MKCATDEFS_SRCS
	args.c 
	msf.c 
	msg.c 
	main.c 
	)

if (CMAKE_SYSTEM_NAME MATCHES Linux)
	# For Linux based new gcc/glib we need set posix as standard or we don't have L_cuserid available
	add_definitions( -posix )
endif (CMAKE_SYSTEM_NAME MATCHES Linux)

add_executable( mkcatdefs ${MKCATDEFS_SRCS} )

#install(TARGETS mkcatdefs RUNTIME DESTINATION bin )
