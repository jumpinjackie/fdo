// 
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
//  
#include "stdafx.h"
#include "SdfDataReader.h"
#include "BinaryReader.h"
#include "PropertyIndex.h"

//just a helpful define for many of the Get... functions
#define PERFORM_CHECKS(type)                                                    \
                                                                                \
    PropertyStub* ps = m_propIndex->GetPropInfo(propertyName);                  \
                                                                                \
    if (ps == NULL)                                                             \
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_35_PROPERTY_NOT_AVAILABLE))); \
                                                                                \
    if (ps->m_dataType != FdoDataType_##type)                                   \
        throw FdoCommandException::Create (FdoException::NLSGetMessage(FDO_NLSID(FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH))); \
                                                                                \
    int len = PositionReader(ps->m_recordIndex);                                \
                                                                                \
    if (len == 0)                                                               \
        throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_51_NULL_VALUE), "Property value is null."));



SdfDataReader::SdfDataReader() :
    m_propIndex(NULL)
{
}

SdfDataReader::~SdfDataReader()
{
    if (NULL != m_propIndex)
        delete m_propIndex;
}

void SdfDataReader::Dispose()
{
    delete this;
}

FdoInt32 SdfDataReader::GetPropertyCount()
{
    return m_propIndex->GetNumProps();
}

FdoString* SdfDataReader::GetPropertyName(FdoInt32 index)
{
    PropertyStub* ps = m_propIndex->GetPropInfo(index);

    if (ps == NULL)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_37_INVALID_PROPERTY_INDEX)));
    
    return ps->m_name;
}

FdoDataType SdfDataReader::GetDataType(FdoString* propertyName)
{
    PropertyStub* ps = m_propIndex->GetPropInfo(propertyName);

    if (ps == NULL)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_38_INVALID_PROPERTY_NAME)));

    _ASSERT (ps->m_dataType != FdoDataType(-1));

    return ps->m_dataType;         
}

FdoPropertyType SdfDataReader::GetPropertyType(FdoString* propertyName)
{
    return (FdoPropertyType)0;         
}

bool SdfDataReader::GetBoolean(FdoString* propertyName)
{
    PERFORM_CHECKS(Boolean);
    return GetBinaryReader()->ReadByte() != 0;
}

FdoByte SdfDataReader::GetByte(FdoString* propertyName)
{
    PERFORM_CHECKS(Byte);
    return GetBinaryReader()->ReadByte();
}

FdoDateTime SdfDataReader::GetDateTime(FdoString* propertyName)
{
    PERFORM_CHECKS(DateTime);
    return GetBinaryReader()->ReadDateTime();
}

double SdfDataReader::GetDouble(FdoString* propertyName)
{
   PERFORM_CHECKS(Double);
   return GetBinaryReader()->ReadDouble();
}

FdoInt16 SdfDataReader::GetInt16(FdoString* propertyName)
{
   PERFORM_CHECKS(Int16);
   return GetBinaryReader()->ReadInt16();
}

FdoInt32 SdfDataReader::GetInt32(FdoString* propertyName)
{
   PERFORM_CHECKS(Int32);
   return GetBinaryReader()->ReadInt32();
}

FdoInt64 SdfDataReader::GetInt64(FdoString* propertyName)
{
   PERFORM_CHECKS(Int64);
   return GetBinaryReader()->ReadInt64();
}

float SdfDataReader::GetSingle(FdoString* propertyName)
{
   PERFORM_CHECKS(Single);
   return GetBinaryReader()->ReadSingle();
}

FdoString* SdfDataReader::GetString(FdoString* propertyName)
{
   PERFORM_CHECKS(String);
   return GetBinaryReader()->ReadRawString(len);
}

FdoLOBValue* SdfDataReader::GetLOB(FdoString* propertyName)
{
    throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_13_LOBS_NOT_SUPPORTED)));
}

FdoIStreamReader* SdfDataReader::GetLOBStreamReader(const wchar_t* propertyName)
{
    throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_13_LOBS_NOT_SUPPORTED)));
}

bool SdfDataReader::IsNull(FdoString* propertyName)
{
    //same NULL handling as SdfSimpleFeatureReader.
    //If it's wrong, at least it is consistent!

    PropertyStub* ps = m_propIndex->GetPropInfo(propertyName);

    _ASSERT(ps);

    int len = PositionReader(ps->m_recordIndex);

    //if there is no data record for this propery, we will
    //say that it is NULL
    //TODO: I am not clear as to whether having a default value for 
    //this property in the schema would mean that the property is not null
    return (len == 0) && !ps->m_isAutoGen;
}

FdoByteArray* SdfDataReader::GetGeometry(FdoString* propertyName)
{
    PropertyStub* ps = m_propIndex->GetPropInfo(propertyName);                  
                                                                                
    if (ps == NULL)                                                             
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_35_PROPERTY_NOT_AVAILABLE)));
                                                                                
    if (ps->m_dataType != FdoDataType(-1))                                   
        throw FdoCommandException::Create (FdoException::NLSGetMessage(FDO_NLSID(FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                                                                                
    int len = PositionReader(ps->m_recordIndex);                                
                                                                                
    _ASSERT(len != 0);

    void* ptr = GetBinaryReader()->GetDataAtCurrentPosition();

    return FdoByteArray::Create((unsigned char*)ptr, len);
}

FdoIRaster* SdfDataReader::GetRaster(FdoString* propertyName)
{
    throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_43_NO_RASTERS)));
}

void SdfDataReader::Close()
{
}


int SdfDataReader::PositionReader(int recordIndex)
{
    BinaryReader* reader = GetBinaryReader();

    //make sure we have data
    if (!reader || (reader->GetDataLen() == 0))
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_35_PROPERTY_NOT_AVAILABLE)));

    //position where offset to property value is stored
    //remember: we have a 2 byte FCID at the beginning
    //and then offsets for each property
    reader->SetPosition(sizeof(FCID_STORAGE) + recordIndex * sizeof(int));

    //get offset to property value and position there
    int offset = reader->ReadInt32();

    //compute how many bytes the
    //property value spans -- we do this by subtracting the two offsets
    int endoffset = 0;

    if (recordIndex < m_propIndex->GetNumProps() - 1)
        endoffset = reader->ReadInt32();
    else
        endoffset = reader->GetDataLen();

    //skip to the beginning of the data value
    reader->SetPosition(offset);

    //the length of the data
    //If it is 0, it means the property value is NULL or equal to
    //the default property value (if specified in the schema)
    int len = endoffset - offset;

    return len;
}


