#include "StdAfx.h"


#include <math.h>
#include "ut_SelectOrderBy.h"

#pragma warning(disable : 4996)


CPPUNIT_TEST_SUITE_REGISTRATION(ut_SelectOrderBy);

ut_SelectOrderBy::ut_SelectOrderBy(void)
{
}

ut_SelectOrderBy::~ut_SelectOrderBy(void)
{
}



void ut_SelectOrderBy::InsertRow(FdoIConnection* Conn,FdoString* ClassName,double* Ordinates,long OrdinatesCount,long Id,FdoString* Text)
{
  // Execute insert  
{  
  FdoPtr<FdoIInsert> comm_insert = (FdoIInsert*)Conn->CreateCommand( FdoCommandType_Insert );    
  comm_insert->SetFeatureClassName( ClassName );
  
  FdoPtr<FdoPropertyValueCollection> propcol = comm_insert->GetPropertyValues();
  
  // Fid property
  //FdoPtr<FdoDataValue> fid_val=  FdoDataValue::Create(1);
  FdoPtr<FdoDataValue> fid_val = FdoDataValue::Create(Id);
  
  FdoPtr<FdoPropertyValue> propval_fid = FdoPropertyValue::Create(L"FID",fid_val);
  propcol->Add(propval_fid);
  
  // Geometry property
  FdoPtr<FdoFgfGeometryFactory> fgf = FdoFgfGeometryFactory::GetInstance();
  
  FdoPtr<FdoILinearRing> ring = fgf->CreateLinearRing(0,OrdinatesCount,Ordinates);
  FdoPtr<FdoIPolygon> polygon = fgf->CreatePolygon(ring,NULL);
  
  FdoPtr<FdoByteArray> barray = fgf->GetFgf(polygon);
  
  FdoPtr<FdoGeometryValue> geom_val=  FdoGeometryValue::Create(barray);
  
  FdoPtr<FdoPropertyValue> propval_geom = FdoPropertyValue::Create(L"GEOM",geom_val);
  
  // Name string property
  FdoPtr<FdoStringValue> name_val=  (FdoStringValue*)FdoDataValue::Create(Text);
  FdoPtr<FdoPropertyValue> propval_name = FdoPropertyValue::Create(L"NAME",name_val);
  
  
  propcol->Add(propval_geom);
  propcol->Add(propval_name);
  
  FdoPtr<FdoIFeatureReader> insreader = comm_insert->Execute();
}  
}//end of ut_SelectOrderBy::InsertRow

//
// Test geometry transform from sdo_geometry to FDF fromat and back to sdo_geometry
//
void ut_SelectOrderBy::SelectOrderBy()
{

  FdoPtr<FdoIConnection> conn = c_KgOraUtil::OpenUnitTestConnection_10_2();
  
  // create test table with different data types
  FdoPtr<FdoISQLCommand> sqlcomm = (FdoISQLCommand*)conn->CreateCommand( FdoCommandType_SQLCommand );

try
{  
  sqlcomm->SetSQLStatement(L"DROP TABLE UT_IUD");
  sqlcomm->ExecuteNonQuery();
}  
catch(FdoException* ex)
{
  ex->Release();
} 
try
{  
  // create sequnce
  sqlcomm->SetSQLStatement(L"DROP SEQUENCE UT_IUD_FDOSEQ");
  sqlcomm->ExecuteNonQuery();
}  
catch(FdoException* ex)
{
  ex->Release();
} 
try
{  
  sqlcomm->SetSQLStatement(L"CREATE TABLE UT_IUD( \
                          fid NUMBER(10,0) \
                          ,geom SDO_GEOMETRY,name VARCHAR2(100) NOT NULL )"
                       );
  sqlcomm->ExecuteNonQuery();
}  
catch(FdoException* ex)
{
  ex->Release();
} 
try
{  
  // create primary key
  sqlcomm->SetSQLStatement(L"alter table ut_IUD add constraint ut_IUD_pk primary key ( fid)");
  sqlcomm->ExecuteNonQuery();
}  
catch(FdoException* ex)
{
  ex->Release();
} 
try
{  
  // create sequnce
  sqlcomm->SetSQLStatement(L"CREATE SEQUENCE UT_IUD_FDOSEQ");
  sqlcomm->ExecuteNonQuery();
}  
catch(FdoException* ex)
{
  ex->Release();
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
catch(FdoException* ex)
{
  ex->Release();
} 
try
{  
//-- Create index
  sqlcomm->SetSQLStatement(L"CREATE INDEX UT_IUD_sid ON UT_IUD (\"GEOM\")\
                            INDEXTYPE IS \"MDSYS\".\"SPATIAL_INDEX\" PARAMETERS ('sdo_indx_dims=2')"
                            );
  
  sqlcomm->ExecuteNonQuery();
}
catch(FdoException* ex)
{
  ex->Release();
}  
 


try
{
  FdoStringP src_classname = L"UNITTEST~UT_IUD~GEOM";
 
// Execute insert  
{  
  
  double ordinates1[10] = { 1,1 ,10,1 ,10,10 ,1,10 ,1,1 }; 
  InsertRow(conn,src_classname,ordinates1,10,1,L"Name1");
  
  double ordinates2[10] = { 1,1 ,10,1 ,10,10 ,1,10 ,1,1 }; 
  InsertRow(conn,src_classname,ordinates1,10,2,L"Name2");
  
  double ordinates3[10] = { 1,1 ,10,1 ,10,10 ,1,10 ,1,1 }; 
  InsertRow(conn,src_classname,ordinates1,10,3,L"Name3");
  
}  
  
// execute select order by  
{  
  FdoPtr<FdoISelect> comm_select = (FdoISelect*)conn->CreateCommand( FdoCommandType_Select );    
  comm_select->SetFeatureClassName( src_classname );
  
  comm_select->SetOrderingOption(FdoOrderingOption_Descending);
  FdoPtr<FdoIdentifierCollection> order_ident_col = comm_select->GetOrdering();
  
  FdoPtr<FdoIdentifier> order_ident = FdoIdentifier::Create(L"FID");
  order_ident_col->Add(order_ident);
  
  comm_select->Execute();
  FdoPtr<FdoIFeatureReader> freader = comm_select->Execute();
  
  CPPUNIT_ASSERT_MESSAGE( "FdoISelect:Execute returns NULL reader" , freader );
  
  unsigned long previd;
  bool isfirst=true;
  while( freader->ReadNext() )
  {
    if( !isfirst && (freader->GetInt32(L"FID") > previd) )
    {
      CPPUNIT_FAIL( "Error SelectOrderBy: FID is not in Descending order!" );
    }
    previd = freader->GetInt32(L"FID");
    isfirst=false;
  }

}  

  conn->Close();
}  
catch(FdoException* ex)
{
  FdoStringP str = ex->GetExceptionMessage();
  ex->Release();
  CPPUNIT_FAIL( (const char*)str );
}

}//end of ut_SelectOrderBy::SelectOrderBy



//
// Test geometry transform from sdo_geometry to FDF fromat and back to sdo_geometry
//
void ut_SelectOrderBy::SelectMultiPoint()
{

  FdoPtr<FdoIConnection> conn = c_KgOraUtil::OpenUnitTestConnection_10_2();
  
  // create test table with different data types
  FdoPtr<FdoISQLCommand> sqlcomm = (FdoISQLCommand*)conn->CreateCommand( FdoCommandType_SQLCommand );


try
// execute select order by  
{  
  FdoPtr<FdoISelect> comm_select = (FdoISelect*)conn->CreateCommand( FdoCommandType_Select );    
  comm_select->SetFeatureClassName( L"TEST~POINTS~GEOMETRY" );
    
  comm_select->Execute();
  FdoPtr<FdoIFeatureReader> freader = comm_select->Execute();
  unsigned long previd;
  bool isfirst=true;
  while( freader->ReadNext() )
  {
    int id = freader->GetInt32(L"FEATID");
    freader->GetGeometry(L"GEOMETRY");
    isfirst=false;
  }
 freader->Close();

  conn->Close();
}  
catch(FdoException* ex)
{
  FdoStringP str = ex->GetExceptionMessage();
  ex->Release();
  CPPUNIT_FAIL( (const char*)str );
}

}//end of ut_SelectOrderBy::SelectOrderBy


