AUTOMAKE_OPTIONS = foreign

SUBDIRS = Fdo Gdbi LongTransactionManager MySQL Rdbi SchemaMgr Util Geometry ODBCDriver ODBC

DIST_SUBDIRS = Fdo Gdbi LongTransactionManager MySQL Rdbi SchemaMgr Util Geometry ODBCDriver ODBC

EXTRA_DIST =

lib_LTLIBRARIES = libFdoMySQL.la libFdoODBC.la

libdir = /usr/local/fdo-3.2.0/lib

libFdoMySQL_la_SOURCES =

##building libFdoMySQL.so
# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libFdoMySQL_la_SOURCES = dummy.cpp

libFdoMySQL_la_LIBADD =  \
  MySQL/libFdoRdbmsMySQL.la  \
  Fdo/libFdoRdbms.la   \
  Rdbi/librdbi.la  \
  MySQL/Driver/libmysqldr.la   \
  $(FDO)/Unmanaged/Src/libFDO.la   \
  -lxalan-c -lxalanMsg \
  -lxerces-c \
  Gdbi/libGdbi.la  \
  LongTransactionManager/libLTManager.la  \
  Util/libutil.la  \
  $(FDOMYSQL)/lib/libmysqlclient.a \
  $(FDOMYSQL)/lib/libz.a \
  $(FDO)/Unmanaged/Src/Spatial/libFDOSpatial.la   \
  $(FDOUTILITIES)/Common/libProvidersCommon.la \
  Geometry/Fgf/libgeometry_fgf.la  \
  SchemaMgr/Lp/libSmLpGrd.la  \
  SchemaMgr/libSmGrd.la   \
  SchemaMgr/Ov/libSchemaMgr_OV.la  \
  SchemaMgr/Ph/Cfg/libSmPhCfgGrd.la  \
  SchemaMgr/Ph/libSmPhGrd.la   \
  SchemaMgr/Ph/Rd/libSmPhRdGrd.la  \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/libSchemaMgr.la \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/Lp/libSmLogicalPhysical.la  \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/Ph/libSmPhysical.la  \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/Ph/Cfg/libSmPhCfg.la  \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/Ph/Mt/libSmPhMt.la  \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/Ph/Rb/libSmPhRb.la  \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/Ph/Rd/libSmPhRd.la

libFdoMySQL_la_LDFLAGS = \
  -shared -version-info 3:2:0  \
  -L$(FDOTHIRDPARTY)/apache/xml-xalan/c/lib \
  -L$(FDOTHIRDPARTY)/apache/xml-xerces/c/lib

#ODBC
libFdoODBC_la_SOURCES =

##building libFdoODBC.so
# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libFdoODBC_la_SOURCES = dummy.cpp

libFdoODBC_la_LIBADD =  \
  ODBC/libFdoRdbmsODBC.la  \
  Fdo/libFdoRdbms.la   \
  Rdbi/librdbi.la  \
  ODBCDriver/libFdoODBCDriver.la   \
  $(FDO)/Unmanaged/Src/libFDO.la   \
  -lxalan-c -lxalanMsg \
  -lxerces-c \
  Gdbi/libGdbi.la  \
  LongTransactionManager/libLTManager.la  \
  Util/libutil.la  \
  $(FDO)/Unmanaged/Src/Spatial/libFDOSpatial.la   \
  $(FDOUTILITIES)/Common/libProvidersCommon.la \
  Geometry/Fgf/libgeometry_fgf.la  \
  SchemaMgr/Lp/libSmLpGrd.la  \
  SchemaMgr/libSmGrd.la   \
  SchemaMgr/Ov/libSchemaMgr_OV.la  \
  SchemaMgr/Ph/Cfg/libSmPhCfgGrd.la  \
  SchemaMgr/Ph/libSmPhGrd.la   \
  SchemaMgr/Ph/Rd/libSmPhRdGrd.la  \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/libSchemaMgr.la \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/Lp/libSmLogicalPhysical.la  \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/Ph/libSmPhysical.la  \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/Ph/Cfg/libSmPhCfg.la  \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/Ph/Mt/libSmPhMt.la  \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/Ph/Rb/libSmPhRb.la  \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/Ph/Rd/libSmPhRd.la

libFdoODBC_la_LDFLAGS = \
  -shared -version-info 3:2:0  \
  -L$(FDOTHIRDPARTY)/apache/xml-xalan/c/lib \
  -L$(FDOTHIRDPARTY)/apache/xml-xerces/c/lib

