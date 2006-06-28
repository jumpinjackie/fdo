#!/bin/bash

if test ! -e "$PWD/Fdo"; then
   echo "Invalid FDO path. The setenvironment script sets the default value to: $PWD/Fdo. Please modify the setenvironment.sh script with a valid path."
fi

if test ! -e "$PWD/Utilities"; then
   echo "Invalid FDO Utilities path provided. The setenvironment script sets the default value to: $PWD/Utilities. Please modify the setenvironment.sh script with a valid path."
fi

if test ! -e "$PWD/Thirdparty"; then
   echo "Invalid FDO Thirdparty path provided. The setenvironment script sets the default value to: $PWD/Thirdparty. Please modify the setenvironment.sh script with a valid path."
fi

if test ! -e "$FDOTHIRDPARTY/ESRI/ArcSDEClient91/Linux"; then
   echo "Invalid OPTIONAL path for ArcSDE SDK files. The setenvironment script sets the default value to: $FDOTHIRDPARTY/ESRI/ArcSDEClient91/Linux. OPTIONALLY modify the setenvironment.sh script with a valid path."
fi

if test ! -e "$FDOTHIRDPARTY/mysql/rhlinux"; then
   echo "Invalid OPTIONAL path for MySQL SDK files. The setenvironment script sets the default value to: $FDOTHIRDPARTY/mysql/rhlinux. OPTIONALLY modify the setenvironment.sh script with a valid path."
fi

if test ! -e "/usr"; then
   echo "Invalid OPTIONAL path for ODBC SDK files. The setenvironment script sets the default value to: /usr. OPTIONALLY modify the setenvironment.sh script with a valid path."
fi

export FDO=$PWD/Fdo
export FDOTHIRDPARTY=$PWD/Thirdparty
export FDOUTILITIES=$PWD/Utilities
export SDEHOME=$FDOTHIRDPARTY/ESRI/ArcSDEClient91/Linux
export FDOMYSQL=$FDOTHIRDPARTY/mysql/rhlinux
export FDOODBC=/usr

mkdir -p "/usr/local/fdo-3.2.0/lib"

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/fdo-3.2.0/lib:$FDOTHIRDPARTY/linux/cppunit/lib:$FDOTHIRDPARTY/apache/xml-xalan/c/lib:$FDOTHIRDPARTY/apache/xml-xerces/c/lib

