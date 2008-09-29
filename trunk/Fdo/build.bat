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

SET FDOORGPATHFDO=%cd%
SET FDOINSPATHFDO=%cd%\Fdo
SET FDOBINPATHFDO=%cd%\Fdo\Bin
SET FDOINCPATHFDO=%cd%\Fdo\Inc
SET FDOLIBPATHFDO=%cd%\Fdo\Lib
SET FDODOCPATHFDO=%cd%\Fdo\Docs
SET DOCENABLEFDO=skip
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
SET DOCENABLEFDO=%2
if "%2"=="build" goto next_param
if "%2"=="skip" goto next_param
goto custom_error

:get_python
SET PYTHONENABLE=%2
if "%2"=="build" goto next_param
if "%2"=="skip" goto next_param
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
SET FDOORGPATHFDO=%~2
SET FDOINSPATHFDO=%~2\Fdo
SET FDOBINPATHFDO=%~2\Fdo\Bin
SET FDOINCPATHFDO=%~2\Fdo\Inc
SET FDOLIBPATHFDO=%~2\Fdo\Lib
SET FDODOCPATHFDO=%~2\Fdo\Docs

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

if "%TYPEACTIONFDO%"=="build" goto start_exbuild
if "%TYPEACTIONFDO%"=="clean" goto start_exbuild
if "%TYPEACTIONFDO%"=="builddocsonly" goto start_exbuild
if not exist "%FDOINSPATHFDO%" mkdir "%FDOINSPATHFDO%"
if not exist "%FDOBINPATHFDO%" mkdir "%FDOBINPATHFDO%"
if not exist "%FDOINCPATHFDO%" mkdir "%FDOINCPATHFDO%"
if not exist "%FDOLIBPATHFDO%" mkdir "%FDOLIBPATHFDO%"
if not exist "%FDODOCPATHFDO%" mkdir "%FDODOCPATHFDO%"

:start_exbuild
if "%TYPEACTIONFDO%"=="clean" SET MSACTIONFDO=Clean
if "%TYPEACTIONFDO%"=="builddocsonly" goto generate_docs
if "%TYPEACTIONFDO%"=="install" goto install_files

echo %MSACTIONFDO% %TYPEBUILDFDO% Fdo dlls
msbuild FDO.sln /t:%MSACTIONFDO% /p:Configuration=%TYPEBUILDFDO% /p:Platform=%TYPEPLATFORMFDO% /nologo /consoleloggerparameters:NoSummary
SET FDOERROR=%errorlevel%
if "%FDOERROR%"=="1" goto error

:rebuild_python
if "%PYTHONENABLE%"=="skip" goto finish_build
if "%TYPEACTIONFDO%"=="clean" goto finish_build
if "%TYPEBUILDFDO%"=="debug" goto finish_build
if not exist Python\build.cmd goto end
pushd Python
call build.cmd
popd
if "%FDOERROR%"=="1" goto error

:finish_build
if "%TYPEACTIONFDO%"=="build" goto generate_docs
if "%TYPEACTIONFDO%"=="clean" goto end
if "%TYPEACTIONFDO%"=="builddocsonly" goto generate_docs

:install_files
echo copy FDO %TYPEBUILDFDO% output files
copy /y "Unmanaged\Bin\Win32\%TYPEBUILDFDO%\FDO.dll" "%FDOBINPATHFDO%"
copy /y "Unmanaged\Bin\Win32\%TYPEBUILDFDO%\FDOMessage.dll" "%FDOBINPATHFDO%"
copy /y "Unmanaged\Bin\Win32\%TYPEBUILDFDO%\FDOCommon.dll" "%FDOBINPATHFDO%"
copy /y "Unmanaged\Bin\Win32\%TYPEBUILDFDO%\FDOGeometry.dll" "%FDOBINPATHFDO%"
copy /y "Unmanaged\Bin\Win32\%TYPEBUILDFDO%\FDOSpatial.dll" "%FDOBINPATHFDO%"
copy /y "Managed\Bin\Win32\%TYPEBUILDFDO%\OSGeo.FDO.dll" "%FDOBINPATHFDO%"
copy /y "Managed\Bin\Win32\%TYPEBUILDFDO%\OSGeo.FDO.Common.dll" "%FDOBINPATHFDO%"
copy /y "Managed\Bin\Win32\%TYPEBUILDFDO%\OSGeo.FDO.Geometry.dll" "%FDOBINPATHFDO%"
copy /y "Managed\Bin\Win32\%TYPEBUILDFDO%\OSGeo.FDO.Spatial.dll" "%FDOBINPATHFDO%"
copy /y "Unmanaged\Lib\Win32\%TYPEBUILDFDO%\FDO.lib" "%FDOLIBPATHFDO%"
copy /y "Unmanaged\Lib\Win32\%TYPEBUILDFDO%\FDOCommon.lib" "%FDOLIBPATHFDO%"
copy /y "Unmanaged\Lib\Win32\%TYPEBUILDFDO%\FDOGeometry.lib" "%FDOLIBPATHFDO%"
copy /y "Unmanaged\Lib\Win32\%TYPEBUILDFDO%\FDOSpatial.lib" "%FDOLIBPATHFDO%"
copy /y "Python\Lib\Win32\FDO.py" "%FDOLIBPATHFDO%"
cscript //Nologo //job:install ../preparebuilds.wsf

echo copy FDO header files
xcopy /S /C /Q /R /Y Unmanaged\Inc\*.h "%FDOINCPATHFDO%\"

:generate_docs
if not "%DOCENABLEFDO%"=="build" goto install_docs
echo Creating FDO Unmanaged and Managed html and chm API documentation
if exist "Docs\HTML\FDO_API" rmdir /S /Q "Docs\HTML\FDO_API"
if exist "Docs\HTML\FDO_API_managed" rmdir /S /Q "Docs\HTML\FDO_API_managed"
if not exist "Docs\HTML\FDO_API" mkdir "Docs\HTML\FDO_API"
if not exist "Docs\HTML\FDO_API_managed" mkdir "Docs\HTML\FDO_API_managed"
if exist Docs\FDO_API.chm attrib -r Docs\FDO_API.chm
if exist Docs\FDO_API_managed.chm attrib -r Docs\FDO_API_managed.chm
pushd Docs\doc_src
doxygen Doxyfile_FDOunmanaged
doxygen Doxyfile_FDOmanaged
popd

:install_docs
if "%TYPEACTIONFDO%"=="build" goto end
if "%TYPEACTIONFDO%"=="clean" goto end
if "%TYPEACTIONFDO%"=="builddocsonly" goto end

pushd Docs\doc_src
if exist "%FDODOCPATHFDO%\HTML\FDO_API" rmdir /S /Q "%FDODOCPATHFDO%\HTML\FDO_API"
if exist "%FDODOCPATHFDO%\HTML\FDO_API_managed" rmdir /S /Q "%FDODOCPATHFDO%\HTML\FDO_API_managed"
if not exist "%FDODOCPATHFDO%\HTML\FDO_API" mkdir "%FDODOCPATHFDO%\HTML\FDO_API"
if not exist "%FDODOCPATHFDO%\HTML\FDO_API_managed" mkdir "%FDODOCPATHFDO%\HTML\FDO_API_managed"

if exist "..\HTML\FDO_API\" xcopy/CQEYI "..\HTML\FDO_API\*" "%FDODOCPATHFDO%\HTML\FDO_API"
if exist "..\HTML\FDO_API_managed\" xcopy/CQEYI "..\HTML\FDO_API_managed\*" "%FDODOCPATHFDO%\HTML\FDO_API_managed"
if exist "..\FDO_API_managed.chm" copy /y "..\FDO_API_managed.chm" "%FDODOCPATHFDO%"
if exist "..\FDO_API.chm" copy /y "..\FDO_API.chm" "%FDODOCPATHFDO%"
if exist "..\FDG_FDODevGuide.pdf" copy /y "..\FDG_FDODevGuide.pdf" "%FDODOCPATHFDO%"
if exist "..\FET_TheEssentialFDO.pdf" copy /y "..\FET_TheEssentialFDO.pdf" "%FDODOCPATHFDO%"
popd

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