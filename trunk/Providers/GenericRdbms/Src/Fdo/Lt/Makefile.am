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

noinst_LTLIBRARIES = libFdoRdbms_Lt.la

libFdoRdbms_Lt_la_SOURCES = \
  FdoRdbmsActivateLongTransaction.cpp \
  FdoRdbmsCommitLongTransaction.cpp \
  FdoRdbmsCreateLongTransaction.cpp \
  FdoRdbmsDeactivateLongTransaction.cpp \
  FdoRdbmsGetLongTransactions.cpp \
  FdoRdbmsLongTransactionConflictDirectiveEnumerator.cpp \
  FdoRdbmsLongTransactionReader.cpp \
  FdoRdbmsRollbackLongTransaction.cpp

noinst_HEADERS = $(libFdoRdbms_Lt_la_SOURCES) \
  cfl_obj.h \
  FdoRdbmsActivateLongTransaction.h \
  FdoRdbmsCommitLongTransaction.h \
  FdoRdbmsCreateLongTransaction.h \
  FdoRdbmsDeactivateLongTransaction.h \
  FdoRdbmsGetLongTransactions.h \
  FdoRdbmsLongTransactionConflictDirectiveEnumerator.h \
  FdoRdbmsLongTransactionReader.h \
  FdoRdbmsRollbackLongTransaction.h \
  prim_key_obj.h \
  tab_cfl_list_obj.h \
  version_info.h

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

libFdoRdbms_Lt_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

