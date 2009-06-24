include_directories( 
	BEFORE
	${MYSQL_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	)

set( mysqldr_STAT_SRCS
   bind.c
   commit.c
   connect.c
   define.c
   desc_bind.c
   desc_slct.c
   disconnect.c
   est_cursor.c
   execute.c
   fetch.c
   fre_cursor.c
   geom_srid.c
   get_gen_id.c
   get_msg.c
   init.c
   nls.c
   null.c
   rdbi_init.c
   run_sql.c
   set_schema.c
   sql.c
   stdafx.c
   term.c
   type.c
   vndr_info.c
   vndr_name.c
   xlt_status.c
   Geometry.c
)

set_source_files_properties( ${mysqldr_STAT_SRCS} PROPERTIES LANGUAGE CXX )

add_library( mysqldr STATIC ${mysqldr_STAT_SRCS} )

