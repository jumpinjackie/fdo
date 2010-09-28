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

EXTRA_DIST = Gdbi.vcproj ReadMe.txt

noinst_LTLIBRARIES = libGdbiCustom.la

libGdbiCustom_la_SOURCES = \
  GdbiCommands.cpp \
  ../../../Gdbi/GdbiConnection.cpp \
  ../../../Gdbi/GdbiException.cpp \
  ../../../Gdbi/GdbiQueryResult.cpp \
  ../../../Gdbi/GdbiStatement.cpp \
  ../../../Gdbi/stdafx.cpp

noinst_HEADERS = $(libGdbiCustom_la_SOURCES) \
  ../../../Gdbi/GdbiCommands.h \
  ../../../Gdbi/GdbiConnection.h \
  ../../../Gdbi/GdbiException.h \
  ../../../Gdbi/GdbiQueryResult.h \
  ../../../Gdbi/GdbiStatement.h \
  ../../../Gdbi/GdbiTypes.h \
  ../../../Gdbi/stdafx.h

INCLUDES = \
  -I../../../Gdbi \
  -I../../../.. \
  -I../.. \
  -I$(FDO)/Unmanaged \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDOUTILITIES)/Common/Inc

libGdbiCustom_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

