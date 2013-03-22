#include "StdAfx.h"
#include "c_FeatureReaderFromPropValues.h"

c_FeatureReaderFromPropValues::c_FeatureReaderFromPropValues(FdoClassDefinition* ClassDef,FdoPropertyValueCollection* PropValues)
{
  m_ClassDef = FDO_SAFE_ADDREF(ClassDef);
  m_PropValues = FDO_SAFE_ADDREF(PropValues);
  m_RowCount = 0;
}

c_FeatureReaderFromPropValues::~c_FeatureReaderFromPropValues(void)
{
}

void c_FeatureReaderFromPropValues::Dispose()
{
  delete this;
}

FdoString* c_FeatureReaderFromPropValues::GetPropertyName( FdoInt32 index )
{
  if( index <0 || index >= m_PropValues->GetCount() )
  {
    throw FdoException::Create(L"c_FeatureReaderFromPropValues::GetPropertyName - Invalid index");
  }
  FdoPtr<FdoPropertyValue> val = m_PropValues->GetItem(index);
  
  FdoPtr<FdoIdentifier> ident = val->GetName();
  
  return ident->GetName();
}

FdoInt32 c_FeatureReaderFromPropValues::GetPropertyIndex( FdoString* propertyName )
{
  for(int ind=0; ind < m_PropValues->GetCount(); ind++ )
  {
    FdoPtr<FdoPropertyValue> val = m_PropValues->GetItem(ind);

    FdoPtr<FdoIdentifier> ident = val->GetName();
    
    if( FdoCommonOSUtil::wcsicmp(ident->GetName(),propertyName) == 0) return ind;
  }
  
  throw FdoException::Create(L"c_FeatureReaderFromPropValues::GetPropertyIndex - Unknown propertyName");
}

FdoClassDefinition* c_FeatureReaderFromPropValues::GetClassDefinition()
{
  return FDO_SAFE_ADDREF(m_ClassDef.p);
}

bool c_FeatureReaderFromPropValues::ReadNext()
{
  m_RowCount++;
  if( m_RowCount == 1 )
  {
    return true;
  }
  
  return false;
}

void c_FeatureReaderFromPropValues::Close()
{
  
}

FdoLiteralValue* c_FeatureReaderFromPropValues::GetLiteralValue( FdoInt32 index )
{
  FdoPtr<FdoPropertyValue> propval = m_PropValues->GetItem(index);
  FdoValueExpression* valex =  propval->GetValue();
  
  FdoLiteralValue* litval = dynamic_cast<FdoLiteralValue*>(valex);
  if(litval) return litval;
  
  valex->Release();
  
  throw FdoException::Create(L"c_FeatureReaderFromPropValues::GetLiteralValue - Invalid Expression type");
}

FdoDataValue* c_FeatureReaderFromPropValues::GetDataValue( FdoInt32 index )
{
  FdoPtr<FdoPropertyValue> propval = m_PropValues->GetItem(index);
  FdoValueExpression* valex =  propval->GetValue();

  FdoDataValue* dataval = dynamic_cast<FdoDataValue*>(valex);
  if(dataval) return dataval;

  valex->Release();

  throw FdoException::Create(L"c_FeatureReaderFromPropValues::GetDataValue - It is not DataValue expression");
}


FdoIRaster* c_FeatureReaderFromPropValues::GetRaster( FdoInt32 index )
{
  throw FdoException::Create(L"c_FeatureReaderFromPropValues::GetRaster - Unsupported Data Type");
}
FdoIRaster* c_FeatureReaderFromPropValues::GetRaster( FdoString* propertyName )
{
  return GetRaster(GetPropertyIndex(propertyName));
}
FdoLOBValue* c_FeatureReaderFromPropValues::GetLOB( FdoInt32 index )
{
  throw FdoException::Create(L"c_FeatureReaderFromPropValues::GetLOB - Unsupported Data Type");
}
FdoLOBValue* c_FeatureReaderFromPropValues::GetLOB( FdoString* propertyName )
{
  return GetLOB(GetPropertyIndex(propertyName));
}
FdoIStreamReader* c_FeatureReaderFromPropValues::GetLOBStreamReader( FdoInt32 index )
{
  throw FdoException::Create(L"c_FeatureReaderFromPropValues::GetLOBStreamReader - Unsupported Data Type");
}
FdoIStreamReader* c_FeatureReaderFromPropValues::GetLOBStreamReader( FdoString* propertyName )
{
  return GetLOBStreamReader(GetPropertyIndex(propertyName));
}

FdoIFeatureReader* c_FeatureReaderFromPropValues::GetFeatureObject( FdoInt32 index )
{
  throw FdoException::Create(L"c_FeatureReaderFromPropValues::GetFeatureObject - Unsupported Data Type");
}
FdoIFeatureReader* c_FeatureReaderFromPropValues::GetFeatureObject( FdoString* propertyName )
{
  return GetFeatureObject(GetPropertyIndex(propertyName));
}

FdoBoolean c_FeatureReaderFromPropValues::IsNull( FdoInt32 index )
{
  FdoPtr<FdoLiteralValue> propval = GetLiteralValue(index);
  if( propval->GetLiteralValueType() == FdoLiteralValueType_Data)
  {
    FdoDataValue * dataval = (FdoDataValue *)(propval.p);
    return dataval->IsNull();
  }
  if( propval->GetLiteralValueType() == FdoLiteralValueType_Geometry)
  {
    FdoGeometryValue * geomval = (FdoGeometryValue *)(propval.p);
    return geomval->IsNull();
  }
  
  return false;
}

bool c_FeatureReaderFromPropValues::IsNull( FdoString* propertyName )
{
  return IsNull(GetPropertyIndex(propertyName));
}

FdoBoolean c_FeatureReaderFromPropValues::GetBoolean( FdoInt32 index )
{
  FdoPtr<FdoDataValue> dataval = GetDataValue(index);
  
  if( dataval->GetDataType()==FdoDataType_Boolean)
  {
    FdoBooleanValue* bval = (FdoBooleanValue*)dataval.p;
    return bval->GetBoolean();
  }
  throw FdoException::Create(L"c_FeatureReaderFromPropValues::GetBoolean - Invalid Data Type");
}
FdoBoolean c_FeatureReaderFromPropValues::GetBoolean( FdoString* propertyName )
{
  return GetBoolean(GetPropertyIndex(propertyName));
}

FdoByte c_FeatureReaderFromPropValues::GetByte( FdoInt32 index )
{
  FdoPtr<FdoDataValue> dataval = GetDataValue(index);

  if( dataval->GetDataType()==FdoDataType_Byte)
  {
    FdoByteValue* bval = (FdoByteValue*)dataval.p;
    return bval->GetByte();
  }
  throw FdoException::Create(L"c_FeatureReaderFromPropValues::GetByte - Invalid Data Type");
}

FdoByte c_FeatureReaderFromPropValues::GetByte( FdoString* propertyName )
{
  return GetByte(GetPropertyIndex(propertyName));
}

FdoDateTime c_FeatureReaderFromPropValues::GetDateTime( FdoInt32 index )
{
  FdoPtr<FdoDataValue> dataval = GetDataValue(index);

  if( dataval->GetDataType()==FdoDataType_Byte)
  {
    FdoDateTimeValue* bval = (FdoDateTimeValue*)dataval.p;
    return bval->GetDateTime();
  }
  throw FdoException::Create(L"c_FeatureReaderFromPropValues::GetDateTime - Invalid Data Type");
}

FdoDateTime c_FeatureReaderFromPropValues::GetDateTime( FdoString* propertyName )
{
  return GetDateTime(GetPropertyIndex(propertyName));
}

FdoDouble c_FeatureReaderFromPropValues::GetDouble( FdoInt32 index )
{
  FdoPtr<FdoDataValue> dataval = GetDataValue(index);

  if( dataval->GetDataType()==FdoDataType_Double)
  {
    FdoDoubleValue* bval = (FdoDoubleValue*)dataval.p;
    return bval->GetDouble();
  }
  throw FdoException::Create(L"c_FeatureReaderFromPropValues::GetDouble - Invalid Data Type");
}

FdoDouble c_FeatureReaderFromPropValues::GetDouble( FdoString* propertyName )
{
  return GetDouble(GetPropertyIndex(propertyName));
}

FdoInt16 c_FeatureReaderFromPropValues::GetInt16( FdoInt32 index )
{
  FdoPtr<FdoDataValue> dataval = GetDataValue(index);

  if( dataval->GetDataType()==FdoDataType_Int16)
  {
    FdoInt16Value* bval = (FdoInt16Value*)dataval.p;
    return bval->GetInt16();
  }
  throw FdoException::Create(L"c_FeatureReaderFromPropValues::GetInt16 - Invalid Data Type");
}

FdoInt16 c_FeatureReaderFromPropValues::GetInt16( FdoString* propertyName )
{
  return GetInt16(GetPropertyIndex(propertyName));
}
FdoInt32 c_FeatureReaderFromPropValues::GetInt32( FdoInt32 index )
{
  FdoPtr<FdoDataValue> dataval = GetDataValue(index);

  if( dataval->GetDataType()==FdoDataType_Int32)
  {
    FdoInt32Value* bval = (FdoInt32Value*)dataval.p;
    return bval->GetInt32();
  }
  throw FdoException::Create(L"c_FeatureReaderFromPropValues::GetInt32 - Invalid Data Type");
}

FdoInt32 c_FeatureReaderFromPropValues::GetInt32( FdoString* propertyName )
{
  return GetInt32(GetPropertyIndex(propertyName));
}
FdoInt64 c_FeatureReaderFromPropValues::GetInt64( FdoInt32 index )
{
  FdoPtr<FdoDataValue> dataval = GetDataValue(index);

  if( dataval->GetDataType()==FdoDataType_Int64)
  {
    FdoInt64Value* bval = (FdoInt64Value*)dataval.p;
    return bval->GetInt64();
  }
  throw FdoException::Create(L"c_FeatureReaderFromPropValues::GetInt64 - Invalid Data Type");
}

FdoInt64 c_FeatureReaderFromPropValues::GetInt64( FdoString* propertyName )
{
  return GetInt64(GetPropertyIndex(propertyName));
}
float c_FeatureReaderFromPropValues::GetSingle( FdoInt32 index )
{
  FdoPtr<FdoDataValue> dataval = GetDataValue(index);

  if( dataval->GetDataType()==FdoDataType_Single)
  {
    FdoSingleValue* bval = (FdoSingleValue*)dataval.p;
    return bval->GetSingle();
  }
  throw FdoException::Create(L"c_FeatureReaderFromPropValues::GetSingle - Invalid Data Type");
}

float c_FeatureReaderFromPropValues::GetSingle( FdoString* propertyName )
{
  return GetSingle(GetPropertyIndex(propertyName));
}

FdoString* c_FeatureReaderFromPropValues::GetString( FdoInt32 index )
{
  FdoPtr<FdoDataValue> dataval = GetDataValue(index);

  if( dataval->GetDataType()==FdoDataType_String)
  {
    FdoStringValue* bval = (FdoStringValue*)dataval.p;
    return bval->GetString();
  }
  throw FdoException::Create(L"c_FeatureReaderFromPropValues::GetString - Invalid Data Type");
}

FdoString* c_FeatureReaderFromPropValues::GetString( FdoString* propertyName )
{
  return GetString(GetPropertyIndex(propertyName));
}

FdoByteArray* c_FeatureReaderFromPropValues::GetGeometry( FdoInt32 index )
{
  FdoPtr<FdoLiteralValue> litval = GetLiteralValue(index);

  if( litval->GetLiteralValueType()==FdoLiteralValueType_Geometry)
  {
    FdoGeometryValue* geomval = (FdoGeometryValue*)litval.p;
    return geomval->GetGeometry();
  }
  throw FdoException::Create(L"c_FeatureReaderFromPropValues::GetGeometry - Invalid Data Type");
}
FdoByteArray* c_FeatureReaderFromPropValues::GetGeometry( FdoString* propertyName)
{
  return GetGeometry(GetPropertyIndex(propertyName));   
}

const FdoByte * c_FeatureReaderFromPropValues::GetGeometry( FdoInt32 index, FdoInt32* count )
{
  m_Geom = GetGeometry( index );
  *count = m_Geom->GetCount();
  return m_Geom->GetData();
}

const FdoByte * c_FeatureReaderFromPropValues::GetGeometry( FdoString* propertyName, FdoInt32* count )
{
  return GetGeometry( GetPropertyIndex(propertyName), count );
}