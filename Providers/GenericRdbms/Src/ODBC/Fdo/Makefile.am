AUTOMAKE_OPTIONS = foreign

noinst_LTLIBRARIES = libFdoRdbmsODBC_Fdo.la

libFdoRdbmsODBC_Fdo_la_SOURCES = \
FdoRdbmsOdbcCommandCapabilities.cpp \
FdoRdbmsOdbcConnectionCapabilities.cpp \
FdoRdbmsOdbcConnection.cpp \
FdoRdbmsOdbcConnectionInfo.cpp \
FdoRdbmsOdbcExpressionCapabilities.cpp \
FdoRdbmsOdbcFilterCapabilities.cpp \
FdoRdbmsOdbcFilterProcessor.cpp \
FdoRdbmsOdbcSchemaCapabilities.cpp \
FdoRdbmsOdbcSpatialManager.cpp \
FdoRdbmsOdbcSpatialSqlFilter.cpp

noinst_HEADERS = $(libFdoRdbmsODBC_Fdo_la_SOURCES) \
  FdoRdbmsOdbcCommandCapabilities.h \
  FdoRdbmsOdbcConnection.h \
  FdoRdbmsOdbcConnectionCapabilities.h \
  FdoRdbmsOdbcConnectionInfo.h \
  FdoRdbmsOdbcCreateDataStore.h \
  FdoRdbmsOdbcDeleteDataStore.h \
  FdoRdbmsOdbcExpressionCapabilities.h \
  FdoRdbmsOdbcFilterCapabilities.h \
  FdoRdbmsOdbcFilterProcessor.h \
  FdoRdbmsOdbcGeometryCapabilities.h \
  FdoRdbmsOdbcProviderInfo.h \
  FdoRdbmsOdbcSchemaCapabilities.h \
  FdoRdbmsOdbcSpatialManager.h \
  FdoRdbmsOdbcSpatialSqlFilter.h

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
  -I../../LockManager \
  -I../../ODBCDriver \
  -I$(FDOODBC)/include


libFdoRdbmsODBC_Fdo_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX



