include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${ODBC_INCLUDE_DIR}
	)

set( FdoODBCDriver_STAT_SRCS
   autocommit_.c
   bind.c
   close_cursor.c
   col_.c
   commit.c
   connect.c
   crt_cursor.c
   define.c
   desc_bind.c
   desc_slct.c
   disconnect.c
   est_cursor.c
   execute.c
   execute_direct.c
   fetch.c
   fre_cursor.c
   geom.c
   get_con_var.c
   get_curr_conn.c
   get_cursor.c
   get_gen_id.c
   get_msg.c
   get_o_t_s.c
   init.c
   null.c
   objects_.c
   pkeys_.c
   rdbi_init.c
   run_sql.c
   set_schema.c
   set_sess_env.c
   show_.c
   sql.c
   stores_.c
   switch.c
   term.c
   users_.c
   usr_exists.c
   vndr_info.c
   vndr_name.c
   xlt_status.c
)

set_source_files_properties( ${FdoODBCDriver_STAT_SRCS} PROPERTIES LANGUAGE CXX )

add_library( FdoODBCDriver STATIC ${FdoODBCDriver_STAT_SRCS} )
