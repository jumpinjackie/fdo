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
  providers.xml

noinst_PROGRAMS = UnitTest

UnitTest_SOURCES = \
  UnitTest.cpp \
  glibpatch.cpp \
  MasterTest.cpp \
  ApplySchemaTest.cpp \
  AssociationSchemaTest.cpp \
  AssociationInsertTest.cpp \
  AssociationSelectTest.cpp \
  AssociationUpdateTest.cpp \
  AssociationDeleteTest.cpp \
  ConnectionInfoTest.cpp \
  ConstraintsTest.cpp \
  ExtendedSelectTest.cpp \
  NlsTest.cpp \
  SelectTest.cpp \
  SpatialContextTest.cpp \
  UnitTestUtil.cpp

noinst_HEADERS = $(UnitTest_SOURCES) \
  ApplySchemaTest.h \
  AssociationDeleteTest.h \
  AssociationInsertTest.h \
  AssociationSchemaTest.h \
  AssociationSelectTest.h \
  AssociationUpdateTest.h \
  ConnectionInfoTest.h \
  ConstraintsTest.h \
  ExtendedSelectTest.h \
  FdoMultiThreadTest.h \
  FgfStreamWriter.h \
  MasterTest.h \
  NlsTest.h \
  SelectTest.h \
  SpatialContextTest.h \
  UnitTestUtil.h \
  tester-linux.h

INCLUDES = \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDOUTILITIES)/TestCommon/Inc \
  -I$(FDOTHIRDPARTY)/linux/cppunit/include/ \
  -I../../Inc

UnitTest_LDADD = \
  $(FDOUTILITIES)/TestCommon/libTestCommon.la \
  $(FDO)/Unmanaged/Src/libFDO.la \
  $(FDOTHIRDPARTY)/linux/cppunit/lib/libcppunit.a \
  -ldl \
  -lxalan-c \
  -lxalanMsg \
  -lxerces-c

UnitTest_LDFLAGS = \
  -L$(FDOTHIRDPARTY)/apache/xml-xalan/c/lib \
  -L$(FDOTHIRDPARTY)/apache/xml-xerces/c/lib

CXXFLAGS = @CXXFLAGS_SDF@

