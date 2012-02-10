REM copy /y "..\..\Bin\Win32\Debug_Fdo_3_2_2\*.dll" "..\..\..\..\..\Server\bin\debug\FDO\"
REM copy /y "..\..\Bin\Win32\Debug_Fdo_3_2_2\*.pdb" "..\..\..\..\..\Server\bin\debug\FDO\"


REM ****************************************
REM * Copy to Workdir
REM ****************************************

REM copy /y "..\..\Bin\Win32\Debug_Fdo_3_2_2\*.dll" "..\..\..\..\..\..\..\..\..\Development\MapGuide OpenSource\WorkDir\bin\FDO\"
REM copy /y "..\..\Bin\Win32\Debug_Fdo_3_2_2\*.pdb" "..\..\..\..\..\..\..\..\..\Development\MapGuide OpenSource\WorkDir\bin\FDO\"



REM ****************************************
REM * Copy to Unit Test
REM ****************************************

copy /y "..\..\Bin\Win32\Rel_Fdo_3_3_1\*.dll" "..\UnitTest\bin\win32\Rel_Fdo_3_3_1\"
copy /y "..\..\Bin\Win32\Rel_Fdo_3_3_1\*.pdb" "..\UnitTest\bin\win32\Rel_Fdo_3_3_1\"

REM ****************************************
REM * Copy to Fdo2Fdo test
REM ****************************************

copy /y "..\..\Bin\Win32\Rel_Fdo_3_3_1\*.dll" "..\..\..\..\..\..\..\..\..\Development\Fdo2Fdo\v1.1\release\"





