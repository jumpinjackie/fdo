// 
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#ifndef _SdfImpScrollableFeatureReader_H_
#define _SdfImpScrollableFeatureReader_H_

#include "SDF/IScrollableFeatureReader.h"


template <class T>
class SdfImpScrollableFeatureReader : public SdfIScrollableFeatureReader
{

public:
    // constructs an SdfImpScrollableFeatureReader using the specified
    // reader. the ownership of the rdr paramter is transfered to this class.
	SdfImpScrollableFeatureReader( T *rdr ) { m_Reader = rdr; }

protected:
    // default destructor
	virtual ~SdfImpScrollableFeatureReader() { m_Reader->Release(); }

    //-------------------------------------------------------
    // FdoIDisposable implementation
    //-------------------------------------------------------

protected:
    // dispose this object
	virtual void Dispose() { delete this; }

public:

	virtual int Count() { return m_Reader->Count(); }

	virtual bool ReadFirst() { return m_Reader->ReadFirst(); }
	
	virtual bool ReadLast() { return m_Reader->ReadLast(); }

    virtual bool ReadNext() { return m_Reader->ReadNext(); }

    virtual bool ReadPrevious() { return m_Reader->ReadPrevious(); }

	virtual bool ReadAt(FdoPropertyValueCollection* key) { return m_Reader->ReadAt( key ); }

	virtual bool  ReadAtIndex( unsigned int recordindex ) { return m_Reader->ReadAtIndex( recordindex ); }

	unsigned int IndexOf(FdoPropertyValueCollection* key) { return m_Reader->IndexOf( key ); }

	// Feature Reader
	virtual FdoClassDefinition* GetClassDefinition() { return m_Reader->GetClassDefinition(); }

    virtual FdoInt32 GetDepth(){ return m_Reader->GetDepth(); }

    virtual const FdoByte * GetGeometry(FdoString* propertyName, FdoInt32 * count){ return m_Reader->GetGeometry(propertyName, count); }

    virtual FdoByteArray* GetGeometry(FdoString* propertyName){ return m_Reader->GetGeometry(propertyName); }

    virtual FdoIFeatureReader* GetFeatureObject(FdoString* propertyName) { return m_Reader->GetFeatureObject(propertyName); }

	// IReader
	virtual bool GetBoolean(FdoString* propertyName) { return m_Reader->GetBoolean(propertyName); }

    virtual FdoByte GetByte(FdoString* propertyName) { return m_Reader->GetByte(propertyName); }

    virtual FdoDateTime GetDateTime(FdoString* propertyName) { return m_Reader->GetDateTime(propertyName); }

    virtual double GetDouble(FdoString* propertyName) { return m_Reader->GetDouble(propertyName); }

    virtual FdoInt16 GetInt16(FdoString* propertyName) { return m_Reader->GetInt16(propertyName); }

    virtual FdoInt32 GetInt32(FdoString* propertyName) { return m_Reader->GetInt32(propertyName); }

    virtual FdoInt64 GetInt64(FdoString* propertyName) { return m_Reader->GetInt64(propertyName); }

    virtual float GetSingle(FdoString* propertyName) { return m_Reader->GetSingle(propertyName); }

    virtual FdoString* GetString(FdoString* propertyName) { return m_Reader->GetString(propertyName); }

    virtual FdoLOBValue* GetLOB(FdoString* propertyName) { return m_Reader->GetLOB(propertyName); }

    virtual FdoIStreamReader* GetLOBStreamReader(const wchar_t* propertyName ) { return m_Reader->GetLOBStreamReader(propertyName); }

    virtual bool IsNull(FdoString* propertyName) { return m_Reader->IsNull(propertyName); }

    virtual FdoIRaster* GetRaster(FdoString* propertyName) { return m_Reader->GetRaster(propertyName); }

    virtual void Close() { return m_Reader->Close(); }

private:
    T *m_Reader;
};

#endif
