#!/usr/bin/perl 

# This script does not expect any command-line arguments.  Simply call with "perl SetupTestData.perl".
# Most configuration is hardcoded within the script; edit the section "Hardcoded Settings" before running.

RunMain ();

sub RunMain
{

# HARDCODED SETTINGS -- EDIT THIS PORTION OF THE SCRIPT BEFORE RUNNING!
# ===========================================================================================================

$SHP_ROOT="C:\\_Pierre\\TestDatabase";  # Root directory where the input SHP files are located (e.g. D:\\Providers\\ArcSDE\\TestData\\TestDatabase)
$ARCSDEUNITTESTDIR="C:\\_Pierre\\ArcSDEUnitTestDebug"; # Directory containing ArcSDE Provider's UnitText.exe

# Oracle settings:
$INSTALLORACLE="Y";  # Whether or not to install data for Oracle, can be "Y" or "N"
$ORACLEADMINNAME="sys";  # The Oracle user name used to create users and grant them privileges
$ORACLEADMINPASSWORD="system";  # The above Oracle user's password.
$ORACLESDEPORT="esri_sde_oracle";  # The port/protocol to use when connecting to ArcSDE Server
$ORACLESDEHOME="c:\\arcgis1\\arcsde\\ora9iexe";  # The SDEHOME directory for the Oracle ArcSDE Server
$ORACLESDEPASSWORD="system";  # The SDE database user's password
$ORACLESDESERVER="localhost";  # The ArcSDE Server's host name or IP
$ORACLESUPPORTSUSERMANAGEDSTRINGIDS="no";  # Whether or not this ArcSDE Server supports user-managed string id columns

# SQL Server settings:
$INSTALLSQLSERVER="N";  # Whether or not to install data for SQL Server, can be "Y" or "N"
$SQLSERVERADMINNAME="sa";  # The SQL Server user name used to create users and grant them privileges
$SQLSERVERADMINPASSWORD="system";  # The above SQL Server user's password.
$SQLSERVERSDEPORT_MULTIDB="5152/tcp";  # The port/protocol to use when connecting to ArcSDE Server (the "multidb" instance)
$SQLSERVERSDEPORT_SINGLEDB="5153/tcp";  # The port/protocol to use when connecting to ArcSDE Server (the "singledb" instance)
$SQLSERVERSDEHOME="c:\\arcgis\\arcsde\\sqlexe";  # The SDEHOME directory for the SQL Server ArcSDE Server
$SQLSERVERSDEPASSWORD="system";  # The SDE database user's password
$SQLSERVERSDESERVER="localhost";  # The ArcSDE Server's host name or IP
$SQLSERVERSUPPORTSUSERMANAGEDSTRINGIDS="no";  # Whether or not this ArcSDE Server supports user-managed string id columns


# Initial output
# ===========================================================================================================

print "\n\nPREREQUISITES FOR THIS SCRIPT TO RUN PROPERLY:\n";
print "\n - This script should be run from the ArcSDE server machine where Oracle and/or SQL Server instances are running.";
print "\n - Oracle's sqlplus.exe and/or SQL Server's osql.exe need to be in your PATH";
print "\n - For SQL Server, the following databases need to have previously been created as follows:";
print "\n     - create database \"testmultidb\" manually via SQL Server Enterprise Manager tool";
print "\n     - create database \"testsingledb\" using the ArcSDE Server Post-Installer tool:";
print "\n         - make sure environment variable SDEHOME is pointing to the right place";
print "\n             (so \$SDEHOME\\etc contains giomgr.defs, dbinit.sde, etc)";
print "\n         - select \"Complete\" (instead of \"Custom\")";
print "\n         - set database name to \"testsingledb\"";
print "\n         - set service name (e.g. \"esri_sde1b\") and service port number (e.g. \"5153/tcp\") as desired";
print "\n         - click OK to start the new ArcSDE service";
print "\n";

print "\n\nHARDCODED SETTINGS (EDIT THIS SCRIPT IF THESE VALUES ARE NOT CORRECT):\n";
print "\n  Root dir containing input SHP test data = " . $SHP_ROOT;
print "\n  Dir containing FDO Provider fod ArcSDE's UnitTest = " . $ARCSDEUNITTESTDIR;
print "\n  Install against ArcSDE for Oracle = " . $INSTALLORACLE;
print "\n  Oracle admin name = " . $ORACLEADMINNAME;
print "\n  Oracle admin password = " . $ORACLEADMINPASSWORD;
print "\n  Oracle ArcSDE server port = " . $ORACLESDEPORT;
print "\n  Oracle ArcSDE SDEHOME dir = " . $ORACLESDEHOME;
print "\n  Oracle ArcSDE SDE user password = " . $ORACLESDEPASSWORD;
print "\n  Oracle ArcSDE server address = " . $ORACLESDESERVER;
print "\n  Install against ArcSDE for SQL Server = " . $INSTALLSQLSERVER;
print "\n  SQL Server admin name = " . $SQLSERVERADMINNAME;
print "\n  SQL Server admin password = " . $SQLSERVERADMINPASSWORD;
print "\n  SQL Server ArcSDE server port (Multi database) = " . $SQLSERVERSDEPORT_MULTIDB;
print "\n  SQL Server ArcSDE server port (Single database) = " . $SQLSERVERSDEPORT_SINGLEDB;
print "\n  Root SQL Server ArcSDE server directory = " . $SQLSERVERSDEHOME;
print "\n  SQL Server ArcSDE SDE user password = " . $SQLSERVERSDEPASSWORD;
print "\n  SQL Server ArcSDE server address = " . $SQLSERVERSDESERVER;
print "\n";


# Gather basic input
# ===========================================================================================================

my $ACTION;
do
{
  print "\nEnter I to install new test data, U to uninstall existing test data, or Q to quit: ";
  chomp($INSTALLACTION=<STDIN>);
  $INSTALLACTION =~ tr/[a-z]/[A-Z]/;

  if ($INSTALLACTION eq "Q") {
    print "\nGoodbye.";
    exit;
  } elsif ($INSTALLACTION eq "U") {
    $ACTION = "Removing";
  } elsif ($INSTALLACTION eq "I") {
    $ACTION = "Creating";
  } else {
    print "\nOption '" . $INSTALLACTION . "' not supported, please try again.\n";
  }
} while ($ACTION eq "");


if ($INSTALLACTION ne "U") {
  print "\nName of test dataset to create: ";
} else {
  print "\nName of test dataset remove: ";
}
chomp($DATASETNAME=<STDIN>);

@usersuffixes = ('METADCOV', 'AUSTRALIA', 'WHEATON', 'OZZIE', 'FDOTEST1');
$username_metadcov  = 'FDO_' . $DATASETNAME . '_' . 'METADCOV';
$username_australia = 'FDO_' . $DATASETNAME . '_' . 'AUSTRALIA';
$username_wheaton   = 'FDO_' . $DATASETNAME . '_' . 'WHEATON';
$username_ozzie     = 'FDO_' . $DATASETNAME . '_' . 'OZZIE';
$username_fdotest1  = 'FDO_' . $DATASETNAME . '_' . 'FDOTEST1';
@usernames = ($username_metadcov,
              $username_australia,
              $username_wheaton,
              $username_ozzie,
              $username_fdotest1);



# Create users  (if installing only)
# ============================================================================================================

if ($INSTALLACTION ne "U") {
  if ($INSTALLORACLE eq "Y")
  {
    print "\nCreating users on Oracle ...";
  
    if (-e "temp.sql") { unlink("temp.sql"); }
  
    open (TEMPSQL, ">temp.sql");
    foreach $username (@usernames)
    {
      print TEMPSQL "\ncreate user " . $username . " identified by test;";
      print TEMPSQL "\ngrant connect, resource, select any table, insert any table, update any table, delete any table to ". $username . ";";
    }
    close (TEMPSQL);
  
    # Run the script:
    my $sysdba="";
    if ($ORACLEADMINNAME eq "sys") { $sysdba=" as sysdba "; }
    system "sqlplus $ORACLEADMINNAME/$ORACLEADMINPASSWORD " . $sysdba . " < temp.sql";
  }
  
  if ($INSTALLSQLSERVER eq "Y")
  {
    print "\nCreating users on SQL Server ...";
  
    if (-e "temp.sql") { unlink("temp.sql"); }
  
    open (TEMPSQL, ">temp.sql");
    foreach $username (@usernames)
    {
      print TEMPSQL "\nEXEC sp_addlogin '" . $username . "', 'test';";
    }
    foreach $username (@usernames, 'sde')
    {
      foreach $db ('master', 'sde', 'testsingledb', 'testmultidb')
      {
        # Grant permissions to all users (including "sde" user) on all databases (including "sde" database)
        # according to Config and Tuning Guide for SQL Server (see sections "Logins And Users: Rules For The Single/Multiple Spatial Database Model")
        # (in SQL Server Enterprise Manager, right-click on the database and select Properties > Permissions tab)
  
        print TEMPSQL "\nuse " . $db . ";";
        print TEMPSQL "\nEXEC sp_grantdbaccess '" . $username . "';";
        # Give users db_owner rights on databases;  this allows user to see eachother's tables as well as perform select/insert/update/delete on eachother's tables:
        print TEMPSQL "\nEXEC sp_addrolemember 'db_owner', '" . $username . "';";
        print TEMPSQL "\nGRANT CREATE TABLE, CREATE PROCEDURE, CREATE VIEW, CREATE FUNCTION TO " . $username . ";";
      }
    }
  
    close (TEMPSQL);
  
    # NOTE: you may need to add all login users to all databases that will contain ArcSDE data
    # 	If you do not do this, you can get strange problems caused by low-level errors when ArcSDE Server tries to access other
    # 	ArcSDE databases intermittently for no apparent reason, even if you are not trying to access those databases explicitly.
  
  
    # Run the script finally:
    system "osql -U " . $SQLSERVERADMINNAME . " -P " . $SQLSERVERADMINPASSWORD . " < temp.sql";
  }
}


# Start of main loop
# ===========================================================================================================

foreach $RDBMS ("ORACLE", "SQLSERVER")
{

  # Skip RDBMS's user didn't select:
  if ($INSTALLORACLE ne "Y" and $RDBMS eq "ORACLE") { next; }
  if ($INSTALLSQLSERVER ne "Y" and $RDBMS eq "SQLSERVER") { next; }

  # Setup environment variable for database/instance settings:
  print "\nSetting up variables for " . $RDBMS . " ... ";
  if ($RDBMS eq "ORACLE")
  {
    $ENV{'SDEHOME'}=$ORACLESDEHOME;
    $SDEPASSWORD=$ORACLESDEPASSWORD;
    $SDESERVER=$ORACLESDESERVER;
    $DB_INST="-i " . $ORACLESDEPORT;
    $DB_INST_SINGLEDB=$DB_INST;
    $DB_INST_MULTIDB=$DB_INST;
    $PORT_SINGLEDB=$ORACLESDEPORT;
    $PORT_MULTIDB=$ORACLESDEPORT;
    $SUPPORTSUSERMANAGEDSTRINGIDS=$ORACLESUPPORTSUSERMANAGEDSTRINGIDS;
  }
  elsif ($RDBMS eq "SQLSERVER")
  {
    $ENV{'SDEHOME'}=$SQLSERVERSDEHOME;
    $SDEPASSWORD=$SQLSERVERSDEPASSWORD;
    $SDESERVER=$SQLSERVERSDESERVER;
    $DB_INST="-D sde -i " . $SQLSERVERSDEPORT_MULTIDB;
    $DB_INST_SINGLEDB="-D testsingledb -i " . $SQLSERVERSDEPORT_SINGLEDB;
    $DB_INST_MULTIDB="-D testmultidb -i " . $SQLSERVERSDEPORT_MULTIDB;
    $PORT_SINGLEDB=$SQLSERVERSDEPORT_SINGLEDB;
    $PORT_MULTIDB=$SQLSERVERSDEPORT_MULTIDB;
    $SUPPORTSUSERMANAGEDSTRINGIDS=$SQLSERVERSUPPORTSUSERMANAGEDSTRINGIDS;
  }


  # Create dummy tables, On both Oracle and SQL Server;
  # (NOTE: this allows each user to show up as an FDO Schema; a user without registered tables will not show up as an ArcSDE schema)
  # ===========================================================================================================

  print "\n" . $ACTION . " dummy tables and layers for " . $RDBMS . " ... \n";
  foreach $username (@usernames)
  {
    if ($INSTALLACTION ne "U") {
      system "sdetable -o create -t dummy -d \"mystring string(10)\" -u " . $username . " -p test " . $DB_INST;
    } else {
      system "echo y | sdetable -o delete -t dummy -u " . $username . " -p test " . $DB_INST;
    }


    # Create one dummy layer, On both Oracle and SQL Server;
    # (NOTE: this allows SE_registration_get_info_list() to work properly; there is an ArcSDE defect where this function
    # will return 0 entries even if you have tables registered):
    # ===========================================================================================================

    if ($INSTALLACTION ne "U") {
      system "sdelayer -o add -l dummy,shape -e npsla+3 -u " . $username . " -p test " . $DB_INST;
    }
  }


  # Create TESTA and TESTB
  # ===========================================================================================================

  print "\n" . $ACTION . " tables TESTA and TESTB for " . $RDBMS . " ... \n";

  # On both Oracle and SQL Server:

  if ($INSTALLACTION ne "U") {
    system "sdetable -o create -t TESTB -d \"MYSHORTINT smallint(4), MYINT integer(10), MYFLOAT float(6,2), MYFLOAT2 float(6,2), "
       . "MYDOUBLE double(38,8), MYSTRING string(512), MYBLOB blob, MYDATE date\" -u " . $username_metadcov . " -p test -k WKB_GEOMETRY " . $DB_INST_SINGLEDB;
    system "echo y | sdetable -o alter_reg -t TESTB -c OBJECTID -C SDE -u " . $username_metadcov . " -p test -k WKB_GEOMETRY " . $DB_INST_SINGLEDB;
    system "sdelayer -o add -l TESTB,SHAPE -e nplsa+ -x -10000,-10000,100000 -g 100 -u " . $username_metadcov . " -p test -k WKB_GEOMETRY " . $DB_INST_SINGLEDB;
  } else {
    system "echo y | sdetable -o delete -t TESTB -u " . $username_metadcov . " -p test " . $DB_INST_SINGLEDB;
  }

  if ($INSTALLACTION ne "U") {
    system "sdetable -o create -t TESTA -d \"MYSTRING string(50)\" -u " . $username_metadcov . " -p test " . $DB_INST;
    system "echo y | sdetable -o alter_reg -t TESTA -c OBJECTID -C SDE -u " . $username_metadcov . " -p test " . $DB_INST;
    system "sdelayer -o add -l TESTA,SHAPE -e nplsa+ -x -10000,-10000,100000 -g 100 -u " . $username_metadcov . " -p test " . $DB_INST;
  } else {
    system "echo y | sdetable -o delete -t TESTA -u " . $username_metadcov . " -p test " . $DB_INST;
  }

  # On SQL Server only: create a table with same name/owner but different database,
  # and enable versionning on these 3 tables to avoid problems with SE_stream_delete_from_table() that happen only on SQL Server:

  if ($RDBMS eq "SQLSERVER")
  {
    print "\n" . $ACTION . " second table TESTA for " . $RDBMS . " ... \n";
    if ($INSTALLACTION ne "U") {
      system "sdetable -o create -t TESTA -d \"MYSTRING string(50)\" -u " . $username_metadcov . " -p test " . $DB_INST_MULTIDB;
      system "echo y | sdetable -o alter_reg -t TESTA -c OBJECTID -C SDE -u " . $username_metadcov . " -p test " . $DB_INST_MULTIDB;
      system "sdelayer -o add -l TESTA,SHAPE -e nplsa+ -x -10000,-10000,100000 -g 100 -u " . $username_metadcov . " -p test " . $DB_INST_MULTIDB;
      system "echo y | sdetable -o alter_reg -V MULTI -t TESTB -u " . $username_metadcov . " -p test " . $DB_INST_SINGLEDB;
      system "echo y | sdetable -o alter_reg -V MULTI -t TESTA  -u " . $username_metadcov . " -p test " . $DB_INST;
      system "echo y | sdetable -o alter_reg -V MULTI -t TESTA  -u " . $username_metadcov . " -p test " . $DB_INST_MULTIDB;
    } else {
      system "echo y | sdetable -o delete -t TESTA -u " . $username_metadcov . " -p test " . $DB_INST_MULTIDB;
    }

  }


  # Create other tables from SHP files
  # ===========================================================================================================

  # On both Oracle and SQL Server:

  print "\n" . $ACTION . " tables TREES, SOILS and PARCELS for " . $RDBMS . " ... \n";

  if ($INSTALLACTION ne "U") {
    system "shp2sde -o create -l TREES,SHAPE -f " . $SHP_ROOT . "\\Metadcov\\Trees.shp -e p -a all -C OBJECTID,USER,0 -u " . $username_metadcov . " -p test " . $DB_INST;
    system "echo y | sdetable -o alter_reg -t TREES -V MULTI -c OBJECTID -C SDE -u " . $username_metadcov . " -p test " . $DB_INST;
    system "shp2sde -o create -l SOILS,SHAPE -f " . $SHP_ROOT . "\\Australia\\Soils.shp -e a -a all -C FID,USER,0 -u " . $username_australia . " -p test " . $DB_INST;
    system "echo y | sdetable -o alter_reg -t SOILS -c FID -C SDE -u " . $username_australia . " -p test " . $DB_INST;
    system "shp2sde -o create -l PARCELS,SHAPE -f " . $SHP_ROOT . "\\Australia\\PARCELS.shp -e a -a all -C FID,USER,0 -u " . $username_australia . " -p test " . $DB_INST;
    system "echo y | sdetable -o alter_reg -t PARCELS -c FID -C SDE -u " . $username_australia . " -p test " . $DB_INST;
  } else {
    system "echo y | sdetable -o delete -t TREES -u " . $username_metadcov . " -p test " . $DB_INST;
    system "echo y | sdetable -o delete -t SOILS -u " . $username_australia . " -p test " . $DB_INST;
    system "echo y | sdetable -o delete -t PARCELS -u " . $username_australia . " -p test " . $DB_INST;
  }

  # On both Oracle and SQL Server:

  print "\n" . $ACTION . " tables ROADS, COUNTY and DEFECT64335202 for " . $RDBMS . " ... \n";

  if ($INSTALLACTION ne "U") {
    system "shp2sde -o create -l ROADS,GEOMETRY -f " . $SHP_ROOT . "\\Australia\\Roads.shp -e sl -a all -C OBJECTID,USER,0 -u " . $username_australia . " -p test " . $DB_INST;
    system "echo y | sdetable -o alter_reg -t ROADS -c OBJECTID -C SDE -V MULTI -u " . $username_australia . " -p test " . $DB_INST;
    system "shp2sde -o create -l COUNTY,SHAPE -f " . $SHP_ROOT . "\\Australia\\COUNTY.shp -e npsla+3M -g 10000 -u " . $username_australia  . " -p test " . $DB_INST;
    system "shp2sde -o create -l DEFECT64335202,SHAPE -f " . $SHP_ROOT . "\\Australia\\DEFECT64335202.shp -e npsla+ -g 10 -a all -C OBJECTID,USER,0 -u " . $username_australia . " -p test " . $DB_INST;
    system "echo y | sdetable -o alter_reg -t DEFECT64335202 -c OBJECTID -C SDE -u " . $username_australia  . " -p test " . $DB_INST;
  } else {
    system "echo y | sdetable -o delete -t ROADS -u " . $username_australia . " -p test " . $DB_INST;
    system "echo y | sdetable -o delete -t COUNTY -u " . $username_australia  . " -p test "  $DB_INST;
    system "echo y | sdetable -o delete -t DEFECT64335202 -u " . $username_australia . " -p test " . $DB_INST;
  }


  # Run unit tests to initialize remaining test tables
  # ===========================================================================================================

  print "\n" . $ACTION . " the remaining ArcSDE test tables via the ArcSDE Provider's UnitTest.exe ... \n";

  if (-e $ARCSDEUNITTESTDIR . "\\UnitTest.exe")
  {
    if ($INSTALLACTION ne "U") {
      my $command = $ARCSDEUNITTESTDIR . "\\UnitTest.exe initschema=true server=" . $SDESERVER . " port_multi=" . $PORT_MULTIDB . " port_single=" . $PORT_SINGLEDB . " sdepwd=" . $SDEPASSWORD . " dataset=" . $DATASETNAME . " rdbms=" . $RDBMS . " supportsusermanagedstringids=" . $SUPPORTSUSERMANAGEDSTRINGIDS;
      print "\nExecuting: " . $command;
      system $command;
    } else {
      print "\nTODO: remove all tables created by UnitTest.exe initschema=true"
    }
  }
  else
  {
    print "\nArcSDE Provider UnitTest not found at " . $ARCSDEUNITTESTDIR . "\\UnitTest.exe !";
  }


  # Fix test tables that have issues
  # ===========================================================================================================

  if ($INSTALLACTION ne "U") {
    print "\nUpdating layer TESTCLASSGEOMZM3,MYGEOMETRY to a larger grid size (the default of 2.5 causes problems when inserting some larger polygons) for " . $RDBMS . " ... \n";

    system "sdelayer -o alter -l testclassgeomzm3,mygeometry -g 100 -u " . $username_metadcov . " -p test " . $DB_INST;
  }


  # End of main loop
  # ===========================================================================================================
}


# Remove users (if uninstalling only)
# ===========================================================================================================

if ($INSTALLACTION eq "U") {
  if ($INSTALLORACLE eq "Y")
  {
    print "\n" . $ACTION . " users on Oracle ...";
  
    if (-e "temp.sql") { unlink("temp.sql"); }
  
    open (TEMPSQL, ">temp.sql");
    foreach $username (@usernames)
    {
      print TEMPSQL "\ndrop user " . $username . ";";
    }
    close (TEMPSQL);
  
    # Run the script:
    my $sysdba="";
    if ($ORACLEADMINNAME eq "sys") { $sysdba=" as sysdba "; }
    system "sqlplus $ORACLEADMINNAME/$ORACLEADMINPASSWORD " . $sysdba . " < temp.sql";
  }
  
  if ($INSTALLSQLSERVER eq "Y")
  {
    print "\n" . $ACTION . " users on SQL Server ...";
  
    if (-e "temp.sql") { unlink("temp.sql"); }
  
    open (TEMPSQL, ">temp.sql");
    foreach $username (@usernames)
    {
      foreach $db ('master', 'sde', 'testsingledb', 'testmultidb')
      {
        # Grant permissions to all users (including "sde" user) on all databases (including "sde" database)
        # according to Config and Tuning Guide for SQL Server (see sections "Logins And Users: Rules For The Single/Multiple Spatial Database Model")
        # (in SQL Server Enterprise Manager, right-click on the database and select Properties > Permissions tab)
  
        print TEMPSQL "\nuse " . $db . ";";
        print TEMPSQL "\nEXEC sp_revokedbaccess '" . $username . "';";
      }
    }
    foreach $username (@usernames)
    {
      print TEMPSQL "\nEXEC sp_droplogin '" . $username . "';";
    }
  
    close (TEMPSQL);
  
    # NOTE: you may need to add all login users to all databases that will contain ArcSDE data
    # 	If you do not do this, you can get strange problems caused by low-level errors when ArcSDE Server tries to access other
    # 	ArcSDE databases intermittently for no apparent reason, even if you are not trying to access those databases explicitly.
  
  
    # Run the script finally:
    system "osql -U " . $SQLSERVERADMINNAME . " -P " . $SQLSERVERADMINPASSWORD . " < temp.sql";
  }
}



# Final output
# ===========================================================================================================

print "\n";
if ($INSTALLACTION ne "U") {
  print "\nDONE!  You should now be able to run the ArcSDE Provider unit tests as dataset=" . $DATASETNAME;
}else {
  print "\nDONE!  The users and data associated to dataset=" . $DATASETNAME . " have been removed.";
}
print "\n";

}  # end of function SubMain()

