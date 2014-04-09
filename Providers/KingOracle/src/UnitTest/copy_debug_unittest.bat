REM SET DESTDIR=%1
SET DESTDIR=%~dp0\x64\Debug


SET KINGORA_SOURCEDIR=%~dp0\..\..\bin\Win64\Debug

 
 xcopy "%KINGORA_SOURCEDIR%\*.dll" %DESTDIR%  /D /Y
 if %errorlevel% neq 0 goto exit_on_copyerror
 xcopy "%KINGORA_SOURCEDIR%\*.pdb" %DESTDIR%  /D /Y
 if %errorlevel% neq 0 goto exit_on_copyerror
 echo King.Oracle files to %DESTDIR%

exit /B %errorlevel%


:exit_on_copyerror
echo Error copy DEEO files to %DESTDIR%
pause
exit /B %errorlevel%