@echo off

SET TYPEACTIONODBC=buildinstall
SET MSACTIONODBC=Rebuild
SET TYPEBUILDODBC=release
SET FDOINSPATHODBC=\Fdo
SET FDOBINPATHODBC=\Fdo\Bin
SET FDOINCPATHODBC=\Fdo\Inc
SET FDOLIBPATHODBC=\Fdo\Lib
SET FDODOCPATHODBC=\Fdo\Docs
SET DOCENABLEODBC=skip
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
SET DOCENABLEODBC=%2
if "%2"=="build" goto next_param
if "%2"=="skip" goto next_param
goto custom_error

:get_action
SET TYPEACTIONODBC=%2
if "%2"=="installonly" goto next_param
if "%2"=="buildonly" goto next_param
if "%2"=="buildinstall" goto next_param
if "%2"=="clean" goto next_param
goto custom_error

:get_conf
SET TYPEBUILDODBC=%2
if "%2"=="release" goto next_param
if "%2"=="debug" goto next_param
goto custom_error

:get_path
if (%2)==() goto custom_error
SET FDOINSPATHODBC=%~2\Fdo
SET FDOBINPATHODBC=%~2\Fdo\Bin
SET FDOINCPATHODBC=%~2\Fdo\Inc
SET FDOLIBPATHODBC=%~2\Fdo\Lib
SET FDODOCPATHODBC=%~2\Fdo\Docs

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

if "%TYPEACTIONODBC%"=="buildonly" goto start_exbuild
if "%TYPEACTIONODBC%"=="clean" goto start_exbuild
if not exist "%FDOINSPATHODBC%" mkdir "%FDOINSPATHODBC%"
if not exist "%FDOBINPATHODBC%" mkdir "%FDOBINPATHODBC%"
if not exist "%FDOINCPATHODBC%" mkdir "%FDOINCPATHODBC%"
if not exist "%FDOLIBPATHODBC%" mkdir "%FDOLIBPATHODBC%"
if not exist "%FDODOCPATHODBC%" mkdir "%FDODOCPATHODBC%"

:start_exbuild
time /t
if "%TYPEACTIONODBC%"=="clean" SET MSACTIONODBC=Clean
if "%TYPEACTIONODBC%"=="installonly" goto install_files_ODBC

echo %MSACTIONODBC% %TYPEBUILDODBC% ODBC provider dlls
SET FDOACTIVEBUILD=%cd%\ODBC
cscript //job:prepare ../../../../preparebuilds.wsf
msbuild ODBC_temp.sln /t:%MSACTIONODBC% /p:Configuration=%TYPEBUILDODBC% /p:Platform="Win32" /nologo
SET FDOERROR=%errorlevel%
if exist ODBC_temp.sln del /Q /F ODBC_temp.sln
if "%FDOERROR%"=="1" goto error
if "%TYPEACTIONODBC%"=="clean" goto end
if "%TYPEACTIONODBC%"=="buildonly" goto generate_docs

:install_files_ODBC
echo copy %TYPEBUILDODBC% ODBC provider output files
copy /y "..\..\Bin\Win32\%TYPEBUILDODBC%\RdbmsMsg.dll" "%FDOBINPATHODBC%"
copy /y "..\..\Bin\Win32\%TYPEBUILDODBC%\ODBCProvider.dll" "%FDOBINPATHODBC%"
copy /y "..\..\Bin\Win32\%TYPEBUILDODBC%\ODBCOverrides.dll" "%FDOBINPATHODBC%"
copy /y "..\..\Bin\Win32\%TYPEBUILDODBC%\RdbmsOverrides.dll" "%FDOBINPATHODBC%"
copy /y "%FDOUTILITIES%\SchemaMgr\Bin\Win32\%TYPEBUILDODBC%\SmMessage.dll" "%FDOBINPATHODBC%"
copy /y "..\..\Lib\Win32\%TYPEBUILDODBC%\RdbmsOverrides.lib" "%FDOLIBPATHODBC%"
copy /y "..\..\Lib\Win32\%TYPEBUILDODBC%\ODBCOverrides.lib" "%FDOLIBPATHODBC%"
copy /y "..\..\Managed\bin\%TYPEBUILDODBC%\OSGeo.FDO.Providers.ODBC.Overrides.dll" "%FDOBINPATHODBC%"
copy /y "..\..\Managed\bin\%TYPEBUILDODBC%\OSGeo.FDO.Providers.Rdbms.dll" "%FDOBINPATHODBC%"
copy /y "..\..\Managed\bin\%TYPEBUILDODBC%\OSGeo.FDO.Providers.Rdbms.Overrides.dll" "%FDOBINPATHODBC%"


echo copy header files
xcopy /S /C /Q /R /Y "..\..\Inc\Rdbms\*.h" "%FDOINCPATHODBC%\Rdbms\"
copy /y "..\..\Inc\Rdbms.h" "%FDOINCPATHODBC%\Rdbms\"
if exist "%FDOINCPATHODBC%\Rdbms\Override\Oracle" rmdir /S /Q "%FDOINCPATHODBC%\Rdbms\Override\Oracle"
if exist "%FDOINCPATHODBC%\Rdbms\Override\SqlServer" rmdir /S /Q "%FDOINCPATHODBC%\Rdbms\Override\SqlServer"
if exist "%FDOINCPATHODBC%\Rdbms\FdoSqlServer.h" del /Q /F "%FDOINCPATHODBC%\Rdbms\FdoSqlServer.h"
if exist "%FDOINCPATHODBC%\Rdbms\FdoOracle.h" del /Q /F "%FDOINCPATHODBC%\Rdbms\FdoOracle.h"

:generate_docs
if "%DOCENABLEODBC%"=="skip" goto end
pushd ..\..\
echo Creating ODBC provider html and chm documentation
if exist "%FDODOCPATHODBC%\HTML\Providers\ODBC" rmdir /S /Q "%FDODOCPATHODBC%\HTML\Providers\ODBC"
if exist "..\Docs\HTML\Providers\ODBC" rmdir /S /Q "..\Docs\HTML\Providers\ODBC"
if not exist "..\Docs\HTML\Providers\ODBC" mkdir "..\Docs\HTML\Providers\ODBC"
if exist ..\Docs\ODBC_Provider_API.chm attrib -r ..\Docs\ODBC_Provider_API.chm
pushd Docs\doc_src
doxygen Doxyfile_ODBC
popd
xcopy/CQEYI ..\Docs\HTML\Providers\ODBC\* "%FDODOCPATHODBC%\HTML\Providers\ODBC"
copy /y "..\Docs\ODBC_Provider_API.chm" "%FDODOCPATHODBC%"
popd

:end
time /t
echo End ODBC %MSACTIONODBC%
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
echo Action:         -a[ction]=buildinstall(default), buildonly, installonly, clean
echo BuildDocs:      -d[ocs]=skip(default), build
echo **************************************************************************
exit /B 0