AUTOMAKE_OPTIONS = foreign

noinst_LTLIBRARIES = libSmPhRdGrd.la

libSmPhRdGrd_la_SOURCES = \
  FieldArray.cpp \
  QueryReader.cpp \
  RowArray.cpp
  
INCLUDES = \
  -I../.. \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged \
  -I$(FDOUTILITIES)/SchemaMgr/Inc \
  -I../../../.. \
  -I../../../../Inc \
  -I$(FDOUTILITIES)/Common/Inc

libSmPhRdGrd_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

