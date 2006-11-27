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

SET DEFMODIFY=no
SET FDOCOREENABLE=yes
SET SHPENABLE=yes
SET SDFENABLE=yes
SET WFSENABLE=yes
SET WMSENABLE=yes
SET ARCSDEENABLE=yes
SET RDBMSENABLE=yes
SET GDALENABLE=yes
SET SHOWHELP=no
SET FDOTARZIPFOLDER=OpenSource_FDO
SET FDOBUILDNUMBER=GXXX
SET FDOSVNROOT=%cd%

:study_params
if (%1)==() goto start_zip

if "%1"=="-help"    goto help_show
if "%1"=="-h"       goto help_show

if "%1"=="-b"       goto get_build
if "%1"=="-build"   goto get_build

if "%1"=="-o"       goto get_output
if "%1"=="-outpath" goto get_output

if "%1"=="-i"       goto get_input
if "%1"=="-inpath"  goto get_input

if "%1"=="-w"       goto get_with
if "%1"=="-with"    goto get_with

goto custom_error

:get_with
if (%2)==() goto custom_error

if "%DEFMODIFY%"=="yes" goto stp1_get_with
	SET DEFMODIFY=yes
    SET FDOCOREENABLE=no
    SET SHPENABLE=no
    SET SDFENABLE=no
    SET WFSENABLE=no
    SET WMSENABLE=no
    SET ARCSDEENABLE=no
    SET RDBMSENABLE=no
    SET GDALENABLE=no
:stp1_get_with
if not "%2"=="sdf" goto stp2_get_with
	SET SDFENABLE=yes
	goto next_param
:stp2_get_with
if not "%2"=="shp" goto stp3_get_with
	SET SHPENABLE=yes
	goto next_param
:stp3_get_with
if not "%2"=="wfs" goto stp4_get_with
	SET WFSENABLE=yes
	goto next_param
:stp4_get_with
if not "%2"=="wms" goto stp5_get_with
	SET WMSENABLE=yes
	goto next_param
:stp5_get_with
if not "%2"=="fdo" goto stp6_get_with
	SET FDOCOREENABLE=yes
	goto next_param
:stp6_get_with
if not "%2"=="rdbms" goto stp7_get_with
	SET RDBMSENABLE=yes
	goto next_param
:stp7_get_with
if not "%2"=="gdal" goto stp8_get_with
	SET GDALENABLE=yes
	goto next_param
:stp8_get_with
if not "%2"=="providers" goto stp9_get_with
    SET FDOCOREENABLE=no
    SET SHPENABLE=yes
    SET SDFENABLE=yes
    SET WFSENABLE=yes
    SET WMSENABLE=yes
    SET ARCSDEENABLE=yes
    SET RDBMSENABLE=yes
    SET GDALENABLE=yes
	goto next_param
:stp9_get_with
if not "%2"=="all" goto custom_error
    SET FDOCOREENABLE=yes
    SET SHPENABLE=yes
    SET SDFENABLE=yes
    SET WFSENABLE=yes
    SET WMSENABLE=yes
    SET ARCSDEENABLE=yes
    SET RDBMSENABLE=yes
    SET GDALENABLE=yes
    goto next_param

:get_build
SET FDOBUILDNUMBER=%2
goto next_param

:get_output
SET FDOTARZIPFOLDER=%2
goto next_param

:get_input
SET FDOSVNROOT=%2
goto next_param

:next_param
shift
shift
goto study_params

:start_zip
:start_zip_fdo
if "%FDOCOREENABLE%"=="no" goto start_zip_shp
   mkdir "%FDOTARZIPFOLDER%"
   svn export "%FDOSVNROOT%" "%FDOTARZIPFOLDER%" --non-recursive --force
   svn export "%FDOSVNROOT%\Fdo" "%FDOTARZIPFOLDER%\Fdo" --force
   svn export "%FDOSVNROOT%\Utilities" "%FDOTARZIPFOLDER%\Utilities" --force
   svn export "%FDOSVNROOT%\Thirdparty" "%FDOTARZIPFOLDER%\Thirdparty" --force
   pushd "%FDOTARZIPFOLDER%"
   if exist .svn del /q /f /s .svn
   popd
   if exist "fdo-3.2.0_%FDOBUILDNUMBER%.zip" del /q /f "fdo-3.2.0_%FDOBUILDNUMBER%.zip"
   7z a -airy -bd -tzip "fdo-3.2.0_%FDOBUILDNUMBER%.zip" "%FDOTARZIPFOLDER%"
   deltree /Y "%FDOTARZIPFOLDER%"
:start_zip_shp
if "%SHPENABLE%"=="no" goto start_zip_sdf
   mkdir "%FDOTARZIPFOLDER%\Providers\SHP"
   svn export "%FDOSVNROOT%\Providers\SHP" "%FDOTARZIPFOLDER%\Providers\SHP" --force
   pushd "%FDOTARZIPFOLDER%"
   if exist .svn del /q /f /s .svn
   popd
   if exist "fdoshp-3.2.0_%FDOBUILDNUMBER%.zip" del /q /f "fdoshp-3.2.0_%FDOBUILDNUMBER%.zip"
   7z a -airy -bd -tzip "fdoshp-3.2.0_%FDOBUILDNUMBER%.zip" "%FDOTARZIPFOLDER%"
   deltree /Y "%FDOTARZIPFOLDER%"
:start_zip_sdf
if "%SDFENABLE%"=="no" goto start_zip_wfs
   mkdir "%FDOTARZIPFOLDER%\Providers\SDF"
   svn export "%FDOSVNROOT%\Providers\SDF" "%FDOTARZIPFOLDER%\Providers\SDF" --force
   pushd "%FDOTARZIPFOLDER%"
   if exist .svn del /q /f /s .svn
   popd
   if exist "fdosdf-3.2.0_%FDOBUILDNUMBER%.zip" del /q /f "fdosdf-3.2.0_%FDOBUILDNUMBER%.zip"
   7z a -airy -bd -tzip "fdosdf-3.2.0_%FDOBUILDNUMBER%.zip" "%FDOTARZIPFOLDER%"
   deltree /Y "%FDOTARZIPFOLDER%"
:start_zip_wfs
if "%WFSENABLE%"=="no" goto start_zip_wms
   mkdir "%FDOTARZIPFOLDER%\Providers\WFS"
   svn export "%FDOSVNROOT%\Providers\WFS" "%FDOTARZIPFOLDER%\Providers\WFS" --force
   pushd "%FDOTARZIPFOLDER%"
   if exist .svn del /q /f /s .svn
   popd
   if exist "fdowfs-3.2.0_%FDOBUILDNUMBER%.zip" del /q /f "fdowfs-3.2.0_%FDOBUILDNUMBER%.zip"
   7z a -airy -bd -tzip "fdowfs-3.2.0_%FDOBUILDNUMBER%.zip" "%FDOTARZIPFOLDER%"
   deltree /Y "%FDOTARZIPFOLDER%"
:start_zip_wms
if "%WMSENABLE%"=="no" goto start_zip_arcsde
   mkdir "%FDOTARZIPFOLDER%\Providers\WMS"
   svn export "%FDOSVNROOT%\Providers\WMS" "%FDOTARZIPFOLDER%\Providers\WMS" --force
   pushd "%FDOTARZIPFOLDER%"
   if exist .svn del /q /f /s .svn
   popd
   if exist "fdowms-3.2.0_%FDOBUILDNUMBER%.zip" del /q /f "fdowms-3.2.0_%FDOBUILDNUMBER%.zip"
   7z a -airy -bd -tzip "fdowms-3.2.0_%FDOBUILDNUMBER%.zip" "%FDOTARZIPFOLDER%"
   deltree /Y "%FDOTARZIPFOLDER%"
:start_zip_arcsde
if "%ARCSDEENABLE%"=="no" goto start_zip_rdbms
   mkdir "%FDOTARZIPFOLDER%\Providers\ArcSDE"
   svn export "%FDOSVNROOT%\Providers\ArcSDE" "%FDOTARZIPFOLDER%\Providers\ArcSDE" --force
   pushd "%FDOTARZIPFOLDER%"
   if exist .svn del /q /f /s .svn
   popd
   if exist "fdoarcsde-3.2.0_%FDOBUILDNUMBER%.zip" del /q /f "fdoarcsde-3.2.0_%FDOBUILDNUMBER%.zip"
   7z a -airy -bd -tzip "fdoarcsde-3.2.0_%FDOBUILDNUMBER%.zip" "%FDOTARZIPFOLDER%"
   deltree /Y "%FDOTARZIPFOLDER%"
:start_zip_rdbms
if "%RDBMSENABLE%"=="no" goto start_zip_gdal
   mkdir "%FDOTARZIPFOLDER%\Providers\GenericRdbms"
   svn export "%FDOSVNROOT%\Providers\GenericRdbms" "%FDOTARZIPFOLDER%\Providers\GenericRdbms" --force
   pushd "%FDOTARZIPFOLDER%"
   if exist .svn del /q /f /s .svn
   popd
   if exist "fdordbms-3.2.0_%FDOBUILDNUMBER%.zip" del /q /f "fdordbms-3.2.0_%FDOBUILDNUMBER%.zip"
   7z a -airy -bd -tzip "fdordbms-3.2.0_%FDOBUILDNUMBER%.zip" "%FDOTARZIPFOLDER%"
   deltree /Y "%FDOTARZIPFOLDER%"
:start_zip_gdal
if "%GDALENABLE%"=="no" goto end
   mkdir "%FDOTARZIPFOLDER%\Providers\GDAL"
   svn export "%FDOSVNROOT%\Providers\GDAL" "%FDOTARZIPFOLDER%\Providers\GDAL" --force
   pushd "%FDOTARZIPFOLDER%"
   if exist .svn del /q /f /s .svn
   popd
   if exist "fdogdal-3.2.0_%FDOBUILDNUMBER%.zip" del /q /f "fdogdal-3.2.0_%FDOBUILDNUMBER%.zip"
   7z a -airy -bd -tzip "fdogdal-3.2.0_%FDOBUILDNUMBER%.zip" "%FDOTARZIPFOLDER%"
   deltree /Y "%FDOTARZIPFOLDER%"

:end
echo End Build
exit /B 0

:error
echo There was a build error.
exit /B 1

:custom_error
echo The command is not recognized.
echo Please use the format:
:help_show
echo **************************************************************************
echo NOTE: To use the createtarzip script, download and install the 7z  
echo       command-line and Windows shell utility for manipulating ZIP 
echo       archives. 7-Zip is free software distributed under the GNU LGPL. 
echo       7z can be obtained from http://www.7-zip.org/
echo *
echo createtarzipfiles.bat [-h] [-i=InFolder] [-o=OutFolder] [-w=WithModule] [-b=BuildNumber]
echo *
echo Help:           -h[elp]
echo InFolder:       -i[npath]=input source svn checkout folder
echo OutFolder:      -o[utpath]=destination folder for exported svn files
echo WithModule:     -w[ith]=all(default), fdo, providers, shp, sdf, wfs, wms, arcsde, rdbms, gdal
echo BuildNumber:    -b[uild]=User-Defined build number appended to the end of the tar.gz files
echo **************************************************************************

exit /B 0

