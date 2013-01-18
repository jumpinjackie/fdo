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

EXTRA_DIST = \
   ArcSDE/Override/FdoArcSDEClassMapping.h \
   ArcSDE/Override/FdoArcSDEIndexMapping.h \
   ArcSDE/Override/FdoArcSDEPropertyMapping.h \
   ArcSDE/Override/FdoArcSDESchemaMapping.h

INSTALL_DIR  = $(includedir)

#install header files
install :
	@if [ ! -d $(INSTALL_DIR) ]; then mkdir -p $(INSTALL_DIR); fi
	@if [ ! -d $(INSTALL_DIR)/ArcSDE/Override ]; then mkdir -p $(INSTALL_DIR)/ArcSDE/Override; fi
	@cp -u -f ./ArcSDE/Override/*.h  $(INSTALL_DIR)/ArcSDE/Override

uninstall :
	@rm -drf $(INSTALL_DIR)/ArcSDE
