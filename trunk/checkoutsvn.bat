@echo off

rem 
rem Copyright (C) 2004-2006  Autodesk, Inc.
rem 
rem This library is free software; you can redistribute it and/or
rem modify it under the terms of version 2.1 of the GNU Lesser
rem General Public License as published by the Free Software Foundation.
rem 
rem This library is distributed in the hope that it will be useful,
rem but WITHOUT ANY WARRANTY; without even the implied warranty of
rem MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
rem Lesser General Public License for more details.
rem 
rem You should have received a copy of the GNU Lesser General Public
rem License along with this library; if not, write to the Free Software
rem Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
rem 

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
SET GDALENABLECHK=yes

if (%FDO_SVN_DESTDIR%)==() SET FDO_SVN_DESTDIR=%cd%

:study_params
if (%1)==() goto start_checkout

if "%1"=="-help"    goto help_show
if "%1"=="-h"       goto help_show

if "%1"=="-s"       goto get_source
if "%1"=="-source"  goto get_source

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
	SET GDALENABLECHK=no
:stp0_get_with
if not "%2"=="providers" goto stp1_get_with
	SET SHPENABLECHK=yes
	SET SDFENABLECHK=yes
	SET WFSENABLECHK=yes
	SET WMSENABLECHK=yes
	SET ARCENABLECHK=yes
	SET RDBMSENABLECHK=yes
	SET GDALENABLECHK=yes
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
	SET GDALENABLECHK=yes
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
if not "%2"=="rdbms" goto stp12_get_with
	SET RDBMSENABLECHK=yes
    goto next_param
:stp12_get_with
if not "%2"=="gdal" goto custom_error
	SET GDALENABLECHK=yes
    goto next_param

:get_source
if (%2)==() goto custom_error
SET FDO_SVN_SOURCEDIR=%2
goto next_param

:get_path
if (%2)==() goto custom_error
if not exist "%~2" mkdir "%~2"
pushd "%~2"
if errorlevel 1 goto error
SET FDO_SVN_DESTDIR=%cd%
popd

:next_param
shift
shift
goto study_params

:start_checkout

if "%FDO_SVN_PASSWORD%"=="" goto custom_error
if "%FDO_SVN_USERNAME%"=="" goto custom_error
if "%FDO_SVN_SOURCEDIR%"=="" goto custom_error

if not exist "%FDO_SVN_DESTDIR%" mkdir "%FDO_SVN_DESTDIR%"

if "%FDOCOREENABLECHK%"=="yes" goto checkout_all

:checkout_fdo
if "%FDOENABLECHK%"=="no" goto checkout_thirdparty
svn checkout https://fdocore.osgeo.org/svn/fdocore/%FDO_SVN_SOURCEDIR%/Fdo "%FDO_SVN_DESTDIR%\Fdo" --username %FDO_SVN_USERNAME% --password %FDO_SVN_PASSWORD%
if errorlevel 1 goto error

:checkout_thirdparty
if "%THRPENABLECHK%"=="no" goto checkout_utilities
svn checkout https://fdocore.osgeo.org/svn/fdocore/%FDO_SVN_SOURCEDIR%/Thirdparty "%FDO_SVN_DESTDIR%\Thirdparty" --username %FDO_SVN_USERNAME% --password %FDO_SVN_PASSWORD%
if errorlevel 1 goto error

:checkout_utilities
if "%UTILENABLECHK%"=="no" goto checkout_shp
svn checkout https://fdocore.osgeo.org/svn/fdocore/%FDO_SVN_SOURCEDIR%/Utilities "%FDO_SVN_DESTDIR%\Utilities" --username %FDO_SVN_USERNAME% --password %FDO_SVN_PASSWORD%
if errorlevel 1 goto error
goto checkout_shp

:checkout_all
svn checkout https://fdocore.osgeo.org/svn/fdocore/%FDO_SVN_SOURCEDIR%/Fdo "%FDO_SVN_DESTDIR%\Fdo" --username %FDO_SVN_USERNAME% --password %FDO_SVN_PASSWORD%
if errorlevel 1 goto error
svn checkout https://fdocore.osgeo.org/svn/fdocore/%FDO_SVN_SOURCEDIR%/www "%FDO_SVN_DESTDIR%\www" --username %FDO_SVN_USERNAME% --password %FDO_SVN_PASSWORD%
if errorlevel 1 goto error
svn checkout https://fdocore.osgeo.org/svn/fdocore/%FDO_SVN_SOURCEDIR%/Thirdparty "%FDO_SVN_DESTDIR%\Thirdparty" --username %FDO_SVN_USERNAME% --password %FDO_SVN_PASSWORD%
if errorlevel 1 goto error
svn checkout https://fdocore.osgeo.org/svn/fdocore/%FDO_SVN_SOURCEDIR%/Utilities "%FDO_SVN_DESTDIR%\Utilities" --username %FDO_SVN_USERNAME% --password %FDO_SVN_PASSWORD%
if errorlevel 1 goto error
svn checkout https://fdocore.osgeo.org/svn/fdocore/%FDO_SVN_SOURCEDIR% -N "%FDO_SVN_DESTDIR%" --username %FDO_SVN_USERNAME% --password %FDO_SVN_PASSWORD%
if errorlevel 1 goto error

:checkout_shp
if "%SHPENABLECHK%"=="no" goto checkout_sdf
svn checkout https://fdoshp.osgeo.org/svn/fdoshp/%FDO_SVN_SOURCEDIR%/Providers/SHP "%FDO_SVN_DESTDIR%\Providers\SHP" --username %FDO_SVN_USERNAME% --password %FDO_SVN_PASSWORD%
if errorlevel 1 goto error

:checkout_sdf
if "%SDFENABLECHK%"=="no" goto checkout_wfs
svn checkout https://fdosdf.osgeo.org/svn/fdosdf/%FDO_SVN_SOURCEDIR%/Providers/SDF "%FDO_SVN_DESTDIR%\Providers\SDF" --username %FDO_SVN_USERNAME% --password %FDO_SVN_PASSWORD%
if errorlevel 1 goto error

:checkout_wfs
if "%WFSENABLECHK%"=="no" goto checkout_wms
svn checkout https://fdowfs.osgeo.org/svn/fdowfs/%FDO_SVN_SOURCEDIR%/Providers/WFS "%FDO_SVN_DESTDIR%\Providers\WFS" --username %FDO_SVN_USERNAME% --password %FDO_SVN_PASSWORD%
if errorlevel 1 goto error

:checkout_wms
if "%WMSENABLECHK%"=="no" goto checkout_arcsde
svn checkout https://fdowms.osgeo.org/svn/fdowms/%FDO_SVN_SOURCEDIR%/Providers/WMS "%FDO_SVN_DESTDIR%\Providers\WMS" --username %FDO_SVN_USERNAME% --password %FDO_SVN_PASSWORD%
if errorlevel 1 goto error

:checkout_arcsde
if "%ARCENABLECHK%"=="no" goto checkout_generic
svn checkout https://fdoarcsde.osgeo.org/svn/fdoarcsde/%FDO_SVN_SOURCEDIR%/Providers/ArcSDE "%FDO_SVN_DESTDIR%\Providers\ArcSDE" --username %FDO_SVN_USERNAME% --password %FDO_SVN_PASSWORD%
if errorlevel 1 goto error

:checkout_generic
if "%RDBMSENABLECHK%"=="no" goto checkout_gdal
svn checkout https://fdordbms.osgeo.org/svn/fdordbms/%FDO_SVN_SOURCEDIR%/Providers/GenericRdbms "%FDO_SVN_DESTDIR%\Providers\GenericRdbms" --username %FDO_SVN_USERNAME% --password %FDO_SVN_PASSWORD%
if errorlevel 1 goto error

:checkout_gdal
if "%GDALENABLECHK%"=="no" goto end
svn checkout https://fdogdal.osgeo.org/svn/fdogdal/%FDO_SVN_SOURCEDIR%/Providers/GDAL "%FDO_SVN_DESTDIR%\Providers\GDAL" --username %FDO_SVN_USERNAME% --password %FDO_SVN_PASSWORD%
if errorlevel 1 goto error

:end
exit /B 0

:error
echo Failed to checkout files.
exit /B 1

:custom_error
echo The command is not recognized.
echo Please use the format:
:help_show
echo **************************************************************
echo checkoutsvn.bat [-h] 
echo                 [-o=OutFolder] 
echo                 [-w=WithModule] 
echo                 -s=Source
echo                 -u=UserId
echo                 -p=UserPassword
echo *
echo Help:           -h[elp]
echo Branch:         -s[ource]=location of source file, either
echo                           branch location, tag location or trunk
echo OutFolder:      -o[utpath]=destination folder for files
echo WithModule:     -w[ith]=all(default), 
echo                         fdo, 
echo                         fdocore, 
echo                         thirdparty, 
echo                         providers, 
echo                         utilities, 
echo                         shp, 
echo                         sdf, 
echo                         wfs, 
echo                         wms, 
echo                         arcsde, 
echo                         rdbms, 
echo                         gdal
echo User:           -u[ser]=user id
echo Password:       -p[assword]=user password
echo **************************************************************
exit /B 0