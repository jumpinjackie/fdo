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
#include <Sm/Lp/PropertyMappingConcrete.h>
#include <Sm/Lp/ObjectPropertyDefinition.h>
#include <Sm/Lp/ObjectPropertyClass.h>
#include <Sm/Error.h>

FdoSmLpPropertyMappingConcrete::FdoSmLpPropertyMappingConcrete( 
	FdoSmLpObjectPropertyDefinition* pParent,
	FdoSmLpClassDefinition* pParentType,
    FdoPhysicalElementMapping* pOverrides
) :
	FdoSmLpPropertyMappingRelation(pParent)
{
}

FdoSmLpPropertyMappingConcrete::FdoSmLpPropertyMappingConcrete( 
	const FdoSmLpPropertyMappingConcrete* pBase,
	FdoSmLpObjectPropertyDefinition* pParent, 
	FdoSmLpClassDefinition* pParentType,
    FdoPhysicalElementMapping* pOverrides
) :
	FdoSmLpPropertyMappingRelation(pParent)
{
}

FdoSmLpPropertyMappingConcrete::~FdoSmLpPropertyMappingConcrete()
{
}

FdoSmLpPropertyMappingType FdoSmLpPropertyMappingConcrete::GetType() const
{
	return( FdoSmLpPropertyMappingType_Concrete );
}

void FdoSmLpPropertyMappingConcrete::XMLSerialize( FILE* xmlFp, int ref ) const
{
	fprintf( xmlFp, "<mappingDefinition xsi:type=\"Concrete\" >\n"
	);

	if ( ref == 0 ) {
		fprintf( xmlFp, "<sourceProperties>\n" );
		for ( int i = 0; i < mpSourceProperties->GetCount(); i++ ) 
			RefSourceProperties()->RefItem(i)->XMLSerialize(xmlFp, 1);
		fprintf( xmlFp, "</sourceProperties>\n" );

		fprintf( xmlFp, "<targetProperties>\n" );
		for ( int i = 0; i < mpTargetProperties->GetCount(); i++ ) 
			RefTargetProperties()->RefItem(i)->XMLSerialize(xmlFp, 1);
		fprintf( xmlFp, "</targetProperties>\n" );

		if ( mpTargetClass ) 
			RefTargetClass()->XMLSerialize(xmlFp, ref);

		FdoSmLpSchemaElement::XMLSerialize( xmlFp, ref );
	}

	fprintf( xmlFp, "</mappingDefinition>\n" );
}

void FdoSmLpPropertyMappingConcrete::Setup( FdoSmLpObjectPropertyClassP pTargetClass )
{
	// Object property references a type class so create a
	// target class from it. 
	//
	// For concrete table mapping the object property table and 
	// referenced class tables are different. Therefore a wrapper
	// class is created around the object property table. Note that
	// this wrapper class is not added to the schema's class collection.

	mpTargetClass = pTargetClass;

	// Source and target properties are calculated by the object
	// property class. 
	mpSourceProperties = pTargetClass->GetSourceProperties();
	mpTargetProperties = pTargetClass->GetTargetProperties();
}


