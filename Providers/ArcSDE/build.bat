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

SET TYPEACTION=build
SET MSACTION=Build
SET TYPEBUILD=release
SET TYPEPLATFORM=Win32
SET INTERMEDIATEDIR=Win32
SET FDOINSPATH=\Fdo
SET FDOBINPATH=\Fdo\Bin
SET FDOINCPATH=\Fdo\Inc
SET FDOLIBPATH=\Fdo\Lib
SET FDODOCPATH=\Fdo\Docs
SET DOCENABLE=skip
SET FDOERROR=0
SET ARCSDEVERSIONACTIVE=9

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

if "%1"=="-d"       goto get_docs
if "%1"=="-docs"    goto get_docs

goto custom_error

:get_docs
SET DOCENABLE=%2
if "%2"=="build" goto next_param
if "%2"=="skip" goto next_param
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

:get_action
SET TYPEACTION=%2
if "%2"=="install" goto next_param
if "%2"=="build" goto next_param
if "%2"=="buildinstall" goto next_param
if "%2"=="clean" goto next_param
goto custom_error 

:get_path
if (%2)==() goto custom_error
SET FDOINSPATH=%~2\Fdo
SET FDOBINPATH=%~2\Fdo\Bin
SET FDOINCPATH=%~2\Fdo\Inc
SET FDOLIBPATH=%~2\Fdo\Lib
SET FDODOCPATH=%~2\Fdo\Docs

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
SET FDOACTENVSTUDY="SDEHOME"
if ("%SDEHOME%")==("") goto env_error
if not exist "%SDEHOME%" goto env_path_error

if not exist "%FDOINSPATH%" mkdir "%FDOINSPATH%"
if not exist "%FDOBINPATH%" mkdir "%FDOBINPATH%"
if not exist "%FDOLIBPATH%" mkdir "%FDOLIBPATH%"
if not exist "%FDOINCPATH%" mkdir "%FDOINCPATH%"
if not exist "%FDOLIBPATH%" mkdir "%FDOLIBPATH%"
if not exist "%FDODOCPATH%" mkdir "%FDODOCPATH%"

if exist "%SDEHOME%\bin\sde.dll" SET SDEVER_ARCUNITTEST=92
if exist "%SDEHOME%\bin\sde91.dll" SET SDEVER_ARCUNITTEST=91

if exist "%FDOTHIRDPARTY%\ESRI\ArcSDEClient91\Windows\bin\sde91.dll" SET ARCSDEVERSIONACTIVE=%ARCSDEVERSIONACTIVE%1
if exist "%SDEHOME%\bin\sde.dll" SET ARCSDEVERSIONACTIVE=%ARCSDEVERSIONACTIVE%2

if "%ARCSDEVERSIONACTIVE%"=="9" SET ARCSDEVERSIONACTIVE=%TYPEBUILD%%SDEVER_ARCUNITTEST%Only
if "%ARCSDEVERSIONACTIVE%"=="912" SET ARCSDEVERSIONACTIVE=%TYPEBUILD%
if "%ARCSDEVERSIONACTIVE%"=="91" SET ARCSDEVERSIONACTIVE=%TYPEBUILD%91Only
if "%ARCSDEVERSIONACTIVE%"=="92" SET ARCSDEVERSIONACTIVE=%TYPEBUILD%92Only

if "%TYPEPLATFORM%"=="Win32" SET INTERMEDIATEDIR="Win32"
if "%TYPEPLATFORM%"=="x64" SET INTERMEDIATEDIR="Win64"

:start_setbuild
if "%TYPEACTION%"=="clean" SET MSACTION=Clean
if "%TYPEACTION%"=="install" goto install_files_ArcSDE

echo %MSACTION% %TYPEBUILD% ArcSDE provider dlls
SET FDOACTIVEBUILD=%cd%\Src\ArcSDE
cscript //Nologo //job:prepare preparebuilds.wsf
pushd Src

msbuild ArcSDE_temp.sln /t:%MSACTION% /p:Configuration=%ARCSDEVERSIONACTIVE% /p:Platform=%TYPEPLATFORM% /nologo /consoleloggerparameters:NoSummary

SET FDOERROR=%errorlevel%
if exist ArcSDE_temp.sln del /Q /F ArcSDE_temp.sln
popd
if "%FDOERROR%"=="1" goto error
if "%TYPEACTION%"=="clean" goto end
if "%TYPEACTION%"=="build" goto generate_docs

:install_files_ArcSDE
echo copy %TYPEBUILD% ArcSDE provider output files
copy /y "Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\ArcSDEMessage.dll" "%FDOBINPATH%"
copy /y "Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\ArcSDEMessage.pdb" "%FDOBINPATH%"
copy /y "Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\ArcSDEProvider.dll" "%FDOBINPATH%"
copy /y "Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\ArcSDEProvider.pdb" "%FDOBINPATH%"
copy /y "%FDOUTILITIES%\ExpressionEngine\bin\%INTERMEDIATEDIR%\%TYPEBUILD%\ExpressionEngine.dll" "%FDOBINPATH%"
copy /y "%FDOUTILITIES%\ExpressionEngine\bin\%INTERMEDIATEDIR%\%TYPEBUILD%\ExpressionEngine.pdb" "%FDOBINPATH%"
copy /y "%FDOUTILITIES%\ExpressionEngine\lib\%INTERMEDIATEDIR%\%TYPEBUILD%\ExpressionEngine.lib" "%FDOLIBPATH%"
if exist "Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\ArcSDEProvider91.dll" copy /y "Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\ArcSDEProvider91.dll" "%FDOBINPATH%"
if exist "Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\ArcSDEProvider91.pdb" copy /y "Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\ArcSDEProvider91.pdb" "%FDOBINPATH%"
if exist "Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\ArcSDEProvider92.dll" copy /y "Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\ArcSDEProvider92.dll" "%FDOBINPATH%"
if exist "Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\ArcSDEProvider92.pdb" copy /y "Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\ArcSDEProvider92.pdb" "%FDOBINPATH%"

echo copy header files
xcopy /C /Q /R /Y /I "%FDOUTILITIES%\ExpressionEngine\Inc\*.h" "%FDOINCPATH%\ExpressionEngine"
del /Q/F "%FDOINCPATH%\ExpressionEngine\FdoExpressionEngineImp.h"
xcopy /C /Q /R /Y /I "%FDOUTILITIES%\ExpressionEngine\Inc\Util\*.h" "%FDOINCPATH%\ExpressionEngine\Util"

:generate_docs
if "%DOCENABLE%"=="skip" goto install_docs
echo Creating ArcSDE provider html and chm documentation
if exist "Docs\HTML\ArcSDE" rmdir /S /Q "Docs\HTML\ArcSDE"
if not exist "Docs\HTML\ArcSDE" mkdir "Docs\HTML\ArcSDE"
copy ..\..\DocResources\geospatial.js Docs\HTML\ArcSDE
copy ..\..\DocResources\osgeo.css Docs\HTML\ArcSDE
if exist Docs\ArcSDE_Provider_API.chm attrib -r Docs\ArcSDE_Provider_API.chm
pushd Docs\doc_src
doxygen Doxyfile_ArcSDE
popd

:install_docs
if "%TYPEACTION%"=="build" goto end
if exist "%FDODOCPATH%\HTML\Providers\ArcSDE" rmdir /S /Q "%FDODOCPATH%\HTML\Providers\ArcSDE"
if exist Docs\HTML\ArcSDE xcopy/CQEYI Docs\HTML\ArcSDE\* "%FDODOCPATH%\HTML\Providers\ArcSDE"
if exist "Docs\ArcSDE_Provider_API.chm" copy /y "Docs\ArcSDE_Provider_API.chm" "%FDODOCPATH%"

:end
echo End ArcSDE %MSACTION%
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
echo           [-d=BuildDocs]
echo *
echo Help:           -h[elp]
echo OutFolder:      -o[utpath]=destination folder for binaries
echo BuildType:      -c[onfig]=release(default), debug
echo Action:         -a[ction]=build(default), buildinstall, install, clean
echo PlatformType:   -p[latform]=Win32(default), x64
echo BuildDocs:      -d[ocs]=skip(default), build
echo **************************************************************************
exit /B 0
