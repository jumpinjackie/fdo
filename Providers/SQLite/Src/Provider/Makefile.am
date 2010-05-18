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

EXTRA_DIST = SQLiteProvider.vcproj SQLiteProvider.rc resource.h

noinst_LTLIBRARIES = libSQLiteProviderSrc.la

libSQLiteProviderSrc_la_SOURCES = \
   slt.cpp \
   SltConversionUtils.cpp \
   SltExprExtensions.cpp \
   SltMetadata.cpp \
   SltProvider.cpp \
   SltQueryTranslator.cpp \
   SltReader.cpp \
   SltSpatialContextReader.cpp \
   SpatialIndexDescriptor.cpp \
   SQLiteSchemaMergeContext.cpp \
   SltBLOBStreamReader.cpp

noinst_HEADERS = $(libSQLiteProviderSrc_la_SOURCES) \
   PropertyNameIndex.h \
   RowidIterator.h \
   slt.h \
   SltCapabilities.h \
   SltCommands.h \
   SltCommandTemplates.h \
   SltConversionUtils.h \
   SltExprExtensions.h \
   SltMetadata.h \
   SltProvider.h \
   SltQueryTranslator.h \
   SltReader.h \
   SltSpatialContextReader.h \
   stdafx.h \
   SQLiteProvider.h \
   StringUtil.h \
   SltTransaction.h \
   SQLiteSchemaMergeContext.h \
   SltBLOBStreamReader.h

libSQLiteProviderSrc_la_LDFLAGS = 

INCLUDES = \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged/Src/Nls \
  -I$(FDOUTILITIES)/Common/Inc \
  -I../Message/Inc \
  -I../sqlite \
  -I../SpatialIndex \
  -I$(FDOUTILITIES)/ExpressionEngine/Inc \
  -I/usr/include

CXXFLAGS = @CXXFLAGS_SQLITE@ -msse -msse2 -mmmx -DSQLITE_OMIT_AUTHORIZATION -DSQLITE_OMIT_PROGRESS_CALLBACK
