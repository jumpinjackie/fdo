:@echo off

:: Add FDO to the PATH
SET PATH=%PATH%;%FDO%\Python\UnitTests\Bin\Win32\Release;%FDO%\Python\Bin\Win32\Release

:: Add _FDO dll and FDO.py to PYTHONPATH
SET PYTHONPATH=%PYTHONPATH%;%PYTHON_HOME%;%PYTHON_HOME%\Lib;%FDO%\Python\Lib\Win32;%FDO%\Python\UnitTests\Lib;%FDO%\Python\Bin\Win32\Release

:end
exit /B 0
