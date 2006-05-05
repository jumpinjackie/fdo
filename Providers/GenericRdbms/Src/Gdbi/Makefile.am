AUTOMAKE_OPTIONS = foreign

noinst_LTLIBRARIES = libGdbi.la

libGdbi_la_SOURCES = \
  GdbiCommands.cpp \
  GdbiConnection.cpp \
  GdbiException.cpp \
  GdbiQueryResult.cpp \
  GdbiStatement.cpp \
  stdafx.cpp

INCLUDES = \
  -I../../ \
  -I$(FDO)/Unmanaged \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDOUTILITIES)/Common/Inc

libGdbi_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

