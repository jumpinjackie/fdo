@echo off

SET TYPEACTIONARCSDE=build
SET MSACTIONARCSDE=Build
SET TYPEBUILDARCSDE=release
SET FDOINSPATHARCSDE=\Fdo
SET FDOBINPATHARCSDE=\Fdo\Bin
SET FDOINCPATHARCSDE=\Fdo\Inc
SET FDOLIBPATHARCSDE=\Fdo\Lib
SET FDODOCPATHARCSDE=\Fdo\Docs
SET DOCENABLEARCSDE=skip
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
SET DOCENABLEARCSDE=%2
if "%2"=="build" goto next_param
if "%2"=="skip" goto next_param
goto custom_error

:get_conf 
SET TYPEBUILDARCSDE=%2
if "%2"=="release" goto next_param
if "%2"=="debug" goto next_param
goto custom_error

:get_action
SET TYPEACTIONARCSDE=%2
if "%2"=="install" goto next_param
if "%2"=="build" goto next_param
if "%2"=="buildinstall" goto next_param
if "%2"=="clean" goto next_param
goto custom_error 

:get_path
if (%2)==() goto custom_error
SET FDOINSPATHARCSDE=%~2\Fdo
SET FDOBINPATHARCSDE=%~2\Fdo\Bin
SET FDOINCPATHARCSDE=%~2\Fdo\Inc
SET FDOLIBPATHARCSDE=%~2\Fdo\Lib
SET FDODOCPATHARCSDE=%~2\Fdo\Docs

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
SET FDOACTENVSTUDY="SDEHOME"
if ("%SDEHOME%")==("") goto env_error
if not exist "%SDEHOME%" goto env_path_error

if "%TYPEACTIONARCSDE%"=="build" goto start_exbuild
if "%TYPEACTIONARCSDE%"=="clean" goto start_exbuild
if not exist "%FDOINSPATHARCSDE%" mkdir "%FDOINSPATHARCSDE%"
if not exist "%FDOBINPATHARCSDE%" mkdir "%FDOBINPATHARCSDE%"
if not exist "%FDOINCPATHARCSDE%" mkdir "%FDOINCPATHARCSDE%"
if not exist "%FDOLIBPATHARCSDE%" mkdir "%FDOLIBPATHARCSDE%"
if not exist "%FDODOCPATHARCSDE%" mkdir "%FDODOCPATHARCSDE%"

:start_exbuild
if "%TYPEACTIONARCSDE%"=="clean" SET MSACTIONARCSDE=Clean
if "%TYPEACTIONARCSDE%"=="install" goto install_files_ArcSDE

echo %MSACTIONARCSDE% %TYPEBUILDARCSDE% ArcSDE provider dlls
SET FDOACTIVEBUILD=%cd%\Src\ArcSDE
cscript //Nologo //job:prepare preparebuilds.wsf
pushd Src
msbuild ArcSDE_temp.sln /t:%MSACTIONARCSDE% /p:Configuration=%TYPEBUILDARCSDE% /p:Platform="Win32" /nologo /consoleloggerparameters:NoSummary
SET FDOERROR=%errorlevel%
if exist ArcSDE_temp.sln del /Q /F ArcSDE_temp.sln
popd
if "%FDOERROR%"=="1" goto error
if "%TYPEACTIONARCSDE%"=="clean" goto end
if "%TYPEACTIONARCSDE%"=="build" goto generate_docs

:install_files_ArcSDE
echo copy %TYPEBUILDARCSDE% ArcSDE provider output files
copy /y "Bin\Win32\%TYPEBUILDARCSDE%\ArcSDEMessage.dll" "%FDOBINPATHARCSDE%"
copy /y "Bin\Win32\%TYPEBUILDARCSDE%\ArcSDEProvider.dll" "%FDOBINPATHARCSDE%"

echo copy header files
rem none

:generate_docs
if "%DOCENABLEARCSDE%"=="skip" goto install_docs
echo Creating ArcSDE provider html and chm documentation
if exist "Docs\HTML\ArcSDE" rmdir /S /Q "Docs\HTML\ArcSDE"
if not exist "Docs\HTML\ArcSDE" mkdir "Docs\HTML\ArcSDE"
if exist Docs\ArcSDE_Provider_API.chm attrib -r Docs\ArcSDE_Provider_API.chm
pushd Docs\doc_src
doxygen Doxyfile_ArcSDE
popd

:install_docs
if "%TYPEACTIONARCSDE%"=="build" goto end
if exist "%FDODOCPATHARCSDE%\HTML\Providers\ArcSDE" rmdir /S /Q "%FDODOCPATHARCSDE%\HTML\Providers\ArcSDE"
if exist Docs\HTML\ArcSDE xcopy/CQEYI Docs\HTML\ArcSDE\* "%FDODOCPATHARCSDE%\HTML\Providers\ArcSDE"
if exist "Docs\ArcSDE_Provider_API.chm" copy /y "Docs\ArcSDE_Provider_API.chm" "%FDODOCPATHARCSDE%"

:end
echo End ArcSDE %MSACTIONARCSDE%
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
echo There was a build error executing action: %MSACTIONARCSDE%
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