AUTOMAKE_OPTIONS = foreign

EXTRA_DIST = MySqlDriver.vcproj

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

noinst_HEADERS = $(libmysqldr_la_SOURCES) \
  bind.h \
  commit.h \
  connect.h \
  define.h \
  desc_bind.h \
  desc_slct.h \
  disconnect.h \
  errno.h \
  est_cursor.h \
  execute.h \
  fetch.h \
  fre_cursor.h \
  Geometry.h \
  get_gen_id.h \
  get_msg.h \
  init.h \
  local.h \
  null.h \
  rdbi_init.h \
  run_sql.h \
  set_schema.h \
  sql.h \
  stdafx.h \
  term.h \
  type.h \
  vndr_info.h \
  vndr_name.h \
  xlt_status.h

INCLUDES = \
  -I../../.. \
  -I$(FDOMYSQL)/include \
  -I$(FDO)/Unmanaged/Inc

libmysqldr_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

##To be consistent with internal build, force the compiler to be g++. 
CC = g++

