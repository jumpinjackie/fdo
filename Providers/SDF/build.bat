@echo off

SET TYPEACTIONSDF=buildinstall
SET MSACTIONSDF=Rebuild
SET TYPEBUILDSDF=release
SET FDOORGPATHSDF=%cd%
SET FDOINSPATHSDF=%cd%\Fdo
SET FDOBINPATHSDF=%cd%\Fdo\Bin
SET FDOINCPATHSDF=%cd%\Fdo\Inc
SET FDOLIBPATHSDF=%cd%\Fdo\Lib
SET FDODOCPATHSDF=%cd%\Fdo\Docs
SET DOCENABLESDF=skip
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
SET DOCENABLESDF=%2
if "%2"=="build" goto next_param
if "%2"=="skip" goto next_param
goto custom_error

:get_action
SET TYPEACTIONSDF=%2
if "%2"=="installonly" goto next_param
if "%2"=="buildonly" goto next_param
if "%2"=="buildinstall" goto next_param
if "%2"=="clean" goto next_param
goto custom_error

:get_conf
SET TYPEBUILDSDF=%2
if "%2"=="release" goto next_param
if "%2"=="debug" goto next_param
goto custom_error

:get_path
if (%2)==() goto custom_error
SET FDOORGPATHSDF=%~2
SET FDOINSPATHSDF=%~2\Fdo
SET FDOBINPATHSDF=%~2\Fdo\Bin
SET FDOINCPATHSDF=%~2\Fdo\Inc
SET FDOLIBPATHSDF=%~2\Fdo\Lib
SET FDODOCPATHSDF=%~2\Fdo\Docs

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
SET FDOACTENVSTUDY="Sed & Bison"
if exist %FDO%\Err.log del /F /Q %FDO%\Err.log
SET FDOACTIVEPATHCHECK=GnuWin32\bin
cscript //job:envcheck ../../preparebuilds.wsf
if exist %FDO%\Err.log goto env_path_error_ex

if "%TYPEACTIONSDF%"=="buildonly" goto start_exbuild
if "%TYPEACTIONSDF%"=="clean" goto start_exbuild
if not exist "%FDOINSPATHSDF%" mkdir "%FDOINSPATHSDF%"
if not exist "%FDOBINPATHSDF%" mkdir "%FDOBINPATHSDF%"
if not exist "%FDOINCPATHSDF%" mkdir "%FDOINCPATHSDF%"
if not exist "%FDOLIBPATHSDF%" mkdir "%FDOLIBPATHSDF%"
if not exist "%FDODOCPATHSDF%" mkdir "%FDODOCPATHSDF%"

:start_exbuild
time /t
if "%TYPEACTIONSDF%"=="clean" SET MSACTIONSDF=Clean
if "%TYPEACTIONSDF%"=="installonly" goto install_files_sdf

echo %MSACTIONSDF% %TYPEBUILDSDF% SDF provider dlls
pushd Src
SET FDOACTIVEBUILD=%cd%\SDFOS
cscript //job:prepare ../../../preparebuilds.wsf
msbuild SDFOS_temp.sln /t:%MSACTIONSDF% /p:Configuration=%TYPEBUILDSDF% /p:Platform="Win32" /nologo
SET FDOERROR=%errorlevel%
if exist SDFOS_temp.sln del /Q /F SDFOS_temp.sln
popd
if "%FDOERROR%"=="1" goto error
if "%TYPEACTIONSDF%"=="clean" goto end
if "%TYPEACTIONSDF%"=="buildonly" goto generate_docs

:install_files_sdf
echo copy %TYPEBUILDSDF% SDF provider output files
copy /y "Bin\Win32\%TYPEBUILDSDF%\SDFMessage.dll" "%FDOBINPATHSDF%"
copy /y "Bin\Win32\%TYPEBUILDSDF%\SDFProvider.dll" "%FDOBINPATHSDF%"

echo copy header files
xcopy /S /C /Q /R /Y Inc\SDF\*.h "%FDOINCPATHSDF%\SDF\"

:generate_docs
if "%DOCENABLESDF%"=="skip" goto end
echo Creating SDF provider html and chm documentation
if exist "%FDODOCPATHSDF%\HTML\Providers\SDF" rmdir /S /Q "%FDODOCPATHSDF%\HTML\Providers\SDF"
if exist "..\Docs\HTML\Providers\SDF" rmdir /S /Q "..\Docs\HTML\Providers\SDF"
if not exist "..\Docs\HTML\Providers\SDF" mkdir "..\Docs\HTML\Providers\SDF"
if exist ..\Docs\SDF_Provider_API.chm attrib -r ..\Docs\SDF_Provider_API.chm
pushd Docs\doc_src
doxygen Doxyfile_SDF
popd
xcopy/CQEYI ..\Docs\HTML\Providers\SDF\* "%FDODOCPATHSDF%\HTML\Providers\SDF"
copy /y "..\Docs\SDF_Provider_API.chm" "%FDODOCPATHSDF%"

:end
time /t
echo End SDF %MSACTIONSDF%
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

:env_path_error_ex
echo Unable to find path (in $PATH) to the : %FDOACTENVSTUDY%
SET FDOERROR=1
time /t
exit /B 1

:error
echo There was a %MSACTIONSDF% error.
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
echo Action:         -a[ction]=buildinstall(default), buildonly, installonly, clean
echo BuildDocs:      -d[ocs]=skip(default), build
echo **************************************************************************
exit /B 0