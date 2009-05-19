include_directories( 
	${CMAKE_CURRENT_SOURCE_DIR}/Unmanaged/Inc
	${CMAKE_CURRENT_BINARY_DIR}/Unmanaged/Inc
	)

add_subdirectory( Unmanaged ) 
add_subdirectory( Install ) 
add_subdirectory( Docs )
if( CPPUNIT_FOUND )
	add_subdirectory( UnitTest )
endif( CPPUNIT_FOUND )
