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

EXTRA_DIST = FDOUtils.vcproj

noinst_LTLIBRARIES = libSDFUtils.la

libSDFUtils_la_SOURCES = \
  StringValue.cpp \
  NullValue.cpp \
  DoubleValue.cpp \
  Int64Value.cpp \
  BooleanValue.cpp \
  DateTimeValue.cpp \
  DataValuePool.cpp \
  FilterExecutor.cpp \
  PropertyIndex.cpp \
  OSUtils.cpp

noinst_HEADERS = $(libSDFUtils_la_SOURCES) \
   BooleanValue.h \
   DataValue.h \
   DataValuePool.h \
   DataValueStack.h \
   DateTimeValue.h \
   DoubleValue.h \
   FilterExecutor.h \
   Int64Value.h \
   NullValue.h \
   OSUtils.h \
   PropertyIndex.h \
   stdafx.h \
   StringValue.h

libSDFUtils_la_LDFLAGS = 

INCLUDES = \
  -I./   \
  -I../../Inc \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged/Src/Nls \
  -I$(FDOUTILITIES)/Common/Inc \
  -I/usr/include 

CXXFLAGS = @CXXFLAGS_SDF@ -DSQLITE 

