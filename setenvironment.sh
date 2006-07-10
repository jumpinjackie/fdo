#!/bin/bash

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

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/fdo-3.2.0/lib:$FDOTHIRDPARTY/apache/xml-xalan/c/lib:$FDOTHIRDPARTY/apache/xml-xerces/c/lib

