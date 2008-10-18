#ifndef FDOSMLPPROPERTYMAPPINGSINGLE_H
#define FDOSMLPPROPERTYMAPPINGSINGLE_H		1
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

#ifdef _WIN32
#pragma once
#endif

#include <Sm/Lp/PropertyMappingDefinition.h>

class FdoSmLpObjectPropertyDefinition;
class FdoSmLpClassDefinition;

// Property Mapping definition for an Object Property. Determines
// that the Object Property is stored in the table for containing class.
class FdoSmLpPropertyMappingSingle : public FdoSmLpPropertyMappingDefinition
{
public:
    /// returns the property mapping definition type
    virtual FdoSmLpPropertyMappingType GetType() const;

    /// Get the prefix to prepend to the columns referenced
    /// by the object property.
    FdoString* GetPrefix() const ;

    /// Write object property attributes particular to this type of object property mapping.
    virtual void WriteDb( FdoSmPhPropertyWriterP pWriter ) const;

    /// Mainly for unit testing.
	virtual void XMLSerialize( FILE* xmlFp, int ref ) const;

    /// Convenience function for casting a property mapping definition to a single mapping definition.
    /// Returns NULL if the definition is not a single mapping.
	static const FdoSmLpPropertyMappingSingle* Cast( const FdoSmLpPropertyMappingDefinition* src )
	{
		return( (src  && (src->GetType() == FdoSmLpPropertyMappingType_Single)) ? 
					(const FdoSmLpPropertyMappingSingle*) src : NULL
		);
	}

protected:
    //Unused constructor needed only to build on Linux
    FdoSmLpPropertyMappingSingle() {}

    /// Construct a new single table mapping definition
	FdoSmLpPropertyMappingSingle( 
        FdoSmLpObjectPropertyDefinition* pParent, 
        FdoSmLpClassDefinition* pParentType,
        FdoPhysicalElementMapping* pOverrides
    );

    /// Construct a single table mapping definition from a base definition.
	FdoSmLpPropertyMappingSingle( 
		const FdoSmLpPropertyMappingSingle* pBase,
		FdoSmLpObjectPropertyDefinition* pParent, 
		FdoSmLpClassDefinition* pParentType,
        FdoPhysicalElementMapping* pOverrides
	);


    /// Initialize from the associated target class.
	void Setup( FdoPtr<FdoSmLpObjectPropertyClass> pTargetClass );

    /// Set the column prefix
    void SetPrefix( FdoString* prefix );

    void AddPrefixCharError(FdoString* prefix);
    void AddPrefixLengthError(FdoString* prefix, FdoSize maxLen);

private:
    FdoStringP mPrefix;
};

typedef FdoPtr<FdoSmLpPropertyMappingSingle> FdoSmLpPropertyMappingSingleP;

#endif


