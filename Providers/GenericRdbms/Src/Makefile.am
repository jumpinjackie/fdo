##
## Copyright (C) 2004-2006  Autodesk, Inc.
## 
## This library is free software; you can redistribute it and/or
## modify it under the terms of version 2.1 of the GNU Lesser
## General Public License as published by the Free Software Foundation.
## 
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## Lesser General Public License for more details.
## 
## You should have received a copy of the GNU Lesser General Public
## License along with this library; if not, write to the Free Software
## Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
##

AUTOMAKE_OPTIONS = foreign

SUBDIRS = Fdo Gdbi LongTransactionManager LockManager MySQL Rdbi SchemaMgr Util Geometry ODBCDriver ODBC UnitTest
DIST_SUBDIRS = Fdo Gdbi LongTransactionManager LockManager MySQL Rdbi SchemaMgr Util Geometry ODBCDriver ODBC UnitTest

EXTRA_DIST = 

lib_LTLIBRARIES = libFdoMySQL.la libFdoODBC.la libSchemaMgr_OV.la

libdir = $(prefix)/lib

libFdoMySQL_la_SOURCES =

##building libFdoMySQL.so
# Dummy C++ source to cause C++ linking.

nodist_EXTRA_libFdoMySQL_la_SOURCES = dummy.cpp

libFdoMySQL_la_LIBADD =  \
  -lxalan-c \
  -lxalanMsg \
  -lxerces-c \
  -lmysqlclient \
  -lz \
  MySQL/libFdoRdbmsMySQL.la  \
  Fdo/libFdoRdbms.la   \
  Rdbi/librdbi.la  \
  MySQL/Driver/libmysqldr.la   \
  Gdbi/libGdbi.la  \
  LongTransactionManager/libLTManager.la  \
  LockManager/libLockManager.la  \
  Util/libutil.la  \
  Geometry/Fgf/libgeometry_fgf.la  \
  SchemaMgr/Lp/libSmLpGrd.la  \
  SchemaMgr/libSmGrd.la   \
  SchemaMgr/Ov/libSchemaMgr_OV.la  \
  SchemaMgr/Ph/Cfg/libSmPhCfgGrd.la  \
  SchemaMgr/Ph/libSmPhGrd.la   \
  SchemaMgr/Ph/Rd/libSmPhRdGrd.la  \
  $(FDO)/Unmanaged/Src/libFDO.la   \
  $(FDOUTILITIES)/ExpressionEngine/Src/libExpressionEngine.la \
  $(FDOUTILITIES)/Common/libProvidersCommon.la \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/libSchemaMgr.la \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/Lp/libSmLogicalPhysical.la  \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/Ph/libSmPhysical.la  \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/Ph/Cfg/libSmPhCfg.la  \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/Ph/Mt/libSmPhMt.la  \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/Ph/Rb/libSmPhRb.la  \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/Ph/Rd/libSmPhRd.la \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/Ov/libSmOverrides.la

libFdoMySQL_la_LDFLAGS = \
  -shared \
  -release $(VERSION) \
  -L/usr/lib/mysql \
  -L$(FDOMYSQL)/lib \
  -L$(FDOTHIRDPARTY)/apache/xml-xalan/c/lib \
  -L$(FDOTHIRDPARTY)/apache/xml-xerces/c/lib

#ODBC
libFdoODBC_la_SOURCES =

##building libFdoODBC.so
# Dummy C++ source to cause C++ linking.

nodist_EXTRA_libFdoODBC_la_SOURCES = dummy.cpp

libFdoODBC_la_LIBADD =  \
  -lxalan-c -lxalanMsg \
  -lxerces-c \
  -lodbc \
  -lodbcinst \
  ODBC/libFdoRdbmsODBC.la  \
  Fdo/libFdoRdbms.la   \
  Rdbi/librdbi.la  \
  ODBCDriver/libFdoODBCDriver.la   \
  Gdbi/libGdbi.la  \
  LongTransactionManager/libLTManager.la  \
  LockManager/libLockManager.la  \
  Util/libutil.la  \
  Geometry/Fgf/libgeometry_fgf.la  \
  SchemaMgr/Lp/libSmLpGrd.la  \
  SchemaMgr/libSmGrd.la   \
  SchemaMgr/Ov/libSchemaMgr_OV.la  \
  SchemaMgr/Ph/Cfg/libSmPhCfgGrd.la  \
  SchemaMgr/Ph/libSmPhGrd.la   \
  SchemaMgr/Ph/Rd/libSmPhRdGrd.la  \
  $(FDO)/Unmanaged/Src/libFDO.la   \
  $(FDOUTILITIES)/ExpressionEngine/Src/libExpressionEngine.la \
  $(FDOUTILITIES)/Common/libProvidersCommon.la \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/libSchemaMgr.la \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/Lp/libSmLogicalPhysical.la  \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/Ph/libSmPhysical.la  \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/Ph/Cfg/libSmPhCfg.la  \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/Ph/Mt/libSmPhMt.la  \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/Ph/Rb/libSmPhRb.la  \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/Ph/Rd/libSmPhRd.la \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/Ov/libSmOverrides.la

libFdoODBC_la_LDFLAGS = \
  -shared \
  -release $(VERSION)  \
  -L$(FDOTHIRDPARTY)/apache/xml-xalan/c/lib \
  -L$(FDOTHIRDPARTY)/apache/xml-xerces/c/lib \
  -L$(FDOODBC)/lib

libSchemaMgr_OV_la_SOURCES =
## building libSchemaMgr_OV.so
# Dummy C++ source to cause C++ linking.

nodist_EXTRA_libSchemaMgr_OV_la_SOURCES = dummy.cpp

libSchemaMgr_OV_la_LIBADD =  \
  SchemaMgr/Ov/libSchemaMgr_OV.la \
  $(FDO)/Unmanaged/Src/libFDO.la \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/Ov/libSmOverrides.la

libSchemaMgr_OV_la_LDFLAGS = \
  -shared \
  -release $(VERSION)
