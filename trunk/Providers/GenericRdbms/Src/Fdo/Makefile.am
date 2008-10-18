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

SUBDIRS = Capability Connection Custom DataStore FeatureCommands Filter Lock Lt Other Pvc Schema SpatialContext

DIST_SUBDIRS = Capability Connection Custom DataStore FeatureCommands Filter Lock Lt Other Pvc Schema SpatialContext

EXTRA_DIST = \
   Fdo.vcproj \
   stdafx.h \
   stdafx.cpp

noinst_LTLIBRARIES = libFdoRdbms.la

libFdoRdbms_la_SOURCES =


# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libFdoRdbms_la_SOURCES = dummy.cpp

libFdoRdbms_la_LIBADD = \
  Capability/libFdoRdbms_Capability.la \
  Connection/libFdoRdbms_Connection.la \
  Custom/libFdoRdbms_Custom.la \
  DataStore/libFdoRdbms_DataStore.la \
  FeatureCommands/libFdoRdbms_FeatureCommands.la  \
  Filter/libFdoRdbms_Filter.la  \
  Lock/libFdoRdbms_Lock.la \
  Lt/libFdoRdbms_Lt.la \
  Other/libFdoRdbms_Other.la \
  Pvc/libFdoRdbms_Pvc.la \
  Schema/libFdoRdbms_Schema.la   \
  SpatialContext/libFdoRdbms_SpatialContext.la

libFdoRdbms_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

