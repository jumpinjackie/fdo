AUTOMAKE_OPTIONS = foreign

SUBDIRS = Cfg Rd

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

