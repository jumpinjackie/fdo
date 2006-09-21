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
   echo "Invalid FDO path. The setenvironment script sets the default value to: $FDO. Please modify the setenvironment.sh script with a valid path."
fi

export FDOUTILITIES=$PWD/Utilities

if test ! -e "$FDOUTILITIES"; then
   echo "Invalid FDO Utilities path provided. The setenvironment script sets the default value to: $FDOUTILITIES. Please modify the setenvironment.sh script with a valid path."
fi

export FDOTHIRDPARTY=$PWD/Thirdparty

if test ! -e "$FDOTHIRDPARTY"; then
   echo "Invalid FDO Thirdparty path provided. The setenvironment script sets the default value to: $FDOTHIRDPARTY. Please modify the setenvironment.sh script with a valid path."
fi

export SDEHOME=$FDOTHIRDPARTY/ESRI/ArcSDEClient91/Linux

if test ! -e "$SDEHOME"; then
   echo "Invalid OPTIONAL path for ArcSDE SDK files. The setenvironment script sets the default value to: $FDOTHIRDPARTY/ESRI/ArcSDEClient91/Linux. OPTIONALLY modify the setenvironment.sh script with a valid path."
fi

export FDOMYSQL=$FDOTHIRDPARTY/mysql/rhlinux

if test ! -e "$FDOMYSQL"; then
   echo "Invalid OPTIONAL path for MySQL SDK files. The setenvironment script sets the default value to: $FDOTHIRDPARTY/mysql/rhlinux. OPTIONALLY modify the setenvironment.sh script with a valid path."
fi

export FDOODBC=/usr

if test ! -e "$FDOODBC"; then
   echo "Invalid OPTIONAL path for ODBC SDK files. The setenvironment script sets the default value to: $FDOODBC. OPTIONALLY modify the setenvironment.sh script with a valid path."
fi

mkdir -p "/usr/local/fdo-3.2.0/lib"

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/fdo-3.2.0/lib

