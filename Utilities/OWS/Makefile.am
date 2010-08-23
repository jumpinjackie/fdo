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

SUBDIRS = Src

DIST_SUBDIRS = Src

EXTRA_DIST = \
  FdoOws.cpp \
  FdoOws.rc \
  OWS.vcproj \
  resource.h \
  stdafx.h \
  stdafx.cpp \
  Docs/doc_src/OWS_Provider_API.dox \
  Docs/doc_src/OWS_Provider_API.doxdb \
  Docs/doc_src/overview_topics/index.dtx \
  Docs/doc_src/overview_topics/OWS_global_class_hierarchy.dtx \
  Inc/OWS/FdoOwsCapabilities.h \
  Inc/OWS/FdoOwsMIMEType.h \
  Inc/OWS/FdoOwsDelegate.h \
  Inc/OWS/FdoOwsOgcFilterSerializer.h \
  Inc/OWS/FdoOwsGeographicBoundingBoxCollection.h \
  Inc/OWS/FdoOwsRequest.h \
  Inc/OWS/FdoOwsGeographicBoundingBox.h \
  Inc/OWS/FdoOwsRequestMetadata.h \
  Inc/OWS/FdoOwsGetCapabilities.h \
  Inc/OWS/FdoOwsResponse.h \
  Inc/OWS/FdoOwsGlobals.h \
  Inc/OWS/FdoOwsServiceIdentification.h \
  Inc/OWS/FdoOWS.h \
  Inc/OWS/FdoOwsServiceMetadata.h \
  Inc/OWS/FdoOwsIEncodable.h \
  Inc/OWS/FdoOwsXmlSaxContext.h \
  Inc/OWS/FdoOwsIHttpHandler.h
  Inc/OWS/FdoOwsOperationsMetadata.h \
  Inc/OWS/FdoOwsServiceProvider.h 

##building libFdoOws.so
lib_LTLIBRARIES = libFdoOws.la

libFdoOws_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libFdoOws_la_SOURCES = dummy.cpp

libFdoOws_la_LIBADD =  \
  Src/libFdoOwsSrc.la \
  $(FDO)/Unmanaged/Src/libFDO.la \
  $(FDO)/Unmanaged/Src/Geometry/libFDOGeometry.la  \
  $(FDO)/Unmanaged/Src/Common/libFDOCommon.la  \
  $(FDOUTILITIES)/Common/libProvidersCommon.la \
  $(FDOTHIRDPARTY)/boost/stage/lib/libboost_thread.a \
  $(FDOTHIRDPARTY)/libcurl/lib/linux/libcurl.a  \
  $(FDOTHIRDPARTY)/openssl/lib/linux/libssl.a \
  $(FDOTHIRDPARTY)/openssl/lib/linux/libcrypto.a \
  -lz \
  -lpthread

libFdoOws_la_LDFLAGS = -shared \
  -release $(VERSION)

