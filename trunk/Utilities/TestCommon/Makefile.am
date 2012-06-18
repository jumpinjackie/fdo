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

noinst_LTLIBRARIES = libTestCommon.la

EXTRA_DIST = \
  TestCommon.sln \
  TestCommon.vcproj

libTestCommon_la_SOURCES = \
  Src/TestCommonConstraints.cpp \
  Src/TestCommonExpressionFunction.cpp \
  Src/TestCommonExpressionFunctionR.cpp \
  Src/TestCommonExpressionFunctionW.cpp \
  Src/TestCommonFeatureCommands.cpp \
  Src/TestCommonInsert.cpp \
  Src/TestCommonMiscUtil.cpp \
  Src/TestCommonFileUtil.cpp \
  Src/TestCommonGeomUtil.cpp \
  Src/TestCommonSchemaUtil.cpp \
  Src/stdafx.cpp

noinst_HEADERS = $(libTestCommon_la_SOURCES) \
  Inc/TestCommonConstraints.h \
  Inc/TestCommonExpressionFunction.h \
  Inc/TestCommonExpressionFunctionR.h \
  Inc/TestCommonExpressionFunctionW.h \
  Inc/TestCommonFeatureCommands.h \
  Inc/TestCommonInsert.h \
  Inc/TestCommonMiscUtil.h \
  Inc/TestCommonFileUtil.h \
  Inc/TestCommonGeomUtil.h \
  Inc/TestCommonSchemaUtil.h \
  Inc/TestCommon.h \
  Inc/stdafx.h

INCLUDES = \
  -I$(FDO)/Unmanaged/Inc \
  -I./Inc \
  -I$(FDOTHIRDPARTY)/linux/cppunit/include \
  -I$(FDOUTILITIES)/Common/Inc \
  -I$(FDO)/Unmanaged/Src/Common

libTestCommon_la_LDFLAGS = 

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32 -pthread -DHAVE_GEOM_INFO_TYPE

