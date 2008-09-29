
/*
* Copyright (C) 2007  Haris Kurtagic
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

#ifndef _c_KgMssFeatureReaderInsert_h
#define _c_KgMssFeatureReaderInsert_h



class c_KgMssFeatureReaderInsert : public FdoIFeatureReader
{

public:
      c_KgMssFeatureReaderInsert( FdoPropertyValueCollection * FeatureValues,FdoClassDefinition* ClassDef );
      

public:
    virtual ~c_KgMssFeatureReaderInsert();
    virtual void Dispose();

public:

  FDOKGMSS_API virtual FdoClassDefinition* GetClassDefinition();
  FDOKGMSS_API virtual int GetDepth();
  FDOKGMSS_API virtual bool     GetBoolean( const wchar_t *propertyName );
  FDOKGMSS_API virtual FdoByte  GetByte( const wchar_t *propertyName );
  FDOKGMSS_API virtual double   GetDouble(const wchar_t* propertyName);
  FDOKGMSS_API virtual short    GetInt16( const wchar_t *propertyName );
  FDOKGMSS_API virtual int      GetInt32( const wchar_t *propertyName );
  FDOKGMSS_API virtual FdoInt64 GetInt64( const wchar_t *propertyName );
  FDOKGMSS_API virtual float    GetSingle( const wchar_t *propertyName );
  FDOKGMSS_API virtual const wchar_t* GetString( const wchar_t *propertyName );
  FDOKGMSS_API virtual FdoLOBValue* GetLOB(const wchar_t* propertyName );
  FDOKGMSS_API virtual FdoIStreamReader* GetLOBStreamReader(const wchar_t* propertyName );
  FDOKGMSS_API virtual bool     IsNull( const wchar_t *propertyName );
  FDOKGMSS_API virtual FdoIFeatureReader* GetFeatureObject(const wchar_t* propertyName);
  FDOKGMSS_API virtual FdoByteArray* GetGeometry(const wchar_t* propertyName);
  FDOKGMSS_API virtual const FdoByte * GetGeometry(const wchar_t* propertyName, FdoInt32 * count);
  FDOKGMSS_API virtual FdoIRaster* GetRaster(const wchar_t* propertyName);
  FDOKGMSS_API virtual bool     ReadNext( );
  FDOKGMSS_API virtual void     Close();

  FDOKGMSS_API virtual FdoDateTime GetDateTime(const wchar_t *propertyName );


protected:
  FdoPtr<FdoPropertyValueCollection> m_FeatureValues;
  FdoPtr<FdoClassDefinition> m_ClassDef;
  
  bool m_IsFirstReadNext;
  
};

#endif // FdoRdbmsFeatureInfoReader_H

