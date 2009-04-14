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

EXTRA_DIST = LockManager.vcproj

noinst_LTLIBRARIES = libLockManager.la

libLockManager_la_SOURCES = \
  Disposable.cpp \
  LockConflict.cpp \
  LockInfo.cpp \
  LockType.cpp \
  LockTypes.cpp \
  OwnerInfo.cpp \
  stdafx.cpp \
  TableInfo.cpp

noinst_HEADERS = $(libLockManager_la_SOURCES) \
  Disposable.h \
  LockConflict.h \
  LockConflictQueryHandler.h \
  LockConstants.h \
  LockInfo.h \
  LockInfoQueryHandler.h \
  LockManager.h \
  LockManagerService.h \
  LockOwnersQueryHandler.h \
  LockStrategy.h \
  LockTableQueryHandler.h \
  LockType.h \
  LockTypeEnum.h \
  LockTypes.h \
  Operation.h \
  OwnerInfo.h \
  stdafx.h \
  TableInfo.h

INCLUDES = \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged \
  -I$(FDOUTILITIES)/SchemaMgr/Inc \
  -I$(FDOUTILITIES)/Common/Inc \
  -I$(FDO)/Unmanaged/Src/Geometry \
  -I../Fdo/Lock \
  -I../Fdo/Other \
  -I../Fdo/Connection \
  -I../Fdo/Capability \
  -I../Fdo/Custom \
  -I../Fdo/FeatureCommands \
  -I../Fdo/Filter \
  -I../Fdo/Lt \
  -I../Fdo/Schema \
  -I../Fdo/SpatialContext \
  -I../ \
  -I../../ \
  -I../../Inc \
  -I../Gdbi \
  -I../LongTransactionManager \
  -I../LockManager

libLockManager_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

