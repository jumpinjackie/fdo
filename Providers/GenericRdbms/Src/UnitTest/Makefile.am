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

SUBDIRS = Common Odbc MySql
DIST_SUBDIRS = Common Odbc MySql

EXTRA_DIST = \
  MsTest.xls \
  Lidar.mdb \
  MSTest.mdb \
  apply_schema_err1_master.txt \
  apply_schema_err2_master.txt \
  apply_schema_err2M_master.txt \
  apply_schema_err3_master.txt \
  apply_schema_err3M_master.txt \
  apply_schema_err3S_master.txt \
  apply_schema_err4_master.txt \
  apply_schema_err5_master.txt \
  apply_schema_err5M_master.txt \
  apply_schema_err6_master.txt \
  apply_schema_err6S_master.txt \
  apply_schema_err7_master.txt \
  apply_schema_err8_master.txt \
  apply_schema_err8M_master.txt \
  apply_schema_err9_master.txt \
  apply_schema_foreign1_master.txt \
  apply_schema_foreign2_master.txt \
  apply_schema_foreign3_master.txt \
  apply_schema_foreign_out1_master.txt \
  apply_schema_foreign_out2_master.txt \
  apply_schema_overrides1_master.txt \
  apply_schema_overrides2_master.txt \
  apply_schema_overrides3_master.txt \
  apply_schema_overrides_def1_master.txt \
  apply_schema_overrides_defout1_master.txt \
  apply_schema_overrides_out1_master.txt \
  apply_schema_overrides_out2_master.txt \
  apply_schema_test1_master.txt \
  apply_schema_test2_master.txt \
  apply_schema_test3_master.txt \
  apply_schema_test4_master.txt \
  apply_schema_test5_master.txt \
  apply_schema_test_ov1_master.txt \
  apply_schema_test_ov2_master.txt \
  apply_schema_test_ov3_master.txt \
  Country-Text.txt \
  g_coords.txt \
  gen_default1_MySql_master.txt \
  gen_default1_Oracle_master.txt \
  gen_default1_SqlServer_master.txt \
  gen_default1_SqlServer2005_master.txt \
  impexp2a_master.txt \
  impexp2b_master.txt \
  impexp_err1_master.txt \
  impexp_err7_master.txt \
  polyAttr.txt \
  schema_rb_nnyn_master.txt \
  schemaGenConfig1_master.txt \
  SchemaOverrideAll2_master.txt \
  SchemaOverrideErrHigh_master.txt \
  SchemaOverrideErrLow_master.txt \
  SchemaOverrideErrNormal_master.txt \
  UnitTestMySql.vcproj \
  UnitTestOdbc.vcproj \
  config1_in.xml \
  MsTestConfig.xml \
  MsTestNoGeometry.xml \
  MsTestXlsConfig.xml \
  OracleTestConfig2.xml \
  OracleTestConfig.xml \
  providers.xml \
  MySqlInit.txt \
  OdbcInit.txt \
  SchemaOverrideAll.xml \
  SchemaOverrideErr.xml \
  LogicalPhysicalBender.xslt \
  LogicalPhysicalFormatter.xslt \
  LogicalPhysicalSorter.xslt \
  OverrideBender.xslt

noinst_PROGRAMS = UnitTestMySql UnitTestOdbc

#MySql
UnitTestMySql_SOURCES = MySqlTestRegister.cpp

UnitTestMySql_LDADD =  \
  -lxalan-c \
  -lxalanMsg \
  -lxerces-c \
  -lz \
  -lmysqlclient \
  ./Common/libCommonUnitTests.la \
  ./MySql/libMySqlUnitTests.la  \
  ../MySQL/libFdoRdbmsMySQL.la  \
  ../Fdo/libFdoRdbms.la   \
  ../Rdbi/librdbi.la  \
  ../MySQL/Driver/libmysqldr.la   \
  ../Gdbi/libGdbi.la  \
  ../LongTransactionManager/libLTManager.la  \
  ../LockManager/libLockManager.la  \
  ../Util/libutil.la  \
  ../Geometry/Fgf/libgeometry_fgf.la  \
  ../SchemaMgr/Lp/libSmLpGrd.la  \
  ../SchemaMgr/libSmGrd.la   \
  ../SchemaMgr/Ov/libSchemaMgr_OV.la  \
  ../SchemaMgr/Ph/Cfg/libSmPhCfgGrd.la  \
  ../SchemaMgr/Ph/libSmPhGrd.la   \
  ../SchemaMgr/Ph/Rd/libSmPhRdGrd.la  \
  $(FDO)/Unmanaged/Src/libFDO.la   \
  $(FDO)/Unmanaged/Src/Spatial/libFDOSpatial.la   \
  $(FDOTHIRDPARTY)/linux/cppunit/lib/libcppunit.a \
  $(FDOUTILITIES)/Common/libProvidersCommon.la \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/libSchemaMgr.la \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/Lp/libSmLogicalPhysical.la  \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/Ph/libSmPhysical.la  \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/Ph/Cfg/libSmPhCfg.la  \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/Ph/Mt/libSmPhMt.la  \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/Ph/Rb/libSmPhRb.la  \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/Ph/Rd/libSmPhRd.la \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/Ov/libSmOverrides.la \
  ../Rdbi/librdbi.la \
  ../Util/libutil.la \
  -lmysqlclient \
  -lz

UnitTestMySql_LDFLAGS = \
  -L/usr/lib/mysql \
  -L$(FDOMYSQL)/lib

#Odbc
UnitTestOdbc_SOURCES = OdbcTestRegister.cpp

UnitTestOdbc_LDADD =  \
  -lxalan-c \
  -lxalanMsg \
  -lxerces-c \
  -lz \
  -lodbc \
  ./Common/libCommonUnitTests.la \
  ./Odbc/libOdbcUnitTests.la  \
  ../ODBC/libFdoRdbmsODBC.la  \
  ../Fdo/libFdoRdbms.la   \
  ../Rdbi/librdbi.la  \
  ../ODBCDriver/libFdoODBCDriver.la \
  ../Gdbi/libGdbi.la  \
  ../LongTransactionManager/libLTManager.la  \
  ../LockManager/libLockManager.la  \
  ../Util/libutil.la  \
  ../Geometry/Fgf/libgeometry_fgf.la  \
  ../SchemaMgr/Lp/libSmLpGrd.la  \
  ../SchemaMgr/libSmGrd.la   \
  ../SchemaMgr/Ov/libSchemaMgr_OV.la  \
  ../SchemaMgr/Ph/Cfg/libSmPhCfgGrd.la  \
  ../SchemaMgr/Ph/libSmPhGrd.la   \
  ../SchemaMgr/Ph/Rd/libSmPhRdGrd.la  \
  $(FDO)/Unmanaged/Src/libFDO.la   \
  $(FDO)/Unmanaged/Src/Spatial/libFDOSpatial.la   \
  $(FDOTHIRDPARTY)/linux/cppunit/lib/libcppunit.a \
  $(FDOUTILITIES)/Common/libProvidersCommon.la \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/libSchemaMgr.la \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/Lp/libSmLogicalPhysical.la  \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/Ph/libSmPhysical.la  \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/Ph/Cfg/libSmPhCfg.la  \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/Ph/Mt/libSmPhMt.la  \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/Ph/Rb/libSmPhRb.la  \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/Ph/Rd/libSmPhRd.la \
  $(FDOUTILITIES)/SchemaMgr/Src/Sm/Ov/libSmOverrides.la \
  $(FDOUTILITIES)/Common/libProvidersCommon.la \
  ../Rdbi/librdbi.la \
  ../Util/libutil.la \
  -lodbc \
  -lz

UnitTestOdbc_LDFLAGS = \
  -L$(FDOODBC)/lib
  
INCLUDES = \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged \
  -I$(FDOTHIRDPARTY)/linux/cppunit/include \
  -I$(FDOUTILITIES)/SchemaMgr/Inc \
  -I$(FDOUTILITIES)/Common/Inc \
  -I../LockManager \
  -I../LongTransactionManager \
  -I../Gdbi \
  -I../../Inc \
  -I../.. \
  -I../Fdo/Connection \
  -I../Fdo/SpatialContext \
  -I../Fdo/Other \
  -I../ \
  -I./ \
  -I../Fdo/FeatureCommands \
  -I./Common \
  -I./MySql \
  -I./Odbc

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32 -D__STDC_LIMIT_MACROS -DRDBI_STATIC
