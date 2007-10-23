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
SET TYPEBUILD=release
SET TYPEPLATFORM=Win32
SET FDOORGPATH=

SET DEFMODIFY=no
SET SHPENABLE=yes
SET SDFENABLE=yes
SET WFSENABLE=yes
SET WMSENABLE=yes
SET ARCENABLE=yes
SET ODBCENABLE=yes
SET MYSQLENABLE=yes
SET GDALENABLE=yes
SET OGRENABLE=yes
SET POSTGISENABLE=yes
SET FDOENABLE=yes
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

if "%1"=="-w"       goto get_with
if "%1"=="-with"    goto get_with

if "%1"=="-d"       goto get_docs
if "%1"=="-docs"    goto get_docs

if "%1"=="-py"      goto get_python
if "%1"=="-python"  goto get_python

goto custom_error

:get_with
if (%2)==() goto custom_error

if "%DEFMODIFY%"=="yes" goto stp1_get_with
	SET DEFMODIFY=yes
	SET SHPENABLE=no
	SET SDFENABLE=no
	SET WFSENABLE=no
	SET WMSENABLE=no
	SET ARCENABLE=no
	SET ODBCENABLE=no
	SET MYSQLENABLE=no
	SET FDOENABLE=no
	SET GDALENABLE=no
	SET OGRENABLE=no
	SET POSTGISENABLE=no
:stp1_get_with
if not "%2"=="shp" goto stp2_get_with
	SET SHPENABLE=yes
	goto next_param
:stp2_get_with
if not "%2"=="sdf" goto stp3_get_with
	SET SDFENABLE=yes
	goto next_param
:stp3_get_with
if not "%2"=="wfs" goto stp4_get_with
	SET WFSENABLE=yes
	goto next_param
:stp4_get_with
if not "%2"=="wms" goto stp5_get_with
	SET WMSENABLE=yes
	goto next_param
:stp5_get_with
if not "%2"=="arcsde" goto stp6_get_with
	SET ARCENABLE=yes
	goto next_param
:stp6_get_with
if not "%2"=="odbc" goto stp7_get_with
	SET ODBCENABLE=yes
	goto next_param
:stp7_get_with
if not "%2"=="mysql" goto stp8_get_with
	SET MYSQLENABLE=yes
	goto next_param
:stp8_get_with
if not "%2"=="gdal" goto stp9_get_with
	SET GDALENABLE=yes
	goto next_param
:stp9_get_with
if not "%2"=="ogr" goto stp10_get_with
	SET OGRENABLE=yes
	goto next_param
:stp10_get_with
if not "%2"=="postgis" goto stp11_get_with
	SET POSTGISENABLE=yes
	goto next_param
:stp11_get_with
if not "%2"=="fdo" goto stp12_get_with
	SET FDOENABLE=yes
	goto next_param
:stp12_get_with
if not "%2"=="providers" goto stp13_get_with
	SET SHPENABLE=yes
	SET SDFENABLE=yes
	SET WFSENABLE=yes
	SET WMSENABLE=yes
	SET ARCENABLE=yes
	SET ODBCENABLE=yes
	SET MYSQLENABLE=yes
	SET GDALENABLE=yes
	SET OGRENABLE=yes
	SET POSTGISENABLE=yes
goto next_param
:stp13_get_with
if not "%2"=="all" goto custom_error
	SET SHPENABLE=yes
	SET SDFENABLE=yes
	SET WFSENABLE=yes
	SET WMSENABLE=yes
	SET ARCENABLE=yes
	SET ODBCENABLE=yes
	SET MYSQLENABLE=yes
	SET FDOENABLE=yes
	SET GDALENABLE=yes
	SET OGRENABLE=yes
	SET POSTGISENABLE=yes
	goto next_param

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

:get_action
SET TYPEACTION=%2
if "%2"=="install" goto next_param
if "%2"=="build" goto next_param
if "%2"=="buildinstall" goto next_param
if "%2"=="clean" goto next_param
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

:get_path
if (%2)==() goto custom_error
if not exist "%~2" mkdir "%~2"
pushd "%~2"
SET FDOORGPATH=%cd%
popd

:next_param
shift
shift
goto study_params

:start_build
if ("%FDO%")==("") SET FDO=%cd%\Fdo
if ("%FDOTHIRDPARTY%")==("") SET FDOTHIRDPARTY=%cd%\Thirdparty
if ("%FDOUTILITIES%")==("") SET FDOUTILITIES=%cd%\Utilities

if "%TYPEACTION%"=="build" goto start_exbuild
if "%TYPEACTION%"=="clean" goto start_exbuild
if not ("%FDOORGPATH%")==("") goto start_exbuildinstall
echo Please provide destination folder location using '-o' option.
exit /B 1

:start_exbuildinstall
if not exist "%FDOORGPATH%" mkdir "%FDOORGPATH%"

:start_exbuild
SET PROVCALLCMDEXFDO=-o="%FDOORGPATH%" -c=%TYPEBUILD% -p=%TYPEPLATFORM% -a=%TYPEACTION% -d=%DOCENABLE% -py=%PYTHONENABLE%
SET PROVCALLCMDEXSDF=-o="%FDOORGPATH%" -c=%TYPEBUILD% -p=%TYPEPLATFORM% -a=%TYPEACTION% -d=%DOCENABLE%
SET PROVCALLCMDEX=-o="%FDOORGPATH%" -c=%TYPEBUILD% -a=%TYPEACTION% -d=%DOCENABLE%

:rebuild_fdo
if "%FDOENABLE%"=="no" goto rebuild_shp
pushd Fdo
call build.bat %PROVCALLCMDEXFDO%
popd
if "%FDOERROR%"=="1" goto error

:rebuild_shp
if "%SHPENABLE%"=="no" goto rebuild_sdf
if not exist Providers\SHP\build.bat goto rebuild_sdf
pushd Providers\SHP
call build.bat %PROVCALLCMDEX%
popd
if "%FDOERROR%"=="1" goto error

:rebuild_sdf
if "%SDFENABLE%"=="no" goto rebuild_wfs
if not exist Providers\SDF\build.bat goto rebuild_wfs
pushd Providers\SDF
call build.bat %PROVCALLCMDEXSDF%
popd
if "%FDOERROR%"=="1" goto error

:rebuild_wfs
if "%WFSENABLE%"=="no" goto rebuild_wms
if not exist Providers\WFS\build.bat goto rebuild_wms
pushd Providers\WFS
call build.bat %PROVCALLCMDEX%
popd
if "%FDOERROR%"=="1" goto error

:rebuild_wms
if "%WMSENABLE%"=="no" goto rebuild_arc
if not exist Providers\WMS\build.bat goto rebuild_arc
pushd Providers\WMS
call build.bat %PROVCALLCMDEX%
popd
if "%FDOERROR%"=="1" goto error

:rebuild_arc
if "%ARCENABLE%"=="no" goto rebuild_odbc
if not exist "%SDEHOME%" goto rebuild_odbc
if not exist Providers\ArcSDE\build.bat goto rebuild_odbc
pushd Providers\ArcSDE
call build.bat %PROVCALLCMDEX%
popd
if "%FDOERROR%"=="1" goto error

:rebuild_odbc
if "%ODBCENABLE%"=="no" goto rebuild_mysql
if not exist Providers\GenericRdbms\Src\ODBC\build.bat goto rebuild_mysql
pushd Providers\GenericRdbms\Src\ODBC
call build.bat %PROVCALLCMDEX%
popd
if "%FDOERROR%"=="1" goto error

:rebuild_mysql
if "%MYSQLENABLE%"=="no" goto rebuild_gdal
if not exist "%FDOMYSQL%" goto rebuild_gdal
if not exist Providers\GenericRdbms\Src\MySQL\build.bat goto rebuild_gdal
pushd Providers\GenericRdbms\Src\MySQL
call build.bat %PROVCALLCMDEX%
popd
if "%FDOERROR%"=="1" goto error

:rebuild_gdal
if "%GDALENABLE%"=="no" goto rebuild_ogr
if not exist Providers\GDAL\build.bat goto rebuild_ogr
pushd Providers\GDAL
call build.bat %PROVCALLCMDEX%
popd
if "%FDOERROR%"=="1" goto error

:rebuild_ogr
if "%OGRENABLE%"=="no" goto rebuild_postgis
if not exist Providers\OGR\build.bat goto rebuild_postgis
pushd Providers\OGR
call build.bat %PROVCALLCMDEX%
popd
if "%FDOERROR%"=="1" goto error

:rebuild_postgis
if "%POSTGISENABLE%"=="no" goto end
if not exist Providers\PostGIS\build.bat goto end
pushd Providers\PostGIS
call build.bat %PROVCALLCMDEX%
popd
if "%FDOERROR%"=="1" goto error

:end
echo End Build
exit /B 0

:error
echo There was a build error.
exit /B 1

:custom_error
echo The command is not recognized.
echo Please use the format:
:help_show
echo ************************************************************************
echo build.bat [-h]
echo           [-o=OutFolder]
echo           [-c=BuildType]
echo           [-p=PlatformType]
echo           [-a=Action]
echo           [-w=WithModule]
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
echo                                  clean
echo BuildDocs:             -d[ocs]=skip(default), build
echo BuildPythonWrappers:   -py[thon]=skip(default), build
SET MROVBYPROVP=
SET MPROVECAPABP=WithModule:            -w[ith]=all(default), fdo
:shp_check
if not exist Providers\SHP\build.bat goto sdf_check
	SET MROVBYPROVP=%MROVBYPROVP%, shp
:sdf_check
if not exist Providers\SDF\build.bat goto wfs_check
	SET MROVBYPROVP=%MROVBYPROVP%, sdf
:wfs_check
if not exist Providers\WFS\build.bat goto wms_check
	SET MROVBYPROVP=%MROVBYPROVP%, wfs
:wms_check
if not exist Providers\WMS\build.bat goto arc_check
	SET MROVBYPROVP=%MROVBYPROVP%, wms
:arc_check
if not exist Providers\ArcSDE\build.bat goto odbc_check
	SET MROVBYPROVP=%MROVBYPROVP%, arcsde
:odbc_check
if not exist Providers\GenericRdbms\Src\ODBC\build.bat goto mysql_check
	SET MROVBYPROVP=%MROVBYPROVP%, odbc
:mysql_check
if not exist Providers\GenericRdbms\Src\MySQL\build.bat goto gdal_check
	SET MROVBYPROVP=%MROVBYPROVP%, mysql
:gdal_check
if not exist Providers\GDAL\build.bat goto ogr_check
	SET MROVBYPROVP=%MROVBYPROVP%, gdal
:ogr_check
if not exist Providers\OGR\build.bat goto postgis_check
	SET MROVBYPROVP=%MROVBYPROVP%, ogr
:postgis_check
if not exist Providers\PostGIS\build.bat goto providers_show
	SET MROVBYPROVP=%MROVBYPROVP%, postgis
:providers_show
if ("%MROVBYPROVP%")==("") goto show_capabilities
	SET MPROVECAPABP=%MPROVECAPABP%, providers%MROVBYPROVP%
:show_capabilities
echo %MPROVECAPABP%
echo ************************************************************************
exit /B 0