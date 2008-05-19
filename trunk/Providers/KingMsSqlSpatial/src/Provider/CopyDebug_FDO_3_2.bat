copy /y "..\..\Bin\Win32\Debug_Fdo_3_2\*.dll" "..\..\..\..\..\Server\bin\debug\FDO\"
copy /y "..\..\Bin\Win32\Debug_Fdo_3_2\*.pdb" "..\..\..\..\..\Server\bin\debug\FDO\"


REM ****************************************
REM * Copy to Workdir
REM ****************************************

copy /y "..\..\Bin\Win32\Debug_Fdo_3_2\*.dll" "..\..\..\..\..\..\..\..\..\Development\MapGuide OpenSource\WorkDir\bin\FDO\"
copy /y "..\..\Bin\Win32\Debug_Fdo_3_2\*.pdb" "..\..\..\..\..\..\..\..\..\Development\MapGuide OpenSource\WorkDir\bin\FDO\"


REM ****************************************
REM * Copy to Unit Test
REM ****************************************

copy /y "..\..\Bin\Win32\Debug_Fdo_3_2\*.dll" "..\UnitTest\Debug_Fdo_3_2\"
copy /y "..\..\Bin\Win32\Debug_Fdo_3_2\*.pdb" "..\UnitTest\Debug_Fdo_3_2\"


REM ****************************************
REM * Copy to Fdo2Fdo test
REM ****************************************

copy /y "..\..\Bin\Win32\Debug_Fdo_3_2\*.dll" "..\..\..\..\..\..\..\..\..\Development\Fdo2Fdo\v1.1\debug\"
copy /y "..\..\Bin\Win32\Debug_Fdo_3_2\*.pdb" "..\..\..\..\..\..\..\..\..\Development\Fdo2Fdo\v1.1\debug\"


