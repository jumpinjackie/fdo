include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../../Inc
	${CMAKE_CURRENT_SOURCE_DIR}/../..
	${CMAKE_CURRENT_SOURCE_DIR}/$(FDOODBC)/include
	${CMAKE_CURRENT_SOURCE_DIR}/$(FDO)/Unmanaged/Inc
	)

add_definitions( -DFULLPROTO -D__USE_GNU -DLINUX )

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

add_library( FdoODBCDriver STATIC ${FdoODBCDriver_STAT_SRCS} )




#original Makefile.am contents follow:

###
### Copyright (C) 2004-2006  Autodesk, Inc.
### 
### This library is free software; you can redistribute it and/or
### modify it under the terms of version 2.1 of the GNU Lesser
### General Public License as published by the Free Software Foundation.
### 
### This library is distributed in the hope that it will be useful,
### but WITHOUT ANY WARRANTY; without even the implied warranty of
### MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
### Lesser General Public License for more details.
### 
### You should have received a copy of the GNU Lesser General Public
### License along with this library; if not, write to the Free Software
### Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
###
#
#AUTOMAKE_OPTIONS = foreign
#
#EXTRA_DIST = odbcdr.vcproj
#
#noinst_LTLIBRARIES = libFdoODBCDriver.la
#
#libFdoODBCDriver_la_SOURCES = \
#autocommit_.c \
#bind.c \
#close_cursor.c \
#col_.c \
#commit.c \
#connect.c \
#crt_cursor.c \
#define.c \
#desc_bind.c \
#desc_slct.c \
#disconnect.c \
#est_cursor.c \
#execute.c \
#execute_direct.c \
#fetch.c \
#fre_cursor.c \
#geom.c \
#get_con_var.c \
#get_curr_conn.c \
#get_cursor.c \
#get_gen_id.c \
#get_msg.c \
#get_o_t_s.c \
#init.c \
#null.c \
#objects_.c \
#pkeys_.c \
#rdbi_init.c \
#run_sql.c \
#set_schema.c \
#set_sess_env.c \
#show_.c \
#sql.c \
#stores_.c \
#switch.c \
#term.c \
#users_.c \
#usr_exists.c \
#vndr_info.c \
#vndr_name.c \
#xlt_status.c
#
#noinst_HEADERS = $(libFdoODBCDriver_la_SOURCES) \
#  constants.h \
#  context.h \
#  init.h \
#  proto_p.h \
#  structs.h
#
#INCLUDES = \
#  -I../../Inc \
#  -I../.. \
#  -I$(FDOODBC)/include \
#  -I$(FDO)/Unmanaged/Inc
#
#libFdoODBCDriver_la_LDFLAGS =
#
#CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX
#
###To be consistent with internal build, force the compiler to be g++. 
#CC = g++
#
