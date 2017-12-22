file( MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../nls/linux/en_US )
file( MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../Inc )

set( messagefile "${CMAKE_CURRENT_BINARY_DIR}/../../Inc/FdoMessage.h" )

get_target_property( MCTOMSF_EXE McToMsf LOCATION )
get_target_property( MKCATDEFS_EXE mkcatdefs LOCATION )

add_custom_command(
                OUTPUT "${messagefile}"
                COMMAND ${MCTOMSF_EXE} ARGS ${CMAKE_CURRENT_SOURCE_DIR}/FDOMessage.mc ${CMAKE_CURRENT_BINARY_DIR}/FDOMessage.msf \"*\" FDO
				COMMAND ${MKCATDEFS_EXE} ARGS FDOMessage FDOMessage.msf > FDOMessage.tmp
                COMMAND gencat ARGS  ${CMAKE_CURRENT_BINARY_DIR}/../../nls/linux/en_US/FDOMessage.cat FDOMessage.tmp
                COMMAND sed ARGS -i "\"/#include/d\""  FDOMessage_msg.h
				COMMAND sed ARGS -i "/MF_/s/\"\(.*\).cat\"/1_cat/"  FDOMessage_msg.h
				COMMAND cp ARGS -f FDOMessage_msg.h ${messagefile}
				COMMENT "Generating FdoMessage.h ..."
        )

add_custom_target( FDOMessage ALL DEPENDS ${messagefile} McToMsf mkcatdefs )
	
install( FILES ${CMAKE_CURRENT_BINARY_DIR}/../../nls/linux/en_US/FDOMessage.cat DESTINATION nls )
install( FILES ${CMAKE_CURRENT_BINARY_DIR}/../../Inc/FdoMessage.h DESTINATION include )

