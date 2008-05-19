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
  Pch.cpp \
  providers.xml

if SDEVER_ARCUNITTEST92
   BUILD_USING_SDE92= -lpe -lsde -lsg
endif

if SDEVER_ARCUNITTEST91
   BUILD_USING_SDE91= -lpe91 -lsde91 -lsg91
endif

noinst_PROGRAMS = UnitTest

UnitTest_SOURCES = \
  UnitTest.cpp \
  ABasicOpenTests.cpp \
  BasicConnectionTests.cpp \
  BasicSchemaTests.cpp \
  glibpatch.cpp \
  LongTransactionTests.cpp \
  ArcSDETests.cpp \
  BasicDeleteTests.cpp \
  BasicTransactionTests.cpp \
  LockTests.cpp \
  SelectTests.cpp \
  BasicArcSDETests.cpp \
  BasicInsertTests.cpp \
  BasicUpdateTests.cpp \
  LongTransactionExternalTests.cpp \
  SpatialContextTests.cpp \
  SQLTests.cpp \
  RecommitTest.cpp \
  SelectAggregatesTests.cpp \
  ThreadingTests.cpp

noinst_HEADERS = $(UnitTest_SOURCES) \
  ABasicOpenTests.h \
  ArcSDETests.h \
  BasicArcSDETests.h \
  BasicConnectionTests.h \
  BasicDeleteTests.h \
  BasicInsertTests.h \
  BasicSchemaTests.h \
  BasicTransactionTests.h \
  BasicUpdateTests.h \
  LockTests.h \
  LongTransactionExternalTests.h \
  LongTransactionTests.h \
  Pch.h \
  RecommitTest.h \
  SelectAggregatesTests.h \
  SelectTests.h \
  SpatialContextTests.h \
  SQLTests.h \
  ThreadingTests.h

INCLUDES = \
  -I../Provider \
  -I. \
  -I$(FDO)/Unmanaged/Inc \
  -I$(SDEHOME)/include \
  -I$(FDOTHIRDPARTY)/linux/cppunit/include \
  -I$(FDOUTILITIES)/Common/Inc \
  -I$(FDOUTILITIES)/ExpressionEngine/Inc \
  -I../../Inc


UnitTest_LDADD = \
  ../Loader/libArcSDEProviderSrc.la \
  $(FDOTHIRDPARTY)/linux/cppunit/lib/libcppunit.a \
  $(FDO)/Unmanaged/Src/libFDO.la \
  $(BUILD_USING_SDE91) \
  $(BUILD_USING_SDE92) \
  $(FDOUTILITIES)/Common/libProvidersCommon.la \
  -lpthread_nonshared \
  -lpthread

UnitTest_LDFLAGS = \
  -L$(SDEHOME)/lib

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32 -D__STDC_LIMIT_MACROS

