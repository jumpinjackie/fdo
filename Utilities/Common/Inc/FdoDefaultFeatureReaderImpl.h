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
#ifndef FDODEFAULTFEATUREREADER_IMPL_H
#define FDODEFAULTFEATUREREADER_IMPL_H

#include "stdafx.h"
#include "FdoCommonMiscUtil.h"
#include <assert.h>

template<class FDO_FEATURE_READER>
class FdoDefaultFeatureReaderImpl : public FDO_FEATURE_READER
{
public:
    /// \brief
    /// Gets the Boolean value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_Boolean or an 
    /// exception is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the Boolean value.
    /// 
    virtual bool GetBoolean(FdoString* propertyName) = 0;

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
    virtual FdoByte GetByte(FdoString* propertyName) = 0;

    /// \brief
    ///  Gets the date and time value of the specified property. No conversion is 
    /// performed, thus the property must be FdoDataType_DateTime or an 
    /// exception is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the date and time value.
    /// 
    virtual FdoDateTime GetDateTime(FdoString* propertyName) = 0;

    /// \brief
    /// Gets the double-precision floating point value of the specified property. No
    /// conversion is performed, thus the property must be FdoDataType_Double
    /// or an exception is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the double floating point value
    /// 
    virtual double GetDouble(FdoString* propertyName) = 0;

    /// \brief
    /// Gets the 16-bit integer value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_Int16 or an exception
    /// is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the FdoInt16 value.
    /// 
    virtual FdoInt16 GetInt16(FdoString* propertyName) = 0;

    /// \brief
    /// Gets the 32-bit integer value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_Int32 or an exception
    /// is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the FdoInt32 value
    /// 
    virtual FdoInt32 GetInt32(FdoString* propertyName) = 0;

    /// \brief
    /// Gets the 64-bit integer value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_Int64 or an exception
    /// is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the FdoInt64 value.
    /// 
    virtual FdoInt64 GetInt64(FdoString* propertyName) = 0;

    /// \brief
    /// Gets the Single floating point value of the specified property. No
    /// conversion is performed, thus the property must be FdoDataType_Single
    /// or an exception is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the single value
    /// 
    virtual float GetSingle(FdoString* propertyName) = 0;

    /// \brief
    /// Gets the string value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_String or an exception
    /// is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the string value
    /// 
    virtual FdoString* GetString(FdoString* propertyName) = 0;

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
    virtual FdoLOBValue* GetLOB(FdoString* propertyName) = 0;

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
    virtual FdoIStreamReader* GetLOBStreamReader(const wchar_t* propertyName ) = 0;

    /// \brief
    /// Returns true if the value of the specified property is null.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns true if the value is null.
    /// 
    virtual bool IsNull(FdoString* propertyName) = 0;

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
    virtual FdoIRaster* GetRaster(FdoString* propertyName) = 0;

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
    virtual const FdoByte * GetGeometry(FdoString* propertyName, FdoInt32 * count) = 0;

    /// \brief
    /// Gets the geometry value of the specified property as a byte array in 
    /// FGF format. Because no conversion is performed, the property must be
    /// of Geometric type; otherwise, an exception is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the byte array in FGF format.
    /// 
    virtual FdoByteArray* GetGeometry(FdoString* propertyName) = 0;

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
    virtual FdoIFeatureReader* GetFeatureObject(FdoString* propertyName) = 0;


    /// \brief
    /// Gets the name of the property at the given ordinal position.
    /// 
    /// \param index 
    /// Input the position of the property.
    /// 
    /// \return
    /// Returns the property name
    /// 
    virtual FdoString* GetPropertyName(FdoInt32 index);

    /// \brief
    /// Gets the index of the property with the specified name.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the property index
    /// 
    virtual FdoInt32 GetPropertyIndex(FdoString* propertyName);

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
    virtual FdoBoolean GetBoolean(FdoInt32 index);

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
    virtual FdoByte GetByte(FdoInt32 index);

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
    virtual FdoDateTime GetDateTime(FdoInt32 index);

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
    virtual FdoDouble GetDouble(FdoInt32 index);

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
    virtual FdoInt16 GetInt16(FdoInt32 index);

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
    virtual FdoInt32 GetInt32(FdoInt32 index);

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
    virtual FdoInt64 GetInt64(FdoInt32 index);

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
    virtual FdoFloat GetSingle(FdoInt32 index);

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
    virtual FdoString* GetString(FdoInt32 index);

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
    virtual FdoLOBValue* GetLOB(FdoInt32 index);

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
    virtual FdoIStreamReader* GetLOBStreamReader(FdoInt32 index);

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
    virtual FdoBoolean IsNull(FdoInt32 index);

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
    virtual FdoIRaster* GetRaster(FdoInt32 index);

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
    virtual const FdoByte * GetGeometry(FdoInt32 index, FdoInt32* count);

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
    virtual FdoByteArray* GetGeometry(FdoInt32 index);

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
    virtual FdoIFeatureReader* GetFeatureObject(FdoInt32 index);

protected:

    FdoDefaultFeatureReaderImpl():m_propertyNamesInitialized(false)
    {
    }

    virtual ~FdoDefaultFeatureReaderImpl()
    {
    }

    void InitializePropertyNames();
    void FillProperties(FdoClassDefinition* classDef);

private:
    FdoStringsP m_propertyNames;
    bool m_propertyNamesInitialized;
};

template<class FDO_FEATURE_READER>
FdoString* FdoDefaultFeatureReaderImpl<FDO_FEATURE_READER>::GetPropertyName(FdoInt32 index)
{
    InitializePropertyNames();

    if (index >=0 && index < m_propertyNames->GetCount())
        return m_propertyNames->GetString(index);
    else
    {
        throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_73_PROPERTY_INDEXOUTOFBOUNDS), index));
    }
}

template<class FDO_FEATURE_READER>
FdoInt32 FdoDefaultFeatureReaderImpl<FDO_FEATURE_READER>::GetPropertyIndex(FdoString* propertyName)
{
    InitializePropertyNames();

    int index = m_propertyNames->IndexOf(propertyName, false);
    if (-1 != index)
    {
        return index;
    }
    else
    {
        throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_74_PROPERTY_NAME_NOT_FOUND), propertyName));
    }
}

template<class FDO_FEATURE_READER>
const FdoByte * FdoDefaultFeatureReaderImpl<FDO_FEATURE_READER>::GetGeometry(FdoInt32 index, FdoInt32* count)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetGeometry(propertyName, count);
}

template<class FDO_FEATURE_READER>
FdoIFeatureReader* FdoDefaultFeatureReaderImpl<FDO_FEATURE_READER>::GetFeatureObject(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetFeatureObject(propertyName);
}

template<class FDO_FEATURE_READER>
FdoBoolean FdoDefaultFeatureReaderImpl<FDO_FEATURE_READER>::IsNull(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return IsNull(propertyName);
}

template<class FDO_FEATURE_READER>
FdoBoolean FdoDefaultFeatureReaderImpl<FDO_FEATURE_READER>::GetBoolean(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetBoolean(propertyName);
}

template<class FDO_FEATURE_READER>
FdoByte FdoDefaultFeatureReaderImpl<FDO_FEATURE_READER>::GetByte(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetByte(propertyName);
}

template<class FDO_FEATURE_READER>
FdoDateTime FdoDefaultFeatureReaderImpl<FDO_FEATURE_READER>::GetDateTime(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetDateTime(propertyName);
}

template<class FDO_FEATURE_READER>
FdoDouble FdoDefaultFeatureReaderImpl<FDO_FEATURE_READER>::GetDouble(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetDouble(propertyName);
}

template<class FDO_FEATURE_READER>
FdoInt16 FdoDefaultFeatureReaderImpl<FDO_FEATURE_READER>::GetInt16(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetInt16(propertyName);
}

template<class FDO_FEATURE_READER>
FdoInt32 FdoDefaultFeatureReaderImpl<FDO_FEATURE_READER>::GetInt32(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetInt32(propertyName);
}

template<class FDO_FEATURE_READER>
FdoInt64 FdoDefaultFeatureReaderImpl<FDO_FEATURE_READER>::GetInt64(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetInt64(propertyName);
}

template<class FDO_FEATURE_READER>
FdoFloat FdoDefaultFeatureReaderImpl<FDO_FEATURE_READER>::GetSingle(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetSingle(propertyName);
}

template<class FDO_FEATURE_READER>
FdoString* FdoDefaultFeatureReaderImpl<FDO_FEATURE_READER>::GetString(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetString(propertyName);
}

template<class FDO_FEATURE_READER>
FdoLOBValue* FdoDefaultFeatureReaderImpl<FDO_FEATURE_READER>::GetLOB(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetLOB(propertyName);
}

template<class FDO_FEATURE_READER>
FdoIStreamReader* FdoDefaultFeatureReaderImpl<FDO_FEATURE_READER>::GetLOBStreamReader(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetLOBStreamReader(propertyName);
}

template<class FDO_FEATURE_READER>
FdoByteArray* FdoDefaultFeatureReaderImpl<FDO_FEATURE_READER>::GetGeometry(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetGeometry(propertyName);
}

template<class FDO_FEATURE_READER>
FdoIRaster* FdoDefaultFeatureReaderImpl<FDO_FEATURE_READER>::GetRaster(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetRaster(propertyName);
}

template<class FDO_FEATURE_READER>
void FdoDefaultFeatureReaderImpl<FDO_FEATURE_READER>::InitializePropertyNames()
{
    if (m_propertyNamesInitialized)
        return;

    FdoPtr<FdoClassDefinition> classDef = this->GetClassDefinition();
    VALIDATE_POINTER(classDef.p);

    m_propertyNames = FdoStringCollection::Create();
    FillProperties(classDef);

    m_propertyNamesInitialized = true;
}

template<class FDO_FEATURE_READER>
void FdoDefaultFeatureReaderImpl<FDO_FEATURE_READER>::FillProperties(FdoClassDefinition* classDef)
{
    if (classDef == NULL)
        return;

    FdoPtr<FdoClassDefinition> baseClassDef = classDef->GetBaseClass();
    FillProperties(baseClassDef);

    FdoPropertiesP props = classDef->GetProperties();
    VALIDATE_POINTER(props.p);

    for (int i = 0; i < props->GetCount(); i++)
    {
        FdoPtr<FdoPropertyDefinition> prop = props->GetItem(i);
        m_propertyNames->Add(prop->GetName());
    }
}

#endif // FDODEFAULTFEATUREREADER_IMPL_H
