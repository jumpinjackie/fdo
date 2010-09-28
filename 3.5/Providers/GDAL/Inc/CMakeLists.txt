include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	)




#original Makefile.am contents follow:

###
### Copyright (C) 2004-2006  Autodesk, Inc.
### 
### This library is free software; you can redistribute it and/or
### modify it under the terms of version 2.1 of the GNU Lesser
### General Public License as published by the Free Software Foundation.
### 
### This library is distributed in the hope that it will be useful,
### but WITHOUT ANY WARRANTY; without even the implied warranty of
### MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
### Lesser General Public License for more details.
### 
### You should have received a copy of the GNU Lesser General Public
### License along with this library; if not, write to the Free Software
### Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
###
#
#AUTOMAKE_OPTIONS = foreign
#
#EXTRA_DIST = \
#   GdalFile/FdoGrfpProvider.h \
#   GdalFile/Override/FdoGrfpClassDefinition.h \
#   GdalFile/Override/FdoGrfpOverrides.h \
#   GdalFile/Override/FdoGrfpPhysicalSchemaMapping.h \
#   GdalFile/Override/FdoGrfpRasterBandDefinition.h \
#   GdalFile/Override/FdoGrfpRasterDefinition.h \
#   GdalFile/Override/FdoGrfpRasterFeatureDefinition.h \
#   GdalFile/Override/FdoGrfpRasterGeoreferenceLocation.h \
#   GdalFile/Override/FdoGrfpRasterImageDefinition.h \
#   GdalFile/Override/FdoGrfpRasterLocation.h
#
#INSTALL_DIR  = $(includedir)
#
#install :
#	@if [ ! -d $(INSTALL_DIR) ]; then mkdir -p $(INSTALL_DIR); fi
#	@if [ ! -d $(INSTALL_DIR)/GdalFile ]; then mkdir -p $(INSTALL_DIR)/GdalFile; fi
#	@cp -urf ./GdalFile  $(INSTALL_DIR)/GdalFile
#
#uninstall :
#	@rm -drf $(INSTALL_DIR)/GdalFile
