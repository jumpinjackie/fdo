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

noinst_LTLIBRARIES = libFDOCommon.la

EXTRA_DIST = \
  Common.rc \
  resource.h \
  Gml212/feature.xsd \
  Gml212/geometry.xsd \
  Gml212/xlinks.xsd

libFDOCommon_la_SOURCES = \
  ArrayHelper.cpp \
  CommonInternal.cpp \
  Context.cpp \
  DictionaryElement.cpp \
  Exception.cpp  \
  IDisposable.cpp \
  Semaphore.cpp  \
  StringP.cpp  \
  StringCollection.cpp  \
  StringUtility.cpp \
  Vector.cpp \
  ConvertUTF.cpp \
  utf8_.cpp  \
  Io/BufferStream.cpp \
  Io/ByteStreamReader.cpp \
  Io/MemoryStream.cpp \
  Io/FileStream.cpp \
  Io/TextReader.cpp \
  Io/TextWriter.cpp \
  Xml/Attribute.cpp \
  Xml/CharDataHandler.cpp \
  Xml/CopyHandler.cpp \
  Xml/Reader.cpp \
  Xml/ReaderXrcs.cpp \
  Xml/SaxContext.cpp \
  Xml/SaxHandler.cpp \
  Xml/UtilXrcs.cpp \
  Xml/Writer.cpp \
  Xml/WriterXrcs.cpp \
  Xml/Xml.cpp \
  Xml/XmlException.cpp \
  Xsl/Transformer.cpp \
  Xsl/TransformerXalan.cpp \
  Xsl/XML885915Transcoder.cpp \
  Gml212/Schema.cpp \
  Gml212/Gml212.cpp \
  Gml311/Gml311.cpp

noinst_HEADERS = $(libFDOCommon_la_SOURCES) \
  ConvertUTF.h \
  StringUtility.h \
  utf8_.h \
  Gml212/feature.h \
  Gml212/geometry.h \
  Gml212/xlinks.h \
  Xml/ReaderXrcs.h \
  Xml/WriterXrcs.h \
  Xsl/XML885915Transcoder.h

INCLUDES = \
  -I../../Inc \
  -I$(FDOTHIRDPARTY)/apache/xalan/src \
  -I$(FDOTHIRDPARTY)/apache/xerces/src 

libFDOCommon_la_LDFLAGS = 

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE

Gml212/Schema.cpp : Gml212/xlinks.h

Gml212/xlinks.h  : Gml212/xlinks.xsd
	@../FileToHdr/FileToHdr Gml212/xlinks.xsd Gml212/xlinks.h xlinks

Gml212/Schema.cpp : Gml212/geometry.h

Gml212/geometry.h  : Gml212/geometry.xsd
	@../FileToHdr/FileToHdr Gml212/geometry.xsd Gml212/geometry.h geometry

Gml212/Schema.cpp : Gml212/feature.h

Gml212/feature.h  : Gml212/feature.xsd
	@../FileToHdr/FileToHdr Gml212/feature.xsd Gml212/feature.h feature

