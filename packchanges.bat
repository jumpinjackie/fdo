@echo off

rem 
rem Copyright (C) 2004-2006  Autodesk, Inc.
rem 
rem This library is free software; you can redistribute it and/or
rem modify it under the terms of version 2.1 of the GNU Lesser
rem General Public License as published by the Free Software Foundation.
rem 
rem This library is distributed in the hope that it will be useful,
rem but WITHOUT ANY WARRANTY; without even the implied warranty of
rem MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
rem Lesser General Public License for more details.
rem 
rem You should have received a copy of the GNU Lesser General Public
rem License along with this library; if not, write to the Free Software
rem Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
rem 

if (%1)==() goto custom_error

if "%1"=="-h"       goto help_show
if "%1"=="-help"    goto help_show
SET CHANGENAME=%1

echo Please wait... Scanning changes!

if exist FilesChanges.log del /F /Q FilesChanges.log
if exist FileListChanges.txt del /F /Q FileListChanges.txt
if exist Read_Me_FilesChanges.txt del /F /Q Read_Me_FilesChanges.txt

echo $(.)>> FilesChanges.log
svn status -q >> FilesChanges.log
pushd Thirdparty
echo $(Thirdparty)>> ..\FilesChanges.log
svn status -q >> ..\FilesChanges.log
popd
pushd Fdo
echo $(Fdo)>> ..\FilesChanges.log
svn status -q >> ..\FilesChanges.log
popd
pushd Utilities
echo $(Utilities)>> ..\FilesChanges.log
svn status -q >> ..\FilesChanges.log
popd
pushd www
echo $(www)>> ..\FilesChanges.log
svn status -q >> ..\FilesChanges.log
popd
pushd Providers\ArcSDE
echo $(ArcSDE)>> ..\..\FilesChanges.log
svn status -q >> ..\..\FilesChanges.log
popd
pushd Providers\GenericRdbms
echo $(GenericRdbms)>> ..\..\FilesChanges.log
svn status -q >> ..\..\FilesChanges.log
popd
pushd Providers\GDAL
echo $(GDAL)>> ..\..\FilesChanges.log
svn status -q >> ..\..\FilesChanges.log
popd
pushd Providers\SDF
echo $(SDF)>> ..\..\FilesChanges.log
svn status -q >> ..\..\FilesChanges.log
popd
pushd Providers\SHP
echo $(SHP)>> ..\..\FilesChanges.log
svn status -q >> ..\..\FilesChanges.log
popd
pushd Providers\WFS
echo $(WFS)>> ..\..\FilesChanges.log
svn status -q >> ..\..\FilesChanges.log
popd
pushd Providers\WMS
echo $(WMS)>> ..\..\FilesChanges.log
svn status -q >> ..\..\FilesChanges.log
popd
echo end>> FilesChanges.log

echo Please wait... Creating zip file!
cscript //Nologo //job:pack packchanges.wsf
echo End action
exit /B 0

:custom_error
echo The command is not recognized.
echo Please use the format:
:help_show
echo *****************************************************************************
echo NOTE: To use the packchanges script, download and install the 7z  
echo       command-line and Windows shell utility for manipulating ZIP 
echo       archives. 7-Zip is free software distributed under the GNU LGPL. 
echo       7z can be obtained from http://www.7-zip.org/
echo *
echo packchanges.bat [-h] [ZipFileName]
echo *
echo Help:          -h[elp]
echo ZipFileName:   ZipFileName name. e.g Change74
echo *
echo The output file will be named [ZipFileName].zip in accordance with the 
echo input argument ZipFileName. This zip will contain all modified/added 
echo files from your Subversion folder. The zip file will also contain a readme 
echo file with all modified/added/deleted files.
echo *****************************************************************************
exit /B 0   