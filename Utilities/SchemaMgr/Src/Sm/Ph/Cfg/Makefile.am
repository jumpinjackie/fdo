AUTOMAKE_OPTIONS = foreign

noinst_LTLIBRARIES = libSmPhCfg.la

libSmPhCfg_la_SOURCES = \
  ClassReader.cpp \
  PropertyReader.cpp \
  SchemaReader.cpp


INCLUDES = \
  -I.. \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged \
  -I$(FDOUTILITIES)/Common/Inc \
  -I../../../../Inc

libSmPhCfg_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

