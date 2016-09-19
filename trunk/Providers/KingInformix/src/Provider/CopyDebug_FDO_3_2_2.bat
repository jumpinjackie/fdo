


REM ****************************************
REM * Copy to Workdir
REM ****************************************

REM copy /y "..\..\Bin\Win32\D_Fdo_3_2_2\*.dll" "..\..\..\..\..\..\..\..\..\Development\MapGuide OpenSource\WorkDir\bin\FDO\"
REM copy /y "..\..\Bin\Win32\D_Fdo_3_2_2\*.pdb" "..\..\..\..\..\..\..\..\..\Development\MapGuide OpenSource\WorkDir\bin\FDO\"






REM ****************************************
REM * Copy to Unit test
REM ****************************************

copy /y "..\..\Bin\Win32\D_Fdo_3_2_2\*.dll" "..\Unittest\D_FDO_3_2_2\"
copy /y "..\..\Bin\Win32\D_Fdo_3_2_2\*.pdb" "..\Unittest\D_FDO_3_2_2\"

REM ****************************************
REM * Copy to Fdo2Fdo test
REM ****************************************

copy /y "..\..\Bin\Win32\D_Fdo_3_2_2\*.dll" "..\..\..\..\..\..\..\..\..\Development\Fdo2Fdo\v1.1\debug\"
copy /y "..\..\Bin\Win32\D_Fdo_3_2_2\*.pdb" "..\..\..\..\..\..\..\..\..\Development\Fdo2Fdo\v1.1\debug\"


