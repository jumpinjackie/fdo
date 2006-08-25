AUTOMAKE_OPTIONS = foreign

EXTRA_DIST = odbcdr.vcproj

noinst_LTLIBRARIES = libFdoODBCDriver.la

libFdoODBCDriver_la_SOURCES = \
autocommit_.c \
bind.c \
close_cursor.c \
col_.c \
commit.c \
connect.c \
crt_cursor.c \
define.c \
desc_bind.c \
desc_slct.c \
disconnect.c \
est_cursor.c \
execute.c \
execute_direct.c \
fetch.c \
fre_cursor.c \
geom.c \
get_con_var.c \
get_curr_conn.c \
get_cursor.c \
get_gen_id.c \
get_msg.c \
get_o_t_s.c \
init.c \
null.c \
objects_.c \
pkeys_.c \
rdbi_init.c \
run_sql.c \
set_schema.c \
show_.c \
sql.c \
stores_.c \
switch.c \
term.c \
users_.c \
usr_exists.c \
vndr_info.c \
vndr_name.c \
xlt_status.c

noinst_HEADERS = $(libFdoODBCDriver_la_SOURCES) \
  constants.h \
  context.h \
  init.h \
  proto_p.h \
  structs.h

INCLUDES = \
  -I../../Inc \
  -I../.. \
  -I$(FDOODBC)/include \
  -I$(FDO)/Unmanaged/Inc

libFdoODBCDriver_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

##To be consistent with internal build, force the compiler to be g++. 
CC = g++

