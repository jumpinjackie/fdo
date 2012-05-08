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

#include <Rdbms/Override/RdbmsOvObjectPropertyDefinition.h>

FdoRdbmsOvObjectPropertyDefinition::FdoRdbmsOvObjectPropertyDefinition()
{
}

FdoRdbmsOvObjectPropertyDefinition::FdoRdbmsOvObjectPropertyDefinition(FdoString* name) : FdoRdbmsOvPropertyDefinition(name)
{
}

FdoRdbmsOvObjectPropertyDefinition::~FdoRdbmsOvObjectPropertyDefinition()
{
}

FdoRdbmsOvPropertyMappingDefinition* FdoRdbmsOvObjectPropertyDefinition::GetMappingDefinition()
{
    return FDO_SAFE_ADDREF( (FdoRdbmsOvPropertyMappingDefinition*) mPropertyMappingDefinition );
}

void FdoRdbmsOvObjectPropertyDefinition::SetMappingDefinition(FdoRdbmsOvPropertyMappingDefinition* propertyMappingDefinition)
{
    mPropertyMappingDefinition = FDO_SAFE_ADDREF(propertyMappingDefinition);
    mPropertyMappingDefinition->SetParent(this);
}

void FdoRdbmsOvObjectPropertyDefinition::InitFromXml(
    FdoXmlSaxContext* pContext, 
    FdoXmlAttributeCollection* attrs,
    FdoString* mappingType,
    FdoXmlAttributeCollection* mappingAttrs
)
{
    FdoPhysicalPropertyMapping::InitFromXml( pContext, attrs );

    // Create the right type of Property Mapping override and add
    // it to the object property
    if ( wcscmp(mappingType, L"PropertyMappingSingle") == 0 ) {
        FdoRdbmsOvPropertyMappingSingleP singleMapping = CreateSingleMapping( true );
        if ( singleMapping ) 
            singleMapping->InitFromXml( pContext, mappingAttrs );   
    }

    if ( wcscmp(mappingType, L"PropertyMappingClass") == 0 ) {
        FdoRdbmsOvPropertyMappingClassP classMapping = CreateClassMapping( true );
        if ( classMapping ) 
            classMapping->InitFromXml( pContext, mappingAttrs );   
    }

    if ( wcscmp(mappingType, L"PropertyMappingConcrete") == 0 ) {
        FdoRdbmsOvPropertyMappingConcreteP concreteMapping = CreateConcreteMapping( true );
        if ( concreteMapping ) 
            concreteMapping->InitFromXml( pContext, mappingAttrs );   
    }
}

void FdoRdbmsOvObjectPropertyDefinition::_writeXmlContents(FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags)
{
    if ( mPropertyMappingDefinition )
        mPropertyMappingDefinition->_writeXml( xmlWriter, flags );
}


