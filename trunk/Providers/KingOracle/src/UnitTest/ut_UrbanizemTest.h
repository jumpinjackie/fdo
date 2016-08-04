#pragma once

class ut_UrbanizemTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( ut_UrbanizemTest );
   CPPUNIT_TEST( InsertTest );
  CPPUNIT_TEST_SUITE_END();
  
public:
  ut_UrbanizemTest(void);
public:
  ~ut_UrbanizemTest(void);
  
    struct ConnectInfo {
    int   connectionId;
    FdoIConnection  *mConn;
};

struct FunctionInfo {
#ifdef _WIN32
    LPTHREAD_START_ROUTINE Function1;
    LPTHREAD_START_ROUTINE Function2;
#else
     void* (*Function1)(void *);
     void* (*Function2)(void *);
#endif
};
 //FdoPtr<FdoIConnection> conn; 
  
  static void Insert(FdoIConnection* conn);
  static void Update(FdoIConnection* conn);
  void Delete(FdoIConnection* conn);
  void Test(FunctionInfo *funInfo);
  virtual FdoIConnection * GetConnection();
  static void DescribeSchema(FdoIConnection* conn2);
  static DWORD WINAPI StartInsert(LPVOID lpParameter);
	static DWORD WINAPI StartUpdate(LPVOID lpParameter);
	static DWORD WINAPI StartDescibeSchema(LPVOID lpParameter);
	
  void InsertTest();
  /*
  void InsertUnicode();*/
  
protected:
  //void PrepareTables(FdoIConnection* Connection);  
  


};
