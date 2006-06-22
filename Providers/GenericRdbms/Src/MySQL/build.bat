@echo off

SET TYPEACTIONMYSQL=build
SET MSACTIONMYSQL=Build
SET TYPEBUILDMYSQL=release
SET FDOINSPATHMYSQL=\Fdo
SET FDOBINPATHMYSQL=\Fdo\Bin
SET FDOINCPATHMYSQL=\Fdo\Inc
SET FDOLIBPATHMYSQL=\Fdo\Lib
SET FDODOCPATHMYSQL=\Fdo\Docs
SET DOCENABLEMYSQL=skip
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
SET DOCENABLEMYSQL=%2
if "%2"=="build" goto next_param
if "%2"=="skip" goto next_param
goto custom_error

:get_action
SET TYPEACTIONMYSQL=%2
if "%2"=="install" goto next_param
if "%2"=="build" goto next_param
if "%2"=="buildinstall" goto next_param
if "%2"=="clean" goto next_param
goto custom_error

:get_conf
SET TYPEBUILDMYSQL=%2
if "%2"=="release" goto next_param
if "%2"=="debug" goto next_param
goto custom_error

:get_path
if (%2)==() goto custom_error
SET FDOINSPATHMYSQL=%~2\Fdo
SET FDOBINPATHMYSQL=%~2\Fdo\Bin
SET FDOINCPATHMYSQL=%~2\Fdo\Inc
SET FDOLIBPATHMYSQL=%~2\Fdo\Lib
SET FDODOCPATHMYSQL=%~2\Fdo\Docs

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
SET FDOACTENVSTUDY="FDOMYSQL"
if ("%FDOMYSQL%")==("") goto env_error
if not exist "%FDOMYSQL%" goto env_path_error

if "%TYPEACTIONMYSQL%"=="build" goto start_exbuild
if "%TYPEACTIONMYSQL%"=="clean" goto start_exbuild
if not exist "%FDOINSPATHMYSQL%" mkdir "%FDOINSPATHMYSQL%"
if not exist "%FDOBINPATHMYSQL%" mkdir "%FDOBINPATHMYSQL%"
if not exist "%FDOINCPATHMYSQL%" mkdir "%FDOINCPATHMYSQL%"
if not exist "%FDOLIBPATHMYSQL%" mkdir "%FDOLIBPATHMYSQL%"
if not exist "%FDODOCPATHMYSQL%" mkdir "%FDODOCPATHMYSQL%"
if not exist "%FDOBINPATHMYSQL%\com" mkdir "%FDOBINPATHMYSQL%\com"

:start_exbuild
time /t
if "%TYPEACTIONMYSQL%"=="clean" SET MSACTIONMYSQL=Clean
if "%TYPEACTIONMYSQL%"=="install" goto install_files_MySQL

echo %MSACTIONMYSQL% %TYPEBUILDMYSQL% MySQL provider dlls
SET FDOACTIVEBUILD=%cd%\MySQL
cscript //job:prepare ../../../../preparebuilds.wsf
msbuild MySQL_temp.sln /t:%MSACTIONMYSQL% /p:Configuration=%TYPEBUILDMYSQL% /p:Platform="Win32" /nologo /consoleloggerparameters:NoSummary
SET FDOERROR=%errorlevel%
if exist MySQL_temp.sln del /Q /F MySQL_temp.sln
if "%FDOERROR%"=="1" goto error
if "%TYPEACTIONMYSQL%"=="clean" goto end
if "%TYPEACTIONMYSQL%"=="build" goto generate_docs

:install_files_MySQL
echo copy %TYPEBUILDMYSQL% MySQL provider output files
copy /y "..\..\Bin\Win32\%TYPEBUILDMYSQL%\RdbmsMsg.dll" "%FDOBINPATHMYSQL%"
copy /y "..\..\Bin\Win32\%TYPEBUILDMYSQL%\MySQLProvider.dll" "%FDOBINPATHMYSQL%"
copy /y "..\..\Bin\Win32\%TYPEBUILDMYSQL%\MySQLOverrides.dll" "%FDOBINPATHMYSQL%"
copy /y "..\..\Bin\Win32\%TYPEBUILDMYSQL%\RdbmsOverrides.dll" "%FDOBINPATHMYSQL%"
copy /y "%FDOUTILITIES%\SchemaMgr\Bin\Win32\%TYPEBUILDMYSQL%\SmMessage.dll" "%FDOBINPATHMYSQL%"
copy /y "..\..\Lib\Win32\%TYPEBUILDMYSQL%\RdbmsOverrides.lib" "%FDOLIBPATHMYSQL%"
copy /y "..\..\Lib\Win32\%TYPEBUILDMYSQL%\MySQLOverrides.lib" "%FDOLIBPATHMYSQL%"
copy /y "..\..\com\fdosys_sys.sql" "%FDOBINPATHMYSQL%\com"
copy /y "..\..\com\fdo_sys_idx.sql" "%FDOBINPATHMYSQL%\com"
copy /y "..\..\com\fdo_sys.sql" "%FDOBINPATHMYSQL%\com"
copy /y "..\..\Managed\bin\%TYPEBUILDMYSQL%\OSGeo.FDO.Providers.MySQL.Overrides.dll" "%FDOBINPATHMYSQL%"
copy /y "..\..\Managed\bin\%TYPEBUILDMYSQL%\OSGeo.FDO.Providers.Rdbms.dll" "%FDOBINPATHMYSQL%"
copy /y "..\..\Managed\bin\%TYPEBUILDMYSQL%\OSGeo.FDO.Providers.Rdbms.Overrides.dll" "%FDOBINPATHMYSQL%"

echo copy header files
xcopy /S /C /Q /R /Y "..\..\inc\Rdbms\*.h" "%FDOINCPATHMYSQL%\Rdbms\"
copy /y "..\..\Inc\Rdbms.h" "%FDOINCPATHMYSQL%\Rdbms\"
if exist "%FDOINCPATHODBC%\Rdbms\Override\Oracle" rmdir /S /Q "%FDOINCPATHODBC%\Rdbms\Override\Oracle"
if exist "%FDOINCPATHODBC%\Rdbms\Override\SqlServer" rmdir /S /Q "%FDOINCPATHODBC%\Rdbms\Override\SqlServer"
if exist "%FDOINCPATHODBC%\Rdbms\FdoSqlServer.h" del /Q /F "%FDOINCPATHODBC%\Rdbms\FdoSqlServer.h"
if exist "%FDOINCPATHODBC%\Rdbms\FdoOracle.h" del /Q /F "%FDOINCPATHODBC%\Rdbms\FdoOracle.h"

:generate_docs
if "%DOCENABLEMYSQL%"=="skip" goto install_docs
pushd ..\..\
echo Creating MySQL provider html and chm documentation
if exist "..\Docs\HTML\Providers\MYSQL" rmdir /S /Q "..\Docs\HTML\Providers\MYSQL"
if not exist "..\Docs\HTML\Providers\MYSQL" mkdir "..\Docs\HTML\Providers\MYSQL"
if exist ..\Docs\MYSQL_Provider_API.chm attrib -r ..\Docs\MYSQL_Provider_API.chm
pushd Docs\doc_src
doxygen Doxyfile_MYSQL
popd
popd

:install_docs
if "%TYPEACTIONMYSQL%"=="build" goto end
pushd ..\..\
if exist "%FDODOCPATHMYSQL%\HTML\Providers\MYSQL" rmdir /S /Q "%FDODOCPATHMYSQL%\HTML\Providers\MYSQL"
if exist ..\Docs\HTML\Providers\MYSQL xcopy/CQEYI ..\Docs\HTML\Providers\MYSQL\* "%FDODOCPATHMYSQL%\HTML\Providers\MYSQL"
if exist "..\Docs\MYSQL_Provider_API.chm" copy /y "..\Docs\MYSQL_Provider_API.chm" "%FDODOCPATHMYSQL%"
popd

:end
time /t
echo End MySQL %MSACTIONMYSQL%
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
echo There was a build error executing action: %MSACTIONMYSQL%
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
echo Action:         -a[ction]=build(default), buildinstall, install, clean
echo BuildDocs:      -d[ocs]=skip(default), build
echo **************************************************************************
exit /B 0