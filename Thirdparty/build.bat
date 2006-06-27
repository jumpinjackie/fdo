@echo off

SET TYPEACTIONTHR=build
SET MSACTIONTHR=Build
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
SET SDFENABLETHR=yes
SET WFSENABLETHR=yes
SET WMSENABLETHR=yes
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
	SET SDFENABLETHR=no
	SET WFSENABLETHR=no
	SET WMSENABLETHR=no
	SET FDOENABLETHR=no
:stp0_get_with
if not "%2"=="providers" goto stp1_get_with
	SET SDFENABLETHR=yes
	SET WFSENABLETHR=yes
	SET WMSENABLETHR=yes
	goto next_param
:stp1_get_with
if not "%2"=="sdf" goto stp2_get_with
	SET SDFENABLETHR=yes
	goto next_param
:stp2_get_with
if not "%2"=="wfs" goto stp3_get_with
	SET WFSENABLETHR=yes
	goto next_param
:stp3_get_with
if not "%2"=="wms" goto stp4_get_with
	SET WMSENABLETHR=yes	
	goto next_param
:stp4_get_with
if not "%2"=="fdo" goto stp5_get_with
	SET FDOENABLETHR=yes
	goto next_param
:stp5_get_with
if not "%2"=="all" goto custom_error
	SET SDFENABLETHR=yes
	SET WFSENABLETHR=yes
	SET WMSENABLETHR=yes
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
if ("%FDOTHIRDPARTY%")==("") SET FDOTHIRDPARTY=%cd%
if ("%XALANROOT%")==("") SET XALANROOT=%cd%\apache\xml-xalan\c
if ("%XERCESCROOT%")==("") SET XERCESCROOT=%cd%\apache\xml-xerces\c
if ("%NLSDIR%")==("") SET NLSDIR=%cd%\apache\xml-xalan\c\Src\xalanc\NLS

if "%TYPEACTIONTHR%"=="build" goto start_exbuild
if "%TYPEACTIONTHR%"=="clean" goto start_exbuild
if not exist "%FDOINSPATHTHR%" mkdir "%FDOINSPATHTHR%"
if not exist "%FDOBINPATHTHR%" mkdir "%FDOBINPATHTHR%"
if not exist "%FDOINCPATHTHR%" mkdir "%FDOINCPATHTHR%"
if not exist "%FDOLIBPATHTHR%" mkdir "%FDOLIBPATHTHR%"

:start_exbuild
if "%TYPEACTIONTHR%"=="clean" SET MSACTIONTHR=Clean

rem # Build FDO API Thirdparty Files
:rebuild_fdo
if "%FDOENABLETHR%"=="no" goto rebuild_sdf
if "%TYPEACTIONTHR%"=="install" goto install_fdo_files

echo %MSACTIONTHR% %TYPEBUILDTHR% Thirdparty FDO Dlls
msbuild Thirdparty_fdo.sln /t:%MSACTIONTHR% /p:Configuration=%TYPEBUILDTHR% /p:Platform="Win32" /nologo /consoleloggerparameters:NoSummary
SET FDOERROR=%errorlevel%
if "%FDOERROR%"=="1" goto error
if not exist util\UpdateVersion\bin mkdir util\UpdateVersion\bin
copy /y util\UpdateVersion\build\UpdateVersion.exe util\UpdateVersion\bin
if "%TYPEACTIONTHR%"=="build" goto rebuild_sdf
if "%TYPEACTIONTHR%"=="clean" goto rebuild_sdf

rem # Install FDO API Thirdparty Files
:install_fdo_files
echo copy %TYPEBUILDTHR% Thirdparty FDO dlls
copy /y "apache\xml-xalan\c\Build\Win32\vc8\%TYPEBUILDTHR%\Xalan-C_1_7_0%TYPEBUILDTHREX%.dll" "%FDOBINPATHTHR%"
copy /y "apache\xml-xalan\c\Build\Win32\vc8\%TYPEBUILDTHR%\XalanMessages_1_7_0%TYPEBUILDTHREX%.dll" "%FDOBINPATHTHR%"
copy /y "apache\xml-xerces\c\Build\Win32\vc8\%TYPEBUILDTHR%\xerces-c_2_5_0%TYPEBUILDTHREX%.dll" "%FDOBINPATHTHR%"

rem # Build SDF Provider Thirdparty Files
:rebuild_sdf
if "%SDFENABLETHR%"=="no" goto rebuild_wfs
if "%TYPEACTIONTHR%"=="install" goto rebuild_wfs

echo %MSACTIONTHR% %TYPEBUILDTHR% Thirdparty SDF dlls
msbuild Thirdparty_sdf.sln /t:%MSACTIONTHR% /p:Configuration=%TYPEBUILDTHR% /p:Platform="Win32" /nologo /consoleloggerparameters:NoSummary
SET FDOERROR=%errorlevel%
if "%FDOERROR%"=="1" goto error

rem # Build WFS Provider Thirdparty Files
:rebuild_wfs
if "%WFSENABLETHR%"=="no" goto rebuild_wms
if "%TYPEACTIONTHR%"=="install" goto install_wfs_files

echo %MSACTIONTHR% %TYPEBUILDTHR% Thirdparty WFS dlls
msbuild openssl\openssl.sln /t:%MSACTIONTHR% /p:Configuration=%TYPEBUILDTHR% /p:Platform="Win32" /nologo /consoleloggerparameters:NoSummary
SET FDOERROR=%errorlevel%
if "%FDOERROR%"=="1" goto error
msbuild libcurl\lib\curllib.sln /t:%MSACTIONTHR% /p:Configuration=%TYPEBUILDTHR% /p:Platform="Win32" /nologo /consoleloggerparameters:NoSummary
SET FDOERROR=%errorlevel%
if "%FDOERROR%"=="1" goto error
msbuild boost_1_32_0\boost_1_32_0.vcproj /t:%MSACTIONTHR% /p:Configuration=%TYPEBUILDTHR% /p:Platform="Win32" /nologo /consoleloggerparameters:NoSummary
SET FDOERROR=%errorlevel%
if "%FDOERROR%"=="1" goto error
if "%TYPEACTIONTHR%"=="build" goto rebuild_wms
if "%TYPEACTIONTHR%"=="clean" goto rebuild_wms

rem # Install WFS Provider Thirdparty Files
:install_wfs_files
echo copy %TYPEBUILDTHR% Thirdparty WFS dlls
copy /y "boost_1_32_0\bin\boost\libs\thread\build\boost_thread.dll\vc-8_0\%TYPEBUILDTHR%\threading-multi\boost_thread-vc80-mt%TYPEBUILDTHRPATH%-1_32.dll" "%FDOBINPATHTHR%"
rem # End WFS part #

rem # Build WMS Provider Thirdparty Files
:rebuild_wms
if "%WMSENABLETHR%"=="no" goto end
if "%TYPEACTIONTHR%"=="install" goto install_wms_files

echo %MSACTIONTHR% %TYPEBUILDTHR% Thirdparty WMS dlls
msbuild openssl\openssl.sln /t:%MSACTIONTHR% /p:Configuration=%TYPEBUILDTHR% /p:Platform="Win32" /nologo /consoleloggerparameters:NoSummary
SET FDOERROR=%errorlevel%
if "%FDOERROR%"=="1" goto error
msbuild libcurl\lib\curllib.sln /t:%MSACTIONTHR% /p:Configuration=%TYPEBUILDTHR% /p:Platform="Win32" /nologo /consoleloggerparameters:NoSummary
SET FDOERROR=%errorlevel%
if "%FDOERROR%"=="1" goto error
msbuild GDAL1.3\src\Gdal1.3.sln /t:%MSACTIONTHR% /p:Configuration=%TYPEBUILDTHR% /p:Platform="Win32" /nologo /consoleloggerparameters:NoSummary
SET FDOERROR=%errorlevel%
if "%FDOERROR%"=="1" goto error
msbuild boost_1_32_0\boost_1_32_0.vcproj /t:%MSACTIONTHR% /p:Configuration=%TYPEBUILDTHR% /p:Platform="Win32" /nologo /consoleloggerparameters:NoSummary
SET FDOERROR=%errorlevel%
if "%FDOERROR%"=="1" goto error
if "%TYPEACTIONTHR%"=="build" goto end
if "%TYPEACTIONTHR%"=="clean" goto end

rem # Install WMS Provider Thirdparty Files
:install_wms_files
echo copy %TYPEBUILDTHR% Thirdparty WMS dlls
copy /y "GDAL1.3\windows\bin\VC8\%TYPEBUILDTHR%\gdal13.dll" "%FDOBINPATHTHR%"
copy /y "boost_1_32_0\bin\boost\libs\thread\build\boost_thread.dll\vc-8_0\%TYPEBUILDTHR%\threading-multi\boost_thread-vc80-mt%TYPEBUILDTHRPATH%-1_32.dll" "%FDOBINPATHTHR%"
rem # End WMS part #

:end
echo End Thirdparty %MSACTIONTHR%
exit /B 0

:error
echo There was a build error executing action: %MSACTIONTHR%
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
echo Action:         -a[ction]=build(default), buildinstall, install, clean
echo WithModule:     -w[ith]=all(default), fdo, providers, sdf, wfs, wms
echo **************************************************************************
exit /B 0