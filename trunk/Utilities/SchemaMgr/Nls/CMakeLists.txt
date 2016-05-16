file( MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../nls/linux/en_US )
file( MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../Inc/Sm )

set( messagefile "${CMAKE_CURRENT_BINARY_DIR}/../Inc/Sm/SmMessage.h" )

get_target_property( MCTOMSF_EXE McToMsf LOCATION )
get_target_property( MKCATDEFS_EXE mkcatdefs LOCATION )

add_custom_command(
                OUTPUT "${messagefile}"
				COMMAND ${MCTOMSF_EXE} ARGS ${CMAKE_CURRENT_SOURCE_DIR}/SmMessage.mc ${CMAKE_CURRENT_BINARY_DIR}/SmMessage.msf \"*\" SmMessage
				COMMAND ${MKCATDEFS_EXE} ARGS SmMessage SmMessage.msf > SmMessage.tmp
				COMMAND gencat ARGS  ${CMAKE_CURRENT_BINARY_DIR}/../../nls/linux/en_US/SmMessage.cat SmMessage.tmp
				COMMAND sed ARGS -i "\"/#include/d\""  SmMessage_msg.h
				COMMAND sed ARGS -i "/MF_/s/\"\(.*\).cat\"/1_cat/"  SmMessage_msg.h
				COMMAND cp ARGS -f SmMessage_msg.h ${messagefile}
				COMMENT "Generating SmMessage.h ..."
        )

add_custom_target( SMMessage ALL DEPENDS ${messagefile} McToMsf mkcatdefs )
	
install( FILES ${CMAKE_CURRENT_BINARY_DIR}/../../nls/linux/en_US/SmMessage.cat DESTINATION nls )
install( FILES ${CMAKE_CURRENT_BINARY_DIR}/../Inc/Sm/SmMessage.h DESTINATION include )

