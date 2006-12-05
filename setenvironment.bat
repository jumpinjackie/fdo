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

if not exist %SDEHOME% SET SDEHOME="%FDOTHIRDPARTY%\ESRI\ArcSDEClient91\Windows"
if not exist %SDEHOME% echo Optional FDO environment variable SDEHOME is not set

if not exist %FDOMYSQL% SET FDOMYSQL="%PROGRAMFILES%\MySQL\MySQL Server 5.0"
if not exist %FDOMYSQL% echo Optional FDO environment variable FDOMYSQL is not set

SET ACTIVENAMECHECK="Microsoft .Net Framework"
SET ACTIVEPATHCHECK="%windir%\Microsoft.NET\Framework\v2.0.50727"
if not exist %ACTIVEPATHCHECK% goto error
set PATH=%PATH%;%ACTIVEPATHCHECK%

SET ACTIVENAMECHECK="Microsoft Visual Studio 8 IDE"
SET ACTIVEPATHCHECK="%PROGRAMFILES%\Microsoft Visual Studio 8\Common7\IDE"
if exist %ACTIVEPATHCHECK% set PATH=%PATH%;%ACTIVEPATHCHECK%

SET ACTIVENAMECHECK="7-Zip"
SET ACTIVEPATHCHECK="%PROGRAMFILES%\7-Zip"
if exist %ACTIVEPATHCHECK% set PATH=%PATH%;%ACTIVEPATHCHECK%

SET ACTIVENAMECHECK="Graphviz"
SET ACTIVEPATHCHECK="%PROGRAMFILES%\ATT\Graphviz\bin"
if exist %ACTIVEPATHCHECK% set PATH=%PATH%;%ACTIVEPATHCHECK%
if not exist %ACTIVEPATHCHECK% echo Optional %ACTIVENAMECHECK% documentation package not recognized in the Windows PATH. Update the setenvironment.bat script file with the correct path

SET ACTIVENAMECHECK="Doxygen"
SET ACTIVEPATHCHECK="%PROGRAMFILES%\doxygen\bin"
if exist %ACTIVEPATHCHECK% set PATH=%PATH%;%ACTIVEPATHCHECK%
if not exist %ACTIVEPATHCHECK% echo Optional %ACTIVENAMECHECK% documentation package not recognized in the Windows PATH. Update the setenvironment.bat script file with the correct path

SET ACTIVENAMECHECK="GnuWin32 Bison/Sed"
SET ACTIVEPATHCHECK="%PROGRAMFILES%\GnuWin32\bin"
if exist %ACTIVEPATHCHECK% set PATH=%PATH%;%ACTIVEPATHCHECK%
if not exist %ACTIVEPATHCHECK% echo Optional %ACTIVENAMECHECK% package not recognized in the Windows PATH. Update the setenvironment.bat script file with the correct path

:end
exit /B 0

:error
echo Unable to find %ACTIVEPATHCHECK%
echo Please update setenvironment script file with the right %ACTIVENAMECHECK% path
exit /B 1
