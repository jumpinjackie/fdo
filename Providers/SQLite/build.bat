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

SET TYPEACTIONSLITE=build
SET MSACTIONSLITE=Build
SET TYPEBUILDSLITE=release
SET TYPEPLATFORMSLITE=Win32
SET FDOORGPATHSLITE=%cd%
SET FDOINSPATHSLITE=%cd%\Fdo
SET FDOBINPATHSLITE=%cd%\Fdo\Bin
SET FDOINCPATHSLITE=%cd%\Fdo\Inc
SET FDOLIBPATHSLITE=%cd%\Fdo\Lib
SET FDODOCPATHSLITE=%cd%\Fdo\Docs
SET DOCENABLESLITE=skip
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

if "%1"=="-d"       goto get_docs
if "%1"=="-docs"    goto get_docs

goto custom_error

:get_docs
SET DOCENABLESLITE=%2
if "%2"=="build" goto next_param
if "%2"=="skip" goto next_param
goto custom_error

:get_conf 
SET TYPEBUILDSLITE=%2
if "%2"=="release" goto next_param
if "%2"=="debug" goto next_param
goto custom_error

:get_action
SET TYPEACTIONSLITE=%2
if "%2"=="install" goto next_param
if "%2"=="build" goto next_param
if "%2"=="buildinstall" goto next_param
if "%2"=="clean" goto next_param
goto custom_error 

:get_platform
SET TYPEPLATFORMSLITE=%2
if "%2"=="Win32" goto next_param
if "%2"=="x64" goto next_param
goto custom_error

:get_path
if (%2)==() goto custom_error
SET FDOORGPATHSLITE=%~2
SET FDOINSPATHSLITE=%~2\Fdo
SET FDOBINPATHSLITE=%~2\Fdo\Bin
SET FDOINCPATHSLITE=%~2\Fdo\Inc
SET FDOLIBPATHSLITE=%~2\Fdo\Lib
SET FDODOCPATHSLITE=%~2\Fdo\Docs

:next_param
shift
shift
goto study_params

:start_build
SET FDOACTENVSTUDY="FDO"
if ("%FDO%")==("") goto env_error
if not exist "%FDO%" goto env_path_error

if "%TYPEACTIONSLITE%"=="build" goto start_exbuild
if "%TYPEACTIONSLITE%"=="clean" goto start_exbuild
if not exist "%FDOINSPATHSLITE%" mkdir "%FDOINSPATHSLITE%"
if not exist "%FDOBINPATHSLITE%" mkdir "%FDOBINPATHSLITE%"

:start_exbuild
if "%TYPEACTIONSLITE%"=="clean" SET MSACTIONSLITE=Clean
if "%TYPEACTIONSLITE%"=="install" goto install_files_SQLite

echo %MSACTIONSLITE% %TYPEBUILDSLITE% SQLite provider dlls
SET FDOACTIVEBUILD=%cd%\Src\SQLiteProvider
cscript //Nologo //job:prepare preparebuilds.wsf
pushd Src
msbuild SQLiteProvider_temp.sln /t:%MSACTIONSLITE% /p:Configuration=%TYPEBUILDSLITE% /p:Platform=%TYPEPLATFORMSLITE% /nologo /consoleloggerparameters:NoSummary
SET FDOERROR=%errorlevel%
if exist SQLiteProvider_temp.sln del /Q /F SQLiteProvider_temp.sln
popd
if "%FDOERROR%"=="1" goto error
if "%TYPEACTIONSLITE%"=="clean" goto end
if "%TYPEACTIONSLITE%"=="build" goto end

:install_files_SQLite
echo copy %TYPEBUILDSLITE% SQLite provider output files
copy /y "Win32\%TYPEBUILDSLITE%\SQLiteProvider.dll" "%FDOBINPATHSLITE%"

:end
echo End SLITE %MSACTIONSLITE%
exit /B 0

:env_error
echo Environment variable undefined: %FDOACTENVSTUDY%
SET FDOERROR=1
exit /B 1

:env_path_error
echo Invalid path contained in FDO environment variable: %FDOACTENVSTUDY%
SET FDOERROR=1
exit /B 1

:env_path_error_ex
echo Unable to find location of %FDOACTENVSTUDY% in the Windows System PATH
SET FDOERROR=1
exit /B 1

:error
echo There was a build error executing action: %MSACTIONSLITE%
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
echo           [-d=BuildDocs]
echo *
echo Help:           -h[elp]
echo OutFolder:      -o[utpath]=destination folder for binaries
echo BuildType:      -c[onfig]=release(default), debug
echo PlatformType:   -p[latform]=Win32(default), x64
echo Action:         -a[ction]=build(default), buildinstall, install, clean
echo BuildDocs:      -d[ocs]=skip(default), build
echo **************************************************************************
exit /B 0
