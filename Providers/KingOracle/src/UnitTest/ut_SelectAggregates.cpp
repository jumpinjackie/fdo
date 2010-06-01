#include "StdAfx.h"
#include "ut_SelectAggregates.h"


CPPUNIT_TEST_SUITE_REGISTRATION(ut_SelectAggregates);

ut_SelectAggregates::ut_SelectAggregates(void)
{
}

ut_SelectAggregates::~ut_SelectAggregates(void)
{
}


void ut_SelectAggregates::SelectCount()
{
  FdoPtr<FdoIConnection> conn = c_KgOraUtil::OpenUnitTestConnection_10_2();
  
  // create test table with different data types
  FdoPtr<FdoISelectAggregates> aggcomm = (FdoISelectAggregates*)conn->CreateCommand( FdoCommandType_SelectAggregates );
  
try
{  
 
  aggcomm->SetFeatureClassName(L"UNITTEST~CITYLIMITS~GEOMETRY");   
  FdoPtr<FdoIdentifierCollection> coll_ident = aggcomm->GetPropertyNames();
  
  FdoPtr<FdoExpression> expr = FdoExpression::Parse(FDO_FUNCTION_SPATIALEXTENTS L"(GEOMETRY)");
  FdoPtr<FdoComputedIdentifier> comp_ident = FdoComputedIdentifier::Create(L"geom_extent",expr);
  
  coll_ident->Add(comp_ident);
  
   expr = FdoExpression::Parse(L"COUNT()");
   comp_ident = FdoComputedIdentifier::Create(L"prop_count",expr);
  
  coll_ident->Add(comp_ident);
  
  FdoPtr<FdoIdentifierCollection> coll_groupby = aggcomm->GetGrouping();
  //FdoPtr<FdoIdentifier> ident_bl_use = FdoIdentifier::Create(L"BL_USE");
  //coll_groupby->Add(ident_bl_use);
  
  
  FdoPtr<FdoByteArray> layer_extent;
  FdoInt32 layer_count;
  
  FdoPtr<FdoIDataReader> reader = aggcomm->Execute();
  FdoInt32 count = reader->GetPropertyCount();
  
  while(reader->ReadNext())
  {
    for(FdoInt32 ind=0;ind<count;ind++)
    {
      FdoString* name = reader->GetPropertyName(ind);
      switch(reader->GetPropertyType(name))
      {
        case FdoPropertyType_DataProperty:
          reader->GetDataType(name);
          layer_count = reader->GetInt32(name);
        break;

      
        /// Represents a Geometric Property type.
        case FdoPropertyType_GeometricProperty:
          layer_extent = reader->GetGeometry(name);
        break;
      }
    }
  }
  
  reader->Close();  
 
}
catch(FdoException* ex)
{
  FdoStringP str = ex->GetExceptionMessage();
  ex->Release();
  CPPUNIT_FAIL( (const char*)str );
}  

}



// Execute insert  
void ut_SelectAggregates::SelectDistinct_Prepare_Insert(FdoIConnection* Conn,int Fid,wchar_t*Name,FdoGeometryValue* Geom)
{  
  FdoPtr<FdoIInsert> comm_insert = (FdoIInsert*)Conn->CreateCommand( FdoCommandType_Insert );    
  comm_insert->SetFeatureClassName( L"UNITTEST~UT_DISTINCT~GEOM" );

  FdoPtr<FdoPropertyValueCollection> propcol = comm_insert->GetPropertyValues();

  // Fid property
  //FdoPtr<FdoDataValue> fid_val=  FdoDataValue::Create(1);
  FdoPtr<FdoDataValue> fid_val = FdoDataValue::Create((FdoInt32)Fid);
  
  FdoPtr<FdoPropertyValue> propval_fid = FdoPropertyValue::Create(L"FID",fid_val);
  propcol->Add(propval_fid);

  // Geometry property
  
  FdoPtr<FdoPropertyValue> propval_geom = FdoPropertyValue::Create(L"GEOM",Geom);

  // Name string property
  FdoPtr<FdoStringValue> name_val=  (FdoStringValue*)FdoDataValue::Create(Name);
  FdoPtr<FdoPropertyValue> propval_name = FdoPropertyValue::Create(L"NAME",name_val);


  propcol->Add(propval_geom);
  propcol->Add(propval_name);

  FdoPtr<FdoIFeatureReader> insreader = comm_insert->Execute();
}  

void ut_SelectAggregates::SelectDistinct_Prepare()
{
  FdoPtr<FdoIConnection> conn = c_KgOraUtil::OpenUnitTestConnection_10_2();
  c_KgOraUtil::RecreateTable(conn,L"UT_DISTINCT");
  
  // Geometry property
  FdoPtr<FdoFgfGeometryFactory> fgf = FdoFgfGeometryFactory::GetInstance();
  double ordinates[10] = { 1,1 ,10,1 ,10,10 ,1,10 ,1,1 }; 
  FdoPtr<FdoILinearRing> ring = fgf->CreateLinearRing(0,10,ordinates);
  FdoPtr<FdoIPolygon> polygon = fgf->CreatePolygon(ring,NULL);
  FdoPtr<FdoByteArray> barray = fgf->GetFgf(polygon);
  FdoPtr<FdoGeometryValue> geom_val=  FdoGeometryValue::Create(barray);
  
  SelectDistinct_Prepare_Insert(conn,1,L"Name1",geom_val);
  SelectDistinct_Prepare_Insert(conn,2,L"Name2",geom_val);
  SelectDistinct_Prepare_Insert(conn,3,L"Name2",geom_val);
  SelectDistinct_Prepare_Insert(conn,4,L"Name2",geom_val);
  SelectDistinct_Prepare_Insert(conn,5,L"Name2",geom_val);
  
  conn->Close();
  
}//end of ut_SelectAggregates::SelectDistinct_Prepare

void ut_SelectAggregates::SelectDistinct()
{

try
{
  SelectDistinct_Prepare();
  
  FdoPtr<FdoIConnection> conn = c_KgOraUtil::OpenUnitTestConnection_10_2();
  FdoPtr<FdoISelectAggregates> aggcomm = (FdoISelectAggregates*)conn->CreateCommand( FdoCommandType_SelectAggregates );  
  aggcomm->SetFeatureClassName(L"UNITTEST~UT_DISTINCT~GEOM");   
  FdoPtr<FdoIdentifierCollection> props =  aggcomm->GetPropertyNames();
  FdoPtr<FdoIdentifier> cid = FdoIdentifier::Create(L"NAME");
  props->Add(cid);
  aggcomm->SetDistinct(true);
  
  FdoPtr<FdoIDataReader> reader = aggcomm->Execute();
  FdoInt32 count = reader->GetPropertyCount();
  int reccount=0;
  while(reader->ReadNext())
  {
    reccount++;
  }
  if( reccount!=2 ) CPPUNIT_FAIL("Should be two records in select distinct");
  reader->Close();  
  
  {
    FdoPtr<FdoISelectAggregates> aggcomm = (FdoISelectAggregates*)conn->CreateCommand( FdoCommandType_SelectAggregates );  
    aggcomm->SetFeatureClassName(L"UNITTEST~UT_DISTINCT~GEOM");   
    FdoPtr<FdoIdentifierCollection> props =  aggcomm->GetPropertyNames();
    FdoPtr<FdoIdentifier> cid = FdoIdentifier::Create(L"NAME");
    props->Add(cid);    

    FdoPtr<FdoIDataReader> reader = aggcomm->Execute();
    FdoInt32 count = reader->GetPropertyCount();
    int reccount=0;
    while(reader->ReadNext())
    {
      reccount++;
    }
    if( reccount <= 2 ) CPPUNIT_FAIL("Should be more than two records when no 'select distinct'");
    reader->Close();  
  }
  
  conn->Close();
  
}
catch(FdoException* ex)
{
  FdoStringP str = ex->GetExceptionMessage();
  ex->Release();
  CPPUNIT_FAIL( (const char*)str );
} 
}//end of ut_SelectAggregates::SelectDistinct

