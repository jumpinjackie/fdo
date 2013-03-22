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
#include <Sm/Lp/ClassTypeMapper.h>
#include <Sm/Error.h>

static FdoSmLpClassTypeMapEntry classEntry( FdoClassType_Class, L"Class" );
static FdoSmLpClassTypeMapEntry featEntry( FdoClassType_FeatureClass, L"Feature" );

FdoSmLpClassTypeMapEntry* FdoSmLpClassTypeMapper::mMap[] = {
	&classEntry,
	&featEntry,
    NULL // This array must be null terminated.
};


FdoSmLpClassTypeMapEntry::FdoSmLpClassTypeMapEntry(FdoClassType classType, wchar_t* classTypeString)
{
	mClassType = classType;
	mClassTypeString = FdoStringP(classTypeString, false);
}

FdoSmLpClassTypeMapEntry::~FdoSmLpClassTypeMapEntry(void)
{
}

FdoSmLpClassTypeMapper::FdoSmLpClassTypeMapper(void)
{
}

FdoSmLpClassTypeMapper::~FdoSmLpClassTypeMapper(void)
{
}

FdoClassType FdoSmLpClassTypeMapper::String2Type( const wchar_t* classTypeString, bool* pbFound )
{
    int i;

	// Find the classtype enum facet in the map
	for ( i = 0; mMap[i] != NULL; i++ )
	{
		FdoSmLpClassTypeMapEntry* mapEntry = mMap[i];

		if ( mapEntry->mClassTypeString == classTypeString )
		{
        	if ( pbFound )
		        (*pbFound) = true;

			// found so return the string.
			return( mapEntry->mClassType );
		}
	}

	if ( pbFound ) {
		(*pbFound) = false;
	}
	else {
		throw FdoSchemaException::Create( 
            FdoSmError::NLSGetMessage(
                FDO_NLSID(FDOSM_11), 
				classTypeString 
            )
		);
	}

	// Get to here only if type not found and pbFound
	// was set.
	// Return value is actually meaningless in this 
	// case but need to return something to prevent a 
	// compilation warning.
	return(FdoClassType_Class);
}

FdoStringP FdoSmLpClassTypeMapper::Type2String( FdoClassType classType )
{
    int i;

	// Find the datatype enum facet in the map
	for ( i = 0; mMap[i] != NULL; i++ )
	{
		FdoSmLpClassTypeMapEntry* mapEntry = mMap[i];

		if ( classType == mapEntry->mClassType )
		{
			// found so return the string.
			return( (FdoString*) (mapEntry->mClassTypeString) );
		}
	}

    throw FdoSchemaException::Create( 
        FdoSmError::NLSGetMessage(
            FDO_NLSID(FDOSM_12), 
        	classType 
            )
	);
}

