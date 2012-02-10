#include "StdAfx.h"
#include "ut_FilterProcessor.h"


CPPUNIT_TEST_SUITE_REGISTRATION(ut_FilterProcessor);

ut_FilterProcessor::ut_FilterProcessor(void)
{
}

ut_FilterProcessor::~ut_FilterProcessor(void)
{
}


void ut_FilterProcessor::ProcessSpatialCondition()
{
  FdoPtr<FdoIConnection> conn = c_KgInfoUtil::OpenUnitTestConnection();
  
  // create test table with different data types
  FdoPtr<FdoISQLCommand> sqlcomm = (FdoISQLCommand*)conn->CreateCommand( FdoCommandType_SQLCommand );
  
try
{  
  sqlcomm->SetSQLStatement(L"CREATE TABLE ut_ProcessSpatialCondition( fid NUMBER(10,0)\
                          ,geom SDO_GEOMETRY )"
                       );
  sqlcomm->ExecuteNonQuery();
  
  sqlcomm->SetSQLStatement(L"INSERT INTO user_SDO_GEOM_METADATA  VALUES ('ut_ProcessSpatialCondition', 'GEOM'\
                ,SDO_DIM_ARRAY(SDO_DIM_ELEMENT('X', -1000000, 1000000, 0.005), SDO_DIM_ELEMENT('Y', -1000000, 1000000, 0.005))\
                ,NULL)");

                       
  sqlcomm->ExecuteNonQuery();
  
  sqlcomm->SetSQLStatement(L"CREATE INDEX ut_ProcessSpatialCondition_sid ON ut_ProcessSpatialCondition (\"GEOM\") INDEXTYPE IS \"MDSYS\".\"SPATIAL_INDEX\" PARAMETERS ('sdo_indx_dims=2')");
  sqlcomm->ExecuteNonQuery();
}
catch(FdoException* ) // exception because table already exist - skeep it
{

}

try
{
   
  sqlcomm->SetSQLStatement(L"INSERT INTO ut_ProcessSpatialCondition VALUES(1,\
                        SDO_GEOMETRY(2003, NULL, NULL,SDO_ELEM_INFO_ARRAY(1,1003,1),SDO_ORDINATE_ARRAY(1,1, 10,1, 10,10, 1,10, 1,1))\
                        ");
  sqlcomm->ExecuteNonQuery();  
  



  sqlcomm->SetSQLStatement(L"DROP TABLE ut_ProcessSpatialCondition");
  sqlcomm->ExecuteNonQuery();  
  
  conn->Close();
}  
catch(FdoException* ex)
{
  FdoStringP str = ex->GetExceptionMessage();
  ex->Release();
  CPPUNIT_FAIL( (const char*)str );
}  
}
