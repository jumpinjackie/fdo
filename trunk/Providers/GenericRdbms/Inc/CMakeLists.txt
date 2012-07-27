include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	)

INSTALL(
	DIRECTORY Rdbms
        DESTINATION include
        PATTERN *.h
		PATTERN .svn EXCLUDE
        )
