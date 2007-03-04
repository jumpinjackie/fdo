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

export FDO=$PWD/Fdo

if test ! -e "$FDO"; then
   echo ""
   echo "Invalid FDO path provided. "
   echo "The setenvironment script sets the default value to: "
   echo "$FDO"
   echo "Please modify the setenvironment.sh script with a valid path."
   echo ""
fi

export FDOUTILITIES=$PWD/Utilities

if test ! -e "$FDOUTILITIES"; then
   echo ""
   echo "Invalid FDO Utilities path provided. "
   echo "The setenvironment script sets the default value to: "
   echo "$FDOUTILITIES" 
   echo "Please modify the setenvironment.sh script with a valid path."
   echo ""
fi

export FDOTHIRDPARTY=$PWD/Thirdparty

if test ! -e "$FDOTHIRDPARTY"; then
   echo ""
   echo "Invalid FDO Thirdparty path provided. "
   echo "The setenvironment script sets the default value to: "
   echo "$FDOTHIRDPARTY"
   echo "Please modify the setenvironment.sh script with a valid path."
   echo ""
fi

export SDEHOME=$FDOTHIRDPARTY/ESRI/ArcSDEClient91/Linux

if test ! -e "$SDEHOME"; then
   echo ""
   echo "OPTIONAL: The default location for the ArcSDE client SDK files"
   echo "was not found. The setenvironment script sets the default value to: "
   echo "$FDOTHIRDPARTY/ESRI/ArcSDEClient91/Linux. "
   echo "If you need to build the FDO ArcSDE Provider, modify the "
   echo "setenvironment.sh script with a valid path and set SDEHOME to "
   echo "point to the location of the ArcSDE client installation."
   echo ""
fi

export FDOGDAL=$FDOTHIRDPARTY/gdal

if test ! -e "$FDOGDAL"; then
   echo ""
   echo "OPTIONAL: The default location for the GDAL SDK files"
   echo "was not found. The setenvironment script sets the default value to: "
   echo "$FDOTHIRDPARTY/gdal. "
   echo "If you need to build the FDO GDAL or WMS Providers, modify the "
   echo "setenvironment.sh script with a valid path and set FDOGDAL to "
   echo "point to the location of the GDAL SDK files."
   echo ""
fi

export FDOMYSQL=$FDOTHIRDPARTY/mysql/rhlinux

if test ! -e "$FDOMYSQL"; then
   echo ""
   echo "OPTIONAL: The default location for the MySQL SDK files "
   echo "was not found. The setenvironment script sets the default value to: "
   echo "$FDOTHIRDPARTY/mysql/rhlinux "
   echo "If you need to build the FDO MySQL Provider, and have chosen not "
   echo "to install the MySQL SDK using the recommended RPM installtion "
   echo "process, modify the setenvironment.sh script and set FDOMYSQL to point"
   echo "to the location of the MySQL client installation. If you have chosen "
   echo "to install the MySQL client using the RPM install, this environment "
   echo "variable does not need to be set"
   echo ""
fi

export FDOODBC=/usr

if test ! -e "$FDOODBC"; then
   echo ""
   echo "OPTIONAL: The default path for the ODBC SDK files was not found. "
   echo "The setenvironment script sets the default value to: "
   echo "$FDOODBC"
   echo "If you need to build the ODBC prvovider, modify the setenvironment.sh "
   echo "script and set FDOODBC to point to the location of the ODBC SDK"
   echo ""
fi

mkdir -p "/usr/local/fdo-3.3.0/lib"

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/fdo-3.3.0/lib:$SDEHOME/lib

