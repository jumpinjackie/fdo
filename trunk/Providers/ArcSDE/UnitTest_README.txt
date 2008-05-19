
This README file explains how to install (or uninstall) test users and test data that are required in order
to run the FDO ArcSDE Provider unit tests, as well as how to run the unit tests after the test data has been installed.


--------------------------------------------------------------------------------------------
Installing/Uninstalling Test Users and Test Data
--------------------------------------------------------------------------------------------

Note that the instructions in this section are currently for Windows only.

1) If you are installing against ArcSDE for SQL Server, you will need to create the following two databases:

    - Create a database called "testmultidb" manually via SQL Server Enterprise Manager tool

    - Create a database called "testsingledb" using the ArcSDE Server Post-Installer tool:
         - make sure environment variable SDEHOME is pointing to the right place
             (so $SDEHOME\etc contains giomgr.defs, dbinit.sde, etc)
         - select "Complete" (instead of "Custom")
         - set database name to "testsingledb"
         - set service name (e.g. "esri_sde1b") and service port number (e.g. "5153/tcp") as desired
         - click OK to start the new ArcSDE service

2) Make sure to have a Perl interpreter installed and available.
For Windows install ActiveState Perl, available from: http://www.activestate.com/ActivePerl

3) Edit the "HARDCODED SETTINGS" section within the SetupTestData.perl script.  This section contains settings for
both Oracle and SQL Server.  If you wish to install test data in an oracle instance, set $INSTALLORACLE="Y" otherwise
set $INSTALLORACLE="N".  If you wish to install test data in a SQL Server instance, set $INSTALLSQLSERVER="Y" otherwise
set $INSTALLSQLSERVER="N".  Note that you can install test data against both Oracle and SQL Server simultaneously.
Make sure to verify the values of all settings; descriptions of each setting is provided within the perl script itself.

4) Run the SetupTestData.perl script (e.g. "perl SetupTestData.perl" from the command line).
Note that the script needs to be run on the same machine as the ArcSDE Server(s) you are installing against.
Also, Oracle's sqlplus.exe and/or SQL Server's osql.exe need to be in your PATH environment variable so they can be found.

5) You will be prompted as to whether or not you wish to install (I) or uninstall (U) test data.

6) You will be prompted as to which test 'dataset' you wish to use when installing the test data.
Note that this dataset name is independent of any operating system user name, or ArcSDE user name.
It is simply used to create RDBMS test users with names in the form "FDO_<dataset>_<suffix>"; these test users
will own the test data.  Note that due to ArcSDE limitations, dataset names longer than 5 characters may cause issues on ArcSDE for SQL Server.

7) The rest of the script should run automatically.
Note that the UnitTest portion of the script (near the end) may not run properly on Windows 2000.  In this case, you will have to run these
as a separate step on a Windows XP machine.
If you encounter any issues installing test users and/or test data, you can run the script again with the Uninstall ("U") option to
undo all changes, then identify the problem and attempt to reinstall the test users and data again.

You should now be ready to run the unit tests against your database and your selected dataset name.


--------------------------------------------------------------------------------------------
Running the Unit Tests
--------------------------------------------------------------------------------------------

The unit tests are built into an executable called "UnitTest.exe" on Windows and "UnitTest" on Linux.

Note that the UnitTest may not run properly on Windows 2000.  Use Windows XP instead if you encounter any issues.

You will need to provide some command-line arguments to the UnitTest executable in order
to point the tests to the location of your test data.  The following are the command-line arguments supported by UnitTest:


    <test names>         Optional; you can specify which test suite(s) to run (e.g. SelectTests).
                         If you do not specify any, all test suites are run.

    server=<value>       Required; you must specify which server to run the unit tests against.
                         This can be either an IP address or a DNS name.

    port_multi=<value>   Optional; you can specify which port on the server to use when running multi-database unit tests.
                         Defaults to "5151/tcp".

    port_single=<value>  Optional; you can specify which port on the server to use when running multi-database unit tests.
                         Defaults to "5151/tcp".

    sdepwd=<value>       Optional; you can specify the "sde" username's password.  This is used by only a few basic tests.
                         Defaults to "sde".

    dataset=<value>     Required; you must specify which 'dataset' to test against.  This vaue should match exactly
                         the dataset you specified when installing test data with the SetupTestData.perl script.

    rdbms=<value>        Required; you must specify which RDBMS you are testing against.  Some tests behave
                         differently on Oracle than on SQL Server and must be taken into account.
                         The only values currently accepted are "ORACLE" and "SQLSERVER".

