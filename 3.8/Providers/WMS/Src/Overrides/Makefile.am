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

EXTRA_DIST = WMSOverrides.vcproj FdoWmsOverrides.rc

noinst_LTLIBRARIES = libWMSOverridesSrc.la

libWMSOverridesSrc_la_SOURCES = \
  FdoWmsOvPhysicalSchemaMapping.cpp \
  FdoWmsOvClassCollection.cpp \
  FdoWmsOvClassDefinition.cpp \
  FdoWmsOvLayerCollection.cpp \
  FdoWmsOvLayerDefinition.cpp \
  FdoWmsOvStyleDefinition.cpp \
  FdoWmsOvRasterDefinition.cpp \
  FdoWmsOverrides.cpp

noinst_HEADERS = $(libWMSOverridesSrc_la_SOURCES) \
  FdoWmsOverrides.h \
  FdoWmsXmlGlobals.h \
  resource.h \
  stdafx.h

libWMSOverridesSrc_la_LDFLAGS =

INCLUDES = \
  -I./ \
  -I../ \
  -I../Message/Inc \
  -I../../Inc \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDOUTILITIES)/Common/Inc

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32

