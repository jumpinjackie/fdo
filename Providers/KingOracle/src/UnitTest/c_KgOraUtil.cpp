#include "StdAfx.h"
#include "c_KgOraUtil.h"

c_KgOraUtil::c_KgOraUtil(void)
{
}

c_KgOraUtil::~c_KgOraUtil(void)
{
}

FdoIConnection* c_KgOraUtil::OpentConnection(wchar_t* ConnString)
{
try
{
  FdoPtr<IConnectionManager> manager = FdoFeatureAccessManager::GetConnectionManager ();
  FdoPtr<FdoIConnection> conn = manager->CreateConnection (L"King.Oracle.0.1.1");
  //conn->SetConnectionString(L"UserName=ose;Password=ose;ServiceName=temp");
  //conn->SetConnectionString(L"Username=test;Password=test;Service=//192.168.0.23/xe;OracleSchema=UNITTEST");
  conn->SetConnectionString(ConnString);
  
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

FdoIConnection* c_KgOraUtil::OpenUnitTestConnection_10_2()
{
  //return c_KgOraUtil::OpentConnection(L"Username=unittest;Password=unittest;Service=//temp10gr2/ora10gr2;OracleSchema=UNITTEST");    
  return c_KgOraUtil::OpentConnection(L"Username=unittest;Password=unittest;Service=//test10gxe/xe;OracleSchema=UNITTEST");    
}

FdoIConnection* c_KgOraUtil::OpenUnitTestConn_10_2_SDE()
{
  //return c_KgOraUtil::OpentConnection(L"Username=unittest;Password=unittest;Service=//temp10gr2/ora10gr2;OracleSchema=UNITTEST");    
  return c_KgOraUtil::OpentConnection(L"Username=sde;Password=sde;Service=//localhost/xe;SDE Schema=SDE;OracleSchema=UNISDETRAIN");    
}


FdoIConnection* c_KgOraUtil::OpenNanaimoConnection_10_2()
{
  return c_KgOraUtil::OpentConnection(L"Username=nanaimo;Password=nanaimo;Service=//temp10gr2/ora10gr2;OracleSchema=NANAIMO");    
}


FdoIConnection* c_KgOraUtil::OpenPerfomanceTestConnection()
{
  return c_KgOraUtil::OpentConnection(L"Username=unittest;Password=unittest;Service=//temp10gr2/ora10gr2;OracleSchema=UNITTEST");    
}

FdoIConnection* c_KgOraUtil::OpenUnitTestConnection_9_2()
{
try
{
  FdoPtr<IConnectionManager> manager = FdoFeatureAccessManager::GetConnectionManager ();
  FdoPtr<FdoIConnection> conn = manager->CreateConnection (L"King.Oracle.0.1.1");
  //conn->SetConnectionString(L"UserName=ose;Password=ose;ServiceName=temp");
  conn->SetConnectionString(L"Username=ose;Password=ose;Service=ora92;OracleSchema=ose;FdoViewsTable=KingFdoClass");
  
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

void c_KgOraUtil::RecreateTable( FdoIConnection* Connection, const wchar_t*TableName )
{
 
  // create test table with different data types
  FdoPtr<FdoISQLCommand> sqlcomm = (FdoISQLCommand*)Connection->CreateCommand( FdoCommandType_SQLCommand );

  FdoStringP tname = TableName;
try
{  
  FdoStringP sql = FdoStringP::Format(L"DROP TABLE %s",tname);
  
  
  sqlcomm->SetSQLStatement(sql);
  sqlcomm->ExecuteNonQuery();
}  
catch(FdoException* ex)
{
  ex->Release();
} 
try
{  
  // drop sequnce
  FdoStringP sql = FdoStringP::Format(L"DROP SEQUENCE %s_FDOSEQ",tname);
  sqlcomm->SetSQLStatement(sql);
  sqlcomm->ExecuteNonQuery();
}  
catch(FdoException* ex  )
{
  ex->Release();
} 
try
{  
  FdoStringP sql = FdoStringP::Format(L"CREATE TABLE %s( \
                          fid NUMBER(10,0) \
                          ,geom SDO_GEOMETRY,name VARCHAR2(100) NOT NULL )",tname);
  sqlcomm->SetSQLStatement(sql);
  sqlcomm->ExecuteNonQuery();
}  
catch(FdoException* ex)
{
  ex->Release();
} 
try
{  
  // create primary key
  FdoStringP sql = FdoStringP::Format(L"alter table %s add constraint %s_pk primary key (fid)",TableName,TableName);
  sqlcomm->SetSQLStatement(sql);
  sqlcomm->ExecuteNonQuery();
}  
catch(FdoException* ex)
{
  ex->Release();
} 
try
{  
  // create sequnce
  FdoStringP sql = FdoStringP::Format(L"CREATE SEQUENCE %s_FDOSEQ",tname);
  sqlcomm->SetSQLStatement(sql);
  sqlcomm->ExecuteNonQuery();
}  
catch(FdoException* ex)
{
  ex->Release();
} 
try
{  
  // insert user_sdo_geom_metadata
  FdoStringP sql = FdoStringP::Format(L"INSERT INTO user_SDO_GEOM_METADATA  VALUES ('%s'\
                      , 'GEOM',SDO_DIM_ARRAY(SDO_DIM_ELEMENT('X', -1000000, 1000000, 0.005),SDO_DIM_ELEMENT('Y', -1000000, 1000000, 0.005))\
                      ,NULL)",tname);
  sqlcomm->SetSQLStatement(sql);

  sqlcomm->ExecuteNonQuery();
}
catch(FdoException* ex)
{
  ex->Release();
} 
try
{  
//-- Create index
    FdoStringP sql = FdoStringP::Format(L"INSERT INTO user_SDO_GEOM_METADATA  VALUES ('%s'\
                      , 'GEOM',SDO_DIM_ARRAY(SDO_DIM_ELEMENT('X', -1000000, 1000000, 0.005),SDO_DIM_ELEMENT('Y', -1000000, 1000000, 0.005))\
                      ,NULL)",tname);
  sqlcomm->SetSQLStatement(sql);
  
  sqlcomm->ExecuteNonQuery();
}
catch(FdoException* ex)
{
  ex->Release();
}  
 
}
