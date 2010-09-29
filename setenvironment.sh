#!/bin/bash

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

# Fully-qualfied location of the FDO files
export FDO=$PWD/Fdo
if test ! -e "$FDO"; then
   echo ""
   echo "Invalid FDO path provided. "
   echo "The setenvironment script sets the default value to: "
   echo "$FDO"
   echo "Please modify the setenvironment.sh script with a valid path."
   echo ""
fi

# Fully-qualfied location of the FDO Utility files
export FDOUTILITIES=$PWD/Utilities
if test ! -e "$FDOUTILITIES"; then
   echo ""
   echo "Invalid FDO Utilities path provided. "
   echo "The setenvironment script sets the default value to: "
   echo "$FDOUTILITIES" 
   echo "Please modify the setenvironment.sh script with a valid path."
   echo ""
fi

# Fully-qualfied location of the FDO Thirdparty files
export FDOTHIRDPARTY=$PWD/Thirdparty
if test ! -e "$FDOTHIRDPARTY"; then
   echo ""
   echo "Invalid FDO Thirdparty path provided. "
   echo "The setenvironment script sets the default value to: "
   echo "$FDOTHIRDPARTY"
   echo "Please modify the setenvironment.sh script with a valid path."
   echo ""
fi

# Fully-qualfied location of the ESRI ArcSDE SDK
export SDEHOME=$FDOTHIRDPARTY/ESRI/ArcSDEClient91/Linux
if test ! -e "$SDEHOME"; then
   echo ""
   echo "NOTE: The default location for the ArcSDE client SDK files"
   echo "was not found. The setenvironment script sets the default value to: "
   echo "$FDOTHIRDPARTY/ESRI/ArcSDEClient91/Linux. "
   echo "If you need to build the FDO ArcSDE Provider, modify the "
   echo "setenvironment.sh script with a valid path and set SDEHOME to "
   echo "point to the location of the ArcSDE client installation."
   echo ""
fi

# Fully-qualfied location of the GDAL Installation
export FDOGDAL=$FDOTHIRDPARTY/gdal
echo ""
echo "NOTE: The setenvironment.sh script sets the installation location for "
echo "the GDAL SDK files to $FDOTHIRDPARTY/gdal. "
echo "If this value remains unchanged, the FDO build process will"
echo "build the version of GDAL located in Thirdparty/gdal and will "
echo "install the resulting libraries in /usr/local/fdo-3.6.0. The FDO build"
echo "process will then use that location when building the GDAL and"
echo "WMS providers. If you wish to build the FDO GDAL or WMS Providers"
echo "using a previously installed version of GDAL, modify the setenvironment.sh "
echo "script and set FDOGDAL to point to the existing GDAL installation."
echo "For example: /user/local (The default GDAL installation path)."
echo ""

# Fully-qualfied location of the MySQL SDK
export FDOMYSQL=$FDOTHIRDPARTY/mysql/rhlinux
if test ! -e "$FDOMYSQL"; then
   echo ""
   echo "NOTE: The default location for the MySQL SDK files "
   echo "was not found. The setenvironment script sets the default value to: "
   echo "$FDOTHIRDPARTY/mysql/rhlinux "
   echo "If you need to build the FDO MySQL Provider, and have chosen not "
   echo "to install the MySQL SDK using the recommended RPM installtion "
   echo "process, modify the setenvironment.sh script and set FDOMYSQL to point"
   echo "to the location of the MySQL client installation. If you have chosen "
   echo "to install the MySQL client using the RPM install, this environment "
   echo "variable does not need to be set and this warning can be ignored."
   echo ""
fi

# Fully-qualfied location of the ODBC SDK
export FDOODBC=/usr
if test ! -e "$FDOODBC"; then
   echo ""
   echo "NOTE: The default path for the ODBC SDK files was not found. "
   echo "The setenvironment script sets the default value to: "
   echo "$FDOODBC"
   echo "If you need to build the ODBC prvovider, modify the setenvironment.sh "
   echo "script and set FDOODBC to point to the location of the ODBC SDK."
   echo ""
fi

# Location of the PYTHON lib files. Typically in /usr/lib/pythonXXX
export PYTHON_LIB_PATH=/usr/lib/python2.4
if test ! -e "$PYTHON_LIB_PATH"; then
   echo ""
   echo "NOTE: The default path for the Python SDK lib files was not found. "
   echo "The setenvironment script sets the default value to: "
   echo "$PYTHON_LIB_PATH"
   echo "If you need to build the FDO Python wrappers, modify the setenvironment.sh "
   echo "script and set PYTHON_LIB_PATH to point to the location of the Python "
   echo "lib files."
   echo ""
fi

# Location of the PYTHON include files. Typically in /usr/include/pythonXXX
export PYTHON_INCLUDE_PATH=/usr/include/python2.4
if test ! -e "$PYTHON_INCLUDE_PATH"; then
   echo ""
   echo "NOTE: The default path for the Python SDK header files was not found. "
   echo "The setenvironment script sets the default value to: "
   echo "$PYTHON_INCLUDE_PATH"
   echo "If you need to build the FDO Python wrappers, modify the setenvironment.sh "
   echo "script and set PYTHON_INCLUDE_PATH to point to the location of the Python "
   echo "include files."
   echo ""
fi

# Buildbot hack (mloskot): if the script is called with single dummy
# parameter no installation directory is created, ie.:
# $ source ./setenvironment.sh --noinstall
if test ! $# -eq 1; then

	mkdir -p "/usr/local/fdo-3.6.0/lib"

	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/fdo-3.6.0/lib:$SDEHOME/lib
fi

export XERCESCROOT=$FDOTHIRDPARTY/apache/xerces
export XALANCROOT=$FDOTHIRDPARTY/apache/xalan
export NLSDIR=$XALANCROOT/src/xalanc/NLS

