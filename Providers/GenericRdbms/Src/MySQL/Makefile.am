AUTOMAKE_OPTIONS = foreign

SUBDIRS = Driver Fdo SchemaMgr

noinst_LTLIBRARIES = libFdoRdbmsMySQL.la

libFdoRdbmsMySQL_la_SOURCES  = \
  MySql.cpp \
  stdafx.cpp

libFdoRdbmsMySQL_la_LDFLAGS = \
  Fdo/libFdoRdbmsMySQL_Fdo.la \
  SchemaMgr/libFdoRdbmsMySQL_SchemaMgr.la \
  SchemaMgr/Lp/libFdoRdbmsMySQL_SchemaMgr_Lp.la \
  SchemaMgr/Ov/libFdoRdbmsMySQL_SchemaMgr_Ov.la \
  SchemaMgr/Ph/libFdoRdbmsMySQL_SchemaMgr_Ph.la

INCLUDES = \
  -I$(FDO)/Unmanaged/Inc \
  -I../../Inc/Rdbi \
  -I../ \
  -I../../ \
  -I../../Inc \
  -I../Fdo/Connection \
  -I../Fdo/Other \
  -I../Fdo/FeatureCommands \
  -I../Fdo/Custom \
  -I../Fdo/Schema \
  -I../Fdo/Lt \
  -I../Fdo/Lock  \
  -I../Fdo/SpatialContext  \
  -I$(FDO)/Unmanaged \
  -I$(FDOUTILITIES)/SchemaMgr/Inc \
  -I$(FDOUTILITIES)/Common/Inc \
  -I../Gdbi \
  -I../LongTransactionManager  \
  -I../GenericLongTransactionManager \
  -I../LockManager

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

