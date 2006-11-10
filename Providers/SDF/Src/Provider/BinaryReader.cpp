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

BinaryReader::BinaryReader(unsigned char* data, int len)
{
	Init();
    m_data = data;
    m_len = len;
}

BinaryReader::BinaryReader()
{
    Init();
}

void BinaryReader::Init()
{
	m_data = NULL;
    m_len = 0;
    m_pos = 0;
    m_wcsCacheLen = 0;
    m_wcsCacheCurrent = 0;
    m_wcsCache = NULL;
}

BinaryReader::~BinaryReader()
{
    //if we used extra string buffers, clear them
    if (!m_stringCaches.empty())
    {
        for (std::list<wchar_t*>::iterator iter=m_stringCaches.begin(); 
            iter != m_stringCaches.end(); iter++)
            delete [] (*iter);
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
    
    if (!(m_wcsCachedStrings).empty())
    {
        (m_wcsCachedStrings).clear();
    }

    //if we used extra string buffers, clear them
    //and only reuse the last one (pointed to by m_wcsCache)
    if (!m_stringCaches.empty())
    {
        for (std::list<wchar_t*>::iterator iter=m_stringCaches.begin(); 
            iter != m_stringCaches.end(); iter++)
            delete [] (*iter);

		 m_stringCaches.clear();
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
    _ASSERT(m_pos <= m_len - (int)sizeof(double));

    //TODO: endian
    double ret = *(double*)(m_data + m_pos);
    m_pos += sizeof(double);
    return ret;
}

float BinaryReader::ReadSingle()
{
    _ASSERT(m_pos <= m_len - (int)sizeof(float));

    //TODO: endian
    float ret = *(float*)(m_data + m_pos);
    m_pos += sizeof(float);
    return ret;
}

int BinaryReader::ReadInt32()
{
    _ASSERT(m_pos <= m_len - (int)sizeof(int));

    //TODO: endian
    int ret = *(int*)(m_data + m_pos);
    m_pos += sizeof(int);
    return ret;
}

unsigned BinaryReader::ReadUInt32()
{
    _ASSERT(m_pos <= m_len - (int)sizeof(int));

    //TODO: endian
    unsigned ret = *(unsigned*)(m_data + m_pos);
    m_pos += sizeof(unsigned);
    return ret;
}

short BinaryReader::ReadInt16()
{
    _ASSERT(m_pos <= m_len - (int)sizeof(short));

    //TODO: endian
    short ret = *(short*)(m_data + m_pos);
    m_pos += sizeof(short);
    return ret;
}

unsigned short BinaryReader::ReadUInt16()
{
    _ASSERT(m_pos <= m_len - (int)sizeof(unsigned short));

    //TODO: endian
    unsigned short ret = *(unsigned short*)(m_data + m_pos);
    m_pos += sizeof(unsigned short);
    return ret;
}

FdoInt64 BinaryReader::ReadInt64()
{
    _ASSERT(m_pos <= m_len - (int)sizeof(FdoInt64));

    //TODO: endian
    FdoInt64 ret = *(FdoInt64*)(m_data + m_pos);
    m_pos += sizeof(FdoInt64);
    return ret;
}


unsigned char BinaryReader::ReadByte()
{
    _ASSERT(m_pos <= m_len - (int)sizeof(unsigned char));

    //TODO: endian
    unsigned char ret = *(m_data + m_pos);
    m_pos += sizeof(unsigned char);
    return ret;
}

char BinaryReader::ReadChar()
{
    _ASSERT(m_pos <= m_len - (int)sizeof(char));

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


const wchar_t* BinaryReader::ReadRawString(unsigned mbstrlen)
{    
    
    //check if we have read this string already (it will be cached)
    wchar_t* thestring = (m_wcsCachedStrings)[m_pos];
    if (thestring != NULL)
	{
		m_pos += mbstrlen;
        return thestring;
	}

    //make sure wchar_t cache size is big enough
    if (m_wcsCacheLen - m_wcsCacheCurrent < mbstrlen + 1)
    {
        //pick size -- minimum size is STRING_BUFFER_SIZE
        m_wcsCacheLen = max(STRING_CACHE_SIZE, m_wcsCacheCurrent + mbstrlen + 1);
        wchar_t* tmp = new wchar_t[m_wcsCacheLen];

        if (m_wcsCache)
        {
            //if we already had a buffer, keep track of it
            //in the list, but set the new current buffer 
            //to the newly allocated one
            m_stringCaches.push_back(m_wcsCache);
            m_wcsCache = tmp;
        }
        else
        {
            //otherwise just keep track of cache buffer pointer
            m_wcsCache = tmp;
        }
    }

    //handle empty string
    if (mbstrlen <= 1)
    {
        m_wcsCache[m_wcsCacheCurrent] = 0;
        (m_wcsCachedStrings)[m_pos] = m_wcsCache + m_wcsCacheCurrent;
        m_wcsCacheCurrent += 1;

        //skip past null character
        m_pos += mbstrlen;

        return m_wcsCache + m_wcsCacheCurrent - 1;
    }
        
    //Note: we pass in m_len as number of characters to read, but we know
    //the string must be null terminated, so the function will terminate before that
    int count = ut_utf8_to_unicode((char*)(m_data + m_pos), mbstrlen, m_wcsCache + m_wcsCacheCurrent, mbstrlen);

    _ASSERT(count > 0 && (unsigned int)count < mbstrlen);

    //remember offset of current string
    unsigned offset = m_wcsCacheCurrent;
    (m_wcsCachedStrings)[m_pos] = m_wcsCache + offset;

    //increment position count to position after string
    //note that "count" is the number of unicode characters
    //read, not the number of bytes we need to increment.
    m_pos += mbstrlen; 

    //remember where to put next string that we read
    m_wcsCacheCurrent += (int)wcslen(m_wcsCache + m_wcsCacheCurrent) + 1;

    return m_wcsCache + offset;
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
        m_wcsCacheLen = max(STRING_CACHE_SIZE, mbstrlen + 1);
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
