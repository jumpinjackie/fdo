@echo off

SET TYPEACTIONSHP=build
SET MSACTIONSHP=Build
SET TYPEBUILDSHP=release
SET FDOORGPATHSHP=%cd%
SET FDOINSPATHSHP=%cd%\Fdo
SET FDOBINPATHSHP=%cd%\Fdo\Bin
SET FDOINCPATHSHP=%cd%\Fdo\Inc
SET FDOLIBPATHSHP=%cd%\Fdo\Lib
SET FDODOCPATHSHP=%cd%\Fdo\Docs
SET DOCENABLESHP=skip
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
SET DOCENABLESHP=%2
if "%2"=="build" goto next_param
if "%2"=="skip" goto next_param
goto custom_error

:get_action
SET TYPEACTIONSHP=%2
if "%2"=="install" goto next_param
if "%2"=="build" goto next_param
if "%2"=="buildinstall" goto next_param
if "%2"=="clean" goto next_param
goto custom_error 

:get_conf 
SET TYPEBUILDSHP=%2
if "%2"=="release" goto next_param
if "%2"=="debug" goto next_param
goto custom_error

:get_path
if (%2)==() goto custom_error
SET FDOORGPATHSHP=%~2
SET FDOINSPATHSHP=%~2\Fdo
SET FDOBINPATHSHP=%~2\Fdo\Bin
SET FDOINCPATHSHP=%~2\Fdo\Inc
SET FDOLIBPATHSHP=%~2\Fdo\Lib
SET FDODOCPATHSHP=%~2\Fdo\Docs

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

if "%TYPEACTIONSHP%"=="build" goto start_exbuild
if "%TYPEACTIONSHP%"=="clean" goto start_exbuild
if not exist "%FDOINSPATHSHP%" mkdir "%FDOINSPATHSHP%"
if not exist "%FDOBINPATHSHP%" mkdir "%FDOBINPATHSHP%"
if not exist "%FDOINCPATHSHP%" mkdir "%FDOINCPATHSHP%"
if not exist "%FDOLIBPATHSHP%" mkdir "%FDOLIBPATHSHP%"
if not exist "%FDODOCPATHSHP%" mkdir "%FDODOCPATHSHP%"

:start_exbuild
if "%TYPEACTIONSHP%"=="clean" SET MSACTIONSHP=Clean
if "%TYPEACTIONSHP%"=="install" goto install_files_shp

echo %MSACTIONSHP% %TYPEBUILDSHP% SHP provider dlls
SET FDOACTIVEBUILD=%cd%\Src\SHP
cscript //Nologo //job:prepare preparebuilds.wsf
pushd Src
msbuild SHP_temp.sln /t:%MSACTIONSHP% /p:Configuration=%TYPEBUILDSHP% /p:Platform="Win32" /nologo /consoleloggerparameters:NoSummary
SET FDOERROR=%errorlevel%
if exist SHP_temp.sln del /Q /F SHP_temp.sln
popd
if "%FDOERROR%"=="1" goto error
if "%TYPEACTIONSHP%"=="clean" goto end
if "%TYPEACTIONSHP%"=="build" goto generate_docs

:install_files_shp
echo copy %TYPEBUILDSHP% SHP provider output files
copy /y "Managed\bin\%TYPEBUILDSHP%\OSGeo.FDO.Providers.SHP.Overrides.dll" "%FDOBINPATHSHP%"
copy /y "Bin\Win32\%TYPEBUILDSHP%\SHPProvider.dll" "%FDOBINPATHSHP%"
copy /y "Bin\Win32\%TYPEBUILDSHP%\SHPOverrides.dll" "%FDOBINPATHSHP%"
copy /y "Bin\Win32\%TYPEBUILDSHP%\SHPMessage.dll" "%FDOBINPATHSHP%"
copy /y "Lib\Win32\%TYPEBUILDSHP%\SHPOverrides.lib" "%FDOLIBPATHSHP%"

echo copy header files
xcopy /S /C /Q /R /Y Inc\SHP\*.h "%FDOINCPATHSHP%\SHP\"

:generate_docs
if "%DOCENABLESHP%"=="skip" goto install_docs
echo Creating SHP provider html and chm documentation
if exist "..\Docs\HTML\Providers\SHP" rmdir /S /Q "..\Docs\HTML\Providers\SHP"
if not exist "..\Docs\HTML\Providers\SHP" mkdir "..\Docs\HTML\Providers\SHP"
if exist ..\Docs\SHP_Provider_API.chm attrib -r ..\Docs\SHP_Provider_API.chm
pushd Docs\doc_src
doxygen Doxyfile_SHP
popd

:install_docs
if "%TYPEACTIONSHP%"=="build" goto end
if exist "%FDODOCPATHSHP%\HTML\Providers\SHP" rmdir /S /Q "%FDODOCPATHSHP%\HTML\Providers\SHP"
if exist ..\Docs\HTML\Providers\SHP xcopy/CQEYI ..\Docs\HTML\Providers\SHP\* "%FDODOCPATHSHP%\HTML\Providers\SHP"
if exist "..\Docs\SHP_Provider_API.chm" copy /y "..\Docs\SHP_Provider_API.chm" "%FDODOCPATHSHP%"

:end
echo End SHP %MSACTIONSHP%
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
echo There was a build error executing action: %MSACTIONSHP%
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
echo Action:         -a[ction]=build(default), buildinstall, install, clean
echo BuildDocs:      -d[ocs]=skip(default), build
echo **************************************************************************
exit /B 0