#include "StdAfx.h"

#include "c_KgInfoUtil.h"
#include <math.h>
#include "ut_DataTypes.h"

#pragma warning(disable : 4996)


CPPUNIT_TEST_SUITE_REGISTRATION(ut_DataTypes);

ut_DataTypes::ut_DataTypes(void)
{
}

ut_DataTypes::~ut_DataTypes(void)
{
}

void ut_DataTypes::DataTypes()
{

  FdoPtr<FdoIConnection> conn = c_KgInfoUtil::OpenUnitTestConnection();
  
  // create test table with different data types
  FdoPtr<FdoISQLCommand> comm = (FdoISQLCommand*)conn->CreateCommand( FdoCommandType_SQLCommand );

try
{  
  comm->SetSQLStatement(L"CREATE TABLE ut_DataTypes( \
                          dt_varchar2 VARCHAR2(64), dt_decimal NUMBER(10,0) \
                          ,dt_double NUMBER(10,3), dt_byte CHAR(1) \
                          ,dt_charstring CHAR(128), dt_binfloat BINARY_FLOAT \
                          ,dt_bindouble BINARY_DOUBLE, dt_datetime DATE \
                          ,dt_date DATE ,dt_time DATE \
                          ,dt_geom SDO_GEOMETRY )"
                       );
  comm->ExecuteNonQuery();
}
catch(FdoException* ) // exception because table already exist - skeep it
{

}

try
{
  
  // add data to be tested      // TO_DATE('01-01-1997', 'MM-DD-YYYY') TO_DATE('10-10-1', 'HH24-MI-SS')
  comm->SetSQLStatement(L"INSERT INTO ut_DataTypes VALUES('dt_varchar2' ,10 ,10.10 ,'b' ,'dt_charstring' ,20.20, 111.111 \
                        ,TO_DATE('01-05-1997 15:16:17','MM-DD-YYYY HH24-MI-SS') \
                        ,TO_DATE('12-02-1998','MM-DD-YYYY') \
                        ,TO_DATE('9:10:11','HH24-MI-SS') \
                        ,NULL\
                        )");
  comm->ExecuteNonQuery();
  
  // query table row
  comm->SetSQLStatement(L"SELECT \
                        dt_varchar2, dt_decimal  \
                        ,dt_double , dt_byte  \
                        ,dt_charstring , dt_binfloat  \
                        ,dt_bindouble , dt_datetime  \
                        ,dt_date  ,dt_time ,dt_geom \
                         from ut_DataTypes");
  FdoPtr<FdoISQLDataReader> sqlreader = comm->ExecuteReader();
  
  // fetch and test values
  sqlreader->ReadNext();
  
  int ccount = sqlreader->GetColumnCount();
  if( ccount != 11 )
  {
    CPPUNIT_FAIL( "Column count is not 11" );
  }
  
  FdoStringP colname;
  int colind=0;
  
  // test column 1
  {
    colname = sqlreader->GetColumnName(colind);
    if( strcmpi((const char*)colname,"dt_varchar2") )
    {
      CPPUNIT_FAIL( "Wrong column name" );
    }
    if( sqlreader->GetPropertyType(colname) != FdoPropertyType_DataProperty )
    {
      CPPUNIT_FAIL( "Wrong column property type" );
    }
    if( sqlreader->GetColumnType(colname) != FdoDataType_String )
    {
      CPPUNIT_FAIL( "Wrong column data type" );
    }
    FdoStringP colval = sqlreader->GetString(colname);
    if( strcmpi((const char*)colval,"dt_varchar2") )
    {
      CPPUNIT_FAIL( "Wrong column value" );
    }
  }
  
  // test column 2
  colind++;
  {
    colname = sqlreader->GetColumnName(colind);
    if( strcmpi((const char*)colname,"dt_decimal") )
    {
      CPPUNIT_FAIL( "Wrong column name" );
    }
    if( sqlreader->GetPropertyType(colname) != FdoPropertyType_DataProperty )
    {
      CPPUNIT_FAIL( "Wrong column property type" );
    }
    if( sqlreader->GetColumnType(colname) != FdoDataType_Int32 )
    {
      CPPUNIT_FAIL( "Wrong column data type" );
    }
    int colval = sqlreader->GetInt32(colname);
    if( colval != 10 )
    {
      CPPUNIT_FAIL( "Wrong column value" );
    }
  }
  // test column 3
  colind++;
  {
    colname = sqlreader->GetColumnName(colind);
    if( strcmpi((const char*)colname,"dt_double") )
    {
      CPPUNIT_FAIL( "Wrong column name" );
    }
    if( sqlreader->GetPropertyType(colname) != FdoPropertyType_DataProperty )
    {
      CPPUNIT_FAIL( "Wrong column property type" );
    }
    if( sqlreader->GetColumnType(colname) != FdoDataType_Decimal )
    {
      CPPUNIT_FAIL( "Wrong column data type" );
    }
    double colval = sqlreader->GetDouble(colname);
    if( colval != 10.10 )
    {
      CPPUNIT_FAIL( "Wrong column value" );
    }
  }
  // test column 4
  colind++;
  {
    colname = sqlreader->GetColumnName(colind);
    if( strcmpi((const char*)colname,"dt_byte") )
    {
      CPPUNIT_FAIL( "Wrong column name" );
    }
    if( sqlreader->GetPropertyType(colname) != FdoPropertyType_DataProperty )
    {
      CPPUNIT_FAIL( "Wrong column property type" );
    }
    if( sqlreader->GetColumnType(colname) != FdoDataType_Byte )
    {
      CPPUNIT_FAIL( "Wrong column data type" );
    }
    FdoByte colval = sqlreader->GetByte(colname);
    if( colval != 'b' )
    {
      CPPUNIT_FAIL( "Wrong column value" );
    }
  }
   // test column 5
   colind++;
  {
    colname = sqlreader->GetColumnName(colind);
    if( strcmpi((const char*)colname,"dt_charstring") )
    {
      CPPUNIT_FAIL( "Wrong column name" );
    }
    if( sqlreader->GetPropertyType(colname) != FdoPropertyType_DataProperty )
    {
      CPPUNIT_FAIL( "Wrong column property type" );
    }
    if( sqlreader->GetColumnType(colname) != FdoDataType_String )
    {
      CPPUNIT_FAIL( "Wrong column data type" );
    }
    FdoStringP colval = sqlreader->GetString(colname);
    char tempbuff[64];
    strncpy(tempbuff,(const char*)colval,strlen("dt_charstring"));
    tempbuff[strlen("dt_charstring")]=0;
    
    if( strcmpi(tempbuff,"dt_charstring") )
    {
      CPPUNIT_FAIL( "Wrong column value" );
    }
  } 
  // test column 6
  colind++;
  {
    colname = sqlreader->GetColumnName(colind);
    if( strcmpi((const char*)colname,"dt_binfloat") )
    {
      CPPUNIT_FAIL( "Wrong column name" );
    }
    if( sqlreader->GetPropertyType(colname) != FdoPropertyType_DataProperty )
    {
      CPPUNIT_FAIL( "Wrong column property type" );
    }
    if( sqlreader->GetColumnType(colname) != FdoDataType_Single )
    {
      CPPUNIT_FAIL( "Wrong column data type" );
    }
    float colval = sqlreader->GetSingle(colname);
    if( fabs(colval - 20.20) > 0.000001 )
    {
      CPPUNIT_FAIL( "Wrong column value" );
    }
  } 
  // test column 7
  colind++;
  {
    colname = sqlreader->GetColumnName(colind);
    if( strcmpi((const char*)colname,"dt_bindouble") )
    {
      CPPUNIT_FAIL( "Wrong column name" );
    }
    if( sqlreader->GetPropertyType(colname) != FdoPropertyType_DataProperty )
    {
      CPPUNIT_FAIL( "Wrong column property type" );
    }
    if( sqlreader->GetColumnType(colname) != FdoDataType_Double )
    {
      CPPUNIT_FAIL( "Wrong column data type" );
    }
    double colval = sqlreader->GetDouble(colname);
    if( colval != 111.111 )
    {
      CPPUNIT_FAIL( "Wrong column value" );
    }
  }
  

   // test column 8   
   colind++;
  {
    colname = sqlreader->GetColumnName(colind);
    if( strcmpi((const char*)colname,"dt_datetime") )
    {
      CPPUNIT_FAIL( "Wrong column name" );
    }
    if( sqlreader->GetPropertyType(colname) != FdoPropertyType_DataProperty )
    {
      CPPUNIT_FAIL( "Wrong column property type" );
    }
    if( sqlreader->GetColumnType(colname) != FdoDataType_DateTime )
    {
      CPPUNIT_FAIL( "Wrong column data type" );
    }
    FdoDateTime colval = sqlreader->GetDateTime(colname);
    if( colval.year != 1997 || colval.month!=1 || colval.day!=5 || colval.hour!=15 || colval.minute!=16 || colval.seconds!=17)
    {
      CPPUNIT_FAIL( "Wrong column value" );
    }
  } 
  
  // test column 9 
  colind++;   
  {
    colname = sqlreader->GetColumnName(colind);
    if( strcmpi((const char*)colname,"dt_date") )
    {
      CPPUNIT_FAIL( "Wrong column name" );
    }
    if( sqlreader->GetPropertyType(colname) != FdoPropertyType_DataProperty )
    {
      CPPUNIT_FAIL( "Wrong column property type" );
    }
    if( sqlreader->GetColumnType(colname) != FdoDataType_DateTime )
    {
      CPPUNIT_FAIL( "Wrong column data type" );
    }
    FdoDateTime colval = sqlreader->GetDateTime(colname);
    if( colval.year != 1998 || colval.month!=12 || colval.day!=2 )
    {
      CPPUNIT_FAIL( "Wrong column value" );
    }
  } 
  // test column 10 
  colind++;
  {
    colname = sqlreader->GetColumnName(colind);
    if( strcmpi((const char*)colname,"dt_time") )
    {
      CPPUNIT_FAIL( "Wrong column name" );
    }
    if( sqlreader->GetPropertyType(colname) != FdoPropertyType_DataProperty )
    {
      CPPUNIT_FAIL( "Wrong column property type" );
    }
    if( sqlreader->GetColumnType(colname) != FdoDataType_DateTime )
    {
      CPPUNIT_FAIL( "Wrong column data type" );
    }
    FdoDateTime colval = sqlreader->GetDateTime(colname);
    if( colval.hour!=9 || colval.minute!=10 || colval.seconds!=11 )
    {
      CPPUNIT_FAIL( "Wrong column value" );
    }
  }
  
  // test column 11
  colind++;
  {
    colname = sqlreader->GetColumnName(colind);
    if( strcmpi((const char*)colname,"dt_geom") )
    {
      CPPUNIT_FAIL( "Wrong column name" );
    }
    if( sqlreader->GetPropertyType(colname) != FdoPropertyType_GeometricProperty )
    {
      CPPUNIT_FAIL( "Wrong column property type" );
    }
    
  } 
  
  sqlreader->Close();
  
  comm->SetSQLStatement(L"DROP TABLE ut_DataTypes");
  comm->ExecuteNonQuery();
  
  
  conn->Close();
}  
catch(FdoException* ex)
{
  FdoStringP str = ex->GetExceptionMessage();
  ex->Release();
  CPPUNIT_FAIL( (const char*)str );
}

}//end of ut_DataTypes::DataTypes



//
// Test geometry transform from sdo_geometry to FDF fromat and back to sdo_geometry
//
void ut_DataTypes::GeometryTransform()
{

  FdoPtr<FdoIConnection> conn = c_KgInfoUtil::OpenUnitTestConnection();
  
  // create test table with different data types
  FdoPtr<FdoISQLCommand> sqlcomm = (FdoISQLCommand*)conn->CreateCommand( FdoCommandType_SQLCommand );

try
{  
  sqlcomm->SetSQLStatement(L"CREATE TABLE ut_GeometryTransform( \
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
  sqlcomm->SetSQLStatement(L"alter table ut_GeometryTransform add constraint ut_GeometryTransform_pk primary key ( fid)");
  sqlcomm->ExecuteNonQuery();
}  
catch(FdoException* )
{
} 
try
{  
  // insert user_sdo_geom_metadata
  sqlcomm->SetSQLStatement(L"INSERT INTO user_SDO_GEOM_METADATA  VALUES ('ut_GeometryTransform'\
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
  sqlcomm->SetSQLStatement(L"CREATE INDEX ut_GeometryTransform_sid ON ut_GeometryTransform (\"GEOM\")\
                            INDEXTYPE IS \"MDSYS\".\"SPATIAL_INDEX\" PARAMETERS ('sdo_indx_dims=2')"
                            );
  
  sqlcomm->ExecuteNonQuery();
}
catch(FdoException* )
{
}  
 
try
{  
  sqlcomm->SetSQLStatement(L"CREATE TABLE ut_GeometryTransform2( \
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
  sqlcomm->SetSQLStatement(L"alter table ut_GeometryTransform2 add constraint ut_GeometryTransform2_pk primary key ( fid)");
  sqlcomm->ExecuteNonQuery();
}  
catch(FdoException* )
{
}
try
{  
    // insert user_sdo_geom_metadata
  sqlcomm->SetSQLStatement(L"INSERT INTO user_SDO_GEOM_METADATA  VALUES ('ut_GeometryTransform2'\
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
  sqlcomm->SetSQLStatement(L"CREATE INDEX ut_GeometryTransform2_sid ON ut_GeometryTransform2 (\"GEOM\")\
                            INDEXTYPE IS \"MDSYS\".\"SPATIAL_INDEX\" PARAMETERS ('sdo_indx_dims=2')"
                            );
  
  sqlcomm->ExecuteNonQuery();
}
catch(FdoException* ) // exception because table already exist - skeep it
{

}

try
{
  FdoStringP src_classname = L"UNITTEST~UT_GEOMETRYTRANSFORM~GEOM";
  
  FdoPtr<FdoIInsert> comm_insert = (FdoIInsert*)conn->CreateCommand( FdoCommandType_Insert );    
  comm_insert->SetFeatureClassName( src_classname );
  
  FdoPtr<FdoPropertyValueCollection> propcol = comm_insert->GetPropertyValues();
  
  // Fid property
  FdoPtr<FdoDataValue> fid_val=  FdoDataValue::Create(1);
  FdoPtr<FdoPropertyValue> propval_fid = FdoPropertyValue::Create(L"FID",fid_val);
  
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
  
  propcol->Add(propval_fid);
  propcol->Add(propval_geom);
  propcol->Add(propval_name);
  
  FdoPtr<FdoIFeatureReader> insreader = comm_insert->Execute();
  
// Read Source features  
  FdoPtr<FdoISelect> comm_select = (FdoISelect*)conn->CreateCommand( FdoCommandType_Select );  
  
  comm_select->SetFeatureClassName( src_classname );
  
  FdoPtr<FdoIFeatureReader> freader = comm_select->Execute();
  
  // fetch and test values
  
  while( freader->ReadNext() )
  {
    long fid = freader->GetInt32(L"FID");
    
    FdoPtr<FdoByteArray> fgf = freader->GetGeometry(L"GEOM");
    
    
    
  }
  
  
  
  freader->Close();
  
  sqlcomm->SetSQLStatement(L"DROP TABLE ut_GeometryTransform");
  sqlcomm->ExecuteNonQuery();
  
  sqlcomm->SetSQLStatement(L"DROP TABLE ut_GeometryTransform2");
  sqlcomm->ExecuteNonQuery();
  
  
  conn->Close();
}  

catch(FdoException* ex)
{
  FdoStringP str = ex->GetExceptionMessage();
  ex->Release();
  CPPUNIT_FAIL( (const char*)str );
}

}//end of ut_DataTypes::GeometryTransform()
