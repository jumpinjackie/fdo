/*
* Copyright (C) 2006  SL-King d.o.o
* 
* This library is free software; you can redistribute it and/or
* modify it under the terms of version 2.1 of the GNU Lesser
* General Public License as published by the Free Software Foundation.
* 
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
* 
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "stdafx.h"

#include "c_KgOraFeatureReaderInsert.h"




c_KgOraFeatureReaderInsert::c_KgOraFeatureReaderInsert( FdoPropertyValueCollection* FeatureValues, FdoClassDefinition* ClassDef )
{
  
  m_FeatureValues = FeatureValues;
  FDO_SAFE_ADDREF(m_FeatureValues.p);
    
  m_IsFirstReadNext = true;
  
  m_ClassDef = ClassDef;
  FDO_SAFE_ADDREF(m_ClassDef.p);
}

c_KgOraFeatureReaderInsert::~c_KgOraFeatureReaderInsert()
{
  
}

void c_KgOraFeatureReaderInsert::Dispose()
{
    delete this;
}

FdoClassDefinition* c_KgOraFeatureReaderInsert::GetClassDefinition()
{
  return FDO_SAFE_ADDREF(m_ClassDef.p);

}

int c_KgOraFeatureReaderInsert::GetDepth()
{
    throw FdoCommandException::Create(L"c_KgOraFeatureReaderInsert::GetDepth unsupported!");
}

FdoLOBValue* c_KgOraFeatureReaderInsert::GetLOB(const wchar_t* propertyName )
{
  throw FdoCommandException::Create(L"c_KgOraFeatureReaderInsert::GetLOB unsupported!");
}

FdoIStreamReader* c_KgOraFeatureReaderInsert::GetLOBStreamReader(const wchar_t* propertyName )
{
  throw FdoCommandException::Create(L"c_KgOraFeatureReaderInsert::GetLOBStreamReader unsupported!");
}

FdoIFeatureReader* c_KgOraFeatureReaderInsert::GetFeatureObject( const wchar_t* propertyName )
{
  throw FdoCommandException::Create(L"c_KgOraFeatureReaderInsert::GetFeatureObject unsupported!");
}

bool c_KgOraFeatureReaderInsert::GetBoolean( const wchar_t *PropName )
{
    return (GetInt32( PropName ) != 0 );
}

FdoByte c_KgOraFeatureReaderInsert::GetByte( const wchar_t *PropName )
{
    return (FdoByte)GetInt32( PropName );
}


FdoDateTime c_KgOraFeatureReaderInsert::GetDateTime( const wchar_t *PropName )
{
     FdoPtr<FdoPropertyValue> propvalue;

  if( m_IsFirstReadNext || m_FeatureValues == NULL )
      throw FdoCommandException::Create( L"c_KgOraFeatureReaderInsert::GetDateTime End of recordset!" );

  try
  {
      propvalue =  m_FeatureValues->GetItem( PropName );
      if( propvalue == NULL )
          throw FdoCommandException::Create( L"c_KgOraFeatureReaderInsert::GetDouble Property not found!" );
  }
  catch ( FdoException * e)
  {
    FDO_SAFE_RELEASE(e);
    throw FdoCommandException::Create( L"c_KgOraFeatureReaderInsert::GetDouble Property not found!" );
  }
  FdoPtr<FdoValueExpression> val = propvalue->GetValue();
  
  FdoDataValue* dataval = (dynamic_cast<FdoDataValue*>(val.p));
  
  if( !dataval && dataval->GetDataType() != FdoDataType_DateTime)
    throw FdoCommandException::Create( L"c_KgOraFeatureReaderInsert::GetDouble DataType not FdoDataType_Int64!" );

  FdoDateTimeValue *dval = (static_cast<FdoDateTimeValue*>(dataval));
  
  
  return dval->GetDateTime();
}


double c_KgOraFeatureReaderInsert::GetDouble( const wchar_t *PropName )
{
   FdoPtr<FdoPropertyValue> propvalue;

  if( m_IsFirstReadNext || m_FeatureValues == NULL )
      throw FdoCommandException::Create( L"c_KgOraFeatureReaderInsert::GetDouble End of recordset!" );

  try
  {
      propvalue =  m_FeatureValues->GetItem( PropName );
      if( propvalue == NULL )
          throw FdoCommandException::Create( L"c_KgOraFeatureReaderInsert::GetDouble Property not found!" );
  }
  catch ( FdoException * e)
  {
    FDO_SAFE_RELEASE(e);
    throw FdoCommandException::Create( L"c_KgOraFeatureReaderInsert::GetDouble Property not found!" );
  }
  FdoPtr<FdoValueExpression> val = propvalue->GetValue();
  
  FdoDataValue* dataval = (dynamic_cast<FdoDataValue*>(val.p));
  
  if( !dataval || dataval->GetDataType() != FdoDataType_Double )
    throw FdoCommandException::Create( L"c_KgOraFeatureReaderInsert::GetDouble DataType not FdoDataType_Int64!" );

  FdoDoubleValue  *dval = (static_cast<FdoDoubleValue*>(dataval));
  
  
  return dval->GetDouble();
}

const wchar_t* c_KgOraFeatureReaderInsert::GetString( const wchar_t * PropName)
{
     FdoPtr<FdoPropertyValue> propvalue;

  if( m_IsFirstReadNext || m_FeatureValues == NULL )
      throw FdoCommandException::Create( L"c_KgOraFeatureReaderInsert::GetString End of recordset!" );

  try
  {
      propvalue =  m_FeatureValues->GetItem( PropName );
      if( propvalue == NULL )
          throw FdoCommandException::Create( L"c_KgOraFeatureReaderInsert::GetString Property not found!" );
  }
  catch ( FdoException * e)
  {
    FDO_SAFE_RELEASE(e);
    throw FdoCommandException::Create( L"c_KgOraFeatureReaderInsert::GetString Property not found!" );
  }
  FdoPtr<FdoValueExpression> val = propvalue->GetValue();
  
  FdoDataValue* dataval = (dynamic_cast<FdoDataValue*>(val.p));
  
  if( !dataval || dataval->GetDataType() != FdoDataType_String )
    throw FdoCommandException::Create( L"c_KgOraFeatureReaderInsert::GetString DataType not FdoDataType_Int64!" );

  FdoStringValue  *str = (static_cast<FdoStringValue*>(dataval));
  
  
  return str->GetString();
}

short c_KgOraFeatureReaderInsert::GetInt16( const wchar_t *PropName )
{
     FdoPtr<FdoPropertyValue> propvalue;

  if( m_IsFirstReadNext || m_FeatureValues == NULL )
      throw FdoCommandException::Create( L"c_KgOraFeatureReaderInsert::GetInt16 End of recordset!" );

  try
  {
      propvalue =  m_FeatureValues->GetItem( PropName );
      if( propvalue == NULL )
          throw FdoCommandException::Create( L"c_KgOraFeatureReaderInsert::GetInt16 Property not found!" );
  }
  catch ( FdoException *e )
  {
    FDO_SAFE_RELEASE(e);
    throw FdoCommandException::Create( L"c_KgOraFeatureReaderInsert::GetInt16 Property not found!" );
  }
  FdoPtr<FdoValueExpression> val = propvalue->GetValue();
  
  FdoDataValue* dataval = (dynamic_cast<FdoDataValue*>(val.p));
  
  if( !dataval || dataval->GetDataType() != FdoDataType_Int16 )
    throw FdoCommandException::Create( L"c_KgOraFeatureReaderInsert::GetInt16 DataType not FdoDataType_Int64!" );

  FdoInt16Value  *int16Value = (static_cast<FdoInt16Value*>(dataval));
  
  
  return int16Value->GetInt16();
}

int c_KgOraFeatureReaderInsert::GetInt32( const wchar_t *PropName )
{
    FdoPtr<FdoPropertyValue> propvalue;

  if( m_IsFirstReadNext || m_FeatureValues == NULL )
      throw FdoCommandException::Create( L"c_KgOraFeatureReaderInsert::GetInt32 End of recordset!" );

  try
  {
      propvalue =  m_FeatureValues->GetItem( PropName );
      if( propvalue == NULL )
          throw FdoCommandException::Create( L"c_KgOraFeatureReaderInsert::GetInt32 Property not found!" );
  }
  catch ( FdoException * e )
  {
    FDO_SAFE_RELEASE(e);
    throw FdoCommandException::Create( L"c_KgOraFeatureReaderInsert::GetInt32 Property not found!" );
  }
  FdoPtr<FdoValueExpression> val = propvalue->GetValue();
  
  FdoDataValue* dataval = (dynamic_cast<FdoDataValue*>(val.p));
  
  if( !dataval || dataval->GetDataType() != FdoDataType_Int32 )
    throw FdoCommandException::Create( L"c_KgOraFeatureReaderInsert::GetInt32 DataType not FdoDataType_Int64!" );

  FdoInt32Value  *int32Value = (static_cast<FdoInt32Value*>(dataval));
  
  
  return int32Value->GetInt32();
}

FdoInt64 c_KgOraFeatureReaderInsert::GetInt64( const wchar_t *PropName )
{
  FdoPtr<FdoPropertyValue> propvalue;

  if( m_IsFirstReadNext || m_FeatureValues == NULL )
      throw FdoCommandException::Create( L"c_KgOraFeatureReaderInsert::GetInt64 End of recordset!" );

  try
  {
      propvalue =  m_FeatureValues->GetItem( PropName );
      if( propvalue == NULL )
          throw FdoCommandException::Create( L"c_KgOraFeatureReaderInsert::GetInt64 Property not found!" );
  }
  catch ( FdoException *e )
  {
    FDO_SAFE_RELEASE(e);
    throw FdoCommandException::Create( L"c_KgOraFeatureReaderInsert::GetInt64 Property not found!" );
  }
  FdoPtr<FdoValueExpression> val = propvalue->GetValue();
  
  FdoDataValue* dataval = (dynamic_cast<FdoDataValue*>(val.p));
  
  if( !dataval || dataval->GetDataType() != FdoDataType_Int64 )
    throw FdoCommandException::Create( L"c_KgOraFeatureReaderInsert::GetInt64 DataType not FdoDataType_Int64!" );

  FdoInt64Value  *int64Value = (static_cast<FdoInt64Value*>(dataval));
  
  
  return int64Value->GetInt64();
  
}

float c_KgOraFeatureReaderInsert::GetSingle( const wchar_t *PropName )
{
     return (float)GetInt32( PropName );
}

bool c_KgOraFeatureReaderInsert::IsNull( const wchar_t *PropName )
{
    FdoPtr<FdoPropertyValue> propval;

    if( m_IsFirstReadNext || m_FeatureValues == NULL )
      throw FdoCommandException::Create( L"c_KgOraFeatureReaderInsert::IsNull End of recordset!" );

    try
    {
        propval =  m_FeatureValues->GetItem( PropName );
        if( propval == NULL )
            throw FdoCommandException::Create( L"c_KgOraFeatureReaderInsert::IsNull Property not found!" );
    }
    catch( FdoException *e )
    {
        FDO_SAFE_RELEASE(e);
        throw FdoCommandException::Create( L"c_KgOraFeatureReaderInsert::IsNull Property not found!" );
    }

    FdoPtr<FdoValueExpression> val = propval->GetValue();
    
    if( val.p == NULL )
        return true;

    
    return false;
}



FdoByteArray* c_KgOraFeatureReaderInsert::GetGeometry(const wchar_t* propertyName )
{
  throw FdoCommandException::Create(L"c_KgOraFeatureReaderInsert::GetGeometry unsupported!"); 
}

const FdoByte * c_KgOraFeatureReaderInsert::GetGeometry(const wchar_t* propertyName, FdoInt32 * count)
{
  throw FdoCommandException::Create(L"c_KgOraFeatureReaderInsert::GetGeometry unsupported!");
}

FdoIRaster* c_KgOraFeatureReaderInsert::GetRaster(const wchar_t* )
{
  throw FdoCommandException::Create(L"c_KgOraFeatureReaderInsert::GetRaster unsupported!");
}

bool c_KgOraFeatureReaderInsert::ReadNext( )
{
    bool ret = m_IsFirstReadNext;

    m_IsFirstReadNext = false;
    
    return ret;
}


void c_KgOraFeatureReaderInsert::Close( )
{

}
