@echo off
SET TYPEACTIONPRO=buildinstall
SET TYPEBUILDPRO=release
SET FDOORGPATHPRO=%cd%

SET DEFMODIFYPRO=no
SET SHPENABLEPRO=yes
SET SDFENABLEPRO=yes
SET WFSENABLEPRO=yes
SET WMSENABLEPRO=yes
SET ARCENABLEPRO=yes
SET MYSQLENABLEPRO=yes
SET ODBCENABLEPRO=yes
SET DOCENABLEPRO=skip
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
if "%DEFMODIFYPRO%"=="yes" goto stp0_get_with
	SET DEFMODIFYPRO=yes
	SET SHPENABLEPRO=no
	SET SDFENABLEPRO=no
	SET WFSENABLEPRO=no
	SET WMSENABLEPRO=no
	SET ARCENABLEPRO=no
	SET MYSQLENABLEPRO=no
	SET ODBCENABLEPRO=no
:stp0_get_with
if not "%2"=="providers" goto stp1_get_with
	SET SHPENABLEPRO=yes
	SET SDFENABLEPRO=yes
	SET WFSENABLEPRO=yes
	SET WMSENABLEPRO=yes
	SET ARCENABLEPRO=yes
	SET MYSQLENABLEPRO=yes
	SET ODBCENABLEPRO=yes
	goto next_param
:stp1_get_with
if not "%2"=="shp" goto stp2_get_with
	SET SHPENABLEPRO=yes
	goto next_param
:stp2_get_with
if not "%2"=="sdf" goto stp3_get_with
	SET SDFENABLEPRO=yes
	goto next_param
:stp3_get_with
if not "%2"=="wfs" goto stp4_get_with
	SET WFSENABLEPRO=yes
	goto next_param
:stp4_get_with
if not "%2"=="wms" goto stp5_get_with
	SET WMSENABLEPRO=yes	
	goto next_param
:stp5_get_with
if not "%2"=="arcsde" goto stp6_get_with
	SET ARCENABLEPRO=yes	
	goto next_param
:stp6_get_with
if not "%2"=="mysql" goto stp7_get_with
	SET MYSQLENABLEPRO=yes	
	goto next_param
:stp7_get_with
if not "%2"=="odbc" goto custom_error
	SET ODBCENABLEPRO=yes	
goto next_param

:get_docs
SET DOCENABLEPRO=%2
if "%2"=="build" goto next_param
if "%2"=="skip" goto next_param
goto custom_error

:get_action
SET TYPEACTIONPRO=%2
if "%2"=="installonly" goto next_param
if "%2"=="buildonly" goto next_param
if "%2"=="buildinstall" goto next_param
if "%2"=="clean" goto next_param
goto custom_error

:get_conf
SET TYPEBUILDPRO=%2
if "%2"=="release" goto next_param
if "%2"=="debug" goto next_param
goto custom_error

:get_path
if (%2)==() goto custom_error
if not exist "%~2" mkdir "%~2"
pushd "%~2"
SET FDOORGPATHPRO=%cd%
popd

:next_param
shift
shift
goto study_params

:start_build
if "%TYPEACTIONPRO%"=="buildonly" goto start_exbuild
if "%TYPEACTIONPRO%"=="clean" goto start_exbuild
if not exist "%FDOORGPATHPRO%" mkdir "%FDOORGPATHPRO%"

:start_exbuild
rem # Begin SHP part #
:rebuild_shp
if "%SHPENABLEPRO%"=="no" goto rebuild_sdf
if not exist SHP\build.bat goto rebuild_sdf
pushd SHP
call build.bat -o="%FDOORGPATHPRO%" -c=%TYPEBUILDPRO% -a=%TYPEACTIONPRO% -d=%DOCENABLEPRO%
popd
if "%FDOERROR%"=="1" goto error
rem # End SHP part #

rem # Begin SDF part #
:rebuild_sdf
if "%SDFENABLEPRO%"=="no" goto rebuild_wfs
if not exist SDF\build.bat goto rebuild_wfs
pushd SDF
call build.bat -o="%FDOORGPATHPRO%" -c=%TYPEBUILDPRO% -a=%TYPEACTIONPRO% -d=%DOCENABLEPRO%
popd
if "%FDOERROR%"=="1" goto error
rem # End SDF part #

rem # Begin WFS part #
:rebuild_wfs
if "%WFSENABLEPRO%"=="no" goto rebuild_wms
if not exist WFS\build.bat goto rebuild_wms
pushd WFS
call build.bat -o="%FDOORGPATHPRO%" -c=%TYPEBUILDPRO% -a=%TYPEACTIONPRO% -d=%DOCENABLEPRO%
popd
if "%FDOERROR%"=="1" goto error
rem # End WFS part #

rem # Begin WMS part #
:rebuild_wms
if "%WMSENABLEPRO%"=="no" goto rebuild_arc
if not exist WMS\build.bat goto rebuild_arc
pushd WMS
call build.bat -o="%FDOORGPATHPRO%" -c=%TYPEBUILDPRO% -a=%TYPEACTIONPRO% -d=%DOCENABLEPRO%
popd
if "%FDOERROR%"=="1" goto error
rem # End WMS part #

rem # Begin ArcSDE part #
:rebuild_arc
if "%ARCENABLEPRO%"=="no" goto rebuild_mysql
if not exist ArcSDE\build.bat goto rebuild_mysql
pushd ArcSDE
call build.bat -o="%FDOORGPATHPRO%" -c=%TYPEBUILDPRO% -a=%TYPEACTIONPRO% -d=%DOCENABLEPRO%
popd
if "%FDOERROR%"=="1" goto error
rem # End ArcSDE part #

rem # Begin MySQL part #
:rebuild_mysql
if "%MYSQLENABLEPRO%"=="no" goto rebuild_odbc
if not exist GenericRdbms\src\MySql\build.bat goto rebuild_odbc
pushd GenericRdbms\src\MySql
call build.bat -o="%FDOORGPATHPRO%" -c=%TYPEBUILDPRO% -a=%TYPEACTIONPRO% -d=%DOCENABLEPRO%
popd
if "%FDOERROR%"=="1" goto error
rem # End MySQL part #

rem # Begin ODBC part #
:rebuild_odbc
if "%ODBCENABLEPRO%"=="no" goto end
if not exist GenericRdbms\src\Odbc\build.bat goto end
pushd GenericRdbms\src\Odbc
call build.bat -o="%FDOORGPATHPRO%" -c=%TYPEBUILDPRO% -a=%TYPEACTIONPRO% -d=%DOCENABLEPRO%
popd
if "%FDOERROR%"=="1" goto error
rem # End ODBC part #

:end
echo End Providers action
time /t
exit /B 0

:error
echo There was an error.
time /t
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
echo Action:         -a[ction]=buildinstall(default), buildonly, installonly, clean
SET PROVBYPROVP=
SET PROVECAPABP=WithModule:     -w[ith]=providers(default)
:shp_check
if not exist SHP\build.bat goto sdf_check
	SET PROVBYPROVP=%PROVBYPROVP%, shp
:sdf_check
if not exist SDF\build.bat goto wfs_check
	SET PROVBYPROVP=%PROVBYPROVP%, sdf
:wfs_check
if not exist WFS\build.bat goto wms_check
	SET PROVBYPROVP=%PROVBYPROVP%, wfs
:wms_check
if not exist WMS\build.bat goto arc_check
	SET PROVBYPROVP=%PROVBYPROVP%, wms
:arc_check
if not exist ArcSDE\build.bat goto mysql_check
	SET PROVBYPROVP=%PROVBYPROVP%, arcsde
:mysql_check
if not exist ArcSDE\build.bat goto odbc_check
	SET PROVBYPROVP=%PROVBYPROVP%, mysql
:odbc_check
if not exist ArcSDE\build.bat goto providers_show
	SET PROVBYPROVP=%PROVBYPROVP%, odbc
:providers_show
if ("%PROVBYPROVP%")==("") goto show_capabilities
	SET PROVECAPABP=%PROVECAPABP%%PROVBYPROVP%
:show_capabilities
echo %PROVECAPABP%
echo BuildDocs:      -d[ocs]=skip(default), build
echo **************************************************************************
exit /B 0