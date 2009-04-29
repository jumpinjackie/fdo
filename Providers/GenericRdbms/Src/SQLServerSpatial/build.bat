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

SET TYPEACTIONSQLSPATIAL=build
SET MSACTIONSQLSPATIAL=Build
SET TYPEBUILDSQLSPATIAL=release
SET FDOINSPATHSQLSPATIAL=\Fdo
SET FDOBINPATHSQLSPATIAL=\Fdo\Bin
SET FDOINCPATHSQLSPATIAL=\Fdo\Inc
SET FDOLIBPATHSQLSPATIAL=\Fdo\Lib
SET DOCENABLESQLSPATIAL=skip
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
SET DOCENABLESQLSPATIAL=%2
if "%2"=="build" goto next_param
if "%2"=="skip" goto next_param
goto custom_error

:get_conf 
SET TYPEBUILDSQLSPATIAL=%2
if "%2"=="release" goto next_param
if "%2"=="debug" goto next_param
goto custom_error

:get_action
SET TYPEACTIONSQLSPATIAL=%2
if "%2"=="install" goto next_param
if "%2"=="build" goto next_param
if "%2"=="buildinstall" goto next_param
if "%2"=="clean" goto next_param
goto custom_error 

:get_path
if (%2)==() goto custom_error
SET FDOINSPATHSQLSPATIAL=%~2\Fdo
SET FDOBINPATHSQLSPATIAL=%~2\Fdo\Bin
SET FDOINCPATHSQLSPATIAL=%~2\Fdo\Inc
SET FDOLIBPATHSQLSPATIAL=%~2\Fdo\Lib
SET FDODOCPATHSQLSPATIAL=%~2\Fdo\Docs

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

if "%TYPEACTIONSQLSPATIAL%"=="build" goto start_exbuild
if "%TYPEACTIONSQLSPATIAL%"=="clean" goto start_exbuild
if not exist "%FDOINSPATHSQLSPATIAL%" mkdir "%FDOINSPATHSQLSPATIAL%"
if not exist "%FDOBINPATHSQLSPATIAL%" mkdir "%FDOBINPATHSQLSPATIAL%"
if not exist "%FDOINCPATHSQLSPATIAL%" mkdir "%FDOINCPATHSQLSPATIAL%"
if not exist "%FDOLIBPATHSQLSPATIAL%" mkdir "%FDOLIBPATHSQLSPATIAL%"
if not exist "%FDODOCPATHSQLSPATIAL%" mkdir "%FDODOCPATHSQLSPATIAL%"
if not exist "%FDOBINPATHSQLSPATIAL%\com" mkdir "%FDOBINPATHSQLSPATIAL%\com"

:start_exbuild
if "%TYPEACTIONSQLSPATIAL%"=="clean" SET MSACTIONSQLSPATIAL=Clean
if "%TYPEACTIONSQLSPATIAL%"=="install" goto install_files_sqlspatial

echo %MSACTIONSQLSPATIAL% %TYPEBUILDSQLSPATIAL% SQLServer Spatial Provider DLLs
SET FDOACTIVEBUILD=%cd%\SQLServerSpatial
cscript //Nologo //job:prepare ../../preparebuilds.wsf
msbuild SQLServerSpatial_temp.sln /t:%MSACTIONSQLSPATIAL% /p:Configuration=%TYPEBUILDSQLSPATIAL% /p:Platform="Win32" /nologo /consoleloggerparameters:NoSummary
SET FDOERROR=%errorlevel%
if exist SQLServerSpatial_temp.sln del /Q /F SQLServerSpatial_temp.sln
if "%FDOERROR%"=="1" goto error
if "%TYPEACTIONSQLSPATIAL%"=="clean" goto end
if "%TYPEACTIONSQLSPATIAL%"=="build" goto generate_docs

:install_files_sqlspatial
echo copy %TYPEBUILDSQLSPATIAL% SQLServer Spatial Provider Output Files
copy /y "..\..\Bin\Win32\%TYPEBUILDSQLSPATIAL%\RdbmsMsg.dll" "%FDOBINPATHSQLSPATIAL%"
copy /y "..\..\Bin\Win32\%TYPEBUILDSQLSPATIAL%\RdbmsMsg.pdb" "%FDOBINPATHSQLSPATIAL%"
copy /y "..\..\Bin\Win32\%TYPEBUILDSQLSPATIAL%\SQLServerSpatialProvider.dll" "%FDOBINPATHSQLSPATIAL%"
copy /y "..\..\Bin\Win32\%TYPEBUILDSQLSPATIAL%\SQLServerSpatialProvider.pdb" "%FDOBINPATHSQLSPATIAL%"
copy /y "..\..\Bin\Win32\%TYPEBUILDSQLSPATIAL%\SQLServerSpatialOverrides.dll" "%FDOBINPATHSQLSPATIAL%"
copy /y "..\..\Bin\Win32\%TYPEBUILDSQLSPATIAL%\SQLServerSpatialOverrides.pdb" "%FDOBINPATHSQLSPATIAL%"
copy /y "..\..\Bin\Win32\%TYPEBUILDSQLSPATIAL%\RdbmsOverrides.dll" "%FDOBINPATHSQLSPATIAL%"
copy /y "..\..\Bin\Win32\%TYPEBUILDSQLSPATIAL%\RdbmsOverrides.pdb" "%FDOBINPATHSQLSPATIAL%"
copy /y "%FDOUTILITIES%\SchemaMgr\Bin\Win32\%TYPEBUILDSQLSPATIAL%\SmMessage.dll" "%FDOBINPATHSQLSPATIAL%"
copy /y "%FDOUTILITIES%\ExpressionEngine\bin\win32\%TYPEBUILDSQLSPATIAL%\ExpressionEngine.dll" "%FDOBINPATHSQLSPATIAL%"
copy /y "%FDOUTILITIES%\ExpressionEngine\bin\win32\%TYPEBUILDSQLSPATIAL%\ExpressionEngine.pdb" "%FDOBINPATHSQLSPATIAL%"
copy /y "%FDOUTILITIES%\ExpressionEngine\lib\win32\%TYPEBUILDSQLSPATIAL%\ExpressionEngine.lib" "%FDOLIBPATHSQLSPATIAL%"
copy /y "..\..\Lib\Win32\%TYPEBUILDSQLSPATIAL%\RdbmsOverrides.lib" "%FDOLIBPATHSQLSPATIAL%"
copy /y "..\..\Lib\Win32\%TYPEBUILDSQLSPATIAL%\SQLServerSpatialOverrides.lib" "%FDOLIBPATHSQLSPATIAL%"
copy /y "..\..\com\fdosys_sys.sql" "%FDOBINPATHSQLSPATIAL%\com"
copy /y "..\..\com\fdo_sys_idx.sql" "%FDOBINPATHSQLSPATIAL%\com"
copy /y "..\..\com\fdo_sys.sql" "%FDOBINPATHSQLSPATIAL%\com"
copy /y "..\..\Managed\bin\%TYPEBUILDSQLSPATIAL%\OSGeo.FDO.Providers.SQLServerSpatial.Overrides.dll" "%FDOBINPATHSQLSPATIAL%"
copy /y "..\..\Managed\bin\%TYPEBUILDSQLSPATIAL%\OSGeo.FDO.Providers.SQLServerSpatial.Overrides.pdb" "%FDOBINPATHSQLSPATIAL%"
copy /y "..\..\Managed\bin\%TYPEBUILDSQLSPATIAL%\OSGeo.FDO.Providers.Rdbms.dll" "%FDOBINPATHSQLSPATIAL%"
copy /y "..\..\Managed\bin\%TYPEBUILDSQLSPATIAL%\OSGeo.FDO.Providers.Rdbms.pdb" "%FDOBINPATHSQLSPATIAL%"
copy /y "..\..\Managed\bin\%TYPEBUILDSQLSPATIAL%\OSGeo.FDO.Providers.Rdbms.Overrides.dll" "%FDOBINPATHSQLSPATIAL%"
copy /y "..\..\Managed\bin\%TYPEBUILDSQLSPATIAL%\OSGeo.FDO.Providers.Rdbms.Overrides.pdb" "%FDOBINPATHSQLSPATIAL%"

echo copy header files
xcopy /S /C /Q /R /Y "..\..\inc\Rdbms\*.h" "%FDOINCPATHSQLSPATIAL%\Rdbms\"
copy /y "..\..\Inc\Rdbms.h" "%FDOINCPATHSQLSPATIAL%\Rdbms\"
if exist "%FDOINCPATHSQLSPATIAL%\Rdbms\Override\PostGis" rmdir /S /Q "%FDOINCPATHSQLSPATIAL%\Rdbms\Override\PostGis"
if exist "%FDOINCPATHSQLSPATIAL%\Rdbms\Override\Oracle" rmdir /S /Q "%FDOINCPATHSQLSPATIAL%\Rdbms\Override\Oracle"
if exist "%FDOINCPATHSQLSPATIAL%\Rdbms\Override\SqlServer" rmdir /S /Q "%FDOINCPATHSQLSPATIAL%\Rdbms\Override\SqlServer"
if exist "%FDOINCPATHSQLSPATIAL%\Rdbms\FdoSqlServer.h" del /Q /F "%FDOINCPATHSQLSPATIAL%\Rdbms\FdoSqlServer.h"
if exist "%FDOINCPATHSQLSPATIAL%\Rdbms\FdoOracle.h" del /Q /F "%FDOINCPATHSQLSPATIAL%\Rdbms\FdoOracle.h"

:generate_docs
if "%DOCENABLESQLSPATIAL%"=="skip" goto install_docs
pushd ..\..\
echo Creating SQLServer Spatial provider html and chm documentation
if exist "Docs\HTML\SQLServerSpatial" rmdir /S /Q "Docs\HTML\SQLServerSpatial"
if not exist "Docs\HTML\SQLServerSpatial" mkdir "Docs\HTML\SQLServerSpatial"
copy ..\..\DocResources\geospatial.js Docs\HTML\SQLServerSpatial
copy ..\..\DocResources\osgeo.css Docs\HTML\SQLServerSpatial
if exist Docs\SQLServerSpatial_Provider_API.chm attrib -r Docs\SQLServerSpatial_Provider_API.chm
if exist "Docs\HTML\SQLServerSpatial_managed" rmdir /S /Q "Docs\HTML\SQLServerSpatial_managed"
if not exist "Docs\HTML\SQLServerSpatial_managed" mkdir "Docs\HTML\SQLServerSpatial_managed"
copy ..\..\DocResources\geospatial.js Docs\HTML\SQLServerSpatial_managed
copy ..\..\DocResources\osgeo.css Docs\HTML\SQLServerSpatial_managed
if exist Docs\SQLServerSpatial_Provider_API_managed.chm attrib -r Docs\SQLServerSpatial_Provider_API_managed.chm
pushd Docs\doc_src
doxygen Doxyfile_SQLServerSpatial
doxygen Doxyfile_SQLServerSpatial_managed
popd
popd

:install_docs
if "%TYPEACTIONSQLSPATIAL%"=="build" goto end
pushd ..\..\
if exist "%FDODOCPATHSQLSPATIAL%\HTML\Providers\SQLServerSpatial" rmdir /S /Q "%FDODOCPATHSQLSPATIAL%\HTML\Providers\SQLServerSpatial"
if exist Docs\HTML\SQLServerSpatial xcopy/CQEYI Docs\HTML\SQLServerSpatial\* "%FDODOCPATHSQLSPATIAL%\HTML\Providers\SQLServerSpatial"
if exist "Docs\SQLServerSpatial_Provider_API.chm" copy /y "Docs\SQLServerSpatial_Provider_API.chm" "%FDODOCPATHSQLSPATIAL%"
if exist "%FDODOCPATHSQLSPATIAL%\HTML\Providers\SQLServerSpatial_managed" rmdir /S /Q "%FDODOCPATHSQLSPATIAL%\HTML\Providers\SQLServerSpatial_managed"
if exist Docs\HTML\SQLServerSpatial_managed xcopy/CQEYI Docs\HTML\SQLServerSpatial_managed\* "%FDODOCPATHSQLSPATIAL%\HTML\Providers\SQLServerSpatial_managed"
if exist "Docs\SQLServerSpatial_Provider_API_managed.chm" copy /y "Docs\SQLServerSpatial_Provider_API_managed.chm" "%FDODOCPATHSQLSPATIAL%"
popd

:end
echo End SQLServer Spatial %MSACTIONSQLSPATIAL%
exit /B 0

:env_error
echo Environment Variable Undefined: %FDOACTENVSTUDY%
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
echo There was a build error executing action: %MSACTIONSQLSPATIAL%
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
