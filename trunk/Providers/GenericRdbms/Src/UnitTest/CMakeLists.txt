include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${MYSQL_INCLUDE_DIR}
	Common
	MySql
	Odbc
	PostGis
	)

add_subdirectory( Common )
add_subdirectory( Odbc )
add_subdirectory( MySql )
add_subdirectory( PostGis )

add_definitions( -D__STDC_LIMIT_MACROS -DRDBI_STATIC )

set( UnitTestMySql_SRCS
   MySqlTestRegister.cpp
)

add_executable( UnitTestMySql ${UnitTestMySql_SRCS} )

target_link_libraries( UnitTestMySql
	xalan-c
	xalanMsg
	xerces-c
	${ZLIB_LIBRARIES}
	${MYSQL_LIBRARIES}
	)

set( UnitTestOdbc_SRCS
   OdbcTestRegister.cpp
)

add_executable( UnitTestOdbc ${UnitTestOdbc_SRCS} )

target_link_libraries( UnitTestOdbc
	)

set( UnitTestPostGis_SRCS
   PostGisTestRegister.cpp
)

add_executable( UnitTestPostGis ${UnitTestPostGis_SRCS} )

target_link_libraries( UnitTestPostGis
	)


