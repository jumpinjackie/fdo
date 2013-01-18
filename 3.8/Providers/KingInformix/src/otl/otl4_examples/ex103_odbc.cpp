#include <iostream>
using namespace std;
#include <stdio.h>

#define OTL_ODBC // Compile OTL 4/ODBC
#define OTL_STL // enable STL / ANSI C++ compliance.
#define OTL_UNCAUGHT_EXCEPTION_ON // enable safe exception handling / error 
                                  // recovery.
#include <otlv4.h> // include the OTL 4 header file

otl_connect db; // connect object

void insert()
// insert rows into table
{ 
 otl_stream o(10, // buffer size
              "insert into test_tab values(:f1<int>,:f2<char[31]>)", 
                 // SQL statement
              db // connect object
             );
 o.set_commit(0); // turning off the stream's autocommit flag
 char tmp[32];

 for(int i=1;i<=100;++i){
  sprintf(tmp,"Name%d",i);
  o<<i;
  if(i==17)
   throw "Making a little trouble in the middle of the INSERT...";
  o<<tmp;
 }
 o.flush(); // flushing the stream's buffer
 db.commit(); // committing the changes to the database
}

int main()
{
 otl_connect::otl_initialize(); // initialize ODBC environment
 try{

  db.rlogon("scott/tiger@mssql"); // connect to MS SQL Server

  otl_cursor::direct_exec
   (
    db,
    "drop table test_tab",
    otl_exception::disabled // disable OTL exceptions
   ); // drop table

  otl_cursor::direct_exec
   (
    db,
    "create table test_tab(f1 int, f2 varchar(30))"
    );  // create table

  insert(); // insert records into table

 }

 catch(otl_exception& p){ // intercept OTL exceptions
  cerr<<p.msg<<endl; // print out error message
  cerr<<p.stm_text<<endl; // print out SQL that caused the error
  cerr<<p.sqlstate<<endl; // print out SQLSTATE info.
  cerr<<p.var_info<<endl; // print out the variable that caused the error
  db.rollback(); // roll back transaction 
 }

 catch(const char* msg){
  cerr<<msg<<endl; // print out the message
  db.rollback(); // roll back transaction
 }

 db.logoff(); // disconnect from MS SQL Server

 return 0;

}
