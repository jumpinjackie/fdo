#!/bin/csh

##########################################
## build third party libraries for SDF: ##
## Sqlite3.1.5                          ##
##########################################

pushd . >& /dev/null
cd Sqlite3.1.5
echo Building Sqlite3.1.5
cd Src
make
popd >& /dev/null

