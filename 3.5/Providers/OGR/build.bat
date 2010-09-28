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

SET TYPEACTION=build
SET MSACTION=Build
SET TYPEBUILD=release
SET TYPEPLATFORM=Win32
SET INTERMEDIATEDIR=Win32
SET FDOPATH=%cd%
SET FDOINSPATH=%cd%\Fdo
SET FDOBINPATH=%cd%\Fdo\Bin
SET FDOERROR=0

:study_params
if (%1)==() goto start_build

if "%1"=="-help"    goto help_show
if "%1"=="-h"       goto help_show

if "%1"=="-o"       goto get_path
if "%1"=="-outpath" goto get_path

if "%1"=="-c"       goto get_conf
if "%1"=="-config"  goto get_conf

if "%1"=="-p"           goto get_platform
if "%1"=="-platform"    goto get_platform

if "%1"=="-a"       goto get_action
if "%1"=="-action"  goto get_action

goto next_param

:get_action
SET TYPEACTION=%2
if "%2"=="install" goto next_param
if "%2"=="build" goto next_param
if "%2"=="buildinstall" goto next_param
if "%2"=="clean" goto next_param
goto custom_error

:get_conf
SET TYPEBUILD=%2
if "%2"=="release" goto next_param
if "%2"=="debug" goto next_param
goto custom_error

:get_platform
SET TYPEPLATFORM=%2
if "%2"=="Win32" goto next_param
if "%2"=="x64" goto next_param
goto custom_error

:get_path
if (%2)==() goto next_param
SET FDOPATH=%~2
SET FDOINSPATH=%~2\Fdo
SET FDOBINPATH=%~2\Fdo\Bin

:next_param
shift
shift
goto study_params

:start_build
if "%TYPEACTION%"=="build" goto start_exbuild
if "%TYPEACTION%"=="clean" goto start_exbuild
if not exist "%FDOINSPATH%" mkdir "%FDOINSPATH%"
if not exist "%FDOBINPATH%" mkdir "%FDOBINPATH%"

if "%TYPEBUILD%"=="debug" SET INTERMEDIATEDIR=Debug
if "%TYPEBUILD%"=="release" SET INTERMEDIATEDIR=Release
if "%TYPEPLATFORM%"=="x64" SET INTERMEDIATEDIR=%INTERMEDIATEDIR%64

:start_exbuild
if "%TYPEACTION%"=="clean" SET MSACTION=Clean
if "%TYPEACTION%"=="install" goto install_files_ogr

echo %MSACTION% %TYPEBUILD% OGR Provider Dlls
msbuild OGRProvider.sln /t:%MSACTION% /p:Configuration=%TYPEBUILD% /p:Platform=%TYPEPLATFORM% /nologo /consoleloggerparameters:NoSummary
SET FDOERROR=%errorlevel%
if "%FDOERROR%"=="1" goto error
if "%TYPEACTION%"=="clean" goto end
if "%TYPEACTION%"=="build" goto end

:install_files_ogr
echo Copy %TYPEBUILD% OGR Provider Output Files
copy /y "%INTERMEDIATEDIR%\OGRProvider.dll" "%FDOBINPATH%"
copy /y "%INTERMEDIATEDIR%\OGRProvider.pdb" "%FDOBINPATH%"

:end
echo End OGR %MSACTION%
exit /B 0

:error
echo There was a build error executing action: %MSACTION%
exit /B 1

:custom_error
echo The command is not recognized.
echo Please use the format:

:help_show
echo **************************************************************************
echo build.bat [-h] 
echo           [-o=OutFolder] 
echo           [-c=BuildType]
echo           [-a=Action] 
echo           [-p=PlatformType]
echo *
echo Help:           -h[elp]
echo OutFolder:      -o[utpath]=destination folder for binaries
echo BuildType:      -c[onfig]=release(default), debug
echo PlatformType:   -p[latform]=Win32(default), x64
echo Action:         -a[ction]=build(default), buildinstall, install, clean
echo **************************************************************************
exit /B 0