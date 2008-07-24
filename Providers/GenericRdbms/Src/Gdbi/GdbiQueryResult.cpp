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
#include "GdbiQueryIdentifier.h"
#ifdef HAVE_GEOM_INFO_TYPE
#include <inc/geometry/fgf/AllGeometry_c.h>
#endif

// Arbitrary size which should be more than enough for the 
// the ASCII or UNICODE representation of any numeric value.
#define GDBI_MIN_STRING_BUFFER_SIZE 50

static const wchar_t* StripTable( const wchar_t *ColName )
{
    // FIXME; use the column name only for now untill rdbi supports fully qualified names.
    const wchar_t* newName = ColName;
    const wchar_t* dotPos = NULL;
    const wchar_t* currPos = ColName;

    // Column name is the part after the last dot. 
    // On SqlServer, there can be multiple dots.
    while ( *(currPos) != '\0' ) 
    {
        if ( *(currPos) == '.' ) 
            dotPos = currPos;

        currPos++;
    }
    if( dotPos ) {
        newName = dotPos;
        newName++;
    }

    return newName;
}

GdbiQueryResult::GdbiQueryResult(GdbiCommands* command, int qid, bool ownsQid ):
m_pGdbiCommands( command ),
mUnicodeBuffer( NULL ),
mUnicodeBufferSize( 0 ),
mAsciiValBuffer( NULL),
mAsciiValBufferSize(0),
mArrayPos( 0 ),
mArrayCCount( 0 ),
mArrayTCount( 0 ),
mHasLobs(false)
{
	m_QueryId = new GdbiQueryIdentifier(command, qid);
	define_exec();
	m_missed=0;
}
GdbiQueryResult::GdbiQueryResult(GdbiCommands* command, GdbiQueryIdentifier *queryObj):
m_pGdbiCommands( command ),
mUnicodeBuffer( NULL ),
mUnicodeBufferSize( 0 ),
mAsciiValBuffer( NULL),
mAsciiValBufferSize(0),
mArrayPos( 0 ),
mArrayCCount( 0 ),
mArrayTCount( 0 ),
mHasLobs(false)
{
	m_QueryId = FDO_SAFE_ADDREF(queryObj);
	define_exec();
	m_missed=0;
}


void GdbiQueryResult::define_exec()
{
    char        buffer[16];
    int			name_length = GDBI_COLUMN_SIZE;
    wchar_t		colName[GDBI_COLUMN_SIZE];
    int			colType;
    int			colSize;
	int			colNullAllowed;


	mColList = new std::vector<GdbiColumnInfoType*>();

    int  idx = 1;
	bool status = false;

    while( m_pGdbiCommands->desc_slct( m_QueryId->GetQueryId(), idx++, name_length, colName, &colType, &colSize, &colNullAllowed) == RDBI_SUCCESS )
    {

        FdoStringP  upperName = FdoStringP(colName).Upper();
        const wchar_t* name = (const wchar_t*)upperName;

		GdbiColumnInfoType*      colInfo = new GdbiColumnInfoType;
		
        mColList->push_back( colInfo );
		
		// Copy info
		colInfo->name = new wchar_t[wcslen(name) + 1];
		wcscpy(colInfo->name, name );

		colInfo->type = colType;
		colInfo->size = colSize;
		colInfo->null_allowed = colNullAllowed;

        colInfo->original_type = colInfo->type;
        colInfo->index = idx - 1;

		m_pGdbiCommands->alcnullind(m_pGdbiCommands->get_array_size(), &(colInfo->isNull));

		// In SQLServer the size of RDBI_CHAR is 2 which really is RDBI_STRING
		if (colInfo->type == RDBI_CHAR && colInfo->size == 2)
			colInfo->type = RDBI_STRING;
		else
		{
			if (colInfo->type == RDBI_FIXED_CHAR || colInfo->type == RDBI_STRING || colInfo->type == RDBI_WSTRING)
				// allocate space for null indicator
				colInfo->size++;
		}

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
                colInfo->value = NULL;
    			m_pGdbiCommands->lob_create_ref( m_QueryId->GetQueryId(), (void **)&(colInfo->value));
                mHasLobs = true;
			}
			else 
			{
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

			m_pGdbiCommands->define( m_QueryId->GetQueryId(), FdoCommonOSUtil::itoa(colInfo->index, buffer), colInfo->type, colInfo->size, (colInfo->type == RDBI_BLOB_REF) ? (char *)&(colInfo->value) : (char*)colInfo->value, colInfo->isNull );
		
		}   
    }

    m_pGdbiCommands->execute( m_QueryId->GetQueryId(), 0, 0 );
}

GdbiQueryResult::~GdbiQueryResult(void)
{
    if( m_QueryId )
    {
        m_pGdbiCommands->end_select( m_QueryId->GetQueryId());
    }

    if( mColList )
    {
        for (size_t i = 0; i < mColList->size(); i++ )
        {
            GdbiColumnInfoType *colInfo = mColList->at(i);

			if( colInfo->value && colInfo->type == RDBI_BLOB_REF )
			{
				if (m_QueryId)
					m_pGdbiCommands->lob_destroy_ref(m_QueryId->GetQueryId(), colInfo->value);
			}
			else if( colInfo->value && colInfo->type == RDBI_GEOMETRY )
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

			if ( colInfo->name )
				delete [] colInfo->name;

            delete colInfo;
        }
        delete mColList;
    }
	FDO_SAFE_RELEASE(m_QueryId);

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
        rc = m_pGdbiCommands->fetch( m_QueryId->GetQueryId(), mHasLobs ? 1 : m_pGdbiCommands->get_array_size(), &rows );
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
	if( m_QueryId )
	{
		m_pGdbiCommands->end_select( m_QueryId->GetQueryId() );
		FDO_SAFE_RELEASE(m_QueryId);
	}

	mArrayPos = 0;
	mArrayCCount = 0;
	mArrayTCount = 0;
}

void GdbiQueryResult::End()
{
	if (m_QueryId)
	{
		m_pGdbiCommands->end_select( m_QueryId->GetQueryId() );
	}
}

int GdbiQueryResult::GetColumnCount( )
{
    if( mColList )
        return (int)mColList->size();

    return 0;
}

int GdbiQueryResult::GetColumnDesc( int colIdx, GdbiColumnDesc &desc )
{
    int idx = colIdx; // 1 based

	if ( colIdx > (int)mColList->size() ) // error
		return false; 

	GdbiColumnInfoType *colInfo = mColList->at(idx-1);

	strncpy( desc.column, (const char*)FdoStringP(colInfo->name), sizeof(desc.column) - 1);
    desc.column[sizeof(desc.column)-1]=0;
    desc.datatype = colInfo->original_type;
	desc.size = (colInfo->type == RDBI_CHAR || colInfo->type == RDBI_FIXED_CHAR || colInfo->type == RDBI_STRING) ? colInfo->size-1 : colInfo->size;
    desc.null_ok = colInfo->null_allowed;

    return true;
}

int GdbiQueryResult::GetBinaryValue( const wchar_t *colName, int length, char *address, bool *null_ind, int *ccode )
{
	GdbiColumnInfoType *colInfo = FindColumnCache(colName);

	bool isNull = (m_pGdbiCommands->is_null(colInfo->isNull, mArrayPos) == 1);

	if (isNull == false)
	{
		if (colInfo->type == RDBI_BLOB_REF)
        {
			memcpy(address, (char*)&(colInfo->value), sizeof(char*));
        }
		else if (colInfo->type == RDBI_CHAR || colInfo->type == RDBI_BOOLEAN)
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

int GdbiQueryResult::GetAsciiValue( GdbiColumnInfoType *colInfo, int length, char *address, bool *null_ind, int *ccode )
{
    char    tmpDblBuffer[60];
	char	lascii[200];
	int		lccode = RDBI_SUCCESS;

	bool isNull = (m_pGdbiCommands->is_null(colInfo->isNull, mArrayPos) == 1);
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
			sprintf(lascii, "%s", ut_dtoa( (double) (*(float *)((char*)colInfo->value + mArrayPos*colInfo->size)), tmpDblBuffer) );
			do_copy(lascii, address, length, &lccode);
			break;
		case RDBI_DOUBLE:
			sprintf(lascii, "%s", ut_dtoa( (*(double *)((char*)colInfo->value + mArrayPos*colInfo->size)), tmpDblBuffer ) );
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
            {
				memcpy(address, (char*)(colInfo->value) + mArrayPos*colInfo->size, size);
                if (size < length) // ensure we don't get garbage at the end...
                    address[size] = '\0';
            }
			
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

	GdbiColumnInfoType *colInfo = FindColumnCache(colName);

	bool isNull = (m_pGdbiCommands->is_null(colInfo->isNull, mArrayPos) == 1);
	
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
				(void)GetBinaryValue(colInfo->name, sizeof(T), (char*)&val, null_ind, NULL );
				break;

		}
	}

    return val;
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
	GdbiColumnInfoType *colInfo = FindColumnCache(ColName);

    if ( colInfo->type == RDBI_DOUBLE ) 
    {
        bool isnull2;
        double dblVal = GetNumber<FdoDouble>(ColName, &isnull2, ccode);

        if ( isnull )
            (*isnull) = isnull2;

        if ( isnull2 )
            return (FdoInt64) dblVal;

        if ( dblVal >= (FdoDouble) LLONG_MAX ) 
            return LLONG_MAX;

        if ( dblVal <= (FdoDouble) LLONG_MIN ) 
            return LLONG_MIN;

        return (FdoInt64) dblVal;
    }

    return GetNumber<FdoInt64>(ColName, isnull, ccode);
}

FdoFloat GdbiQueryResult::GetFloat( const wchar_t *ColName, bool *isnull, int *ccode)
{
    return GetNumber<float>(ColName, isnull, ccode);
}

// Return a const wchar_t that must be copied ASP
FdoString* GdbiQueryResult::GetString( const wchar_t *colName, bool *isnull, int *ccode)
{
	GdbiColumnInfoType *colInfo = FindColumnCache(colName);

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
    
        if (GetAsciiValue( colInfo, mAsciiValBufferSize, mAsciiValBuffer, NULL, NULL ) != RDBI_SUCCESS)
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

FdoString* GdbiQueryResult::GetString(int index, bool *isnull, int *ccode)
{
	FdoStringP	colName = FdoStringP::Format(L"%ld", index );

	return GetString( (const wchar_t *)colName, isnull, ccode);
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
	GdbiColumnInfoType *colInfo = FindColumnCache(colName );

    return ( m_pGdbiCommands->is_null( colInfo->isNull, mArrayPos ) == 1 );
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

FdoBoolean GdbiQueryResult::LobGetSize(void *lob_ref, unsigned int *size)
{
    return (m_pGdbiCommands->lob_get_size( m_QueryId->GetQueryId(), lob_ref, size ) != 0 );
}

FdoBoolean GdbiQueryResult::LobReadNext(void *lob_ref, int rdbi_lob_type, unsigned int block_size, char *block, unsigned int *block_size_out, int *eol)
{
    return (m_pGdbiCommands->lob_read_next( m_QueryId->GetQueryId(), lob_ref, rdbi_lob_type, block_size, block, block_size_out, eol ) != 0 );
}

bool GdbiQueryResult::GetIsNull( const char *ColName )
{
	return GetIsNull( (const wchar_t*) FdoStringP( ColName ) );
}

FdoByteArray * GdbiQueryResult::GetFgfFromGeomInfo( char * geomInfo, int defaultDim )
{
#ifdef HAVE_GEOM_INFO_TYPE
    FdoByteArray * byteArray = NULL;
    (void) m_pGdbiCommands->geom_to_fgf( m_QueryId->GetQueryId(), geomInfo, defaultDim, (void **)(&byteArray) );
    return byteArray;
#else
    return NULL;
#endif
}

int	GdbiQueryResult::GetColumnIndex( const wchar_t *colName )
{
	GdbiColumnInfoType *colInfoCached = FindColumnCache( colName );

	return colInfoCached->index; // 1 - based
}

GdbiColumnInfoType *GdbiQueryResult::FindColumnCache( const wchar_t *colName )
{
	GdbiColumnInfoType *colInfo = NULL;
	bool				found = false;

	int		colIndex = FdoCommonOSUtil::wtoi( colName );

	// Number ?
	if ( colIndex > 0 )
	{
		if( mColList && colIndex <= (int)mColList->size())
		{
			colInfo = mColList->at( colIndex - 1);
			found = true;
		}
	}
	else
	{
		FdoStringP  upperName = FdoStringP(colName).Upper();
		const wchar_t* name = StripTable(upperName);

        int     index_empty = -1;
		for ( int i = 0; !found && i < (int)mColList->size(); i++ )
		{
			colInfo = mColList->at(i);
			found = ( wcscmp( colInfo->name, name ) == 0 );

            // bug in SQL Server 2008 - returns "" for geometry column wrapped in a conversion function.
            if ( wcscmp( colInfo->name, L"") == 0 )
                index_empty = i;
		}
        if ( !found && (index_empty != -1) )
        {
            colInfo = mColList->at(index_empty);
            found = true;
        }
	}

	if ( !found )
		throw FdoCommandException::Create(NlsMsgGet1(FDORDBMS_42, "Column %1$ls not found", colName));

	return colInfo;
}
