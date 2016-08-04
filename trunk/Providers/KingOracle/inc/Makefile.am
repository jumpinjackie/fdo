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

EXTRA_DIST = \
   KingOracle/FdoKgOraOverrides.h \
   KingOracle/fdokgoraprovider.h \
   KingOracle/Overrides/FdoKgOraClassCollection.h \
   KingOracle/Overrides/FdoKgOraPhysicalSchemaMapping.h \
   KingOracle/Overrides/FdoKgOraClassDefinition.h \
   KingOracle/Overrides/FdoKgOraPropertyDefinitionCollection.h \
   KingOracle/Overrides/FdoKgOraColumnDefinition.h \
   KingOracle/Overrides/FdoKgOraPropertyDefinition.h

INSTALL_DIR  = $(includedir)

install :
	@if [ ! -d $(INSTALL_DIR) ]; then mkdir -p $(INSTALL_DIR); fi
	@if [ ! -d $(INSTALL_DIR)/KingOracle ]; then mkdir -p $(INSTALL_DIR)/KingOracle; fi
	@cp -urf ./KingOracle  $(INSTALL_DIR)/KingOracle

uninstall :
	@rm -drf $(INSTALL_DIR)/KingOracle
