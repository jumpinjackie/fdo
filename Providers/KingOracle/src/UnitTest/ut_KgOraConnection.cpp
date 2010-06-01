#include "StdAfx.h"
#include "c_KgOraUtil.h"
#include "ut_KgOraConnection.h"


CPPUNIT_TEST_SUITE_REGISTRATION(ut_KgOraConnection);



ut_KgOraConnection::ut_KgOraConnection(void)
{
}

ut_KgOraConnection::~ut_KgOraConnection(void)
{
}


void ut_KgOraConnection::OpenConnection()
{
try
{
  FdoPtr<FdoIConnection> conn = c_KgOraUtil::OpenUnitTestConnection_10_2();
  conn->Close();
}
catch(FdoException* ex)
{
  FdoStringP str = ex->GetExceptionMessage();
  ex->Release();
  CPPUNIT_FAIL( (const char*)str );
}  
  
}


void ut_KgOraConnection::QueryData()
{
try
{
  FdoPtr<FdoIConnection> conn = c_KgOraUtil::OpenUnitTestConnection_10_2();
  
  FdoPtr<FdoIDescribeSchema> comm_describe = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema);
  FdoPtr<FdoFeatureSchemaCollection> coll_schema = comm_describe->Execute();
  
  FdoPtr<FdoFeatureSchema> schema = coll_schema->FindItem(L"KingOra");
  FdoPtr<FdoClassCollection> coll_class = schema->GetClasses();
  FdoPtr<FdoClassDefinition> fdoclass = coll_class->FindItem(L"UNITTEST~BLDG_POLYGON~GEOMETRY");
  
  FdoPtr<FdoISelect> comm = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select);
  comm->SetFeatureClassName(L"KingOra:UNITTEST~BLDG_POLYGON~GEOMETRY");

  // -		testsql	0x089e7778 "GEOMETRY ENVELOPEINTERSECTS GeomFromText('POLYGON ((406319.018404908 120000, 453680.981595092 120000, 453680.981595092 150000, 406319.018404908 150000, 406319.018404908 120000))')"	const wchar_t *
  // -		testsql	0x088331e8 "GEOMETRY INTERSECTS GeomFromText('POLYGON ((424110.429447853 144969.325153374, 424355.828220859 144969.325153374, 424355.828220859 144723.926380368, 424110.429447853 144723.926380368, 424110.429447853 144969.325153374))')"	const wchar_t *
  
  // -		testsql	0x03de5d48 "GEOMETRY ENVELOPEINTERSECTS GeomFromText('POLYGON ((424846.625766871 135378.111192346, 429754.601226994 135378.111192346, 429754.601226994 138486.919482501, 424846.625766871 138486.919482501, 424846.625766871 135378.111192346))')"	const wchar_t *
  //-		testsql	0x0867a888 "GEOMETRY INTERSECTS GeomFromText('POLYGON ((425889.252678089 135994.786865444, 425914.682602753 135994.786865444, 425914.682602753 135969.35694078, 425889.252678089 135969.35694078, 425889.252678089 135994.786865444))')"	const wchar_t *
  
  //comm->SetFilter(L"GEOMETRY ENVELOPEINTERSECTS GeomFromText('POLYGON ((406319.018404908 120000, 453680.981595092 120000, 453680.981595092 150000, 406319.018404908 150000, 406319.018404908 120000))')");

  //FdoPtr<FdoIdentifierCollection> coll_ident = comm->GetPropertyNames();
  //FdoPtr<FdoIdentifier> ident1 = FdoIdentifier::Create(L"GEOMETRY");
  //FdoPtr<FdoIdentifier> ident2 = FdoIdentifier::Create(L"FEATID");
  //coll_ident->Add(ident1);
  //coll_ident->Add(ident2);
  
  
  clock_t elog_t1=clock();
  FdoPtr<FdoIFeatureReader> freader = comm->Execute();
  
  CPPUNIT_ASSERT_MESSAGE( "FdoISelect:Execute returns NULL reader" , freader );
  
  
  int row_count=0;

  
  FdoPtr<FdoPropertyDefinitionCollection> coll_prop = fdoclass->GetProperties();
  long count_prop = coll_prop->GetCount();
  
  FdoPtr<FdoIdentifierCollection> coll_ident = FdoIdentifierCollection::Create();
  
  for(int ind=0;ind<count_prop;ind++)
  {
    FdoPtr<FdoPropertyDefinition> prop = coll_prop->GetItem(ind);
    FdoPtr<FdoIdentifier> ident = FdoIdentifier::Create(prop->GetName());
    coll_ident->Add(ident);
  }
  
  
  long count_ident = coll_ident->GetCount();

  while( freader->ReadNext() )
  {
    row_count++;
    // read properties os source classdef
    
    for(int ind=0; ind<count_ident;ind++)
    {
      FdoPtr<FdoIdentifier> ident = coll_ident->GetItem(ind);
      //FdoPtr<FdoIdentifier> pident = propval->GetName();
      FdoPtr<FdoPropertyDefinition> pdef = coll_prop->FindItem(ident->GetName());
      
      
      switch( pdef->GetPropertyType() )
      {
        case FdoPropertyType_GeometricProperty:
        {
          if( !freader->IsNull(pdef->GetName()) )            
            FdoPtr<FdoByteArray> barray =  freader->GetGeometry(pdef->GetName());
          
          
        }
        break;
        
        case FdoPropertyType_DataProperty:
        {
          FdoDataPropertyDefinition* datadef = (FdoDataPropertyDefinition*)pdef.p;
          if( !freader->IsNull(datadef->GetName()) )
          {
            switch( datadef->GetDataType() )
            {
                case FdoDataType_Boolean:
                  freader->GetBoolean(ident->GetName());
                break;

                /// Represents unsigned 8-bit integers with values between 0 and 255.
                case FdoDataType_Byte:
                  freader->GetByte(ident->GetName());
                break;

                /// Represents a date and time value.
                case FdoDataType_DateTime:
                  freader->GetDateTime(ident->GetName());
                break;

                /// Represents values ranging from 1.0 x 10^-28 to approximately 7.9 x 10^28
                /// with 28-29 significant digits.
                case FdoDataType_Decimal:
                  freader->GetDouble(ident->GetName());
                break;

                /// Represents a floating point value ranging from approximately 5.0 
                /// x 10^-324 to 1.7 x 10^308 with a precision of 15-16 digits.
                case FdoDataType_Double:
                  freader->GetDouble(ident->GetName());
                break;

                /// Represents signed 16-bit integers with values between -32768 and 32767.
                case FdoDataType_Int16:
                  freader->GetInt16(ident->GetName());
                break;

                /// Represents signed 32-bit integers with values between -2147483648 
                /// and 2147483647.
                case FdoDataType_Int32:
                  freader->GetInt32(ident->GetName());
                break;

                /// Represents signed 64-bit integers with values 
                /// between -9223372036854775808 and 9223372036854775807.
                case FdoDataType_Int64:
                  freader->GetInt64(ident->GetName());
                break;

                /// Represents floating point values ranging from approximately 1.5 x 10^-45
                /// to 3.4 x 10^38, with a precision of 7 digits. 
                case FdoDataType_Single:
                  freader->GetSingle(ident->GetName());
                break;

                /// Represents a Unicode character strings.
                case FdoDataType_String:
                  freader->GetString(ident->GetName());
                break;

                /// Represents a binary large object stored as a collection of bytes.
                case FdoDataType_BLOB:
                  //freader->GetBl(ident->GetName());
                break;
                

                /// Represents a character large object stored as a collection of
                /// characters.
                case FdoDataType_CLOB:
                  //freader->GetBoolean(ident->GetName());
                break;
            }
          }
                    
        }
        
        break;
        
      } 
           
    }
    //FdoPtr<FdoByteArray> fgf = freader->GetGeometry(L"GEOMETRY");
    
  }
  
  clock_t elog_t2=clock();
  double msecs = (double)((elog_t2-elog_t1)* CLOCKS_PER_SEC / 1000);
  
  freader->Close();
  conn->Close();
  
  double secs = msecs;
  secs = secs / 1000.0;
  
  char buff[128];
  sprintf(buff,"Ellapsed %.2lfs rows_count=%d ",secs,row_count);
  CPPUNIT_FAIL( buff );
}
catch(FdoException* ex)
{
  FdoStringP str = ex->GetExceptionMessage();
  ex->Release();
  CPPUNIT_FAIL( (const char*)str );
}
  
}//end of ut_KgOraConnection::QueryData()

void ut_KgOraConnection::QueryDataSameConnection()
{
try
{
  
  
  FdoPtr<FdoIDescribeSchema> comm_describe = (FdoIDescribeSchema*)m_Conn->CreateCommand(FdoCommandType_DescribeSchema);
  FdoPtr<FdoFeatureSchemaCollection> coll_schema = comm_describe->Execute();
  
  FdoPtr<FdoFeatureSchema> schema = coll_schema->FindItem(L"KingOra");
  FdoPtr<FdoClassCollection> coll_class = schema->GetClasses();
  //FdoPtr<FdoClassDefinition> fdoclass = coll_class->FindItem(L"UNITTEST~BLDG_POLYGON~GEOMETRY");
  FdoPtr<FdoClassDefinition> fdoclass = coll_class->FindItem(L"NANAIMO~SP_PARCEL~GEOMETRY");
  
  FdoPtr<FdoISelect> comm = (FdoISelect*)m_Conn->CreateCommand(FdoCommandType_Select);
  comm->SetFeatureClassName(L"KingOra:NANAIMO~SP_PARCEL~GEOMETRY");

  // -		testsql	0x089e7778 "GEOMETRY ENVELOPEINTERSECTS GeomFromText('POLYGON ((406319.018404908 120000, 453680.981595092 120000, 453680.981595092 150000, 406319.018404908 150000, 406319.018404908 120000))')"	const wchar_t *
  // -		testsql	0x088331e8 "GEOMETRY INTERSECTS GeomFromText('POLYGON ((424110.429447853 144969.325153374, 424355.828220859 144969.325153374, 424355.828220859 144723.926380368, 424110.429447853 144723.926380368, 424110.429447853 144969.325153374))')"	const wchar_t *
  
  // -		testsql	0x03de5d48 "GEOMETRY ENVELOPEINTERSECTS GeomFromText('POLYGON ((424846.625766871 135378.111192346, 429754.601226994 135378.111192346, 429754.601226994 138486.919482501, 424846.625766871 138486.919482501, 424846.625766871 135378.111192346))')"	const wchar_t *
  //-		testsql	0x0867a888 "GEOMETRY INTERSECTS GeomFromText('POLYGON ((425889.252678089 135994.786865444, 425914.682602753 135994.786865444, 425914.682602753 135969.35694078, 425889.252678089 135969.35694078, 425889.252678089 135994.786865444))')"	const wchar_t *
  
  //comm->SetFilter(L"GEOMETRY ENVELOPEINTERSECTS GeomFromText('POLYGON ((406319.018404908 120000, 453680.981595092 120000, 453680.981595092 150000, 406319.018404908 150000, 406319.018404908 120000))')");

  //FdoPtr<FdoIdentifierCollection> coll_ident = comm->GetPropertyNames();
  //FdoPtr<FdoIdentifier> ident1 = FdoIdentifier::Create(L"GEOMETRY");
  //FdoPtr<FdoIdentifier> ident2 = FdoIdentifier::Create(L"FEATID");
  //coll_ident->Add(ident1);
  //coll_ident->Add(ident2);
  
  
  clock_t elog_t1=clock();
  FdoPtr<FdoIFeatureReader> freader = comm->Execute();
  
  CPPUNIT_ASSERT_MESSAGE( "FdoISelect:Execute returns NULL reader" , freader );
  
  
  int row_count=0;

  
  FdoPtr<FdoPropertyDefinitionCollection> coll_prop = fdoclass->GetProperties();
  long count_prop = coll_prop->GetCount();
  
  FdoPtr<FdoIdentifierCollection> coll_ident = FdoIdentifierCollection::Create();
  
  for(int ind=0;ind<count_prop;ind++)
  {
    FdoPtr<FdoPropertyDefinition> prop = coll_prop->GetItem(ind);
    FdoPtr<FdoIdentifier> ident = FdoIdentifier::Create(prop->GetName());
    coll_ident->Add(ident);
  }
  
  
  long count_ident = coll_ident->GetCount();

  while( freader->ReadNext() )
  {
    row_count++;
    // read properties os source classdef
    
    for(int ind=0; ind<count_ident;ind++)
    {
      FdoPtr<FdoIdentifier> ident = coll_ident->GetItem(ind);
      //FdoPtr<FdoIdentifier> pident = propval->GetName();
      FdoPtr<FdoPropertyDefinition> pdef = coll_prop->FindItem(ident->GetName());
      
      
      switch( pdef->GetPropertyType() )
      {
        case FdoPropertyType_GeometricProperty:
        {
          if( !freader->IsNull(pdef->GetName()) )            
            FdoPtr<FdoByteArray> barray =  freader->GetGeometry(pdef->GetName());
          
          
        }
        break;
        
        case FdoPropertyType_DataProperty:
        {
          FdoDataPropertyDefinition* datadef = (FdoDataPropertyDefinition*)pdef.p;
          if( !freader->IsNull(datadef->GetName()) )
          {
            switch( datadef->GetDataType() )
            {
                case FdoDataType_Boolean:
                  freader->GetBoolean(ident->GetName());
                break;

                /// Represents unsigned 8-bit integers with values between 0 and 255.
                case FdoDataType_Byte:
                  freader->GetByte(ident->GetName());
                break;

                /// Represents a date and time value.
                case FdoDataType_DateTime:
                  freader->GetDateTime(ident->GetName());
                break;

                /// Represents values ranging from 1.0 x 10^-28 to approximately 7.9 x 10^28
                /// with 28-29 significant digits.
                case FdoDataType_Decimal:
                  freader->GetDouble(ident->GetName());
                break;

                /// Represents a floating point value ranging from approximately 5.0 
                /// x 10^-324 to 1.7 x 10^308 with a precision of 15-16 digits.
                case FdoDataType_Double:
                  freader->GetDouble(ident->GetName());
                break;

                /// Represents signed 16-bit integers with values between -32768 and 32767.
                case FdoDataType_Int16:
                  freader->GetInt16(ident->GetName());
                break;

                /// Represents signed 32-bit integers with values between -2147483648 
                /// and 2147483647.
                case FdoDataType_Int32:
                  freader->GetInt32(ident->GetName());
                break;

                /// Represents signed 64-bit integers with values 
                /// between -9223372036854775808 and 9223372036854775807.
                case FdoDataType_Int64:
                  freader->GetInt64(ident->GetName());
                break;

                /// Represents floating point values ranging from approximately 1.5 x 10^-45
                /// to 3.4 x 10^38, with a precision of 7 digits. 
                case FdoDataType_Single:
                  freader->GetSingle(ident->GetName());
                break;

                /// Represents a Unicode character strings.
                case FdoDataType_String:
                  freader->GetString(ident->GetName());
                break;

                /// Represents a binary large object stored as a collection of bytes.
                case FdoDataType_BLOB:
                  //freader->GetBl(ident->GetName());
                break;
                

                /// Represents a character large object stored as a collection of
                /// characters.
                case FdoDataType_CLOB:
                  //freader->GetBoolean(ident->GetName());
                break;
            }
          }
                    
        }
        
        break;
        
      } 
           
    }
    //FdoPtr<FdoByteArray> fgf = freader->GetGeometry(L"GEOMETRY");
    
  }
  
  clock_t elog_t2=clock();
  double msecs = (double)((elog_t2-elog_t1)* CLOCKS_PER_SEC / 1000);
  
  freader->Close();
  
  
  double secs = msecs;
  secs = secs / 1000.0;
  
  char buff[128];
  sprintf(buff,"Ellapsed %.2lfs rows_count=%d ",secs,row_count);
  CPPUNIT_FAIL( buff );
}
catch(FdoException* ex)
{
  FdoStringP str = ex->GetExceptionMessage();
  ex->Release();
  CPPUNIT_FAIL( (const char*)str );
}
  
}//end of ut_KgOraConnection::QueryData()

void ut_KgOraConnection::PerfomanceTest()
{
try
{
  FdoPtr<FdoIConnection> conn = c_KgOraUtil::OpenPerfomanceTestConnection();
  
  FdoPtr<FdoISelect> comm = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select);
  comm->SetFeatureClassName(L"KingOra:OSE~POLYLINE_PARCELE~GEOMETRY");

  // -		testsql	0x089e7778 "GEOMETRY ENVELOPEINTERSECTS GeomFromText('POLYGON ((406319.018404908 120000, 453680.981595092 120000, 453680.981595092 150000, 406319.018404908 150000, 406319.018404908 120000))')"	const wchar_t *
  // -		testsql	0x088331e8 "GEOMETRY INTERSECTS GeomFromText('POLYGON ((424110.429447853 144969.325153374, 424355.828220859 144969.325153374, 424355.828220859 144723.926380368, 424110.429447853 144723.926380368, 424110.429447853 144969.325153374))')"	const wchar_t *
  
  // -		testsql	0x03de5d48 "GEOMETRY ENVELOPEINTERSECTS GeomFromText('POLYGON ((424846.625766871 135378.111192346, 429754.601226994 135378.111192346, 429754.601226994 138486.919482501, 424846.625766871 138486.919482501, 424846.625766871 135378.111192346))')"	const wchar_t *
  //-		testsql	0x0867a888 "GEOMETRY INTERSECTS GeomFromText('POLYGON ((425889.252678089 135994.786865444, 425914.682602753 135994.786865444, 425914.682602753 135969.35694078, 425889.252678089 135969.35694078, 425889.252678089 135994.786865444))')"	const wchar_t *
  
  comm->SetFilter(L"GEOMETRY ENVELOPEINTERSECTS GeomFromText('POLYGON ((406319.018404908 120000, 453680.981595092 120000, 453680.981595092 150000, 406319.018404908 150000, 406319.018404908 120000))')");

  
  FdoPtr<FdoIFeatureReader> freader = comm->Execute();
  
  CPPUNIT_ASSERT_MESSAGE( "FdoISelect:Execute returns NULL reader" , freader );
  
  


  while( freader->ReadNext() )
  {
    freader->GetGeometry(L"GEOMETRY");
  }
  
  conn->Close();
}
catch(FdoException* ex)
{
  FdoStringP str = ex->GetExceptionMessage();
  ex->Release();
  CPPUNIT_FAIL( (const char*)str );
}
  
}//end of ut_KgOraConnection::PerfomanceTest


UINT MyThreadProc( LPVOID pParam )
{
    
try
{
  FdoPtr<FdoIConnection> conn = c_KgOraUtil::OpenPerfomanceTestConnection();
  
  FdoPtr<FdoISelect> comm = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select);
  comm->SetFeatureClassName(L"KingOra:UNITTEST~REDDING_PARCELS~GEOMETRY");

  // -		testsql	0x089e7778 "GEOMETRY ENVELOPEINTERSECTS GeomFromText('POLYGON ((406319.018404908 120000, 453680.981595092 120000, 453680.981595092 150000, 406319.018404908 150000, 406319.018404908 120000))')"	const wchar_t *
  // -		testsql	0x088331e8 "GEOMETRY INTERSECTS GeomFromText('POLYGON ((424110.429447853 144969.325153374, 424355.828220859 144969.325153374, 424355.828220859 144723.926380368, 424110.429447853 144723.926380368, 424110.429447853 144969.325153374))')"	const wchar_t *
  
  // -		testsql	0x03de5d48 "GEOMETRY ENVELOPEINTERSECTS GeomFromText('POLYGON ((424846.625766871 135378.111192346, 429754.601226994 135378.111192346, 429754.601226994 138486.919482501, 424846.625766871 138486.919482501, 424846.625766871 135378.111192346))')"	const wchar_t *
  //-		testsql	0x0867a888 "GEOMETRY INTERSECTS GeomFromText('POLYGON ((425889.252678089 135994.786865444, 425914.682602753 135994.786865444, 425914.682602753 135969.35694078, 425889.252678089 135969.35694078, 425889.252678089 135994.786865444))')"	const wchar_t *
  
  comm->SetFilter(L"GEOMETRY ENVELOPEINTERSECTS GeomFromText('POLYGON ((406319.018404908 120000, 453680.981595092 120000, 453680.981595092 150000, 406319.018404908 150000, 406319.018404908 120000))')");

  
  FdoPtr<FdoIFeatureReader> freader = comm->Execute();
  
  CPPUNIT_ASSERT_MESSAGE( "FdoISelect:Execute returns NULL reader" , freader );
  
  

  long count=0;
  while( freader->ReadNext() )
  {
    count++;;
    freader->GetGeometry(L"GEOMETRY");
  }
  
  conn->Close();
  
  
  //CPPUNIT_ASSERT_MESSAGE( "FdoISelect:Execute returns NULL reader" , freader );
}
catch(FdoException* ex)
{
  FdoStringP str = ex->GetExceptionMessage();
  ex->Release();
  CPPUNIT_FAIL( (const char*)str );
}
    return 0;   // thread completed successfully
}





void ut_KgOraConnection::MultithreadTest()
{
try
{
 CWinThread *th1 = AfxBeginThread(MyThreadProc, NULL);
 CWinThread *th2 = AfxBeginThread(MyThreadProc, NULL);
}
catch(FdoException* ex)
{
  FdoStringP str = ex->GetExceptionMessage();
  ex->Release();
  CPPUNIT_FAIL( (const char*)str );
}
  
}//end of ut_KgOraConnection::MultithreadTest

void ut_KgOraConnection::setUp()
{
  //m_Conn = c_KgOraUtil::OpenUnitTestConnection_10_2();
  m_Conn = c_KgOraUtil::OpenNanaimoConnection_10_2();
}

void ut_KgOraConnection::tearDown()
{
  m_Conn->Close();
}