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

#include "stdafx.h"
#include <Sm/Lp/PropertyMappingSingle.h>
#include <Sm/Lp/ObjectPropertyDefinition.h>
#include <Sm/Lp/ObjectPropertyClass.h>
#include <Sm/Error.h>

FdoSmLpPropertyMappingSingle::FdoSmLpPropertyMappingSingle( 
	FdoSmLpObjectPropertyDefinition* pParent,
	FdoSmLpClassDefinition* pParentType,
    FdoPhysicalElementMapping* pOverrides
) :
	FdoSmLpPropertyMappingDefinition(pParent)
{
}

FdoSmLpPropertyMappingSingle::FdoSmLpPropertyMappingSingle( 
	const FdoSmLpPropertyMappingSingle* pBase,
	FdoSmLpObjectPropertyDefinition* pParent, 
	FdoSmLpClassDefinition* pParentType,
    FdoPhysicalElementMapping* pOverrides
) :
	FdoSmLpPropertyMappingDefinition(pParent)
{
}


FdoSmLpPropertyMappingType FdoSmLpPropertyMappingSingle::GetType() const
{
	return( FdoSmLpPropertyMappingType_Single );
}

FdoString* FdoSmLpPropertyMappingSingle::GetPrefix() const
{
	return( mPrefix );
}

void FdoSmLpPropertyMappingSingle::WriteDb( FdoSmPhPropertyWriterP pWriter ) const
{
    FdoSmLpPropertyMappingDefinition::WriteDb( pWriter );
    
    // Put the column prefix in column name.
    pWriter->SetColumnName( mPrefix );
}

void FdoSmLpPropertyMappingSingle::XMLSerialize( FILE* xmlFp, int ref ) const
{
	fprintf( xmlFp, "<mappingDefinition xsi:type=\"Single\" prefix=\"%ls\">\n",
        GetPrefix()
	);

	if ( ref == 0 ) {
		if ( mpTargetClass ) 
			RefTargetClass()->XMLSerialize(xmlFp, ref);

		FdoSmLpSchemaElement::XMLSerialize( xmlFp, ref );
	}

	fprintf( xmlFp, "</mappingDefinition>\n" );
}

void FdoSmLpPropertyMappingSingle::Setup( FdoSmLpObjectPropertyClassP pTargetClass )
{
	// Object property references a type class so create a
	// target class from it. 
	//
	// For concrete table mapping the object property table and 
	// referenced class tables are different. Therefore a wrapper
	// class is created around the object property table. Note that
	// this wrapper class is not added to the schema's class collection.

	mpTargetClass = pTargetClass;
}

void FdoSmLpPropertyMappingSingle::AddPrefixCharError(FdoString* prefix)
{
	GetErrors()->Add( FdoSmErrorType_ObjectPropertyMapping, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
			    FDO_NLSID(FDOSM_316),
				(FdoString*) (GetParent()->GetQName()),
				prefix
			)
		)
	);
}

void FdoSmLpPropertyMappingSingle::SetPrefix( FdoString* prefix )
{
    mPrefix = prefix;
}

void FdoSmLpPropertyMappingSingle::AddPrefixLengthError(FdoString* prefix, FdoSize maxLen)
{
	GetErrors()->Add( FdoSmErrorType_ObjectPropertyMapping, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
			    FDO_NLSID(FDOSM_189),
				(FdoString*) (GetParent()->GetQName()),
                maxLen,
				prefix
			)
		)
	);
}

