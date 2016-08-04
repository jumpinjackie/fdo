#include "StdAfx.h"


#include <math.h>
#include "ut_InsertUpdateDelete.h"

#pragma warning(disable : 4996)


CPPUNIT_TEST_SUITE_REGISTRATION(ut_InsertUpdateDelete);

ut_InsertUpdateDelete::ut_InsertUpdateDelete(void)
{
}

ut_InsertUpdateDelete::~ut_InsertUpdateDelete(void)
{
}

void ut_InsertUpdateDelete::PrepareTables(FdoIConnection* Connection)
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



//
// Test geometry transform from sdo_geometry to FDF fromat and back to sdo_geometry
//
void ut_InsertUpdateDelete::InsertUpdateDelete()
{

  FdoPtr<FdoIConnection> conn = c_KgInfoUtil::OpenUnitTestConnection();
  
  PrepareTables(conn);

  FdoPtr<FdoISQLCommand> sqlcomm = (FdoISQLCommand*)conn->CreateCommand( FdoCommandType_SQLCommand );
  
try
{
  FdoStringP src_classname = L"UNITTEST~UT_IUD~GEOM";
 
// Execute insert  
{  
  FdoPtr<FdoIInsert> comm_insert = (FdoIInsert*)conn->CreateCommand( FdoCommandType_Insert );    
  comm_insert->SetFeatureClassName( src_classname );
  
  FdoPtr<FdoPropertyValueCollection> propcol = comm_insert->GetPropertyValues();
  
  // Fid property
  //FdoPtr<FdoDataValue> fid_val=  FdoDataValue::Create(1);
  FdoPtr<FdoDataValue> fid_val = FdoDataValue::Create(FdoDataType_Int32);
  
  FdoPtr<FdoPropertyValue> propval_fid = FdoPropertyValue::Create(L"FID",fid_val);
  propcol->Add(propval_fid);
  
  // Geometry property
  FdoPtr<FdoFgfGeometryFactory> fgf = FdoFgfGeometryFactory::GetInstance();
  
  double ordinates[10] = { 1,1 ,10,1 ,10,10 ,1,10 ,1,1 }; 
  
  FdoPtr<FdoILinearRing> ring = fgf->CreateLinearRing(0,10,ordinates);
  FdoPtr<FdoIPolygon> polygon = fgf->CreatePolygon(ring,NULL);
  
  FdoPtr<FdoByteArray> barray = fgf->GetFgf(polygon);
  
  FdoPtr<FdoGeometryValue> geom_val=  FdoGeometryValue::Create(barray);
  
  FdoPtr<FdoPropertyValue> propval_geom = FdoPropertyValue::Create(L"GEOM",geom_val);
  
  // Name string property
  FdoPtr<FdoStringValue> name_val=  (FdoStringValue*)FdoDataValue::Create(L"Name1");
  FdoPtr<FdoPropertyValue> propval_name = FdoPropertyValue::Create(L"NAME",name_val);
  
  
  propcol->Add(propval_geom);
  propcol->Add(propval_name);
  
  FdoPtr<FdoIFeatureReader> insreader = comm_insert->Execute();
}  
  
// execute update  
{  
  FdoPtr<FdoIUpdate> comm_update = (FdoIUpdate*)conn->CreateCommand( FdoCommandType_Update );    
  comm_update->SetFeatureClassName( src_classname );
  
  FdoPtr<FdoPropertyValueCollection> propcol = comm_update->GetPropertyValues();
  
  // Fid property
  FdoPtr<FdoDataValue> fid_val=  FdoDataValue::Create(2);
  FdoPtr<FdoPropertyValue> propval_fid = FdoPropertyValue::Create(L"FID",fid_val);
  
  // Geometry property
  FdoPtr<FdoFgfGeometryFactory> fgf = FdoFgfGeometryFactory::GetInstance();
  
  double ordinates[10] = { 2,2 ,20,2 ,20,20 ,2,20 ,2,2 }; 
  
  FdoPtr<FdoILinearRing> ring = fgf->CreateLinearRing(0,10,ordinates);
  FdoPtr<FdoIPolygon> polygon = fgf->CreatePolygon(ring,NULL);
  
  FdoPtr<FdoByteArray> barray = fgf->GetFgf(polygon);
  
  FdoPtr<FdoGeometryValue> geom_val=  FdoGeometryValue::Create(barray);
  
  FdoPtr<FdoPropertyValue> propval_geom = FdoPropertyValue::Create(L"GEOM",geom_val);
  
  // Name string property
  FdoPtr<FdoStringValue> name_val=  (FdoStringValue*)FdoDataValue::Create(L"NameUpdate");
  FdoPtr<FdoPropertyValue> propval_name = FdoPropertyValue::Create(L"NAME",name_val);
  
  propcol->Add(propval_fid);
  propcol->Add(propval_geom);
  propcol->Add(propval_name);
  
  // set filter for update 
  FdoPtr<FdoIdentifier> ident =  FdoIdentifier::Create(L"FID");
  FdoPtr<FdoDataValue> val2 =  FdoDataValue::Create(1);
  FdoPtr<FdoFilter> filter = FdoComparisonCondition::Create(ident, FdoComparisonOperations_EqualTo, val2);
  comm_update->SetFilter(filter);
  
  FdoInt32 updated = comm_update->Execute();
  if( updated != 1 )
  {
    CPPUNIT_FAIL( "ut_InsertUpdateDelete::InsertUpdateDelete: Update should returned 1 for num of changed rows" );
  }
}  
// execute delete 
{
  FdoPtr<FdoIDelete> comm_delete = (FdoIDelete*)conn->CreateCommand( FdoCommandType_Delete );    
  comm_delete->SetFeatureClassName( src_classname );
  
  // set filter for delete
  FdoPtr<FdoIdentifier> ident =  FdoIdentifier::Create(L"FID");
  FdoPtr<FdoDataValue> val2 =  FdoDataValue::Create(2);
  FdoPtr<FdoFilter> filter = FdoComparisonCondition::Create(ident, FdoComparisonOperations_EqualTo, val2);
  comm_delete->SetFilter(filter);
  
  FdoInt32 deleted = comm_delete->Execute();
  if( deleted != 1 )
  {
    CPPUNIT_FAIL( "ut_InsertUpdateDelete::InsertUpdateDelete: Delete should returned 1 for num of changed rows" );
  }
}  

  sqlcomm->SetSQLStatement(L"DROP TABLE UT_IUD");
  sqlcomm->ExecuteNonQuery();
  
  conn->Close();
}  

catch(FdoException* ex)
{
  FdoStringP str = ex->GetExceptionMessage();
  ex->Release();
  CPPUNIT_FAIL( (const char*)str );
}

}//end of ut_InsertUpdateDelete::InsertUpdateDelete()


//
// Test geometry transform from sdo_geometry to FDF fromat and back to sdo_geometry
//
void ut_InsertUpdateDelete::InsertUnicode()
{

  FdoPtr<FdoIConnection> conn = c_KgInfoUtil::OpenUnitTestConnection();
  
  PrepareTables(conn);

  FdoPtr<FdoISQLCommand> sqlcomm = (FdoISQLCommand*)conn->CreateCommand( FdoCommandType_SQLCommand );
  
try
{
  FdoStringP src_classname = L"UNITTEST~UT_IUD~GEOM";
 
// Execute insert  
{  
  FdoPtr<FdoIInsert> comm_insert = (FdoIInsert*)conn->CreateCommand( FdoCommandType_Insert );    
  comm_insert->SetFeatureClassName( src_classname );
  
  FdoPtr<FdoPropertyValueCollection> propcol = comm_insert->GetPropertyValues();
  
  // Fid property
  //FdoPtr<FdoDataValue> fid_val=  FdoDataValue::Create(1);
  FdoPtr<FdoDataValue> fid_val = FdoDataValue::Create(FdoDataType_Int32);
  
  FdoPtr<FdoPropertyValue> propval_fid = FdoPropertyValue::Create(L"FID",fid_val);
  propcol->Add(propval_fid);
  
  // Geometry property
  FdoPtr<FdoFgfGeometryFactory> fgf = FdoFgfGeometryFactory::GetInstance();
  
  double ordinates[10] = { 1,1 ,10,1 ,10,10 ,1,10 ,1,1 }; 
  
  FdoPtr<FdoILinearRing> ring = fgf->CreateLinearRing(0,10,ordinates);
  FdoPtr<FdoIPolygon> polygon = fgf->CreatePolygon(ring,NULL);
  
  FdoPtr<FdoByteArray> barray = fgf->GetFgf(polygon);
  
  FdoPtr<FdoGeometryValue> geom_val=  FdoGeometryValue::Create(barray);
  
  FdoPtr<FdoPropertyValue> propval_geom = FdoPropertyValue::Create(L"GEOM",geom_val);
  
  // Name string property
  FdoPtr<FdoStringValue> name_val=  (FdoStringValue*)FdoDataValue::Create(L"Name1 šðžèæ ŠÐŽÈÆ");
  FdoPtr<FdoPropertyValue> propval_name = FdoPropertyValue::Create(L"NAME",name_val);
  
  
  propcol->Add(propval_geom);
  propcol->Add(propval_name);
  
  FdoPtr<FdoIFeatureReader> insreader = comm_insert->Execute();
}  
  
// execute query  
{  
  FdoPtr<FdoISelect> comm_select = (FdoISelect*)conn->CreateCommand( FdoCommandType_Select );    
  comm_select->SetFeatureClassName( src_classname );
  
  FdoPtr<FdoIFeatureReader> reader = comm_select->Execute();
  while( reader->ReadNext() )
  {
    FdoStringP name = reader->GetString(L"NAME");
    
    if( name.ICompare(L"Name1 šðžèæ ŠÐŽÈÆ") != 0 )
    {
      CPPUNIT_FAIL( "ut_InsertUpdateDelete::InsertUnicode: Not valid NAME column data" );
    }
  }      
}  
  //sqlcomm->SetSQLStatement(L"DROP TABLE UT_IUD");
  //sqlcomm->ExecuteNonQuery();
  
  conn->Close();
}  

catch(FdoException* ex)
{
  FdoStringP str = ex->GetExceptionMessage();
  ex->Release();
  CPPUNIT_FAIL( (const char*)str );
}

}//end of ut_InsertUpdateDelete::InsertUnicode
