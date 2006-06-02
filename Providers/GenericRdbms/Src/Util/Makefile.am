AUTOMAKE_OPTIONS = foreign

EXTRA_DIST = util.vcproj

noinst_LTLIBRARIES = libutil.la

libutil_la_SOURCES = \
  da_.c  \
  dtoa.c \
  dtoa_p.c \
  itoa.c \
  thread.c \
  trace.c

noinst_HEADERS = $(libutil_la_SOURCES) \
  stdafx.h

INCLUDES = \
  -I../.. \
  -I$(FDO)/Unmanaged/Inc  \
  -I$(FDO)/Unmanaged

libutil_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

CC = g++

