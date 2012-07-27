include_directories( 
	BEFORE
	${postgis_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	)

set( postgisdr_STAT_SRCS
  bind.c
  break.c
  commit.c
  connect.c
  conn_status.c
  define.c
  desc_bind.c
  disconnect.c
  est_cursor.c
  execute.c
  fetch.c
  fre_cursor.c
  geom_srid.c
  Geometry.c
  get_msg.c
  get_next_seq.c
  init.c
  null.c
  pgconn_status.c
  pgresult_clear.c
  pgresult_status.c
  rdbi_init.c
  run_sql.c
  set_err_msg.c
  set_database.c
  sql.c
  stdafx.c
  term.c
  type.c
  vndr_info.c
  vndr_name.c 
)

set_source_files_properties( ${postgisdr_STAT_SRCS} PROPERTIES LANGUAGE CXX )

add_library( postgisdr STATIC ${postgisdr_STAT_SRCS} )
add_dependencies( postgisdr fdordbmsmsg )

