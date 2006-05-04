// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  
#include <stdlib.h>
#include <Fdo/Schema/PropertyDefinition.h>
#include "XmlContext.h"


FdoPropertyDefinition::~FdoPropertyDefinition()
{
}

FdoPropertyDefinition::FdoPropertyDefinition() :
    FdoSchemaElement() ,
    m_system(false)
{
}

FdoPropertyDefinition::FdoPropertyDefinition(FdoString* name, FdoString* description, bool system) :
    FdoSchemaElement(name, description),
    m_system(system)
{
}

FdoStringP FdoPropertyDefinition::GetQualifiedName()
{
    FdoPtr<FdoSchemaElement> parent = GetParent();
    FdoStringP qName;
    
    if ( parent )
        qName += parent->GetQualifiedName() + L".";

    qName += GetName();

    return qName;
}

bool FdoPropertyDefinition::GetIsSystem()
{
    return m_system;
}

void FdoPropertyDefinition::SetIsSystem( bool value )
{
    m_system = value;
}

void FdoPropertyDefinition::Set( FdoPropertyDefinition* pProperty, FdoSchemaXmlContext* pContext )
{
    if ( GetPropertyType() != pProperty->GetPropertyType() ) {
        pContext->AddError( 
            FdoSchemaExceptionP(
                FdoSchemaException::Create(
                    FdoException::NLSGetMessage(
                        FDO_NLSID(SCHEMA_25_PROPERTYTYPECONFLICT),
                        (FdoString*) GetQualifiedName()
                    )
                )
            )
        );

        return;
    }

    FdoSchemaElement::Set(pProperty, pContext);
}

void FdoPropertyDefinition::InitFromXml(const FdoString* propertyTypeName, FdoSchemaXmlContext* pContext, FdoXmlAttributeCollection* attrs)
{
    InitFromXml( pContext, attrs );
}

void FdoPropertyDefinition::InitFromXml(FdoSchemaXmlContext* pContext, FdoXmlAttributeCollection* attrs)
{
    FdoSchemaXmlContext* fdoContext = (FdoSchemaXmlContext*) pContext;

    FdoSchemaElement::InitFromXml(pContext, attrs );

    // If property already exists, make sure type change is not attempted.

    if (FdoSchemaElementP (GetParent())) {
        FdoClassDefinitionP pOldClass = fdoContext->FindClass( 
            FdoFeatureSchemasP( fdoContext->GetSchemas() ),
            FdoFeatureSchemaP(GetFeatureSchema())->GetName(),
            FdoSchemaElementP(GetParent())->GetName()
        );

        if ( pOldClass != NULL ) {
            FdoPropertyP pOldProp = FdoPropertiesP( pOldClass->GetProperties() )->FindItem( GetName() );

            if ( pOldProp && (pOldProp->GetPropertyType() != GetPropertyType()) ) {
                pContext->AddError( 
                    FdoSchemaExceptionP(
                        FdoSchemaException::Create(
                            FdoException::NLSGetMessage(
                                FDO_NLSID(SCHEMA_25_PROPERTYTYPECONFLICT),
                                (FdoString*) GetQualifiedName()
                            )
                        )
                    )
                );
                
                return;
            }
        }
    }

}


void FdoPropertyDefinition::_writeXml( FdoSchemaXmlContext* pContext )
{
    FdoSchemaElement::_writeXml(pContext);
}





