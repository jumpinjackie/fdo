include_directories( 
	BEFORE
	${MYSQL_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	)

set( mysqldr_STAT_SRCS
   bind.cpp
   commit.cpp
   connect.cpp
   define.cpp
   desc_bind.cpp
   desc_slct.cpp
   disconnect.cpp
   est_cursor.cpp
   execute.cpp
   fetch.cpp
   fre_cursor.cpp
   geom_srid.cpp
   get_gen_id.cpp
   get_msg.cpp
   init.cpp
   nls.cpp
   null.cpp
   rdbi_init.cpp
   run_sql.cpp
   set_schema.cpp
   sql.cpp
   stdafx.cpp
   term.cpp
   type.cpp
   vndr_info.cpp
   vndr_name.cpp
   xlt_status.cpp
   Geometry.cpp
)

set_source_files_properties( ${mysqldr_STAT_SRCS} PROPERTIES LANGUAGE CXX )

add_library( mysqldr STATIC ${mysqldr_STAT_SRCS} )

