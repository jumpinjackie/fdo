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

#include "../FdoExpressionEngine.h"


//A generic feature reader. The underlying feature reader and the ExpressionEngine
// are used to get the value of a property, computed identifiers and expressions
class FdoExpressionEngineUtilFeatureReader : public FdoIFeatureReader
{
public:

    EXPRESSIONENGINE_API static FdoExpressionEngineUtilFeatureReader* Create (FdoClassDefinition* classDef, FdoIFeatureReader* reader, FdoFilter *filter, FdoIdentifierCollection* selectedIds, FdoExpressionEngineFunctionCollection *userDefinedFunctions);

    EXPRESSIONENGINE_API FdoExpressionEngineUtilFeatureReader (FdoClassDefinition* classDef, FdoIFeatureReader* reader, FdoFilter *filter, FdoIdentifierCollection* selectedIds, FdoExpressionEngineFunctionCollection *userDefinedFunctions);

protected:
	EXPRESSIONENGINE_API virtual void Dispose();
    EXPRESSIONENGINE_API FdoExpressionEngineUtilFeatureReader() {};  // dummy 0-arg constructor to please FdoPtr::operator->
    EXPRESSIONENGINE_API virtual ~FdoExpressionEngineUtilFeatureReader();

public:

    //------------------------------------------------------------
    /// FdoIReader implementation
    //------------------------------------------------------------

    /// \brief
    /// Gets the name of the property at the given ordinal position.
    /// 
    /// \param index 
    /// Input the position of the property.
    /// 
    /// \return
    /// Returns the property name
    /// 
    EXPRESSIONENGINE_API virtual FdoString* GetPropertyName(FdoInt32 index);

    /// \brief
    /// Gets the index of the property with the specified name.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the property index
    /// 
    EXPRESSIONENGINE_API virtual FdoInt32 GetPropertyIndex(FdoString* propertyName);

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
    EXPRESSIONENGINE_API virtual bool GetBoolean(FdoString* propertyName);

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

    EXPRESSIONENGINE_API virtual FdoByte GetByte(FdoString* propertyName);

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
    EXPRESSIONENGINE_API virtual FdoDateTime GetDateTime(FdoString* propertyName);

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
    EXPRESSIONENGINE_API virtual double GetDouble(FdoString* propertyName);

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
    EXPRESSIONENGINE_API virtual FdoInt16 GetInt16(FdoString* propertyName);

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
    EXPRESSIONENGINE_API virtual FdoInt32 GetInt32(FdoString* propertyName);

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
    EXPRESSIONENGINE_API virtual FdoInt64 GetInt64(FdoString* propertyName);

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
    EXPRESSIONENGINE_API virtual float GetSingle(FdoString* propertyName);

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
    EXPRESSIONENGINE_API virtual FdoString* GetString(FdoString* propertyName);

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
    EXPRESSIONENGINE_API virtual FdoLOBValue* GetLOB(FdoString* propertyName);

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
    EXPRESSIONENGINE_API virtual FdoIStreamReader* GetLOBStreamReader(const wchar_t* propertyName );

    /// \brief
    /// Returns true if the value of the specified property is null.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns true if the value is null.
    /// 
    EXPRESSIONENGINE_API virtual bool IsNull(FdoString* propertyName);

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
    EXPRESSIONENGINE_API virtual FdoByteArray* GetGeometry(FdoString* propertyName);

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
    EXPRESSIONENGINE_API virtual FdoIRaster* GetRaster(FdoString* propertyName);

    /// \brief
    /// Returns true if the value of the property at the specified 
    /// index is null.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns true if the value is null.
    /// 
    EXPRESSIONENGINE_API virtual FdoBoolean IsNull(FdoInt32 index);

    /// \brief
    /// Gets the Boolean value of the property specified at the index position. 
    /// No conversion is performed, thus the property must be FdoDataType_Boolean 
    /// or an exception is thrown.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns the Boolean value.
    /// 
    EXPRESSIONENGINE_API virtual FdoBoolean GetBoolean(FdoInt32 index);

    /// \brief
    /// Gets the Byte value of the property specified at the index position. 
    /// No conversion is performed, thus the property must be FdoDataType_Byte 
    /// or an exception is thrown.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns the byte value.
    /// 
    EXPRESSIONENGINE_API virtual FdoByte GetByte(FdoInt32 index);

    /// \brief
    /// Gets the date and time value of the of the property specified at 
    /// the index position. No conversion is performed, thus the property 
    /// must be FdoDataType_DateTime or an exception is thrown.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns the date and time value.
    /// 
    EXPRESSIONENGINE_API virtual FdoDateTime GetDateTime(FdoInt32 index);

    /// \brief
    /// Gets the double-precision floating point value of the property specified at 
    /// the index position. No conversion is performed, thus the property must be 
    /// FdoDataType_Double or an exception is thrown.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns the double floating point value
    /// 
    EXPRESSIONENGINE_API virtual FdoDouble GetDouble(FdoInt32 index);

    /// \brief
    /// Gets the 16-bit integer value of the property specified at 
    /// the index position. No conversion is performed, thus the 
    /// property must be FdoDataType_Int16 or an exception is thrown.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns the FdoInt16 value.
    /// 
    EXPRESSIONENGINE_API virtual FdoInt16 GetInt16(FdoInt32 index);

    /// \brief
    /// Gets the 32-bit integer value of the property specified at 
    /// the index position. No conversion is performed, thus the 
    /// property must be FdoDataType_Int32 or an exception is thrown.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns the FdoInt32 value
    /// 
    EXPRESSIONENGINE_API virtual FdoInt32 GetInt32(FdoInt32 index);

    /// \brief
    /// Gets the 64-bit integer value of the property specified at 
    /// the index position. No conversion is performed, thus the 
    /// property must be FdoDataType_Int64 or an exception is thrown.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns the FdoInt64 value.
    /// 
    EXPRESSIONENGINE_API virtual FdoInt64 GetInt64(FdoInt32 index);

    /// \brief
    /// Gets the Single floating point value of the property specified at 
    /// the index position. No conversion is performed, thus the property 
    /// must be FdoDataType_Single or an exception is thrown.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns the single value
    /// 
    EXPRESSIONENGINE_API virtual FdoFloat GetSingle(FdoInt32 index);

    /// \brief
    /// Gets the string value of the property specified at the index
    /// position. No conversion is performed, thus the property must
    /// be FdoDataType_String or an exception is thrown.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns the string value
    /// 
    EXPRESSIONENGINE_API virtual FdoString* GetString(FdoInt32 index);

    /// \brief
    /// Gets a LOBValue reference to the property specified at the index
    /// position. The LOB is fully read in and data available.
    /// Because no conversion is performed, the property must be 
    /// FdoDataType_BLOB or FdoDataType_CLOB etc. (a LOB type)
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns the reference to LOBValue
    /// 
    EXPRESSIONENGINE_API virtual FdoLOBValue* GetLOB(FdoInt32 index);

    /// \brief
    /// Gets a reference to the specified LOB property, specified at the index
    /// position. The reference is returned as an FdoBLOBStreamReader or an 
    /// FdoCLOBStreamReader, to allow reading in blocks of data. Because 
    /// no conversion is performed, the property must be FdoDataType_BLOB 
    /// or FdoDataType_CLOB etc. (a LOB type) Cast the FdoIStreamReader 
    /// to the appropiate LOB Stream Reader.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns a reference to a LOB stream reader
    /// 
    EXPRESSIONENGINE_API virtual FdoIStreamReader* GetLOBStreamReader(FdoInt32 index);

    /// \brief
    /// Gets the geometry value of the property, at the specified index, as  
    /// a byte array in FGF format. Because no conversion is performed, the 
    /// property must be of Geometric type; otherwise, an exception is thrown.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns the byte array in FGF format.
    /// 
    EXPRESSIONENGINE_API virtual FdoByteArray* GetGeometry(FdoInt32 index);

    /// \brief
    /// Gets the raster object of the property at the specified index.
    /// Because no conversion is performed, the property must be
    /// of Raster type; otherwise, an exception is thrown.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns the raster object.
    /// 
    EXPRESSIONENGINE_API virtual FdoIRaster* GetRaster(FdoInt32 index);

    /// \brief
    /// Advances the reader to the next item. The default position of the
    /// reader is prior to the first item. Thus, you must call ReadNext
    /// to begin accessing any data.
    /// 
    /// \return
    /// Returns true if there is a next item.
    /// 
    EXPRESSIONENGINE_API virtual bool ReadNext();

    /// \brief
    /// Closes the FdoIDataReader object, freeing any resources it may be holding.
    /// 
    /// \return
    /// Returns nothing
    /// 
    EXPRESSIONENGINE_API virtual void Close();

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
    EXPRESSIONENGINE_API virtual FdoClassDefinition* GetClassDefinition();

    /// \brief
    /// Gets a value indicating the depth of nesting for the current reader.
    /// The depth value increases each time GetFeatureObject is called and a new 
    /// reader is returned. The outermost reader has a depth of 0.
    /// 
    /// \return
    /// Returns the depth
    /// 
    EXPRESSIONENGINE_API virtual FdoInt32 GetDepth();

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
    EXPRESSIONENGINE_API virtual const FdoByte * GetGeometry(FdoString* propertyName, FdoInt32 * count);


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
    EXPRESSIONENGINE_API virtual FdoIFeatureReader* GetFeatureObject(FdoString* propertyName);

    /// \brief
    /// Gets the geometry value of the property, at the specified index, 
    /// as a byte array in FGF format. Because no conversion is performed, 
    /// the property must be of Geometric type; otherwise, an exception is thrown. 
    /// This method is a language-specific performance optimization that returns a
    /// pointer to the array data, rather than to an object that encapsulates
    /// the array.  The array's memory area is only guaranteed to be valid
    /// until a call to ReadNext() or Close(), or the disposal of this reader
    /// object.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// \param count 
    /// Output the number of bytes in the array.
    /// 
    /// \return
    /// Returns a pointer to the byte array in FGF format.
    /// 
    EXPRESSIONENGINE_API virtual const FdoByte * GetGeometry(FdoInt32 index, FdoInt32* count);

    /// \brief
    /// Gets a reference to an FdoIFeatureReader to read the data contained in
    /// the object or object collection property defined at the specified index 
    /// position. If the property is not an object property, an exception is thrown.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns the nested feature reader
    /// 
    EXPRESSIONENGINE_API virtual FdoIFeatureReader* GetFeatureObject(FdoInt32 index);

private:
	bool IsComputedProperty(FdoString* name);

private:
	FdoPtr<FdoIFeatureReader>		m_reader;
	FdoPtr<FdoFilter>				m_filter;
	FdoPtr<FdoIdentifierCollection> m_computedIds;
	FdoPtr<FdoExpressionEngine>		m_filterExec;
	FdoPtr<FdoClassDefinition>		m_classDef;
    FdoPtr<FdoIdentifierCollection> m_selectedIds;
};

#endif  // FdoExpressionEngineUtilFeatureReader_H


