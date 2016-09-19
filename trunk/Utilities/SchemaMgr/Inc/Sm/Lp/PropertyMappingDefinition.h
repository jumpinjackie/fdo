#ifndef FDOSMLPPROPERTYMAPPINGDEFINITION_H
#define FDOSMLPPROPERTYMAPPINGDEFINITION_H		1
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

#include <Sm/Lp/PropertyMappingType.h>
#include <Sm/Lp/SchemaElement.h>
#include <Sm/Ph/PropertyWriter.h>

class FdoSmLpObjectPropertyDefinition;
class FdoSmLpObjectPropertyClass;

// Abstract base class for Property Mapping definitions for 
// an Object Property. 
class FdoSmLpPropertyMappingDefinition : public FdoSmLpSchemaElement
{
public:

    /// Abstract operation that must be implemented by derived classes to return
    /// the concrete class type.
    virtual FdoSmLpPropertyMappingType GetType() const = 0;

    /// Get the target class for this mapping definition.
	const FdoSmLpObjectPropertyClass* RefTargetClass() const;

    /// Get the Schema Mappings for this object property mapping definition
	//
    /// Parameters:
    /// 	bIncludeDefaults - 
    ///          true: include all schema mappings.
    ///          false: include only non-default mappings.
    /// Returns:
    ///      returns NULL. Sub-classes override this function to fill in the
    ///      schema mappings specific to the property mapping type.

    virtual bool AddSchemaMappings( FdoPhysicalPropertyMappingP propMapping, bool bIncludeDefaults ) const;
    
    /// Write object property attributes particular to this type of object property mapping.
    virtual void WriteDb( FdoSmPhPropertyWriterP pWriter ) const;

protected:
    //Unused constructor needed only to build on Linux
    FdoSmLpPropertyMappingDefinition() {}

	FdoSmLpPropertyMappingDefinition(FdoSmLpObjectPropertyDefinition* pParent);
	virtual ~FdoSmLpPropertyMappingDefinition(void);

	FdoPtr<FdoSmLpObjectPropertyClass> mpTargetClass;
};

typedef FdoPtr<FdoSmLpPropertyMappingDefinition> FdoSmLpPropertyMappingP;

#endif


