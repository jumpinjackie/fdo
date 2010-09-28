#include <iostream>
using namespace std;
#include <stdio.h>

#if !defined(_WINDOWS_) && !defined(_WIN64)
#define OTL_ODBC
#else 
#define OTL_ODBC_POSTGRESQL // required with PG ODBC on Windows
#endif

// #define OTL_ODBC_UNIX // uncomment this line if UnixODBC is used
#include <otlv4.h> // include the OTL 4.0 header file


otl_connect db; // connect object

void insert(void)
// insert rows into table
{ 
// for PostgreSQL's temporal datatypes, DB2 compatible bind variable 
// placeholder's datatypes should be used. PostgreSQL's ODBC driver behaves the same 
// way as DB2 CLI, as far as temporal datatypes are concerned.
 otl_stream o(1, // PostgreSQL 8.1 and higher, the buffer can be > 1
              "insert into test_tab "
              "values(:f1<int>,:f2<db2date>, "
              "       :f3<db2time>,:f4<timestamp>)", 
                   // INSERT statement
              db // connect object
             );

 otl_datetime tm;

 for(int i=1;i<=10;++i){
  tm.year=1998;
  tm.month=10;
  tm.day=19;
  tm.hour=23;
  tm.minute=12;
  tm.second=12;
  o<<i<<tm<<tm<<tm;
 }
}

void select(void)
{ 
 otl_stream i(5, // stream buffer size can be > great 1 on SELECT.
              "select "
              "  f1, "
              "  f2, "
              "  f3, "
              "  f4 :#4<timestamp> " 
        // f4's type is retunred as CHAR by default by the PG ODBC on
        // Linux, explicit type override is needed
              "from test_tab",
                 // SELECT statement
              db // connect object
             ); 
 
 int f1;
 otl_datetime f2,f3,f4;

 while(!i.eof()){ // while not end-of-data
  i>>f1>>f2>>f3>>f4;
  cout<<"f1="<<f1;
  cout<<", f2="<<f2.month<<"/"<<f2.day<<"/"
      <<f2.year<<" "<<f2.hour<<":"<<f2.minute<<":"
      <<f2.second;
  cout<<", f3="<<f3.month<<"/"<<f3.day<<"/"
      <<f3.year<<" "<<f3.hour<<":"<<f3.minute<<":"
      <<f3.second;
  cout<<", f4="<<f4.month<<"/"<<f4.day<<"/"
      <<f4.year<<" "<<f4.hour<<":"<<f4.minute<<":"
      <<f4.second;
  cout<<endl;
 }
 
}

int main()
{
 otl_connect::otl_initialize(); // initialize ODBC environment
 try{

  db.rlogon("UID=scott;PWD=tiger;DSN=postgresql"); // connect to ODBC
//  db.rlogon("scott/tiger@postgresql"); // connect to ODBC, alternative format
                                    // of connect string

  otl_cursor::direct_exec
   (
    db,
    "drop table test_tab",
    otl_exception::disabled // disable OTL exceptions
   ); // drop table
  db.commit();
  otl_cursor::direct_exec
   (
    db,
    "create table test_tab(f1 int, f2 date, f3 time, f4 timestamp)"
    );  // create table
  db.commit();
  insert(); // insert records into table
  select(); // select records from table

 }

 catch(otl_exception& p){ // intercept OTL exceptions
  cerr<<p.msg<<endl; // print out error message
  cerr<<p.stm_text<<endl; // print out SQL that caused the error
  cerr<<p.sqlstate<<endl; // print out SQLSTATE message
  cerr<<p.var_info<<endl; // print out the variable that caused the error
 }

 db.logoff(); // disconnect from ODBC

 return 0;

}
