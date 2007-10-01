// 
//  Copyright (C) 2004-2008  Autodesk, Inc.
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
#ifndef FdoExpressionEngineUtilFeatureReader_H
#define FdoExpressionEngineUtilFeatureReader_H

#include <FdoExpressionEngine.h>


//A generic feature reader. The underlying feature reader and the ExpressionEngine
// are used to get the value of a property, computed identifiers and expressions
class FdoExpressionEngineUtilFeatureReader : public FdoIFeatureReader
{
public:

    FdoExpressionEngineUtilFeatureReader (FdoClassDefinition* classDef, FdoIFeatureReader* reader, FdoFilter *filter, FdoIdentifierCollection* selectedIds, FdoExpressionEngineFunctionCollection *userDefinedFunctions);

protected:
	virtual void Dispose();
    FdoExpressionEngineUtilFeatureReader() {};  // dummy 0-arg constructor to please FdoPtr::operator->
    virtual ~FdoExpressionEngineUtilFeatureReader();

public:

    //------------------------------------------------------------
    /// FdoIReader implementation
    //------------------------------------------------------------

    /// \brief
    /// Gets the Boolean value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_Boolean or an
    /// exception is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the Boolean value
    /// 
    virtual bool GetBoolean(FdoString* propertyName);

    /// \brief
    /// Gets the byte value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_Byte or an
    /// exception is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the byte value.
    /// 

    virtual FdoByte GetByte(FdoString* propertyName);

    /// \brief
    /// Gets the date time value of the specified property. No conversion
    /// is performed, thus the property must be FdoDataType_DateTime or
    /// an exception is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the date and time value.
    /// 
    virtual FdoDateTime GetDateTime(FdoString* propertyName);

    /// \brief
    /// Gets the double-precision floating point value of the specified property.
    /// No conversion is performed, thus the property must be of type
    /// Double or an exception is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the double value.
    /// 
    virtual double GetDouble(FdoString* propertyName);

    /// \brief
    /// Gets the signed 16-bit integer value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_Int16 or an
    /// exception is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the FdoInt16 value.
    /// 
    virtual FdoInt16 GetInt16(FdoString* propertyName);

    /// \brief
    /// Gets the signed 32-bit integer value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_Int32 or an
    /// exception is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the FdoInt32 value.
    /// 
    virtual FdoInt32 GetInt32(FdoString* propertyName);

    /// \brief
    /// Gets the signed 64-bit integer value of the specified property. No conversion
    /// is performed, thus the property must be FdoDataType_Int64 or an
    /// exception is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the FdoInt64 value.
    /// 
    virtual FdoInt64 GetInt64(FdoString* propertyName);

    /// \brief
    /// Gets the single-precision floating point value of the specified property.
    /// No conversion is performed, thus the property must be FdoDataType_Single
    /// or an exception is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the single value
    /// 
    virtual float GetSingle(FdoString* propertyName);

    /// \brief
    /// Gets the string value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_String or an
    /// exception is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the string value.
    /// 
    virtual FdoString* GetString(FdoString* propertyName);

    /// \brief
    /// Gets a LOBValue reference. The LOB is fully read in and data available.
    /// Because no conversion is performed, the property must be FdoDataType_BLOB or
    /// FdoDataType_CLOB etc. (a LOB type)
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the reference to LOBValue
    /// 
    virtual FdoLOBValue* GetLOB(FdoString* propertyName);

    /// \brief
    /// Gets a reference of the specified LOB property as a FdoBLOBStreamReader or
    /// FdoCLOBStreamReader etc. to allow reading in blocks of data.
    /// Because no conversion is performed, the property must be FdoDataType_BLOB 
    /// or FdoDataType_CLOB etc. (a LOB type)
    /// Cast the FdoIStreamReader to the appropiate LOB Stream Reader.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns a reference to a LOB stream reader
    /// 
    virtual FdoIStreamReader* GetLOBStreamReader(const wchar_t* propertyName );

    /// \brief
    /// Returns true if the value of the specified property is null.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns true if the value is null.
    /// 
    virtual bool IsNull(FdoString* propertyName);

    /// \brief
    /// Gets the geometry value of the specified property as a byte array
    /// in FGF format. No conversion is performed, thus the property
    /// must be of Geometric type or an exception is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the FGF byte array value.
    /// 
    virtual FdoByteArray* GetGeometry(FdoString* propertyName);

    /// \brief
    /// Gets the raster object of the specified property.
    /// Because no conversion is performed, the property must be
    /// of Raster type; otherwise, an exception is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the raster object.
    /// 
    virtual FdoIRaster* GetRaster(FdoString* propertyName);

    /// \brief
    /// Advances the reader to the next item. The default position of the
    /// reader is prior to the first item. Thus, you must call ReadNext
    /// to begin accessing any data.
    /// 
    /// \return
    /// Returns true if there is a next item.
    /// 
    virtual bool ReadNext();

    /// \brief
    /// Closes the FdoIDataReader object, freeing any resources it may be holding.
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual void Close();

    //------------------------------------------------------------
    /// FdoIFeatureReader implementation
    //------------------------------------------------------------

	/// \brief
    /// Gets the definition of the object currently being read. If the user
    /// has requested only a subset of the class properties, the class 
    /// definition reflects what the user has asked, rather than the full class 
    /// definition.
    /// 
    /// \return
    /// Returns the class definition object.
    /// 
    virtual FdoClassDefinition* GetClassDefinition();

    /// \brief
    /// Gets a value indicating the depth of nesting for the current reader.
    /// The depth value increases each time GetFeatureObject is called and a new 
    /// reader is returned. The outermost reader has a depth of 0.
    /// 
    /// \return
    /// Returns the depth
    /// 
    virtual FdoInt32 GetDepth();

    /// \brief
    /// Gets the geometry value of the specified property as a byte array in 
    /// FGF format. Because no conversion is performed, the property must be
    /// of Geometric type; otherwise, an exception is thrown. 
    /// This method is a language-specific performance optimization that returns a
    /// pointer to the array data, rather than to an object that encapsulates
    /// the array.  The array's memory area is only guaranteed to be valid
    /// until a call to ReadNext() or Close(), or the disposal of this reader
    /// object.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// \param count 
    /// Output the number of bytes in the array.
    /// 
    /// \return
    /// Returns a pointer to the byte array in FGF format.
    /// 
    virtual const FdoByte * GetGeometry(FdoString* propertyName, FdoInt32 * count);


    /// \brief
    /// Gets a reference to an FdoIFeatureReader to read the data contained in
    /// the object or object collection property. If the property is not an
    /// object property, an exception is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the nested feature reader
    /// 
    virtual FdoIFeatureReader* GetFeatureObject(FdoString* propertyName);

private:
	bool IsComputedProperty(FdoString* name);

private:
	FdoPtr<FdoIFeatureReader>		m_reader;
	FdoPtr<FdoFilter>				m_filter;
	FdoPtr<FdoIdentifierCollection> m_computedIds;
	FdoPtr<FdoExpressionEngine>		m_filterExec;
	FdoPtr<FdoClassDefinition>		m_classDef;
};

#endif  // FdoExpressionEngineUtilFeatureReader_H


