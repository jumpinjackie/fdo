Feature Data Objects (FDO) OpenSource Test Data README
======================================================

This ReadMe contains important information required to Run and Test 
the FDO Open Source build using the FDO Unit Test execucatbles. 
Ensure you read through this document before running the FDO Unit 
Tests. Also, be sure to first follow the FDO OpenSource build steps 
located in OpenSourceBuild__README.txt

Contents:
Windows instructions
   - Windows Notes
   - Windows Steps for Extracting UnitTest Data
Linux RedHat 3.0 instructions
   - Linux Notes
   - Linux Steps for Extracting UnitTest Data


-----------------------------

Windows Instructions
-----------------------------

I. Windows Notes:

  1. Extract the FDO Unit Test data to the same location as the FDO OpenSource
     Files (see OpenSourceBuild__README.txt)

  2. In order to run the UnitTests from within VisualStudio .Net using Providers.sln:

     (NOTE: Be sure to substitute <OpenSource_FDO> for your build location)


     1) Open and build Providers.sln
 
            a) devenv.exe Providers.sln 

            b) In Visual Studio .Net, choose menu option: Build->Build Solution

     2) In order to run the SDF Provider Unit Tests from within Visual Studio .Net
  
            a) Open Solution Folder ‘SDF Provider->Unit Tests’
 
            b) Right-Click on the UnitTest project

            c) Select ‘Set as StartUp Project’

            d) Right-Click on the UnitTest project

            e) Select ‘Properties’

            f) Select ‘Configuration Properties->Debugging’

            g) In the ‘Working Directory’ field enter the value <OpenSource_FDO>\Providers\SDFPlus\UnitTest\Debug\win32
  
            h) Select Ok and close the dialog

            i) Select Menu Item ‘Debug->Start Debugging’

    3) In order to run the SDF Provider Unit Tests from the command line

            cd <OpenSource_FDO>\Providers\SDFPlus\UnitTest\Debug\win32
            .\UnitTest

    4) NOTE: If the SDF UnitTests are run two consecutive times, the following errors will be encountered. This is expected.

            !!!FAILURES!!!
            Test Results:
            Run:  50   Failures: 3   Errors: 0

            1) test: MasterTest.keyFilterBeforeDelete (F) line: 252 c:\testbuild\opensource_fdo\Providers\sdfplus\unittest\Src\mastertest.cpp
             "count2 == 154"

            2) test: MasterTest.keyFilterBeforeDelete (F) line: 252 c:\testbuild\opensource_fdo\Providers\sdfplus\unittest\Src\mastertest.cpp
             "count2 == 154"

            3) test: MasterTest.deleteTest (F) line: 145 c:\testbuild\opensource_fdo\Providers\sdfplus\unittest\Src\mastertest.cpp
             "count == 2"

    5) In order to run the SHP Provider Unit Tests from within Visual Studio .Net

            a) Open Solution Folder ‘Shape Provider->Unit Tests’

            b) Right-Click on the UnitTest project

            c) Select ‘Set as StartUp Project’

            d) Right-Click on the UnitTest project

            e) Select ‘Properties’

            f) Select ‘Configuration Properties->Debugging’

            g) In the ‘Working Directory’ field enter the value <OpenSource_FDO>\Providers\SHP\Src\UnitTest

            h) Select Ok and close the dialog

            i) Select Menu Item ‘Debug->Start Debugging’

    3) In order to run the SHP Provider Unit Tests from the command line

            cd <OpenSource_FDO>\Providers\SHP\Src\UnitTest
            .\Dbg\UnitTest


II. Windows Steps for Extracting UnitTest Data: 

  1.  Extract the FDO OpenSource test data files using either gunzip/tar 
      or Winzip.

       Use gunzip and tar utilities as follows:

            gunzip fdo-3.2.0_data.tar.gz
            tar -xvf fdo-3.2.0_data.tar

       - or -

       Use Windows Winzip utility as follows:

            WINZIP32.EXE fdo-3.2.0_data.tar.gz

-----------------------------

Linux Redhat 3.0 Instructions
-----------------------------
 
I. Linux Notes:

  1. Extract the FDO Unit Test data to the same location as the FDO OpenSource
     Files (see OpenSourceBuild__README.txt)

  2. To run the FDO unit tests, you must set LD_LIBRARY_PATH as follows:

     $ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:
/usr/local/gis/fdo/3.0:/usr/local/gis/fdo/3.0/SHP:/usr/local/gis/fdo/3.0/SDF:
$FDOTHIRDPARTY/linux/cppunit/lib
      
     (Note: The above path should be entered all on one line.)

  3. Due to a path issue encountered in the FDO OpenSource Unit test(s), you must
     move some files in a directory.

     After the FDO UnitTest data files are extracted, move the directory:

     <OpenSource_FDO>/Providers/SDFPlus/UnitTest/data/ 
     to 
     <OpenSource_FDO/Providers/data/

  4. In order to run the various FDO Provider UnitTests on Linux follow 
     the following steps:

     'FDO SDF Provider'

      cd <OpenSource_FDO>/Providers/SDFPlus/UnitTest
      run ./UnitTest

     'FDO SDF Provider'

      cd <OpenSource_FDO>/Providers/SHP/Src/UnitTest
      run ./UnitTest
    

II. Linux Steps for Extracting UnitTest Data:

  1.  gunzip fdo-3.2.0_data.tar.gz

  2.  tar -xvf fdo-3.2.0_data.tar.gz

<end>