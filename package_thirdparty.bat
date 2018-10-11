@echo off
setlocal EnableExtensions EnableDelayedExpansion
set "INTEXTFILE=thirdparty_filelist_temp.txt"
set "OUTTEXTFILE=thirdparty_filelist.txt"
set "SEARCHTEXT=%CD%\"
set "REPLACETEXT="
where /R Thirdparty\ *.dll *.lib *.pdb Xerces_autoconf_config.hpp LocalMsgIndex.hpp opensslconf.h > %INTEXTFILE%
rem Catch everything in gdal\include as well
dir Thirdparty\gdal\include /S /B >> %INTEXTFILE%
for /f "delims=" %%A in ('type "%INTEXTFILE%"') do (
    set "string=%%A"
    set "modified=!string:%SEARCHTEXT%=%REPLACETEXT%!"
    echo !modified!>>"%OUTTEXTFILE%"
)

del "%INTEXTFILE%"
rename "%OUTTEXTFILE%" "%INTEXTFILE%"
7z a fdo_thirdparty.7z @%INTEXTFILE%
endlocal