@echo off

SET TYPEACTIONWMS=buildinstall
SET TYPEBUILDWMS=release
SET TYPEBUILDWMSPATH=rel
SET FDOORGPATHWMS=%cd%
SET FDOINSPATHWMS=%cd%\Fdo
SET FDOBINPATHWMS=%cd%\Fdo\Bin
SET FDOINCPATHWMS=%cd%\Fdo\Inc
SET FDOLIBPATHWMS=%cd%\Fdo\Lib
SET FDODOCPATHWMS=%cd%\Fdo\Docs
SET DOCENABLEWMS=skip
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
SET DOCENABLEWMS=%2
if "%2"=="build" goto next_param
if "%2"=="skip" goto next_param
goto custom_error

:get_action
SET TYPEACTIONWMS=%2
if "%2"=="installonly" goto next_param
if "%2"=="buildonly" goto next_param
if "%2"=="buildinstall" goto next_param
goto custom_error

:get_conf
SET TYPEBUILDWMS=%2
if "%2"=="release" goto next_param
if "%2"=="debug" SET TYPEBUILDWMSPATH=dbg
if "%2"=="debug" goto next_param
goto custom_error

:get_path
if (%2)==() goto custom_error
SET FDOORGPATHWMS=%~2
SET FDOINSPATHWMS=%~2\Fdo
SET FDOBINPATHWMS=%~2\Fdo\Bin
SET FDOINCPATHWMS=%~2\Fdo\Inc
SET FDOLIBPATHWMS=%~2\Fdo\Lib
SET FDODOCPATHWMS=%~2\Fdo\Docs

:next_param
shift
shift
goto study_params

:start_build
SET FDOACTENVSTUDY="FDO"
if (%FDO%)==() goto env_error
if not exist "%FDO%" goto env_path_error
SET FDOACTENVSTUDY="FDOTHIRDPARTY"
if (%FDOTHIRDPARTY%)==() goto env_error
if not exist "%FDOTHIRDPARTY%" goto env_path_error
SET FDOACTENVSTUDY="FDOUTILITIES"
if (%FDOUTILITIES%)==() goto env_error
if not exist "%FDOUTILITIES%" goto env_path_error

if "%TYPEACTIONWMS%"=="buildonly" goto start_exbuild
if not exist "%FDOINSPATHWMS%" mkdir "%FDOINSPATHWMS%"
if not exist "%FDOBINPATHWMS%" mkdir "%FDOBINPATHWMS%"
if not exist "%FDOINCPATHWMS%" mkdir "%FDOINCPATHWMS%"
if not exist "%FDOLIBPATHWMS%" mkdir "%FDOLIBPATHWMS%"
if not exist "%FDODOCPATHWMS%" mkdir "%FDODOCPATHWMS%"

:start_exbuild
time /t
if "%TYPEACTIONWMS%"=="installonly" goto install_files_wms

echo building %TYPEBUILDWMS% WMS provider dlls
pushd Src
SET FDOACTIVEBUILD=%cd%\WMSOS
cscript //job:prepare ../../../preparebuilds.wsf
msbuild WMSOS_temp.sln /t:Rebuild /p:Configuration=%TYPEBUILDWMS% /p:Platform="Win32" /nologo
SET FDOERROR=%errorlevel%
if exist WMSOS_temp.sln del /Q /F WMSOS_temp.sln
popd
if "%FDOERROR%"=="1" goto error
if "%TYPEACTIONWMS%"=="buildonly" goto generate_docs

:install_files_wms
echo copy %TYPEBUILDWMS% WMS provider output files
copy /y "%FDOUTILITIES%\OWS\Bin\Win32\%TYPEBUILDWMS%\OWS.dll" "%FDOBINPATHWMS%"
copy /y "Bin\Win32\%TYPEBUILDWMS%\WmsMessage.dll" "%FDOBINPATHWMS%"
copy /y "Bin\Win32\%TYPEBUILDWMS%\WmsOverrides.dll" "%FDOBINPATHWMS%"
copy /y "Bin\Win32\%TYPEBUILDWMS%\WmsProvider.dll" "%FDOBINPATHWMS%"
copy /y "Managed\Bin\%TYPEBUILDWMS%\OSGeo.FDO.Providers.WMS.Overrides.dll" "%FDOBINPATHWMS%"
copy /y "Lib\Win32\%TYPEBUILDWMS%\WMSOverrides.lib" "%FDOLIBPATHWMS%"

echo copy header files
xcopy /S /C /Q /R /Y Inc\WMS\*.h "%FDOINCPATHWMS%\WMS\"

:generate_docs
if "%DOCENABLEWMS%"=="skip" goto end
echo Creating WMS provider html and chm documentation
if exist "%FDODOCPATHWMS%\HTML\Providers\WMS" rmdir /S /Q "%FDODOCPATHWMS%\HTML\Providers\WMS"
if exist "..\Docs\HTML\Providers\WMS" rmdir /S /Q "..\Docs\HTML\Providers\WMS"
if not exist "..\Docs\HTML\Providers\WMS" mkdir "..\Docs\HTML\Providers\WMS"
if exist ..\Docs\WMS_Provider_API.chm attrib -r ..\Docs\WMS_Provider_API.chm
pushd Docs\doc_src
doxygen Doxyfile_WMS
popd
xcopy/CQEYI ..\Docs\HTML\Providers\WMS\* "%FDODOCPATHWMS%\HTML\Providers\WMS"
copy /y "..\Docs\WMS_Provider_API.chm" "%FDODOCPATHWMS%"

:end
time /t
echo End WMS Build
exit /B 0

:env_error
echo Environment variable undefined: %FDOACTENVSTUDY%
SET FDOERROR=1
time /t
exit /B 1

:env_path_error
echo Environment variable invalid path: %FDOACTENVSTUDY%
SET FDOERROR=1
time /t
exit /B 1

:error
echo There was a build error.
time /t
exit /B 1

:custom_error
echo The command is not recognized.
echo Please use the format:
:help_show
echo **************************************************************************
echo build.bat [-h] [-o=OutFolder] [-c=BuildType] [-a=Action] [-d=BuildDocs]
echo *
echo Help:           -h[elp]
echo OutFolder:      -o[utpath]=destination folder for binaries
echo BuildType:      -c[onfig]=release(default), debug
echo Action:         -a[ction]=buildinstall(default), buildonly, installonly
echo BuildDocs:      -d[ocs]=skip(default), build
echo **************************************************************************
exit /B 0