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

  FdoPtr<FdoIConnection> conn = c_KgOraUtil::OpenUnitTestConnection_10_2();
  
  PrepareTables(conn);

  FdoPtr<FdoISQLCommand> sqlcomm = (FdoISQLCommand*)conn->CreateCommand( FdoCommandType_SQLCommand );
  
try
{
  FdoStringP src_classname = L"UNITTEST~UT_IUD~GEOM";
 
// Delete ALL 
{
  FdoPtr<FdoIDelete> comm_delete = (FdoIDelete*)conn->CreateCommand( FdoCommandType_Delete );    
  comm_delete->SetFeatureClassName( src_classname );
  comm_delete->Execute();
} 
 
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
if( false )
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

}//end of ut_InsertUpdateDelete::InsertUpdateDelete()



void ut_InsertUpdateDelete::InsertCurvedPolygon()
{

  FdoPtr<FdoIConnection> conn = c_KgOraUtil::OpenUnitTestConnection_10_2();

  PrepareTables(conn);

  FdoPtr<FdoISQLCommand> sqlcomm = (FdoISQLCommand*)conn->CreateCommand( FdoCommandType_SQLCommand );

  FdoPtr<FdoByteArray> barray_start;
  FdoPtr<FdoGeometryValue> geom_val_start;
  std::wstring geom_string_start;
  
  try
  {
    FdoStringP src_classname = L"UNITTEST~UT_IUD~GEOM";

    // Delete ALL 
    {
      FdoPtr<FdoIDelete> comm_delete = (FdoIDelete*)conn->CreateCommand( FdoCommandType_Delete );    
      comm_delete->SetFeatureClassName( src_classname );
      comm_delete->Execute();
    } 

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

      FdoPtr<FdoIGeometry> polygon = fgf->CreateGeometry(L"CURVEPOLYGON ((757057.98399999994 245592.943 (LINESTRINGSEGMENT(757058.40300000005 245594.383, 757034.647 245610.43299999999),\
        CIRCULARARCSEGMENT (757032.81790019607 245607.87801543999,\
        757030.73199999996 245605.52800000002), LINESTRINGSEGMENT(757018.41400000011 245592.943, 757038.57400000002 245573.158,757057.98399999994 245592.943))))");

      
      barray_start = fgf->GetFgf(polygon);

      geom_val_start =  FdoGeometryValue::Create(barray_start);
      
      geom_string_start = geom_val_start->ToString();

      FdoPtr<FdoPropertyValue> propval_geom = FdoPropertyValue::Create(L"GEOM",geom_val_start);

      // Name string property
      FdoPtr<FdoStringValue> name_val=  (FdoStringValue*)FdoDataValue::Create(L"Name1");
      FdoPtr<FdoPropertyValue> propval_name = FdoPropertyValue::Create(L"NAME",name_val);


      propcol->Add(propval_geom);
      propcol->Add(propval_name);

      FdoPtr<FdoIFeatureReader> insreader = comm_insert->Execute();                 
    }  
    // execute select
    {
      FdoPtr<FdoISelect> comm_select = (FdoISelect*)conn->CreateCommand( FdoCommandType_Select );          
      comm_select->SetFeatureClassName( src_classname );  
      comm_select->SetFilter(L"NAME = 'Name1'");


      FdoPtr<FdoIFeatureReader> freader = comm_select->Execute();
      if( !freader->ReadNext() )
      {
        CPPUNIT_FAIL( "Missing inserted feature!!" );    
      }
      FdoPtr<FdoByteArray> barray = freader->GetGeometry(L"GEOM");
      
      FdoPtr<FdoGeometryValue> geomval = FdoGeometryValue::Create( barray );
      std::wstring geom_string =geomval->ToString();
      
      
      if(geom_string != geom_string_start ) //if(*geomval == *geom_val_start )
      {
        CPPUNIT_FAIL( "Curved geometry is not equal!" );    
      }
    }
  }
  catch(FdoException* ex)
  {
    FdoStringP str = ex->GetExceptionMessage();
    ex->Release();
    CPPUNIT_FAIL( (const char*)str );
  }
}  

//
// Test geometry transform from sdo_geometry to FDF fromat and back to sdo_geometry
//
void ut_InsertUpdateDelete::InsertUnicode()
{

  FdoPtr<FdoIConnection> conn = c_KgOraUtil::OpenUnitTestConnection_10_2();
  
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


//
// Test geometry transform from sdo_geometry to FDF fromat and back to sdo_geometry
//
void ut_InsertUpdateDelete::InsertMultiPoint()
{

  FdoPtr<FdoIConnection> conn = c_KgOraUtil::OpenUnitTestConnection_10_2();
  
  PrepareTables(conn);

  FdoPtr<FdoISQLCommand> sqlcomm = (FdoISQLCommand*)conn->CreateCommand( FdoCommandType_SQLCommand );
  
  
  
try
{
  FdoStringP src_classname = L"TEST~UT_IUD~GEOM";
 
// Execute insert  
{  
  FdoPtr<FdoIInsert> comm_insert = (FdoIInsert*)conn->CreateCommand( FdoCommandType_Insert );    
  comm_insert->SetFeatureClassName( src_classname );
  
  FdoPtr<FdoPropertyValueCollection> propcol = comm_insert->GetPropertyValues();
  
  // Fid property
  FdoPtr<FdoDataValue> fid_val=  FdoDataValue::Create(1);
 // FdoPtr<FdoDataValue> fid_val = FdoDataValue::Create(FdoDataType_Int32);
 
  FdoPtr<FdoPropertyValue> propval_fid = FdoPropertyValue::Create(L"FID",fid_val);
  propcol->Add(propval_fid);
  
   // Name string property
  FdoPtr<FdoStringValue> name_val=  (FdoStringValue*)FdoDataValue::Create(L"Name1");
  FdoPtr<FdoPropertyValue> propval_name = FdoPropertyValue::Create(L"NAME",name_val);
  
  propcol->Add(propval_name);
  
  // Geometry property
  FdoPtr<FdoFgfGeometryFactory> fgf = FdoFgfGeometryFactory::GetInstance();
  
  double ordinates[6] = {7.57596377226628,6.42242229267311, 1.00668318024867, 2.24612559045716, 9.43394215307453, 1.20205141490317 }; 
  
  FdoPtr<FdoIMultiPoint> mpoint = fgf->CreateMultiPoint(0,6,ordinates);
    
  FdoPtr<FdoByteArray> barray = fgf->GetFgf(mpoint);
  
  FdoPtr<FdoGeometryValue> geom_val=  FdoGeometryValue::Create(barray);
  
  FdoPtr<FdoPropertyValue> propval_geom = FdoPropertyValue::Create(L"GEOM",geom_val);


  propcol->Add(propval_geom);
  
  FdoPtr<FdoIFeatureReader> insreader = comm_insert->Execute();
}  
  
  {
    FdoPtr<FdoISelect> comm_select = (FdoISelect*)conn->CreateCommand( FdoCommandType_Select );    
    comm_select->SetFeatureClassName( src_classname);
   
   
    FdoPtr<FdoIFeatureReader> freader = comm_select->Execute();
    
    CPPUNIT_ASSERT_MESSAGE( "FdoISelect:Execute returns NULL reader" , freader );
    
    unsigned long previd;
    bool isfirst=true;
    
    while( freader->ReadNext() )
    {
      freader->GetGeometry(L"GEOM");      
      isfirst=true;
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

}//end of ut_InsertUpdateDelete::InsertUpdateDelete()

void ut_InsertUpdateDelete::Update_BLDG_POLYGON()
{
  FdoPtr<FdoIConnection> conn = c_KgOraUtil::OpenUnitTestConnection_10_2();
  
  
  
  
try
{
  FdoStringP src_classname = L"UNITTEST~BLDG_POLYGON~GEOMETRY";
 

  
// execute update  
{  
  FdoPtr<FdoIUpdate> comm_update = (FdoIUpdate*)conn->CreateCommand( FdoCommandType_Update );    
  comm_update->SetFeatureClassName( src_classname );
  
  FdoPtr<FdoPropertyValueCollection> propcol = comm_update->GetPropertyValues();
  
  // Fid property
  FdoPtr<FdoDataValue> fid_val=  FdoDataValue::Create(8645);
  FdoPtr<FdoPropertyValue> propval_fid = FdoPropertyValue::Create(L"FEATID",fid_val);
  
  FdoPtr<FdoISelect> comm_sel = (FdoISelect*)conn->CreateCommand( FdoCommandType_Select );    
  comm_sel->SetFeatureClassName(src_classname);
  FdoPtr<FdoFilter> filter_sel = FdoFilter::Parse(L"FEATID = 8465");
  comm_sel->SetFilter(filter_sel);
  FdoPtr<FdoIFeatureReader> reader = comm_sel->Execute();
  if(reader->ReadNext())
  {
    FdoPtr<FdoClassDefinition> classdef = reader->GetClassDefinition();
    
    FdoPtr<FdoPropertyDefinitionCollection> coll_propdef = classdef->GetProperties();
    FdoInt32 count_prop = coll_propdef->GetCount(); //reader->GetPropertyCount();
    for(FdoInt32 ind=0;ind<count_prop;ind++)
    {
      FdoPtr<FdoPropertyDefinition> propdef = coll_propdef->GetItem(ind);
      FdoString* name_prop = propdef->GetName(); //reader->GetPropertyName(ind);
      //switch(reader->GetPropertyType(name_prop))
      switch(propdef->GetPropertyType())
      {
        
        case FdoPropertyType_DataProperty:
        {
          FdoDataPropertyDefinition* datapropdef = (FdoDataPropertyDefinition*)propdef.p;
          //switch( reader->GetDataType(name_prop) )
          switch( datapropdef->GetDataType() )
          {
            case FdoDataType_String:
            {
              if( reader->IsNull(name_prop) )
              {
                FdoPtr<FdoDataValue>data_val=  FdoDataValue::Create(FdoDataType_String);
                FdoPtr<FdoPropertyValue> propval = FdoPropertyValue::Create(name_prop,data_val);
                
                propcol->Add(propval);
              }
              else              
              {
                FdoPtr<FdoDataValue>data_val=  FdoDataValue::Create(reader->GetString(name_prop));
                FdoPtr<FdoPropertyValue> propval = FdoPropertyValue::Create(name_prop,data_val);
                
                propcol->Add(propval);
              }
            }
            break;   
              
            case FdoDataType_Double:
            {
              if( reader->IsNull(name_prop) )
              {
                FdoPtr<FdoDataValue>data_val=  FdoDataValue::Create(FdoDataType_Double);
                FdoPtr<FdoPropertyValue> propval = FdoPropertyValue::Create(name_prop,data_val);
                
                propcol->Add(propval);
              }
              else              
              {
                FdoPtr<FdoDataValue>data_val=  FdoDataValue::Create(reader->GetDouble(name_prop),FdoDataType_Double);
                FdoPtr<FdoPropertyValue> propval = FdoPropertyValue::Create(name_prop,data_val);
                
                propcol->Add(propval);
              }
            }
            break;
            case FdoDataType_Decimal:
            {
              if( reader->IsNull(name_prop) )
              {
                FdoPtr<FdoDataValue>data_val=  FdoDataValue::Create(FdoDataType_Decimal);
                FdoPtr<FdoPropertyValue> propval = FdoPropertyValue::Create(name_prop,data_val);
                
                propcol->Add(propval);
              }
              else              
              {
                FdoPtr<FdoDataValue>data_val=  FdoDataValue::Create(reader->GetDouble(name_prop),FdoDataType_Decimal);
                FdoPtr<FdoPropertyValue> propval = FdoPropertyValue::Create(name_prop,data_val);
                
                propcol->Add(propval);
              }
            }
            break;     
             case FdoDataType_Single:
            {
              if( reader->IsNull(name_prop) )
              {
                FdoPtr<FdoDataValue>data_val=  FdoDataValue::Create(FdoRasterDataType_Float);
                FdoPtr<FdoPropertyValue> propval = FdoPropertyValue::Create(name_prop,data_val);
                
                propcol->Add(propval);
              }
              else              
              {
                FdoPtr<FdoDataValue>data_val=  FdoDataValue::Create(reader->GetSingle(name_prop));
                FdoPtr<FdoPropertyValue> propval = FdoPropertyValue::Create(name_prop,data_val);
                
                propcol->Add(propval);
              }
            }
            break;    
            
            case FdoDataType_Int16:
            {
              if( reader->IsNull(name_prop) )
              {
                FdoPtr<FdoDataValue>data_val=  FdoDataValue::Create(FdoDataType_Int16);
                FdoPtr<FdoPropertyValue> propval = FdoPropertyValue::Create(name_prop,data_val);
                
                propcol->Add(propval);
              }
              else              
              {
                FdoPtr<FdoDataValue>data_val=  FdoDataValue::Create(reader->GetInt16(name_prop));
                FdoPtr<FdoPropertyValue> propval = FdoPropertyValue::Create(name_prop,data_val);
                
                propcol->Add(propval);
              }
            }
            break; 
            case FdoDataType_Int32:
            {
              if( reader->IsNull(name_prop) )
              {
                FdoPtr<FdoDataValue>data_val=  FdoDataValue::Create(FdoDataType_Int32);
                FdoPtr<FdoPropertyValue> propval = FdoPropertyValue::Create(name_prop,data_val);
                
                propcol->Add(propval);
              }
              else              
              {
                FdoPtr<FdoDataValue>data_val=  FdoDataValue::Create(reader->GetInt32(name_prop));
                FdoPtr<FdoPropertyValue> propval = FdoPropertyValue::Create(name_prop,data_val);
                
                propcol->Add(propval);
              }
            }
            break; 
            case FdoDataType_Int64:
            {
              if( reader->IsNull(name_prop) )
              {
                FdoPtr<FdoDataValue>data_val=  FdoDataValue::Create(FdoDataType_Int64);
                FdoPtr<FdoPropertyValue> propval = FdoPropertyValue::Create(name_prop,data_val);
                
                propcol->Add(propval);
              }
              else              
              {
                FdoPtr<FdoDataValue>data_val=  FdoDataValue::Create(reader->GetInt64(name_prop));
                FdoPtr<FdoPropertyValue> propval = FdoPropertyValue::Create(name_prop,data_val);
                
                propcol->Add(propval);
              }
            }
            break;     
            case FdoDataType_DateTime:
            {
              if( reader->IsNull(name_prop) )
              {
                FdoPtr<FdoDataValue>data_val=  FdoDataValue::Create(FdoDataType_DateTime);
                FdoPtr<FdoPropertyValue> propval = FdoPropertyValue::Create(name_prop,data_val);
                
                propcol->Add(propval);
              }
              else              
              {
                FdoPtr<FdoDataValue>data_val=  FdoDataValue::Create(reader->GetDateTime(name_prop));
                FdoPtr<FdoPropertyValue> propval = FdoPropertyValue::Create(name_prop,data_val);
                
                propcol->Add(propval);
              }
            }
            break;            
          }
          
          
          
        }
        break;
      }
    } 
  }
  reader->Close();
  
 
  
  // set filter for update 
  FdoPtr<FdoIdentifier> ident =  FdoIdentifier::Create(L"FEATID");
  FdoPtr<FdoDataValue> val2 =  FdoDataValue::Create(8465);
  FdoPtr<FdoFilter> filter = FdoComparisonCondition::Create(ident, FdoComparisonOperations_EqualTo, val2);
  comm_update->SetFilter(filter);
  
  FdoInt32 updated = comm_update->Execute();
  if( updated != 1 )
  {
    CPPUNIT_FAIL( "ut_InsertUpdateDelete::InsertUpdateDelete: Update should returned 1 for num of changed rows" );
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
  
}