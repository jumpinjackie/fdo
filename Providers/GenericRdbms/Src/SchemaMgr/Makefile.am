AUTOMAKE_OPTIONS = foreign

SUBDIRS = Lp Ov Ph

DIST_SUBDIRS = Lp Ov Ph

EXTRA_DIST = RdbmsOverrides.vcproj GrdSchemaMgr.vcproj SchemaMgr.h SchemaMgr.cpp resource.h RdbmsOverrides.rc

noinst_LTLIBRARIES = libSmGrd.la

libSmGrd_la_SOURCES = \
  SchemaManager.cpp \
  stdafx.cpp

noinst_HEADERS = $(libSmGrd_la_SOURCES) \
  SchemaManager.h \
  stdafx.h

INCLUDES = \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged \
  -I$(FDOUTILITIES)/SchemaMgr/Inc \
  -I../.. \
  -I../../Inc \
  -I$(FDOUTILITIES)/Common/Inc

libSmGrd_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX
                             
