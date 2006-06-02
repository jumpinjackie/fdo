AUTOMAKE_OPTIONS = foreign

EXTRA_DIST = Gdbi.vcproj ReadMe.txt

noinst_LTLIBRARIES = libGdbi.la

libGdbi_la_SOURCES = \
  GdbiCommands.cpp \
  GdbiConnection.cpp \
  GdbiException.cpp \
  GdbiQueryResult.cpp \
  GdbiStatement.cpp \
  stdafx.cpp

noinst_HEADERS = $(libGdbi_la_SOURCES) \
  GdbiCommands.h \
  GdbiConnection.h \
  GdbiException.h \
  GdbiQueryResult.h \
  GdbiStatement.h \
  GdbiTypes.h \
  stdafx.h

INCLUDES = \
  -I../../ \
  -I$(FDO)/Unmanaged \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDOUTILITIES)/Common/Inc

libGdbi_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

