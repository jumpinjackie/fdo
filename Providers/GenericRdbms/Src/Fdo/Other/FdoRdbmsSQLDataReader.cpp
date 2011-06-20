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
 */
#include "stdafx.h"
#include <assert.h>
#include "FdoRdbmsConnection.h"
#include "FdoRdbmsSQLDataReader.h"

#include "FdoRdbmsUtil.h"
#include "FdoCommonOSUtil.h"
#include "FdoRdbmsSimpleBLOBStreamReader.h"

static  char*  noMoreRows = "End of rows or ReadNext not called"; // error message that repeats
static  char  *strNUllColumnExp = "Column '%1$ls' value is NULL; use IsNull method before trying to access this column value";

#define GET_VALUE( type, colIdx, FN ) { \
    type    value; \
    bool  isNULL = false; \
    if( ! mHasMoreRows ) \
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_62, noMoreRows)); \
    ValidateIndex(colIdx);\
    value = FN(colIdx+1, &isNULL, NULL); \
    if( isNULL ) \
        throw FdoCommandException::Create(NlsMsgGet1( FDORDBMS_386, strNUllColumnExp,  mColList[colIdx].column)); \
    return value; \
}

FdoRdbmsSQLDataReader::FdoRdbmsSQLDataReader( FdoIConnection *connection, GdbiQueryResult   *query ):
mQueryResult( query ),
mFdoConnection( NULL ),
mConnection( NULL ),
mHasMoreRows( false ),
mColCount( 0 ),
mColList( NULL ),
mWkbBuffer(NULL),
mWkbBufferLen(0),
mWkbGeomLen(0),
mSprops(NULL),
mGeomIdx(-1),
mUppNameBuffer(NULL),
mUppNameBufferLen(0)
{

    mFdoConnection = dynamic_cast<FdoRdbmsConnection*>(connection);
    if( mFdoConnection )
    {
        mFdoConnection->AddRef();
        mConnection = mFdoConnection->GetDbiConnection();
    }

    mColCount = mQueryResult->GetColumnCount();
    if( mColCount == 0 )
        return;

    mColList = new GdbiColumnDesc[mColCount];

    for ( int i=0; i<mColCount; i++ )
    {
        GdbiColumnDesc* pCol = &mColList[i];
        if( !mQueryResult->GetColumnDesc( i+1, *pCol ) )
            pCol->column[0] = L'\0';
        
        if (*pCol->column == L'\0')
            wcsncpy(pCol->column, L"GeneratedProperty", 18);

        if (mColMap.find(pCol->column) != mColMap.end())
            GenerateUniqueName(pCol->column, pCol->column);
        mColMap[pCol->column] = std::make_pair(pCol, i);
    }
    mSprops = new StringRec[mColCount];
}

FdoRdbmsSQLDataReader::~FdoRdbmsSQLDataReader()
{
    Close();

    delete mQueryResult;
    delete[] mColList;

    if( mFdoConnection )
        mFdoConnection->Release();
	
    delete[] mSprops;
    delete[] mWkbBuffer;
    
    delete[] mUppNameBuffer;
}

wchar_t* FdoRdbmsSQLDataReader::GenerateUniqueName(const wchar_t* name, wchar_t* dest, int maxLen)
{
    int idxProp = 1;
    int idx = 0;
    wchar_t buffer[5];
    
    int propsize = wcslen(name);
    if ((propsize + 1 + 4) <= maxLen) // from 1 to max 999
    {
        *(dest+propsize) = L'$';
        do
        {
            swprintf(buffer, 5, L"%d", idxProp);
            idx = 0;
            while(buffer[idx] != L'\0')
                *(dest+propsize+1+idx) = buffer[idx++];
            *(dest+propsize+1+idx) = L'\0';
            idxProp++;
        }
        while(mColMap.find(dest) != mColMap.end());
    }
    else // we cannot generate the name (too long)
    {
        *dest = L'G';
        *(dest+1) = L'$';
        do
        {
            swprintf(buffer, 5, L"%d", idxProp);
            idx = 0;
            while(buffer[idx] != L'\0')
                *(dest+2+1+idx) = buffer[idx++];
            *(dest+2+1+idx) = L'\0';
            idxProp++;
        }
        while(mColMap.find(dest) != mColMap.end());
    }
    return dest;
}

FdoInt32 FdoRdbmsSQLDataReader::GetColumnCount()
{
    return mColCount;
}

const wchar_t* FdoRdbmsSQLDataReader::GetColumnName(FdoInt32 index)
{
    ValidateIndex(index);
    return mColList[index].column;
}

FdoInt32 FdoRdbmsSQLDataReader::GetColumnIndex(const wchar_t* columnName)
{
    return NameToIndex(columnName);
}

FdoDataType FdoRdbmsSQLDataReader::GetColumnType( const wchar_t* columnName )
{
    return FdoRdbmsUtil::DbiToFdoType( mColList[NameToIndex(columnName)].datatype );
}

FdoDataType FdoRdbmsSQLDataReader::GetColumnType(FdoInt32 index)
{
    ValidateIndex(index);
    return FdoRdbmsUtil::DbiToFdoType( mColList[index].datatype );
}

FdoPropertyType FdoRdbmsSQLDataReader::GetPropertyType(const wchar_t* columnName)
{
    return FdoRdbmsSQLDataReader::GetPropertyType(NameToIndex(columnName));
}

FdoPropertyType FdoRdbmsSQLDataReader::GetPropertyType(FdoInt32 index)
{
    ValidateIndex(index);
    if( mColList[index].datatype == RDBI_GEOMETRY )
        return FdoPropertyType_GeometricProperty;
    else
        return FdoPropertyType_DataProperty;
}

bool FdoRdbmsSQLDataReader::GetBoolean(const wchar_t* columnName)
{
    return FdoRdbmsSQLDataReader::GetBoolean(NameToIndex(columnName));
}

FdoBoolean FdoRdbmsSQLDataReader::GetBoolean(FdoInt32 index)
{
    GET_VALUE (bool, index, mQueryResult->GetBoolean);
}

FdoByte FdoRdbmsSQLDataReader::GetByte(FdoInt32 index)
{
    return (FdoByte)FdoRdbmsSQLDataReader::GetInt16(index);
}

FdoByte FdoRdbmsSQLDataReader::GetByte(const wchar_t* columnName)
{
    return (FdoByte)FdoRdbmsSQLDataReader::GetInt16(NameToIndex(columnName));
}

FdoDateTime FdoRdbmsSQLDataReader::GetDateTime(const wchar_t* columnName)
{
    return mFdoConnection->DbiToFdoTime(FdoRdbmsSQLDataReader::GetString(NameToIndex(columnName)));
}

FdoDateTime FdoRdbmsSQLDataReader::GetDateTime(FdoInt32 index)
{
    return mFdoConnection->DbiToFdoTime(FdoRdbmsSQLDataReader::GetString(index));
}

double FdoRdbmsSQLDataReader::GetDouble(const wchar_t* columnName)
{
    return FdoRdbmsSQLDataReader::GetDouble(NameToIndex(columnName));
}

double FdoRdbmsSQLDataReader::GetDouble(FdoInt32 index)
{
    GET_VALUE (double, index, mQueryResult->GetDouble);
}

FdoInt16 FdoRdbmsSQLDataReader::GetInt16(const wchar_t* columnName)
{
    return FdoRdbmsSQLDataReader::GetInt16(NameToIndex(columnName));
}

FdoInt16 FdoRdbmsSQLDataReader::GetInt16(FdoInt32 index)
{
    GET_VALUE (short, index, mQueryResult->GetInt16);
}

FdoInt32 FdoRdbmsSQLDataReader::GetInt32(const wchar_t* columnName)
{
    return FdoRdbmsSQLDataReader::GetInt32(NameToIndex(columnName));
}

FdoInt32 FdoRdbmsSQLDataReader::GetInt32(FdoInt32 index)
{
    GET_VALUE (int, index, mQueryResult->GetInt32);
}

FdoInt64 FdoRdbmsSQLDataReader::GetInt64(const wchar_t* columnName)
{
    return FdoRdbmsSQLDataReader::GetInt64(NameToIndex(columnName));
}

FdoInt64 FdoRdbmsSQLDataReader::GetInt64(FdoInt32 index)
{
    GET_VALUE (FdoInt64, index, mQueryResult->GetInt64);
}

float FdoRdbmsSQLDataReader::GetSingle(const wchar_t* columnName)
{
    return FdoRdbmsSQLDataReader::GetSingle(NameToIndex(columnName));
}

float FdoRdbmsSQLDataReader::GetSingle(FdoInt32 index)
{
    GET_VALUE (float, index, mQueryResult->GetFloat);
}

const wchar_t* FdoRdbmsSQLDataReader::GetString(const wchar_t* columnName)
{
    return FdoRdbmsSQLDataReader::GetString(NameToIndex(columnName));
}

const wchar_t* FdoRdbmsSQLDataReader::GetString(FdoInt32 index)
{
    bool isNULL = false;
    if (! mHasMoreRows)
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_62, noMoreRows));
    ValidateIndex(index);

    int i = index;
	if (mSprops[i].valid)
		return mSprops[i].data;

    const wchar_t* tmpVal = NULL;
    if (mColList[index].datatype == RDBI_WSTRING_ULEN)
    {
        FdoByteArray* arr = NULL;
        mQueryResult->GetBinaryValue (index+1, sizeof(FdoByteArray*), (char*)&arr, &isNULL, NULL);
        if (!isNULL && arr != NULL && arr->GetCount() != 0)
        {
            int sizeW = (int)(arr->GetCount()/sizeof(wchar_t));
            mSprops[i].EnsureSize(sizeW + 1);
            memcpy(mSprops[i].data, arr->GetData(), arr->GetCount());
            *(mSprops[i].data+sizeW) = L'\0';
            mSprops[i].valid = 1;
            return mSprops[i].data;
        }
    }
    else if (mColList[index].datatype == RDBI_STRING_ULEN)
    {
        FdoByteArray* arr = NULL;
        mQueryResult->GetBinaryValue (index+1, sizeof(FdoByteArray*), (char*)&arr, &isNULL, NULL);
        if (!isNULL && arr != NULL && arr->GetCount() != 0)
        {
            int cntArr = arr->GetCount();
            mSprops[i].EnsureSize(2*cntArr + 1);
            char* startCh = (char*)(mSprops[i].data+cntArr);
            memcpy(startCh, arr->GetData(), cntArr);
            *(startCh+cntArr) = L'\0'; // add string ending
            ut_utf8_to_unicode(startCh, mSprops[i].data, cntArr+1);
            mSprops[i].valid = 1;
            return mSprops[i].data;
        }
    }
    else
        tmpVal = mQueryResult->GetString(index+1, &isNULL, NULL);

    if (isNULL || tmpVal == NULL)
    {
        mSprops[i].EnsureSize(1);
        mSprops[i].data = L'\0';
        mSprops[i].valid = 1;
        throw FdoCommandException::Create(NlsMsgGet1( FDORDBMS_386, strNUllColumnExp, mColList[index].column ));
    }
    
    size_t sz = wcslen(tmpVal);
    mSprops[i].EnsureSize(sz+1);
    wcscpy(mSprops[i].data, tmpVal);
    mSprops[i].valid = 1;

    return mSprops[i].data;
}

FdoIStreamReader* FdoRdbmsSQLDataReader::GetLOBStreamReader(const wchar_t* columnName)
{
    return FdoRdbmsSQLDataReader::GetLOBStreamReader(NameToIndex(columnName));
}

FdoIStreamReader* FdoRdbmsSQLDataReader::GetLOBStreamReader(FdoInt32 index)
{
    FdoPtr<FdoLOBValue> blob = GetLOB(index);
    return new FdoRdbmsSimpleBLOBStreamReader(blob); 
}

FdoLOBValue* FdoRdbmsSQLDataReader::GetLOB(const wchar_t* columnName)
{
    return FdoRdbmsSQLDataReader::GetLOB(NameToIndex(columnName));
}

FdoLOBValue* FdoRdbmsSQLDataReader::GetLOB(FdoInt32 index)
{
    ValidateIndex(index);

    bool isNull = false;
    if (mColList[index].datatype == RDBI_BLOB_ULEN)
    {
        FdoByteArray* arr = NULL;
        mQueryResult->GetBinaryValue (index+1, sizeof(FdoByteArray*), (char*)&arr, &isNull, NULL);
        if (!isNull && arr != NULL && arr->GetCount() != 0)
            return static_cast<FdoLOBValue*>(FdoDataValue::Create(arr->GetData(), arr->GetCount(), FdoDataType_BLOB));
    }
    else if (mColList[index].size > 0)
    {
        FdoByte* vblob = new FdoByte[mColList[index].size];
        try
        {
            mQueryResult->GetBinaryValue (index+1, mColList[index].size, (char*)vblob, &isNull, NULL);
            FdoLOBValue* retVal = (!isNull) ? (FdoLOBValue*)FdoDataValue::Create(vblob, mColList[index].size, FdoDataType_BLOB) : NULL;
            delete[] vblob;
            return retVal;
        }
        catch(...)
        {
            delete[] vblob;
            throw;
        }
    }

    throw FdoCommandException::Create(NlsMsgGet1( FDORDBMS_386, strNUllColumnExp, mColList[index].column ));
}

bool FdoRdbmsSQLDataReader::IsNull(const wchar_t* columnName)
{
    return FdoRdbmsSQLDataReader::IsNull(NameToIndex(columnName));
}

bool FdoRdbmsSQLDataReader::IsNull(FdoInt32 index)
{
    bool isNull = true;
    if( ! mHasMoreRows )
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_62, noMoreRows));
    ValidateIndex(index);

    switch(mColList[index].datatype)
    {
    case RDBI_GEOMETRY:
        {
            FdoInt32 len = 0;
            GetGeometry(index, &len, true);
            isNull = (len == 0);
        }
        break;
    case RDBI_BLOB_ULEN:
    case RDBI_WSTRING_ULEN:
    case RDBI_STRING_ULEN:
        {
            FdoByteArray* arr = NULL;
            bool isNullArr = false;
            mQueryResult->GetBinaryValue (index+1, sizeof(FdoByteArray*), (char*)&arr, &isNullArr, NULL);
            isNull = (isNullArr || arr == NULL || arr->GetCount() == 0);
        }
        break;
    default:
        isNull = mQueryResult->GetIsNull(index+1);
        break;
    }
    return isNull;
}

FdoByteArray* FdoRdbmsSQLDataReader::GetGeometry(const wchar_t* propertyName)
{
	int len = 0;
    const void* ptr = FdoRdbmsSQLDataReader::GetGeometry(NameToIndex(propertyName), &len);
    return (len) ? FdoByteArray::Create((unsigned char*)ptr, len) : NULL;
}

FdoByteArray* FdoRdbmsSQLDataReader::GetGeometry(FdoInt32 index)
{
	int len = 0;
    const void* ptr = FdoRdbmsSQLDataReader::GetGeometry(index, &len);
    return (len) ? FdoByteArray::Create((unsigned char*)ptr, len) : NULL;
}

const void* FdoRdbmsSQLDataReader::GetGeometry(FdoInt32 index, FdoInt32* len, bool noExcOnInvalid)
{
    ValidateIndex(index);
    if( ! mHasMoreRows )
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_62, noMoreRows));

    if (mGeomIdx != index)
    {
        FdoIGeometry* geom = NULL;
        bool isSupportedType = false;
        bool isNull = false;

        mGeomIdx = index;
        if (mWkbBuffer)
            *mWkbBuffer = 0;
        mQueryResult->GetBinaryValue (index+1, sizeof(FdoIGeometry *), (char*)&geom, &isNull, NULL);

        if ( !isNull && geom && geom->GetDerivedType() != FdoGeometryType_None )
            isSupportedType = true;

        if ( !isNull && geom != NULL )
        {
            if ( isSupportedType )
            {
                FdoPtr<FdoFgfGeometryFactory>  gf = FdoFgfGeometryFactory::GetInstance();
                FdoPtr<FdoByteArray> fgf = gf->GetFgf(geom);
                if (fgf != NULL && fgf->GetCount() != 0)
                {
                    mWkbGeomLen = fgf->GetCount();
                    if (mWkbBufferLen < mWkbGeomLen)
                    {
                        delete[] mWkbBuffer;
                        mWkbBufferLen = mWkbGeomLen;
                        mWkbBuffer = new unsigned char[mWkbGeomLen];
                    }
                    memcpy(mWkbBuffer, fgf->GetData(), mWkbGeomLen);
                }
                else
                    mWkbGeomLen = 0;
            }
            else
                mWkbGeomLen = -1;
        }
        else // isNull indicator is not set by dbi_get_val_b for geometry columns
            mWkbGeomLen = 0;
    }
    *len = mWkbGeomLen;
    if (*len <= 0)
    {
        if (noExcOnInvalid)
            return NULL;
        else if (*len == 0)
            throw FdoCommandException::Create(NlsMsgGet1( FDORDBMS_385, "Property '%1$ls' value is NULL; use IsNull method before trying to access the property value", mColList[index].column ));
        else
            throw FdoCommandException::Create( NlsMsgGet(FDORDBMS_116, "Unsupported geometry type" ) );
    }

    return mWkbBuffer;
}

bool FdoRdbmsSQLDataReader::ReadNext()
{
    if( mQueryResult == NULL )
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_43, "Query ended"));

    mHasMoreRows = false;
    mGeomIdx = -1;

    if ( ! mQueryResult->ReadNext() )
    {
        Close();
        return false;
    }

 	for (int i=0; i < mColCount; i++)
		mSprops[i].valid = 0;

    return (mHasMoreRows = true);
}

void FdoRdbmsSQLDataReader::Close()
{
    // Free all the cursors
    if ( mQueryResult != NULL )
    {
        mHasMoreRows = false;
        mQueryResult->Close();
        delete mQueryResult;
        mQueryResult = NULL;
    }
}

