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

noinst_LTLIBRARIES = libFdoOwsSrc.la

libFdoOwsSrc_la_SOURCES = \
  FdoOwsCapabilities.cpp \
  FdoOwsRequest.cpp \
  FdoOwsDelegate.cpp \
  FdoOwsRequestMetadata.cpp \
  FdoOwsExceptionReport.cpp \
  FdoOwsResponse.cpp \
  FdoOwsServiceIdentification.cpp \
  FdoOwsGetCapabilities.cpp \
  FdoOwsServiceMetadata.cpp \
  FdoOwsHttpHandler.cpp \
  FdoOwsUrlResolver.cpp \
  FdoOwsIHttpHandler.cpp \
  FdoOwsOgcFilterSerializer.cpp \
  FdoOwsGeographicBoundingBox.cpp \
  FdoOwsGeographicBoundingBoxCollection.cpp \
  FdoOwsOgcGeometrySerializer.cpp 

noinst_HEADERS = $(libFdoOwsSrc_la_SOURCES) \
  FdoOwsExceptionReport.h \
  FdoOwsHttpHandler.h \
  FdoOwsOgcGeometrySerializer.h \
  FdoOwsUrlResolver.h

INCLUDES = \
  -I../ \
  -I../Inc \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDOUTILITIES)/Common/Inc \
  -I$(FDOTHIRDPARTY)/boost_1_32_0

libFdoOwsSrc_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32 -pthread

