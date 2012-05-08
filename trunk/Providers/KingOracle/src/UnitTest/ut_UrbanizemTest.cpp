
#include "StdAfx.h"


#include <math.h>
#include "ut_UrbanizemTest.h"

#pragma warning(disable : 4996)

#define   NUMBER_OF_THREADS     10
static    bool   use_threads = true;

CPPUNIT_TEST_SUITE_REGISTRATION(ut_UrbanizemTest);

ut_UrbanizemTest::ut_UrbanizemTest(void)
{
   //conn = c_KgOraUtil::OpenUrbanizemUnitTestConnection_10_2();
}

ut_UrbanizemTest::~ut_UrbanizemTest(void)
{
 
  //conn->Close();
}

/*
void ut_UrbanizemTest::PrepareTables(FdoIConnection* Connection)
{
 
  // create test table with different data types
  FdoPtr<FdoISQLCommand> sqlcomm = (FdoISQLCommand*)Connection->CreateCommand( FdoCommandType_SQLCommand );

try
{  
  sqlcomm->SetSQLStatement(L"DROP TABLE UT_IUD");
  sqlcomm->ExecuteNonQuery();
}  
catch(FdoException* )
{
} 
try
{  
  // create sequnce
  sqlcomm->SetSQLStatement(L"DROP SEQUENCE UT_IUD_FDOSEQ");
  sqlcomm->ExecuteNonQuery();
}  
catch(FdoException* )
{
} 
try
{  
  sqlcomm->SetSQLStatement(L"CREATE TABLE UT_IUD( \
                          fid NUMBER(10,0) \
                          ,geom SDO_GEOMETRY,name VARCHAR2(100) NOT NULL )"
                       );
  sqlcomm->ExecuteNonQuery();
}  
catch(FdoException* )
{
} 
try
{  
  // create primary key
  sqlcomm->SetSQLStatement(L"alter table ut_IUD add constraint ut_IUD_pk primary key ( fid)");
  sqlcomm->ExecuteNonQuery();
}  
catch(FdoException* )
{
} 
try
{  
  // create sequnce
  sqlcomm->SetSQLStatement(L"CREATE SEQUENCE UT_IUD_FDOSEQ");
  sqlcomm->ExecuteNonQuery();
}  
catch(FdoException* )
{
} 
try
{  
  // insert user_sdo_geom_metadata
  sqlcomm->SetSQLStatement(L"INSERT INTO user_SDO_GEOM_METADATA  VALUES ('UT_IUD'\
                      , 'GEOM',SDO_DIM_ARRAY(SDO_DIM_ELEMENT('X', -1000000, 1000000, 0.005),SDO_DIM_ELEMENT('Y', -1000000, 1000000, 0.005))\
                      ,NULL)"
                        );

  sqlcomm->ExecuteNonQuery();
}
catch(FdoException* )
{
} 
try
{  
//-- Create index
  sqlcomm->SetSQLStatement(L"CREATE INDEX UT_IUD_sid ON UT_IUD (\"GEOM\")\
                            INDEXTYPE IS \"MDSYS\".\"SPATIAL_INDEX\" PARAMETERS ('sdo_indx_dims=2')"
                            );
  
  sqlcomm->ExecuteNonQuery();
}
catch(FdoException* )
{
}  
 
}
*/


//
// Test geometry transform from sdo_geometry to FDF fromat and back to sdo_geometry
//

void ut_UrbanizemTest::Insert(FdoIConnection* conn)
{
  

  FdoPtr<FdoISQLCommand> sqlcomm = (FdoISQLCommand*)conn->CreateCommand( FdoCommandType_SQLCommand );
  
  FdoStringP src_classname = L"URBANIST~ZAHTJEVA~GEOMETRY";
 try{
// Execute insert  
 {
  FdoPtr<FdoIInsert> comm_insert = (FdoIInsert*)conn->CreateCommand( FdoCommandType_Insert );    
  comm_insert->SetFeatureClassName( src_classname );
  
  FdoPtr<FdoPropertyValueCollection> propcol = comm_insert->GetPropertyValues();
  
  // Fid property
  //FdoPtr<FdoDataValue> fid_val=  FdoDataValue::Create(1);
  FdoPtr<FdoDataValue> fid_val = FdoDataValue::Create(FdoDataType_Int32);
  
  FdoPtr<FdoPropertyValue> propval_fid = FdoPropertyValue::Create(L"ID",fid_val);
  propcol->Add(propval_fid);
  
  // Geometry property
  FdoPtr<FdoFgfGeometryFactory> fgf = FdoFgfGeometryFactory::GetInstance();
  
  double ordinates[10] = { 1,1 ,10,1 ,10,10 ,1,10 ,1,1 }; 
  
  FdoPtr<FdoILinearRing> ring = fgf->CreateLinearRing(0,10,ordinates);
  FdoPtr<FdoIPolygon> polygon = fgf->CreatePolygon(ring,NULL);
  
  FdoPtr<FdoByteArray> barray = fgf->GetFgf(polygon);
  
  FdoPtr<FdoGeometryValue> geom_val=  FdoGeometryValue::Create(barray);
  
  FdoPtr<FdoPropertyValue> propval_geom = FdoPropertyValue::Create(L"GEOMETRY",geom_val);
  
  // Name string property
  FdoPtr<FdoStringValue> name_val=  (FdoStringValue*)FdoDataValue::Create(L"IME");
  FdoPtr<FdoPropertyValue> propval_name = FdoPropertyValue::Create(L"IME",name_val);
  
  // Name string property
  FdoPtr<FdoStringValue> name_val_broj=  (FdoStringValue*)FdoDataValue::Create(L"BROJ");
  FdoPtr<FdoPropertyValue> propval_broj = FdoPropertyValue::Create(L"BROJ",name_val_broj);
  
  
  propcol->Add(propval_geom);
  propcol->Add(propval_name);
  propcol->Add(propval_broj);
  
  FdoPtr<FdoIFeatureReader> insreader = comm_insert->Execute();
}  
  
}  

catch(FdoException* ex)
{
  FdoStringP str = ex->GetExceptionMessage();
  ex->Release();
  printf("FDO error: %ls\n", ex->GetExceptionMessage());
}

}//end of ut_UrbanizemTest::Insert()





void ut_UrbanizemTest::Update(FdoIConnection* conn)
{
  

  //FdoPtr<FdoISQLCommand> sqlcomm = (FdoISQLCommand*)conn->CreateCommand( FdoCommandType_SQLCommand );
  
  FdoStringP src_classname = L"URBANIST~ZAHTJEVA~GEOMETRY";
 try{
  
// execute update  
{  
  FdoPtr<FdoIUpdate> comm_update = (FdoIUpdate*)conn->CreateCommand( FdoCommandType_Update );    
  comm_update->SetFeatureClassName( src_classname );
  
  FdoPtr<FdoPropertyValueCollection> propcol = comm_update->GetPropertyValues();
  
  // Fid property
  FdoPtr<FdoDataValue> fid_val=  FdoDataValue::Create(2);
  FdoPtr<FdoPropertyValue> propval_fid = FdoPropertyValue::Create(L"ID",fid_val);
  
  // Geometry property
  FdoPtr<FdoFgfGeometryFactory> fgf = FdoFgfGeometryFactory::GetInstance();
  
  double ordinates[10] = { 2,2 ,20,2 ,20,20 ,2,20 ,2,2 }; 
  
  FdoPtr<FdoILinearRing> ring = fgf->CreateLinearRing(0,10,ordinates);
  FdoPtr<FdoIPolygon> polygon = fgf->CreatePolygon(ring,NULL);
  
  FdoPtr<FdoByteArray> barray = fgf->GetFgf(polygon);
  
  FdoPtr<FdoGeometryValue> geom_val=  FdoGeometryValue::Create(barray);
  
  FdoPtr<FdoPropertyValue> propval_geom = FdoPropertyValue::Create(L"GEOMETRY",geom_val);
  
  // Name string property
  FdoPtr<FdoStringValue> name_val=  (FdoStringValue*)FdoDataValue::Create(L"imeporpavljeno");
  FdoPtr<FdoPropertyValue> propval_name = FdoPropertyValue::Create(L"IME",name_val);
  
  propcol->Add(propval_fid);
  propcol->Add(propval_geom);
  propcol->Add(propval_name);
  
  // set filter for update 
  FdoPtr<FdoIdentifier> ident =  FdoIdentifier::Create(L"ID");
  FdoPtr<FdoDataValue> val2 =  FdoDataValue::Create(300);
  FdoPtr<FdoFilter> filter = FdoComparisonCondition::Create(ident, FdoComparisonOperations_EqualTo, val2);
  comm_update->SetFilter(filter);
  
  FdoInt32 updated = comm_update->Execute();
  if( updated < 1 )
  {
  //  CPPUNIT_FAIL( "ut_UrbanizemTest::Update: Update should returned 1 for num of changed rows" );
  }
} 

}  

catch(FdoException* ex)
{
  FdoStringP str = ex->GetExceptionMessage();
  ex->Release();
  printf("FDO error: %ls\n", ex->GetExceptionMessage());
}

}//end of ut_UrbanizemTest::Insert()



void ut_UrbanizemTest::Delete(FdoIConnection* conn)
{
 FdoStringP src_classname = L"URBANIST~ZAHTJEVA~GEOMETRY";
 try{
 
// execute delete 
{
  FdoPtr<FdoIDelete> comm_delete = (FdoIDelete*)conn->CreateCommand( FdoCommandType_Delete );    
  comm_delete->SetFeatureClassName( src_classname );
  
  // set filter for delete
  FdoPtr<FdoIdentifier> ident =  FdoIdentifier::Create(L"IME");
  //FdoPtr<FdoDataValue> val2 =  FdoDataValue::Create(295);
  FdoPtr<FdoDataValue> val2 =  FdoDataValue::Create(L"IME");
  FdoPtr<FdoFilter> filter = FdoComparisonCondition::Create(ident, FdoComparisonOperations_Like, val2);
  comm_delete->SetFilter(filter);
  
  FdoInt32 deleted = comm_delete->Execute();

}  

/*
  sqlcomm->ExecuteNonQuery();
  */
 // conn->Close();

}  

catch(FdoException* ex)
{
  FdoStringP str = ex->GetExceptionMessage();
  ex->Release();
  printf("FDO error: %ls\n", ex->GetExceptionMessage());
}

}//end of ut_UrbanizemTest::Delete()

FdoIConnection * ut_UrbanizemTest::GetConnection()
{
	return c_KgOraUtil::OpenUrbanizemUnitTestConnection_10_2();
}

void ut_UrbanizemTest::DescribeSchema(FdoIConnection* conn2)
{

  try{
  
  FdoPtr<FdoIDescribeSchema> comm = (FdoIDescribeSchema*)conn2->CreateCommand(FdoCommandType_DescribeSchema);

  
  FdoPtr<FdoFeatureSchemaCollection> schemas = comm->Execute();
  
  CPPUNIT_ASSERT_MESSAGE( "FdoIDescribeSchema:Execute returns NULL schema collection" , schemas );
    

  long count=schemas->GetCount();
  long countClass=schemas->GetItem(0)->GetClasses()->GetCount();
  

}

catch(FdoException* ex)
{
  FdoStringP str = ex->GetExceptionMessage();
  ex->Release();
  printf("FDO error: %ls\n", ex->GetExceptionMessage());
}
  
}//end of ut_KgOraSchema::PerfomanceTest


DWORD WINAPI ut_UrbanizemTest::StartInsert(LPVOID lpParameter)
{
    ConnectInfo  *cnInfo = (ConnectInfo*)lpParameter;
    
    Insert( cnInfo->mConn );

    return 0;
}

DWORD WINAPI ut_UrbanizemTest::StartDescibeSchema(LPVOID lpParameter)
{
    ConnectInfo  *cnInfo = (ConnectInfo*)lpParameter;
    
    DescribeSchema( cnInfo->mConn );

    return 0;
}

void ut_UrbanizemTest::InsertTest()
{
    FunctionInfo funcInfo;
    
    funcInfo.Function1 = StartDescibeSchema;
    funcInfo.Function2 = StartDescibeSchema;

    Test( &funcInfo );
}


void ut_UrbanizemTest::Test(FunctionInfo *funInfo)
{
  
  HANDLE phThreads[NUMBER_OF_THREADS];
  DWORD dwThreadId = 0;
  
  ConnectInfo   info[NUMBER_OF_THREADS];
  int i;
    try
    {
        for (int i = 0; i < NUMBER_OF_THREADS; i++)
	    {
            info[i].connectionId = i;
            info[i].mConn = GetConnection();
	    } 
    }
    catch (FdoException *ex )
	  {
		  char msg[512];
		  sprintf(msg,"%ls", ex->GetExceptionMessage());
		  CPPUNIT_FAIL(msg);
      return;
	  }
	
      bool  toggle = true;
      for ( i = 0; i < NUMBER_OF_THREADS; i++)
	    {
        phThreads[i] = CreateThread(NULL, 0,(toggle)?funInfo->Function1:funInfo->Function2, &info[i], 0, &dwThreadId);

	    } 

	    WaitForMultipleObjects(NUMBER_OF_THREADS, phThreads, TRUE, INFINITE);


      for (i = 0; i < NUMBER_OF_THREADS; i++){
            if(info[i].mConn){
            info[i].mConn->Release();
            }
		    CloseHandle(phThreads[i]);
      }
	  
  
}