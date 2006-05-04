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
#include <Fdo/Schema/ObjectPropertyDefinition.h>
#include <Fdo/Schema/FeatureSchema.h>
#include "XmlContext.h"

FdoObjectPropertyDefinition* FdoObjectPropertyDefinition::Create()
{
    return new FdoObjectPropertyDefinition();
}

FdoObjectPropertyDefinition* FdoObjectPropertyDefinition::Create(FdoString* name, FdoString* description, bool system)
{
    return new FdoObjectPropertyDefinition(name, description, system);
}

FdoObjectPropertyDefinition::~FdoObjectPropertyDefinition()
{
    FDO_SAFE_RELEASE(m_classCHANGED);
    FDO_SAFE_RELEASE(m_identityPropertyCHANGED);

    FDO_SAFE_RELEASE(m_class);
    FDO_SAFE_RELEASE(m_identityProperty);
}

void FdoObjectPropertyDefinition::Dispose()
{
    delete this;
}

FdoObjectPropertyDefinition::FdoObjectPropertyDefinition() : FdoPropertyDefinition()
{
    m_class = NULL;
    m_objectType = (FdoObjectType)0;
    m_orderType = (FdoOrderType)0;
    m_identityProperty = NULL;

    m_classCHANGED = NULL;
    m_objectTypeCHANGED = (FdoObjectType)0;
    m_orderTypeCHANGED = (FdoOrderType)0;
    m_identityPropertyCHANGED = NULL;
}

FdoObjectPropertyDefinition::FdoObjectPropertyDefinition(FdoString* name, FdoString* description, bool system) :
    FdoPropertyDefinition(name, description, system)
{
    m_class = NULL;
    m_objectType = (FdoObjectType)0;
    m_orderType = (FdoOrderType)0;
    m_identityProperty = NULL;

    m_classCHANGED = NULL;
    m_objectTypeCHANGED = (FdoObjectType)0;
    m_orderTypeCHANGED = (FdoOrderType)0;
    m_identityPropertyCHANGED = NULL;
}

FdoPropertyType FdoObjectPropertyDefinition::GetPropertyType()
{
    return FdoPropertyType_ObjectProperty;
}

FdoClassDefinition* FdoObjectPropertyDefinition::GetClass()
{
    return FDO_SAFE_ADDREF(m_class);
}

void FdoObjectPropertyDefinition::SetClass(FdoClassDefinition* value)
{
    _StartChanges();
    FDO_SAFE_RELEASE(m_class);
    m_class = FDO_SAFE_ADDREF(value);
    SetElementState(FdoSchemaElementState_Modified);
}

FdoDataPropertyDefinition* FdoObjectPropertyDefinition::GetIdentityProperty()
{
    return FDO_SAFE_ADDREF(m_identityProperty);
}

void FdoObjectPropertyDefinition::SetIdentityProperty(FdoDataPropertyDefinition* value)
{
    _StartChanges();
    FDO_SAFE_RELEASE(m_identityProperty);
    m_identityProperty = FDO_SAFE_ADDREF(value);
    SetElementState(FdoSchemaElementState_Modified);
}

FdoObjectType FdoObjectPropertyDefinition::GetObjectType()
{
    return m_objectType;
}

void FdoObjectPropertyDefinition::SetObjectType(FdoObjectType value)
{
    _StartChanges();
    m_objectType = value;
    SetElementState(FdoSchemaElementState_Modified);
}

FdoOrderType FdoObjectPropertyDefinition::GetOrderType()
{
    return m_orderType;
}

void FdoObjectPropertyDefinition::SetOrderType(FdoOrderType value)
{
    _StartChanges();
    m_orderType = value;
    SetElementState(FdoSchemaElementState_Modified);
}


void FdoObjectPropertyDefinition::_StartChanges()
{
    if (!(m_changeInfoState & (CHANGEINFO_PRESENT|CHANGEINFO_PROCESSING)))
    {
        FdoPropertyDefinition::_StartChanges();

        m_classCHANGED = FDO_SAFE_ADDREF(m_class);
        m_objectTypeCHANGED = m_objectType;
        m_orderTypeCHANGED = m_orderType;
        m_identityPropertyCHANGED = FDO_SAFE_ADDREF(m_identityProperty);
    }
}

void FdoObjectPropertyDefinition::_RejectChanges()
{
    if (m_changeInfoState & CHANGEINFO_PROCESSED)
        return; // already processed
    FdoPropertyDefinition::_RejectChanges();

    if (m_changeInfoState & CHANGEINFO_PRESENT)
    {
        // restore
        if (m_class != m_classCHANGED)
        {
            if (m_class)
                m_class->_RejectChanges();
            FDO_SAFE_RELEASE(m_class);
            m_class = FDO_SAFE_ADDREF(m_classCHANGED);
        }
        m_objectType = m_objectTypeCHANGED;
        m_orderType = m_orderTypeCHANGED;
        if (m_identityProperty != m_identityPropertyCHANGED)
        {
            if (m_identityProperty)
                m_identityProperty->_RejectChanges();
            FDO_SAFE_RELEASE(m_identityProperty);
            m_identityProperty = FDO_SAFE_ADDREF(m_identityPropertyCHANGED);
        }

        // reset
        FDO_SAFE_RELEASE(m_classCHANGED);
        m_objectTypeCHANGED = (FdoObjectType)0;
        m_orderTypeCHANGED = (FdoOrderType)0;
        FDO_SAFE_RELEASE(m_identityPropertyCHANGED);
    }

    if (m_class)
        m_class->_RejectChanges();
    if (m_identityProperty)
        m_identityProperty->_RejectChanges();
}

void FdoObjectPropertyDefinition::_AcceptChanges()
{
    if (m_changeInfoState & CHANGEINFO_PROCESSED)
        return; // already processed
    FdoPropertyDefinition::_AcceptChanges();

    if (m_changeInfoState & CHANGEINFO_PRESENT)
    {
        // reset
        if (m_classCHANGED)
            m_classCHANGED->_AcceptChanges();
        FDO_SAFE_RELEASE(m_classCHANGED);
        m_objectTypeCHANGED = (FdoObjectType)0;
        m_orderTypeCHANGED = (FdoOrderType)0;
        if (m_identityPropertyCHANGED)
            m_identityPropertyCHANGED->_AcceptChanges();
        FDO_SAFE_RELEASE(m_identityPropertyCHANGED);
    }

    if (m_class)
        m_class->_AcceptChanges();
    if (m_identityProperty)
        m_identityProperty->_AcceptChanges();
}

void FdoObjectPropertyDefinition::_BeginChangeProcessing()
{
    if (m_changeInfoState & CHANGEINFO_PROCESSING)
        return; // already marked for processing
    FdoPropertyDefinition::_BeginChangeProcessing();

    if (m_class)
        m_class->_BeginChangeProcessing();
    if (m_identityProperty)
        m_identityProperty->_BeginChangeProcessing();
}

void FdoObjectPropertyDefinition::_EndChangeProcessing()
{
    if (!(m_changeInfoState & CHANGEINFO_PROCESSING))
        return; // processing flag already cleared

    FdoPropertyDefinition::_EndChangeProcessing();
    if (m_class)
        m_class->_EndChangeProcessing();
    if (m_identityProperty)
        m_identityProperty->_EndChangeProcessing();
}

void FdoObjectPropertyDefinition::Set( FdoPropertyDefinition* pProperty, FdoSchemaXmlContext* pContext )
{
    FdoPropertyDefinition::Set(pProperty, pContext);

    FdoObjectPropertyDefinition* pObjProperty = (FdoObjectPropertyDefinition*) pProperty;

    SetClass( FdoClassDefinitionP(pObjProperty->GetClass()) );
    SetIdentityProperty( FdoDataPropertyP(pObjProperty->GetIdentityProperty()) );
    SetObjectType( pObjProperty->GetObjectType() );
    SetOrderType( pObjProperty->GetOrderType() );
}

void FdoObjectPropertyDefinition::InitFromXml(const FdoString* propertyTypeName, FdoSchemaXmlContext* pContext, FdoXmlAttributeCollection* attrs)
{
    // Error if FDO and XML property types are not the same.
    if ( wcscmp( propertyTypeName, L"ObjectProperty" ) != 0 ) {
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

    // Initialize generic property parts
    FdoPropertyDefinition::InitFromXml(pContext, attrs );

    // Reset this object property back to default settings.
    FDO_SAFE_RELEASE(m_class);
    m_objectType = (FdoObjectType)0;
    m_orderType = (FdoOrderType)0;
    FDO_SAFE_RELEASE(m_identityProperty);

    // Update object property from XML attributes.
    FdoXmlAttributeP attr = attrs->FindItem( L"objectType" );

    if ( attr != NULL ) {
        if ( wcscmp( attr->GetValue(), L"collection" ) == 0 )
            m_objectType = FdoObjectType_Collection;

        if ( wcscmp( attr->GetValue(), L"orderedcollection" ) == 0 )
            m_objectType = FdoObjectType_OrderedCollection;
    }

    attr = attrs->FindItem( L"orderType" );

    if ( attr && wcscmp( attr->GetValue(), L"descending" ) == 0 )
        m_orderType = FdoOrderType_Descending;

    FdoXmlAttributeP opSchema = attrs->FindItem( L"classSchema" );
    FdoXmlAttributeP opClass = attrs->FindItem( L"class" );

    if ( (opSchema != NULL) && (opClass != NULL) )
        // Object property class might not have been read yet.
        // Just add a reference to be resolved later
        pContext->AddObjPropRef( this, pContext->DecodeName(opSchema->GetValue()), pContext->DecodeName(opClass->GetValue()) );

    attr = attrs->FindItem( L"identityProperty" );

    if ( attr != NULL )
        // Object property class might not have been read yet.
        // Just add a reference to be resolved later
        pContext->AddObjIdPropRef( this, pContext->DecodeName(attr->GetValue()) );
}

void FdoObjectPropertyDefinition::_writeXml( FdoSchemaXmlContext* pContext )
{
    FdoXmlWriterP writer = pContext->GetXmlWriter();

    // Write an object property element
    writer->WriteStartElement( L"ObjectProperty" );

    // Convert object type to string attribute
    switch ( m_objectType ) {
    case FdoObjectType_Value:
        writer->WriteAttribute( L"objectType", L"value" );
        break;
    case FdoObjectType_Collection:
        writer->WriteAttribute( L"objectType", L"collection" );
        break;
    case FdoObjectType_OrderedCollection:
        writer->WriteAttribute( L"objectType", L"orderedcollection" );
        writer->WriteAttribute( 
            L"orderType", 
            (m_orderType == FdoOrderType_Ascending) ? L"ascending" : L"descending"
        );
        break;
    }

    // Write identity property as attribute
    if ( m_identityProperty ) 
        writer->WriteAttribute( 
            L"identityProperty",
            pContext->EncodeName(m_identityProperty->GetName())
        );

    // Write the object property class as a reference
    if ( m_class ) {
        writer->WriteAttribute( 
            L"classSchema", 
            pContext->EncodeName( pContext->RefClass2SchemaName(this, m_class) )
        );
        writer->WriteAttribute( 
            L"class",
            pContext->EncodeName(m_class->GetName())
        );
    }

    //Write generic property parts
    FdoPropertyDefinition::_writeXml(pContext);

    // close the element
    writer->WriteEndElement();
}



