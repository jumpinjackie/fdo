@echo off
REM Overview:
REM  Given existing FDO binary output, prepare the nuget content and lib directories using the
REM  existing file lists to filter out the correct files.
REM
REM  Once in place, pack all the specified .nuspec files

SET OLD_PATH=%PATH%
SET PATH=%PATH%;%CD%
SET HAVE_ARCSDE=1
SET FDO_BIN_SRC=%CD%\..\SDK64
SET TMP_FILE_LIST=tmp_exclude.txt
SET FX_VERSION=net40
SET PACKAGE_ROOT=
SET PACKAGE_LIB=
SET PACKAGE_CONTENT=
SET CURRENT_PACKAGE=
SET CPU_PLATFORM=x64
SET FDO_VERSION=3.7.0.4308

call:make_content_package fdo-core
call:make_content_package fdo-rdbms
call:make_content_package fdo-provider-arcsde
call:make_content_package fdo-provider-gdal
call:make_content_package fdo-provider-mysql
call:make_content_package fdo-provider-odbc
call:make_content_package fdo-provider-ogr
call:make_content_package fdo-provider-oracle
call:make_content_package fdo-provider-postgresql
call:make_content_package fdo-provider-sdf
call:make_content_package fdo-provider-shp
call:make_content_package fdo-provider-sqlite
call:make_content_package fdo-provider-sqlspatial
call:make_content_package fdo-provider-wfs
call:make_content_package fdo-provider-wms

rem HACK: fdo-core-mgd doesn't want to play by our file-exclusion based
rem preparation mechanism. So copy the files manually
echo === Making lib package: fdo-core-mgd ===
SET PACKAGE_ROOT=%CD%\%CPU_PLATFORM%\fdo-core-mgd
SET PACKAGE_LIB=%PACKAGE_ROOT%\lib\%FX_VERSION%
SET PACKAGE_CONTENT=%PACKAGE_ROOT%\content\%FX_VERSION%\fdo-libs
copy /Y "%FDO_BIN_SRC%\OSGeo.FDO.dll" "%PACKAGE_LIB%"
copy /Y "%FDO_BIN_SRC%\OSGeo.FDO.Common.dll" "%PACKAGE_LIB%"
copy /Y "%FDO_BIN_SRC%\OSGeo.FDO.Geometry.dll" "%PACKAGE_LIB%"
copy /Y "%FDO_BIN_SRC%\OSGeo.FDO.Spatial.dll" "%PACKAGE_LIB%"
pushd %CPU_PLATFORM%
nuget pack fdo-core-mgd.nuspec -BasePath fdo-core-mgd -Version %FDO_VERSION%
popd

call:make_lib_package fdo-rdbms-mgd
call:make_lib_package fdo-provider-mysql-mgd
call:make_lib_package fdo-provider-odbc-mgd
call:make_lib_package fdo-provider-shp-mgd
call:make_lib_package fdo-provider-sqlspatial-mgd
call:make_lib_package fdo-provider-wms-mgd

rem Despite being a meta-package, still need a stub basepath to prevent
rem accidental inclusion of content
pushd %CPU_PLATFORM%
nuget pack fdo-sdk.nuspec -BasePath fdo-sdk -Version %FDO_VERSION%
popd
goto end

:make_content_package
SET CURRENT_PACKAGE=%~1
echo === Making content package: %CURRENT_PACKAGE% ===
call prepare_exclude.bat %CURRENT_PACKAGE%
SET PACKAGE_ROOT=%CD%\%CPU_PLATFORM%\%CURRENT_PACKAGE%
SET PACKAGE_LIB=%PACKAGE_ROOT%\lib\%FX_VERSION%
SET PACKAGE_CONTENT=%PACKAGE_ROOT%\content\%FX_VERSION%\fdo-libs
xcopy /S /Y /I "%FDO_BIN_SRC%\*" "%PACKAGE_CONTENT%" /EXCLUDE:tmp_exclude_list.txt
pushd %CPU_PLATFORM%
nuget pack %CURRENT_PACKAGE%.nuspec -BasePath %CURRENT_PACKAGE% -Version %FDO_VERSION%
popd
goto:eof

:make_lib_package
SET CURRENT_PACKAGE=%~1
echo === Making lib package: %CURRENT_PACKAGE% ===
call prepare_exclude.bat %CURRENT_PACKAGE%
SET PACKAGE_ROOT=%CD%\%CPU_PLATFORM%\%CURRENT_PACKAGE%
SET PACKAGE_LIB=%PACKAGE_ROOT%\lib\%FX_VERSION%
SET PACKAGE_CONTENT=%PACKAGE_ROOT%\content\%FX_VERSION%\fdo-libs
xcopy /S /Y /I "%FDO_BIN_SRC%\*" "%PACKAGE_LIB%" /EXCLUDE:tmp_exclude_list.txt
pushd %CPU_PLATFORM%
nuget pack %CURRENT_PACKAGE%.nuspec -BasePath %CURRENT_PACKAGE% -Version %FDO_VERSION%
popd
goto:eof

:end
SET PATH=%OLD_PATH%
SET HAVE_ARCSDE=
SET FDO_BIN_SRC=
SET TMP_FILE_LIST=
SET FX_VERSION=
SET PACKAGE_ROOT=
SET PACKAGE_LIB=
SET PACKAGE_CONTENT=
SET CURRENT_PACKAGE=
SET CPU_PLATFORM=
SET FDO_VERSION=