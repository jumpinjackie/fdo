file( MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../nls/linux/en_US )
file( MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../Inc )

set( catalog "PostGisMessage" )
set( messagefile "${CMAKE_CURRENT_BINARY_DIR}/../../Inc/${catalog}.h" )

get_target_property( MCTOMSF_EXE McToMsf LOCATION )
get_target_property( MKCATDEFS_EXE mkcatdefs LOCATION )

add_custom_command(
                OUTPUT "${messagefile}"
				COMMAND ${MCTOMSF_EXE} ARGS ${CMAKE_CURRENT_SOURCE_DIR}/${catalog}.mc ${CMAKE_CURRENT_BINARY_DIR}/${catalog}.msf \"*\" POSTGISMESSAGE
				COMMAND ${MKCATDEFS_EXE} ARGS ${catalog} ${catalog}.msf > ${catalog}.tmp
				COMMAND gencat ARGS  ${CMAKE_CURRENT_BINARY_DIR}/../../nls/linux/en_US/${catalog}.cat ${catalog}.tmp
				COMMAND sed ARGS -i "\"/#include/d\""  ${catalog}_msg.h
				COMMAND sed ARGS -i "/MF_/s/\"\(.*\).cat\"/1_cat/"  ${catalog}_msg.h
				COMMAND cp ARGS -f ${catalog}_msg.h ${messagefile}
				COMMENT "Generating ${catalog}.h ..."
        )

add_custom_target( PostGisMessage ALL DEPENDS ${messagefile} McToMsf mkcatdefs )
	
install( FILES ${CMAKE_CURRENT_BINARY_DIR}/../../nls/linux/en_US/${catalog}.cat DESTINATION share/locale/en )

