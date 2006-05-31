@echo off

SET TYPEACTIONTHR=buildinstall
SET MSACTIONTHR=Rebuild
SET TYPEBUILDTHR=release
SET FDOBASPATHTHR=%cd%
SET FDOINSPATHTHR=%cd%\Fdo
SET FDOBINPATHTHR=%cd%\Fdo\Bin
SET FDOINCPATHTHR=%cd%\Fdo\Inc
SET FDOLIBPATHTHR=%cd%\Fdo\Lib
SET TYPEBUILDTHRPATH=
SET TYPEBUILDTHREX=

SET DEFMODIFYTHR=no
SET FDOENABLETHR=yes
SET SHPENABLETHR=yes
SET SDFENABLETHR=yes
SET WFSENABLETHR=yes
SET WMSENABLETHR=yes
SET ARCENABLETHR=yes
SET MYSQLENABLETHR=yes
SET ODBCENABLETHR=yes
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
if "%DEFMODIFYTHR%"=="yes" goto stp0_get_with
	SET DEFMODIFYTHR=yes
	SET SHPENABLETHR=no
	SET SDFENABLETHR=no
	SET WFSENABLETHR=no
	SET WMSENABLETHR=no
	SET ARCENABLETHR=no
	SET MYSQLENABLETHR=no
	SET ODBCENABLETHR=no
	SET FDOENABLETHR=no
:stp0_get_with
if not "%2"=="providers" goto stp1_get_with
	SET SHPENABLETHR=yes
	SET SDFENABLETHR=yes
	SET WFSENABLETHR=yes
	SET WMSENABLETHR=yes
	SET ARCENABLETHR=yes
	SET MYSQLENABLETHR=yes
	SET ODBCENABLETHR=yes
	goto next_param
:stp1_get_with
if not "%2"=="shp" goto stp2_get_with
	SET SHPENABLETHR=yes
	goto next_param
:stp2_get_with
if not "%2"=="sdf" goto stp3_get_with
	SET SDFENABLETHR=yes
	goto next_param
:stp3_get_with
if not "%2"=="wfs" goto stp4_get_with
	SET WFSENABLETHR=yes
	goto next_param
:stp4_get_with
if not "%2"=="wms" goto stp5_get_with
	SET WMSENABLETHR=yes	
	goto next_param
:stp5_get_with
if not "%2"=="arcsde" goto stp6_get_with
	SET ARCENABLETHR=yes	
	goto next_param
:stp6_get_with
if not "%2"=="mysql" goto stp7_get_with
	SET MYSQLENABLETHR=yes	
	goto next_param
:stp7_get_with
if not "%2"=="odbc" goto stp8_get_with
	SET ODBCENABLETHR=yes	
	goto next_param
:stp8_get_with
if not "%2"=="fdo" goto stp9_get_with
	SET FDOENABLETHR=yes
	goto next_param
:stp9_get_with
if not "%2"=="all" goto custom_error
	SET SHPENABLETHR=yes
	SET SDFENABLETHR=yes
	SET WFSENABLETHR=yes
	SET WMSENABLETHR=yes
	SET ARCENABLETHR=yes
	SET MYSQLENABLETHR=yes
	SET ODBCENABLETHR=yes
	SET FDOENABLETHR=yes
goto next_param

:get_action
SET TYPEACTIONTHR=%2
if "%2"=="install" goto next_param
if "%2"=="build" goto next_param
if "%2"=="buildinstall" goto next_param
if "%2"=="clean" goto next_param
goto custom_error

:get_conf
SET TYPEBUILDTHR=%2
if "%2"=="release" goto next_param
SET TYPEBUILDTHRPATH=-gd
SET TYPEBUILDTHREX=D
if "%2"=="debug" goto next_param
goto custom_error

:get_path
if (%2)==() goto custom_error
SET FDOBASPATHTHR=%~2
SET FDOINSPATHTHR=%~2\Fdo
SET FDOBINPATHTHR=%~2\Fdo\Bin
SET FDOINCPATHTHR=%~2\Fdo\Inc
SET FDOLIBPATHTHR=%~2\Fdo\Lib

:next_param
shift
shift
goto study_params

:start_build
SET FDOACTENVSTUDY="FDOTHIRDPARTY"
if ("%FDOTHIRDPARTY%")==("") goto env_error
if not exist "%FDOTHIRDPARTY%" goto env_path_error
SET FDOACTENVSTUDY="XALANROOT"
if ("%XALANROOT%")==("") goto env_error
if not exist "%XALANROOT%" goto env_path_error
SET FDOACTENVSTUDY="XERCESCROOT"
if ("%XERCESCROOT%")==("") goto env_error
if not exist "%XERCESCROOT%" goto env_path_error
SET FDOACTENVSTUDY="NLSDIR"
if ("%NLSDIR%")==("") goto env_error
if not exist "%NLSDIR%" goto env_path_error

if "%TYPEACTIONTHR%"=="build" goto start_exbuild
if "%TYPEACTIONTHR%"=="clean" goto start_exbuild
if not exist "%FDOINSPATHTHR%" mkdir "%FDOINSPATHTHR%"
if not exist "%FDOBINPATHTHR%" mkdir "%FDOBINPATHTHR%"
if not exist "%FDOINCPATHTHR%" mkdir "%FDOINCPATHTHR%"
if not exist "%FDOLIBPATHTHR%" mkdir "%FDOLIBPATHTHR%"

:start_exbuild
time /t

if "%TYPEACTIONTHR%"=="clean" SET MSACTIONTHR=Clean
rem # Clean operation is not supported by thirdparty #
if "%TYPEACTIONTHR%"=="clean" goto end

rem # Begin FDO part #
:rebuild_fdo
if "%FDOENABLETHR%"=="no" goto rebuild_sdf
if "%TYPEACTIONTHR%"=="install" goto install_fdo_files

echo Rebuild %TYPEBUILDTHR% Thirdparty FDO dlls
msbuild Thirdparty_fdo.sln /t:Rebuild /p:Configuration=%TYPEBUILDTHR% /p:Platform="Win32" /nologo
SET FDOERROR=%errorlevel%
if "%FDOERROR%"=="1" goto error


if not exist util\UpdateVersion\bin mkdir util\UpdateVersion\bin
copy /y util\UpdateVersion\build\UpdateVersion.exe util\UpdateVersion\bin

if "%TYPEACTIONTHR%"=="build" goto rebuild_sdf

:install_fdo_files
echo copy %TYPEBUILDTHR% Thirdparty FDO dlls
copy /y "apache\xml-xalan\c\Build\Win32\vc8\%TYPEBUILDTHR%\Xalan-C_1_7_0%TYPEBUILDTHREX%.dll" "%FDOBINPATHTHR%"
copy /y "apache\xml-xalan\c\Build\Win32\vc8\%TYPEBUILDTHR%\XalanMessages_1_7_0%TYPEBUILDTHREX%.dll" "%FDOBINPATHTHR%"
copy /y "apache\xml-xerces\c\Build\Win32\vc8\%TYPEBUILDTHR%\xerces-c_2_5_0%TYPEBUILDTHREX%.dll" "%FDOBINPATHTHR%"
rem # End FDO part #

rem # Begin SDF part #
:rebuild_sdf
if "%SDFENABLETHR%"=="no" goto rebuild_wfs
if "%TYPEACTIONTHR%"=="install" goto install_sdf_files

echo Rebuild %TYPEBUILDTHR% Thirdparty SDF dlls
msbuild Thirdparty_sdf.sln /t:Rebuild /p:Configuration=%TYPEBUILDTHR% /p:Platform="Win32" /nologo
SET FDOERROR=%errorlevel%
if "%FDOERROR%"=="1" goto error


:install_sdf_files
echo copy %TYPEBUILDTHR% Thirdparty SDF dlls
rem No files to copy
rem # End SDF part #

rem # Begin WFS part #
:rebuild_wfs
if "%WFSENABLETHR%"=="no" goto rebuild_wms
if "%TYPEACTIONTHR%"=="install" goto install_wfs_files

echo Rebuild %TYPEBUILDTHR% Thirdparty WFS dlls
msbuild Thirdparty_wfs.sln /t:Rebuild /p:Configuration=%TYPEBUILDTHR% /p:Platform="Win32" /nologo
SET FDOERROR=%errorlevel%
if "%FDOERROR%"=="1" goto error
msbuild boost_1_32_0\boost_1_32_0.vcproj /t:Rebuild /p:Configuration=%TYPEBUILDTHR% /p:Platform="Win32" /nologo
SET FDOERROR=%errorlevel%
if "%FDOERROR%"=="1" goto error

if "%TYPEACTIONTHR%"=="build" goto rebuild_wms

:install_wfs_files
echo copy %TYPEBUILDTHR% Thirdparty WFS dlls
copy /y "boost_1_32_0\bin\boost\libs\thread\build\boost_thread.dll\vc-8_0\%TYPEBUILDTHR%\threading-multi\boost_thread-vc80-mt%TYPEBUILDTHRPATH%-1_32.dll" "%FDOBINPATHTHR%"
rem # End WFS part #

rem # Begin WMS part #
:rebuild_wms
if "%WMSENABLETHR%"=="no" goto rebuild_shp
if "%TYPEACTIONTHR%"=="install" goto install_wms_files

echo Rebuild %TYPEBUILDTHR% Thirdparty WMS dlls
msbuild Thirdparty_wms.sln /t:Rebuild /p:Configuration=%TYPEBUILDTHR% /p:Platform="Win32" /nologo
SET FDOERROR=%errorlevel%
if "%FDOERROR%"=="1" goto error
msbuild boost_1_32_0\boost_1_32_0.vcproj /t:Rebuild /p:Configuration=%TYPEBUILDTHR% /p:Platform="Win32" /nologo
SET FDOERROR=%errorlevel%
if "%FDOERROR%"=="1" goto error

if "%TYPEACTIONTHR%"=="build" goto rebuild_shp

:install_wms_files
echo copy %TYPEBUILDTHR% Thirdparty WMS dlls
copy /y "GDAL1.3\windows\bin\VC8\%TYPEBUILDTHR%\gdal13.dll" "%FDOBINPATHTHR%"
copy /y "boost_1_32_0\bin\boost\libs\thread\build\boost_thread.dll\vc-8_0\%TYPEBUILDTHR%\threading-multi\boost_thread-vc80-mt%TYPEBUILDTHRPATH%-1_32.dll" "%FDOBINPATHTHR%"
rem # End WMS part #

rem # Begin SHP part #
:rebuild_shp
if "%SHPENABLETHR%"=="no" goto rebuild_arc
if "%TYPEACTIONTHR%"=="install" goto rebuild_arc
echo No dependencies to build for SHP.
rem # End SHP part #

rem # Begin ArcSDE part #
:rebuild_arc
if "%ARCENABLETHR%"=="no" goto rebuild_mysql
if "%TYPEACTIONTHR%"=="install" goto rebuild_mysql
echo No dependencies to build for ArcSDE.
rem # End ArcSDE part #

rem # Begin MySQL part #
:rebuild_mysql
if "%MYSQLENABLETHR%"=="no" goto rebuild_odbc
if "%TYPEACTIONTHR%"=="install" goto rebuild_odbc
echo No dependencies to build for MySQL.
rem # End MySQL part #

rem # Begin ODBC part #
:rebuild_odbc
if "%ODBCENABLETHR%"=="no" goto end
if "%TYPEACTIONTHR%"=="install" goto end
echo No dependencies to build for ODBC.
rem # End ODBC part #


:end
echo End Thirdparty %MSACTIONTHR%
time /t
exit /B 0

:env_error
echo Environment variable undefined: %FDOACTENVSTUDY%
SET FDOERROR=1
time /t
exit /B 1

:env_path_error
echo Invalid path contained in FDO environment variable: %FDOACTENVSTUDY%
SET FDOERROR=1
time /t
exit /B 1

:error
echo There was a build error executing action: %MSACTIONFDO%
time /t
exit /B 1

:custom_error
echo The command is not recognized.
echo Please use the format:
:help_show
echo **************************************************************************
echo build.bat [-h] [-o=DestFolder] [-c=BuildType] [-a=Action] [-w=WithModule]
echo *
echo Help:           -h[elp]
echo OutFolder:      -o[utpath]=destination folder for binaries
echo BuildType:      -c[onfig]=release(default), debug
echo Action:         -a[ction]=buildinstall(default), build, install, clean
SET TROVBYPROVP=
SET TPROVECAPABP=WithModule:     -w[ith]=all(default), fdo
:shp_check
if not exist ..\providers\Shp\build.bat goto sdf_check
	SET TROVBYPROVP=%TROVBYPROVP%, shp
:sdf_check
if not exist ..\providers\SDFPlus\build.bat goto wfs_check
	SET TROVBYPROVP=%TROVBYPROVP%, sdf
:wfs_check
if not exist ..\providers\WFS\build.bat goto wms_check
	SET TROVBYPROVP=%TROVBYPROVP%, wfs
:wms_check
if not exist ..\providers\WMS\build.bat goto arc_check
	SET TROVBYPROVP=%TROVBYPROVP%, wms
:arc_check
if not exist ..\providers\ArcSDE\build.bat goto mysql_check
	SET TROVBYPROVP=%TROVBYPROVP%, arcsde
:mysql_check
if not exist ..\providers\GenericRdbms\Src\MySQL\build.bat goto odbc_check
	SET TROVBYPROVP=%TROVBYPROVP%, mysql
:odbc_check
if not exist ..\providers\GenericRdbms\Src\ODBC\build.bat goto providers_show
	SET TROVBYPROVP=%TROVBYPROVP%, odbc
:providers_show
if ("%TROVBYPROVP%")==("") goto show_capabilities
	SET TPROVECAPABP=%TPROVECAPABP%, providers%TROVBYPROVP%
:show_capabilities
echo %TPROVECAPABP%
echo **************************************************************************
exit /B 0