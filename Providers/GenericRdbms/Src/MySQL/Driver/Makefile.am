AUTOMAKE_OPTIONS = foreign

noinst_LTLIBRARIES = libmysqldr.la

libmysqldr_la_SOURCES = \
  bind.c \
  commit.c \
  connect.c \
  define.c \
  desc_bind.c \
  desc_slct.c \
  disconnect.c \
  est_cursor.c \
  execute.c \
  fetch.c \
  fre_cursor.c \
  get_gen_id.c \
  get_msg.c \
  init.c \
  null.c \
  rdbi_init.c \
  run_sql.c \
  set_schema.c \
  sql.c \
  stdafx.c \
  term.c \
  type.c \
  vndr_info.c \
  vndr_name.c \
  xlt_status.c \
  Geometry.c

INCLUDES = \
  -I../../.. \
  -I$(FDOMYSQL)/include \
  -I$(FDO)/Unmanaged/Inc

libmysqldr_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

##To be consistent with internal build, force the compiler to be g++. 
CC = g++

