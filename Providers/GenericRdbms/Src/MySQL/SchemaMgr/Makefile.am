AUTOMAKE_OPTIONS = foreign

SUBDIRS = Lp Ov Ph

DIST_SUBDIRS = Lp Ov Ph

EXTRA_DIST = MySqlSchemaMgr.vcproj MySqlOverrides.vcproj MySqlOverrides.rc SchemaMgr.h resource.h SchemaMgr.cpp

noinst_LTLIBRARIES = libFdoRdbmsMySQL_SchemaMgr.la

libFdoRdbmsMySQL_SchemaMgr_la_SOURCES = \
  SchemaManager.cpp \
  stdafx.cpp

noinst_HEADERS = $(libFdoRdbmsMySQL_SchemaMgr_la_SOURCES) \
  SchemaManager.h \
  stdafx.h

INCLUDES = \
  -I../ \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged \
  -I../../../../GenericRdbms/Inc \
  -I$(FDOUTILITIES)/Common/Inc \
  -I$(FDOUTILITIES)/SchemaMgr/Inc/Sm/Lp \
  -I$(FDOUTILITIES)/SchemaMgr/Inc \
  -I../../../Inc \
  -I../../../Inc/Rdbi \
  -I../../ \
  -I../../../ \
  -I../../../Inc \
  -I../../../Src/Fdo/Other \
  -I../../../Src/Fdo/Capability \
  -I../../../Src/Fdo/Connection \
  -I../../../Src/Fdo/Custom \
  -I../../../Src/Fdo/FeatureCommands \
  -I../../../Src/Fdo/Filter \
  -I../../../Src/Fdo/Lock \
  -I../../../Src/Fdo/Lt \
  -I../../../Src/Fdo/Schema \
  -I../../../Src/Fdo/SpatialContext \
  -I$(FDO)/Unmanaged \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged/Src/Geometry \
  -I$(FDOUTILITIES)/SchemaMgr/Inc \
  -I$(FDOUTILITIES)/Common/Inc \
  -I../../Gdbi \
  -I../../LongTransactionManager \
  -I../../LockManager

libFdoRdbmsMySQL_SchemaMgr_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

