AUTOMAKE_OPTIONS = foreign

SUBDIRS = Lp Ov Ph

noinst_LTLIBRARIES = libSmGrd.la

libSmGrd_la_SOURCES = \
  SchemaManager.cpp \
  stdafx.cpp

INCLUDES = \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged \
  -I$(FDOUTILITIES)/SchemaMgr/Inc \
  -I../.. \
  -I../../Inc \
  -I$(FDOUTILITIES)/Common/Inc

libSmGrd_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX
                             
