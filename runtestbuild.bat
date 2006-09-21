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

SET FDOTARZIPFOLDERRTB=OpenSource_FDO
SET FDOTARGETFOLDER=OpenSource
SET FDOBUILDNUMBERRTB=GXXX
SET FDOSOURCEZIP=%cd%
SET FDOERRORMESSAGE=

:study_params
if (%1)==() goto start_zip

if "%1"=="-help"    goto help_show
if "%1"=="-h"       goto help_show

if "%1"=="-b"       goto get_build
if "%1"=="-build"   goto get_build

if "%1"=="-o"       goto get_output
if "%1"=="-outpath" goto get_output

if "%1"=="-i"       goto get_input
if "%1"=="-inpath"  goto get_input

goto custom_error

:get_build
SET FDOBUILDNUMBERRTB=%2
goto next_param

:get_input
SET FDOSOURCEZIP=%2
goto next_param

:get_output
if (%2)==() goto custom_error
if not exist "%~2" mkdir "%~2"
pushd "%~2"
SET FDOTARGETFOLDER=%cd%
popd

:next_param
shift
shift
goto study_params

:start_zip
	SET ENV_EMPTY=FDOMYSQL
	if ("%FDOMYSQL%")==("") goto custom_error_ex
	if not exist "%FDOMYSQL%" goto custom_error_ex
	SET ENV_EMPTY=SDEHOME
	if ("%SDEHOME%")==("") goto custom_error_ex
	if not exist "%SDEHOME%" goto custom_error_ex
	if not exist "%FDOTARGETFOLDER%" mkdir "%FDOTARGETFOLDER%"
	pushd "%FDOTARGETFOLDER%"
	SET FDOTARGETFOLDER=%cd%
	popd
	if exist "%FDOTARGETFOLDER%\%FDOTARZIPFOLDERRTB%" rmdir /S /Q "%FDOTARGETFOLDER%\%FDOTARZIPFOLDERRTB%"

	if not ("%FDOBUILDNUMBERRTB%") == ("GXXX") goto start_copy
	if not exist "%FDOSOURCEZIP%\release.txt" echo Release not ready yet. Waiting...
	for /L %%g in (1,1,48) do if not exist "%FDOSOURCEZIP%\release.txt" perl -e "sleep(3);"
	if exist "%FDOSOURCEZIP%\release.txt" goto get_buildno
	echo Error: time out waiting for the release...
	exit /B 1
:get_buildno
	for /F %%f in (%FDOSOURCEZIP%\release.txt) do SET FDOBUILDNUMBERRTB=%%f
:start_copy
   copy /Y "%FDOSOURCEZIP%\fdo-3.2.0_%FDOBUILDNUMBERRTB%.tar.gz" "%FDOTARGETFOLDER%"
   copy /Y "%FDOSOURCEZIP%\fdoshp-3.2.0_%FDOBUILDNUMBERRTB%.tar.gz" "%FDOTARGETFOLDER%"
   copy /Y "%FDOSOURCEZIP%\fdosdf-3.2.0_%FDOBUILDNUMBERRTB%.tar.gz" "%FDOTARGETFOLDER%"
   copy /Y "%FDOSOURCEZIP%\fdowfs-3.2.0_%FDOBUILDNUMBERRTB%.tar.gz" "%FDOTARGETFOLDER%"
   copy /Y "%FDOSOURCEZIP%\fdowms-3.2.0_%FDOBUILDNUMBERRTB%.tar.gz" "%FDOTARGETFOLDER%"
   copy /Y "%FDOSOURCEZIP%\fdoarcsde-3.2.0_%FDOBUILDNUMBERRTB%.tar.gz" "%FDOTARGETFOLDER%"
   copy /Y "%FDOSOURCEZIP%\fdordbms-3.2.0_%FDOBUILDNUMBERRTB%.tar.gz" "%FDOTARGETFOLDER%"
   copy /Y "%FDOSOURCEZIP%\fdogdal-3.2.0_%FDOBUILDNUMBERRTB%.tar.gz" "%FDOTARGETFOLDER%"
:start_unzip
   echo *start unzip files
   pushd "%FDOTARGETFOLDER%"
   7z x -ry "fdo-3.2.0_%FDOBUILDNUMBERRTB%.tar.gz" >>nul
   7z x -ry "fdoshp-3.2.0_%FDOBUILDNUMBERRTB%.tar.gz" >>nul
   7z x -ry "fdosdf-3.2.0_%FDOBUILDNUMBERRTB%.tar.gz" >>nul
   7z x -ry "fdowfs-3.2.0_%FDOBUILDNUMBERRTB%.tar.gz" >>nul
   7z x -ry "fdowms-3.2.0_%FDOBUILDNUMBERRTB%.tar.gz" >>nul
   7z x -ry "fdoarcsde-3.2.0_%FDOBUILDNUMBERRTB%.tar.gz" >>nul
   7z x -ry "fdordbms-3.2.0_%FDOBUILDNUMBERRTB%.tar.gz" >>nul
   7z x -ry "fdogdal-3.2.0_%FDOBUILDNUMBERRTB%.tar.gz" >>nul
   
   7z x -ry "fdo-3.2.0_%FDOBUILDNUMBERRTB%.tar" >>nul
   7z x -ry "fdoshp-3.2.0_%FDOBUILDNUMBERRTB%.tar" >>nul
   7z x -ry "fdosdf-3.2.0_%FDOBUILDNUMBERRTB%.tar" >>nul
   7z x -ry "fdowfs-3.2.0_%FDOBUILDNUMBERRTB%.tar" >>nul
   7z x -ry "fdowms-3.2.0_%FDOBUILDNUMBERRTB%.tar" >>nul
   7z x -ry "fdoarcsde-3.2.0_%FDOBUILDNUMBERRTB%.tar" >>nul
   7z x -ry "fdordbms-3.2.0_%FDOBUILDNUMBERRTB%.tar" >>nul
   7z x -ry "fdogdal-3.2.0_%FDOBUILDNUMBERRTB%.tar" >>nul
   
   del /Q /F "*.tar"
   echo unzip files done...
   popd

:start_build
	SET FDOERROR=0
	pushd "%FDOTARGETFOLDER%\%FDOTARZIPFOLDERRTB%"
	SET FDO=%cd%\Fdo
	SET FDOUTILITIES=%cd%\Utilities
	SET FDOTHIRDPARTY=%cd%\Thirdparty
	SET XALANROOT=%FDOTHIRDPARTY%\apache\xml-xalan\c
	SET XERCESCROOT=%FDOTHIRDPARTY%\apache\xml-xerces\c
	SET NLSDIR=%FDOTHIRDPARTY%\apache\xml-xalan\c\Src\xalanc\NLS
	popd
	
:start_build_debug
	echo *Starting DEBUG build
	pushd "%FDOTARGETFOLDER%\%FDOTARZIPFOLDERRTB%"
	if exist thirdparty_debug_build_log.txt del /Q /F thirdparty_debug_build_log.txt
	call build_thirdparty.bat -c=debug ^> thirdparty_debug_build_log.txt
	SET FDOERRORMESSAGE="Thirdparty debug build returned an error, please check thirdparty_debug_build_log.txt for more information"
	if "%FDOERROR%"=="1" goto error
	if exist fdocore_debug_build_log.txt del /Q /F fdocore_debug_build_log.txt
	call build.bat -c=debug -d=build ^> fdocore_debug_build_log.txt
	SET FDOERRORMESSAGE="Fdocore debug build returned an error, please check fdocore_debug_build_log.txt for more information"
	if "%FDOERROR%"=="1" goto error
	popd
	echo End DEBUG build...

:start_build_release
	echo *Starting RELEASE build
	pushd "%FDOTARGETFOLDER%\%FDOTARZIPFOLDERRTB%"
	if exist thirdparty_release_build_log.txt del /Q /F thirdparty_release_build_log.txt
	call build_thirdparty.bat -c=release ^> thirdparty_release_build_log.txt
	SET FDOERRORMESSAGE="Thirdparty release build returned an error, please check thirdparty_release_build_log.txt for more information"
	if "%FDOERROR%"=="1" goto error
	if exist fdocore_release_build_log.txt del /Q /F fdocore_release_build_log.txt
	call build.bat -c=release -d=build ^> fdocore_release_build_log.txt
	SET FDOERRORMESSAGE="Fdocore release build returned an error, please check fdocore_release_build_log.txt for more information"
	if "%FDOERROR%"=="1" goto error
	popd
	echo End RELEASE build...

:start_tests
:fdo_test
	pushd "%FDOTARGETFOLDER%\%FDOTARZIPFOLDERRTB%"
	echo *starting fdo tests
	if exist Fdo_unit_test_log.txt del /Q /F Fdo_unit_test_log.txt
	pushd Fdo\Unmanaged\Bin\Win32\Debug
	UnitTest.exe -NoWAIT >..\..\..\..\..\Fdo_unit_test_log.txt
	if errorlevel 1 echo Fdo unit test returned an error, please check Fdo_unit_test_log.txt for more information
	popd
:shp_test
	echo *starting shp tests
	if exist Shp_unit_test_log.txt del /Q /F Shp_unit_test_log.txt
	pushd Providers\SHP\src\UnitTest
	..\..\Bin\win32\debug\UnitTest.exe -NoWAIT >..\..\..\..\Shp_unit_test_log.txt
	if errorlevel 1 echo Shp unit test returned an error, please check Shp_unit_test_log.txt for more information
	popd
:sdf_test
	echo *starting sdf tests
	if exist SDF_unit_test_log.txt del /Q /F SDF_unit_test_log.txt
	pushd Providers\SDF\Bin\Win32
	Debug\UnitTest.exe -NoWAIT >..\..\..\..\SDF_unit_test_log.txt
	if errorlevel 1 echo Sdf unit test returned an error, please check SDF_unit_test_log.txt for more information
	popd
:wms_test
	echo *starting wms tests
	if exist WMS_unit_test_log.txt del /Q /F WMS_unit_test_log.txt
	pushd Providers\WMS\Bin\Win32\Debug
	UnitTest.exe -NoWAIT >..\..\..\..\..\WMS_unit_test_log.txt
	if errorlevel 1 echo Wms unit test returned an error, please check Wms_unit_test_log.txt for more information
	popd
:arcsde_test
	echo *starting arcsde tests
	if exist ArcSDE_unit_test_log.txt del /Q /F ArcSDE_unit_test_log.txt
	pushd Providers\ArcSDE\Bin\Win32\Debug
	UnitTest.exe server=adfdo.dnsalias.com port_multi=5151/tcp port_single=5151/tcp sdepwd=system dataset=FC4 rdbms=ORACLE -NoWAIT >..\..\..\..\..\ArcSDE_unit_test_log.txt
	if errorlevel 1 echo ArcSDE unit test returned an error, please check ArcSDE_unit_test_log.txt for more information
	popd
:gdal_test
	echo *starting gdal tests
	if exist GDAL_unit_test_log.txt del /Q /F GDAL_unit_test_log.txt
	pushd Providers\ArcSDE\Src\UnitTest
	..\..\Bin\Win32\Debug\UnitTest.exe -NoWAIT >..\..\..\..\GDAL_unit_test_log.txt
	if errorlevel 1 echo GDAL unit test returned an error, please check GDAL_unit_test_log.txt for more information
	popd

	popd
	
:end
echo Build test ended
exit /B 0

:custom_error_ex
echo *
echo Setup Error:
echo Environment variable %ENV_EMPTY% is undefined or contains an invalid path
echo Please setup this environment variable and try again
exit /B 1

:error
echo %FDOERRORMESSAGE%.
exit /B 1

:custom_error
echo The command is not recognized.
echo Please use the format:
:help_show
echo **************************************************************************
echo NOTE: To use the runtestbuild script, download and install the 7z  
echo       command-line and Windows shell utility for manipulating ZIP 
echo       archives. 7-Zip is free software distributed under the GNU LGPL. 
echo       7z can be obtained from http://www.7-zip.org/
echo *
echo runtestbuild.bat [-h] [-i=InFolder] [-o=OutFolder] [-b=BuildNumber]
echo *
echo Help:           -h[elp]
echo InFolder:       -i[npath]=input zip source folder
echo OutFolder:      -o[utpath]=destination build test folder, default (C:\OpenSource)
echo BuildNumber:    -b[uild]=User-Defined build number appended to the end of the tar.gz files
echo **************************************************************************

exit /B 0
