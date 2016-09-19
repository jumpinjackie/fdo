#
# Copyright (C) 2004-2006  Autodesk, Inc.
# 
# This library is free software; you can redistribute it and/or
# modify it under the terms of version 2.1 of the GNU Lesser
# General Public License as published by the Free Software Foundation.
# 
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#
# GNUmakefile - building fdogdal tests
#
# Mateusz Loskot <mateusz@loskot.net>
#
###### CONFIGURATION ######
#
# - fdocore top level sources directory
FDOCORE = ${HOME}/fdobase
#
# - fdogdal top level sources directory
FDOGDAL = ${FDOCORE}/Providers/GDAL
#
# - fdocore installation prefix (default: /usr/local/fdo-3.3.0)
FDOCOREPREFIX = /usr/local/fdo-3.3.0
#
# - GDAL installation prefix
GDALPREFIX = ${FDOCORE}/Thirdparty/gdal
#
###### END OF CONFIGURATION ######

CXX = g++
CXXFLAGS = -g # -Wall

FDO = ${FDOCORE}/Fdo
FDOUTILITIES = ${FDOCORE}/Utilities
FDOTHIRDPARTY = ${FDOCORE}/ThirdParty

INCLUDES = \
	-I${FDOGDAL}/Inc \
	-I${FDOGDAL}/Src \
	-I$(FDO)/Unmanaged/Inc \
	-I$(FDOUTILITIES)/Common/Inc \
	-I$(FDOTHIRDPARTY)/linux/cppunit/include \
	-I${GDALPREFIX}/include

LIBDIRS = \
	-L${FDOCOREPREFIX}/lib \
	-L${FDOCORE}/Fdo/Unmanaged/Src/.libs \
	-L${FDOGDAL}/Src/.libs \
	-L${GDALPREFIX}/lib

LIBS = \
	-lFDO \
	-lGRFPOverrides \
	-lGRFPProvider \
	-lgdal \
	-lxerces-c \
	-lxalan-c \
	-lxalanMsg

all : fdorastutil

fdorastutil: fdorastutil.cpp
	${CXX} fdorastutil.cpp -o $@ ${CXXFLAGS} ${INCLUDES} ${LIBDIRS} ${LIBS}

clean :
	${RM} ~*
	${RM} *.o
	${RM} fdorastutil
