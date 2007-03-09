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

INSTALL_DIR  = /usr/local/fdo-3.3.0

SUBDIRS = doc_src

DIST_SUBDIRS = doc_src

EXTRA_DIST = XmlSchema/FdoMySQLOverride.xsd XmlSchema/FdoRdbmsOverride.xsd XmlSchema/FdoSQLServerOverride.xsd

#install doc files
install :
	@if [ ! -d $(INSTALL_DIR)/docs ]; then mkdir -p $(INSTALL_DIR)/docs; fi
	@if [ ! -d $(INSTALL_DIR)/docs/XmlSchema ]; then mkdir -p $(INSTALL_DIR)/docs/XmlSchema; fi
	@$(FDO)/Install/XsdInstallPaths/XsdInstallPaths ./XmlSchema $(INSTALL_DIR)/docs/XmlSchema/XmlSchema

