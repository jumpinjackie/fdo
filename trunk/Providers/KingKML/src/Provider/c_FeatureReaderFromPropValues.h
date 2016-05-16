/*
* Copyright (C) 2010  SL-King d.o.o
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


#ifndef _c_FeatureReaderFromPropValues_h
#define _c_FeatureReaderFromPropValues_h

class c_FeatureReaderFromPropValues : public FdoIFeatureReader
{
public:
  c_FeatureReaderFromPropValues(FdoClassDefinition*,FdoPropertyValueCollection* PropValues);
  ~c_FeatureReaderFromPropValues(void);
  
  virtual void Dispose();
  
  
  virtual FdoString* GetPropertyName(FdoInt32 index);

  virtual FdoInt32 GetPropertyIndex(FdoString* propertyName);
  
  virtual FdoClassDefinition* GetClassDefinition();
  virtual bool ReadNext();
  virtual void Close();
  virtual FdoBoolean IsNull(FdoInt32 index);
  virtual bool IsNull(FdoString* propertyName);
  virtual FdoInt32 GetDepth() { return 0; }
  
  virtual FdoIRaster* GetRaster(FdoInt32 index);
  virtual FdoIRaster* GetRaster(FdoString* propertyName);
  virtual FdoIFeatureReader* GetFeatureObject(FdoInt32 index);
  virtual FdoIFeatureReader* GetFeatureObject( FdoString* propertyName );
  virtual FdoIStreamReader* GetLOBStreamReader( FdoInt32 index );
  virtual FdoIStreamReader* GetLOBStreamReader( FdoString* propertyName );
  virtual FdoLOBValue* GetLOB( FdoInt32 index );
  virtual FdoLOBValue* GetLOB( FdoString* propertyName );

  
  virtual FdoBoolean GetBoolean(FdoInt32 index);
  virtual FdoBoolean GetBoolean(FdoString* propertyName);
  virtual FdoByte GetByte(FdoInt32 index);
  virtual FdoByte GetByte(FdoString* propertyName);
  virtual FdoDateTime GetDateTime(FdoInt32 index);
  virtual FdoDateTime GetDateTime(FdoString* propertyName);
  virtual FdoDouble GetDouble(FdoInt32 index);
  virtual FdoDouble GetDouble(FdoString* propertyName);
  virtual FdoInt16 GetInt16(FdoInt32 index);
  virtual FdoInt16 GetInt16(FdoString* propertyName);
  virtual FdoInt32 GetInt32( FdoInt32 index );
  virtual FdoInt32 GetInt32( FdoString* propertyName );
  virtual FdoInt64 GetInt64( FdoInt32 index );
  virtual FdoInt64 GetInt64( FdoString* propertyName );
  virtual float GetSingle( FdoInt32 index );
  virtual float GetSingle( FdoString* propertyName );
  virtual FdoString* GetString( FdoInt32 index );
  virtual FdoString* GetString( FdoString* propertyName );
  
  virtual FdoByteArray* GetGeometry(FdoInt32 index);
  virtual FdoByteArray* GetGeometry( FdoString* propertyName);
  
  virtual const FdoByte * GetGeometry(FdoInt32 index, FdoInt32* count);
  virtual const FdoByte * GetGeometry(FdoString* propertyName, FdoInt32* count);

protected:
  FdoPtr<FdoPropertyValueCollection> m_PropValues;
  FdoPtr<FdoClassDefinition> m_ClassDef;
  
  int m_RowCount;
  
  FdoPtr<FdoByteArray> m_Geom;
  
protected:
  FdoLiteralValue* GetLiteralValue( FdoInt32 index );  
  FdoDataValue* GetDataValue( FdoInt32 index );
  
};

#endif