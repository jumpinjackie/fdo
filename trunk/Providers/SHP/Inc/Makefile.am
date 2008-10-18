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
   SHP/FdoShpOverrides.h \
   SHP/FdoShpProvider.h \
   SHP/Override/FdoShpClassCollection.h \
   SHP/Override/FdoShpClassDefinition.h \
   SHP/Override/FdoShpColumnDefinition.h \
   SHP/Override/FdoShpPhysicalSchemaMapping.h \
   SHP/Override/FdoShpPropertyDefinition.h \
   SHP/Override/FdoShpPropertyDefinitionCollection.h

INSTALL_DIR  = $(includedir)

#install header files
install :
	@if [ ! -d $(INSTALL_DIR) ]; then mkdir -p $(INSTALL_DIR); fi
	@if [ ! -d $(INSTALL_DIR)/SHP ]; then mkdir -p $(INSTALL_DIR)/SHP; fi
	@if [ ! -d $(INSTALL_DIR)/SHP/Override ]; then mkdir -p $(INSTALL_DIR)/SHP/Override; fi
	@cp -u -f ./SHP/*.h  $(INSTALL_DIR)/SHP
	@cp -u -f ./SHP/Override/*.h  $(INSTALL_DIR)/SHP/Override

uninstall :
	@rm -drf $(INSTALL_DIR)/SHP

