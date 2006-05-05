AUTOMAKE_OPTIONS = foreign

noinst_LTLIBRARIES = libSmPhCfgGrd.la

libSmPhCfgGrd_la_SOURCES = \
  ClassReader.cpp \
  PropertyReader.cpp \
  SchemaReader.cpp

INCLUDES = \
  -I../.. \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged \
  -I$(FDOUTILITIES)/SchemaMgr/Inc \
  -I../../../.. \
  -I../../../../Inc \
  -I$(FDOUTILITIES)/Common/Inc

libSmPhCfgGrd_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

