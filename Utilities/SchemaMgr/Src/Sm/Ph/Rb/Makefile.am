AUTOMAKE_OPTIONS = foreign

noinst_LTLIBRARIES = libSmPhRb.la

libSmPhRb_la_SOURCES = \
  Cache.cpp \
  Column.cpp \
  Table.cpp


INCLUDES = \
      -I.. \
      -I$(FDO)/Unmanaged/Inc \
      -I$(FDO)/Unmanaged \
      -I$(FDOUTILITIES)/Common/Inc \
      -I../../../../Inc

libSmPhRb_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

