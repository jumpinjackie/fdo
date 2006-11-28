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

static  char*  noMoreRows = "End of rows or ReadNext not called"; // error message that repeats
static  char  *strNUllColumnExp = "Column '%1$ls' value is NULL; use IsNull method before trying to access this column value";


#define GET_VALUE( type, colName, FN ) { \
    type    value; \
    bool  isNULL = false; \
    if( ! mHasMoreRows ) \
    { \
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_62, noMoreRows)); \
    } \
    try \
    { \
        value = FN((char*)mConnection->GetUtility()->UnicodeToUtf8( colName), &isNULL, NULL); \
        if( isNULL ) \
            throw FdoCommandException::Create(NlsMsgGet1( FDORDBMS_386, strNUllColumnExp,  colName )); \
    } \
    catch ( char * ) \
    { \
        FindColumnIndex(colName); /*throws a column not found exception*/ \
        throw; \
    } \
    return value; \
}

FdoRdbmsSQLDataReader::FdoRdbmsSQLDataReader( FdoIConnection *connection, GdbiQueryResult   *query ):
mQueryResult( query ),
mFdoConnection( NULL ),
mConnection( NULL ),
mHasMoreRows( false ),
mColCount( 0 ),
mColList( NULL )
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
        if( ! mQueryResult->GetColumnDesc( i+1, mColList[i] ) )
            mColList[i].column[0] = '\0';
    }

}

FdoRdbmsSQLDataReader::~FdoRdbmsSQLDataReader()
{
    Close();

    if( mQueryResult )
        delete mQueryResult;

    if( mColList != NULL )
        delete[] mColList;

    if( mFdoConnection )
        mFdoConnection->Release();
}

FdoInt32 FdoRdbmsSQLDataReader::GetColumnCount()
{
    return mColCount;
}

const wchar_t* FdoRdbmsSQLDataReader::GetColumnName(FdoInt32 index)
{
    if ( index >= mColCount )
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_52, "Index out of range"));

    return mConnection->GetUtility()->Utf8ToUnicode(mColList[index].column);
}

int FdoRdbmsSQLDataReader::FindColumnIndex( const wchar_t* columnName )
{
    const char  *tmpColName = mConnection->GetUtility()->UnicodeToUtf8( columnName );
    int i;

    for( i=0; i<mColCount; i++ )
    {
#ifdef _WIN32
        if( _stricmp( tmpColName, mColList[i].column ) == 0 )
#else
        if( strcasecmp( tmpColName, mColList[i].column ) == 0 )
#endif
            break;
    }
    if( i == mColCount )
        throw FdoCommandException::Create(NlsMsgGet1(FDORDBMS_42, "Column %1$ls not found", columnName));

    return i;
}

FdoDataType FdoRdbmsSQLDataReader::GetColumnType( const wchar_t* columnName )
{
    return FdoRdbmsUtil::DbiToFdoType( mColList[FindColumnIndex(columnName)].datatype );
}

FdoPropertyType FdoRdbmsSQLDataReader::GetPropertyType(FdoString* columnName)
{
    if( mColList[FindColumnIndex(columnName)].datatype == RDBI_GEOMETRY )
        return FdoPropertyType_GeometricProperty;
    else
        return FdoPropertyType_DataProperty;
}

bool FdoRdbmsSQLDataReader::GetBoolean(const wchar_t* columnName)
{
    return ( GetInt16( columnName ) != 0 );
}


FdoByte FdoRdbmsSQLDataReader::GetByte(const wchar_t* columnName)
{
    return (FdoByte)GetInt16( columnName );
}

FdoDateTime FdoRdbmsSQLDataReader::GetDateTime(const wchar_t* columnName)
{
    return mFdoConnection->DbiToFdoTime( mConnection->GetUtility()->UnicodeToUtf8( GetString( columnName ) ) );
}


double FdoRdbmsSQLDataReader::GetDouble(const wchar_t* columnName)
{
    GET_VALUE( double, columnName, mQueryResult->GetDouble );
}


FdoInt16 FdoRdbmsSQLDataReader::GetInt16(const wchar_t* columnName)
{
    GET_VALUE( short, columnName, mQueryResult->GetInt16 )
}


FdoInt32 FdoRdbmsSQLDataReader::GetInt32(const wchar_t* columnName)
{
    GET_VALUE( int, columnName, mQueryResult->GetInt32 )
}


FdoInt64 FdoRdbmsSQLDataReader::GetInt64(const wchar_t* columnName)
{
    GET_VALUE( FdoInt64, columnName, mQueryResult->GetInt64 ) // DBI should support a 64 bit long. As it stands now we have to deal with long(32bits).
}


float FdoRdbmsSQLDataReader::GetSingle(const wchar_t* columnName)
{
    GET_VALUE( float, columnName, mQueryResult->GetFloat )
}

const wchar_t* FdoRdbmsSQLDataReader::GetString(const wchar_t* columnName)
{
    wchar_t *colValue = NULL;
    bool isNULL = false;
    if( ! mHasMoreRows )
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_62, noMoreRows));

    try
    {
        char *colName = (char *) mConnection->GetUtility()->UnicodeToUtf8( columnName);
        const wchar_t* tmpVal = mQueryResult->GetString( colName,&isNULL, NULL);
        if( isNULL )
            throw FdoCommandException::Create(NlsMsgGet1( FDORDBMS_386, strNUllColumnExp, columnName ));

        colValue = mStringMap.AddtoMap(colName, tmpVal, mConnection->GetUtility());
    }
    catch ( FdoCommandException* exc )
    {
        try
        {
            FindColumnIndex( columnName ); /*throws a column not found exception*/
            throw exc;
        }
        catch ( FdoCommandException* exc2 )
        {
            exc->Release();
            throw exc2;
        }
        catch ( FdoException* exc2 )
        {
            exc->Release();
            throw exc2;
        }
    }
    catch ( FdoException* exc )
    {
        try
        {
            FindColumnIndex( columnName ); /*throws a column not found exception*/
            throw exc;
        }
        catch ( FdoCommandException* exc2 )
        {
            exc->Release();
            throw exc2;
        }
        catch ( FdoException* exc2 )
        {
            exc->Release();
            throw exc2;
        }
    }
    catch ( ... )
    {
        // Try to throw a better exception, otherwise re-throw the dbi exception
        // Note that we only do this when something goes wrong for performance reason; we don't want all that agly
        // string compares to happen every time.
        FindColumnIndex( columnName ); /*throws a column not found exception*/

        throw;
    }

    return colValue;
}


FdoIStreamReader* FdoRdbmsSQLDataReader::GetLOBStreamReader(const wchar_t* columnName)
{
    void    *pLOBLocator;
    bool	isNULL = false;

    if( ! mHasMoreRows )
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_62, noMoreRows));

    try
    {
        mQueryResult->GetBinaryValue(  columnName, sizeof(void *), (char *)&pLOBLocator, &isNULL, NULL);
		if( isNULL )
            throw FdoCommandException::Create(NlsMsgGet1( FDORDBMS_386, strNUllColumnExp,  columnName ));
    }
    catch ( FdoCommandException* exc )
    {
        try
        {
            FindColumnIndex( columnName ); /*throws a column not found exception*/
            throw exc;
        }
        catch ( FdoCommandException* exc2 )
        {
            exc->Release();
            throw exc2;
        }
        catch ( FdoException* exc2 )
        {
            exc->Release();
            throw exc2;
        }
    }
    catch ( FdoException* exc )
    {
        try
        {
            FindColumnIndex( columnName ); /*throws a column not found exception*/
            throw exc;
        }
        catch ( FdoCommandException* exc2 )
        {
            exc->Release();
            throw exc2;
        }
        catch ( FdoException* exc2 )
        {
            exc->Release();
            throw exc2;
        }
    }
    catch ( ... )
    {
        FindColumnIndex( columnName ); /*throws a column not found exception*/
        throw;
    }

    // Assume BLOB
    assert(false);
    throw "FIXME please: FdoRdbmsSQLDataReader::GetLOBStreamReader";
//    return FdoRdbmsBLOBStreamReader::Create(mFdoConnection, mQid, pLOBLocator );
}

FdoLOBValue* FdoRdbmsSQLDataReader::GetLOB(const wchar_t* columnName)
{
    // Assume BLOB
    FdoRdbmsBLOBStreamReader* blobReader = (FdoRdbmsBLOBStreamReader *)GetLOBStreamReader( columnName );

    FdoInt32  size = (FdoInt32) blobReader->GetLength();
    FdoByteArray    * byteArray =  FdoByteArray::Create( size );

    if ( size != 0 )
        blobReader->ReadNext( byteArray, 0, -1 );

    FdoBLOBValue *blobValue = FdoBLOBValue::Create( byteArray );
    return blobValue;
}


bool FdoRdbmsSQLDataReader::IsNull(const wchar_t* columnName)
{
    int     isNull = 0;

    if( ! mHasMoreRows )
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_62, noMoreRows));

    try
    {
        if( mColList[FindColumnIndex(columnName)].datatype == RDBI_GEOMETRY )
        {
            FdoPtr<FdoByteArray>    geom = GetGeometry( columnName, true );
            isNull = ( geom == NULL );
        }
        else
        {
            return mQueryResult->GetIsNull( (char *)mConnection->GetUtility()->UnicodeToUtf8( columnName) );
        }
    }
    catch ( FdoCommandException* exc )
    {
        try
        {
            // Make sure the column exists
            FindColumnIndex( columnName ); /*throws a column not found exception*/
            exc->Release();
        }
        catch ( FdoCommandException* exc2 )
        {
            throw exc2;
        }
        catch ( FdoException* exc2 )
        {
            throw exc2;
        }
        // In this case this is NULL
        return true;
    }
    catch ( FdoException* exc )
    {
        try
        {
            // Make sure the column exists
            FindColumnIndex( columnName ); /*throws a column not found exception*/
            exc->Release();
        }
        catch ( FdoCommandException* exc2 )
        {
            throw exc2;
        }
        catch ( FdoException* exc2 )
        {
            throw exc2;
        }
        // In this case this is NULL
        return true;
    }
    catch ( ... )
    {
        // Make sure the column exists
        FindColumnIndex( columnName ); /*throws a column not found exception*/

        // In this case this is NULL
        return true;
    }

    return (isNull != 0);
}

FdoByteArray* FdoRdbmsSQLDataReader::GetGeometry(const wchar_t* propertyName)
{
    return GetGeometry( propertyName, false );
}

FdoByteArray* FdoRdbmsSQLDataReader::GetGeometry(const wchar_t* columnName, bool checkIsNullOnly)
{
    FdoIGeometry    *geom = NULL;
    FdoByteArray   *byteArray = NULL;
    bool            isSupportedType = false;
    bool            unsupportedTypeExp = false;

    if( ! mHasMoreRows )
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_62, noMoreRows));

    try
    {       
        bool isNull = false;

        mQueryResult->GetBinaryValue( columnName, sizeof(FdoIGeometry *), (char*)&geom, &isNull, NULL);

        if ( !isNull && geom && geom->GetDerivedType() != FdoGeometryType_None )
            isSupportedType = true;

        if ( !isNull && geom != NULL )
        {
            if ( isSupportedType )
            {
                FdoPtr<FdoFgfGeometryFactory>  gf = FdoFgfGeometryFactory::GetInstance();
                byteArray = gf->GetFgf( geom );
            }
            else
            {
                if ( checkIsNullOnly )
                {
                    byteArray = FdoByteArray::Create( (FdoInt32) 1);
                }
                else
                {
                    unsupportedTypeExp = true;
                    FDO_SAFE_RELEASE( geom );
                    throw FdoCommandException::Create( NlsMsgGet(FDORDBMS_116, "Unsupported geometry type" ) );
                }
            }
        }
        else // isNull indicator is not set by dbi_get_val_b for geometry columns
        {
            throw FdoCommandException::Create(NlsMsgGet1( FDORDBMS_385, "Property '%1$ls' value is NULL; use IsNull method before trying to access the property value",
                                             columnName ));
        }
    }
    catch ( FdoCommandException* exc )
    {
        try
        {
            if ( !unsupportedTypeExp)
                FindColumnIndex( columnName ); /*throws a column not found exception*/
            throw exc;
        }
        catch ( FdoCommandException* exc2 )
        {
            exc->Release();
            throw exc2;
        }
        catch ( FdoException* exc2 )
        {
            exc->Release();
            throw exc2;
        }
    }
    catch ( FdoException* exc )
    {
        try
        {
            if ( !unsupportedTypeExp)
                FindColumnIndex( columnName ); /*throws a column not found exception*/
            throw exc;
        }
        catch ( FdoCommandException* exc2 )
        {
            exc->Release();
            throw exc2;
        }
        catch ( FdoException* exc2 )
        {
            exc->Release();
            throw exc2;
        }
    }
    catch ( ... )
    {
        if ( !unsupportedTypeExp)
        {
            // Try to throw a better exception, otherwise re-throw the dbi exception
            // Note that we only do this when something goes wrong for performance reason; we don't want all that agly
            // string compares to happen every time.
            FindColumnIndex( columnName ); /*throws a column not found exception*/
        }
        throw;
    }

    return byteArray;
}

bool FdoRdbmsSQLDataReader::ReadNext()
{
    if( mQueryResult == NULL )
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_43, "Query ended"));

    mHasMoreRows = false;

    if ( ! mQueryResult->ReadNext() )
    {
        mQueryResult->Close();
        delete mQueryResult;
        mQueryResult = NULL;
        return false;
    }

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

