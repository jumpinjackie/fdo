#!/bin/sh

##########################################
## build third party libraries for fdo: ##
## apache, linux/cppunit                ##
##########################################

pushd . >& /dev/null
cd apache
echo Building apche
./build
popd >& /dev/null

pushd . >& /dev/null
cd linux/cppunit
echo Building linux/cppunit
./build
popd >& /dev/null

