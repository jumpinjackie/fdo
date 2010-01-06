#include "StdAfx.h"


#include <math.h>
#include "ut_KgIdsSelect.h"

#pragma warning(disable : 4996)


CPPUNIT_TEST_SUITE_REGISTRATION(ut_KgIdsSelect);

ut_KgIdsSelect::ut_KgIdsSelect(void)
{
}

ut_KgIdsSelect::~ut_KgIdsSelect(void)
{
}



void ut_KgIdsSelect::InsertRow(FdoIConnection* Conn,FdoString* ClassName,double* Ordinates,long OrdinatesCount,long Id,FdoString* Text)
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
}//end of ut_KgIdsSelect::InsertRow

//
// Test geometry transform from sdo_geometry to FDF fromat and back to sdo_geometry
//
void ut_KgIdsSelect::SelectOrderBy()
{

  //FdoPtr<FdoIConnection> conn = c_KgInfoUtil::OpenUnitTestConnection();
  FdoPtr<FdoIConnection> conn = c_KgInfoUtil::OpenUnitTestConnection();
  
  FdoPtr<FdoIDescribeSchema> comm = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema);
  FdoPtr<FdoFeatureSchemaCollection> schemas = comm->Execute();

/*  
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
*/ 


try
{
  FdoStringP src_classname = L"informix~cities~locn";
 
 /*
// Execute insert  
{  
  
  double ordinates1[10] = { 1,1 ,10,1 ,10,10 ,1,10 ,1,1 }; 
  InsertRow(conn,src_classname,ordinates1,10,1,L"Name1");
  
  double ordinates2[10] = { 1,1 ,10,1 ,10,10 ,1,10 ,1,1 }; 
  InsertRow(conn,src_classname,ordinates1,10,2,L"Name2");
  
  double ordinates3[10] = { 1,1 ,10,1 ,10,10 ,1,10 ,1,1 }; 
  InsertRow(conn,src_classname,ordinates1,10,3,L"Name3");
  
}  
  */
// execute select order by  
{  
  FdoPtr<FdoISelect> comm_select = (FdoISelect*)conn->CreateCommand( FdoCommandType_Select );    
  comm_select->SetFeatureClassName( src_classname );
  
  //comm_select->SetOrderingOption(FdoOrderingOption_Descending);
  //FdoPtr<FdoIdentifierCollection> order_ident_col = comm_select->GetOrdering();
  
  //FdoPtr<FdoIdentifier> order_ident = FdoIdentifier::Create(L"FID");
  //order_ident_col->Add(order_ident);
  
  
  FdoPtr<FdoIFeatureReader> freader = comm_select->Execute();
  
  CPPUNIT_ASSERT_MESSAGE( "FdoISelect:Execute returns NULL reader" , freader );
  

  bool isfirst=true;
  while( freader->ReadNext() )
  {
    FdoString* name = freader->GetString(L"name");
    FdoPtr<FdoByteArray> geom = freader->GetGeometry(L"locn");
    /*
    if( !isfirst && (freader->GetInt32(L"FID") > previd) )
    {
      CPPUNIT_FAIL( "Error SelectOrderBy: FID is not in Descending order!" );
    }
    previd = freader->GetInt32(L"FID");
    isfirst=false;
    */
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

}//end of ut_KgIdsSelect::SelectOrderBy


void ut_KgIdsSelect::SelectEnvelope()
{

  //FdoPtr<FdoIConnection> conn = c_KgInfoUtil::OpenUnitTestConnection();
  FdoPtr<FdoIConnection> conn = c_KgInfoUtil::OpenUnitTestConnection();

  FdoPtr<FdoIDescribeSchema> comm = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema);
  FdoPtr<FdoFeatureSchemaCollection> schemas = comm->Execute();



  try
  {
    FdoStringP src_classname = L"informix~cities~locn";

    /*
    // Execute insert  
    {  

    double ordinates1[10] = { 1,1 ,10,1 ,10,10 ,1,10 ,1,1 }; 
    InsertRow(conn,src_classname,ordinates1,10,1,L"Name1");

    double ordinates2[10] = { 1,1 ,10,1 ,10,10 ,1,10 ,1,1 }; 
    InsertRow(conn,src_classname,ordinates1,10,2,L"Name2");

    double ordinates3[10] = { 1,1 ,10,1 ,10,10 ,1,10 ,1,1 }; 
    InsertRow(conn,src_classname,ordinates1,10,3,L"Name3");

    }  
    */
    // execute select order by  
    {  
      FdoPtr<FdoISelect> comm_select = (FdoISelect*)conn->CreateCommand( FdoCommandType_Select );    
      comm_select->SetFeatureClassName( src_classname );
      
      comm_select->SetFilter(L"locn INTERSECTS GeomFromText('POLYGON ((-50 70, 50 70, 50 20, -50 20, -50 70))')");

      //comm_select->SetOrderingOption(FdoOrderingOption_Descending);
      //FdoPtr<FdoIdentifierCollection> order_ident_col = comm_select->GetOrdering();

      //FdoPtr<FdoIdentifier> order_ident = FdoIdentifier::Create(L"FID");
      //order_ident_col->Add(order_ident);


      FdoPtr<FdoIFeatureReader> freader = comm_select->Execute();

      CPPUNIT_ASSERT_MESSAGE( "FdoISelect:Execute returns NULL reader" , freader );

      bool isfirst=true;
      while( freader->ReadNext() )
      {
        FdoString* name = freader->GetString(L"name");
        FdoPtr<FdoByteArray> geom = freader->GetGeometry(L"locn");
        /*
        if( !isfirst && (freader->GetInt32(L"FID") > previd) )
        {
        CPPUNIT_FAIL( "Error SelectOrderBy: FID is not in Descending order!" );
        }
        previd = freader->GetInt32(L"FID");
        isfirst=false;
        */
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

}//end of ut_KgIdsSelect::SelectEnvelope


//simon
void ut_KgIdsSelect::SelectQueryAllSpatialFilters()
{

  //FdoPtr<FdoIConnection> conn = c_KgInfoUtil::OpenUnitTestConnection();
  FdoPtr<FdoIConnection> conn = c_KgInfoUtil::OpenUnitTestConnection();

  FdoPtr<FdoIDescribeSchema> comm = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema);
  FdoPtr<FdoFeatureSchemaCollection> schemas = comm->Execute();
  int cnt = 0;

  try
  {
    FdoStringP src_classname = L"informix~cities~locn";
    //FdoStringP src_classname = L"partner3~ne_roads~shape";

    double *ordinates;
    int numOrd = 10;
    ordinates = (double*)malloc(10*sizeof(double));
    ordinates[0] = -50;
    ordinates[1] = 70;
    ordinates[2] = 50;
    ordinates[3] = 70;
    ordinates[4] = 50;
    ordinates[5] = 20;
    ordinates[6] = -50;
    ordinates[7] = 20;
    ordinates[8] = -50;
    ordinates[9] = 70;
    {  
      FdoPtr<FdoISelect> comm_select = (FdoISelect*)conn->CreateCommand( FdoCommandType_Select );    
      comm_select->SetFeatureClassName( src_classname );
      
      FdoPtr<FdoFgfGeometryFactory> fgfFactory = FdoFgfGeometryFactory::GetInstance();
      FdoPtr<FdoILinearRing> lr = fgfFactory->CreateLinearRing(FdoDimensionality_XY,numOrd,ordinates);
	    FdoPtr<FdoIPolygon> polygon = fgfFactory->CreatePolygon(lr,NULL);
	    FdoPtr<FdoIGeometry> geometry = fgfFactory->CreateGeometry(polygon);
	    FdoPtr<FdoGeometryValue> geoValue = FdoGeometryValue::Create(fgfFactory->GetFgf(geometry));
      // using spatial operator FdoSpatialOperations_Intersects
	    FdoPtr<FdoFilter> filterIntersect = FdoSpatialCondition::Create(L"locn",FdoSpatialOperations_Intersects, geoValue);
	    FdoPtr<FdoFilter> filterEnvIntersect = FdoSpatialCondition::Create(L"locn",FdoSpatialOperations_EnvelopeIntersects, geoValue);
	    FdoPtr<FdoFilter> filterContains = FdoSpatialCondition::Create(L"locn",FdoSpatialOperations_Contains, geoValue);
	    FdoPtr<FdoFilter> filterCoveredBy = FdoSpatialCondition::Create(L"locn",FdoSpatialOperations_CoveredBy, geoValue);
	    FdoPtr<FdoFilter> filterCross = FdoSpatialCondition::Create(L"locn",FdoSpatialOperations_Crosses, geoValue);
	    FdoPtr<FdoFilter> filterDisjoint = FdoSpatialCondition::Create(L"locn",FdoSpatialOperations_Disjoint, geoValue);
	    FdoPtr<FdoFilter> filterEquals = FdoSpatialCondition::Create(L"locn",FdoSpatialOperations_Equals, geoValue);
	    FdoPtr<FdoFilter> filterInside = FdoSpatialCondition::Create(L"locn",FdoSpatialOperations_Inside, geoValue);
	    FdoPtr<FdoFilter> filterWithin = FdoSpatialCondition::Create(L"locn",FdoSpatialOperations_Within, geoValue);
	    //CString filter2 = L"name = 'Croatia'";
      //FdoPtr<FdoFilter> filterCombined = FdoFilter::Combine(filter2,FdoBinaryLogicalOperations_And,filter);
     
     //intersect
      comm_select->SetFilter(filterIntersect);
      FdoPtr<FdoIFeatureReader> freader = comm_select->Execute();
      while( freader->ReadNext() )
      {
        FdoString* name = freader->GetString(L"name");
        FdoPtr<FdoByteArray> geom = freader->GetGeometry(L"locn");
        cnt++;
      }
      freader->Close();

      //filterContains
      comm_select->SetFilter(filterContains);
      freader = comm_select->Execute();
      while( freader->ReadNext() )
      {
        FdoString* name = freader->GetString(L"name");
        FdoPtr<FdoByteArray> geom = freader->GetGeometry(L"locn");
        cnt++;
      }
      freader->Close();
      //filterCross
      comm_select->SetFilter(filterCross);
      freader = comm_select->Execute();
      while( freader->ReadNext() )
      {
        FdoString* name = freader->GetString(L"name");
        FdoPtr<FdoByteArray> geom = freader->GetGeometry(L"locn");
        cnt++;
      }
      freader->Close();
      //filterCoveredBy
      comm_select->SetFilter(filterCoveredBy);
      freader = comm_select->Execute();
      while( freader->ReadNext() )
      {
        FdoString* name = freader->GetString(L"name");
        FdoPtr<FdoByteArray> geom = freader->GetGeometry(L"locn");
        cnt++;
      }
      freader->Close();
      //filterDisjoint
      comm_select->SetFilter(filterDisjoint);
      freader = comm_select->Execute();
      while( freader->ReadNext() )
      {
        FdoString* name = freader->GetString(L"name");
        FdoPtr<FdoByteArray> geom = freader->GetGeometry(L"locn");
        cnt++;
      }
      freader->Close();
      
      //filterEquals
      comm_select->SetFilter(filterEquals);
      freader = comm_select->Execute();
      while( freader->ReadNext() )
      {
        FdoString* name = freader->GetString(L"name");
        FdoPtr<FdoByteArray> geom = freader->GetGeometry(L"locn");
        cnt++;
      }
      freader->Close();
      
      //filterInside
      comm_select->SetFilter(filterInside);
      freader = comm_select->Execute();
      while( freader->ReadNext() )
      {
        FdoString* name = freader->GetString(L"name");
        FdoPtr<FdoByteArray> geom = freader->GetGeometry(L"locn");
        cnt++;
      }
      freader->Close();
      
      //filterWithin
      comm_select->SetFilter(filterWithin);
      freader = comm_select->Execute();
      while( freader->ReadNext() )
      {
        FdoString* name = freader->GetString(L"name");
        FdoPtr<FdoByteArray> geom = freader->GetGeometry(L"locn");
        cnt++;
      }
      freader->Close();
      
      
    }  
    free(ordinates);
    conn->Close();
  }  
  catch(FdoException* ex)
  {
    FdoStringP str = ex->GetExceptionMessage();
    ex->Release();
    CPPUNIT_FAIL( (const char*)str );
  }
  
}//end of ut_KgIdsSelect::SelectQueryAllSpatialFilters


//simon
void ut_KgIdsSelect::SelectQuerySpatialCross()
{

  //FdoPtr<FdoIConnection> conn = c_KgInfoUtil::OpenUnitTestConnection();
  FdoPtr<FdoIConnection> conn = c_KgInfoUtil::OpenUnitTestConnection();

  FdoPtr<FdoIDescribeSchema> comm = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema);
  FdoPtr<FdoFeatureSchemaCollection> schemas = comm->Execute();
  int cnt = 0;

  try
  {
    FdoStringP src_classname = L"informix~world~locn";
    //FdoStringP src_classname = L"partner3~ne_roads~shape";

    double *ordinates;
    int numOrd = 10;
    ordinates = (double*)malloc(10*sizeof(double));
    ordinates[0] = -50;
    ordinates[1] = 70;
    ordinates[2] = 50;
    ordinates[3] = 70;
    ordinates[4] = 50;
    ordinates[5] = 20;
    ordinates[6] = -50;
    ordinates[7] = 20;
    ordinates[8] = -50;
    ordinates[9] = 70;
    {  
      FdoPtr<FdoISelect> comm_select = (FdoISelect*)conn->CreateCommand( FdoCommandType_Select );    
      comm_select->SetFeatureClassName( src_classname );
      
      FdoPtr<FdoFgfGeometryFactory> fgfFactory = FdoFgfGeometryFactory::GetInstance();
      FdoPtr<FdoILinearRing> lr = fgfFactory->CreateLinearRing(FdoDimensionality_XY,numOrd,ordinates);
	    FdoPtr<FdoIPolygon> polygon = fgfFactory->CreatePolygon(lr,NULL);
	    FdoPtr<FdoIGeometry> geometry = fgfFactory->CreateGeometry(polygon);
	    FdoPtr<FdoGeometryValue> geoValue = FdoGeometryValue::Create(fgfFactory->GetFgf(geometry));

	    FdoPtr<FdoFilter> filterCross = FdoSpatialCondition::Create(L"locn",FdoSpatialOperations_Crosses, geoValue);

     
      //filterCross
      comm_select->SetFilter(filterCross);
      FdoPtr<FdoIFeatureReader> freader = comm_select->Execute();
      while( freader->ReadNext() )
      {
        FdoString* name = freader->GetString(L"name");
        FdoPtr<FdoByteArray> geom = freader->GetGeometry(L"locn");
        cnt++;
      }
      freader->Close();
    
    }  
    free(ordinates);
    conn->Close();
  }  
  catch(FdoException* ex)
  {
    FdoStringP str = ex->GetExceptionMessage();
    ex->Release();
    CPPUNIT_FAIL( (const char*)str );
  }
  
}//end of ut_KgIdsSelect

void ut_KgIdsSelect::SelectQuerySpatialOverlaps()
{

  //FdoPtr<FdoIConnection> conn = c_KgInfoUtil::OpenUnitTestConnection();
  FdoPtr<FdoIConnection> conn = c_KgInfoUtil::OpenUnitTestConnection();

  FdoPtr<FdoIDescribeSchema> comm = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema);
  FdoPtr<FdoFeatureSchemaCollection> schemas = comm->Execute();
  int cnt = 0;

  try
  {
    FdoStringP src_classname = L"informix~world~locn";
    //FdoStringP src_classname = L"partner3~ne_roads~shape";

    double *ordinates;
    int numOrd = 10;
    ordinates = (double*)malloc(10*sizeof(double));
    ordinates[0] = -50;
    ordinates[1] = 70;
    ordinates[2] = 50;
    ordinates[3] = 70;
    ordinates[4] = 50;
    ordinates[5] = 20;
    ordinates[6] = -50;
    ordinates[7] = 20;
    ordinates[8] = -50;
    ordinates[9] = 70;
    {  
      FdoPtr<FdoISelect> comm_select = (FdoISelect*)conn->CreateCommand( FdoCommandType_Select );    
      comm_select->SetFeatureClassName( src_classname );
      
      FdoPtr<FdoFgfGeometryFactory> fgfFactory = FdoFgfGeometryFactory::GetInstance();
      FdoPtr<FdoILinearRing> lr = fgfFactory->CreateLinearRing(FdoDimensionality_XY,numOrd,ordinates);
	    FdoPtr<FdoIPolygon> polygon = fgfFactory->CreatePolygon(lr,NULL);
	    FdoPtr<FdoIGeometry> geometry = fgfFactory->CreateGeometry(polygon);
	    FdoPtr<FdoGeometryValue> geoValue = FdoGeometryValue::Create(fgfFactory->GetFgf(geometry));

	    FdoPtr<FdoFilter> filter = FdoSpatialCondition::Create(L"locn",FdoSpatialOperations_Overlaps, geoValue);
     
      comm_select->SetFilter(filter);
      FdoPtr<FdoIFeatureReader> freader = comm_select->Execute();
      while( freader->ReadNext() )
      {
        FdoString* name = freader->GetString(L"name");
        FdoPtr<FdoByteArray> geom = freader->GetGeometry(L"locn");
        cnt++;
      }
      freader->Close();
    
    }  
    free(ordinates);
    conn->Close();
  }  
  catch(FdoException* ex)
  {
    FdoStringP str = ex->GetExceptionMessage();
    ex->Release();
    CPPUNIT_FAIL( (const char*)str );
  }
  
}//end of ut_KgIdsSelect:


void ut_KgIdsSelect::SelectQuerySpatialInside()
{

  //FdoPtr<FdoIConnection> conn = c_KgInfoUtil::OpenUnitTestConnection();
  FdoPtr<FdoIConnection> conn = c_KgInfoUtil::OpenUnitTestConnection();

  FdoPtr<FdoIDescribeSchema> comm = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema);
  FdoPtr<FdoFeatureSchemaCollection> schemas = comm->Execute();
  int cnt = 0;

  try
  {
    FdoStringP src_classname = L"informix~cities~locn";
    //FdoStringP src_classname = L"partner3~ne_roads~shape";

    double *ordinates;
    int numOrd = 10;
    ordinates = (double*)malloc(10*sizeof(double));
    ordinates[0] = -40;
    ordinates[1] = 60;
    ordinates[2] = 40;
    ordinates[3] = 60;
    ordinates[4] = 40;
    ordinates[5] = 10;
    ordinates[6] = -40;
    ordinates[7] = 10;
    ordinates[8] = -40;
    ordinates[9] = 60;
    {  
      FdoPtr<FdoISelect> comm_select = (FdoISelect*)conn->CreateCommand( FdoCommandType_Select );    
      comm_select->SetFeatureClassName( src_classname );
      
      FdoPtr<FdoFgfGeometryFactory> fgfFactory = FdoFgfGeometryFactory::GetInstance();
      FdoPtr<FdoILinearRing> lr = fgfFactory->CreateLinearRing(FdoDimensionality_XY,numOrd,ordinates);
	    FdoPtr<FdoIPolygon> polygon = fgfFactory->CreatePolygon(lr,NULL);
	    FdoPtr<FdoIGeometry> geometry = fgfFactory->CreateGeometry(polygon);
	    FdoPtr<FdoGeometryValue> geoValue = FdoGeometryValue::Create(fgfFactory->GetFgf(geometry));

	    FdoPtr<FdoFilter> filter = FdoSpatialCondition::Create(L"locn",FdoSpatialOperations_Inside, geoValue);
     
      
     
      comm_select->SetFilter(filter);
      FdoPtr<FdoIFeatureReader> freader = comm_select->Execute();
      while( freader->ReadNext() )
      {
        //FdoString* name = freader->GetString(L"name");
        FdoPtr<FdoByteArray> geom = freader->GetGeometry(L"locn");
        cnt++;
      }
      freader->Close();
    
    }  
    free(ordinates);
    conn->Close();
  }  
  catch(FdoException* ex)
  {
    FdoStringP str = ex->GetExceptionMessage();
    ex->Release();
    CPPUNIT_FAIL( (const char*)str );
  }
  
}//end of ut_KgIdsInside:

