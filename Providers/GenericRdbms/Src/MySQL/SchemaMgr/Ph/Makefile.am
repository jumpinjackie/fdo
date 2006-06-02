AUTOMAKE_OPTIONS = foreign

SUBDIRS = Rd

DIST_SUBDIRS = Rd

noinst_LTLIBRARIES = libFdoRdbmsMySQL_SchemaMgr_Ph.la

libFdoRdbmsMySQL_SchemaMgr_Ph_la_SOURCES = \
  ColTypeMapper.cpp \
  ColumnChar.cpp \
  Column.cpp \
  ColumnDecimal.cpp \
  Database.cpp \
  DbObject.cpp \
  Fkey.cpp \
  Index.cpp \
  Mgr.cpp \
  Owner.cpp \
  Table.cpp \
  TempObject.cpp \
  View.cpp

noinst_HEADERS = $(libFdoRdbmsMySQL_SchemaMgr_Ph_la_SOURCES) \
  ColTypeMapper.h \
  Column.h \
  ColumnBLOB.h \
  ColumnBool.h \
  ColumnByte.h \
  ColumnChar.h \
  ColumnDate.h \
  ColumnDecimal.h \
  ColumnDouble.h \
  ColumnGeom.h \
  ColumnInt16.h \
  ColumnInt32.h \
  ColumnInt64.h \
  ColumnSingle.h \
  ColumnUnknown.h \
  Database.h \
  DbObject.h \
  Fkey.h \
  Index.h \
  Mgr.h \
  Owner.h \
  Table.h \
  TempObject.h \
  View.h

INCLUDES = \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged \
  -I../../../../../GenericRdbms/Inc \
  -I$(FDOUTILITIES)/Common/Inc \
  -I$(FDOUTILITIES)/SchemaMgr/Inc/Sm/Lp \
  -I$(FDOUTILITIES)/SchemaMgr/Inc \
  -I../../../../Inc \
  -I../../../../Inc/Rdbi \
  -I../../../ \
  -I../../../../ \
  -I../../../../Inc \
  -I../../../../Src/Fdo/Other \
  -I../../../../Src/Fdo/Capability \
  -I../../../../Src/Fdo/Connection \
  -I../../../../Src/Fdo/Custom \
  -I../../../../Src/Fdo/FeatureCommands \
  -I../../../../Src/Fdo/Filter \
  -I../../../../Src/Fdo/Lock \
  -I../../../../Src/Fdo/Lt \
  -I../../../../Src/Fdo/Schema \
  -I../../../../Src/Fdo/SpatialContext \
  -I$(FDO)/Unmanaged \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged/Src/Geometry \
  -I$(FDOUTILITIES)/SchemaMgr/Inc \
  -I$(FDOUTILITIES)/Common/Inc \
  -I../../../Gdbi \
  -I../../../LongTransactionManager \
  -I../../../LockManager

libFdoRdbmsMySQL_SchemaMgr_Ph_la_LIBADD = \
  Rd/libFdoRdbmsMySQL_SchemaMgr_Ph_Rd.la

libFdoRdbmsMySQL_SchemaMgr_Ph_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

