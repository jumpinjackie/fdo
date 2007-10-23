@echo off

rem 
rem Copyright (C) 2004-2007  Autodesk, Inc.
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

SET TYPEACTIONPGIS=build
SET MSACTIONPGIS=Build
SET TYPEBUILDPGIS=release
SET FDOPATHPGIS=%cd%
SET FDOINSPATHPGIS=%cd%\Fdo
SET FDOBINPATHPGIS=%cd%\Fdo\Bin
SET FDOINCPATHPGIS=%cd%\Fdo\Inc
SET FDOLIBPATHPGIS=%cd%\Fdo\Lib
SET FDOERROR=0

:study_params
if (%1)==() goto start_build

if "%1"=="-help"    goto help_show
if "%1"=="-h"       goto help_show

if "%1"=="-o"       goto get_path
if "%1"=="-outpath" goto get_path

if "%1"=="-c"       goto get_conf
if "%1"=="-config"  goto get_conf

if "%1"=="-a"       goto get_action
if "%1"=="-action"  goto get_action

goto next_param

:get_action
SET TYPEACTIONPGIS=%2
if "%2"=="install" goto next_param
if "%2"=="build" goto next_param
if "%2"=="buildinstall" goto next_param
if "%2"=="clean" goto next_param
goto next_param

:get_conf
SET TYPEBUILDPGIS=%2
if "%2"=="release" goto next_param
if "%2"=="debug" goto next_param
goto custom_error

:get_path
if (%2)==() goto next_param
SET FDOPATHPGIS=%~2
SET FDOINSPATHPGIS=%~2\Fdo
SET FDOBINPATHPGIS=%~2\Fdo\Bin
SET FDOINCPATHPGIS=%~2\Fdo\Inc
SET FDOLIBPATHPGIS=%~2\Fdo\Lib

:next_param
shift
shift
goto study_params

:start_build
if "%TYPEACTIONPGIS%"=="build" goto start_exbuild
if "%TYPEACTIONPGIS%"=="clean" goto start_exbuild

:start_exbuild
if "%TYPEACTIONPGIS%"=="clean" SET MSACTIONPGIS=Clean
if "%TYPEACTIONPGIS%"=="install" goto install_files_pgis

echo %MSACTIONPGIS% %TYPEBUILDPGIS% PGIS Provider Dlls
msbuild Src/PostGis.sln /t:%MSACTIONPGIS% /p:Configuration=%TYPEBUILDPGIS% /p:Platform="Win32" /nologo /consoleloggerparameters:NoSummary
SET FDOERROR=%errorlevel%
if "%FDOERROR%"=="1" goto error
if "%TYPEACTIONPGIS%"=="clean" goto end
if "%TYPEACTIONPGIS%"=="build" goto end

:install_files_pgis
if not exist "%FDOINSPATHPGIS%" mkdir "%FDOINSPATHPGIS%"
if not exist "%FDOBINPATHPGIS%" mkdir "%FDOBINPATHPGIS%"
if not exist "%FDOINCPATHPGIS%" mkdir "%FDOINCPATHPGIS%"
if not exist "%FDOLIBPATHPGIS%" mkdir "%FDOLIBPATHPGIS%"
echo Copy %TYPEBUILDPGIS% PostGIS Provider Output Files
copy /y "Bin\Win32\%TYPEBUILDPGIS%\PostGISMessage.dll" "%FDOBINPATHPGIS%"
copy /y "Bin\Win32\%TYPEBUILDPGIS%\PostGISProvider.dll" "%FDOBINPATHPGIS%"
copy /y "Bin\Win32\%TYPEBUILDPGIS%\PostGISOverrides.dll" "%FDOBINPATHPGIS%"
copy /y "Bin\Win32\%TYPEBUILDPGIS%\PostGISOverrides.dll" "%FDOBINPATHPGIS%"
echo Copy %TYPEBUILDPGIS% PostGIS Library Files
copy /y "Lib\Win32\%TYPEBUILDPGIS%\PostGISOverrides.lib" "%FDOLIBPATHPGIS%"
echo Copy PostGIS Header Files
xcopy /S /C /Q /R /Y Inc\PostGIS\*.h "%FDOINCPATHPGIS%\PostGIS\"

:end
echo End PostGIS %MSACTIONPGIS%
exit /B 0

:error
echo There was a build error executing action: %MSACTIONPGIS%
exit /B 1

:custom_error
echo The command is not recognized.
echo Please use the format:

:help_show
echo **************************************************************************
echo build.bat [-h] [-o=OutFolder] [-c=BuildType] [-a=Action]
echo *
echo Help:           -h[elp]
echo OutFolder:      -o[utpath]=destination folder for binaries
echo BuildType:      -c[onfig]=release(default), debug
echo Action:         -a[ction]=build(default), buildinstall, install, clean
echo **************************************************************************
exit /B 0