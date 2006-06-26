@echo off

SET FDO=%cd%\Fdo
SET ACTIVEPATHCHECK=%FDO%
SET ACTIVENAMECHECK="FDO"
if not exist "%FDO%" goto error

SET FDOUTILITIES=%cd%\Utilities
SET ACTIVEPATHCHECK=%FDOUTILITIES%
SET ACTIVENAMECHECK="FDO Utilities"
if not exist "%FDOUTILITIES%" goto error

SET FDOTHIRDPARTY=%cd%\Thirdparty
SET ACTIVEPATHCHECK=%FDOTHIRDPARTY%
SET ACTIVENAMECHECK="FDO Thirdparty"
if not exist "%FDOTHIRDPARTY%" goto error

SET XALANROOT=%FDOTHIRDPARTY%\apache\xml-xalan\c
SET XERCESCROOT=%FDOTHIRDPARTY%\apache\xml-xerces\c
SET NLSDIR=%FDOTHIRDPARTY%\apache\xml-xalan\c\Src\xalanc\NLS

if not exist "%SDEHOME%" SET SDEHOME=%FDOTHIRDPARTY%\ESRI\ArcSDEClient91\Windows
if not exist "%SDEHOME%" echo Optional FDO environment variable SDEHOME is not set

if not exist "%FDOMYSQL%" SET FDOMYSQL=C:\Program Files\MySQL\MySQL Server 5.0
if not exist "%FDOMYSQL%" echo Optional FDO environment variable FDOMYSQL is not set

SET ACTIVENAMECHECK="Microsoft .Net Framework"
SET ACTIVEPATHCHECK="C:\WINDOWS\Microsoft.NET\Framework\v2.0.50727"
if not exist %ACTIVEPATHCHECK% goto error
set PATH=%PATH%;%ACTIVEPATHCHECK%

SET ACTIVENAMECHECK="Microsoft Visual Studio 8 IDE"
SET ACTIVEPATHCHECK="C:\Program Files\Microsoft Visual Studio 8\Common7\IDE"
if exist %ACTIVEPATHCHECK% set PATH=%PATH%;%ACTIVEPATHCHECK%

SET ACTIVENAMECHECK="Graphviz"
SET ACTIVEPATHCHECK="C:\Program Files\ATT\Graphviz\bin"
if exist %ACTIVEPATHCHECK% set PATH=%PATH%;%ACTIVEPATHCHECK%
if not exist %ACTIVEPATHCHECK% echo Optional %ACTIVENAMECHECK% documentation package not recognized in the Windows PATH. Update the setenvironment.bat script file with the correct path

SET ACTIVENAMECHECK="Doxygen"
SET ACTIVEPATHCHECK="C:\Program Files\doxygen\bin"
if exist %ACTIVEPATHCHECK% set PATH=%PATH%;%ACTIVEPATHCHECK%
if not exist %ACTIVEPATHCHECK% echo Optional %ACTIVENAMECHECK% documentation package not recognized in the Windows PATH. Update the setenvironment.bat script file with the correct path

SET ACTIVENAMECHECK="GnuWin32 Bison/Sed"
SET ACTIVEPATHCHECK="c:\Program Files\GnuWin32\bin"
if exist %ACTIVEPATHCHECK% set PATH=%PATH%;%ACTIVEPATHCHECK%
if not exist %ACTIVEPATHCHECK% echo Optional %ACTIVENAMECHECK% package not recognized in the Windows PATH. Update the setenvironment.bat script file with the correct path

:end
exit /B 0

:error
echo Unable to find %ACTIVEPATHCHECK%
echo Please update setenvironment script file with the right %ACTIVENAMECHECK% path
exit /B 1