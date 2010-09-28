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
SET FDOINCPATH=%cd%\Fdo\Inc
SET FDOLIBPATH=%cd%\Fdo\Lib
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
goto next_param

:get_platform
SET TYPEPLATFORM=%2
if "%2"=="Win32" goto next_param
if "%2"=="x64" goto next_param
goto custom_error

:get_conf
SET TYPEBUILD=%2
if "%2"=="release" goto next_param
if "%2"=="debug" goto next_param
goto custom_error

:get_path
if (%2)==() goto next_param
SET FDOPATH=%~2
SET FDOINSPATH=%~2\Fdo
SET FDOBINPATH=%~2\Fdo\Bin
SET FDOINCPATH=%~2\Fdo\Inc
SET FDOLIBPATH=%~2\Fdo\Lib

:next_param
shift
shift
goto study_params

:start_build
if "%TYPEPLATFORM%"=="Win32" SET INTERMEDIATEDIR="Win32"
if "%TYPEPLATFORM%"=="x64" SET INTERMEDIATEDIR="Win64"

if "%TYPEACTION%"=="build" goto start_exbuild
if "%TYPEACTION%"=="clean" goto start_exbuild

:start_exbuild
if "%TYPEACTION%"=="clean" SET MSACTION=Clean
if "%TYPEACTION%"=="install" goto install_files_pgis

echo %MSACTION% %TYPEBUILD% PGIS Provider Dlls
msbuild Src/PostGis.sln /t:%MSACTION% /p:Configuration=%TYPEBUILD% /p:Platform=%TYPEPLATFORM% /nologo /consoleloggerparameters:NoSummary
SET FDOERROR=%errorlevel%
if "%FDOERROR%"=="1" goto error
if "%TYPEACTION%"=="clean" goto end
if "%TYPEACTION%"=="build" goto end

:install_files_pgis
if not exist "%FDOINSPATH%" mkdir "%FDOINSPATH%"
if not exist "%FDOBINPATH%" mkdir "%FDOBINPATH%"
if not exist "%FDOINCPATH%" mkdir "%FDOINCPATH%"
if not exist "%FDOLIBPATH%" mkdir "%FDOLIBPATH%"
echo Copy %TYPEBUILD% PostGIS Provider Output Files
copy /y "Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\PostGISMessage.dll" "%FDOBINPATH%"
copy /y "Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\PostGISMessage.pdb" "%FDOBINPATH%"
copy /y "Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\PostGISProvider.dll" "%FDOBINPATH%"
copy /y "Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\PostGISProvider.pdb" "%FDOBINPATH%"
copy /y "Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\PostGISOverrides.dll" "%FDOBINPATH%"
copy /y "Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\PostGISOverrides.pdb" "%FDOBINPATH%"
echo Copy %TYPEBUILD% PostGIS Library Files
copy /y "Lib\%INTERMEDIATEDIR%\%TYPEBUILD%\PostGISOverrides.lib" "%FDOLIBPATH%"
echo Copy PostGIS Header Files
xcopy /S /C /Q /R /Y Inc\PostGIS\*.h "%FDOINCPATH%\PostGIS\"

:end
echo End PostGIS %MSACTION%
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