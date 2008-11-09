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
#include <Sm/Lp/PropertyMappingDefinition.h>
#include <Sm/Lp/ObjectPropertyDefinition.h>
#include <Sm/Lp/ObjectPropertyClass.h>


FdoSmLpPropertyMappingDefinition::FdoSmLpPropertyMappingDefinition(FdoSmLpObjectPropertyDefinition* pParent) :
	FdoSmLpSchemaElement( L"", L"", pParent )
{
    SetElementState( pParent->GetElementState() );
}

FdoSmLpPropertyMappingDefinition::~FdoSmLpPropertyMappingDefinition(void)
{
}

const FdoSmLpObjectPropertyClass* FdoSmLpPropertyMappingDefinition::RefTargetClass() const
{
	return (FdoSmLpObjectPropertyClass*) mpTargetClass;
}

bool FdoSmLpPropertyMappingDefinition::AddSchemaMappings( FdoPhysicalPropertyMappingP propMapping, bool bIncludeDefaults ) const
{
    return false;
}

void FdoSmLpPropertyMappingDefinition::WriteDb( FdoSmPhPropertyWriterP pWriter ) const
{
    if ( mpTargetClass ) 
        mpTargetClass->WriteDb( pWriter );
    
    pWriter->SetColumnName( L"n/a" );
}


