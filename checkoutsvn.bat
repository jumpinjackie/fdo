@echo off
if (%FDO_SVN_ROOT%)==() SET FDO_SVN_ROOT=%cd%
time /t
:study_params
if (%1)==() goto start_checkout

if "%1"=="-help"    goto help_show
if "%1"=="-h"       goto help_show

if "%1"=="-o"       goto get_path
if "%1"=="-outpath" goto get_path

if "%1"=="-u"       goto get_user
if "%1"=="-user"    goto get_user

if "%1"=="-p"         goto get_password
if "%1"=="-password"  goto get_password

goto custom_error

:get_password
if (%2)==() goto custom_error
SET FDO_SVN_PASSWORD=%2
goto next_param

:get_user
if (%2)==() goto custom_error
SET FDO_SVN_USERNAME=%2
goto next_param

:get_path
if (%2)==() goto custom_error
if not exist "%~2" mkdir "%~2"
pushd "%~2"
SET FDO_SVN_ROOT=%cd%
popd

:next_param
shift
shift
goto study_params

:start_checkout

if not exist "%FDO_SVN_ROOT%" mkdir "%FDO_SVN_ROOT%"
if not exist "%FDO_SVN_ROOT%\Providers\SDF" mkdir "%FDO_SVN_ROOT%\Providers\SDF"
if not exist "%FDO_SVN_ROOT%\Providers\SHP" mkdir "%FDO_SVN_ROOT%\Providers\SHP"
if not exist "%FDO_SVN_ROOT%\Providers\WMS" mkdir "%FDO_SVN_ROOT%\Providers\WMS"
if not exist "%FDO_SVN_ROOT%\Providers\WFS" mkdir "%FDO_SVN_ROOT%\Providers\WFS"
if not exist "%FDO_SVN_ROOT%\Providers\ArcSDE" mkdir "%FDO_SVN_ROOT%\Providers\ArcSDE"
if not exist "%FDO_SVN_ROOT%\Providers\GenericRdbms" mkdir "%FDO_SVN_ROOT%\Providers\GenericRdbms"

svn checkout https://fdocore.osgeo.org/svn/fdocore/trunk "%FDO_SVN_ROOT%" --username %FDO_SVN_USERNAME% --password %FDO_SVN_PASSWORD%
if errorlevel 1 goto error
svn checkout https://fdosdf.osgeo.org/svn/fdosdf/trunk/Providers/SDF "%FDO_SVN_ROOT%\Providers\SDF" --username %FDO_SVN_USERNAME% --password %FDO_SVN_PASSWORD%
if errorlevel 1 goto error
svn checkout https://fdoshp.osgeo.org/svn/fdoshp/trunk/Providers/SHP "%FDO_SVN_ROOT%\Providers\SHP" --username %FDO_SVN_USERNAME% --password %FDO_SVN_PASSWORD%
if errorlevel 1 goto error
svn checkout https://fdowms.osgeo.org/svn/fdowms/trunk/Providers/WMS "%FDO_SVN_ROOT%\Providers\WMS" --username %FDO_SVN_USERNAME% --password %FDO_SVN_PASSWORD%
if errorlevel 1 goto error
svn checkout https://fdowfs.osgeo.org/svn/fdowfs/trunk/Providers/WFS "%FDO_SVN_ROOT%\Providers\WFS" --username %FDO_SVN_USERNAME% --password %FDO_SVN_PASSWORD%
if errorlevel 1 goto error
svn checkout https://fdoarcsde.osgeo.org/svn/fdoarcsde/trunk/Providers/ArcSDE "%FDO_SVN_ROOT%\Providers\ArcSDE" --username %FDO_SVN_USERNAME% --password %FDO_SVN_PASSWORD%
if errorlevel 1 goto error
svn checkout https://fdordbms.osgeo.org/svn/fdordbms/trunk/Providers/GenericRdbms "%FDO_SVN_ROOT%\Providers\GenericRdbms" --username %FDO_SVN_USERNAME% --password %FDO_SVN_PASSWORD%
if errorlevel 1 goto error

time /t

:end
echo End checkout
time /t
exit /B 0

:error
echo Failed to checkout files.
time /t
exit /B 1

:custom_error
echo The command is not recognized.
echo Please use the format:
:help_show
echo **************************************************************************
echo checkoutsvn.bat [-h] [-o=OutFolder] [-u=UserId] [-p=UserPassword]
echo *
echo Help:           -h[elp]
echo OutFolder:      -o[utpath]=destination folder for files
echo User:           -u[ser]=user id
echo Password:       -p[assword]=user password
echo **************************************************************************
exit /B 0