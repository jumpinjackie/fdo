@echo off

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
pushd Providers
echo $(Providers)>> ..\FilesChanges.log
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
echo packchanges.bat [-h] [ZipFileName]
echo *
echo Help:          -h[elp]
echo ZipFileName:   ZipFileName name e.g. ZipFDO10
echo *
echo In order to use this script, you will need to download and install the 7-Zip
echo Download Location:             http://www.7-zip.org
echo Following the installation, ensure that in the environment's PATH you have:
echo      7-Zip  -- e.g.          c:\Program Files\7-Zip
echo      e.g.:                   set PATH=%PATH%;"c:\Program Files\7-Zip"
echo The output file will be ZipFDO10.zip
echo This zip will contain all modified/added files from your SubVersion folder
echo Also it will contain a readme file with all modified/added/deleted files.
echo *****************************************************************************
exit /B 0   