include_directories( BEFORE ${CMAKE_CURRENT_SOURCE_DIR} )
add_executable( McToMsf McToMsf.cpp )

mark_as_advanced( MCTOMSF_EXE )
#install( TARGETS McToMsf RUNTIME DESTINATION bin )

