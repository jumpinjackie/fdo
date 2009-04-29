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

SET TYPEACTIONKORA=build
SET MSACTIONKORA=Build
SET TYPEBUILDKORA=release
SET FDOPATHKORA=%cd%
SET FDOINSPATHKORA=%cd%\Fdo
SET FDOBINPATHKORA=%cd%\Fdo\Bin
SET FDOINCPATHKORA=%cd%\Fdo\Inc
SET FDOLIBPATHKORA=%cd%\Fdo\Lib
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
SET TYPEACTIONKORA=%2
if "%2"=="install" goto next_param
if "%2"=="build" goto next_param
if "%2"=="buildinstall" goto next_param
if "%2"=="clean" goto next_param
goto next_param

:get_conf
SET TYPEBUILDKORA=%2
if "%2"=="release" goto next_param
if "%2"=="debug" goto next_param
goto custom_error

:get_path
if (%2)==() goto next_param
SET FDOPATHKORA=%~2
SET FDOINSPATHKORA=%~2\Fdo
SET FDOBINPATHKORA=%~2\Fdo\Bin
SET FDOINCPATHKORA=%~2\Fdo\Inc
SET FDOLIBPATHKORA=%~2\Fdo\Lib

:next_param
shift
shift
goto study_params

:start_build
if not exist "%FDOORACLE%" goto error
if "%TYPEACTIONKORA%"=="build" goto start_exbuild
if "%TYPEACTIONKORA%"=="clean" goto start_exbuild

:start_exbuild
if "%TYPEACTIONKORA%"=="clean" SET MSACTIONKORA=Clean
if "%TYPEACTIONKORA%"=="install" goto install_files_KORA

echo %MSACTIONKORA% %TYPEBUILDKORA% KORA Provider Dlls
msbuild Src/KingOracle.sln /t:%MSACTIONKORA% /p:Configuration=%TYPEBUILDKORA% /p:Platform="Win32" /nologo /consoleloggerparameters:NoSummary
SET FDOERROR=%errorlevel%
if "%FDOERROR%"=="1" goto error
if "%TYPEACTIONKORA%"=="clean" goto end
if "%TYPEACTIONKORA%"=="build" goto end

:install_files_KORA
if not exist "%FDOINSPATHKORA%" mkdir "%FDOINSPATHKORA%"
if not exist "%FDOBINPATHKORA%" mkdir "%FDOBINPATHKORA%"
if not exist "%FDOINCPATHKORA%" mkdir "%FDOINCPATHKORA%"
if not exist "%FDOLIBPATHKORA%" mkdir "%FDOLIBPATHKORA%"
echo Copy %TYPEBUILDKORA% KingOracle Provider Output Files
copy /y "Bin\Win32\%TYPEBUILDKORA%\KingOracleMessage.dll" "%FDOBINPATHKORA%"
copy /y "Bin\Win32\%TYPEBUILDKORA%\KingOracleMessage.pdb" "%FDOBINPATHKORA%"
copy /y "Bin\Win32\%TYPEBUILDKORA%\KingOracleProvider.dll" "%FDOBINPATHKORA%"
copy /y "Bin\Win32\%TYPEBUILDKORA%\KingOracleProvider.pdb" "%FDOBINPATHKORA%"
copy /y "Bin\Win32\%TYPEBUILDKORA%\KingOracleOverrides.dll" "%FDOBINPATHKORA%"
copy /y "Bin\Win32\%TYPEBUILDKORA%\KingOracleOverrides.pdb" "%FDOBINPATHKORA%"
echo Copy %TYPEBUILDKORA% KingOracle Library Files
copy /y "Lib\Win32\%TYPEBUILDKORA%\KingOracleOverrides.lib" "%FDOLIBPATHKORA%"
echo Copy KingOracle Header Files
xcopy /S /C /Q /R /Y Inc\KingOracle\*.h "%FDOINCPATHKORA%\KingOracle\"

:end
echo End KingOracle %MSACTIONKORA%
exit /B 0

:error
echo There was a build error executing action: %MSACTIONKORA%
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