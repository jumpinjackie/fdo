
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

#ifndef _c_KgOraFeatureReaderInsert_h
#define _c_KgOraFeatureReaderInsert_h
#include "FdoDefaultFeatureReader.h"


class c_KgOraFeatureReaderInsert : public FdoDefaultFeatureReader
{

public:
      c_KgOraFeatureReaderInsert( FdoPropertyValueCollection * FeatureValues,FdoClassDefinition* ClassDef );
      

public:
    virtual ~c_KgOraFeatureReaderInsert();
    virtual void Dispose();

public:

  FDOKGORA_API virtual FdoClassDefinition* GetClassDefinition();
  FDOKGORA_API virtual int GetDepth();
  FDOKGORA_API virtual bool     GetBoolean( const wchar_t *propertyName );
  FDOKGORA_API virtual FdoByte  GetByte( const wchar_t *propertyName );
  FDOKGORA_API virtual double   GetDouble(const wchar_t* propertyName);
  FDOKGORA_API virtual short    GetInt16( const wchar_t *propertyName );
  FDOKGORA_API virtual int      GetInt32( const wchar_t *propertyName );
  FDOKGORA_API virtual FdoInt64 GetInt64( const wchar_t *propertyName );
  FDOKGORA_API virtual float    GetSingle( const wchar_t *propertyName );
  FDOKGORA_API virtual const wchar_t* GetString( const wchar_t *propertyName );
  FDOKGORA_API virtual FdoLOBValue* GetLOB(const wchar_t* propertyName );
  FDOKGORA_API virtual FdoIStreamReader* GetLOBStreamReader(const wchar_t* propertyName );
  FDOKGORA_API virtual bool     IsNull( const wchar_t *propertyName );
  FDOKGORA_API virtual FdoIFeatureReader* GetFeatureObject(const wchar_t* propertyName);
  FDOKGORA_API virtual FdoByteArray* GetGeometry(const wchar_t* propertyName);
  FDOKGORA_API virtual const FdoByte * GetGeometry(const wchar_t* propertyName, FdoInt32 * count);
  FDOKGORA_API virtual FdoIRaster* GetRaster(const wchar_t* propertyName);
  FDOKGORA_API virtual bool     ReadNext( );
  FDOKGORA_API virtual void     Close();

  FDOKGORA_API virtual FdoDateTime GetDateTime(const wchar_t *propertyName );


protected:
  FdoPtr<FdoPropertyValueCollection> m_FeatureValues;
  FdoPtr<FdoClassDefinition> m_ClassDef;
  
  bool m_IsFirstReadNext;
  
};

#endif // FdoRdbmsFeatureInfoReader_H

