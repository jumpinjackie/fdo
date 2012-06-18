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

SET TYPEACTIONFDO=build
SET MSACTIONFDO=Build
SET TYPEBUILDFDO=release
SET TYPEPLATFORMFDO=Win32
SET FDOBINPATHFDO=%cd%\Fdo\Bin
SET FDOSAMPLEPATHFDO=%cd%\Fdo\Samples
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

goto custom_error

:get_conf
SET TYPEBUILDFDO=%2
if "%2"=="release" goto next_param
if "%2"=="debug" goto next_param
goto custom_error

:get_platform
SET TYPEPLATFORMFDO=%2
if "%2"=="Win32" goto next_param
if "%2"=="x64" goto next_param
goto custom_error

:get_action
SET TYPEACTIONFDO=%2
if "%2"=="install" goto next_param
if "%2"=="build" goto next_param
if "%2"=="buildinstall" goto next_param
if "%2"=="clean" goto next_param
if "%2"=="builddocsonly" goto next_param
goto custom_error

:get_path
if (%2)==() goto custom_error
SET FDOBINPATHFDO=%~2\Fdo\Bin
SET FDOSAMPLEPATHFDO=%~2\Fdo\Samples

:next_param
shift
shift
goto study_params

:start_build
if "%TYPEACTIONFDO%"=="build" goto start_exbuild
if "%TYPEACTIONFDO%"=="clean" goto start_exbuild
if "%TYPEACTIONFDO%"=="builddocsonly" goto end

if not exist "%FDOSAMPLEPATHFDO%" mkdir "%FDOSAMPLEPATHFDO%"
if not exist "%FDOBINPATHFDO%" mkdir "%FDOBINPATHFDO%"
if not exist "%FDOSAMPLEPATHFDO%\ProviderCapabilities" mkdir "%FDOSAMPLEPATHFDO%\ProviderCapabilities"
if not exist "%FDOSAMPLEPATHFDO%\ProviderList" mkdir "%FDOSAMPLEPATHFDO%\ProviderList"
if not exist "%FDOSAMPLEPATHFDO%\DevGuideExample" mkdir "%FDOSAMPLEPATHFDO%\DevGuideExample"
if not exist "%FDOSAMPLEPATHFDO%\OdbcSample" mkdir "%FDOSAMPLEPATHFDO%\OdbcSample"
if not exist "%FDOSAMPLEPATHFDO%\RasterFile" mkdir "%FDOSAMPLEPATHFDO%\RasterFile"

:start_exbuild
if "%TYPEACTIONFDO%"=="clean" SET MSACTIONFDO=Clean
if "%TYPEACTIONFDO%"=="install" goto install_files
if "%TYPEACTIONFDO%"=="builddocsonly" goto end

echo %MSACTIONFDO% %TYPEBUILDFDO% Fdo dlls
msbuild Sample.sln /t:%MSACTIONFDO% /p:Configuration=%TYPEBUILDFDO% /p:Platform=%TYPEPLATFORMFDO% /nologo /consoleloggerparameters:NoSummary
SET FDOERROR=%errorlevel%
if "%FDOERROR%"=="1" goto error

:finish_build
if "%TYPEACTIONFDO%"=="build" goto end
if "%TYPEACTIONFDO%"=="builddocsonly" goto end
if "%TYPEACTIONFDO%"=="clean" goto end

:install_files
echo Copy FDO Samples Files
copy /y "ProviderCapabilities\stdafx.h" "%FDOSAMPLEPATHFDO%\ProviderCapabilities"
copy /y "ProviderCapabilities\ProviderCapabilities.cpp" "%FDOSAMPLEPATHFDO%\ProviderCapabilities"
copy /y "ProviderCapabilities\ProviderCapabilities.vcproj" "%FDOSAMPLEPATHFDO%\ProviderCapabilities"
copy /y "ProviderCapabilities\stdafx.cpp" "%FDOSAMPLEPATHFDO%\ProviderCapabilities"
copy /y "ProviderCapabilities\ReadMe.txt" "%FDOSAMPLEPATHFDO%\ProviderCapabilities"
copy /y "ProviderList\stdafx.h" "%FDOSAMPLEPATHFDO%\ProviderList"
copy /y "ProviderList\ProviderList.cpp" "%FDOSAMPLEPATHFDO%\ProviderList"
copy /y "ProviderList\ProviderList.vcproj" "%FDOSAMPLEPATHFDO%\ProviderList"
copy /y "ProviderList\stdafx.cpp" "%FDOSAMPLEPATHFDO%\ProviderList"
copy /y "ProviderList\ReadMe.txt" "%FDOSAMPLEPATHFDO%\ProviderList"
copy /y "DevGuideExample\stdafx.h" "%FDOSAMPLEPATHFDO%\DevGuideExample"
copy /y "DevGuideExample\Dialog.cpp" "%FDOSAMPLEPATHFDO%\DevGuideExample"
copy /y "DevGuideExample\ParseSchemaFilterFile.cpp" "%FDOSAMPLEPATHFDO%\DevGuideExample"
copy /y "DevGuideExample\ProviderConfigInfo.h" "%FDOSAMPLEPATHFDO%\DevGuideExample"
copy /y "DevGuideExample\ArcSDEProvider.data" "%FDOSAMPLEPATHFDO%\DevGuideExample"
copy /y "DevGuideExample\OGC98046r1.schema" "%FDOSAMPLEPATHFDO%\DevGuideExample"
copy /y "DevGuideExample\OGC98046r1.delete" "%FDOSAMPLEPATHFDO%\DevGuideExample"
copy /y "DevGuideExample\strToFdoType.cpp" "%FDOSAMPLEPATHFDO%\DevGuideExample"
copy /y "DevGuideExample\Menu.h" "%FDOSAMPLEPATHFDO%\DevGuideExample"
copy /y "DevGuideExample\ParseSchemaDefinitionFile.h" "%FDOSAMPLEPATHFDO%\DevGuideExample"
copy /y "DevGuideExample\ExerciseFdoApi.spp" "%FDOSAMPLEPATHFDO%\DevGuideExample"
copy /y "DevGuideExample\ExerciseFdoCapabilities.cpp" "%FDOSAMPLEPATHFDO%\DevGuideExample"
copy /y "DevGuideExample\ExerciseFdoApi.vcproj" "%FDOSAMPLEPATHFDO%\DevGuideExample"
copy /y "DevGuideExample\ParseSchemaOperationFile.cpp" "%FDOSAMPLEPATHFDO%\DevGuideExample"
copy /y "DevGuideExample\conversions.h" "%FDOSAMPLEPATHFDO%\DevGuideExample"
copy /y "DevGuideExample\SDFProvider.data" "%FDOSAMPLEPATHFDO%\DevGuideExample"
copy /y "DevGuideExample\Logger.cpp" "%FDOSAMPLEPATHFDO%\DevGuideExample"
copy /y "DevGuideExample\ExerciseFdoSchema.cpp" "%FDOSAMPLEPATHFDO%\DevGuideExample"
copy /y "DevGuideExample\ExerciseFdoConnection.h" "%FDOSAMPLEPATHFDO%\DevGuideExample"
copy /y "DevGuideExample\fdoinfo.h" "%FDOSAMPLEPATHFDO%\DevGuideExample"
copy /y "DevGuideExample\stdafx.cpp" "%FDOSAMPLEPATHFDO%\DevGuideExample"
copy /y "DevGuideExample\ParseSchemaInsertionFile.h" "%FDOSAMPLEPATHFDO%\DevGuideExample"
copy /y "DevGuideExample\ProviderConfigInfo.cpp" "%FDOSAMPLEPATHFDO%\DevGuideExample"
copy /y "DevGuideExample\ExerciseFdoData.h" "%FDOSAMPLEPATHFDO%\DevGuideExample"
copy /y "DevGuideExample\ExerciseFdoUtilities.h" "%FDOSAMPLEPATHFDO%\DevGuideExample"
copy /y "DevGuideExample\ParseSchemaDefinitionFile.cpp" "%FDOSAMPLEPATHFDO%\DevGuideExample"
copy /y "DevGuideExample\Dialog.h" "%FDOSAMPLEPATHFDO%\DevGuideExample"
copy /y "DevGuideExample\Menu.cpp" "%FDOSAMPLEPATHFDO%\DevGuideExample"
copy /y "DevGuideExample\ParseSchemaFilterFile.h" "%FDOSAMPLEPATHFDO%\DevGuideExample"
copy /y "DevGuideExample\OracleProvider.data" "%FDOSAMPLEPATHFDO%\DevGuideExample"
copy /y "DevGuideExample\OGC98046r1.insert" "%FDOSAMPLEPATHFDO%\DevGuideExample"
copy /y "DevGuideExample\conversions.cpp" "%FDOSAMPLEPATHFDO%\DevGuideExample"
copy /y "DevGuideExample\strToFdoType.h" "%FDOSAMPLEPATHFDO%\DevGuideExample"
copy /y "DevGuideExample\ExerciseFdoCapabilities.h" "%FDOSAMPLEPATHFDO%\DevGuideExample"
copy /y "DevGuideExample\ExerciseFdoConnection.cpp" "%FDOSAMPLEPATHFDO%\DevGuideExample"
copy /y "DevGuideExample\ParseSchemaOperationFile.h" "%FDOSAMPLEPATHFDO%\DevGuideExample"
copy /y "DevGuideExample\fdoinfo.cpp" "%FDOSAMPLEPATHFDO%\DevGuideExample"
copy /y "DevGuideExample\Logger.h" "%FDOSAMPLEPATHFDO%\DevGuideExample"
copy /y "DevGuideExample\ParseSchemaInsertionFile.cpp" "%FDOSAMPLEPATHFDO%\DevGuideExample"
copy /y "DevGuideExample\ExerciseFdoSchema.h" "%FDOSAMPLEPATHFDO%\DevGuideExample"
copy /y "DevGuideExample\ExerciseFdoApi.cpp" "%FDOSAMPLEPATHFDO%\DevGuideExample"
copy /y "DevGuideExample\ExerciseFdoData.cpp" "%FDOSAMPLEPATHFDO%\DevGuideExample"
copy /y "DevGuideExample\README.txt" "%FDOSAMPLEPATHFDO%\DevGuideExample"
copy /y "DevGuideExample\ExerciseFdoUtilities.cpp" "%FDOSAMPLEPATHFDO%\DevGuideExample"
copy /y "DevGuideExample\Ogc98046FeatureSchema.xsd" "%FDOSAMPLEPATHFDO%\DevGuideExample"
copy /y "OdbcSample\Cities.xml" "%FDOSAMPLEPATHFDO%\OdbcSample"
copy /y "OdbcSample\stdafx.h" "%FDOSAMPLEPATHFDO%\OdbcSample"
copy /y "OdbcSample\Cities.mdb" "%FDOSAMPLEPATHFDO%\OdbcSample"
copy /y "OdbcSample\OdbcSample.cpp" "%FDOSAMPLEPATHFDO%\OdbcSample"
copy /y "OdbcSample\stdafx.cpp" "%FDOSAMPLEPATHFDO%\OdbcSample"
copy /y "OdbcSample\OdbcSample.vcproj" "%FDOSAMPLEPATHFDO%\OdbcSample"
copy /y "OdbcSample\ReadMe.txt" "%FDOSAMPLEPATHFDO%\OdbcSample"
copy /y "OdbcSample\providers.xml" "%FDOSAMPLEPATHFDO%\OdbcSample"
copy /y "RasterFile\airport.xml" "%FDOSAMPLEPATHFDO%\RasterFile"
copy /y "RasterFile\leftbottom.JPG" "%FDOSAMPLEPATHFDO%\RasterFile"
copy /y "RasterFile\lefttop.JPG" "%FDOSAMPLEPATHFDO%\RasterFile"
copy /y "RasterFile\RasterFile.cpp" "%FDOSAMPLEPATHFDO%\RasterFile"
copy /y "RasterFile\RasterFile.vcproj" "%FDOSAMPLEPATHFDO%\RasterFile"
copy /y "RasterFile\ReadMe.txt" "%FDOSAMPLEPATHFDO%\RasterFile"
copy /y "RasterFile\rightbottom.JPG" "%FDOSAMPLEPATHFDO%\RasterFile"
copy /y "RasterFile\righttop.JPG" "%FDOSAMPLEPATHFDO%\RasterFile"
copy /y "RasterFile\Samples.cpp" "%FDOSAMPLEPATHFDO%\RasterFile"
copy /y "RasterFile\stdafx.cpp" "%FDOSAMPLEPATHFDO%\RasterFile"
copy /y "RasterFile\stdafx.h" "%FDOSAMPLEPATHFDO%\RasterFile"
copy /y "Sample.sln" "%FDOSAMPLEPATHFDO%"

REM echo Copy FDO Samples %TYPEBUILDFDO% Output Files
REM copy /y "DevGuideExample\%TYPEBUILDFDO%\ExerciseFdoApi.exe" "%FDOBINPATHFDO%"
REM copy /y "OdbcSample\%TYPEBUILDFDO%\OdbcSample.exe" "%FDOBINPATHFDO%"
REM copy /y "ProviderCapabilities\%TYPEBUILDFDO%\ProviderCapabilities.exe" "%FDOBINPATHFDO%"
REM copy /y "ProviderList\%TYPEBUILDFDO%\ProviderList.exe" "%FDOBINPATHFDO%"
REM copy /y "RasterFile\%TYPEBUILDFDO%\RasterFile.exe" "%FDOBINPATHFDO%"

:end
echo End FDO %MSACTIONFDO%
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
echo There was a build error executing action: %MSACTIONFDO%
exit /B 1

:custom_error
echo The command is not recognized.
echo Please use the format:
:help_show
echo ********************************************************************************
echo build.bat [-h]
echo           [-o=OutFolder]
echo           [-c=BuildType]
echo           [-p=PlatformType]
echo           [-a=Action]
echo *
echo Help:                  -h[elp]
echo OutFolder:             -o[utpath]=destination folder for binaries
echo BuildType:             -c[onfig]=release(default), debug
echo PlatformType:          -p[latform]=Win32(default), x64
echo Action:                -a[ction]=build(default), 
echo                                  buildinstall, 
echo                                  install, 
echo                                  clean
echo ********************************************************************************
exit /B 0