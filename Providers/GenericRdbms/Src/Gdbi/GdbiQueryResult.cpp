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
#include "GdbiQueryResult.h"
#include "GdbiCommands.h"
#ifdef USE_NONRDBMS_HEADER
#include <inc/ut.h>
#else
#include <Inc/ut.h>
#endif
#include "FdoCommonOSUtil.h"

// Arbitrary size which should be more than enough for the 
// the ASCII or UNICODE representation of any numeric value.
#define GDBI_MIN_STRING_BUFFER_SIZE 50

static const wchar_t* StripTable( const wchar_t *ColName )
{
    // FIXME; use the column name only for now untill rdbi supports fully qualified names.
    const wchar_t* newName = ColName;
    while( *(newName) != '.' && *(newName) != '\0' ) newName++;
    if( *(newName) == '\0' )
        newName = ColName;
    else
        newName++; // get passed the "."

    return newName;
}

GdbiQueryResult::GdbiQueryResult(GdbiCommands* command, int qid, bool ownsQid ):
m_pGdbiCommands( command ),
mQueryId( qid ),
mUnicodeBuffer( NULL ),
mUnicodeBufferSize( 0 ),
mAsciiValBuffer( NULL),
mAsciiValBufferSize(0),
mArrayPos( 0 ),
mArrayCCount( 0 ),
mArrayTCount( 0 ),
mOwnsQueryId( ownsQid )
{
    GdbiColumnInfoType*      colInfo = new GdbiColumnInfoType;
    char        buffer[16];
    int name_length = GDBI_COLUMN_SIZE;
    wchar_t colName[GDBI_COLUMN_SIZE];
    mColMap = new std::map<std::wstring,GdbiColumnInfoType*>();
    int  idx = 1;

    while( m_pGdbiCommands->desc_slct( mQueryId, idx++, name_length, colName, &colInfo->type, &colInfo->size, &colInfo->null_allowed) == RDBI_SUCCESS )
    {

        FdoStringP  upperName = FdoStringP(colName).Upper();
        const wchar_t* name = (const wchar_t*)upperName;
        if (mColMap->find (name) != mColMap->end())
            continue;
        mColMap->insert( std::pair<std::wstring,GdbiColumnInfoType*> ( name, colInfo ) );
        colInfo->original_type = colInfo->type;
        colInfo->index = idx-1;

		if (!m_pGdbiCommands->alcnullind(m_pGdbiCommands->get_array_size(), &(colInfo->isNull)))
			break;

		if (colInfo->size == 0)
		{
			colInfo->value = NULL;
			m_pGdbiCommands->set_null(colInfo->isNull, 0, m_pGdbiCommands->get_array_size()-1);
		}
		else
		{
			m_pGdbiCommands->set_nnull(colInfo->isNull, 0, m_pGdbiCommands->get_array_size()-1);

			if (colInfo->type == RDBI_BLOB_REF)
			{
				for (int i = 0; i < m_pGdbiCommands->get_array_size(); i++)
				{
					if (m_pGdbiCommands->lob_create_ref( mQueryId, (void **)&(colInfo->value)) != RDBI_SUCCESS)
						break;
				}
			}
			else 
			{
				// In SQLServer the size of RDBI_CHAR is 2 which really is RDBI_STRING
				if (colInfo->type == RDBI_CHAR && colInfo->size == 2)
					colInfo->type = RDBI_STRING;
				else
				{
					if (colInfo->type == RDBI_FIXED_CHAR || colInfo->type == RDBI_STRING)
						// allocate space for null indicator
						colInfo->size++;
				}

				if( ( m_pGdbiCommands->SupportsUnicode() && colInfo->type == RDBI_STRING ) || colInfo->type == RDBI_WSTRING  )
				{
					int size =  colInfo->size * m_pGdbiCommands->get_array_size();
					colInfo->value = new wchar_t[size];
					colInfo->size = colInfo->size*sizeof(wchar_t);
					colInfo->type = RDBI_WSTRING;
					memset( colInfo->value, '\0', size*sizeof(wchar_t) );
				}
				else
				{
					int size = colInfo->size * m_pGdbiCommands->get_array_size();
					colInfo->value = new char[size];
					memset( colInfo->value, '\0', size );
				}
			}

			if( m_pGdbiCommands->define( mQueryId, FdoCommonOSUtil::itoa(colInfo->index, buffer), colInfo->type, colInfo->size, (colInfo->type == RDBI_BLOB_REF) ? (char *)&(colInfo->value) : (char*)colInfo->value, colInfo->isNull ) != RDBI_SUCCESS )
					break;
		
		}
        
        colInfo = new GdbiColumnInfoType;
    }
    delete colInfo;

    if( m_pGdbiCommands->execute( mQueryId, 0, 0 ) == RDBI_SUCCESS ) // Should be done outside the constructor
        return;
}

GdbiQueryResult::~GdbiQueryResult(void)
{
    if( mQueryId != -1 )
    {
        m_pGdbiCommands->end_select( mQueryId );
    }

	if( mOwnsQueryId && mQueryId != -1 )
    {
        m_pGdbiCommands->free_cursor( mQueryId );
    }

    if( mColMap )
    {
        for (std::map <std::wstring,GdbiColumnInfoType*>::iterator i = mColMap->begin(); i != mColMap->end(); ++i )
        {
            GdbiColumnInfoType *colInfo = i->second;
			if( colInfo->value && colInfo->type == RDBI_GEOMETRY )
			{
				delete[] (char *) colInfo->value;
			}
            else if( colInfo->value && colInfo->type != RDBI_GEOMETRY )
			{
				if (colInfo->size > 0)
					delete [] ((wchar_t*)(colInfo->value));
			}

			if( colInfo->isNull )
				free( (void*)colInfo->isNull );
            delete colInfo;
        }
        delete mColMap;
    }
    if( mUnicodeBuffer != NULL )
        delete[] mUnicodeBuffer;

    if(mAsciiValBuffer != NULL )
        delete [] mAsciiValBuffer;
}

int GdbiQueryResult::ReadNext()
{
    int rows;
	int rc = RDBI_SUCCESS;

	if (mArrayPos < mArrayCCount)
		mArrayPos++;

	if (mArrayPos == mArrayCCount)
	{
		rc = m_pGdbiCommands->fetch( mQueryId, m_pGdbiCommands->get_array_size(), &rows );
		if (rc != RDBI_GENERIC_ERROR)
		{
			// adjust counters
			mArrayPos = 0;
			mArrayCCount = rows - mArrayTCount;
			mArrayTCount = rows;
		}
	}
	
	return (rc == RDBI_SUCCESS) ;
}

void GdbiQueryResult::Close()
{
   if( mQueryId != -1 )
		m_pGdbiCommands->end_select( mQueryId );

    if( mOwnsQueryId && mQueryId != -1 )
    {
        m_pGdbiCommands->free_cursor( mQueryId );
    }

    mQueryId = -1;
	mArrayPos = 0;
	mArrayCCount = 0;
	mArrayTCount = 0;
}

void GdbiQueryResult::End()
{
	if (mQueryId != -1)
	{
		m_pGdbiCommands->end_select( mQueryId );
	}
}

int GdbiQueryResult::GetColumnCount( )
{
    if( mColMap )
        return (int)mColMap->size();

    return 0;
}

int GdbiQueryResult::GetColumnDesc( int colIdx, GdbiColumnDesc &desc )
{
    int idx = colIdx;
    std::map <std::wstring,GdbiColumnInfoType*>::iterator iter = mColMap->begin();
    for( iter = mColMap->begin(); iter != mColMap->end(); iter++ )
        if( idx == iter->second->index )
            break;

    if( iter == mColMap->end() )  // error
        return false;

    GdbiColumnInfoType *colInfo = iter->second;
    strncpy( desc.column, (const char*)FdoStringP(iter->first.c_str(),true), sizeof(desc.column) - 1);
    desc.column[sizeof(desc.column)-1]=0;
    desc.datatype = colInfo->original_type;
	desc.size = (colInfo->type == RDBI_CHAR || colInfo->type == RDBI_FIXED_CHAR || colInfo->type == RDBI_STRING) ? colInfo->size-1 : colInfo->size;
    desc.null_ok = colInfo->null_allowed;

    return true;
}

int GdbiQueryResult::GetBinaryValue( const wchar_t *colName, int length, char *address, bool *null_ind, int *ccode )
{
    std::map <std::wstring,GdbiColumnInfoType*>::iterator iter;
	bool isNull;
    FdoStringP  upperName = FdoStringP(colName).Upper();
    const wchar_t* name = (const wchar_t*)upperName;
    iter = mColMap->find( StripTable(name) );
    if( iter != mColMap->end() )
    {
        GdbiColumnInfoType *colInfo = iter->second;
		isNull = (m_pGdbiCommands->is_null(colInfo->isNull, mArrayPos) == 1);
		if (isNull == false)
		{
			if (colInfo->type == RDBI_CHAR || colInfo->type == RDBI_BOOLEAN)
			{
				memcpy(address, (char*)(colInfo->value) + mArrayPos*colInfo->size, 1);
				if (length != 1)
					address[1] = '\0';
			}
			else
			{
				int  size = ( length > colInfo->size )? colInfo->size : length;
				memcpy(address, ((char*)colInfo->value) + mArrayPos*colInfo->size, size );
			}
		}
        if( null_ind )
            *null_ind = isNull;

        if( ccode )
            *ccode = RDBI_SUCCESS;

        return RDBI_SUCCESS;
    }

    throw new GdbiException(L"Column X not selected");
}

int GdbiQueryResult::GetAsciiValue( const wchar_t *colName, int length, char *address, bool *null_ind, int *ccode )
{
    std::map <std::wstring,GdbiColumnInfoType*>::iterator iter;
	char	lascii[200];
	int		lccode = RDBI_SUCCESS;
    FdoStringP  upperName = FdoStringP(colName).Upper();
    const wchar_t* name = (const wchar_t*)upperName;
    iter = mColMap->find( StripTable(name) );
	bool isNull;
    if( iter != mColMap->end() )
    {
        GdbiColumnInfoType *colInfo = iter->second;
		isNull = (m_pGdbiCommands->is_null(colInfo->isNull, mArrayPos) == 1);
		if (isNull == false)
		{
			int  size = ( length > colInfo->size )? colInfo->size : length;

			switch (colInfo->type)
			{
			case RDBI_SHORT:
				sprintf(lascii, "%d", (*(short *)((char*)colInfo->value + mArrayPos*colInfo->size)));
				do_copy(lascii, address, length, &lccode); 
				break;
			case RDBI_INT:
			case RDBI_LONG:
				sprintf(lascii, "%ld", (*(long *)((char*)colInfo->value + mArrayPos*colInfo->size)));
				do_copy(lascii, address, length, &lccode); 
				break;
			case RDBI_LONGLONG:
#ifdef _WIN32
				sprintf(lascii, "%I64d", (*(FdoInt64 *)((char*)colInfo->value + mArrayPos*colInfo->size)));
#else
				sprintf(lascii, "%lld", (*(FdoInt64 *)((char*)colInfo->value + mArrayPos*colInfo->size)));
#endif
				do_copy(lascii, address, length, &lccode); 
				break;
			case RDBI_FLOAT:
				sprintf(lascii, "%s", ut_dtoa( (double) (*(float *)((char*)colInfo->value + mArrayPos*colInfo->size))) );
				do_copy(lascii, address, length, &lccode);
				break;
			case RDBI_DOUBLE:
				sprintf(lascii, "%s", ut_dtoa( (*(double *)((char*)colInfo->value + mArrayPos*colInfo->size))) );
				do_copy(lascii, address, length, &lccode);
				break;
			default:
				if (colInfo->type == RDBI_CHAR)
				{
					// Oracle and MySQL do not put null indicators
					memcpy(address, (char*)(colInfo->value) + mArrayPos*colInfo->size, colInfo->size);
					if (length != 1)
						address[1] = '\0';
				}
				else
					memcpy(address, (char*)(colInfo->value) + mArrayPos*colInfo->size, size); 
				
                if (size < colInfo->size)
                {
                    address[size-1] = '\0';
                    lccode = RDBI_DATA_TRUNCATED;
                }
				break;
			}
		}
        if( null_ind )
            *null_ind = isNull;

        if( ccode )
            *ccode = lccode;

        return RDBI_SUCCESS;
    }

    throw new GdbiException(L"Column X not selected");
}

template<typename T> T GdbiQueryResult::GetNumber(
        const wchar_t *colName,
        bool *null_ind,
        int *ccode
        )
{
	T val = 0;
	short  shortVal;
	int    intVal;
	long   longVal;
	float  floatVal;
	double doubleVal;
	FdoInt64  int64Val;

	std::map <std::wstring,GdbiColumnInfoType*>::iterator iter;
    FdoStringP  upperName = FdoStringP(colName).Upper();
    const wchar_t* name = (const wchar_t*)upperName;
    iter = mColMap->find( StripTable(name) );
	bool isNull;
    if( iter != mColMap->end() )
    {
        GdbiColumnInfoType *colInfo = iter->second;
		isNull = (m_pGdbiCommands->is_null(colInfo->isNull, mArrayPos) == 1);
		
		if( null_ind )
            *null_ind = isNull;

 		if( ccode )
			*ccode = RDBI_SUCCESS;

		if (isNull == false)
		{
			switch (colInfo->type)
			{
				case RDBI_SHORT:
					memcpy(&shortVal, ((char*)colInfo->value) + mArrayPos*colInfo->size, sizeof(short) );
					val = (T)shortVal;
					break;

				case RDBI_INT:
					memcpy(&intVal, ((char*)colInfo->value) + mArrayPos*colInfo->size, sizeof(int) );
					val = (T)intVal;
					break;

				case RDBI_LONG:
					memcpy(&longVal, ((char*)colInfo->value) + mArrayPos*colInfo->size, sizeof(long) );
					val = (T)longVal;
					break;

				case RDBI_LONGLONG:
					memcpy(&int64Val, ((char*)colInfo->value) + mArrayPos*colInfo->size, sizeof(FdoInt64) );
					val = (T)int64Val;
					break;

				case RDBI_FLOAT:
					memcpy(&floatVal, ((char*)colInfo->value) + mArrayPos*colInfo->size, sizeof(float) );
					val = (T)floatVal;
					break;

				case RDBI_DOUBLE:
					memcpy(&doubleVal, ((char*)colInfo->value) + mArrayPos*colInfo->size, sizeof(double) );
					val = (T)doubleVal;
					break;

				case RDBI_STRING:
					if( sizeof(T) == sizeof(FdoInt64) )
					{
						// Sql server workaround
						sscanf((char*)colInfo->value+ mArrayPos*colInfo->size,"%lld", &int64Val);
						val = (T)int64Val;
						break;
					}

				case RDBI_WSTRING:
					if( sizeof(T) == sizeof(FdoInt64) )
					{
						// Sql server workaround
						swscanf((wchar_t*)colInfo->value+ mArrayPos*colInfo->size/sizeof(wchar_t),L"%lld", &int64Val);
						val = (T)int64Val;
						break;
					}
				default:
					(void)GetBinaryValue(colName, sizeof(T), (char*)&val, null_ind, ccode );
					break;

			}
		}

        return val;
    }
    throw new GdbiException(L"Column X not selected");
}

FdoDouble GdbiQueryResult::GetDouble( const wchar_t *ColName, bool *isnull, int *ccode )
{
    return GetNumber<double>(ColName, isnull, ccode);
}

FdoInt32 GdbiQueryResult::GetInt32( const wchar_t *ColName, bool *isnull, int *ccode )
{
	return GetNumber<int>(ColName, isnull, ccode);
}

FdoInt16 GdbiQueryResult::GetInt16( const wchar_t *ColName, bool *isnull, int *ccode )
{
    return GetNumber<short>(ColName, isnull, ccode);
}

FdoInt8 GdbiQueryResult::GetInt8( const wchar_t *ColName, bool *isnull, int *ccode )
{
    return GetNumber<FdoInt8>(ColName, isnull, ccode);
}

FdoInt64 GdbiQueryResult::GetInt64( const wchar_t *ColName, bool *isnull, int *ccode )
{
    return GetNumber<FdoInt64>(ColName, isnull, ccode);
}

FdoFloat GdbiQueryResult::GetFloat( const wchar_t *ColName, bool *isnull, int *ccode )
{
    return GetNumber<float>(ColName, isnull, ccode);
}


// Return a const wchar_t that must be copied ASP
FdoString* GdbiQueryResult::GetString( const wchar_t *colName, bool *isnull, int *ccode )
{
    std::map <std::wstring,GdbiColumnInfoType*>::iterator iter;

    FdoStringP  upperName = FdoStringP(colName).Upper();
    const wchar_t* name = (const wchar_t*)upperName;
    iter = mColMap->find( StripTable(name) );
    if( iter != mColMap->end() )
    {
        GdbiColumnInfoType *colInfo = iter->second;

		if( isnull )
			*isnull = (m_pGdbiCommands->is_null(colInfo->isNull, mArrayPos) == 1);

        bool isNull = (m_pGdbiCommands->is_null(colInfo->isNull, mArrayPos) == 1);
        if (isNull == false)
        {
			if( ( m_pGdbiCommands->SupportsUnicode() && colInfo->original_type == RDBI_STRING ) || 
				colInfo->original_type == RDBI_WSTRING  )
			{	
				if( ccode )
					*ccode = RDBI_SUCCESS;
				return (const wchar_t*)(((char*)colInfo->value) + mArrayPos*colInfo->size);
			}

            if ((mAsciiValBuffer != NULL) && (mAsciiValBufferSize <= colInfo->size))
            {
                delete [] mAsciiValBuffer;
                mAsciiValBuffer = NULL;
            }
            if (mAsciiValBuffer == NULL)
            {
                mAsciiValBufferSize = (colInfo->size < GDBI_MIN_STRING_BUFFER_SIZE) ? GDBI_MIN_STRING_BUFFER_SIZE : colInfo->size;
                mAsciiValBuffer = new char[mAsciiValBufferSize];
            }
        
            if (GetAsciiValue( name, mAsciiValBufferSize, mAsciiValBuffer, NULL, NULL) != RDBI_SUCCESS)
                return NULL;
    
            if ((mUnicodeBuffer != NULL) && (mUnicodeBufferSize < mAsciiValBufferSize)) 
            {
                delete[] mUnicodeBuffer;
                mUnicodeBuffer = NULL;
            }
            if( mUnicodeBuffer == NULL )
            {
                mUnicodeBufferSize = mAsciiValBufferSize;
                mUnicodeBuffer = new wchar_t[mUnicodeBufferSize];
            }

			if( mAsciiValBuffer[0] == '\0' )
				mUnicodeBuffer[0] = '\0';
            else if( ! ut_utf8_to_unicode( mAsciiValBuffer, mUnicodeBuffer,  mUnicodeBufferSize ) )
                throw GdbiException::Create(NlsMsgGet(FDORDBMS_47, "UTF8 conversion failed"));
        }

        if( ccode )
            *ccode = RDBI_SUCCESS;

        return mUnicodeBuffer;
    }

    throw new GdbiException(L"Column X not selected");
}

FdoString* GdbiQueryResult::GetString( int index, bool *isnull, int *ccode )
{
   if( mColMap )
    {
        for (std::map <std::wstring,GdbiColumnInfoType*>::iterator i = mColMap->begin(); i != mColMap->end(); ++i )
        {
            GdbiColumnInfoType *colInfo = i->second;
            if( colInfo->index == index )
			{
				return GetString( i->first.c_str() , isnull, ccode );
			}
        }
    }
   throw new GdbiException(L"Column X not selected");
}

FdoBoolean GdbiQueryResult::GetBoolean( const wchar_t *ColName, bool *isnull, int *ccode )
{
    char   boolVal;
    if( GetBinaryValue(ColName, sizeof(char), &boolVal, isnull, ccode ) == RDBI_SUCCESS )
        return (boolVal==1 || boolVal == '1');

    throw new GdbiException(L"Internal query error"); // not reached as GetBinaryValue would throw an exception
}

bool GdbiQueryResult::GetIsNull( const wchar_t *colName )
{
    std::map <std::wstring,GdbiColumnInfoType*>::iterator iter;

    FdoStringP  upperName = FdoStringP(colName).Upper();
    const wchar_t* name = (const wchar_t*)upperName;
    iter = mColMap->find( StripTable(name) );
    if( iter != mColMap->end() )
    {
        GdbiColumnInfoType *colInfo = iter->second;

        return ( m_pGdbiCommands->is_null( colInfo->isNull, mArrayPos ) == 1 );
    }
    throw new GdbiException(L"Column X not selected");
}
void GdbiQueryResult::do_copy(char *ascii_I, char *ascii_O, int len, int *ccode)
{
	int	llen = (int)strlen(ascii_I);
	*ccode = RDBI_SUCCESS;

	if (llen+1 > len)
	{
		strncpy(ascii_O, ascii_I, len-1);
		ascii_I[len-1] = '\0';
		*ccode = RDBI_DATA_TRUNCATED;
	}
	else
		strcpy(ascii_O, ascii_I);
}

FdoDouble GdbiQueryResult::GetDouble( const char *ColName, bool *isnull, int *ccode )
{
	return GetDouble( (const wchar_t*) FdoStringP( ColName ), isnull, ccode );
}

FdoInt32 GdbiQueryResult::GetInt32( const char *ColName, bool *isnull, int *ccode )
{
	return GetInt32( (const wchar_t*) FdoStringP( ColName ), isnull, ccode );
}

FdoInt16 GdbiQueryResult::GetInt16( const char *ColName, bool *isnull, int *ccode )
{
	return GetInt16( (const wchar_t*) FdoStringP( ColName ), isnull, ccode );
}

FdoInt8 GdbiQueryResult::GetInt8( const char *ColName, bool *isnull, int *ccode )
{
	return GetInt8( (const wchar_t*) FdoStringP( ColName ), isnull, ccode );
}

FdoInt64 GdbiQueryResult::GetInt64( const char *ColName, bool *isnull, int *ccode )
{
	return GetInt64( (const wchar_t*) FdoStringP( ColName ), isnull, ccode );
}

FdoFloat GdbiQueryResult::GetFloat( const char *ColName, bool *isnull, int *ccode )
{
	return GetFloat( (const wchar_t*) FdoStringP( ColName ), isnull, ccode );
}

FdoString* GdbiQueryResult::GetString( const char *ColName, bool *isnull, int *ccode )
{
	return GetString( (const wchar_t*) FdoStringP( ColName ), isnull, ccode );
}

FdoBoolean GdbiQueryResult::GetBoolean( const char *ColName, bool *isnull, int *ccode )
{
	return GetBoolean( (const wchar_t*) FdoStringP( ColName ), isnull, ccode );
}

bool GdbiQueryResult::GetIsNull( const char *ColName )
{
	return GetIsNull( (const wchar_t*) FdoStringP( ColName ) );
}
