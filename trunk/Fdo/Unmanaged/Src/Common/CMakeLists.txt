include_directories( BEFORE	${CMAKE_CURRENT_SOURCE_DIR}	)

# Generate headers
get_target_property( FILETOHDR_EXE FileToHdr LOCATION )

set( outfiles )
set( infiles
	Gml212/xlinks.h
	Gml212/geometry.h
	Gml212/feature.h
	)

foreach( infileName ${infiles} )
	string(REGEX REPLACE ".h\$" "" basefileName "${infileName}")
	string(REGEX REPLACE "Gml212/" "" spec "${basefileName}")
	set( outFile "${CMAKE_CURRENT_BINARY_DIR}/${infileName}")
	file( MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Gml212 )
	add_custom_command( 
		OUTPUT ${outFile} 
		COMMAND ${FILETOHDR_EXE} ARGS ${CMAKE_CURRENT_SOURCE_DIR}/${basefileName}.xsd ${infileName} ${spec}
		COMMENT "Generating ${infileName} ..." 
	)
	set( outfiles ${outfiles} "${outFile}" )
endforeach( infileName )

add_custom_target( ProcessedFiles ALL DEPENDS FileToHdr ${outfiles} )
