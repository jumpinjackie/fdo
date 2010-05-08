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

# Filename: build_linux.sh
#
# Usage:    ./build_linux.sh
#
# Description: Build the Linux version of the Python wrappers for FDO
#              This script assumes that the following software packages are installed:
#              - SWIG 1.3.24
#              - Python 2.4

#
# GLOBALS
#

#
# Merge the standard FDO header file (found in /usr/local/fdo-3.6.0/include/)
# with the local 'overriding' header files (found in ./Fdo_Inc).  The merged
# files will be put in ./Inc_Merged
#
echo "Creating the ./Inc_Merged directory...";
rm -rf ./Inc_Merged >& /dev/null
if [ $? -ne 0 ]
then
	echo "Error: Cannot delete ./Inc_merged.";
	echo "Exiting.";
	exit 1;
fi

mkdir Inc_Merged >& /dev/null
if [ $? -ne 0 ]
then
	echo "Error: Unable to mkdir Inc_Merged.";
	echo "Exiting.";
	exit 1;
fi 	

cp -R $FDO/Unmanaged/Inc/* ./Inc_Merged/. >& /dev/null
if [ $? -ne 0 ]
then
	echo "Error: Unable to copy $FDO/Unmanaged/Inc to ./Inc_Merged";
	echo "Exiting.";
	exit 1;
fi

chmod -R +w ./Inc_Merged/* >& /dev/null
if [ $? -ne 0 ]
then
	echo "Error: Unable to chmod 644 on ./Inc_Merged/*";
	echo "Exiting.";
	exit 1;
fi

cp -R ./Fdo_Inc/* ./Inc_Merged/. >& /dev/null
if [ $? -ne 0 ]
then
	echo "Error: Unable to copy ./Fdo_Inc/. to ./Inc_Merged";
	echo "Exiting.";
	exit 1;
fi


#
# Execute the makefiles to build the wrappers
#
pushd . >& /dev/null
echo "Building FDO wrappers for Python.  This will take a few minutes..."

#
# List of Makefiles.  If you have a new provider makefile, add it here.
#
makeFileNames=(Makefile) 

#
# Execute each makefile
#
for makeFileName in ${makeFileNames[@]}; do
	echo "Running make on '$makeFileName'..."
	make -f $makeFileName
	make -f $makeFileName install
	if [ $? -ne 0 ]
	then
		echo "Error: Building Makefile '$makeFileName' failed.";
		echo "Exiting.";
		exit 1;
	fi
done

popd >& /dev/null

echo "Script completed."
