@echo off

SET TYPEACTIONFDO=buildinstall
SET MSACTIONFDO=Rebuild
SET TYPEBUILDFDO=release
SET FDOORGPATHFDO=%cd%
SET FDOINSPATHFDO=%cd%\Fdo
SET FDOBINPATHFDO=%cd%\Fdo\Bin
SET FDOINCPATHFDO=%cd%\Fdo\Inc
SET FDOLIBPATHFDO=%cd%\Fdo\Lib
SET FDODOCPATHFDO=%cd%\Fdo\Docs
SET DOCENABLEFDO=skip
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
SET DOCENABLEFDO=%2
if "%2"=="build" goto next_param
if "%2"=="skip" goto next_param
goto custom_error

:get_action
SET TYPEACTIONFDO=%2
if "%2"=="install" goto next_param
if "%2"=="build" goto next_param
if "%2"=="buildinstall" goto next_param
if "%2"=="clean" goto next_param
goto custom_error

:get_conf
SET TYPEBUILDFDO=%2
if "%2"=="release" goto next_param
if "%2"=="debug" goto next_param
goto custom_error

:get_path
if (%2)==() goto custom_error
SET FDOORGPATHFDO=%~2
SET FDOINSPATHFDO=%~2\Fdo
SET FDOBINPATHFDO=%~2\Fdo\Bin
SET FDOINCPATHFDO=%~2\Fdo\Inc
SET FDOLIBPATHFDO=%~2\Fdo\Lib
SET FDODOCPATHFDO=%~2\Fdo\Docs

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
SET FDOACTENVSTUDY="Sed & Bison"
if exist %FDO%\Err.log del /F /Q %FDO%\Err.log
SET FDOACTIVEPATHCHECK=GnuWin32\bin
cscript //job:envcheck ../preparebuilds.wsf
if exist %FDO%\Err.log goto env_path_error_ex

if "%TYPEACTIONFDO%"=="build" goto start_exbuild
if "%TYPEACTIONFDO%"=="clean" goto start_exbuild
if not exist "%FDOINSPATHFDO%" mkdir "%FDOINSPATHFDO%"
if not exist "%FDOBINPATHFDO%" mkdir "%FDOBINPATHFDO%"
if not exist "%FDOINCPATHFDO%" mkdir "%FDOINCPATHFDO%"
if not exist "%FDOLIBPATHFDO%" mkdir "%FDOLIBPATHFDO%"
if not exist "%FDODOCPATHFDO%" mkdir "%FDODOCPATHFDO%"

:start_exbuild
time /t
if "%TYPEACTIONFDO%"=="clean" SET MSACTIONFDO=Clean
if "%TYPEACTIONFDO%"=="install" goto install_files

echo %MSACTIONFDO% %TYPEBUILDFDO% Fdo dlls
msbuild FDO.sln /t:%MSACTIONFDO% /p:Configuration=%TYPEBUILDFDO% /p:Platform="Win32" /nologo
SET FDOERROR=%errorlevel%
if "%FDOERROR%"=="1" goto error
if "%TYPEACTIONFDO%"=="build" goto generate_docs
if "%TYPEACTIONFDO%"=="clean" goto end

:install_files
echo copy FDO %TYPEBUILDFDO% output files
copy /y "Unmanaged\Bin\Win32\%TYPEBUILDFDO%\FDO.dll" "%FDOBINPATHFDO%"
copy /y "Unmanaged\Bin\Win32\%TYPEBUILDFDO%\FDOMessage.dll" "%FDOBINPATHFDO%"
copy /y "Unmanaged\Bin\Win32\%TYPEBUILDFDO%\FDOCommon.dll" "%FDOBINPATHFDO%"
copy /y "Unmanaged\Bin\Win32\%TYPEBUILDFDO%\FDOGeometry.dll" "%FDOBINPATHFDO%"
copy /y "Unmanaged\Bin\Win32\%TYPEBUILDFDO%\FDOSpatial.dll" "%FDOBINPATHFDO%"
copy /y "Managed\bin\%TYPEBUILDFDO%\OSGeo.FDO.dll" "%FDOBINPATHFDO%"
copy /y "Managed\bin\%TYPEBUILDFDO%\OSGeo.FDO.Common.dll" "%FDOBINPATHFDO%"
copy /y "Managed\bin\%TYPEBUILDFDO%\OSGeo.FDO.Geometry.dll" "%FDOBINPATHFDO%"
copy /y "Managed\bin\%TYPEBUILDFDO%\OSGeo.FDO.Spatial.dll" "%FDOBINPATHFDO%"
copy /y "Unmanaged\Lib\Win32\%TYPEBUILDFDO%\FDO.lib" "%FDOLIBPATHFDO%"
copy /y "Unmanaged\Lib\Win32\%TYPEBUILDFDO%\FDOCommon.lib" "%FDOLIBPATHFDO%"
copy /y "Unmanaged\Lib\Win32\%TYPEBUILDFDO%\FDOGeometry.lib" "%FDOLIBPATHFDO%"
cscript //job:install ../preparebuilds.wsf

echo copy FDO header files
xcopy /S /C /Q /R /Y Unmanaged\Inc\*.h "%FDOINCPATHFDO%\"
del /F /Q "%FDOINCPATHFDO%\Spatial.h"
rmdir /S /Q "%FDOINCPATHFDO%\Spatial"


:generate_docs
if "%DOCENABLEFDO%"=="skip" goto install_docs
echo Creating FDO Unmanaged and Managed html and chm API documentation
if exist "Docs\HTML\FDO_API" rmdir /S /Q "Docs\HTML\FDO_API"
if exist "Docs\HTML\FDO_API_managed" rmdir /S /Q "Docs\HTML\FDO_API_managed"
if not exist "Docs\HTML\FDO_API" mkdir "Docs\HTML\FDO_API"
if not exist "Docs\HTML\FDO_API_managed" mkdir "Docs\HTML\FDO_API_managed"
if exist Docs\FDO_API.chm attrib -r Docs\FDO_API.chm
if exist Docs\FDO_API_managed.chm attrib -r Docs\FDO_API_managed.chm
pushd Docs\doc_src
doxygen Doxyfile_FDOunmanaged
doxygen Doxyfile_FDOmanaged
popd
if "%TYPEACTIONFDO%"=="build" goto end

:install_docs
pushd Docs\doc_src
if exist "%FDODOCPATHFDO%\HTML\FDO_API" rmdir /S /Q "%FDODOCPATHFDO%\HTML\FDO_API"
if exist "%FDODOCPATHFDO%\HTML\FDO_API_managed" rmdir /S /Q "%FDODOCPATHFDO%\HTML\FDO_API_managed"
if not exist "%FDODOCPATHFDO%\HTML\FDO_API" mkdir "%FDODOCPATHFDO%\HTML\FDO_API"
if not exist "%FDODOCPATHFDO%\HTML\FDO_API_managed" mkdir "%FDODOCPATHFDO%\HTML\FDO_API_managed"

if exist "..\HTML\FDO_API\" xcopy/CQEYI "..\HTML\FDO_API\*" "%FDODOCPATHFDO%\HTML\FDO_API"
if exist "..\HTML\FDO_API_managed\" xcopy/CQEYI "..\HTML\FDO_API_managed\*" "%FDODOCPATHFDO%\HTML\FDO_API_managed"
if exist "..\FDO_API_managed.chm" copy /y "..\FDO_API_managed.chm" "%FDODOCPATHFDO%"
if exist "..\FDO_API.chm" copy /y "..\FDO_API.chm" "%FDODOCPATHFDO%"
if exist "..\FDG_FDODevGuide.pdf" copy /y "..\FDG_FDODevGuide.pdf" "%FDODOCPATHFDO%"
if exist "..\FET_TheEssentialFDO.pdf" copy /y "..\FET_TheEssentialFDO.pdf" "%FDODOCPATHFDO%"
popd

:end
echo End FDO %MSACTIONFDO%
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

:env_path_error_ex
echo Unable to find location of %FDOACTENVSTUDY% in the Windows System PATH
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
echo build.bat [-h] [-o=OutFolder] [-c=BuildType] [-a=Action] [-d=BuildDocs]
echo *
echo Help:           -h[elp]
echo OutFolder:      -o[utpath]=destination folder for binaries
echo BuildType:      -c[onfig]=release(default), debug
echo Action:         -a[ction]=buildinstall(default), build, install, clean
echo BuildDocs:      -d[ocs]=skip(default), build
echo **************************************************************************
exit /B 0