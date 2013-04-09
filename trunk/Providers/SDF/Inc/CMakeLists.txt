include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	)


INSTALL(
	DIRECTORY SDF
        DESTINATION include
        PATTERN *.h
		PATTERN .svn EXCLUDE
        )

