AUTOMAKE_OPTIONS = foreign

SUBDIRS = Ov Lp Ph

noinst_LTLIBRARIES = libSchemaMgr.la

libSchemaMgr_la_SOURCES = \
  Base.cpp \
  Disposable.cpp \
  Error.cpp \
  SchemaElement.cpp \
  SchemaManager.cpp \
  stdafx.cpp

INCLUDES = \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged \
  -I$(FDOUTILITIES)/Common/Inc \
  -I../../Inc

libSchemaMgr_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

