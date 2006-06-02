AUTOMAKE_OPTIONS = foreign

SUBDIRS = Fdo SchemaMgr

DIST_SUBDIRS = Fdo SchemaMgr

EXTRA_DIST = Odbc.sln Odbc.vcproj build.bat resource.h Odbc.rc

noinst_LTLIBRARIES = libFdoRdbmsODBC.la

libFdoRdbmsODBC_la_SOURCES  = \
  Odbc.cpp \
  stdafx.cpp

noinst_HEADERS = $(libFdoRdbmsODBC_la_SOURCES) \
  stdafx.h

libFdoRdbmsODBC_la_LDFLAGS = \
  Fdo/libFdoRdbmsODBC_Fdo.la \
  SchemaMgr/libFdoRdbmsODBC_SchemaMgr.la \
  SchemaMgr/Lp/libFdoRdbmsODBC_SchemaMgr_Lp.la \
  SchemaMgr/Ov/libFdoRdbmsODBC_SchemaMgr_Ov.la \
  SchemaMgr/Ph/libFdoRdbmsODBC_SchemaMgr_Ph.la

INCLUDES = \
  -I./Fdo  \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged \
  -I../../../GenericRdbms/Inc \
  -I$(FDOUTILITIES)/Common/Inc \
  -I$(FDOUTILITIES)/SchemaMgr/Inc/Sm/Lp \
  -I$(FDOUTILITIES)/SchemaMgr/Inc \
  -I../../Inc \
  -I../../Inc/Rdbi \
  -I../ \
  -I../../ \
  -I../../Inc \
  -I../Fdo/Other \
  -I../Fdo/Capability \
  -I../Fdo/Connection \
  -I../Fdo/Custom \
  -I../Fdo/FeatureCommands \
  -I../Fdo/Filter \
  -I../Fdo/Lock \
  -I../Fdo/Lt \
  -I../Fdo/Schema \
  -I../Fdo/SpatialContext \
  -I$(FDO)/Unmanaged \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged/Src/Geometry \
  -I$(FDOUTILITIES)/SchemaMgr/Inc \
  -I$(FDOUTILITIES)/Common/Inc \
  -I../Gdbi \
  -I../LongTransactionManager \
  -I../LockManager

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

