@echo off

SET DEFMODIFYCHK=no
SET FDOCOREENABLECHK=yes
SET THRPENABLECHK=no
SET FDOENABLECHK=no
SET UTILENABLECHK=no
SET SHPENABLECHK=yes
SET SDFENABLECHK=yes
SET WFSENABLECHK=yes
SET WMSENABLECHK=yes
SET ARCENABLECHK=yes
SET RDBMSENABLECHK=yes

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

if "%1"=="-w"       goto get_with
if "%1"=="-with"    goto get_with

goto custom_error

:get_password
if (%2)==() goto custom_error
SET FDO_SVN_PASSWORD=%2
goto next_param

:get_user
if (%2)==() goto custom_error
SET FDO_SVN_USERNAME=%2
goto next_param

:get_with
if "%DEFMODIFYCHK%"=="yes" goto stp0_get_with
	SET DEFMODIFYCHK=yes
	SET THRPENABLECHK=no
	SET FDOCOREENABLECHK=no
	SET FDOENABLECHK=no
	SET SHPENABLECHK=no
	SET SDFENABLECHK=no
	SET WFSENABLECHK=no
	SET WMSENABLECHK=no
	SET ARCENABLECHK=no
	SET RDBMSENABLECHK=no
	SET UTILENABLECHK=no
:stp0_get_with
if not "%2"=="providers" goto stp1_get_with
	SET SHPENABLECHK=yes
	SET SDFENABLECHK=yes
	SET WFSENABLECHK=yes
	SET WMSENABLECHK=yes
	SET ARCENABLECHK=yes
	SET RDBMSENABLECHK=yes
	goto next_param
:stp1_get_with
if not "%2"=="all" goto stp2_get_with
	SET THRPENABLECHK=no
	SET FDOENABLECHK=no
	SET UTILENABLECHK=no
	SET FDOCOREENABLECHK=yes
	SET SHPENABLECHK=yes
	SET SDFENABLECHK=yes
	SET WFSENABLECHK=yes
	SET WMSENABLECHK=yes
	SET ARCENABLECHK=yes
	SET RDBMSENABLECHK=yes
	goto next_param
:stp2_get_with
if not "%2"=="fdocore" goto stp3_get_with
	SET FDOCOREENABLECHK=yes
	SET THRPENABLECHK=no
	SET FDOENABLECHK=no
	SET UTILENABLECHK=no
	goto next_param
:stp3_get_with
if not "%2"=="thirdparty" goto stp4_get_with
	SET THRPENABLECHK=yes
	goto next_param
:stp4_get_with
if not "%2"=="utilities" goto stp5_get_with
	SET UTILENABLECHK=yes
	goto next_param
:stp5_get_with
if not "%2"=="fdo" goto stp6_get_with
	SET FDOENABLECHK=yes
	goto next_param
:stp6_get_with
if not "%2"=="shp" goto stp7_get_with
	SET SHPENABLECHK=yes
	goto next_param
:stp7_get_with
if not "%2"=="sdf" goto stp8_get_with
	SET SDFENABLECHK=yes
	goto next_param
:stp8_get_with
if not "%2"=="wfs" goto stp9_get_with
	SET WFSENABLECHK=yes
	goto next_param
:stp9_get_with
if not "%2"=="wms" goto stp10_get_with
	SET WMSENABLECHK=yes	
	goto next_param
:stp10_get_with
if not "%2"=="arcsde" goto stp11_get_with
	SET ARCENABLECHK=yes	
	goto next_param
:stp11_get_with
if not "%2"=="rdbms" goto custom_error
	SET RDBMSENABLECHK=yes
goto next_param

:get_path
if (%2)==() goto custom_error
if not exist "%~2" mkdir "%~2"
pushd "%~2"
if errorlevel 1 goto error
SET FDO_SVN_ROOT=%cd%
popd

:next_param
shift
shift
goto study_params

:start_checkout
if not exist "%FDO_SVN_ROOT%" mkdir "%FDO_SVN_ROOT%"

svn checkout -N https://fdocore.osgeo.org/svn/fdocore/trunk "%FDO_SVN_ROOT%" --username %FDO_SVN_USERNAME% --password %FDO_SVN_PASSWORD% >>nul
svn checkout -N https://fdocore.osgeo.org/svn/fdocore/trunk/Providers "%FDO_SVN_ROOT%\Providers" --username %FDO_SVN_USERNAME% --password %FDO_SVN_PASSWORD% >>nul

if "%FDOCOREENABLECHK%"=="yes" goto checkout_all

:checkout_fdo
if "%FDOENABLECHK%"=="no" goto checkout_thirdparty
svn checkout https://fdocore.osgeo.org/svn/fdocore/trunk/Fdo "%FDO_SVN_ROOT%\Fdo" --username %FDO_SVN_USERNAME% --password %FDO_SVN_PASSWORD% >>nul
if errorlevel 1 goto error

:checkout_thirdparty
if "%THRPENABLECHK%"=="no" goto checkout_utilities
svn checkout https://fdocore.osgeo.org/svn/fdocore/trunk/Thirdparty "%FDO_SVN_ROOT%\Thirdparty" --username %FDO_SVN_USERNAME% --password %FDO_SVN_PASSWORD% >>nul
if errorlevel 1 goto error

:checkout_utilities
if "%UTILENABLECHK%"=="no" goto checkout_shp
svn checkout https://fdocore.osgeo.org/svn/fdocore/trunk/Utilities "%FDO_SVN_ROOT%\Utilities" --username %FDO_SVN_USERNAME% --password %FDO_SVN_PASSWORD% >>nul
if errorlevel 1 goto error
goto checkout_shp

:checkout_all
svn checkout https://fdocore.osgeo.org/svn/fdocore/trunk "%FDO_SVN_ROOT%" --username %FDO_SVN_USERNAME% --password %FDO_SVN_PASSWORD% >>nul
if errorlevel 1 goto error

:checkout_shp
if "%SHPENABLECHK%"=="no" goto checkout_sdf
svn checkout https://fdoshp.osgeo.org/svn/fdoshp/trunk/Providers/SHP "%FDO_SVN_ROOT%\Providers\SHP" --username %FDO_SVN_USERNAME% --password %FDO_SVN_PASSWORD% >>nul
if errorlevel 1 goto error

:checkout_sdf
if "%SDFENABLECHK%"=="no" goto checkout_wfs
svn checkout https://fdosdf.osgeo.org/svn/fdosdf/trunk/Providers/SDF "%FDO_SVN_ROOT%\Providers\SDF" --username %FDO_SVN_USERNAME% --password %FDO_SVN_PASSWORD% >>nul
if errorlevel 1 goto error

:checkout_wfs
if "%WFSENABLECHK%"=="no" goto checkout_wms
svn checkout https://fdowfs.osgeo.org/svn/fdowfs/trunk/Providers/WFS "%FDO_SVN_ROOT%\Providers\WFS" --username %FDO_SVN_USERNAME% --password %FDO_SVN_PASSWORD% >>nul
if errorlevel 1 goto error

:checkout_wms
if "%WMSENABLECHK%"=="no" goto checkout_arcsde
svn checkout https://fdowms.osgeo.org/svn/fdowms/trunk/Providers/WMS "%FDO_SVN_ROOT%\Providers\WMS" --username %FDO_SVN_USERNAME% --password %FDO_SVN_PASSWORD% >>nul
if errorlevel 1 goto error

:checkout_arcsde
if "%ARCENABLECHK%"=="no" goto checkout_generic
svn checkout https://fdoarcsde.osgeo.org/svn/fdoarcsde/trunk/Providers/ArcSDE "%FDO_SVN_ROOT%\Providers\ArcSDE" --username %FDO_SVN_USERNAME% --password %FDO_SVN_PASSWORD% >>nul
if errorlevel 1 goto error

:checkout_generic
if "%RDBMSENABLECHK%"=="no" goto end
svn checkout https://fdordbms.osgeo.org/svn/fdordbms/trunk/Providers/GenericRdbms "%FDO_SVN_ROOT%\Providers\GenericRdbms" --username %FDO_SVN_USERNAME% --password %FDO_SVN_PASSWORD% >>nul
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
echo checkoutsvn.bat [-h] [-o=OutFolder] [-w=WithModule] [-u=UserId] [-p=UserPassword]
echo *
echo Help:           -h[elp]
echo OutFolder:      -o[utpath]=destination folder for files
echo WithModule:     -w[ith]=all(default), fdo, fdocore, thirdparty, providers, utilities, shp, sdf, wfs, wms, arcsde, rdbms
echo User:           -u[ser]=user id
echo Password:       -p[assword]=user password
echo **************************************************************************
exit /B 0