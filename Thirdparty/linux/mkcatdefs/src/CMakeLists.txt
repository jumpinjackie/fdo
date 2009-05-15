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

add_executable( mkcatdefs ${MKCATDEFS_SRCS} )

#install(TARGETS mkcatdefs RUNTIME DESTINATION bin )
