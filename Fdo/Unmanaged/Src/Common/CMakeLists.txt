include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	)

add_definitions( -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE )

set( FDOCommon_STAT_SRCS
   ArrayHelper.cpp
   CommonInternal.cpp
   Context.cpp
   DictionaryElement.cpp
   Exception.cpp
   Semaphore.cpp
   StringP.cpp
   StringCollection.cpp
   StringUtility.cpp
   Vector.cpp
   ConvertUTF.cpp
   utf8_.cpp
   Io/BufferStream.cpp
   Io/ByteStreamReader.cpp
   Io/MemoryStream.cpp
   Io/FileStream.cpp
   Io/TextReader.cpp
   Io/TextWriter.cpp
   Xml/Attribute.cpp
   Xml/CharDataHandler.cpp
   Xml/CopyHandler.cpp
   Xml/Reader.cpp
   Xml/ReaderXrcs.cpp
   Xml/SaxContext.cpp
   Xml/SaxHandler.cpp
   Xml/UtilXrcs.cpp
   Xml/Writer.cpp
   Xml/WriterXrcs.cpp
   Xml/Xml.cpp
   Xml/XmlException.cpp
   Xsl/Transformer.cpp
   Xsl/TransformerXalan.cpp
   Gml212/Schema.cpp
   Gml212/Gml212.cpp
)

add_library( FDOCommon STATIC ${FDOCommon_STAT_SRCS} )
add_dependencies( FDOCommon ProcessedFiles )

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
