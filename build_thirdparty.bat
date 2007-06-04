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
SET FDOORGPATH=

SET DEFMODIFY=no
SET ALLENABLE=yes
SET SDFENABLE=no
SET WFSENABLE=no
SET WMSENABLE=no
SET GDALENABLE=no
SET FDOENABLE=no
SET POSTGISENABLE=no
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

if "%1"=="-w"       goto get_with
if "%1"=="-with"    goto get_with

goto custom_error

:get_with
if (%2)==() goto custom_error

if "%DEFMODIFY%"=="yes" goto stp1_get_with
	SET DEFMODIFY=yes
	SET ALLENABLE=no
	SET SDFENABLE=no
	SET WFSENABLE=no
	SET WMSENABLE=no
	SET GDALENABLE=no
	SET POSTGISENABLE=no
	SET FDOENABLE=no
:stp1_get_with
if not "%2"=="sdf" goto stp2_get_with
	SET SDFENABLE=yes
	goto next_param
:stp2_get_with
if not "%2"=="wfs" goto stp3_get_with
	SET WFSENABLE=yes
	goto next_param
:stp3_get_with
if not "%2"=="wms" goto stp4_get_with
	SET WMSENABLE=yes
	goto next_param
:stp4_get_with
if not "%2"=="postgis" goto stp5_get_with
	SET POSTGISENABLE=yes
	goto next_param
:stp5_get_with
if not "%2"=="gdal" goto stp6_get_with
	SET GDALENABLE=yes
	goto next_param
:stp6_get_with
if not "%2"=="fdo" goto stp7_get_with
	SET FDOENABLE=yes
	goto next_param
:stp7_get_with
if not "%2"=="providers" goto stp8_get_with
	SET SDFENABLE=yes
	SET WFSENABLE=yes
	SET WMSENABLE=yes
	SET GDALENABLE=yes
	SET POSTGISENABLE=yes
	goto next_param
:stp8_get_with
if not "%2"=="all" goto custom_error
	SET ALLENABLE=yes
	SET SDFENABLE=no
	SET WFSENABLE=no
	SET WMSENABLE=no
	SET GDALENABLE=no
	SET POSTGISENABLE=no
	SET FDOENABLE=no
    goto next_param

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
echo Please provide destination binaries folder using '-o' option.
exit /B 1

:start_exbuildinstall
if not exist "%FDOORGPATH%" mkdir "%FDOORGPATH%"

:start_exbuild
SET PROVCALLCMD=
SET PROVCALLCMDEX=-o="%FDOORGPATH%" -c=%TYPEBUILD% -a=%TYPEACTION% -d=%DOCENABLE%

:study_rebuild_all
if "%ALLENABLE%"=="no" goto study_rebuild_sdf
SET PROVCALLCMD=%PROVCALLCMD% -w=all

:study_rebuild_sdf
if "%SDFENABLE%"=="no" goto study_rebuild_wfs
SET PROVCALLCMD=%PROVCALLCMD% -w=sdf

:study_rebuild_wfs
if "%WFSENABLE%"=="no" goto study_rebuild_gdal
SET PROVCALLCMD=%PROVCALLCMD% -w=wfs

:study_rebuild_gdal
if "%GDALENABLE%"=="no" goto study_rebuild_wms
SET PROVCALLCMD=%PROVCALLCMD% -w=gdal

:study_rebuild_wms
if "%WMSENABLE%"=="no" goto study_rebuild_postgis
SET PROVCALLCMD=%PROVCALLCMD% -w=wms

:study_rebuild_postgis
if "%POSTGISENABLE%"=="no" goto study_rebuild_fdo
SET PROVCALLCMD=%PROVCALLCMD% -w=postgis

:study_rebuild_fdo
SET PROVCALLCMDEX=%PROVCALLCMDEX%%PROVCALLCMD%
if "%FDOENABLE%"=="no" goto rebuild_thp
SET PROVCALLCMD=%PROVCALLCMD% -w=fdo

:rebuild_thp
pushd %FDOTHIRDPARTY%
call build.bat -o="%FDOORGPATH%" -c=%TYPEBUILD% -a=%TYPEACTION% %PROVCALLCMD%
popd
if "%FDOERROR%"=="1" goto error
rem # End thirdparty part #

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
echo **************************************************************************
echo build_thirdparty.bat [-h] [-o=OutFolder] [-c=BuildType] [-a=Action] [-w=WithModule]
echo *
echo Help:           -h[elp]
echo OutFolder:      -o[utpath]=destination folder for binaries
echo BuildType:      -c[onfig]=release(default), debug
echo Action:         -a[ction]=build(default), buildinstall, install, clean
echo WithModule:     -w[ith]=all(default), fdo, providers, sdf, wfs, wms, postgis
echo **************************************************************************
exit /B 0