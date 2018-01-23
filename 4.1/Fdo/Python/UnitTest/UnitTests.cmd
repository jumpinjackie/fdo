@setlocal
@echo off

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

:: Check for the required files and directories
if NOT exist %FDO%\Python\Lib\Win32 GOTO NO_WRAPPERS
if NOT exist %FDO%\Python\Bin\Win32\Release\_FDO.dll GOTO NO_WRAPPERS
if NOT exist %FDO%\Python\Lib\Win32\FDO.py GOTO NO_WRAPPERS
if NOT exist %FDO%\Unmanaged\bin\win32\Release GOTO NO_FDO_SDK

:: Set Environment
call .\SetEnvironment.bat

:: Display the current environment
echo ---------------------------------------------------------------------
echo Script: UnitTests.cmd
echo ---------------------------------------------------------------------
echo Using the following environment:
echo FDO:                %FDO%
echo PYTHON_HOME:        %PYTHON_HOME%
echo PYTHONPATH:         %PYTHONPATH%
echo ---------------------------------------------------------------------

:: Execute the unit test script
python Src/main.py
GOTO END

:: Show error message - FDO wrapper DLLs are missing
:NO_WRAPPERS
echo Script: unittest.cmd
echo -------------------------------------------------------------------------
echo This script requires the FDO and FDO Python Wrapper DLLs to exist
GOTO END


:: Show error message - FDO is missing
:NO_FDO_SDK
echo Script: unittest.cmd
echo -------------------------------------------------------------------------
echo This script requires FDO to be built in Release Mode. Please rebuild FDO
echo before running the unit tests
GOTO END


:END
exit /B 0
