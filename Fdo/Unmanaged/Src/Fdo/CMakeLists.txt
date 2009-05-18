include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Expression
    ${CMAKE_CURRENT_SOURCE_DIR}/Commands
    ${CMAKE_CURRENT_SOURCE_DIR}/Commands/SpatialContext
	${CMAKE_CURRENT_SOURCE_DIR}/Commands/Schema
	${CMAKE_CURRENT_SOURCE_DIR}/Parse
	${CMAKE_CURRENT_SOURCE_DIR}/Xml
	${CMAKE_CURRENT_SOURCE_DIR}/Schema
	${CMAKE_CURRENT_SOURCE_DIR}/ClientServices
	${CMAKE_CURRENT_SOURCE_DIR}/Connections
	${CMAKE_CURRENT_SOURCE_DIR}/Connections/Capabilities
	${CMAKE_CURRENT_SOURCE_DIR}/Raster
	${CMAKE_CURRENT_SOURCE_DIR}/Filter
	${UNMANAGED_INCLUDE_DIR}
	)

# Generate header
get_target_property( FILETOHDR_EXE FileToHdr LOCATION )

set( schemafile "${CMAKE_CURRENT_SOURCE_DIR}/Schema/SchemaFromInternal.h" )

add_custom_command( 
	OUTPUT "${schemafile}"
	COMMAND ${FILETOHDR_EXE} ARGS ${CMAKE_CURRENT_SOURCE_DIR}/Schema/SchemaFromInternal.xslt ${schemafile} schemaFromInternal
	COMMENT "Generating Schema/SchemaFromInternal.h ..." 
	)

add_custom_target( SchemaFromInternal ALL DEPENDS FileToHdr Schema/SchemaFromInternal.h )
