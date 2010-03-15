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

SET TYPEACTIONTHR=build
SET MSACTIONTHR=Build
SET TYPEBUILDTHR=release
SET PLATFORMTHR=Win32

SET FDOBASPATHTHR=%cd%
SET FDOINSPATHTHR=%cd%\Fdo
SET FDOBINPATHTHR=%cd%\Fdo\Bin
SET FDOINCPATHTHR=%cd%\Fdo\Inc
SET FDOLIBPATHTHR=%cd%\Fdo\Lib
SET TYPEBUILDTHRPATH=
SET TYPEBUILDTHREX=

SET DEFMODIFYTHR=no
SET ALLENABLETHR=yes
SET FDOENABLETHR=no
SET SDFENABLETHR=no
SET WFSENABLETHR=no
SET WMSENABLETHR=no
SET GDALENABLETHR=no
SET POSTGISENABLETHR=no
SET FDOERROR=0

:study_params
if (%1)==() goto start_build

if "%1"=="-help"    goto help_show
if "%1"=="-h"       goto help_show

if "%1"=="-o"       goto get_path
if "%1"=="-outpath" goto get_path

if "%1"=="-c"       goto get_conf
if "%1"=="-config"  goto get_conf

if "%1"=="-p"       	goto get_platform
if "%1"=="-platform"    goto get_platform

if "%1"=="-a"       goto get_action
if "%1"=="-action"  goto get_action

if "%1"=="-w"       goto get_with
if "%1"=="-with"    goto get_with

goto custom_error

:get_with
if (%2)==() goto custom_error
if "%DEFMODIFYTHR%"=="yes" goto stp0_get_with
	SET DEFMODIFYTHR=yes
	SET ALLENABLETHR=no
	SET SDFENABLETHR=no
	SET WFSENABLETHR=no
	SET WMSENABLETHR=no
	SET GDALENABLETHR=no
	SET POSTGISENABLETHR=no
	SET OGRENABLETHR=no
	SET FDOENABLETHR=no
:stp0_get_with
if not "%2"=="providers" goto stp1_get_with
	SET SDFENABLETHR=yes
	SET WFSENABLETHR=yes
	SET WMSENABLETHR=yes
	SET GDALENABLETHR=yes
	SET POSTGISENABLETHR=yes
	SET OGRENABLETHR=yes
	goto next_param
:stp1_get_with
if not "%2"=="sdf" goto stp2_get_with
	SET SDFENABLETHR=yes
	goto next_param
:stp2_get_with
if not "%2"=="wfs" goto stp3_get_with
	SET WFSENABLETHR=yes
	goto next_param
:stp3_get_with
if not "%2"=="wms" goto stp4_get_with
	SET WMSENABLETHR=yes
	goto next_param
:stp4_get_with
if not "%2"=="postgis" goto stp5_get_with
	SET POSTGISENABLETHR=yes
	goto next_param
:stp5_get_with
if not "%2"=="fdo" goto stp6_get_with
	SET FDOENABLETHR=yes
	goto next_param
:stp6_get_with
if not "%2"=="gdal" goto stp7_get_with
	SET GDALENABLETHR=yes
	goto next_param
:stp7_get_with
if not "%2"=="all" goto stp8_get_with
	SET ALLENABLETHR=yes
	SET SDFENABLETHR=no
	SET WFSENABLETHR=no
	SET WMSENABLETHR=no
	SET GDALENABLETHR=no
	SET POSTGISENABLETHR=no
	SET FDOENABLETHR=no
	goto next_param
:stp8_get_with
if not "%2"=="ogr" goto custom_error
	SET GDALENABLETHR=yes
	goto next_param

:get_action
SET TYPEACTIONTHR=%2
if "%2"=="install" goto next_param
if "%2"=="build" goto next_param
if "%2"=="buildinstall" goto next_param
if "%2"=="clean" goto next_param
goto custom_error

:get_conf
SET TYPEBUILDTHR=%2
if "%2"=="release" goto next_param
SET TYPEBUILDTHRPATH=-gd
SET TYPEBUILDTHREX=D
if "%2"=="debug" goto next_param
goto custom_error

:get_platform
SET PLATFORMTHR=%2
if "%2"=="Win32" goto next_param
if "%2"=="x64" goto next_param
goto custom_error

:get_path
if (%2)==() goto custom_error
SET FDOBASPATHTHR=%~2
SET FDOINSPATHTHR=%~2\Fdo
SET FDOBINPATHTHR=%~2\Fdo\Bin
SET FDOINCPATHTHR=%~2\Fdo\Inc
SET FDOLIBPATHTHR=%~2\Fdo\Lib

:next_param
shift
shift
goto study_params

:start_build
if ("%FDOTHIRDPARTY%")==("") SET FDOTHIRDPARTY=%cd%
if ("%XALANROOT%")==("") SET XALANROOT=%cd%\apache\xml-xalan\c
if ("%XERCESCROOT%")==("") SET XERCESCROOT=%cd%\apache\xml-xerces\c
if ("%NLSDIR%")==("") SET NLSDIR=%cd%\apache\xml-xalan\c\Src\xalanc\NLS

if "%PLATFORMTHR%"=="Win32" SET INTERMEDIATEDIR=Win32
if "%PLATFORMTHR%"=="x64" SET INTERMEDIATEDIR=Win64

if "%TYPEACTIONTHR%"=="build" goto start_exbuild
if "%TYPEACTIONTHR%"=="clean" goto start_exbuild

if not exist "%FDOINSPATHTHR%" mkdir "%FDOINSPATHTHR%"
if not exist "%FDOBINPATHTHR%" mkdir "%FDOBINPATHTHR%"
if not exist "%FDOINCPATHTHR%" mkdir "%FDOINCPATHTHR%"
if not exist "%FDOLIBPATHTHR%" mkdir "%FDOLIBPATHTHR%"

:start_exbuild
if "%TYPEACTIONTHR%"=="clean" SET MSACTIONTHR=Clean

rem # Build all Thirdparty Files
:rebuild_all
if "%ALLENABLETHR%"=="no" goto rebuild_fdo
if "%TYPEACTIONTHR%"=="install" goto install_all_files

echo %MSACTIONTHR% %TYPEBUILDTHR% Thirdparty files
msbuild Thirdparty_fdo.sln /t:%MSACTIONTHR% /p:Configuration=%TYPEBUILDTHR% /p:Platform=%PLATFORMTHR% /nologo /consoleloggerparameters:NoSummary
SET FDOERROR=%errorlevel%
if "%FDOERROR%"=="1" goto error
msbuild Thirdparty_sdf.sln /t:%MSACTIONTHR% /p:Configuration=%TYPEBUILDTHR% /p:Platform=%PLATFORMTHR% /nologo /consoleloggerparameters:NoSummary
SET FDOERROR=%errorlevel%

if "%FDOERROR%"=="1" goto error
msbuild openssl\openssl.sln /t:%MSACTIONTHR% /p:Configuration=%TYPEBUILDTHR% /p:Platform=%PLATFORMTHR% /nologo /consoleloggerparameters:NoSummary
SET FDOERROR=%errorlevel%
if "%FDOERROR%"=="1" goto error
msbuild libcurl\lib\curllib.sln /t:%MSACTIONTHR% /p:Configuration=%TYPEBUILDTHR% /p:Platform=%PLATFORMTHR% /nologo /consoleloggerparameters:NoSummary
SET FDOERROR=%errorlevel%
if "%FDOERROR%"=="1" goto error
msbuild boost\boost.sln /t:%MSACTIONTHR% /p:Configuration=%TYPEBUILDTHR% /p:Platform=%PLATFORMTHR% /nologo /consoleloggerparameters:NoSummary
SET FDOERROR=%errorlevel%
msbuild gdal\gdal.sln /t:%MSACTIONTHR% /p:Configuration=%TYPEBUILDTHR% /p:Platform=%PLATFORMTHR% /nologo /consoleloggerparameters:NoSummary
SET FDOERROR=%errorlevel%
if "%FDOERROR%"=="1" goto error
if not exist util\UpdateVersion\bin mkdir util\UpdateVersion\bin
copy /y util\UpdateVersion\build\UpdateVersion.exe util\UpdateVersion\bin
if "%TYPEACTIONTHR%"=="build" goto rebuild_fdo
if "%TYPEACTIONTHR%"=="clean" goto rebuild_fdo

rem # Install all Thirdparty Files
:install_all_files
echo copy %TYPEBUILDTHR% Thirdparty files
copy /y "apache\xml-xalan\c\Build\%INTERMEDIATEDIR%\vc9\%TYPEBUILDTHR%\Xalan-C_1_7_0%TYPEBUILDTHREX%.dll" "%FDOBINPATHTHR%"
copy /y "apache\xml-xalan\c\Build\%INTERMEDIATEDIR%\vc9\%TYPEBUILDTHR%\XalanMessages_1_7_0%TYPEBUILDTHREX%.dll" "%FDOBINPATHTHR%"
copy /y "apache\xml-xerces\c\Build\%INTERMEDIATEDIR%\vc9\%TYPEBUILDTHR%\xerces-c_2_5_0%TYPEBUILDTHREX%.dll" "%FDOBINPATHTHR%"
copy /y "gdal\bin\%INTERMEDIATEDIR%\%TYPEBUILDTHR%\gdal17.dll" "%FDOBINPATHTHR%"
copy /y "boost\stage\%INTERMEDIATEDIR%\%TYPEBUILDTHR%\lib\boost_thread-vc90-mt%TYPEBUILDTHRPATH%-1_42.dll" "%FDOBINPATHTHR%"
copy /y "boost\stage\%INTERMEDIATEDIR%\%TYPEBUILDTHR%\lib\boost_date_time-vc90-mt%TYPEBUILDTHRPATH%-1_42.dll" "%FDOBINPATHTHR%"

rem # Build FDO API Thirdparty Files
:rebuild_fdo
if "%FDOENABLETHR%"=="no" goto rebuild_sdf
if "%TYPEACTIONTHR%"=="install" goto install_fdo_files

echo %MSACTIONTHR% %TYPEBUILDTHR% Thirdparty FDO files
msbuild Thirdparty_fdo.sln /t:%MSACTIONTHR% /p:Configuration=%TYPEBUILDTHR% /p:Platform=%PLATFORMTHR% /nologo /consoleloggerparameters:NoSummary
SET FDOERROR=%errorlevel%
if "%FDOERROR%"=="1" goto error
if not exist util\UpdateVersion\bin mkdir util\UpdateVersion\bin
copy /y util\UpdateVersion\build\UpdateVersion.exe util\UpdateVersion\bin
if "%TYPEACTIONTHR%"=="build" goto rebuild_sdf
if "%TYPEACTIONTHR%"=="clean" goto rebuild_sdf

rem # Install FDO API Thirdparty Files
:install_fdo_files
echo copy %TYPEBUILDTHR% Thirdparty files
copy /y "apache\xml-xalan\c\Build\%PLATFORMTHR%\vc9\%PLATFORMTHR%\Xalan-C_1_7_0%TYPEBUILDTHREX%.dll" "%FDOBINPATHTHR%"
copy /y "apache\xml-xalan\c\Build\%PLATFORMTHR%\vc9\%PLATFORMTHR%\XalanMessages_1_7_0%TYPEBUILDTHREX%.dll" "%FDOBINPATHTHR%"
copy /y "apache\xml-xerces\c\Build\%PLATFORMTHR%\vc9\%PLATFORMTHR%\xerces-c_2_5_0%TYPEBUILDTHREX%.dll" "%FDOBINPATHTHR%"

rem # Build SDF Provider Thirdparty Files
:rebuild_sdf
if "%SDFENABLETHR%"=="no" goto rebuild_wfs
if "%TYPEACTIONTHR%"=="install" goto rebuild_wfs

echo %MSACTIONTHR% %TYPEBUILDTHR% Thirdparty SDF files
msbuild Thirdparty_sdf.sln /t:%MSACTIONTHR% /p:Configuration=%TYPEBUILDTHR% /p:Platform=%PLATFORMTHR% /nologo /consoleloggerparameters:NoSummary
SET FDOERROR=%errorlevel%
if "%FDOERROR%"=="1" goto error

rem # Build WFS Provider Thirdparty Files
:rebuild_wfs
if "%WFSENABLETHR%"=="no" goto rebuild_wms
if "%TYPEACTIONTHR%"=="install" goto install_wfs_files

echo %MSACTIONTHR% %TYPEBUILDTHR% Thirdparty WFS files
msbuild openssl\openssl.sln /t:%MSACTIONTHR% /p:Configuration=%TYPEBUILDTHR% /p:Platform=%PLATFORMTHR% /nologo /consoleloggerparameters:NoSummary
SET FDOERROR=%errorlevel%
if "%FDOERROR%"=="1" goto error
msbuild libcurl\lib\curllib.sln /t:%MSACTIONTHR% /p:Configuration=%TYPEBUILDTHR% /p:Platform=%PLATFORMTHR% /nologo /consoleloggerparameters:NoSummary
SET FDOERROR=%errorlevel%
if "%FDOERROR%"=="1" goto error
if not "%PLATFORMTHR%"=="Win32" goto rebuild_wms
msbuild boost\boost.sln /t:%MSACTIONTHR% /p:Configuration=%TYPEBUILDTHR% /p:Platform=%PLATFORMTHR% /nologo /consoleloggerparameters:NoSummary
SET FDOERROR=%errorlevel%
if "%FDOERROR%"=="1" goto error
if "%TYPEACTIONTHR%"=="build" goto rebuild_wms
if "%TYPEACTIONTHR%"=="clean" goto rebuild_wms

rem # Install WFS Provider Thirdparty Files
:install_wfs_files
echo copy %TYPEBUILDTHR% Thirdparty WFS files
copy /y "boost\stage\%PLATFORMTHR%\%TYPEBUILDTHR%\lib\boost_thread-vc90-mt%TYPEBUILDTHRPATH%-1_42.dll" "%FDOBINPATHTHR%"
rem # End WFS part #

rem # Build WMS Provider Thirdparty Files
:rebuild_wms
if "%WMSENABLETHR%"=="no" goto rebuild_gdal
if "%TYPEACTIONTHR%"=="install" goto install_wms_files

echo %MSACTIONTHR% %TYPEBUILDTHR% Thirdparty WMS files
msbuild openssl\openssl.sln /t:%MSACTIONTHR% /p:Configuration=%TYPEBUILDTHR% /p:Platform=%PLATFORMTHR% /nologo /consoleloggerparameters:NoSummary
SET FDOERROR=%errorlevel%
if "%FDOERROR%"=="1" goto error
msbuild libcurl\lib\curllib.sln /t:%MSACTIONTHR% /p:Configuration=%TYPEBUILDTHR% /p:Platform=%PLATFORMTHR% /nologo /consoleloggerparameters:NoSummary
SET FDOERROR=%errorlevel%
if "%FDOERROR%"=="1" goto error
msbuild gdal\gdal.sln /t:%MSACTIONTHR% /p:Configuration=%TYPEBUILDTHR% /p:Platform=%PLATFORMTHR% /nologo /consoleloggerparameters:NoSummary
SET FDOERROR=%errorlevel%
if "%FDOERROR%"=="1" goto error
if not "%PLATFORMTHR%"=="Win32" goto rebuild_gdal
msbuild boost\boost.sln /t:%MSACTIONTHR% /p:Configuration=%TYPEBUILDTHR% /p:Platform=%PLATFORMTHR% /nologo /consoleloggerparameters:NoSummary
SET FDOERROR=%errorlevel%
if "%FDOERROR%"=="1" goto error
if "%TYPEACTIONTHR%"=="build" goto end
if "%TYPEACTIONTHR%"=="clean" goto end

rem # Install WMS Provider Thirdparty Files
:install_wms_files
echo copy %TYPEBUILDTHR% Thirdparty WMS files
copy /y "gdal\bin\Win32\%TYPEBUILDTHR%\gdal17.dll" "%FDOBINPATHTHR%"
copy /y "boost\stage\%PLATFORMTHR%\%TYPEBUILDTHR%\lib\boost_thread-vc90-mt%TYPEBUILDTHRPATH%-1_42.dll" "%FDOBINPATHTHR%"
rem # End WMS part #

rem # Build GDAL Provider Thirdparty Files
:rebuild_gdal
if "%GDALENABLETHR%"=="no" goto rebuild_postgis
if "%TYPEACTIONTHR%"=="install" goto install_gdal_files

echo %MSACTIONTHR% %TYPEBUILDTHR% Thirdparty GDAL files
msbuild gdal\gdal.sln /t:%MSACTIONTHR% /p:Configuration=%TYPEBUILDTHR% /p:Platform=%PLATFORMTHR% /nologo /consoleloggerparameters:NoSummary
SET FDOERROR=%errorlevel%
if "%FDOERROR%"=="1" goto error
if "%TYPEACTIONTHR%"=="build" goto end
if "%TYPEACTIONTHR%"=="clean" goto end

rem # Install GDAL Provider Thirdparty Files
:install_gdal_files
echo copy %TYPEBUILDTHR% Thirdparty GDAL files
copy /y "gdal\bin\Win32\%TYPEBUILDTHR%\gdal17.dll" "%FDOBINPATHTHR%"
rem # End GDAL part #

rem # Build PostGIS Provider Thirdparty Files
:rebuild_postgis
if "%POSTGISENABLETHR%"=="no" goto end
if not "%PLATFORMTHR%"=="Win32" goto end
if "%TYPEACTIONTHR%"=="install" goto install_postgis_files

echo %MSACTIONTHR% %TYPEBUILDTHR% Thirdparty PostGIS dlls
msbuild boost\boost.sln /t:%MSACTIONTHR% /p:Configuration=%TYPEBUILDTHR% /p:Platform=%PLATFORMTHR% /nologo /consoleloggerparameters:NoSummary
SET FDOERROR=%errorlevel%
if "%FDOERROR%"=="1" goto error
if "%TYPEACTIONTHR%"=="build" goto end
if "%TYPEACTIONTHR%"=="clean" goto end

rem # Install PostGIS Provider Thirdparty Files
:install_postgis_files

echo copy %TYPEBUILDTHR% Thirdparty PostGIS dlls
copy /y "boost\stage\%PLATFORMTHR%\%TYPEBUILDTHR%\lib\boost_thread-vc90-mt%TYPEBUILDTHRPATH%-1_42.dll" "%FDOBINPATHTHR%"
copy /y "boost\stage\%PLATFORMTHR%\%TYPEBUILDTHR%\lib\boost_date_time-vc90-mt%TYPEBUILDTHRPATH%-1_42.dll" "%FDOBINPATHTHR%"
rem # End PostGIS part 

:end
echo End Thirdparty %MSACTIONTHR%
exit /B 0

:error
echo There was a build error executing action: %MSACTIONTHR%
exit /B 1

:custom_error
echo The command is not recognized.
echo Please use the format:
:help_show
echo **************************************************************************
echo build.bat [-h] 
echo           [-o=DestFolder]
echo           [-c=BuildType]
echo           [-p=PlatformType] 
echo           [-a=Action]
echo           [-w=WithModule]
echo *
echo Help:           -h[elp]
echo OutFolder:      -o[utpath]=destination folder for binaries
echo BuildType:      -c[onfig]=release(default), debug
echo PlatformType:   -p[latform]=Win32(default), x64
echo Action:         -a[ction]=build(default), buildinstall, install, clean
echo WithModule:     -w[ith]=all(default), fdo, providers, sdf, wfs, wms, gdal, postgis
echo **************************************************************************
exit /B 0
