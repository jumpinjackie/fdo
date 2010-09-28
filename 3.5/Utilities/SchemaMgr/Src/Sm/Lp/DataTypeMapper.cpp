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
#include <Sm/Lp/DataTypeMapper.h>
#include <Sm/Error.h>

static FdoSmLpDataTypeMapEntry boolEntry( FdoDataType_Boolean, L"boolean" );
static FdoSmLpDataTypeMapEntry byteEntry( FdoDataType_Byte, L"byte" );
static FdoSmLpDataTypeMapEntry dateEntry( FdoDataType_DateTime, L"datetime" );
static FdoSmLpDataTypeMapEntry decimalEntry( FdoDataType_Decimal, L"decimal" );
static FdoSmLpDataTypeMapEntry doubleEntry( FdoDataType_Double, L"double" );
static FdoSmLpDataTypeMapEntry int16Entry( FdoDataType_Int16, L"int16" );
static FdoSmLpDataTypeMapEntry int32Entry( FdoDataType_Int32, L"int32" );
static FdoSmLpDataTypeMapEntry int64Entry( FdoDataType_Int64, L"int64" );
static FdoSmLpDataTypeMapEntry singleEntry( FdoDataType_Single, L"single" );
static FdoSmLpDataTypeMapEntry stringEntry( FdoDataType_String, L"string" );
static FdoSmLpDataTypeMapEntry blobEntry( FdoDataType_BLOB, L"blob" );
static FdoSmLpDataTypeMapEntry clobEntry( FdoDataType_CLOB, L"clob" );

FdoSmLpDataTypeMapEntry* FdoSmLpDataTypeMapper::mMap[] = {
	&boolEntry,
	&byteEntry,
	&dateEntry,
	&decimalEntry,
	&doubleEntry,
	&int16Entry,
	&int32Entry,
	&int64Entry,
	&singleEntry,
	&stringEntry,
	&blobEntry,
	&clobEntry,
    NULL // This array must be null terminated.
};


FdoSmLpDataTypeMapEntry::FdoSmLpDataTypeMapEntry(FdoDataType dataType, const wchar_t* dataTypeString)
{
	mDataType = dataType;
	mDataTypeString = FdoStringP(dataTypeString, false);
}

FdoSmLpDataTypeMapEntry::~FdoSmLpDataTypeMapEntry(void)
{
}

FdoSmLpDataTypeMapper::FdoSmLpDataTypeMapper(void)
{
}

FdoSmLpDataTypeMapper::~FdoSmLpDataTypeMapper(void)
{
}

FdoDataType FdoSmLpDataTypeMapper::String2Type( const wchar_t* dataTypeString, bool* pbFound )
{
    int i;

	// Find the datatype enum facet in the map
	for ( i = 0; mMap[i] != NULL; i++ )
	{
		FdoSmLpDataTypeMapEntry* mapEntry = mMap[i];

		if ( mapEntry->mDataTypeString == dataTypeString )
		{
        	if ( pbFound )
		        (*pbFound) = true;

			// found so return the string.
			return( mapEntry->mDataType );
		}
	}

	if ( pbFound ) {
		(*pbFound) = false;
	}
	else {
		throw FdoSchemaException::Create( 
            FdoSmError::NLSGetMessage(
                FDO_NLSID(FDOSM_159), 
				dataTypeString 
            )
		);
	}

	// Get to here only if type not found and pbFound
	// was set.
	// Return value is actually meaningless in this 
	// case but need to return something to prevent a 
	// compilation warning.
	return(FdoDataType_String);
}

FdoStringP FdoSmLpDataTypeMapper::Type2String( FdoDataType dataType )
{
    int i;

	// Find the datatype enum facet in the map
	for ( i = 0; mMap[i] != NULL; i++ )
	{
		FdoSmLpDataTypeMapEntry* mapEntry = mMap[i];

		if ( dataType == mapEntry->mDataType )
		{
			// found so return the string.
			return( (FdoString*) (mapEntry->mDataTypeString) );
		}
	}

	throw FdoSchemaException::Create( 
        FdoSmError::NLSGetMessage(
            FDO_NLSID(FDOSM_159), 
        	dataType 
            )
	);
}

