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

EXTRA_DIST = \
  UnitTest.vcproj \
  providers.xml \
  ShpOverridesConfig.xml \
  ShpEmptySchemaConfig.xml \
  ShpSchemaConfig.xml \
  Pch.cpp

noinst_PROGRAMS = UnitTest

UnitTest_SOURCES = \
  UnitTest.cpp \
  glibpatch.cpp \
  ShpTests.cpp \
  FileAccessTests.cpp \
  ConnectTests.cpp \
  ConnectionInfoTests.cpp \
  IndexTests.cpp \
  SelectTests.cpp \
  SelectAggregatesTests.cpp \
  InsertTests.cpp \
  DeleteTests.cpp \
  SchemaTests.cpp \
  OverridesTest.cpp \
  FilterTests.cpp \
  SpatialFilterTests.cpp \
  XmlFormatter.cpp \
  PerformanceTests.cpp \
  UpdateTests.cpp \
  FidelityTests.cpp \
  BigPerformanceTests.cpp \
  ExtendedSelectTests.cpp

noinst_HEADERS = $(UnitTest_SOURCES) \
  BigPerformanceTests.h \
  FileAccessTests.h \
  Pch.h \
  SelectTests.h \
  XmlFormatter.h \
  ComparisonTests.h \
  FilterTests.h \
  PerformanceTests.h \
  ShpTests.h \
  ConnectTests.h \
  ConnectionInfoTests.h \
  IndexTests.h \
  ReadOnlyTests.h \
  SpatialFilterTests.h \
  DeleteTests.h \
  InsertTests.h \
  SchemaTests.h \
  FidelityTests.h \
  OverridesTest.h \
  SelectAggregatesTests.h \
  UpdateTests.h \
  ExtendedSelectTests.h

INCLUDES = \
  -I../ShpRead \
  -I../ShpSpatialIndex \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDOTHIRDPARTY)/linux/cppunit/include \
  -I$(FDOUTILITIES)/Common/Inc \
  -I$(FDOUTILITIES)/TestCommon/Inc \
  -I../../Inc  \
  -I../Provider

UnitTest_LDADD = \
  ../ShpRead/libSHPRead.la \
  ../ShpSpatialIndex/libSHPSpatialIndex.la \
  ../Overrides/libSHPOverridesSrc.la \
  $(FDOUTILITIES)/Common/libProvidersCommon.la \
  $(FDOUTILITIES)/TestCommon/libTestCommon.la \
  $(FDOTHIRDPARTY)/linux/cppunit/lib/libcppunit.a \
  $(FDO)/Unmanaged/Src/libFDO.la \
  -lxerces-c \
  -lxalan-c \
  -lxalanMsg

UnitTest_LDFLAGS = \
  -L$(FDOTHIRDPARTY)/apache/xml-xalan/c/lib \
  -L$(FDOTHIRDPARTY)/apache/xml-xerces/c/lib

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32 -D__STDC_LIMIT_MACROS

