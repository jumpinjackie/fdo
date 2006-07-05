AUTOMAKE_OPTIONS = foreign

noinst_LTLIBRARIES = libFDONLS.la

EXTRA_DIST = \
  CacheAccess.cpp \
  CacheAccess.h \
  ftoa.c \
  func.h \
  Inc/ctype.h \
  Inc/datum.h \
  Inc/debugext.h \
  Inc/langinfo.h \
  Inc/limits.h \
  Inc/mio.h \
  Inc/msg.h \
  Inc/Nls/coremsg.h \
  Inc/Nls/nls_msg.h \
  Inc/nls.h \
  Inc/nl_types.h \
  Inc/os.h \
  Inc/stdlib.h \
  Inc/string.h \
  Inc/Sys/stat.h \
  Inc/tfn.h \
  Inc/time.h \
  Inc/types.h \
  Inc/ut.h

libFDONLS_la_SOURCES = \
  msg_get.c \
  cat_open.c \
  locale_s.c

INCLUDES = \
  -I../../Inc

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX      

