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
#include "BinaryReader.h"
#include "utf8_.h"
#include "BinaryWriter.h"

//use this to avoid entres equal to 0 in the 
//unicode string hash table -- because hash_map
//uses 0 as default empty values.
const int ADD_TO_OFFSET = 1;

BinaryReader::BinaryReader(unsigned char* data, int len, int propCount )
{
	Init(propCount);
    m_data = data;
    m_len = len;
}

BinaryReader::BinaryReader(unsigned char* data, int len )
{
    Init(SDF_STRING_CACHE_COUNT);
    m_data = data;
    m_len = len;
	
}

BinaryReader::BinaryReader()
{
    Init(SDF_STRING_CACHE_COUNT);
}

void BinaryReader::Init( int propCount )
{
	m_data = NULL;
    m_len = 0;
    m_pos = 0;
    m_wcsCacheLen = 0;
    m_wcsCacheCurrent = 0;
    m_wcsCache = NULL;
	wcsCacheLastIndex = 0;
    m_wcsStringCache = NULL;
	wcsCacheLen = propCount;
    m_wcsStringCache = new SdfStringCacheDef[propCount];
    for(int i=0; i<propCount; i++ )
    {
        m_wcsStringCache[i].wcsLen = 0;
        m_wcsStringCache[i].wcsString = NULL;
    }
}

BinaryReader::~BinaryReader()
{
    if( m_wcsStringCache != NULL )
    {
        for(unsigned int i=0; i<wcsCacheLen; i++ )
        {
            if( m_wcsStringCache[i].wcsString != NULL )
                delete[] m_wcsStringCache[i].wcsString;
        }
        delete[] m_wcsStringCache;
    }

    if (m_wcsCache)
        delete [] m_wcsCache;
}

void BinaryReader::Reset(unsigned char* data, int len)
{
    
    m_data = data;
    m_len = len;
    m_pos = 0;
    m_wcsCacheCurrent = 0;

    for(unsigned int i=0; i<wcsCacheLen; i++ )
    {
        if( m_wcsStringCache[i].wcsString != NULL )
            m_wcsStringCache[i].wcsString[0] = '\0';
    }
}

void BinaryReader::SetPosition(int offset)
{
    m_pos = offset;
}

int BinaryReader::GetPosition()
{
    return m_pos;
}

unsigned char* BinaryReader::GetDataAtCurrentPosition()
{
    return m_data + m_pos;
}

unsigned BinaryReader::GetDataLen()
{
    return m_len;
}

double BinaryReader::ReadDouble()
{
    if (m_pos > (m_len - (int)sizeof(double)))
    {
        _ASSERT(false);
        throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_100_READ_DOUBLE_ERROR)));
    }

    //TODO: endian
    double ret = *(double*)(m_data + m_pos);
    m_pos += sizeof(double);
    return ret;
}

float BinaryReader::ReadSingle()
{
    if (m_pos > (m_len - (int)sizeof(float)))
    {
        _ASSERT(false);
        throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_101_READ_SINGLE_ERROR)));
    }

    //TODO: endian
    float ret = *(float*)(m_data + m_pos);
    m_pos += sizeof(float);
    return ret;
}

int BinaryReader::ReadInt32()
{
    if (m_pos > (m_len - (int)sizeof(int)))
    {
        _ASSERT(false);
        throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_102_READ_INT32_ERROR)));
    }

    //TODO: endian
    int ret = *(int*)(m_data + m_pos);
    m_pos += sizeof(int);
    return ret;
}

unsigned BinaryReader::ReadUInt32()
{
    if (m_pos > (m_len - (int)sizeof(int)))
    {
        _ASSERT(false);
        throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_103_READ_UINT32_ERROR)));
    }

    //TODO: endian
    unsigned ret = *(unsigned*)(m_data + m_pos);
    m_pos += sizeof(unsigned);
    return ret;
}

short BinaryReader::ReadInt16()
{
    if (m_pos > (m_len - (int)sizeof(short)))
    {
        _ASSERT(false);
        throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_104_READ_INT16_ERROR)));
    }

    //TODO: endian
    short ret = *(short*)(m_data + m_pos);
    m_pos += sizeof(short);
    return ret;
}

unsigned short BinaryReader::ReadUInt16()
{
    if (m_pos > (m_len - (int)sizeof(unsigned short)))
    {
        _ASSERT(false);
        throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_105_READ_UINT16_ERROR)));
    }

    //TODO: endian
    unsigned short ret = *(unsigned short*)(m_data + m_pos);
    m_pos += sizeof(unsigned short);
    return ret;
}

FdoInt64 BinaryReader::ReadInt64()
{
    if (m_pos > (m_len - (int)sizeof(FdoInt64)))
    {
        _ASSERT(false);
        throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_106_READ_INT64_ERROR)));
    }

    //TODO: endian
    FdoInt64 ret = *(FdoInt64*)(m_data + m_pos);
    m_pos += sizeof(FdoInt64);
    return ret;
}


unsigned char BinaryReader::ReadByte()
{
    if (m_pos > (m_len - (int)sizeof(unsigned char)))
    {
        _ASSERT(false);
        throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_107_READ_BYTE_ERROR)));
    }

    //TODO: endian
    unsigned char ret = *(m_data + m_pos);
    m_pos += sizeof(unsigned char);
    return ret;
}

char BinaryReader::ReadChar()
{
    if (m_pos > (m_len - (int)sizeof(char)))
    {
        _ASSERT(false);
        throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_108_READ_CHAR_ERROR)));
    }

    //TODO: endian
    char ret = *(m_data + m_pos);
    m_pos += sizeof(char);
    return ret;
}

const wchar_t* BinaryReader::ReadString()
{
    //read string length (number of bytes, not characters!!!)
    //this includes null terminator character
    unsigned mbstrlen = ReadUInt32();


    return ReadRawString(mbstrlen);
}

#pragma warning(push)
#pragma warning(disable: 4018)  // '<' : signed/unsigned mismatch

const wchar_t* BinaryReader::ReadRawString( unsigned mbstrlen, int index )
{
    if( mbstrlen <= 1 )
    {
        m_pos += mbstrlen;
        return L"";
    }
    if( index >= wcsCacheLen )
        return ReadRawString( mbstrlen );

    if( m_wcsStringCache[index].wcsString != NULL && m_wcsStringCache[index].wcsString[0] != '\0' )
    {
        m_pos += mbstrlen;
		wcsCacheLastIndex = index;
        return m_wcsStringCache[index].wcsString;
    }

    if( mbstrlen > m_wcsStringCache[index].wcsLen )
    {
        if( NULL != m_wcsStringCache[index].wcsString )
            delete[] m_wcsStringCache[index].wcsString;

        m_wcsStringCache[index].wcsString = new wchar_t[mbstrlen+1];
        m_wcsStringCache[index].wcsLen =  mbstrlen;
    }
    int count = ut_utf8_to_unicode((char*)(m_data + m_pos), mbstrlen, m_wcsStringCache[index].wcsString, mbstrlen);

    _ASSERT(count > 0 && (unsigned int)count < mbstrlen);

    m_pos += mbstrlen;
	wcsCacheLastIndex = index;
    return m_wcsStringCache[index].wcsString;
}

#pragma warning(pop) // '<' : signed/unsigned mismatch

//
// Should only be called from internal code that does not require the returned 
// string to stay around until the next reset. It uses a circular buffer of
// SDF_STRING_CACHE_COUNT elements and the individual elements may get overwriten before the reset. 
// The Fdo requirement for feature reader is that string stays around untill the next ReadNext. 
const wchar_t* BinaryReader::ReadRawString(unsigned mbstrlen)
{
    if( mbstrlen <= 1 )
    {
        m_pos += mbstrlen;
        return L"";
    }
	if( (++wcsCacheLastIndex) >= wcsCacheLen )
		wcsCacheLastIndex = 0;

	if( mbstrlen > m_wcsStringCache[wcsCacheLastIndex].wcsLen )
    {
        if( NULL != m_wcsStringCache[wcsCacheLastIndex].wcsString )
            delete[] m_wcsStringCache[wcsCacheLastIndex].wcsString;

        m_wcsStringCache[wcsCacheLastIndex].wcsString = new wchar_t[mbstrlen+1];
        m_wcsStringCache[wcsCacheLastIndex].wcsLen =  mbstrlen;
    }
    int count = ut_utf8_to_unicode((char*)(m_data + m_pos), mbstrlen, m_wcsStringCache[wcsCacheLastIndex].wcsString, mbstrlen);

    _ASSERT(count > 0 && (unsigned int)count < mbstrlen);

    m_pos += mbstrlen;
    return m_wcsStringCache[wcsCacheLastIndex].wcsString;
}

//deserializes a FdoDateTime
FdoDateTime BinaryReader::ReadDateTime()
{
    FdoDateTime ret;

    ret.year = ReadInt16();
    ret.month = ReadChar();
    ret.day = ReadChar();
    ret.hour = ReadChar();
    ret.minute = ReadChar();
    ret.seconds = ReadSingle();

    return ret;
}


FdoDataValue* BinaryReader::ReadDataValue()
{
    FdoDataType dataValueType = (FdoDataType)ReadByte();
    FdoPtr<FdoDataValue> retDataValue;
    bool bIsNull = ReadByte() != 0;

    switch (dataValueType)
    {
        case FdoDataType_Boolean:
        {
            FdoPtr<FdoBooleanValue> dataValue = FdoBooleanValue::Create();
            if (!bIsNull)
                dataValue->SetBoolean(ReadByte() != 0);
            retDataValue = FDO_SAFE_ADDREF(dataValue.p);
        }
        break;

        case FdoDataType_Byte:
        {
            FdoPtr<FdoByteValue> dataValue = FdoByteValue::Create();
            if (!bIsNull)
                dataValue->SetByte(ReadByte());
            retDataValue = FDO_SAFE_ADDREF(dataValue.p);
        }
        break;

        case FdoDataType_DateTime:
        {
            FdoPtr<FdoDateTimeValue> dataValue = FdoDateTimeValue::Create();
            if (!bIsNull)
                dataValue->SetDateTime(ReadDateTime());
            retDataValue = FDO_SAFE_ADDREF(dataValue.p);
        }
        break;

        case FdoDataType_Decimal:
        {
            FdoPtr<FdoDecimalValue> dataValue = FdoDecimalValue::Create();
            if (!bIsNull)
                dataValue->SetDecimal(ReadDouble());
            retDataValue = FDO_SAFE_ADDREF(dataValue.p);
        }
        break;

        case FdoDataType_Double:
        {
            FdoPtr<FdoDoubleValue> dataValue = FdoDoubleValue::Create();
            if (!bIsNull)
                dataValue->SetDouble(ReadDouble());
            retDataValue = FDO_SAFE_ADDREF(dataValue.p);
        }
        break;

        case FdoDataType_Int16:
        {
            FdoPtr<FdoInt16Value> dataValue = FdoInt16Value::Create();
            if (!bIsNull)
                dataValue->SetInt16(ReadInt16());
            retDataValue = FDO_SAFE_ADDREF(dataValue.p);
        }
        break;

        case FdoDataType_Int32:
        {
            FdoPtr<FdoInt32Value> dataValue = FdoInt32Value::Create();
            if (!bIsNull)
                dataValue->SetInt32(ReadInt32());
            retDataValue = FDO_SAFE_ADDREF(dataValue.p);
        }
        break;

        case FdoDataType_Int64:
        {
            FdoPtr<FdoInt64Value> dataValue = FdoInt64Value::Create();
            if (!bIsNull)
                dataValue->SetInt64(ReadInt64());
            retDataValue = FDO_SAFE_ADDREF(dataValue.p);
        }
        break;

        case FdoDataType_Single:
        {
            FdoPtr<FdoSingleValue> dataValue = FdoSingleValue::Create();
            if (!bIsNull)
                dataValue->SetSingle(ReadSingle());
            retDataValue = FDO_SAFE_ADDREF(dataValue.p);
        }
        break;

        case FdoDataType_String:
        {
            FdoPtr<FdoStringValue> dataValue = FdoStringValue::Create();
            if (!bIsNull)
                dataValue->SetString(ReadString());
            retDataValue = FDO_SAFE_ADDREF(dataValue.p);
        }
        break;

        case SDF_UNDEFINED_DATATYPE:
            return NULL;
            break;             


        // Unsupported types:
        case FdoDataType_BLOB:
        case FdoDataType_CLOB:

        default:
            throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_20_SCHEMA_STORAGE_ERROR)));
        break;
    }

    // Set isnull:
    if (bIsNull)
        retDataValue->SetNull();

    return FDO_SAFE_ADDREF(retDataValue.p);
}

const wchar_t* BinaryReader::ReadRawStringNoCache(unsigned mbstrlen)
{    

    //make sure wchar_t cache size is big enough
    if (m_wcsCacheLen < mbstrlen + 1)
    {
		if( m_wcsCache != NULL )
			delete[] m_wcsCache;
		m_wcsCache = NULL;
        //pick size -- minimum size is STRING_BUFFER_SIZE
        m_wcsCacheLen = max(SDF_STRING_CACHE_SIZE, mbstrlen + 1);
        m_wcsCache = new wchar_t[m_wcsCacheLen];
    }

    //handle empty string
    if (mbstrlen <= 1)
    {
        //skip past null character
        m_pos += mbstrlen;

        return L"";
    }
        
    //Note: we pass in m_len as number of characters to read, but we know
    //the string must be null terminated, so the function will terminate before that
    int count = ut_utf8_to_unicode((char*)(m_data + m_pos), mbstrlen, m_wcsCache, mbstrlen);

    _ASSERT(count > 0 && (unsigned int)count < mbstrlen);

    //increment position count to position after string
    //note that "count" is the number of unicode characters
    //read, not the number of bytes we need to increment.
    m_pos += mbstrlen; 

    return m_wcsCache;
}
