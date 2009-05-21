#include <iostream>
using namespace std;
#include <string.h>

#if defined(_MSC_VER) 
#if (_MSC_VER >= 1400)
#pragma warning (disable:4996)
#endif
#endif

class user_defined_string{
public:

  user_defined_string(void)
  {
    buf[0]=0;
  }

  user_defined_string(const char* s)
  {
    strcpy(buf,s);
  }

  user_defined_string(const user_defined_string& s)
  {
    strcpy(buf,s.buf);
  }

  ~user_defined_string(){}

  user_defined_string& operator=(const char* s)
  {
    strcpy(buf,s);
    return *this;
  }

  user_defined_string& operator+=(const user_defined_string& s)
  {
    strcat(buf,s.buf);
    return *this;
  }


  void assign(int size,char filler)
  {
    for(int i=0;i<size;++i)
      buf[i]=filler;
    buf[size]=0;
  }

  void assign(char* temp_buf,int len)
  {
    for(int i=0;i<len;++i)
      buf[i]=temp_buf[i];
    buf[len]=0;
  }

  char& operator[](int ndx)
  {
    return buf[ndx];
  }

  int length(void) const 
  {
    return static_cast<int>(strlen(buf));
  }

  user_defined_string& operator=(const user_defined_string& s)
  {
    strcpy(buf,s.buf);
    return *this;
  }

  const char* c_str(void) const
  {
    return buf;
  }

protected:

   char buf[70000];

};

ostream& operator<<(ostream& s,const user_defined_string& str)
{
  s<<str.c_str();
  return s;
}

// #define OTL_ORA8I // Compile OTL 4.0/OCI8
// #define OTL_ORA8I // Compile OTL 4.0/OCI8i
#define OTL_ORA9I // Compile OTL 4.0/OCI9i
#define OTL_USER_DEFINED_STRING_CLASS_ON
#define USER_DEFINED_STRING_CLASS user_defined_string
#include <otlv4.h> // include the OTL 4.0 header file

otl_connect db; // connect object

void insert()
// insert rows into table
{

 user_defined_string f2; 
 db.set_max_long_size(70000); // set maximum long string size for connect object

 otl_stream o(1, // buffer size has to be set to 1 for operations with LOBs
              "insert into test_tab values(:f1<int>,empty_clob()) "
              "returning f2 into :f2<clob>",
                 // SQL statement
              db // connect object
             );

 for(int i=1;i<=20;++i){
  f2.assign(50001,' '); 
  for(int j=0;j<50000;++j)
   f2[j]='*';
  f2[50000]='?';
  f2[50001]=0;
  o<<i<<f2;
 }

}

void select()
{ 
 user_defined_string f2;
 db.set_max_long_size(70000); // set maximum long string size for connect object

 otl_stream i(1, // buffer size needs to be set to 1 for operations with LOBs
              "select * from test_tab where f1>=:f11<int> and f1<=:f12<int>*2",
                 // SELECT statement
              db // connect object
             ); 
   // create select stream
 
 float f1;

 i<<8<<8; // assigning :f11 = 8, :f12 = 8
   // SELECT automatically executes when all input variables are
   // assigned. First portion of output rows is fetched to the buffer

 while(!i.eof()){ // while not end-of-data
  i>>f1>>f2;
  cout<<"f1="<<f1<<", f2="<<f2[0]<<f2[f2.length()-1]<<", len="
      <<f2.length()<<endl;
 }

 i<<4<<4; // assigning :f11 = 4, :f12 = 4
   // SELECT automatically executes when all input variables are
   // assigned. First portion of output rows is fetched to the buffer

 while(!i.eof()){ // while not end-of-data
  i>>f1>>f2;
  cout<<"f1="<<f1<<", f2="<<f2[0]<<f2[f2.length()-1]<<", len="
      <<f2.length()<<endl;
 }

}

int main()
{
 otl_connect::otl_initialize(); // initialize OCI environment
 try{

  db.rlogon("scott/tiger"); // connect to Oracle

  otl_cursor::direct_exec
   (
    db,
    "drop table test_tab",

    otl_exception::disabled // disable OTL exceptions
   ); // drop table

  otl_cursor::direct_exec
   (
    db,
    "create table test_tab(f1 number, f2 clob)"
    );  // create table

  insert(); // insert records into table
  select(); // select records from table

 }

 catch(otl_exception& p){ // intercept OTL exceptions
  cerr<<p.msg<<endl; // print out error message
  cerr<<p.stm_text<<endl; // print out SQL that caused the error
  cerr<<p.var_info<<endl; // print out the variable that caused the error
 }

 db.logoff(); // disconnect from Oracle

 return 0;

}
