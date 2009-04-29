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

SET TYPEACTIONOGR=build
SET MSACTIONOGR=Build
SET TYPEBUILDOGR=release
SET FDOPATHOGR=%cd%
SET FDOINSPATHOGR=%cd%\Fdo
SET FDOBINPATHOGR=%cd%\Fdo\Bin
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
SET TYPEACTIONOGR=%2
if "%2"=="install" goto next_param
if "%2"=="build" goto next_param
if "%2"=="buildinstall" goto next_param
if "%2"=="clean" goto next_param
goto next_param

:get_conf
SET TYPEBUILDOGR=%2
if "%2"=="release" goto next_param
if "%2"=="debug" goto next_param
goto custom_error

:get_path
if (%2)==() goto next_param
SET FDOPATHOGR=%~2
SET FDOINSPATHOGR=%~2\Fdo
SET FDOBINPATHOGR=%~2\Fdo\Bin

:next_param
shift
shift
goto study_params

:start_build
if "%TYPEACTIONOGR%"=="build" goto start_exbuild
if "%TYPEACTIONOGR%"=="clean" goto start_exbuild
if not exist "%FDOINSPATHOGR%" mkdir "%FDOINSPATHOGR%"
if not exist "%FDOBINPATHOGR%" mkdir "%FDOBINPATHOGR%"

:start_exbuild
if "%TYPEACTIONOGR%"=="clean" SET MSACTIONOGR=Clean
if "%TYPEACTIONOGR%"=="install" goto install_files_ogr

echo %MSACTIONOGR% %TYPEBUILDOGR% OGR Provider Dlls
msbuild OGRProvider.sln /t:%MSACTIONOGR% /p:Configuration=%TYPEBUILDOGR% /p:Platform="Win32" /nologo /consoleloggerparameters:NoSummary
SET FDOERROR=%errorlevel%
if "%FDOERROR%"=="1" goto error
if "%TYPEACTIONOGR%"=="clean" goto end
if "%TYPEACTIONOGR%"=="build" goto end

:install_files_ogr
echo Copy %TYPEBUILDOGR% OGR Provider Output Files
copy /y "%TYPEBUILDOGR%\OGRProvider.dll" "%FDOBINPATHOGR%"
copy /y "%TYPEBUILDOGR%\OGRProvider.pdb" "%FDOBINPATHOGR%"

:end
echo End OGR %MSACTIONOGR%
exit /B 0

:error
echo There was a build error executing action: %MSACTIONOGR%
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