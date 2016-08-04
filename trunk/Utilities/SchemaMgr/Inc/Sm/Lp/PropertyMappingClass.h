#ifndef FDOSMLPPROPERTYMAPPINGCLASS_H
#define FDOSMLPPROPERTYMAPPINGCLASS_H		1
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

// Property Mapping definition for Class table mapping.
// Object properties that use Class table mapping store their values
// in the table for their type class.
class FdoSmLpPropertyMappingClass : public FdoSmLpPropertyMappingRelation
{
public:
	FdoSmLpPropertyMappingClass( FdoSmLpObjectPropertyDefinition* pParent );

    /// returns the property mapping definition type
    virtual FdoSmLpPropertyMappingType GetType() const;

    /// Convenience function for casting a property mapping definition to a Class type mapping definition.
    /// Returns NULL if the definition is not a class type mapping.
	static const FdoSmLpPropertyMappingClass* Cast( const FdoSmLpPropertyMappingDefinition* src )
	{
		return( (src  && (src->GetType() == FdoSmLpPropertyMappingType_Class)) ? 
					(const FdoSmLpPropertyMappingClass*) src : NULL
		);
	}

protected:
    //Unused constructor needed only to build on Linux
    FdoSmLpPropertyMappingClass() {}

};

typedef FdoPtr<FdoSmLpPropertyMappingClass> FdoSmLpPropertyMappingClassP;

#endif


