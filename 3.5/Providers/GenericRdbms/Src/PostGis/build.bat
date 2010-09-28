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

:get_action
SET TYPEACTION=%2
if "%2"=="install" goto next_param
if "%2"=="build" goto next_param
if "%2"=="buildinstall" goto next_param
if "%2"=="clean" goto next_param
goto custom_error 

:get_platform
SET TYPEPLATFORM=%2
if "%2"=="Win32" goto next_param
if "%2"=="x64" goto next_param
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

rem SET FDOACTENVSTUDY="FDOPOSTGRES"
rem if ("%FDOPOSTGRES%")==("") goto env_error
rem if not exist "%FDOPOSTGRES%" goto env_path_error

if "%TYPEPLATFORM%"=="Win32" SET INTERMEDIATEDIR=Win32
if "%TYPEPLATFORM%"=="x64" SET INTERMEDIATEDIR=Win64
if "%TYPEPLATFORM%"=="Win32" SET INTERMEDIATEMANAGEDDIR=Win32
if "%TYPEBUILD%"=="debug" SET INTERMEDIATEMANAGEDDIR=Debug
if "%TYPEBUILD%"=="release" SET INTERMEDIATEMANAGEDDIR=Release
if "%TYPEPLATFORM%"=="x64" SET INTERMEDIATEMANAGEDDIR=%INTERMEDIATEMANAGEDDIR%64

if "%TYPEACTION%"=="build" goto start_exbuild
if "%TYPEACTION%"=="clean" goto start_exbuild

if not exist "%FDOINSPATH%" mkdir "%FDOINSPATH%"
if not exist "%FDOBINPATH%" mkdir "%FDOBINPATH%"
if not exist "%FDOINCPATH%" mkdir "%FDOINCPATH%"
if not exist "%FDOLIBPATH%" mkdir "%FDOLIBPATH%"
if not exist "%FDODOCPATH%" mkdir "%FDODOCPATH%"
if not exist "%FDOBINPATH%\com" mkdir "%FDOBINPATH%\com"

:start_exbuild
if "%TYPEACTION%"=="clean" SET MSACTION=Clean
if "%TYPEACTION%"=="install" goto install_files
echo %MSACTION% %TYPEBUILD% PostgreSQL Provider DLLs
SET FDOACTIVEBUILD=%cd%\PostGIS
cscript //Nologo //job:prepare ../../preparebuilds.wsf
msbuild PostGIS_temp.sln /t:%MSACTION% /p:Configuration=%TYPEBUILD% /p:Platform=%TYPEPLATFORM% /nologo /consoleloggerparameters:NoSummary
SET FDOERROR=%errorlevel%
if exist PostGIS_temp.sln del /Q /F PostGIS_temp.sln
if "%FDOERROR%"=="1" goto error
if "%TYPEACTION%"=="clean" goto end
if "%TYPEACTION%"=="build" goto generate_docs

:install_files
echo copy %TYPEBUILD% PostgreSQL provider output files
copy /y "..\..\Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\RdbmsMsg.dll" "%FDOBINPATH%"
copy /y "..\..\Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\RdbmsMsg.pdb" "%FDOBINPATH%"
copy /y "..\..\Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\PostgreSQLProvider.dll" "%FDOBINPATH%"
copy /y "..\..\Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\PostgreSQLProvider.pdb" "%FDOBINPATH%"
copy /y "..\..\Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\PostgreSQLOverrides.dll" "%FDOBINPATH%"
copy /y "..\..\Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\PostgreSQLOverrides.pdb" "%FDOBINPATH%"
copy /y "..\..\Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\RdbmsOverrides.dll" "%FDOBINPATH%"
copy /y "..\..\Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\RdbmsOverrides.pdb" "%FDOBINPATH%"
copy /y "%FDOUTILITIES%\SchemaMgr\Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\SmMessage.dll" "%FDOBINPATH%"
copy /y "%FDOUTILITIES%\ExpressionEngine\Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\ExpressionEngine.dll" "%FDOBINPATH%"
copy /y "%FDOUTILITIES%\ExpressionEngine\Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\ExpressionEngine.pdb" "%FDOBINPATH%"
copy /y "%FDOUTILITIES%\ExpressionEngine\Lib\%INTERMEDIATEDIR%\%TYPEBUILD%\ExpressionEngine.lib" "%FDOLIBPATH%"
copy /y "..\..\Lib\%INTERMEDIATEDIR%\%TYPEBUILD%\RdbmsOverrides.lib" "%FDOLIBPATH%"
copy /y "..\..\Lib\%INTERMEDIATEDIR%\%TYPEBUILD%\PostgreSQLOverrides.lib" "%FDOLIBPATH%"
copy /y "..\..\com\fdosys_sys.sql" "%FDOBINPATH%\com"
copy /y "..\..\com\fdo_sys_idx.sql" "%FDOBINPATH%\com"
copy /y "..\..\com\fdo_sys.sql" "%FDOBINPATH%\com"

rem copy /y "..\..\Managed\Bin\%INTERMEDIATEMANAGEDDIR%\OSGeo.FDO.Providers.PostgreSQL.Overrides.dll" "%FDOBINPATH%"
rem copy /y "..\..\Managed\Bin\%INTERMEDIATEMANAGEDDIR%\OSGeo.FDO.Providers.PostgreSQL.Overrides.pdb" "%FDOBINPATH%"
rem copy /y "..\..\Managed\Bin\%INTERMEDIATEMANAGEDDIR%\OSGeo.FDO.Providers.Rdbms.dll" "%FDOBINPATH%"
rem copy /y "..\..\Managed\Bin\%INTERMEDIATEMANAGEDDIR%\OSGeo.FDO.Providers.Rdbms.pdb" "%FDOBINPATH%"
rem copy /y "..\..\Managed\Bin\%INTERMEDIATEMANAGEDDIR%\OSGeo.FDO.Providers.Rdbms.Overrides.dll" "%FDOBINPATH%"
rem copy /y "..\..\Managed\Bin\%INTERMEDIATEMANAGEDDIR%\OSGeo.FDO.Providers.Rdbms.Overrides.pdb" "%FDOBINPATH%"

echo copy header files
xcopy /S /C /Q /R /Y "..\..\inc\Rdbms\*.h" "%FDOINCPATH%\Rdbms\"
copy /y "..\..\Inc\Rdbms.h" "%FDOINCPATH%\Rdbms\"
if exist "%FDOINCPATH%\Rdbms\Override\Oracle" rmdir /S /Q "%FDOINCPATH%\Rdbms\Override\Oracle"
if exist "%FDOINCPATH%\Rdbms\Override\SqlServer" rmdir /S /Q "%FDOINCPATH%\Rdbms\Override\SqlServer"
if exist "%FDOINCPATH%\Rdbms\FdoSqlServer.h" del /Q /F "%FDOINCPATH%\Rdbms\FdoSqlServer.h"
if exist "%FDOINCPATH%\Rdbms\FdoOracle.h" del /Q /F "%FDOINCPATH%\Rdbms\FdoOracle.h"
xcopy /C /Q /R /Y /I "%FDOUTILITIES%\ExpressionEngine\Inc\*.h" "%FDOINCPATH%\ExpressionEngine"
del /Q/F "%FDOINCPATH%\ExpressionEngine\FdoExpressionEngineImp.h"
xcopy /C /Q /R /Y /I "%FDOUTILITIES%\ExpressionEngine\Inc\Util\*.h" "%FDOINCPATH%\ExpressionEngine\Util"

:generate_docs
if "%DOCENABLE%"=="skip" goto install_docs
rem pushd ..\..\
rem echo Creating PostGIS provider html and chm documentation
rem if exist "Docs\HTML\PostGIS" rmdir /S /Q "Docs\HTML\PostGIS"
rem if not exist "Docs\HTML\PostGIS" mkdir "Docs\HTML\PostGIS"
rem copy ..\..\DocResources\geospatial.js Docs\HTML\PostGIS
rem copy ..\..\DocResources\osgeo.css Docs\HTML\PostGIS
rem if exist Docs\PostGIS_Provider_API.chm attrib -r Docs\PostGIS_Provider_API.chm
rem if exist "Docs\HTML\PostGIS_managed" rmdir /S /Q "Docs\HTML\PostGIS_managed"
rem if not exist "Docs\HTML\PostGIS_managed" mkdir "Docs\HTML\PostGIS_managed"
rem copy ..\..\DocResources\geospatial.js Docs\HTML\PostGIS_managed
rem copy ..\..\DocResources\osgeo.css Docs\HTML\PostGIS_managed
rem if exist Docs\PostGIS_Provider_API_managed.chm attrib -r Docs\PostGIS_Provider_API_managed.chm
rem pushd Docs\doc_src
rem doxygen Doxyfile_PostGIS
rem doxygen Doxyfile_PostGIS_managed
rem popd
rem popd

:install_docs
if "%TYPEACTION%"=="build" goto end
rem pushd ..\..\
rem if exist "%FDODOCPATH%\HTML\Providers\PostGIS" rmdir /S /Q "%FDODOCPATH%\HTML\Providers\PostGIS"
rem if exist Docs\HTML\PostGIS xcopy/CQEYI Docs\HTML\PostGIS\* "%FDODOCPATH%\HTML\Providers\PostGIS"
rem if exist "Docs\PostGIS_Provider_API.chm" copy /y "Docs\PostGIS_Provider_API.chm" "%FDODOCPATH%"
rem if exist "%FDODOCPATH%\HTML\Providers\PostGIS_managed" rmdir /S /Q "%FDODOCPATH%\HTML\Providers\PostGIS_managed"
rem if exist Docs\HTML\PostGIS_managed xcopy/CQEYI Docs\HTML\PostGIS_managed\* "%FDODOCPATH%\HTML\Providers\PostGIS_managed"
rem if exist "Docs\PostGIS_Provider_API_managed.chm" copy /y "Docs\PostGIS_Provider_API_managed.chm" "%FDODOCPATH%"
rem popd

:end
echo End PostgreSQL %MSACTION%
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
echo PlatformType:   -p[latform]=Win32(default), x64
echo Action:         -a[ction]=build(default), buildinstall, install, clean
echo BuildDocs:      -d[ocs]=skip(default), build
echo **************************************************************************
exit /B 0
