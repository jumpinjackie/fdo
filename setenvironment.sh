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
   echo "Invalid FDO path. The setenvironment script sets the default "
   echo "value to: $FDO. Please modify the setenvironment.sh script "
   echo "with a valid path."
   echo ""
fi

export FDOUTILITIES=$PWD/Utilities

if test ! -e "$FDOUTILITIES"; then
   echo "Invalid FDO Utilities path provided. The setenvironment script "
   echo "sets the default value to: $FDOUTILITIES. Please modify the "
   echo "setenvironment.sh script with a valid path."
   echo ""
fi

export FDOTHIRDPARTY=$PWD/Thirdparty

if test ! -e "$FDOTHIRDPARTY"; then
   echo "Invalid FDO Thirdparty path provided. The setenvironment script "
   echo "sets the default value to: $FDOTHIRDPARTY. Please modify the "
   echo "setenvironment.sh script with a valid path."
   echo ""
fi

export SDEHOME=$FDOTHIRDPARTY/ESRI/ArcSDEClient91/Linux

if test ! -e "$SDEHOME"; then
   echo "OPTIONAL: The default location for the ArcSDE client SDK"
   echo "files was not found. The setenvironment script sets the default"
   echo "value to: $FDOTHIRDPARTY/ESRI/ArcSDEClient91/Linux. If you need to "
   echo "build the FDO ArcSDE Provider, modify the setenvironment.sh script "
   echo "with a valid path and set $SDEHOME to point to the location of the "
   echo "ArcSDE client installation."
   echo ""
fi

export FDOMYSQL=$FDOTHIRDPARTY/mysql/rhlinux

if test ! -e "$FDOMYSQL"; then
   echo "OPTIONAL: The default location for the MySQL SDK files "
   echo "was not found. The setenvironment script sets the default value to: "
   echo "$FDOTHIRDPARTY/mysql/rhlinux. If you need to build the FDO MySQL "
   echo "Provider, and have chosen not to install the MySQL SDK using the"
   echo "recommended RPM installtion process, modify the setenvironment.sh "
   echo "script and set $FDOMYSQL to point to the location of the MySQL client"
   echo "installation. If you have chosen to install the MySQL client using "
   echo "the RPM install, this environment variable does not need to be set"
   echo ""
fi

export FDOODBC=/usr

if test ! -e "$FDOODBC"; then
   echo "OPTIONAL: The default path for the ODBC SDK files was not found. "
   echo "The setenvironment script sets the default value to: $FDOODBC."
   echo "If you need to build the ODBC prvovider, modify the setenvironment.sh "
   echo "script and set $FDOODBC to point to the location of the ODBC SDK"
   echo ""
fi

mkdir -p "/usr/local/fdo-3.2.0/lib"

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/fdo-3.2.0/lib

