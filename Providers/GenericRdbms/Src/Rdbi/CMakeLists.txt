include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${UNMANAGED_INCLUDE_DIR}
	)

set( rdbi_STAT_SRCS
   autocommit_.c
   bind.c
   break.c
   col_.c
   commit.c
   connect.c
   context.c
   crsr.c
   define.c
   desc_bind.c
   desc_slct.c
   disconnect.c
   end_select.c
   est_cursor.c
   exec_coc.c
   execute.c
   fetch.c
   run_sql.c
   fre_cursor.c
   free_all.c
   geom_dimens.c
   geom_srid.c
   get_geoms_ext.c
   get_info.c
   get_msg.c
   get_size.c
   id_len.c
   init.c
   lob_.c
   msg_issue.c
   null.c
   objects_.c
   pkeys_.c
   get_gen_id.c
   set_schema.c
   sql.c
   sql_va.c
   stores_.c
   switch.c
   term.c
   tran_begin.c
   tran_end.c
   tran_getid.c
   tran_rolbk.c
   users_.c
   usr_exists.c
   version.c
   vis_owner.c
   vndr_info.c
   vndr_name.c
)

set_source_files_properties( ${rdbi_STAT_SRCS} PROPERTIES LANGUAGE CXX )

add_library( rdbi STATIC ${rdbi_STAT_SRCS} )
add_dependencies( rdbi fdordbmsmsg )
