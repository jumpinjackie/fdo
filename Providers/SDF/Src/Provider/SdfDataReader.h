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
#ifndef SDFDATAREADER_H
#define SDFDATAREADER_H

class PropertyIndex;
class BinaryReader;


//A somewhat generic data reader, based on the PropertyIndex and
//BinaryReader helper classes. The PropertyIndex is used to obtain
//information about a property and the BinaryReader is used to get the
//value of a property. This class is abstract and the ReadNext()
//function needs to be implemented by child classes.
//TODO: this class can eventually become a base class for the 
//SdfSimpleFeatureReader also, but right now there is not time to do
//that refactoring.
class SdfDataReader : public FdoIDataReader
{
public:

    SDF_API SdfDataReader();
    SDF_API ~SdfDataReader();

protected:

    SDF_API virtual void Dispose();

public:

    //------------------------------------------------------------
    // FdoIDataReader implementation
    //------------------------------------------------------------

    /// <summary>Gets the number of propertys in the result set.</summary>
    /// <returns>Returns the number of propertys.</returns> 
    SDF_API virtual FdoInt32 GetPropertyCount();

    /// <summary>Gets the name of the property at the given ordinal position.</summary>
    /// <param name="index">Input the position of the property.</param> 
    /// <returns>Returns the property name</returns> 
    SDF_API virtual FdoString* GetPropertyName(FdoInt32 index);

    /// <summary>Gets the data type of the property with the specified name.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the type of the property.</returns> 
    SDF_API virtual FdoDataType GetDataType(FdoString* propertyName);

    /// <summary>Gets the FDO property type of a given property. This is used
    //  to indicate if a given property is a geometric property or a data property. If the property is
    //  a FdoPropertyType_Data, then GetDataType can be used to to find the data type of the property.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the FDO property type.</returns> 
    SDF_API virtual FdoPropertyType GetPropertyType(FdoString* propertyName);

    /// <summary>Gets the Boolean value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_Boolean or an
    /// exception is thrown.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the Boolean value</returns> 
    SDF_API virtual bool GetBoolean(FdoString* propertyName);

    /// <summary>Gets the byte value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_Byte or an
    /// exception is thrown.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the byte value.</returns> 
    SDF_API virtual FdoByte GetByte(FdoString* propertyName);

    /// <summary>Gets the date time value of the specified property. No conversion
    /// is performed, thus the property must be FdoDataType_DateTime or
    /// an exception is thrown.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the date and time value.</returns> 
    SDF_API virtual FdoDateTime GetDateTime(FdoString* propertyName);

    /// <summary>Gets the double-precision floating point value of the specified property.
    /// No conversion is performed, thus the property must be of type
    /// Double or an exception is thrown.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the double value.</returns> 
    SDF_API virtual double GetDouble(FdoString* propertyName);

    /// <summary>Gets the signed 16-bit integer value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_Int16 or an
    /// exception is thrown.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the FdoInt16 value.</returns> 
    SDF_API virtual FdoInt16 GetInt16(FdoString* propertyName);

    /// <summary>Gets the signed 32-bit integer value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_Int32 or an
    /// exception is thrown.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the FdoInt32 value.</returns> 
    SDF_API virtual FdoInt32 GetInt32(FdoString* propertyName);

    /// <summary>Gets the signed 64-bit integer value of the specified property. No conversion
    /// is performed, thus the property must be FdoDataType_Int64 or an
    /// exception is thrown.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the FdoInt64 value.</returns> 
    SDF_API virtual FdoInt64 GetInt64(FdoString* propertyName);

    /// <summary>Gets the single-precision floating point value of the specified property.
    /// No conversion is performed, thus the property must be FdoDataType_Single
	/// or an exception is thrown.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the single value</returns> 
    SDF_API virtual float GetSingle(FdoString* propertyName);

    /// <summary>Gets the string value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_String or an
    /// exception is thrown.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the string value.</returns> 
    SDF_API virtual FdoString* GetString(FdoString* propertyName);

    /// <summary>Gets a LOBValue reference. The LOB is fully read in and data available.
    /// Because no conversion is performed, the property must be FdoDataType_BLOB or
    /// FdoDataType_CLOB etc. (a LOB type)
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the reference to LOBValue</returns> 
    SDF_API virtual FdoLOBValue* GetLOB(FdoString* propertyName);

    /// <summary>Gets a reference of the specified LOB property as a FdoBLOBStreamReader or
    /// FdoCLOBStreamReader etc. to allow reading in blocks of data.
    /// Because no conversion is performed, the property must be FdoDataType_BLOB 
    /// or FdoDataType_CLOB etc. (a LOB type)
    /// Cast the FdoIStreamReader to the appropiate LOB Stream Reader.
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns a reference to a LOB stream reader</returns> 
    SDF_API virtual FdoIStreamReader* GetLOBStreamReader(const wchar_t* propertyName );

    /// <summary>Returns true if the value of the specified property is null.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns true if the value is null.</returns> 
    SDF_API virtual bool IsNull(FdoString* propertyName);

    /// <summary>Gets the geometry value of the specified property as a byte array
    /// in FGF format. No conversion is performed, thus the property
    /// must be of Geometric type or an exception is thrown.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the FGF byte array value.</returns> 
    SDF_API virtual FdoByteArray* GetGeometry(FdoString* propertyName);

    /// <summary>Gets the raster object of the specified property.
    /// Because no conversion is performed, the property must be
    /// of Raster type; otherwise, an exception is thrown.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the raster object.</returns> 
    SDF_API virtual FdoIRaster* GetRaster(FdoString* propertyName);

    /// <summary>Advances the reader to the next item. The default position of the
    /// reader is prior to the first item. Thus, you must call ReadNext
    /// to begin accessing any data.</summary>
    /// <returns>Returns true if there is a next item.</returns> 
    SDF_API virtual bool ReadNext() = 0;

    /// <summary>Closes the FdoIDataReader object, freeing any resources it may be holding.</summary>
    /// <returns>Returns nothing</returns> 
    SDF_API virtual void Close();

    //------------------------------------------------------------
    // SdfDataReader
    //------------------------------------------------------------

    SDF_API virtual BinaryReader* GetBinaryReader() = 0;
        

protected:

    int PositionReader(int recordIndex);

    PropertyIndex* m_propIndex;


};

#endif

