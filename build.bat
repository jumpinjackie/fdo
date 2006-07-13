@echo off

SET TYPEACTION=build
SET TYPEBUILD=release
SET FDOORGPATH=%cd%

SET DEFMODIFY=no
SET SHPENABLE=yes
SET SDFENABLE=yes
SET WFSENABLE=yes
SET WMSENABLE=yes
SET ARCENABLE=yes
SET ODBCENABLE=yes
SET MYSQLENABLE=yes
SET FDOENABLE=yes
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

if "%1"=="-a"       goto get_action
if "%1"=="-action"  goto get_action

if "%1"=="-w"       goto get_with
if "%1"=="-with"    goto get_with

if "%1"=="-d"       goto get_docs
if "%1"=="-docs"    goto get_docs

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
if not "%2"=="fdo" goto stp9_get_with
	SET FDOENABLE=yes
	goto next_param
:stp9_get_with
if not "%2"=="providers" goto stp10_get_with
	SET SHPENABLE=yes
	SET SDFENABLE=yes
	SET WFSENABLE=yes
	SET WMSENABLE=yes
	SET ARCENABLE=yes
	SET ODBCENABLE=yes
	SET MYSQLENABLE=yes
	goto next_param
:stp10_get_with
if not "%2"=="all" goto custom_error
	SET SHPENABLE=yes
	SET SDFENABLE=yes
	SET WFSENABLE=yes
	SET WMSENABLE=yes
	SET ARCENABLE=yes
	SET ODBCENABLE=yes
	SET MYSQLENABLE=yes
	SET FDOENABLE=yes
goto next_param

:get_docs
SET DOCENABLE=%2
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
if not exist "%FDOORGPATH%" mkdir "%FDOORGPATH%"

:start_exbuild
SET PROVCALLCMDEX=-o="%FDOORGPATH%" -c=%TYPEBUILD% -a=%TYPEACTION% -d=%DOCENABLE%

:rebuild_fdo
if "%FDOENABLE%"=="no" goto rebuild_shp
pushd Fdo
call build.bat %PROVCALLCMDEX%
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
call build.bat %PROVCALLCMDEX%
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
if "%MYSQLENABLE%"=="no" goto end
if not exist Providers\GenericRdbms\Src\MySQL\build.bat goto study_rebuild
pushd Providers\GenericRdbms\Src\MySQL
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
echo **************************************************************************
echo build.bat [-h] [-o=OutFolder] [-c=BuildType] [-a=Action] [-w=WithModule] [-d=BuildDocs]
echo *
echo Help:           -h[elp]
echo OutFolder:      -o[utpath]=destination folder for binaries
echo BuildType:      -c[onfig]=release(default), debug
echo Action:         -a[ction]=build(default), buildinstall, install, clean
SET MROVBYPROVP=
SET MPROVECAPABP=WithModule:     -w[ith]=all(default), fdo
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
if not exist Providers\GenericRdbms\Src\MySQL\build.bat goto providers_show
	SET MROVBYPROVP=%MROVBYPROVP%, mysql
:providers_show
if ("%MROVBYPROVP%")==("") goto show_capabilities
	SET MPROVECAPABP=%MPROVECAPABP%, providers%MROVBYPROVP%
:show_capabilities
echo %MPROVECAPABP%
echo BuildDocs:      -d[ocs]=skip(default), build
echo **************************************************************************
exit /B 0