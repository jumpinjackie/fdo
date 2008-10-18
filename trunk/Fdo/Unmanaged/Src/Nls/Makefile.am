##
## Copyright (C) 2004-2006  Autodesk, Inc.
## 
## This library is free software; you can redistribute it and/or
## modify it under the terms of version 2.1 of the GNU Lesser
## General Public License as published by the Free Software Foundation.
## 
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## Lesser General Public License for more details.
## 
## You should have received a copy of the GNU Lesser General Public
## License along with this library; if not, write to the Free Software
## Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
##

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

