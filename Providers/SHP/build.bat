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

SET TYPEACTIONSHP=build
SET MSACTIONSHP=Build
SET TYPEBUILDSHP=release
SET TYPEPLATFORMSHP=Win32
SET FDOORGPATHSHP=%cd%
SET FDOINSPATHSHP=%cd%\Fdo
SET FDOBINPATHSHP=%cd%\Fdo\Bin
SET FDOINCPATHSHP=%cd%\Fdo\Inc
SET FDOLIBPATHSHP=%cd%\Fdo\Lib
SET FDODOCPATHSHP=%cd%\Fdo\Docs
SET DOCENABLESHP=skip
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
SET DOCENABLESHP=%2
if "%2"=="build" goto next_param
if "%2"=="skip" goto next_param
goto custom_error

:get_conf 
SET TYPEBUILDSHP=%2
if "%2"=="release" goto next_param
if "%2"=="debug" goto next_param
goto custom_error

:get_action
SET TYPEACTIONSHP=%2
if "%2"=="install" goto next_param
if "%2"=="build" goto next_param
if "%2"=="buildinstall" goto next_param
if "%2"=="clean" goto next_param
goto custom_error 

:get_platform
SET TYPEPLATFORMSHP=%2
if "%2"=="Win32" goto next_param
if "%2"=="x64" goto next_param
goto custom_error

:get_path
if (%2)==() goto custom_error
SET FDOORGPATHSHP=%~2
SET FDOINSPATHSHP=%~2\Fdo
SET FDOBINPATHSHP=%~2\Fdo\Bin
SET FDOINCPATHSHP=%~2\Fdo\Inc
SET FDOLIBPATHSHP=%~2\Fdo\Lib
SET FDODOCPATHSHP=%~2\Fdo\Docs

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

if "%TYPEACTIONSHP%"=="build" goto start_exbuild
if "%TYPEACTIONSHP%"=="clean" goto start_exbuild
if not exist "%FDOINSPATHSHP%" mkdir "%FDOINSPATHSHP%"
if not exist "%FDOBINPATHSHP%" mkdir "%FDOBINPATHSHP%"
if not exist "%FDOINCPATHSHP%" mkdir "%FDOINCPATHSHP%"
if not exist "%FDOLIBPATHSHP%" mkdir "%FDOLIBPATHSHP%"
if not exist "%FDODOCPATHSHP%" mkdir "%FDODOCPATHSHP%"

:start_exbuild
if "%TYPEACTIONSHP%"=="clean" SET MSACTIONSHP=Clean
if "%TYPEACTIONSHP%"=="install" goto install_files_shp

echo %MSACTIONSHP% %TYPEBUILDSHP% SHP provider dlls
SET FDOACTIVEBUILD=%cd%\Src\SHP
cscript //Nologo //job:prepare preparebuilds.wsf
pushd Src
msbuild SHP_temp.sln /t:%MSACTIONSHP% /p:Configuration=%TYPEBUILDSHP% /p:Platform=%TYPEPLATFORMSHP% /nologo /consoleloggerparameters:NoSummary
SET FDOERROR=%errorlevel%
if exist SHP_temp.sln del /Q /F SHP_temp.sln
popd
if "%FDOERROR%"=="1" goto error
if "%TYPEACTIONSHP%"=="clean" goto end
if "%TYPEACTIONSHP%"=="build" goto generate_docs

:install_files_shp
echo copy %TYPEBUILDSHP% SHP provider output files
copy /y "Managed\bin\%TYPEBUILDSHP%\OSGeo.FDO.Providers.SHP.Overrides.dll" "%FDOBINPATHSHP%"
copy /y "Managed\bin\%TYPEBUILDSHP%\OSGeo.FDO.Providers.SHP.Overrides.pdb" "%FDOBINPATHSHP%"
copy /y "Bin\Win32\%TYPEBUILDSHP%\SHPProvider.dll" "%FDOBINPATHSHP%"
copy /y "Bin\Win32\%TYPEBUILDSHP%\SHPProvider.pdb" "%FDOBINPATHSHP%"
copy /y "Bin\Win32\%TYPEBUILDSHP%\SHPOverrides.dll" "%FDOBINPATHSHP%"
copy /y "Bin\Win32\%TYPEBUILDSHP%\SHPOverrides.pdb" "%FDOBINPATHSHP%"
copy /y "Bin\Win32\%TYPEBUILDSHP%\SHPMessage.dll" "%FDOBINPATHSHP%"
copy /y "Bin\Win32\%TYPEBUILDSHP%\SHPMessage.pdb" "%FDOBINPATHSHP%"
copy /y "Lib\Win32\%TYPEBUILDSHP%\SHPOverrides.lib" "%FDOLIBPATHSHP%"
copy /y "%FDOUTILITIES%\ExpressionEngine\bin\win32\%TYPEBUILDSHP%\ExpressionEngine.dll" "%FDOBINPATHSHP%"
copy /y "%FDOUTILITIES%\ExpressionEngine\bin\win32\%TYPEBUILDSHP%\ExpressionEngine.pdb" "%FDOBINPATHSHP%"
copy /y "%FDOUTILITIES%\ExpressionEngine\lib\win32\%TYPEBUILDSHP%\ExpressionEngine.lib" "%FDOLIBPATHSHP%"

echo copy header files
xcopy /S /C /Q /R /Y Inc\SHP\*.h "%FDOINCPATHSHP%\SHP\"
xcopy /C /Q /R /Y /I "%FDOUTILITIES%\ExpressionEngine\Inc\*.h" "%FDOINCPATHSHP%\ExpressionEngine"
del /Q/F "%FDOINCPATHSHP%\ExpressionEngine\FdoExpressionEngineImp.h"
xcopy /C /Q /R /Y /I "%FDOUTILITIES%\ExpressionEngine\Inc\Util\*.h" "%FDOINCPATHSHP%\ExpressionEngine\Util"

:generate_docs
if "%DOCENABLESHP%"=="skip" goto install_docs
echo Creating SHP provider html and chm documentation
if exist "Docs\HTML\SHP" rmdir /S /Q "Docs\HTML\SHP"
if not exist "Docs\HTML\SHP" mkdir "Docs\HTML\SHP"
copy ..\..\DocResources\geospatial.js Docs\HTML\SHP
copy ..\..\DocResources\osgeo.css Docs\HTML\SHP
if exist Docs\SHP_Provider_API.chm attrib -r Docs\SHP_Provider_API.chm
if exist "Docs\HTML\SHP_managed" rmdir /S /Q "Docs\HTML\SHP_managed"
if not exist "Docs\HTML\SHP_managed" mkdir "Docs\HTML\SHP_managed"
copy ..\..\DocResources\geospatial.js Docs\HTML\SHP_managed
copy ..\..\DocResources\osgeo.css Docs\HTML\SHP_managed
if exist Docs\SHP_Provider_API_managed.chm attrib -r Docs\SHP_Provider_API_managed.chm
pushd Docs\doc_src
doxygen Doxyfile_SHP
doxygen Doxyfile_SHP_managed
popd

:install_docs
if "%TYPEACTIONSHP%"=="build" goto end
if exist "%FDODOCPATHSHP%\HTML\Providers\SHP" rmdir /S /Q "%FDODOCPATHSHP%\HTML\Providers\SHP"
if exist Docs\HTML\SHP xcopy/CQEYI Docs\HTML\SHP\* "%FDODOCPATHSHP%\HTML\Providers\SHP"
if exist "Docs\SHP_Provider_API.chm" copy /y "Docs\SHP_Provider_API.chm" "%FDODOCPATHSHP%"
if exist "%FDODOCPATHSHP%\HTML\Providers\SHP_managed" rmdir /S /Q "%FDODOCPATHSHP%\HTML\Providers\SHP_managed"
if exist Docs\HTML\SHP_managed xcopy/CQEYI Docs\HTML\SHP_managed\* "%FDODOCPATHSHP%\HTML\Providers\SHP_managed"
if exist "Docs\SHP_Provider_API_managed.chm" copy /y "Docs\SHP_Provider_API_managed.chm" "%FDODOCPATHSHP%"

:end
echo End SHP %MSACTIONSHP%
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
echo There was a build error executing action: %MSACTIONSHP%
exit /B 1

:custom_error
echo The command is not recognized.
echo Please use the format:
:help_show
echo **************************************************************************
echo build.bat [-h] [-o=OutFolder] [-c=BuildType] [-a=Action] [-p=PlatformType] [-d=BuildDocs]
echo *
echo Help:           -h[elp]
echo OutFolder:      -o[utpath]=destination folder for binaries
echo BuildType:      -c[onfig]=release(default), debug
echo PlatformType:   -p[latform]=Win32(default), x64
echo Action:         -a[ction]=build(default), buildinstall, install, clean
echo BuildDocs:      -d[ocs]=skip(default), build
echo **************************************************************************
exit /B 0
