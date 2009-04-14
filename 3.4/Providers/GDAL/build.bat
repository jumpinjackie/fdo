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

SET TYPEACTIONRFP=build
SET MSACTIONRFP=Build
SET TYPEBUILDRFP=release
SET FDOORGPATHRFP=%cd%
SET FDOINSPATHRFP=%cd%\Fdo
SET FDOBINPATHRFP=%cd%\Fdo\Bin
SET FDOINCPATHRFP=%cd%\Fdo\Inc
SET FDOLIBPATHRFP=%cd%\Fdo\Lib
SET FDODOCPATHRFP=%cd%\Fdo\Docs
SET DOCENABLERFP=skip
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

if "%1"=="-d"       goto get_docs
if "%1"=="-docs"    goto get_docs

goto custom_error

:get_docs
SET DOCENABLERFP=%2
if "%2"=="build" goto next_param
if "%2"=="skip" goto next_param
goto custom_error

:get_action
SET TYPEACTIONRFP=%2
if "%2"=="install" goto next_param
if "%2"=="build" goto next_param
if "%2"=="buildinstall" goto next_param
if "%2"=="clean" goto next_param
goto custom_error

:get_conf
SET TYPEBUILDRFP=%2
if "%2"=="release" goto next_param
if "%2"=="debug" goto next_param
goto custom_error

:get_path
if (%2)==() goto custom_error
SET FDOORGPATHRFP=%~2
SET FDOINSPATHRFP=%~2\Fdo
SET FDOBINPATHRFP=%~2\Fdo\Bin
SET FDOINCPATHRFP=%~2\Fdo\Inc
SET FDOLIBPATHRFP=%~2\Fdo\Lib
SET FDODOCPATHRFP=%~2\Fdo\Docs

:next_param
shift
shift
goto study_params

:start_build
SET FDOACTENVSTUDY="FDO"
if ("%FDO%")==("") goto env_error
if not exist "%FDO%" goto env_path_error
SET FDOACTENVSTUDY="FDOTHIRDPARTY"
if ("%FDOTHIRDPARTY%")==("") goto env_error
if not exist "%FDOTHIRDPARTY%" goto env_path_error
SET FDOACTENVSTUDY="FDOUTILITIES"
if ("%FDOUTILITIES%")==("") goto env_error
if not exist "%FDOUTILITIES%" goto env_path_error

if "%TYPEACTIONRFP%"=="build" goto start_exbuild
if "%TYPEACTIONRFP%"=="clean" goto start_exbuild
if not exist "%FDOINSPATHRFP%" mkdir "%FDOINSPATHRFP%"
if not exist "%FDOBINPATHRFP%" mkdir "%FDOBINPATHRFP%"
if not exist "%FDOINCPATHRFP%" mkdir "%FDOINCPATHRFP%"
if not exist "%FDOLIBPATHRFP%" mkdir "%FDOLIBPATHRFP%"
if not exist "%FDODOCPATHRFP%" mkdir "%FDODOCPATHRFP%"

:start_exbuild
if "%TYPEACTIONRFP%"=="clean" SET MSACTIONRFP=Clean
if "%TYPEACTIONRFP%"=="install" goto install_files_rfp

echo %MSACTIONRFP% %TYPEBUILDRFP% GdalFile provider dlls
pushd Src
SET FDOACTIVEBUILD=%cd%\RFP
cscript //job:prepare ../../../preparebuilds.wsf
msbuild RFP_temp.sln /t:%MSACTIONRFP% /p:Configuration=%TYPEBUILDRFP% /p:Platform="Win32" /nologo /consoleloggerparameters:NoSummary
SET FDOERROR=%errorlevel%
if exist RFP_temp.sln del /Q /F RFP_temp.sln
popd
if "%FDOERROR%"=="1" goto error
if "%TYPEACTIONRFP%"=="clean" goto end
if "%TYPEACTIONRFP%"=="build" goto generate_docs

:install_files_rfp
echo Copy %TYPEBUILDRFP% GdalFile Provider Output Files
copy /y "Bin\Win32\%TYPEBUILDRFP%\GRFPMessage.dll" "%FDOBINPATHRFP%"
copy /y "Bin\Win32\%TYPEBUILDRFP%\GRFPOverrides.dll" "%FDOBINPATHRFP%"
copy /y "Lib\Win32\%TYPEBUILDRFP%\GRFPOverrides.lib" "%FDOLIBPATHRFP%"
copy /y "Bin\Win32\%TYPEBUILDRFP%\GRFPProvider.dll" "%FDOBINPATHRFP%"

echo Copy GdalFile SDK Header Files
xcopy /S /C /Q /R /Y Inc\GdalFile\*.h "%FDOINCPATHRFP%\GdalFile\"

:generate_docs
if "%DOCENABLERFP%"=="skip" goto install_docs
echo Creating GDAL provider html and chm documentation
if exist "Docs\HTML\GDAL" rmdir /S /Q "Docs\HTML\GDAL"
if not exist "Docs\HTMLGDAL" mkdir "Docs\HTML\GDAL"
copy ..\..\DocResources\geospatial.js Docs\HTML\GDAL
copy ..\..\DocResources\osgeo.css Docs\HTML\GDAL
if exist Docs\GDAL_Provider_API.chm attrib -r Docs\GDAL_Provider_API.chm
pushd Docs\doc_src
doxygen Doxyfile_GDAL
popd

:install_docs
if "%TYPEACTIONRFP%"=="build" goto end
if exist "%FDODOCPATHRFP%\HTML\Providers\GDAL" rmdir /S /Q "%FDODOCPATHRFP%\HTML\Providers\GDAL"
if exist Docs\HTML\GDAL xcopy/CQEYI Docs\HTML\GDAL\* "%FDODOCPATHRFP%\HTML\Providers\GDAL"
if exist "Docs\GDAL_Provider_API.chm" copy /y "Docs\GDAL_Provider_API.chm" "%FDODOCPATHRFP%"

:end
echo End RFP %MSACTIONRFP%
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
echo There was a build error executing action: %MSACTIONRFP%
exit /B 1

:custom_error
echo The command is not recognized.
echo Please use the format:
:help_show
echo **************************************************************************
echo build.bat [-h] [-o=OutFolder] [-c=BuildType] [-a=Action] [-d=BuildDocs]
echo *
echo Help:           -h[elp]
echo OutFolder:      -o[utpath]=destination folder for binaries
echo BuildType:      -c[onfig]=release(default), debug
echo Action:         -a[ction]=build(default), buildinstall, install, clean
echo BuildDocs:      -d[ocs]=skip(default), build
echo **************************************************************************
exit /B 0
