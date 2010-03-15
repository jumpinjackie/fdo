##
## Copyright (C) 2004-2007  Autodesk, Inc.
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

EXTRA_DIST = PostGisOverrides.vcproj PostGisOverrides.rc resource.h 

noinst_LTLIBRARIES = libPostGISOverridesSrc.la

libPostGISOverridesSrc_la_SOURCES = \
 ClassCollection.cpp \
 ClassDefinition.cpp \
 ColumnDefinition.cpp \
 PhysicalSchemaMapping.cpp \
 PostGisOverrides.cpp \
 PropertyDefinitionCollection.cpp \
 PropertyDefinition.cpp \
 XmlGlobals.cpp

noinst_HEADERS = $(libPostGISOverridesSrc_la_SOURCES) \
 PostGisOverrides.h \
 XmlGlobals.h \
 stdafx.h

libPostGISOverridesSrc_la_LDFLAGS = 

INCLUDES = \
  -I../ \
  -I../../Inc \
  -I../../../../Fdo/Unmanaged/Inc \
  -I../../../../Utilities/Common/Inc \
  -I../../../../Thirdparty/pgsql/include \
  -I../../../../Thirdparty/boost

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32
