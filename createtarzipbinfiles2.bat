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
SET KINGORACLEENABLE=yes
SET OGRENABLE=yes
SET POSTGISENABLE=yes
SET SQLITEENABLE=yes
SET MYSQLENABLE=yes
SET ODBCENABLE=yes
SET SQLSPATIALENABLE=yes
SET SHOWHELP=no
SET FDOTARZIPFOLDER=c:\OpenSource_FDO
SET FDOBUILDNUMBER=FXXX
SET FDORELNUMBER=3.6.0
SET FDOZIPTEMP=7zTemp
SET FILEPPLATFORMPREFIX=Win32

:study_params
if (%1)==() goto start_zip

if "%1"=="-help"    goto help_show
if "%1"=="-h"       goto help_show

if "%1"=="-r"         goto get_release
if "%1"=="-release"   goto get_release

if "%1"=="-b"       goto get_build
if "%1"=="-build"   goto get_build

if "%1"=="-o"       goto get_output
if "%1"=="-outpath" goto get_output

if "%1"=="-i"       goto get_input
if "%1"=="-inpath"  goto get_input

if "%1"=="-p"           goto get_platform
if "%1"=="-platform"    goto get_platform

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
    SET KINGORACLEENABLE=no
    SET OGRENABLE=no
    SET POSTGISENABLE=no
    SET SQLITEENABLE=no
    SET MYSQLENABLE=no
    SET ODBCENABLE=no
    SET SQLSPATIALENABLE=no
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
if not "%2"=="kingoracle" goto stp9_get_with
    SET KINGORACLEENABLE=yes
    goto next_param
:stp9_get_with
if not "%2"=="ogr" goto stp10_get_with
    SET OGRENABLE=yes
    goto next_param
:stp10_get_with
if not "%2"=="postgis" goto stp11_get_with
    SET POSTGISENABLE=yes
    goto next_param
:stp11_get_with
if not "%2"=="sqlite" goto stp12_get_with
    SET SQLITEENABLE=yes
    goto next_param
:stp12_get_with
if not "%2"=="mysql" goto stp13_get_with
    SET MYSQLENABLE=yes
    goto next_param
:stp13_get_with
if not "%2"=="odbc" goto stp14_get_with
    SET ODBCENABLE=yes
    goto next_param
:stp14_get_with
if not "%2"=="sqlspatial" goto stp15_get_with
    SET SQLSPATIALENABLE=yes
    goto next_param
:stp15_get_with
if not "%2"=="providers" goto stp16_get_with
    SET FDOCOREENABLE=no
    SET SHPENABLE=yes
    SET SDFENABLE=yes
    SET WFSENABLE=yes
    SET WMSENABLE=yes
    SET ARCSDEENABLE=yes
    SET RDBMSENABLE=yes
    SET GDALENABLE=yes
    SET KINGORACLEENABLE=yes
    SET OGRENABLE=yes
    SET POSTGISENABLE=yes
    SET SQLITEENABLE=yes
    SET MYSQLENABLE=yes
    SET ODBCENABLE=yes
    SET SQLSPATIALENABLE=yes
    goto next_param
:stp16_get_with
if not "%2"=="all" goto custom_error
    SET FDOCOREENABLE=yes
    SET SHPENABLE=yes
    SET SDFENABLE=yes
    SET WFSENABLE=yes
    SET WMSENABLE=yes
    SET ARCSDEENABLE=yes
    SET RDBMSENABLE=yes
    SET GDALENABLE=yes
    SET KINGORACLEENABLE=yes
    SET OGRENABLE=yes
    SET POSTGISENABLE=yes
    SET SQLITEENABLE=yes
    SET MYSQLENABLE=yes
    SET ODBCENABLE=yes
    SET SQLSPATIALENABLE=yes
    goto next_param

:get_release
SET FDORELNUMBER=%2
goto next_param

:get_platform
SET TYPEPLATFORM=%2
if "%2"=="Win32" goto next_param
if "%2"=="x64" goto next_param
goto custom_error

:get_build
SET FDOBUILDNUMBER=%2
goto next_param

:get_output
SET FDOTARZIPFOLDER=%2
mkdir %FDOTARZIPFOLDER%
goto next_param

:get_input
SET FDOROOT=%2
goto next_param

:next_param
shift
shift
goto study_params

:start_zip

if "%TYPEPLATFORM%"=="Win32" SET FILEPPLATFORMPREFIX=win32
if "%TYPEPLATFORM%"=="x64" SET FILEPPLATFORMPREFIX=win64


:start_zip_fdo
if "%FDOCOREENABLE%"=="no" goto start_zip_shp
   mkdir %FDOZIPTEMP%
   mkdir %FDOZIPTEMP%\Bin
   mkdir %FDOZIPTEMP%\Bin\com
   copy "%FDOROOT%\Bin\providers.xml" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\xerces-c_3_1.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\Xalan-C_1_11.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\XalanMessages_1_11.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\boost_date_time-vc90-mt-1_42.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\boost_thread-vc90-mt-1_42.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\gdal17.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\FDOMessage.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\FDOCommon.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\FDOGeometry.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\FDOSpatial.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\FDO.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\OSGeo.FDO.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\OSGeo.FDO.Common.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\OSGeo.FDO.Geometry.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\ExpressionEngine.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\OWS.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\com\fdo_sys.sql" %FDOZIPTEMP%\Bin\com\
   copy "%FDOROOT%\Bin\com\fdo_sys_idx.sql" %FDOZIPTEMP%\Bin\com\
   copy "%FDOROOT%\Bin\com\fdosys_sys.sql" %FDOZIPTEMP%\Bin\com\
   pushd "%FDOZIPTEMP%"
   if exist "fdo-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar" del /q /f "fdo-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar"
   if exist "fdo-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar.gz" del /q /f "fdo-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar.gz"
   tar -cvf "fdo-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar" Bin
   gzip -9 "fdo-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar"
   cp "fdo-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar.gz" "%FDOTARZIPFOLDER%"\
   popd
   deltree /Y "%FDOZIPTEMP%"
:start_zip_shp
if "%SHPENABLE%"=="no" goto start_zip_sdf
   mkdir %FDOZIPTEMP%
   mkdir %FDOZIPTEMP%\Bin
   copy "%FDOROOT%\Bin\SHPMessage.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\SHPOverrides.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\SHPProvider.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\OSGeo.FDO.Providers.SHP.Overrides.dll" %FDOZIPTEMP%\Bin\
   pushd "%FDOZIPTEMP%"
   if exist "fdoshp-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar" del /q /f "fdoshp-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar"
   if exist "fdoshp-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar.gz" del /q /f "fdoshp-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar.gz"
   tar -cf "fdoshp-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar" Bin
   gzip -9 "fdoshp-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar"
   cp "fdoshp-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar.gz" "%FDOTARZIPFOLDER%"\
   popd
   deltree /Y "%FDOZIPTEMP%"
:start_zip_sdf
if "%SDFENABLE%"=="no" goto start_zip_wfs
   mkdir %FDOZIPTEMP%
   mkdir %FDOZIPTEMP%\Bin
   copy "%FDOROOT%\Bin\SDFMessage.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\SDFProvider.dll" %FDOZIPTEMP%\Bin\
   pushd "%FDOZIPTEMP%"
   if exist "fdosdf-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar" del /q /f "fdosdf-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar"
   if exist "fdosdf-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar.gz" del /q /f "fdosdf-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar.gz"
   tar -cf "fdosdf-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar" Bin
   gzip -9 "fdosdf-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar"
   cp "fdosdf-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar.gz" "%FDOTARZIPFOLDER%"\
   popd
   deltree /Y "%FDOZIPTEMP%"
:start_zip_wfs
if "%WFSENABLE%"=="no" goto start_zip_wms
   mkdir %FDOZIPTEMP%
   mkdir %FDOZIPTEMP%\Bin
   copy "%FDOROOT%\Bin\WFSMessage.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\WFSProvider.dll" %FDOZIPTEMP%\Bin\
   pushd "%FDOZIPTEMP%"
   if exist "fdowfs-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar" del /q /f "fdowfs-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar"
   if exist "fdowfs-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar.gz" del /q /f "fdowfs-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar.gz"
   tar -cf "fdowfs-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar" Bin
   gzip -9 "fdowfs-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar"
   cp "fdowfs-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar.gz" "%FDOTARZIPFOLDER%"\
   popd
   deltree /Y "%FDOZIPTEMP%"
:start_zip_wms
if "%WMSENABLE%"=="no" goto start_zip_arcsde
   mkdir %FDOZIPTEMP%
   mkdir %FDOZIPTEMP%\Bin
   copy "%FDOROOT%\Bin\WMSMessage.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\WMSOverrides.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\WMSProvider.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\OSGeo.FDO.Providers.WMS.Overrides.dll" %FDOZIPTEMP%\Bin\
   pushd "%FDOZIPTEMP%"
   if exist "fdowms-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar" del /q /f "fdowms-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar"
   if exist "fdowms-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar.gz" del /q /f "fdowms-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar.gz"
   tar -cf "fdowms-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar" Bin
   gzip -9 "fdowms-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar"
   cp "fdowms-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar.gz" "%FDOTARZIPFOLDER%"\
   popd
   deltree /Y "%FDOZIPTEMP%"
:start_zip_arcsde
if "%ARCSDEENABLE%"=="no" goto start_zip_rdbms
   mkdir %FDOZIPTEMP%
   mkdir %FDOZIPTEMP%\Bin
   copy "%FDOROOT%\Bin\ArcSDEMessage.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\ArcSDEProvider91.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\ArcSDEProvider92.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\ArcSDEProvider.dll" %FDOZIPTEMP%\Bin\
   pushd "%FDOZIPTEMP%"
   if exist "fdoarcsde-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar" del /q /f "fdoarcsde-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar"
   if exist "fdoarcsde-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar.gz" del /q /f "fdoarcsde-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar.gz"
   tar -cf "fdoarcsde-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar" Bin
   gzip -9 "fdoarcsde-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar"
   cp "fdoarcsde-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar.gz" "%FDOTARZIPFOLDER%"\
   popd
   deltree /Y "%FDOZIPTEMP%"
:start_zip_rdbms
if "%RDBMSENABLE%"=="no" goto start_zip_mysql
   mkdir %FDOZIPTEMP%
   mkdir %FDOZIPTEMP%\Bin
   copy "%FDOROOT%\Bin\RdbmsMsg.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\SmMessage.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\RdbmsOverrides.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\ODBCOverrides.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\ODBCProvider.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\MySQLOverrides.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\MySQLProvider.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\SQLServerSpatialOverrides.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\SQLServerSpatialProvider.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\OSGeo.FDO.Providers.SQLServerSpatial.Overrides.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\OSGeo.FDO.Providers.Rdbms.Overrides.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\OSGeo.FDO.Providers.MySQL.Overrides.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\OSGeo.FDO.Providers.ODBC.Overrides.dll" %FDOZIPTEMP%\Bin\
   pushd "%FDOZIPTEMP%"
   if exist "fdordbms-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar" del /q /f "fdordbms-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar"
   if exist "fdordbms-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar.gz" del /q /f "fdordbms-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar.gz"
   tar -cf "fdordbms-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar" Bin
   gzip -9 "fdordbms-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar"
   cp "fdordbms-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar.gz" "%FDOTARZIPFOLDER%"\
   popd
   deltree /Y "%FDOZIPTEMP%"
:start_zip_mysql
if "%MYSQLENABLE%"=="no" goto start_zip_odbc
   mkdir %FDOZIPTEMP%
   mkdir %FDOZIPTEMP%\Bin
   copy "%FDOROOT%\Bin\RdbmsMsg.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\SmMessage.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\RdbmsOverrides.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\MySQLOverrides.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\MySQLProvider.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\OSGeo.FDO.Providers.Rdbms.Overrides.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\OSGeo.FDO.Providers.MySQL.Overrides.dll" %FDOZIPTEMP%\Bin\
   pushd "%FDOZIPTEMP%"
   if exist "fdomysql-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar" del /q /f "fdomysql-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar"
   if exist "fdomysql-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar.gz" del /q /f "fdomysql-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar.gz"
   tar -cf "fdomysql-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar" Bin
   gzip -9 "fdomysql-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar"
   cp "fdomysql-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar.gz" "%FDOTARZIPFOLDER%"\
   popd
   deltree /Y "%FDOZIPTEMP%"
:start_zip_odbc
if "%ODBCENABLE%"=="no" goto start_zip_sqlspatial
   mkdir %FDOZIPTEMP%
   mkdir %FDOZIPTEMP%\Bin
   copy "%FDOROOT%\Bin\RdbmsMsg.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\SmMessage.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\RdbmsOverrides.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\ODBCOverrides.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\ODBCProvider.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\OSGeo.FDO.Providers.Rdbms.Overrides.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\OSGeo.FDO.Providers.ODBC.Overrides.dll" %FDOZIPTEMP%\Bin\
   pushd "%FDOZIPTEMP%"
   if exist "fdoodbc-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar" del /q /f "fdoodbc-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar"
   if exist "fdoodbc-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar.gz" del /q /f "fdoodbc-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar.gz"
   tar -cf "fdoodbc-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar" Bin
   gzip -9 "fdoodbc-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar"
   cp "fdoodbc-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar.gz" "%FDOTARZIPFOLDER%"\
   popd
   deltree /Y "%FDOZIPTEMP%"
:start_zip_sqlspatial
if "%SQLSPATIALENABLE%"=="no" goto start_zip_gdal
   mkdir %FDOZIPTEMP%
   mkdir %FDOZIPTEMP%\Bin
   copy "%FDOROOT%\Bin\RdbmsMsg.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\SmMessage.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\RdbmsOverrides.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\SQLServerSpatialOverrides.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\SQLServerSpatialProvider.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\OSGeo.FDO.Providers.SQLServerSpatial.Overrides.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\OSGeo.FDO.Providers.Rdbms.Overrides.dll" %FDOZIPTEMP%\Bin\
   pushd "%FDOZIPTEMP%"
   if exist "fdosqlspatial-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar" del /q /f "fdosqlspatial-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar"
   if exist "fdosqlspatial-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar.gz" del /q /f "fdosqlspatial-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar.gz"
   tar -cf "fdosqlspatial-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar" Bin
   gzip -9 "fdosqlspatial-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar"
   cp "fdosqlspatial-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar.gz" "%FDOTARZIPFOLDER%"\
   popd
   deltree /Y "%FDOZIPTEMP%"
:start_zip_gdal
if "%GDALENABLE%"=="no" goto start_zip_king_oracle
   mkdir %FDOZIPTEMP%
   mkdir %FDOZIPTEMP%\Bin
   copy "%FDOROOT%\Bin\GRFPMessage.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\GRFPOverrides.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\GRFPProvider.dll" %FDOZIPTEMP%\Bin\
   pushd "%FDOZIPTEMP%"
   if exist "fdogdal-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar" del /q /f "fdogdal-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar"
   if exist "fdogdal-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar.gz" del /q /f "fdogdal-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar.gz"
   tar -cf "fdogdal-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar" Bin
   gzip -9 "fdogdal-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar"
   cp "fdogdal-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar.gz" "%FDOTARZIPFOLDER%"\
   popd
   deltree /Y "%FDOZIPTEMP%"
:start_zip_king_oracle
if "%KINGORACLEENABLE%"=="no" goto start_zip_postgis
   mkdir %FDOZIPTEMP%
   mkdir %FDOZIPTEMP%\Bin
   copy "%FDOROOT%\Bin\KingOracleMessage.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\KingOracleOverrides.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\KingOracleProvider.dll" %FDOZIPTEMP%\Bin\
   pushd "%FDOZIPTEMP%"
   if exist "fdokingoracle-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar" del /q /f "fdokingoracle-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar"
   if exist "fdokingoracle-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar.gz" del /q /f "fdokingoracle-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar.gz"
   tar -cf "fdokingoracle-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar" Bin
   gzip -9 "fdokingoracle-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar"
   cp "fdokingoracle-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar.gz" "%FDOTARZIPFOLDER%"\
   popd
   deltree /Y "%FDOZIPTEMP%"
:start_zip_postgis
if "%POSTGISENABLE%"=="no" goto start_zip_ogr
   mkdir %FDOZIPTEMP%
   mkdir %FDOZIPTEMP%\Bin
   copy "%FDOROOT%\Bin\PostGISMessage.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\PostGISOverrides.dll" %FDOZIPTEMP%\Bin\
   copy "%FDOROOT%\Bin\PostGISProvider.dll" %FDOZIPTEMP%\Bin\
   pushd "%FDOZIPTEMP%"
   if exist "fdopostgis-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar" del /q /f "fdopostgis-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar"
   if exist "fdopostgis-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar.gz" del /q /f "fdopostgis-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar.gz"
   tar -cf "fdopostgis-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar" Bin
   gzip -9 "fdopostgis-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar"
   cp "fdopostgis-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar.gz" "%FDOTARZIPFOLDER%"\
   popd
   deltree /Y "%FDOZIPTEMP%"
:start_zip_ogr
if "%OGRENABLE%"=="no" goto start_zip_sqlite
   mkdir %FDOZIPTEMP%
   mkdir %FDOZIPTEMP%\Bin
   copy "%FDOROOT%\Bin\OGRProvider.dll" %FDOZIPTEMP%\Bin\
   pushd "%FDOZIPTEMP%"
   if exist "fdoogr-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar" del /q /f "fdoogr-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar"
   if exist "fdoogr-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar.gz" del /q /f "fdoogr-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar.gz"
   tar -cf "fdoogr-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar" Bin
   gzip -9 "fdoogr-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar"
   cp "fdoogr-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar.gz" "%FDOTARZIPFOLDER%"\
   popd
   deltree /Y "%FDOZIPTEMP%"
:start_zip_sqlite
if "%SQLITEENABLE%"=="no" goto end
   mkdir %FDOZIPTEMP%
   mkdir %FDOZIPTEMP%\Bin
   copy "%FDOROOT%\Bin\SQLiteProvider.dll" %FDOZIPTEMP%\Bin\
   pushd "%FDOZIPTEMP%"
   if exist "fdosqlite-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar" del /q /f "fdosqlite-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar"
   if exist "fdosqlite-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar.gz" del /q /f "fdosqlite-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar.gz"
   tar -cf "fdosqlite-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar" Bin
   gzip -9 "fdosqlite-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar"
   cp "fdosqlite-%FILEPPLATFORMPREFIX%-%FDORELNUMBER%_%FDOBUILDNUMBER%.tar.gz" "%FDOTARZIPFOLDER%"\
   popd
   deltree /Y "%FDOZIPTEMP%"
	
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
echo NOTE: To use the createtarzipbinfiles script, download and install the  
echo       GnuWin32 command-line and Windows shell utilities for manipulating 
echo       tar archives. 
echo *
echo Example: createtarzipbinfiles.bat -i=C:\G029 -o=C:\G029 -b=F001 -r=3.6.0
echo *
echo createtarzipbinfiles.bat [-h] 
echo                          [-i=InFolder] 
echo                          [-o=OutFolder] 
echo                          [-w=WithModule] 
echo                          [-p=PlatformType]
echo                          [-b=BuildNumber]
echo                          [-r=ReleaseNumber]
echo *
echo Help:           -h[elp]
echo InFolder:       -i[npath]=input FDO SDK build folder
echo OutFolder:      -o[utpath]=destination folder for exported zipped files
echo WithModule:     -w[ith]=all(default), 
echo                         fdo, 
echo                         providers, 
echo                         shp, 
echo                         sdf, 
echo                         wfs, 
echo                         wms, 
echo                         arcsde, 
echo                         rdbms,
echo                         mysql,
echo                         odbc,
echo                         sqlspatial,
echo                         gdal, 
echo                         kingoracle, 
echo                         ogr,
echo                         postgis,
echo                         sqlite
echo PlatformType:   -p[latform]=Win32(default), x64
echo BuildNumber:    -b[uild]=User-Defined build number appended to the end of the .tar.gz files
echo ReleaseNumber:  -r[elease]=FDO build number appended to the end of the .tar.gz files
echo **************************************************************************

exit /B 0

