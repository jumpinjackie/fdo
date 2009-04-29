@echo off

rem 
rem Copyright (C) 2007  SL-King d.o.o
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

SET TYPEACTIONKMSS=build
SET MSACTIONKMSS=Build
SET TYPEBUILDKMSS=release
SET FDOPATHKMSS=%cd%
SET FDOINSPATHKMSS=%cd%\Fdo
SET FDOBINPATHKMSS=%cd%\Fdo\Bin
SET FDOINCPATHKMSS=%cd%\Fdo\Inc
SET FDOLIBPATHKMSS=%cd%\Fdo\Lib
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
SET TYPEACTIONKMSS=%2
if "%2"=="install" goto next_param
if "%2"=="build" goto next_param
if "%2"=="buildinstall" goto next_param
if "%2"=="clean" goto next_param
goto next_param

:get_conf
SET TYPEBUILDKMSS=%2
if "%2"=="release" goto next_param
if "%2"=="debug" goto next_param
goto custom_error

:get_path
if (%2)==() goto next_param
SET FDOPATHKMSS=%~2
SET FDOINSPATHKMSS=%~2\Fdo
SET FDOBINPATHKMSS=%~2\Fdo\Bin
SET FDOINCPATHKMSS=%~2\Fdo\Inc
SET FDOLIBPATHKMSS=%~2\Fdo\Lib

:next_param
shift
shift
goto study_params

:start_build
if "%TYPEACTIONKMSS%"=="build" goto start_exbuild
if "%TYPEACTIONKMSS%"=="clean" goto start_exbuild

:start_exbuild
if "%TYPEACTIONKMSS%"=="clean" SET MSACTIONKMSS=Clean
if "%TYPEACTIONKMSS%"=="install" goto install_files_KMSS

echo %MSACTIONKMSS% %TYPEBUILDKMSS% King SQLServer Provider Dlls
msbuild Src/KingMssProvider.sln /t:%MSACTIONKMSS% /p:Configuration=%TYPEBUILDKMSS% /p:Platform="Win32" /nologo /consoleloggerparameters:NoSummary
SET FDOERROR=%errorlevel%
if "%FDOERROR%"=="1" goto error
if "%TYPEACTIONKMSS%"=="clean" goto end
if "%TYPEACTIONKMSS%"=="build" goto end

:install_files_KMSS
if not exist "%FDOINSPATHKMSS%" mkdir "%FDOINSPATHKMSS%"
if not exist "%FDOBINPATHKMSS%" mkdir "%FDOBINPATHKMSS%"
if not exist "%FDOINCPATHKMSS%" mkdir "%FDOINCPATHKMSS%"
if not exist "%FDOLIBPATHKMSS%" mkdir "%FDOLIBPATHKMSS%"
echo Copy %TYPEBUILDKMSS% KingOracle Provider Output Files
copy /y "Bin\Win32\%TYPEBUILDKMSS%\KingMssProvider.dll" "%FDOBINPATHKMSS%"
copy /y "Bin\Win32\%TYPEBUILDKMSS%\KingMssProvider.pdb" "%FDOBINPATHKMSS%"

:end
echo End KingOracle %MSACTIONKMSS%
exit /B 0

:error
echo There was a build error executing action: %MSACTIONKMSS%
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