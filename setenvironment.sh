#!/bin/bash

export FDO=$PWD/Fdo
export FDOTHIRDPARTY=$PWD/Thirdparty
export FDOUTILITIES=$PWD/Utilities
export SDEHOME=$FDOTHIRDPARTY/ESRI/ArcSDEClient91/Linux
export FDOMYSQL=$FDOTHIRDPARTY/mysql/rhlinux
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/fdo-3.2.0/lib:$FDOTHIRDPARTY/linux/cppunit/lib:$(FDOTHIRDPARTY)/apache/xml-xalan/c/lib:$FDOTHIRDPARTY/apache/xml-xerces/c/lib