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
#include "FdoRdbmsUtil.h"
#include "FdoRdbmsException.h"
#include <time.h>
#include "Inc/ut.h"
#include "GdbiCommands.h"

//#include "../../../src/fdo/Rdbms/SchemaMgr/LogicalPhysical/ClassDefinition.h"
//#include "../../../src/fdo/Rdbms/SchemaMgr/LogicalPhysical/GeometricPropertyDefinition.h"
#include "Geometry.h"

#ifdef _WIN32
char *fdordbms_cat = "RdbmsMsg.dll";
#else
char *fdordbms_cat = "fdordbmsmsg.cat";
#endif

//
// Returns the next static char buffer
char* FdoRdbmsUtil::newCharP()
{
    FdoRdbmsUtil::mNextCharBuffIdx++;
    FdoRdbmsUtil::mNextCharBuffIdx = (FdoRdbmsUtil::mNextCharBuffIdx)%MAX_TMP_ARRAY_SIZE;
    FdoRdbmsUtil::mTempCharBuffer[FdoRdbmsUtil::mNextCharBuffIdx][0] = '\0';
    return FdoRdbmsUtil::mTempCharBuffer[FdoRdbmsUtil::mNextCharBuffIdx];
}

//
// Returns the next static wchar_t buffer
wchar_t* FdoRdbmsUtil::newWcharP()
{
    FdoRdbmsUtil::mNextWcharBuffIdx++;
    FdoRdbmsUtil::mNextWcharBuffIdx = (FdoRdbmsUtil::mNextWcharBuffIdx)%MAX_TMP_ARRAY_SIZE;
    FdoRdbmsUtil::mTempWcharBuffer[FdoRdbmsUtil::mNextWcharBuffIdx][0] = '\0';
    return FdoRdbmsUtil::mTempWcharBuffer[FdoRdbmsUtil::mNextWcharBuffIdx];
}

//
// Converts a unicode string to a UTF8 string
const char* FdoRdbmsUtil::UnicodeToUtf8( const wchar_t* value )
{
	if( value == NULL )
		return NULL;

    char    *tmpString = FdoRdbmsUtil::newCharP();
    if( ! ut_utf8_from_unicode( value, tmpString,  GDBI_MAXIMUM_TEXT_SIZE ) )
        throw FdoRdbmsException::Create(NlsMsgGet(FDORDBMS_47, "UTF8 conversion failed"));
    tmpString[GDBI_MAXIMUM_TEXT_SIZE-1] = '\0';

    return tmpString;
}

//
// Converts a UTF8 string to a unicode string
const wchar_t* FdoRdbmsUtil::Utf8ToUnicode( const char* value )
{
	if( value == NULL )
		return NULL;

    wchar_t *tmpString = FdoRdbmsUtil::newWcharP();
    if( ! ut_utf8_to_unicode( value, tmpString,  GDBI_MAXIMUM_TEXT_SIZE ) )
        throw FdoRdbmsException::Create(NlsMsgGet(FDORDBMS_47, "UTF8 conversion failed"));
    tmpString[GDBI_MAXIMUM_TEXT_SIZE-1] = L'\0';

    return tmpString;
}

//
// Convert a DBI data type to an FDO data type
FdoDataType  FdoRdbmsUtil::DbiToFdoType( int type /* Inc/rdbi.h */ )
{
    FdoDataType fdotype;
    switch (type)
    {
        case RDBI_STRING:
            fdotype = FdoDataType_String;
            break;
        case RDBI_CHAR:
            fdotype = FdoDataType_String;
            break;
        case RDBI_BOOLEAN:
            fdotype = FdoDataType_Boolean;
            break;
        case RDBI_SHORT:
            fdotype = FdoDataType_Int16;
            break;
        case RDBI_FIXED_CHAR:
            fdotype = FdoDataType_String;
            break;
        case RDBI_INT:
            fdotype = FdoDataType_Int32;
            break;
        case RDBI_FLOAT:
            fdotype = FdoDataType_Single;
            break;
        case RDBI_BLOB:
            fdotype = FdoDataType_BLOB;
            break;
        case RDBI_RAW:
            fdotype = FdoDataType_BLOB;
            break;
        case RDBI_DOUBLE:
            fdotype = FdoDataType_Double;
            break;
     //   case RDBI_ROWID:
     //       fdotype = "RDBI_ROWID";
     //       break;
        case RDBI_DATE:
            fdotype = FdoDataType_DateTime;
            break;
        case RDBI_LONG:
            fdotype = FdoDataType_Int64;  //TODO: this should most likely map to FdoDataType_Int32!
            break;
        case RDBI_LONGLONG:
            fdotype = FdoDataType_Int64;
            break;
      //  case RDBI_GEOMETRY:
      //      fdotype = "RDBI_GEOMETRY";
      //      break;
        default:
            throw FdoRdbmsException::Create(NlsMsgGet(FDORDBMS_48, "Unknown database data type"));
            break;
    }

    return fdotype;
}

//
// Converts an FDO data type to a DBI data type.
int  FdoRdbmsUtil::FdoToDbiType( FdoDataType type )
{
    int dbitype = -1;
    switch( type )
    {
        case FdoDataType_Boolean:
            dbitype = RDBI_BOOLEAN;
            break;

        case FdoDataType_Byte:
            dbitype = RDBI_CHAR;
            break;

        case FdoDataType_DateTime:
            dbitype = RDBI_DATE;
            break;

        case FdoDataType_Decimal:
            dbitype = RDBI_DOUBLE;
            break;

        case FdoDataType_Double:
            dbitype = RDBI_DOUBLE;
            break;

        case FdoDataType_Int16:
            dbitype = RDBI_SHORT;
            break;

        case FdoDataType_Int32:
            /*dbitype = RDBI_INT;
            break;*/

        case FdoDataType_Int64:
            dbitype = RDBI_LONG;
            break;

        case FdoDataType_Single:
            dbitype = RDBI_FLOAT;
            break;

        case FdoDataType_String:
            dbitype = RDBI_STRING;
            break;

        case FdoDataType_BLOB:
            dbitype = RDBI_BLOB;
            break;

        case FdoDataType_CLOB:
            dbitype = RDBI_RAW;
            break;

        default:
            throw FdoRdbmsException::Create(NlsMsgGet(FDORDBMS_49, "Unknown fdo data type"));
            break;
    }
    return dbitype;
}

//
// A portable string compare utility function
int  FdoRdbmsUtil::StrCmp( const wchar_t * str1, const wchar_t * str2 )
{
#ifdef _WIN32
    return _wcsicmp( str1, str2 );
#else
    return wcscasecmp( str1, str2 );
#endif
}
