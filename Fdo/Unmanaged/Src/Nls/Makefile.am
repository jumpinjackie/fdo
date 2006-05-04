AUTOMAKE_OPTIONS = foreign

noinst_LTLIBRARIES = libFDONLS.la

libFDONLS_la_SOURCES = \
  msg_get.c \
  cat_open.c \
  locale_s.c

INCLUDES = \
  -I../../Inc

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX      

