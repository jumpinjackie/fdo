@echo off
@setlocal

::
:: Copyright (C) 2004-2007  Autodesk, Inc.
:: 
:: This library is free software; you can redistribute it and/or
:: modify it under the terms of version 2.1 of the GNU Lesser
:: General Public License as published by the Free Software Foundation.
:: 
:: This library is distributed in the hope that it will be useful,
:: but WITHOUT ANY WARRANTY; without even the implied warranty of
:: MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
:: Lesser General Public License for more details.
:: 
:: You should have received a copy of the GNU Lesser General Public
:: License along with this library; if not, write to the Free Software
:: Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
::
:: Description: Build the Windows version of the FDO Phython wrappers.  This script assumes
::              that the following software packages are installed:
::              - Microsoft Visual Studio .NET 2002
::              - SWIG 1.3.31
::              - Python 2.4.4
::  
::  NOTE: The following environment variables must be set:
::              - PYTHON_HOME: home directory of Python (e.g. c:\python24)
::              - SWIG_HOME:   home directory of SWIG (e.g. c:\swig)
::

:: Check for the required environment variables
if NOT defined PYTHON_HOME        goto NO_ENV_VARS
if NOT defined SWIG_HOME          goto NO_ENV_VARS

:: Check that the required files exist
if NOT exist "%PYTHON_HOME%\python.exe" goto NO_PYTHON_LIB
if NOT exist "%SWIG_HOME%\swig.exe" goto NO_SWIG
set PATH=%windir%\system32;%windir%;%PATH%

:: Compile the DLLs
echo %0: Build FDO Python Wrappers
msbuild PythonWrappers.sln /t:"rebuild" /p:Configuration="Release" /p:Platform="Win32" /nologo /consoleloggerparameters:NoSummary
SET FDOERROR=%errorlevel%
if "%FDOERROR%"=="1" goto error
goto END

:COMPILE_FAIL
echo %0: ERROR: Compile failed
echo %0: View build log at Fdo\Python\Lib\Win32\Release\Intermediate\BuildLog.htm
exit /B 1

:NO_ENV_VARS
echo This script requires the following environment variables to be set:
if not defined PYTHON_HOME  set PYTHON_HOME=
if not defined SWIG_HOME    set SWIG_HOME=

echo ------------ ------------------------------------- ------------------------
echo NAME         DESCRIPTION                   	    CURRENT VALUE
echo ------------ ------------------------------------- ------------------------
echo PYTHON_HOME  home directory of python              %PYTHON_HOME%
echo SWIG_HOME    home directory of SWIG                %SWIG_HOME%
echo ------------ ------------------------------------- ------------------------
echo Build aborted.
goto END

:NO_PYTHON_LIB
echo %PYTHON_HOME%\python.exe not found. 
echo Build aborted.
goto END

:NO_SWIG
echo %SWIG_HOME%\swig.exe not found. 
echo Build aborted.
goto END

:END
exit /B 0

