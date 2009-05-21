#include "StdAfx.h"
#include "c_KgInfoUtil.h"


c_KgInfoUtil::c_KgInfoUtil(void)
{
}

c_KgInfoUtil::~c_KgInfoUtil(void)
{
}

FdoIConnection* c_KgInfoUtil::OpenUnitTestConnection()
{
try
{
  FdoPtr<IConnectionManager> manager = FdoFeatureAccessManager::GetConnectionManager ();
  FdoPtr<FdoIConnection> conn = manager->CreateConnection (L"King.Informix");
  //conn->SetConnectionString(L"UserName=ose;Password=ose;ServiceName=temp");
  conn->SetConnectionString(L"Username=informix;Password=informix;Dsn=IDS;KingFdoClass=myfdoclass");
  
  conn->Open();
  
  
  return FDO_SAFE_ADDREF(conn.p);

}  
catch(FdoException* ex)
{
  FdoStringP str = ex->GetExceptionMessage();
  ex->Release();
  CPPUNIT_FAIL( (const char*)str );
}
return NULL;
}

FdoIConnection* c_KgInfoUtil::OpenPerfomanceTestConnection()
{
try
{
  FdoPtr<IConnectionManager> manager = FdoFeatureAccessManager::GetConnectionManager ();
  FdoPtr<FdoIConnection> conn = manager->CreateConnection (L"King.Informix");
  //conn->SetConnectionString(L"UserName=ose;Password=ose;ServiceName=temp");
  conn->SetConnectionString(L"Username=unittest;Password=unittest;Dsn=IDS");
  
  conn->Open();
  
  return FDO_SAFE_ADDREF(conn.p);
}  
catch(FdoException* ex)
{
  FdoStringP str = ex->GetExceptionMessage();
  ex->Release();
  CPPUNIT_FAIL( (const char*)str );
}
return NULL;  
}

