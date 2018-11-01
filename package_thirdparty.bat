@echo off
setlocal EnableExtensions EnableDelayedExpansion
set "INTEXTFILE=thirdparty_filelist_temp.txt"
set "OUTTEXTFILE=thirdparty_filelist.txt"
set "SEARCHTEXT=%CD%\"
set "REPLACETEXT="
rem These unused pdbs/libs will bloat our archive so delete them before packaging
if exist Thirdparty\gdal\apps\*.pdb del Thirdparty\gdal\apps\*.pdb
if exist Thirdparty\gdal\apps\*.lib del Thirdparty\gdal\apps\*.lib
if exist Thirdparty\openssl\test\*.pdb del Thirdparty\openssl\test\*.pdb
if exist Thirdparty\openssl\test\*.lib del Thirdparty\openssl\test\*.lib
if exist Thirdparty\openssl\fuzz\*.pdb del Thirdparty\openssl\fuzz\*.pdb
if exist Thirdparty\openssl\apps\*.pdb del Thirdparty\openssl\apps\*.pdb
if exist Thirdparty\openssl\apps\*.lib del Thirdparty\openssl\apps\*.lib
if exist Thirdparty\openssl\*.pdb del Thirdparty\openssl\*.pdb
if exist Thirdparty\openssl\*.lib del Thirdparty\openssl\*.lib
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