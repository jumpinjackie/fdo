#ifndef FDOSMLPPROPERTYMAPPINGCONCRETE_H
#define FDOSMLPPROPERTYMAPPINGCONCRETE_H		1
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

#include <Sm/Lp/PropertyMappingRelation.h>

class FdoSmLpObjectPropertyDefinition;
class FdoSmLpClassDefinition;

// Property Mapping definition for Concrete table mapping.
// Each object properties that uses Concrete table mapping 
// has its own table for storing values.
class FdoSmLpPropertyMappingConcrete : public FdoSmLpPropertyMappingRelation
{
public:
    /// Construct a new concrete mapping definition
	FdoSmLpPropertyMappingConcrete( 
        FdoSmLpObjectPropertyDefinition* pParent, 
        FdoSmLpClassDefinition* pParentType,
        FdoPhysicalElementMapping* pOverrides
    );

    /// Construct a concrete mapping definition from a base definition.
	FdoSmLpPropertyMappingConcrete( 
		const FdoSmLpPropertyMappingConcrete* pBase,
		FdoSmLpObjectPropertyDefinition* pParent, 
		FdoSmLpClassDefinition* pParentType,
        FdoPhysicalElementMapping* pOverrides
	);

    virtual ~FdoSmLpPropertyMappingConcrete();

    /// Abstract operation that must be implemented by derived classes to return
    /// the concrete class type.
    virtual FdoSmLpPropertyMappingType GetType() const;

    /// Convenience function for casting a property mapping definition to a concrete definition
    /// Returns NULL if the definition is not a concrete definition.
	static const FdoSmLpPropertyMappingConcrete* Cast( const FdoSmLpPropertyMappingDefinition* src )
	{
		return( (src  && (src->GetType() == FdoSmLpPropertyMappingType_Concrete)) ? 
					(const FdoSmLpPropertyMappingConcrete*) src : NULL
		);
	}

    /// Mainly for unit testing.
	virtual void XMLSerialize( FILE* xmlFp, int ref ) const;

protected:
    //Unused constructor needed only to build on Linux
    FdoSmLpPropertyMappingConcrete() {}

    /// Initialize from the associated target class.
	void Setup( FdoPtr<FdoSmLpObjectPropertyClass> pTargetClass );
};

typedef FdoPtr<FdoSmLpPropertyMappingConcrete> FdoSmLpPropertyMappingConcreteP;

#endif


