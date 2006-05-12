@echo off
time /t
echo Start Settings

SET ACTIVEPATHCHECK="C:\WINDOWS\Microsoft.NET\Framework\v2.0.50727"
SET ACTIVENAMECHECK="Framework"
if not exist %ACTIVEPATHCHECK% goto error
set PATH=%PATH%;%ACTIVEPATHCHECK%

SET ACTIVEPATHCHECK="C:\Program Files\ATT\Graphviz\bin"
SET ACTIVENAMECHECK="Graphviz"
if not exist %ACTIVEPATHCHECK% goto error
set PATH=%PATH%;%ACTIVEPATHCHECK%

SET ACTIVEPATHCHECK="C:\Program Files\doxygen\bin"
SET ACTIVENAMECHECK="doxygen"
if not exist %ACTIVEPATHCHECK% goto error
set PATH=%PATH%;%ACTIVEPATHCHECK%

SET ACTIVEPATHCHECK="C:\Program Files\Microsoft Visual Studio 8\Common7\IDE"
SET ACTIVENAMECHECK="Microsoft Visual Studio 8 IDE"
if not exist %ACTIVEPATHCHECK% goto error
set PATH=%PATH%;%ACTIVEPATHCHECK%

SET FDO=%cd%\Fdo
SET ACTIVEPATHCHECK=%FDO%
SET ACTIVENAMECHECK="Fdo"
if not exist "%FDO%" goto error

SET FDOTHIRDPARTY=%cd%\Thirdparty
SET ACTIVEPATHCHECK=%FDOTHIRDPARTY%
SET ACTIVENAMECHECK="Fdo Thirdparty"
if not exist "%FDOTHIRDPARTY%" goto error

SET XALANROOT=%FDOTHIRDPARTY%\apache\xml-xalan\c
SET XERCESCROOT=%FDOTHIRDPARTY%\apache\xml-xerces\c
SET NLSDIR=%FDOTHIRDPARTY%\apache\xml-xalan\c\Src\xalanc\NLS

SET FDOUTILITIES=%cd%\Utilities
SET ACTIVEPATHCHECK=%FDOUTILITIES%
SET ACTIVENAMECHECK="Fdo Utilities"
if not exist "%FDOUTILITIES%" goto error

SET SDEHOME=%FDOTHIRDPARTY%\ESRI\ArcSDEClient91\Windows
SET ACTIVEPATHCHECK=%SDEHOME%
SET ACTIVENAMECHECK="Fdo thirdparty ESRI libraries"
if not exist "%SDEHOME%" goto error

SET FDOMYSQL=C:\Program Files\MySQL\MySQL Server 5.0
SET ACTIVEPATHCHECK=%FDOMYSQL%
SET ACTIVENAMECHECK="Fdo thirdparty MySQL libraries"
if not exist "%FDOMYSQL%" goto error

:end
echo End Settings
time /t
exit /B 0

:error
echo Unable to find %ACTIVEPATHCHECK%
echo Please update script file with the right %ACTIVENAMECHECK% path
time /t
exit /B 1