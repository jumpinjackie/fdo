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

noinst_LTLIBRARIES = libFdoRdbms_Lock.la

libFdoRdbms_Lock_la_SOURCES = \
  FdoRdbmsAcquireLock.cpp         \
  FdoRdbmsLockedObjectsReader.cpp \
  FdoRdbmsGetLockedObjects.cpp    \
  FdoRdbmsLockInfoReader.cpp \
  FdoRdbmsGetLockInfo.cpp         \
  FdoRdbmsLockOwnersReader.cpp \
  FdoRdbmsGetLockOwners.cpp       \
  FdoRdbmsReleaseLock.cpp \
  FdoRdbmsLockConflictReader.cpp  \
  LockUtility.cpp \
  FdoRdbmsLockConflicts.cpp

noinst_HEADERS = $(libFdoRdbms_Lock_la_SOURCES) \
  FdoRdbmsAcquireLock.h \
  FdoRdbmsGetLockedObjects.h \
  FdoRdbmsGetLockInfo.h \
  FdoRdbmsGetLockOwners.h \
  FdoRdbmsLockConflictReader.h \
  FdoRdbmsLockConflicts.h \
  FdoRdbmsLockedObjectsReader.h \
  FdoRdbmsLockInfoReader.h \
  FdoRdbmsLockOwnersReader.h \
  FdoRdbmsReleaseLock.h \
  lk_data_obj.h \
  lknm_spec.h \
  LockUtility.h

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

libFdoRdbms_Lock_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

