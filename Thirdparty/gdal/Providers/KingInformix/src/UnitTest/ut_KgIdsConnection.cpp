#include "StdAfx.h"
#include "c_KgInfoUtil.h"
#include "ut_KgIdsConnection.h"


CPPUNIT_TEST_SUITE_REGISTRATION(ut_KgIdsConnection);



ut_KgIdsConnection::ut_KgIdsConnection(void)
{
}

ut_KgIdsConnection::~ut_KgIdsConnection(void)
{
}


void ut_KgIdsConnection::OpenConnection()
{
try
{
  FdoPtr<FdoIConnection> conn = c_KgInfoUtil::OpenUnitTestConnection();
  conn->Close();

}
catch(FdoException* ex)
{
  FdoStringP str = ex->GetExceptionMessage();
  ex->Release();
  CPPUNIT_FAIL( (const char*)str );
}  
  
}

void ut_KgIdsConnection::PerfomanceTest()
{
  
}//end of ut_KgOraConnection::PerfomanceTest