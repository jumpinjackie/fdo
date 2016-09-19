#ifndef FDOSMLPPROPERTYMAPPINGRELATION_H
#define FDOSMLPPROPERTYMAPPINGRELATION_H		1
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

class FdoSmLpDataPropertyDefinitionCollection;

// Abstract class for relation type object property mapping definitions.
class FdoSmLpPropertyMappingRelation : public FdoSmLpPropertyMappingDefinition
{
public:
    /// The source and target properties join the table for 
    /// the object property's class with the object property's
    /// table.

    /// Gets the properties in the table for the object property's containing class,
    /// that related to properties in the object property's table.
	const FdoSmLpDataPropertyDefinitionCollection* RefSourceProperties() const;

    /// Gets the properties in the object property's table,
    /// that related to properties in the table for the 
    /// object property's containing class.
	const FdoSmLpDataPropertyDefinitionCollection* RefTargetProperties()const ;

protected:
    //Unused constructor needed only to build on Linux
    FdoSmLpPropertyMappingRelation() {}

	FdoSmLpPropertyMappingRelation(FdoSmLpObjectPropertyDefinition* pParent);

	~FdoSmLpPropertyMappingRelation(void);

	FdoPtr<FdoSmLpDataPropertyDefinitionCollection> mpSourceProperties;

	FdoPtr<FdoSmLpDataPropertyDefinitionCollection> mpTargetProperties;
};

typedef FdoPtr<FdoSmLpPropertyMappingRelation> FdoSmLpPropertyMappingRelationP;

#endif


