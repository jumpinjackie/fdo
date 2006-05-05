AUTOMAKE_OPTIONS = foreign

noinst_LTLIBRARIES = libFdoRdbmsMySQL_Fdo.la

libFdoRdbmsMySQL_Fdo_la_SOURCES = \
  FdoRdbmsMySqlConnectionCapabilities.cpp \
  FdoRdbmsMySqlFilterCapabilities.cpp \
  FdoRdbmsMySqlFilterProcessor.cpp \
  FdoRdbmsMySqlGeometryCapabilities.cpp \
  FdoRdbmsMySqlConnection.cpp \
  FdoRdbmsMySqlCommandCapabilities.cpp \
  FdoRdbmsMySqlConnectionInfo.cpp

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
  -I../../../Src/Fdo/Topo \
  -I$(FDO)/Unmanaged \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged/Src/Geometry \
  -I$(FDOUTILITIES)/SchemaMgr/Inc \
  -I$(FDOUTILITIES)/Common/Inc \
  -I../../Gdbi  \
  -I../../LongTransactionManager  \
  -I../../LockManager


libFdoRdbmsMySQL_Fdo_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

