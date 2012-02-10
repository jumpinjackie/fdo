#include "StdAfx.h"
#include <FdoExpressionEngine.h>
#include "c_TestExpressionEngine.h"

CPPUNIT_TEST_SUITE_REGISTRATION(c_TestExpressionEngine);

c_TestExpressionEngine::c_TestExpressionEngine(void)
{
}

c_TestExpressionEngine::~c_TestExpressionEngine(void)
{
}

void c_TestExpressionEngine::TryEngine()
{
  try
  {
    c_Util::CreateTestKML();
    FdoPtr<FdoIConnection> conn = c_Util::OpenUnitTestConnection();
    c_KgKmlConnection * kingkmlconn = (c_KgKmlConnection*)conn.p;
    FdoPtr<c_KgKmlFdoData> kmlfdodata = kingkmlconn->GetKmlFdoData();

    FdoPtr<c_KgKmlFdoClassConfig> kmlfdocfg = kmlfdodata->GetKmlFdoClassConfig();

    FdoPtr<FdoIDescribeSchema> comm = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema);
    FdoPtr<FdoFeatureSchemaCollection> schemas = comm->Execute();

    FdoPtr<FdoFeatureSchema> schema = schemas->GetItem(0);
    FdoPtr<FdoClassCollection> classes = schema->GetClasses();
    FdoPtr<FdoClassDefinition> classdef = classes->GetItem(0);
    // Read Data
    // select source data
    FdoPtr<FdoISelect> selcomm = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select);  
    selcomm->SetFeatureClassName(classdef->GetName());

    FdoPtr<FdoIFeatureReader> freader = selcomm->Execute();
    freader->ReadNext();

    FdoPtr<FdoExpressionEngine> expengine = FdoExpressionEngine::Create(freader,classdef,NULL);
    
    FdoPtr<FdoExpression> exp1 = FdoExpression::Parse(L"9900");
    
    FdoPtr<FdoLiteralValue> litvalue = expengine->Evaluate(exp1);
    if (litvalue->GetLiteralValueType() == FdoLiteralValueType_Data)
    {
      FdoDataValue *dataValue = static_cast<FdoDataValue *> (litvalue.p);
      
       FdoStringP strp = dataValue->ToString();
       
      switch( dataValue->GetDataType() )
      {
          case FdoDataType_Boolean:
          break;
          /// Represents unsigned 8-bit integers with values between 0 and 255.
          case FdoDataType_Byte:
          break;
          /// Represents a date and time value.
          case FdoDataType_DateTime:
            break;
          /// Represents values ranging from 1.0 x 10^-28 to approximately 7.9 x 10^28
          /// with 28-29 significant digits.
          case FdoDataType_Decimal:
            break;
          /// Represents a floating point value ranging from approximately 5.0 
          /// x 10^-324 to 1.7 x 10^308 with a precision of 15-16 digits.
          case FdoDataType_Double:
            break;
          /// Represents signed 16-bit integers with values between -32768 and 32767.
          case FdoDataType_Int16:
            break;
          /// Represents signed 32-bit integers with values between -2147483648 
          /// and 2147483647.
          case FdoDataType_Int32:
            break;
          /// Represents signed 64-bit integers with values 
          /// between -9223372036854775808 and 9223372036854775807.
          case FdoDataType_Int64:
            break;
          /// Represents floating point values ranging from approximately 1.5 x 10^-45
          /// to 3.4 x 10^38, with a precision of 7 digits. 
          case FdoDataType_Single:
            break;
          /// Represents a Unicode character strings.
          case FdoDataType_String:
            break;
          /// Represents a binary large object stored as a collection of bytes.
          case FdoDataType_BLOB:
            break;
          /// Represents a character large object stored as a collection of
          /// characters.
          case FdoDataType_CLOB:
            break;
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

void c_TestExpressionEngine::TestMemory()
{

}

void c_TestExpressionEngine::TestSdfConn()
{
  try
  {
    c_Util::CreateTestKML();
    //FdoPtr<FdoIConnection> conn = c_Util::OpenUnitTestConnection_ParceleKMZ();
    FdoPtr<FdoIConnection> conn = c_Util::OpenUnitTestConnection_ParceleSDF();
    
    //c_KgKmlConnection * kingkmlconn = (c_KgKmlConnection*)conn.p;
    //FdoPtr<c_KgKmlFdoData> kmlfdodata = kingkmlconn->GetKmlFdoData();
    //FdoPtr<c_KgKmlFdoClassConfig> kmlfdocfg = kmlfdodata->GetKmlFdoClassConfig();

    FdoPtr<FdoIDescribeSchema> comm = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema);
    FdoPtr<FdoFeatureSchemaCollection> schemas = comm->Execute();

    FdoPtr<FdoFeatureSchema> schema = schemas->GetItem(0);
    FdoPtr<FdoClassCollection> classes = schema->GetClasses();
    FdoPtr<FdoClassDefinition> classdef = classes->GetItem(0);
    // Read Data
    // select source data
    FdoPtr<FdoISelect> selcomm1 = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select);  
    selcomm1->SetFeatureClassName(classdef->GetName());

    FdoPtr<FdoIFeatureReader> freader1 = selcomm1->Execute();
    freader1->ReadNext();


    FdoPtr<FdoISelect> selcomm2 = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select);  
    selcomm2->SetFeatureClassName(classdef->GetName());

    FdoPtr<FdoIFeatureReader> freader2 = selcomm2->Execute();
    freader2->ReadNext();
    
    int cnt1;
    const FdoByte* bytes1 = freader1->GetGeometry(L"KML_FDO_GEOMETRY",&cnt1);
    
    int cnt2;
    const FdoByte* bytes2 = freader2->GetGeometry(L"KML_FDO_GEOMETRY",&cnt2);
    
    int cnt3;
    freader1->ReadNext();
    const FdoByte* bytes3 = freader1->GetGeometry(L"KML_FDO_GEOMETRY",&cnt3);
    
    int cnt4;
    freader2->ReadNext();
    const FdoByte* bytes4 = freader2->GetGeometry(L"KML_FDO_GEOMETRY",&cnt4);
    
    int cnt5;
    freader1->ReadNext();
    const FdoByte* bytes5 = freader1->GetGeometry(L"KML_FDO_GEOMETRY",&cnt5);

    int cnt6;
    freader2->ReadNext();
    const FdoByte* bytes6 = freader2->GetGeometry(L"KML_FDO_GEOMETRY",&cnt6);

    freader1->Close();
    freader2->Close();
   
  }  
  catch(FdoException* ex)
  {
    FdoStringP str = ex->GetExceptionMessage();
    ex->Release();
    CPPUNIT_FAIL( (const char*)str );
  }
}