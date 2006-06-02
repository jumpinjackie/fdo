AUTOMAKE_OPTIONS = foreign

noinst_LTLIBRARIES = libFdoRdbms_SpatialContext.la

libFdoRdbms_SpatialContext_la_SOURCES = \
  FdoRdbmsActivateSpatialContext.cpp  \
  FdoRdbmsGetSpatialContexts.cpp \
  FdoRdbmsCreateSpatialContext.cpp    \
  FdoRdbmsSpatialContextReader.cpp \
  FdoRdbmsDestroySpatialContext.cpp

noinst_HEADERS = $(libFdoRdbms_SpatialContext_la_SOURCES) \
  FdoRdbmsActivateSpatialContext.h \
  FdoRdbmsCreateSpatialContext.h \
  FdoRdbmsDestroySpatialContext.h \
  FdoRdbmsGetSpatialContexts.h \
  FdoRdbmsSpatialContextReader.h

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
  -I.././Other \
  -I.././Capability \
  -I.././Connection \
  -I.././Custom \
  -I.././FeatureCommands \
  -I.././Filter \
  -I.././Lock \
  -I.././Lt \
  -I.././Schema \
  -I.././SpatialContext \
  -I$(FDO)/Unmanaged \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged/src/geometry \
  -I$(FDOUTILITIES)/SchemaMgr/Inc \
  -I$(FDOUTILITIES)/Common/Inc \
  -I../../Gdbi \
  -I../../LongTransactionManager \
  -I../../LockManager

libFdoRdbms_SpatialContext_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

