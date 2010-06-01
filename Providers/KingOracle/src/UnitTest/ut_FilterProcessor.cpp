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
  FdoPtr<FdoIConnection> conn = c_KgOraUtil::OpenUnitTestConnection_10_2();
  
  // create test table with different data types
  FdoPtr<FdoISQLCommand> sqlcomm = (FdoISQLCommand*)conn->CreateCommand( FdoCommandType_SQLCommand );
  
try
{  
  sqlcomm->SetSQLStatement(L"DROP TABLE ut_ProcessSpatialCondition");
  sqlcomm->ExecuteNonQuery();
}  
catch(FdoException* )
{
}  
try
{  
  sqlcomm->SetSQLStatement(L"CREATE TABLE ut_ProcessSpatialCondition( fid NUMBER(10,0)\
                          ,geom SDO_GEOMETRY )"
                       );
  sqlcomm->ExecuteNonQuery();
}  
catch(FdoException* ex)
{
  ex->Release();  
}  
try
{ 
  sqlcomm->SetSQLStatement(L"INSERT INTO user_SDO_GEOM_METADATA  VALUES ('ut_ProcessSpatialCondition', 'GEOM'\
                ,SDO_DIM_ARRAY(SDO_DIM_ELEMENT('X', -1000000, 1000000, 0.005), SDO_DIM_ELEMENT('Y', -1000000, 1000000, 0.005)),NULL)");

                       
  sqlcomm->ExecuteNonQuery();
}  
catch(FdoException* ex)
{
  ex->Release();  
}
try
{
 
  sqlcomm->SetSQLStatement(L"CREATE INDEX ut_ProcessSpatialCondition_sid ON ut_ProcessSpatialCondition (\"GEOM\") INDEXTYPE IS \"MDSYS\".\"SPATIAL_INDEX\" PARAMETERS ('sdo_indx_dims=2')");
  sqlcomm->ExecuteNonQuery();
}
catch(FdoException* ex ) 
{
  ex->Release();  
}

try
{
   
  sqlcomm->SetSQLStatement(L"INSERT INTO ut_ProcessSpatialCondition VALUES(1,\
                        SDO_GEOMETRY(2003, NULL, NULL,SDO_ELEM_INFO_ARRAY(1,1003,1),SDO_ORDINATE_ARRAY(1,1, 10,1, 10,10, 1,10, 1,1)))\
                        ");
  sqlcomm->ExecuteNonQuery();  
  
  sqlcomm->SetSQLStatement(L"INSERT INTO ut_ProcessSpatialCondition VALUES(2,\
                            SDO_GEOMETRY(2003, NULL, NULL,SDO_ELEM_INFO_ARRAY(1,1003,1),SDO_ORDINATE_ARRAY(100,100, 110,100, 110,110, 100,110, 100,100)))\
                            ");
  sqlcomm->ExecuteNonQuery();


  
}  
catch(FdoException* ex)
{
  FdoStringP str = ex->GetExceptionMessage();
  ex->Release();
  CPPUNIT_FAIL( (const char*)str );
}  

try
{
FdoPtr<FdoISelect> selectcomm = (FdoISelect*)conn->CreateCommand( FdoCommandType_Select );
selectcomm->SetFeatureClassName(L"UNITTEST~UT_PROCESSSPATIALCONDITION~GEOM");   

selectcomm->SetFilter(L"GEOM INTERSECTS GEOMFROMTEXT ('POLYGON ((5 5, 15 5, 15 15, 5 15, 5 5))')");
FdoPtr<FdoIFeatureReader> reader = selectcomm->Execute();
int count = 0;
while(reader->ReadNext())
{
  count++;
}
if( count != 1) CPPUNIT_FAIL( "Should be only 1 feature" );
reader->Close();

selectcomm->SetFilter(L"GEOM INTERSECTS GEOMFROMTEXT ('POLYGON ((55 55, 60 55, 60 60, 55 60, 55 55))')");
FdoPtr<FdoIdentifierCollection> props = selectcomm->GetPropertyNames();
FdoPtr<FdoIdentifier> cid = FdoIdentifier::Create(L"FID");
props->Add(cid);
reader = selectcomm->Execute();
count = 0;
while(reader->ReadNext())
{
  count++;
}
if( count ) CPPUNIT_FAIL( "Should be zero feature" );
reader->Close();

selectcomm->SetFilter(L"GEOM INTERSECTS GEOMFROMTEXT ('POLYGON ((5 5, 105 5, 105 105, 5 105, 5 5))')");
reader = selectcomm->Execute();
count = 0;
while(reader->ReadNext())
{
  count++;
}
if( count != 2) CPPUNIT_FAIL( "Should be only 2 features" );
reader->Close();

conn->Close();


}  
catch(FdoException* ex)
{
  FdoStringP str = ex->GetExceptionMessage();
  ex->Release();
  CPPUNIT_FAIL( (const char*)str );
}  

}

void ut_FilterProcessor::ProcessSpatialConditionSRID()
{
  FdoPtr<FdoIConnection> conn = c_KgOraUtil::OpenUnitTestConnection_10_2();

  // create test table with different data types
  FdoPtr<FdoISQLCommand> sqlcomm = (FdoISQLCommand*)conn->CreateCommand( FdoCommandType_SQLCommand );

  try
  {  
    sqlcomm->SetSQLStatement(L"DROP TABLE ut_SpatialSRID");
    sqlcomm->ExecuteNonQuery();
  }  
  catch(FdoException* )
  {
  }  
  try
  {  
    sqlcomm->SetSQLStatement(L"CREATE TABLE ut_SpatialSRID( fid NUMBER(10,0)\
                              ,geom SDO_GEOMETRY )"
                              );
    sqlcomm->ExecuteNonQuery();
  }  
  catch(FdoException* ex)
  {
    ex->Release();  
  }  
  try
  { 
    sqlcomm->SetSQLStatement(L"INSERT INTO user_SDO_GEOM_METADATA  VALUES ('ut_SpatialSRID', 'GEOM'\
                              ,SDO_DIM_ARRAY(SDO_DIM_ELEMENT('X', -1000000, 1000000, 0.005), SDO_DIM_ELEMENT('Y', -1000000, 1000000, 0.005)),2000)");


    sqlcomm->ExecuteNonQuery();
  }  
  catch(FdoException* ex)
  {
    ex->Release();  
  }
  try
  {

    sqlcomm->SetSQLStatement(L"CREATE INDEX ut_SpatialSRID_sid ON ut_SpatialSRID (\"GEOM\") INDEXTYPE IS \"MDSYS\".\"SPATIAL_INDEX\" PARAMETERS ('sdo_indx_dims=2')");
    sqlcomm->ExecuteNonQuery();
  }
  catch(FdoException* ex ) 
  {
    ex->Release();  
  }

  try
  {

    sqlcomm->SetSQLStatement(L"INSERT INTO ut_SpatialSRID VALUES(1,\
                              SDO_GEOMETRY(2003, 2000, NULL,SDO_ELEM_INFO_ARRAY(1,1003,1),SDO_ORDINATE_ARRAY(1,1, 10,1, 10,10, 1,10, 1,1)))\
                              ");
    sqlcomm->ExecuteNonQuery();  

    sqlcomm->SetSQLStatement(L"INSERT INTO ut_SpatialSRID VALUES(2,\
                              SDO_GEOMETRY(2003, 2000, NULL,SDO_ELEM_INFO_ARRAY(1,1003,1),SDO_ORDINATE_ARRAY(100,100, 110,100, 110,110, 100,110, 100,100)))\
                              ");
    sqlcomm->ExecuteNonQuery();



  }  
  catch(FdoException* ex)
  {
    FdoStringP str = ex->GetExceptionMessage();
    ex->Release();
    CPPUNIT_FAIL( (const char*)str );
  }  

  try
  {
    FdoPtr<FdoISelect> selectcomm = (FdoISelect*)conn->CreateCommand( FdoCommandType_Select );
    selectcomm->SetFeatureClassName(L"UNITTEST~UT_SPATIALSRID~GEOM");   

    selectcomm->SetFilter(L"GEOM INTERSECTS GEOMFROMTEXT ('POLYGON ((5 5, 15 5, 15 15, 5 15, 5 5))')");
    FdoPtr<FdoIFeatureReader> reader = selectcomm->Execute();
    int count = 0;
    while(reader->ReadNext())
    {
      count++;
    }
    if( count != 1) CPPUNIT_FAIL( "Should be only 1 feature" );
    reader->Close();

    selectcomm->SetFilter(L"GEOM INTERSECTS GEOMFROMTEXT ('POLYGON ((55 55, 60 55, 60 60, 55 60, 55 55))')");
    FdoPtr<FdoIdentifierCollection> props = selectcomm->GetPropertyNames();
    FdoPtr<FdoIdentifier> cid = FdoIdentifier::Create(L"FID");
    props->Add(cid);
    reader = selectcomm->Execute();
    count = 0;
    while(reader->ReadNext())
    {
      count++;
    }
    if( count ) CPPUNIT_FAIL( "Should be zero feature" );
    reader->Close();

    selectcomm->SetFilter(L"GEOM INTERSECTS GEOMFROMTEXT ('POLYGON ((5 5, 105 5, 105 105, 5 105, 5 5))')");
    reader = selectcomm->Execute();
    count = 0;
    while(reader->ReadNext())
    {
      count++;
    }
    if( count != 2) CPPUNIT_FAIL( "Should be only 2 features" );
    reader->Close();

    conn->Close();

  }  
  catch(FdoException* ex)
  {
    FdoStringP str = ex->GetExceptionMessage();
    ex->Release();
    CPPUNIT_FAIL( (const char*)str );
  }  

}