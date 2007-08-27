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

SUBDIRS = 

EXTRA_DIST = 

noinst_LTLIBRARIES = libExpressionEngine.la

libExpressionEngine_la_SOURCES = \
  FdoExpressionEngine.cpp \
  FdoExpressionEngineFunctionCollection.cpp \
  FdoExpressionEngineImp.cpp \
  stdafx.cpp


INCLUDES = \
  -I$(FDOUTILITIES)/ExpressionEngine/Inc \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged/Src/common \
  -I$(FDOUTILITIES)/Common/Inc

libExpressionEngine_la_LDFLAGS = 

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32

INSTALL_DIR  = $(libdir)

#install lib files

install :
	@if [ ! -d $(INSTALL_DIR) ]; then mkdir -p $(INSTALL_DIR); fi
	@cp -u -f libExpressionEngine.la $(INSTALL_DIR)
	@cp -u -f .libs/libExpressionEngine.a $(INSTALL_DIR)

uninstall :
	@rm -drf $(INSTALL_DIR)/libExpressionEngine.a
	@rm -drf $(INSTALL_DIR)/libExpressionEngine.la

