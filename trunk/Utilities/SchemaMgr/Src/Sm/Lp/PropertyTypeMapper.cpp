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
#include <Sm/Lp/PropertyTypeMapper.h>
#include <Sm/Error.h>

static FdoSmLpPropertyTypeMapEntry dataEntry( FdoPropertyType_DataProperty, L"Data" );
static FdoSmLpPropertyTypeMapEntry geomEntry( FdoPropertyType_GeometricProperty, L"Geometric" );
static FdoSmLpPropertyTypeMapEntry objEntry( FdoPropertyType_ObjectProperty, L"Object" );
static FdoSmLpPropertyTypeMapEntry assocEntry( FdoPropertyType_AssociationProperty, L"Association" );

FdoSmLpPropertyTypeMapEntry* FdoSmLpPropertyTypeMapper::mMap[] = {
	&dataEntry,
	&geomEntry,
	&objEntry,
	&assocEntry,
    NULL // This array must be null terminated.
};


FdoSmLpPropertyTypeMapEntry::FdoSmLpPropertyTypeMapEntry(FdoPropertyType propType, wchar_t* propTypeString)
{
	mPropertyType = propType;
	mPropertyTypeString = FdoStringP(propTypeString, false);
}

FdoSmLpPropertyTypeMapEntry::~FdoSmLpPropertyTypeMapEntry(void)
{
}

FdoSmLpPropertyTypeMapper::FdoSmLpPropertyTypeMapper(void)
{
}

FdoSmLpPropertyTypeMapper::~FdoSmLpPropertyTypeMapper(void)
{
}

FdoPropertyType FdoSmLpPropertyTypeMapper::String2Type( const wchar_t* propTypeString, bool* pbFound )
{
    int i;

	// Find the datatype enum facet in the map
	for ( i = 0; mMap[i] != NULL; i++ )
	{
		FdoSmLpPropertyTypeMapEntry* mapEntry = mMap[i];

		if ( mapEntry->mPropertyTypeString == propTypeString )
		{
        	if ( pbFound )
		        (*pbFound) = true;

			// found so return the string.
			return( mapEntry->mPropertyType );
		}
	}

	if ( pbFound ) {
		(*pbFound) = false;
	}
	else {
		throw FdoSchemaException::Create( 
            FdoSmError::NLSGetMessage(
                FDO_NLSID(FDOSM_13), 
				propTypeString 
            )
		);
	}

	// Get to here only if type not found and pbFound
	// was set.
	// Return value is actually meaningless in this 
	// case but need to return something to prevent a 
	// compilation warning.
	return(FdoPropertyType_DataProperty);
}

FdoStringP FdoSmLpPropertyTypeMapper::Type2String( FdoPropertyType propType )
{
    int i;

	// Find the datatype enum facet in the map
	for ( i = 0; mMap[i] != NULL; i++ )
	{
		FdoSmLpPropertyTypeMapEntry* mapEntry = mMap[i];

		if ( propType == mapEntry->mPropertyType )
		{
			// found so return the string.
			return( (FdoString*) (mapEntry->mPropertyTypeString) );
		}
	}

	throw FdoSchemaException::Create( 
        FdoSmError::NLSGetMessage(
            FDO_NLSID(FDOSM_14), 
        	propType 
            )
	);
}

