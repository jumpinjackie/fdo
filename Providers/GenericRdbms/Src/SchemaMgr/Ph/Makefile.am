AUTOMAKE_OPTIONS = foreign

SUBDIRS = Cfg Rd

DIST_SUBDIRS = Cfg Rd

noinst_LTLIBRARIES = libSmPhGrd.la

libSmPhGrd_la_SOURCES = \
  ClassWriter.cpp \
  CommandWriter.cpp \
  Fkey.cpp \
  Index.cpp \
  Mgr.cpp \
  Owner.cpp \
  SpatialContextGroupWriter.cpp \
  SpatialContextWriter.cpp \
  Table.cpp \
  View.cpp

noinst_HEADERS = $(libSmPhGrd_la_SOURCES) \
  ClassWriter.h \
  CommandWriter.h \
  Fkey.h \
  Index.h \
  Mgr.h \
  Owner.h \
  SpatialContextGroupWriter.h \
  SpatialContextWriter.h \
  Table.h \
  View.h

INCLUDES = \
  -I.. \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged \
  -I$(FDOUTILITIES)/SchemaMgr/Inc \
  -I../../.. \
  -I../../../Inc \
  -I$(FDOUTILITIES)/Common/Inc

libSmPhGrd_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

