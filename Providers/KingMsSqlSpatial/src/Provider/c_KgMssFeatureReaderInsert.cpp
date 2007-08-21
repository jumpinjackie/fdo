/*
* Copyright (C) 2006  Haris Kurtagic
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

#include "c_KgMssFeatureReaderInsert.h"




c_KgMssFeatureReaderInsert::c_KgMssFeatureReaderInsert( FdoPropertyValueCollection* FeatureValues, FdoClassDefinition* ClassDef )
{
  
  m_FeatureValues = FeatureValues;
  FDO_SAFE_ADDREF(m_FeatureValues.p);
    
  m_IsFirstReadNext = true;
  
  m_ClassDef = ClassDef;
  FDO_SAFE_ADDREF(m_ClassDef.p);
}

c_KgMssFeatureReaderInsert::~c_KgMssFeatureReaderInsert()
{
  
}

void c_KgMssFeatureReaderInsert::Dispose()
{
    delete this;
}

FdoClassDefinition* c_KgMssFeatureReaderInsert::GetClassDefinition()
{
  return FDO_SAFE_ADDREF(m_ClassDef.p);

}

int c_KgMssFeatureReaderInsert::GetDepth()
{
    throw FdoCommandException::Create(L"c_KgMssFeatureReaderInsert::GetDepth unsupported!");
}

FdoLOBValue* c_KgMssFeatureReaderInsert::GetLOB(const wchar_t* propertyName )
{
  throw FdoCommandException::Create(L"c_KgMssFeatureReaderInsert::GetLOB unsupported!");
}

FdoIStreamReader* c_KgMssFeatureReaderInsert::GetLOBStreamReader(const wchar_t* propertyName )
{
  throw FdoCommandException::Create(L"c_KgMssFeatureReaderInsert::GetLOBStreamReader unsupported!");
}

FdoIFeatureReader* c_KgMssFeatureReaderInsert::GetFeatureObject( const wchar_t* propertyName )
{
  throw FdoCommandException::Create(L"c_KgMssFeatureReaderInsert::GetFeatureObject unsupported!");
}

bool c_KgMssFeatureReaderInsert::GetBoolean( const wchar_t *PropName )
{
    return (GetInt32( PropName ) != 0 );
}

FdoByte c_KgMssFeatureReaderInsert::GetByte( const wchar_t *PropName )
{
    return (FdoByte)GetInt32( PropName );
}


FdoDateTime c_KgMssFeatureReaderInsert::GetDateTime( const wchar_t *PropName )
{
     FdoPtr<FdoPropertyValue> propvalue = NULL;

  if( m_IsFirstReadNext || m_FeatureValues == NULL )
      throw FdoCommandException::Create( L"c_KgMssFeatureReaderInsert::GetDateTime End of recordset!" );

  try
  {
      propvalue =  m_FeatureValues->GetItem( PropName );
      if( propvalue == NULL )
          throw FdoCommandException::Create( L"c_KgMssFeatureReaderInsert::GetDouble Property not found!" );
  }
  catch ( FdoException * e)
  {
    FDO_SAFE_RELEASE(e);
    throw FdoCommandException::Create( L"c_KgMssFeatureReaderInsert::GetDouble Property not found!" );
  }
  FdoPtr<FdoValueExpression> val = propvalue->GetValue();
  
  FdoDataValue* dataval = (dynamic_cast<FdoDataValue*>(val.p));
  
  if( !dataval && dataval->GetDataType() != FdoDataType_DateTime)
    throw FdoCommandException::Create( L"c_KgMssFeatureReaderInsert::GetDouble DataType not FdoDataType_Int64!" );

  FdoDateTimeValue *dval = (static_cast<FdoDateTimeValue*>(dataval));
  
  
  return dval->GetDateTime();
}


double c_KgMssFeatureReaderInsert::GetDouble( const wchar_t *PropName )
{
   FdoPtr<FdoPropertyValue> propvalue = NULL;

  if( m_IsFirstReadNext || m_FeatureValues == NULL )
      throw FdoCommandException::Create( L"c_KgMssFeatureReaderInsert::GetDouble End of recordset!" );

  try
  {
      propvalue =  m_FeatureValues->GetItem( PropName );
      if( propvalue == NULL )
          throw FdoCommandException::Create( L"c_KgMssFeatureReaderInsert::GetDouble Property not found!" );
  }
  catch ( FdoException * e)
  {
    FDO_SAFE_RELEASE(e);
    throw FdoCommandException::Create( L"c_KgMssFeatureReaderInsert::GetDouble Property not found!" );
  }
  FdoPtr<FdoValueExpression> val = propvalue->GetValue();
  
  FdoDataValue* dataval = (dynamic_cast<FdoDataValue*>(val.p));
  
  if( !dataval || dataval->GetDataType() != FdoDataType_Double )
    throw FdoCommandException::Create( L"c_KgMssFeatureReaderInsert::GetDouble DataType not FdoDataType_Int64!" );

  FdoDoubleValue  *dval = (static_cast<FdoDoubleValue*>(dataval));
  
  
  return dval->GetDouble();
}

const wchar_t* c_KgMssFeatureReaderInsert::GetString( const wchar_t * PropName)
{
     FdoPtr<FdoPropertyValue> propvalue = NULL;

  if( m_IsFirstReadNext || m_FeatureValues == NULL )
      throw FdoCommandException::Create( L"c_KgMssFeatureReaderInsert::GetString End of recordset!" );

  try
  {
      propvalue =  m_FeatureValues->GetItem( PropName );
      if( propvalue == NULL )
          throw FdoCommandException::Create( L"c_KgMssFeatureReaderInsert::GetString Property not found!" );
  }
  catch ( FdoException * e)
  {
    FDO_SAFE_RELEASE(e);
    throw FdoCommandException::Create( L"c_KgMssFeatureReaderInsert::GetString Property not found!" );
  }
  FdoPtr<FdoValueExpression> val = propvalue->GetValue();
  
  FdoDataValue* dataval = (dynamic_cast<FdoDataValue*>(val.p));
  
  if( !dataval || dataval->GetDataType() != FdoDataType_String )
    throw FdoCommandException::Create( L"c_KgMssFeatureReaderInsert::GetString DataType not FdoDataType_Int64!" );

  FdoStringValue  *str = (static_cast<FdoStringValue*>(dataval));
  
  
  return str->GetString();
}

short c_KgMssFeatureReaderInsert::GetInt16( const wchar_t *PropName )
{
     FdoPtr<FdoPropertyValue> propvalue = NULL;

  if( m_IsFirstReadNext || m_FeatureValues == NULL )
      throw FdoCommandException::Create( L"c_KgMssFeatureReaderInsert::GetInt16 End of recordset!" );

  try
  {
      propvalue =  m_FeatureValues->GetItem( PropName );
      if( propvalue == NULL )
          throw FdoCommandException::Create( L"c_KgMssFeatureReaderInsert::GetInt16 Property not found!" );
  }
  catch ( FdoException *e )
  {
    FDO_SAFE_RELEASE(e);
    throw FdoCommandException::Create( L"c_KgMssFeatureReaderInsert::GetInt16 Property not found!" );
  }
  FdoPtr<FdoValueExpression> val = propvalue->GetValue();
  
  FdoDataValue* dataval = (dynamic_cast<FdoDataValue*>(val.p));
  
  if( !dataval || dataval->GetDataType() != FdoDataType_Int16 )
    throw FdoCommandException::Create( L"c_KgMssFeatureReaderInsert::GetInt16 DataType not FdoDataType_Int64!" );

  FdoInt16Value  *int16Value = (static_cast<FdoInt16Value*>(dataval));
  
  
  return int16Value->GetInt16();
}

int c_KgMssFeatureReaderInsert::GetInt32( const wchar_t *PropName )
{
    FdoPtr<FdoPropertyValue> propvalue = NULL;

  if( m_IsFirstReadNext || m_FeatureValues == NULL )
      throw FdoCommandException::Create( L"c_KgMssFeatureReaderInsert::GetInt32 End of recordset!" );

  try
  {
      propvalue =  m_FeatureValues->GetItem( PropName );
      if( propvalue == NULL )
          throw FdoCommandException::Create( L"c_KgMssFeatureReaderInsert::GetInt32 Property not found!" );
  }
  catch ( FdoException * e )
  {
    FDO_SAFE_RELEASE(e);
    throw FdoCommandException::Create( L"c_KgMssFeatureReaderInsert::GetInt32 Property not found!" );
  }
  FdoPtr<FdoValueExpression> val = propvalue->GetValue();
  
  FdoDataValue* dataval = (dynamic_cast<FdoDataValue*>(val.p));
  
  if( !dataval || dataval->GetDataType() != FdoDataType_Int32 )
    throw FdoCommandException::Create( L"c_KgMssFeatureReaderInsert::GetInt32 DataType not FdoDataType_Int64!" );

  FdoInt32Value  *int32Value = (static_cast<FdoInt32Value*>(dataval));
  
  
  return int32Value->GetInt32();
}

FdoInt64 c_KgMssFeatureReaderInsert::GetInt64( const wchar_t *PropName )
{
  FdoPtr<FdoPropertyValue> propvalue = NULL;

  if( m_IsFirstReadNext || m_FeatureValues == NULL )
      throw FdoCommandException::Create( L"c_KgMssFeatureReaderInsert::GetInt64 End of recordset!" );

  try
  {
      propvalue =  m_FeatureValues->GetItem( PropName );
      if( propvalue == NULL )
          throw FdoCommandException::Create( L"c_KgMssFeatureReaderInsert::GetInt64 Property not found!" );
  }
  catch ( FdoException *e )
  {
    FDO_SAFE_RELEASE(e);
    throw FdoCommandException::Create( L"c_KgMssFeatureReaderInsert::GetInt64 Property not found!" );
  }
  FdoPtr<FdoValueExpression> val = propvalue->GetValue();
  
  FdoDataValue* dataval = (dynamic_cast<FdoDataValue*>(val.p));
  
  if( !dataval || dataval->GetDataType() != FdoDataType_Int64 )
    throw FdoCommandException::Create( L"c_KgMssFeatureReaderInsert::GetInt64 DataType not FdoDataType_Int64!" );

  FdoInt64Value  *int64Value = (static_cast<FdoInt64Value*>(dataval));
  
  
  return int64Value->GetInt64();
  
}

float c_KgMssFeatureReaderInsert::GetSingle( const wchar_t *PropName )
{
     return (float)GetInt32( PropName );
}

bool c_KgMssFeatureReaderInsert::IsNull( const wchar_t *PropName )
{
    FdoPtr<FdoPropertyValue> propval;

    if( m_IsFirstReadNext || m_FeatureValues == NULL )
      throw FdoCommandException::Create( L"c_KgMssFeatureReaderInsert::IsNull End of recordset!" );

    try
    {
        propval =  m_FeatureValues->GetItem( PropName );
        if( propval == NULL )
        {
          FdoStringP msg;
          msg = msg.Format(L"c_KgMssFeatureReaderInsert::IsNull Property not found (%s)!",PropName); 
            throw FdoCommandException::Create( msg );
        }
    }
    catch( FdoException *e )
    {
        throw e;
    }

    FdoPtr<FdoValueExpression> val = propval->GetValue();
    
    if( val.p == NULL )
        return true;

    
    return false;
}



FdoByteArray* c_KgMssFeatureReaderInsert::GetGeometry(const wchar_t* propertyName )
{
  throw FdoCommandException::Create(L"c_KgMssFeatureReaderInsert::GetGeometry unsupported!"); 
}

const FdoByte * c_KgMssFeatureReaderInsert::GetGeometry(const wchar_t* propertyName, FdoInt32 * count)
{
  throw FdoCommandException::Create(L"c_KgMssFeatureReaderInsert::GetGeometry unsupported!");
}

FdoIRaster* c_KgMssFeatureReaderInsert::GetRaster(const wchar_t* )
{
  throw FdoCommandException::Create(L"c_KgMssFeatureReaderInsert::GetRaster unsupported!");
}

bool c_KgMssFeatureReaderInsert::ReadNext( )
{
    bool ret = m_IsFirstReadNext;

    m_IsFirstReadNext = false;
    
    return ret;
}


void c_KgMssFeatureReaderInsert::Close( )
{

}