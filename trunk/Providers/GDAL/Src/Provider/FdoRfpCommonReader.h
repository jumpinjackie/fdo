/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
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
 *
 * Revision Control Modification History
 *
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpCommonReader.h $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */

#ifndef FDORFPCOMMONREADER_H
#define FDORFPCOMMONREADER_H

#ifdef _WIN32
#pragma once
#endif //_WIN32

#include "FdoRfpQueryResult.h"
#include "FdoRfpRaster.h"

//
// this class is the common implementation of IFeatureReader and IDataReader
//
template<typename READER> 
class FdoRfpCommonReader : public READER
{
//
// data members
//
protected:
	FdoPtr<FdoRfpQueryResult>		m_queryResult;
	// Record cursor
	FdoInt32						m_cursor;

// protected constructor
protected:
	FdoRfpCommonReader(const FdoPtr<FdoRfpQueryResult>& queryResult) : m_queryResult(queryResult), m_cursor(-1) {}
	void Dispose() { delete this; }

//
// exposed functions
//
public:
    /// <summary>Gets the Boolean value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_Boolean or an
    /// exception is thrown.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the Boolean value</returns> 
    virtual bool GetBoolean(FdoString* propertyName)
	{
		_throwException(L"Boolean");
		return false;
	}

    /// <summary>Gets the byte value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_Byte or an
    /// exception is thrown.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the byte value.</returns> 
    virtual FdoByte GetByte(FdoString* propertyName)
	{
		_throwException(L"Byte");
		return FdoByte(0);
	}

    /// <summary>Gets the date time value of the specified property. No conversion
    /// is performed, thus the property must be FdoDataType_DateTime or
    /// an exception is thrown.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the date and time value.</returns> 
    virtual FdoDateTime GetDateTime(FdoString* propertyName)
	{
		_throwException(L"DataTime");
		return FdoDateTime();
	}

    /// <summary>Gets the double-precision floating point value of the specified property.
    /// No conversion is performed, thus the property must be of type
    /// Double or an exception is thrown.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the double value.</returns> 
    virtual double GetDouble(FdoString* propertyName)
	{
		_throwException(L"double");
		return 0.0;
	}

    /// <summary>Gets the signed 16-bit integer value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_Int16 or an
    /// exception is thrown.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the FdoInt16 value.</returns> 
    virtual FdoInt16 GetInt16(FdoString* propertyName)
	{
		_throwException(L"Int16");
		return 0;
	}

    /// <summary>Gets the signed 32-bit integer value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_Int32 or an
    /// exception is thrown.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the FdoInt32 value.</returns> 
    virtual FdoInt32 GetInt32(FdoString* propertyName)
	{
		_throwException(L"Int32");
		return 0;
	}

    /// <summary>Gets the signed 64-bit integer value of the specified property. No conversion
    /// is performed, thus the property must be FdoDataType_Int64 or an
    /// exception is thrown.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the FdoInt64 value.</returns> 
    virtual FdoInt64 GetInt64(FdoString* propertyName)
	{
		_throwException(L"Int64");
		return 0;
	}

    /// <summary>Gets the single-precision floating point value of the specified property.
    /// No conversion is performed, thus the property must be FdoDataType_Single
	/// or an exception is thrown.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the single value</returns> 
    virtual float GetSingle(FdoString* propertyName)
	{
		_throwException(L"Single");
		return 0.0;
	}

    /// <summary>Gets the string value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_String or an
    /// exception is thrown.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the string value.</returns> 
    virtual FdoString* GetString(FdoString* propertyName);

    /// <summary>Gets a LOBValue reference. The LOB is fully read in and data available.
    /// Because no conversion is performed, the property must be FdoDataType_BLOB or
    /// FdoDataType_CLOB etc. (a LOB type)
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the reference to LOBValue</returns> 
    virtual FdoLOBValue* GetLOB(FdoString* propertyName);

    /// <summary>Gets a reference of the specified LOB property as a FdoBLOBStreamReader or
    /// FdoCLOBStreamReader etc. to allow reading in blocks of data.
    /// Because no conversion is performed, the property must be FdoDataType_BLOB 
    /// or FdoDataType_CLOB etc. (a LOB type)
    /// Cast the FdoIStreamReader to the appropiate LOB Stream Reader.
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns a reference to a LOB stream reader</returns> 
    virtual FdoIStreamReader* GetLOBStreamReader(const wchar_t* propertyName );

    /// <summary>Returns true if the value of the specified property is null.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns true if the value is null.</returns> 
    virtual bool IsNull(FdoString* propertyName);

    /// <summary>Gets the geometry value of the specified property as a byte array
    /// in AGF format. No conversion is performed, thus the property
    /// must be of Geometric type or an exception is thrown.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the AGF byte array value.</returns> 
    virtual FdoByteArray* GetGeometry(FdoString* propertyName);

    /// <summary>Gets the geometry value of the specified property as a byte array in 
    /// AGF format. Because no conversion is performed, the property must be
    /// of Geometric type; otherwise, an exception is thrown. 
    /// This method is a language-specific performance optimization that returns a
    /// pointer to the array data, rather than to an object that encapsulates
    /// the array.  The array's memory area is only guaranteed to be valid
    /// until a call to ReadNext() or Close(), or the disposal of this reader
    /// object.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <param name="count">Output the number of bytes in the array.</param> 
    /// <returns>Returns a pointer to the byte array in AGF format.</returns> 
    virtual const FdoByte * GetGeometry(FdoString* propertyName, FdoInt32 * count);

    /// <summary>Gets the raster object of the specified property.
    /// Because no conversion is performed, the property must be
    /// of Raster type; otherwise, an exception is thrown.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the raster object.</returns> 
    virtual FdoIRaster* GetRaster(FdoString* propertyName);

    /// <summary>Advances the reader to the next item. The default position of the
    /// reader is prior to the first item. Thus, you must call ReadNext
    /// to begin accessing any data.</summary>
    /// <returns>Returns true if there is a next item.</returns> 
    virtual bool ReadNext();

    /// <summary>Closes the FdoIDataReader object, freeing any resources it may be holding.</summary>
    /// <returns>Returns nothing</returns> 
    virtual void Close();

//
// Internally used helper functions
//
protected:
	// Throw out an exception if the requested typeName does not exist
	void _throwException(FdoString* typeName);

	void _validateCursor();

	// get column index according to the property name
	int _getColumnIndex(FdoString* propertyName);

};

template<typename READER>
FdoByteArray* FdoRfpCommonReader<READER>::GetGeometry(FdoString* propertyName)
{
	_throwException(L"Geometry");
	return NULL;
}

template<typename READER>
const FdoByte * FdoRfpCommonReader<READER>::GetGeometry(FdoString* propertyName, FdoInt32 * count)
{
	_throwException(L"Geometry");
	return NULL;
}

template<typename READER>
FdoIRaster* FdoRfpCommonReader<READER>::GetRaster(FdoString* propertyName)
{
    if (propertyName == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_2_BADPARAMETER, "Bad parameter to method."));
	_validateCursor();
	int col = _getColumnIndex(propertyName);
	if (m_queryResult->propertyTypes[col] != PropertyType_Raster)
		throw FdoCommandException::Create(NlsMsgGet2(GRFP_58_TYPE_MISMATCH, "The requested property '%1$ls' is not a '%2$ls'.", propertyName, L"Raster"));

    FdoRfpRect* clippingBounds = m_queryResult->clippingBounds[col];
	// copy the source rasters and add the clipping bounds
    FdoPtr<FdoRfpGeoRasterCollection> src = m_queryResult->resultSet->GetItem(m_cursor);
    FdoPtr<FdoIRaster> rv = new FdoRfpRaster(src, clippingBounds);
	FdoSize height = m_queryResult->heightValues[col];
	FdoSize width = m_queryResult->widthValues[col];
	if (height && width)
	{
		rv->SetImageXSize(width);
		rv->SetImageYSize(height);
	}

	return FDO_SAFE_ADDREF(rv.p);
}


template<typename READER>
bool FdoRfpCommonReader<READER>::ReadNext()
{
	FdoInt32 count = (FdoInt32)m_queryResult->resultSet->GetCount();
	if (m_cursor + 1 == count)
		return false;
	m_cursor++;
	return true;
}


template<typename READER>
void FdoRfpCommonReader<READER>::Close()
{
	m_queryResult = NULL;
	m_cursor = -1;
}


template<typename READER>
FdoLOBValue* FdoRfpCommonReader<READER>::GetLOB(FdoString* propertyName)
{
    if (propertyName == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_2_BADPARAMETER, "Bad parameter to method."));
	FdoIStreamReader* streamReader = GetLOBStreamReader(propertyName);
	FdoPtr<FdoBLOBStreamReader> blobReader = static_cast<FdoBLOBStreamReader*>(streamReader);

	FdoPtr<FdoByteArray> buffer;

	blobReader->ReadNext(buffer.p);
		
	return FdoBLOBValue::Create(buffer);
}

template<typename READER>
FdoIStreamReader* FdoRfpCommonReader<READER>::GetLOBStreamReader(const wchar_t* propertyName )
{
	// first get the IRaster
	FdoPtr<FdoIRaster> raster = GetRaster(propertyName);

	return raster->GetStreamReader();
}


template<typename READER>
bool FdoRfpCommonReader<READER>::IsNull(FdoString* propertyName)
{
	try {
		_getColumnIndex(propertyName);
	} catch (FdoException* e) {
		FDO_SAFE_RELEASE(e);
		return true;
	}
	return false;
}

template<typename READER>
void FdoRfpCommonReader<READER>::_throwException(FdoString* typeName)
{
	throw FdoCommandException::Create(NlsMsgGet1(GRFP_47_PROPERTY_TYPE_NOT_SUPPORTED, "Property type '%1$ls' not supported.", typeName));
}

template<typename READER>
void FdoRfpCommonReader<READER>::_validateCursor()
{
	if (m_cursor == -1)
		throw FdoCommandException::Create(NlsMsgGet(GRFP_57_READNEXT_MUST_BE_CALLED, "ReadNext must be called before fetching data."));
}

template<typename READER>
int FdoRfpCommonReader<READER>::_getColumnIndex(FdoString* propertyName)
{
	int numColumns = (int)m_queryResult->identifiers.size();
	for (int col = 0; col < numColumns; col++)
	{
		std::vector<FdoStringP>& ids = *m_queryResult->identifiers[col];
		for (std::vector<FdoStringP>::iterator it = ids.begin(); it != ids.end(); it++)
			if (STRCASEEQ(propertyName, *it))
				return col;
	}
	// not found, throw out exception
	throw FdoCommandException::Create(NlsMsgGet1(GRFP_56_ILLEGAL_REQUESTED_PROPERTY, "Illegal requested property '%1$ls'.", propertyName));
	return -1;

}



template<typename READER>
FdoString* FdoRfpCommonReader<READER>::GetString(FdoString* propertyName)
{
    if (propertyName == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_2_BADPARAMETER, "Bad parameter to method."));

	_validateCursor();
	int col = _getColumnIndex(propertyName);
	FdoPtr<FdoRfpGeoRasterCollection> rasters = m_queryResult->resultSet->GetItem(m_cursor);
	if (m_queryResult->propertyTypes[col] != PropertyType_ID ||
		rasters->GetCount() > 1)
		throw FdoCommandException::Create(NlsMsgGet2(GRFP_58_TYPE_MISMATCH, "The requested property '%1$ls' is not a '%2$ls'.", propertyName, L"String"));
    return FdoRfpGeoRasterP(rasters->GetItem(0))->GetId();
}

#endif
