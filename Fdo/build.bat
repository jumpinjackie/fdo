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
SET FDOORGPATH=%cd%
SET FDOINSPATH=%cd%\Fdo
SET FDOBINPATH=%cd%\Fdo\Bin
SET FDOINCPATH=%cd%\Fdo\Inc
SET FDOLIBPATH=%cd%\Fdo\Lib
SET FDODOCPATH=%cd%\Fdo\Docs
SET DOCENABLE=skip
SET PYTHONENABLE=skip
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

if "%1"=="-py"       goto get_python
if "%1"=="-python"   goto get_python

goto custom_error

:get_docs
SET DOCENABLE=%2
if "%2"=="build" goto next_param
if "%2"=="skip" goto next_param
goto custom_error

:get_python
SET PYTHONENABLE=%2
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
if "%2"=="builddocsonly" goto next_param
goto custom_error

:get_path
if (%2)==() goto custom_error
SET FDOORGPATH=%~2
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

if "%TYPEBUILD%"=="Win32" SET INTERMEDIATEDIR="Win32"
if "%TYPEBUILD%"=="x64" SET INTERMEDIATEDIR="Win64"

if "%TYPEACTION%"=="build" goto start_exbuild
if "%TYPEACTION%"=="clean" goto start_exbuild
if "%TYPEACTION%"=="builddocsonly" goto start_exbuild

if not exist "%FDOINSPATH%" mkdir "%FDOINSPATH%"
if not exist "%FDOBINPATH%" mkdir "%FDOBINPATH%"
if not exist "%FDOINCPATH%" mkdir "%FDOINCPATH%"
if not exist "%FDOLIBPATH%" mkdir "%FDOLIBPATH%"
if not exist "%FDODOCPATH%" mkdir "%FDODOCPATH%"

:start_exbuild
if "%TYPEACTION%"=="clean" SET MSACTION=Clean
if "%TYPEACTION%"=="builddocsonly" goto generate_docs
if "%TYPEACTION%"=="install" goto install_files

echo %MSACTION% %TYPEBUILD% Fdo dlls
msbuild FDO.sln /t:%MSACTION% /p:Configuration=%TYPEBUILD% /p:Platform=%TYPEPLATFORM% /nologo /consoleloggerparameters:NoSummary
SET FDOERROR=%errorlevel%
if "%FDOERROR%"=="1" goto error

:rebuild_python
if "%PYTHONENABLE%"=="skip" goto finish_build
if "%TYPEACTION%"=="clean" goto finish_build
if "%TYPEBUILD%"=="debug" goto finish_build
if not exist Python\build.cmd goto end
pushd Python
call build.cmd
popd
if "%FDOERROR%"=="1" goto error

:finish_build
if "%TYPEACTION%"=="build" goto generate_docs
if "%TYPEACTION%"=="clean" goto end
if "%TYPEACTION%"=="builddocsonly" goto generate_docs

:install_files
echo copy FDO %TYPEBUILD% output files
copy /y "Unmanaged\Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\FDO.dll" "%FDOBINPATH%"
copy /y "Unmanaged\Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\FDO.pdb" "%FDOBINPATH%"
copy /y "Unmanaged\Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\FDOMessage.dll" "%FDOBINPATH%"
copy /y "Unmanaged\Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\FDOMessage.pdb" "%FDOBINPATH%"
copy /y "Unmanaged\Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\FDOCommon.dll" "%FDOBINPATH%"
copy /y "Unmanaged\Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\FDOCommon.pdb" "%FDOBINPATH%"
copy /y "Unmanaged\Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\FDOGeometry.dll" "%FDOBINPATH%"
copy /y "Unmanaged\Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\FDOGeometry.pdb" "%FDOBINPATH%"
copy /y "Unmanaged\Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\FDOSpatial.dll" "%FDOBINPATH%"
copy /y "Unmanaged\Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\FDOSpatial.pdb" "%FDOBINPATH%"
copy /y "Managed\Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\OSGeo.FDO.dll" "%FDOBINPATH%"
copy /y "Managed\Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\OSGeo.FDO.pdb" "%FDOBINPATH%"
copy /y "Managed\Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\OSGeo.FDO.Common.dll" "%FDOBINPATH%"
copy /y "Managed\Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\OSGeo.FDO.Common.pdb" "%FDOBINPATH%"
copy /y "Managed\Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\OSGeo.FDO.Geometry.dll" "%FDOBINPATH%"
copy /y "Managed\Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\OSGeo.FDO.Geometry.pdb" "%FDOBINPATH%"
copy /y "Managed\Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\OSGeo.FDO.Spatial.dll" "%FDOBINPATH%"
copy /y "Managed\Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\OSGeo.FDO.Spatial.pdb" "%FDOBINPATH%"
copy /y "Unmanaged\Lib\%INTERMEDIATEDIR%\%TYPEBUILD%\FDO.lib" "%FDOLIBPATH%"
copy /y "Unmanaged\Lib\%INTERMEDIATEDIR%\%TYPEBUILD%\FDOCommon.lib" "%FDOLIBPATH%"
copy /y "Unmanaged\Lib\%INTERMEDIATEDIR%\%TYPEBUILD%\FDOGeometry.lib" "%FDOLIBPATH%"
copy /y "Unmanaged\Lib\%INTERMEDIATEDIR%\%TYPEBUILD%\FDOSpatial.lib" "%FDOLIBPATH%"
if exist "Python\Lib\%INTERMEDIATEDIR%\FDO.py" copy /y "Python\Lib\%INTERMEDIATEDIR%\FDO.py" "%FDOLIBPATH%"
cscript //Nologo //job:install ../preparebuilds.wsf

echo copy FDO header files
xcopy /S /C /Q /R /Y Unmanaged\Inc\*.h "%FDOINCPATH%\"

:generate_docs
if not "%DOCENABLE%"=="build" goto install_docs
echo Creating FDO Unmanaged and Managed html and chm API documentation
if exist "Docs\HTML\FDO_API" rmdir /S /Q "Docs\HTML\FDO_API"
if exist "Docs\HTML\FDO_API_managed" rmdir /S /Q "Docs\HTML\FDO_API_managed"
if not exist "Docs\HTML\FDO_API" mkdir "Docs\HTML\FDO_API"
copy ..\DocResources\geospatial.js Docs\HTML\FDO_API
copy ..\DocResources\osgeo.css Docs\HTML\FDO_API
copy ..\DocResources\FDO_architecture.png Docs\HTML\FDO_API
if not exist "Docs\HTML\FDO_API_managed" mkdir "Docs\HTML\FDO_API_managed"
copy ..\DocResources\geospatial.js Docs\HTML\FDO_API_managed
copy ..\DocResources\osgeo.css Docs\HTML\FDO_API_managed
copy ..\DocResources\FDO_architecture.png Docs\HTML\FDO_API_managed
if exist Docs\FDO_API.chm attrib -r Docs\FDO_API.chm
if exist Docs\FDO_API_managed.chm attrib -r Docs\FDO_API_managed.chm
pushd Docs\doc_src
doxygen Doxyfile_FDOunmanaged
doxygen Doxyfile_FDOmanaged
popd

:install_docs
if "%TYPEACTION%"=="build" goto end
if "%TYPEACTION%"=="clean" goto end
if "%TYPEACTION%"=="builddocsonly" goto end

pushd Docs\doc_src
if exist "%FDODOCPATH%\HTML\FDO_API" rmdir /S /Q "%FDODOCPATH%\HTML\FDO_API"
if exist "%FDODOCPATH%\HTML\FDO_API_managed" rmdir /S /Q "%FDODOCPATH%\HTML\FDO_API_managed"
if not exist "%FDODOCPATH%\HTML\FDO_API" mkdir "%FDODOCPATH%\HTML\FDO_API"
if not exist "%FDODOCPATH%\HTML\FDO_API_managed" mkdir "%FDODOCPATH%\HTML\FDO_API_managed"

if exist "..\HTML\FDO_API\" xcopy/CQEYI "..\HTML\FDO_API\*" "%FDODOCPATH%\HTML\FDO_API"
if exist "..\HTML\FDO_API_managed\" xcopy/CQEYI "..\HTML\FDO_API_managed\*" "%FDODOCPATH%\HTML\FDO_API_managed"
if exist "..\FDO_API_managed.chm" copy /y "..\FDO_API_managed.chm" "%FDODOCPATH%"
if exist "..\FDO_API.chm" copy /y "..\FDO_API.chm" "%FDODOCPATH%"
if exist "..\FDG_FDODevGuide.pdf" copy /y "..\FDG_FDODevGuide.pdf" "%FDODOCPATH%"
if exist "..\FET_TheEssentialFDO.pdf" copy /y "..\FET_TheEssentialFDO.pdf" "%FDODOCPATH%"
popd

:end
echo End FDO %MSACTION%
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
echo ********************************************************************************
echo build.bat [-h]
echo           [-o=OutFolder]
echo           [-c=BuildType]
echo           [-p=PlatformType]
echo           [-a=Action]
echo           [-d=BuildDocs]
echo           [-py=BuildPythonWrappers]
echo *
echo Help:                  -h[elp]
echo OutFolder:             -o[utpath]=destination folder for binaries
echo BuildType:             -c[onfig]=release(default), debug
echo PlatformType:          -p[latform]=Win32(default), x64
echo Action:                -a[ction]=build(default), 
echo                                  buildinstall, 
echo                                  install, 
echo                                  clean, 
echo                                  builddocsonly, 
echo BuildDocs:             -d[ocs]=skip(default), build
echo BuildPythonWrappers:   -py[thon]=skip(default), build
echo ********************************************************************************
exit /B 0
