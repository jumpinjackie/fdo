AUTOMAKE_OPTIONS = foreign

EXTRA_DIST = Rdbi.vcproj

noinst_LTLIBRARIES = librdbi.la

librdbi_la_SOURCES = \
  autocommit_.c \
  bind.c \
  break.c \
  col_.c \
  commit.c \
  connect.c \
  context.c \
  crsr.c \
  define.c \
  desc_bind.c \
  desc_slct.c \
  disconnect.c \
  end_select.c \
  est_cursor.c \
  exec_coc.c \
  execute.c \
  fetch.c \
  run_sql.c \
  fre_cursor.c \
  free_all.c \
  geom_dimens.c \
  geom_srid.c \
  get_geoms_ext.c \
  get_info.c \
  get_msg.c \
  get_size.c \
  id_len.c \
  init.c \
  lob_.c \
  msg_issue.c \
  nom_db.c \
  null.c \
  objects_.c \
  pkeys_.c \
  get_gen_id.c \
  set_schema.c \
  sql.c \
  sql_va.c \
  stores_.c \
  switch.c \
  term.c \
  tran_begin.c \
  tran_end.c \
  tran_getid.c \
  tran_rolbk.c \
  users_.c \
  usr_exists.c \
  version.c \
  vis_owner.c \
  vndr_info.c \
  vndr_name.c

noinst_HEADERS = $(librdbi_la_SOURCES) \
  global.h

INCLUDES = \
  -I../.. \
  -I$(FDO)/Unmanaged/Inc

librdbi_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

CC = g++

