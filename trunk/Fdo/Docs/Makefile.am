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

INSTALL_DIR  = $(prefix)

SUBDIRS = doc_src XmlSchema

DIST_SUBDIRS = doc_src XmlSchema

#install Docs
install :
	@if [ ! -d $(INSTALL_DIR)/docs ]; then mkdir -p $(INSTALL_DIR)/docs; fi
	@if [ ! -d $(INSTALL_DIR)/docs/XmlSchema ]; then mkdir -p $(INSTALL_DIR)/docs/XmlSchema; fi
	@if [ ! -d $(INSTALL_DIR)/docs/XmlSchema/Gml ]; then mkdir -p $(INSTALL_DIR)/docs/XmlSchema/Gml; fi
	@if [ ! -d $(INSTALL_DIR)/docs/XmlSchema/Xlink ]; then mkdir -p $(INSTALL_DIR)/docs/XmlSchema/Xlink; fi
	@if [ ! -d $(INSTALL_DIR)/docs/XmlSchema/XmlSchema ]; then mkdir -p $(INSTALL_DIR)/docs/XmlSchema/XmlSchema; fi
	@../Install/XsdInstallPaths/XsdInstallPaths ./XmlSchema $(INSTALL_DIR)/docs/XmlSchema
	@../Install/XsdInstallPaths/XsdInstallPaths ./XmlSchema/Gml $(INSTALL_DIR)/docs/XmlSchema/Gml
	@../Install/XsdInstallPaths/XsdInstallPaths ./XmlSchema/Xlink $(INSTALL_DIR)/docs/XmlSchema/Xlink
	@../Install/XsdInstallPaths/XsdInstallPaths ./XmlSchema/XmlSchema $(INSTALL_DIR)/docs/XmlSchema/XmlSchema
	@cp -u -f XmlSchema/Gml/CopyRight.txt $(INSTALL_DIR)/docs/XmlSchema/Gml
	@cp -u -f XmlSchema/Xlink/CopyRight.txt $(INSTALL_DIR)/docs/XmlSchema/Xlink
	@cp -u -f XmlSchema/XmlSchema/base.css $(INSTALL_DIR)/docs/XmlSchema/XmlSchema

uninstall :
	@rm -drf $(INSTALL_DIR)/docs

