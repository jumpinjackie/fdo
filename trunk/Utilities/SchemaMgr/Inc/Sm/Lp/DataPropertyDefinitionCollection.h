#ifndef FDOSMLPDATAPROPERTYCOLLECTION_H
#define FDOSMLPDATAPROPERTYCOLLECTION_H		1
//
// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#ifdef _WIN32
#pragma once
#endif

#include <Sm/Lp/DataPropertyDefinition.h>
#include <Sm/Lp/PropertyDefinitionCollection.h>
#include <Sm/NamedCollection.h>

// Represents a collection of DataPropertyDefinitions.
class FdoSmLpDataPropertyDefinitionCollection : public FdoSmNamedCollection<FdoSmLpDataPropertyDefinition>
{
public:
    /// Given a property collection and a column, this function finds the 
    /// the property for the column.
	//
    /// Parameters:
    /// 	pProperties: the property collection.
    /// 	columnName: name of the column to search for.
	static const FdoSmLpDataPropertyDefinition* ColName2Property( 
		const FdoSmLpPropertyDefinitionCollection* pProperties,
		FdoStringP columnName 
	);

    /// Given a property collection find the Feature id property if any.
	//
    /// Parameters:
    /// 	pProperties: the property collection.
	static const FdoSmLpDataPropertyDefinition* RefFeatIDProperty( 
		const FdoSmLpPropertyDefinitionCollection* pProperties
	);
	static FdoSmLpDataPropertyP FindFeatIDProperty( 
		FdoSmLpPropertiesP pProperties
	);

};

typedef FdoPtr<FdoSmLpDataPropertyDefinitionCollection> FdoSmLpDataPropertiesP;

#endif


