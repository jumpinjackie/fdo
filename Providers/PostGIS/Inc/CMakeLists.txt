INSTALL(
	DIRECTORY PostGIS
        DESTINATION include
        PATTERN *.h
		PATTERN .svn EXCLUDE
        )

