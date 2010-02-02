AUTOMAKE_OPTIONS = foreign

EXTRA_DIST = PostGisDriver.vcproj

noinst_LTLIBRARIES = libpostgisdr.la

libpostgisdr_la_SOURCES = \
  bind.c \
  break.c \
  commit.c \
  connect.c \
  conn_status.c \
  define.c \
  desc_bind.c \
  disconnect.c \
  est_cursor.c \
  execute.c \
  fetch.c \
  fre_cursor.c \
  geom_srid.c \
  Geometry.c \
  get_gen_id.c \
  desc_slct.c \
  pgresult_error.c \
  get_msg.c \
  init.c \
  null.c \
  pgconn_status.c \
  pgresult_clear.c \
  pgresult_status.c \
  rdbi_init.c \
  run_sql.c \
  set_err_msg.c \
  sql.c \
  stdafx.c \
  term.c \
  type.c \
  vndr_info.c \
  vndr_name.c 
  

noinst_HEADERS = $(libpostgisdr_la_SOURCES) \
  bind.h \
  break.h \
  commit.h \
  connect.h \
  conn_status.h \
  define.h \
  desc_bind.h \
  desc_slct.h  \
  digits.h \
  disconnect.h \
  est_cursor.h \
  execute.h  \
  fetch.h  \
  fre_cursor.h \
  geom_srid.h \
  Geometry.h \
  get_gen_id.h \
  get_msg.h \
  init.h \
  local.h \
  null.h \
  pgconn_status.h \
  pgresult_clear.h \
  pgresult_error.h \
  pgresult_status.h \
  rdbi_init.h \
  run_sql.h \
  set_err_msg.h \
  sql.h \
  stdafx.h \
  term.h \
  type.h \
  vndr_info.h \
  vndr_name.h

INCLUDES = \
  -I../../.. \
  -L$(FDOPOSTGRESQL)/include \
  -I$(FDOTHIRDPARTY)/pgsql/include \
  -I$(FDOUTILITIES)/Common/Inc \
  -I$(FDO)/Unmanaged/Inc

libpostgisdr_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

##To be consistent with internal build, force the compiler to be g++. 
CC = g++

