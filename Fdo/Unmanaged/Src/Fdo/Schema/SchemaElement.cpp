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
#include <Fdo/Schema/SchemaElement.h>
#include <Fdo/Schema/FeatureSchema.h>
#include "XmlContext.h"
#include "XmlError.h"
#include "StringUtility.h"

FdoSchemaElement::FdoSchemaElement()
{
    m_parent = NULL;
    m_name = NULL;
    m_description = NULL;
    m_attributes = NULL;
    m_state = FdoSchemaElementState_Added;

    m_changeInfoState = 0;
    m_parentCHANGED = NULL;
    m_nameCHANGED = NULL;
    m_descriptionCHANGED = NULL;
}


FdoSchemaElement::FdoSchemaElement(FdoString* name, FdoString* description)
{
    VldName(name);

    m_parent = NULL;
    m_name = FdoStringUtility::MakeString(name);
    m_description = FdoStringUtility::MakeString(description);
    m_attributes = NULL;
    m_state = FdoSchemaElementState_Added;

    m_changeInfoState = 0;
    m_parentCHANGED = NULL;
    m_nameCHANGED = NULL;
    m_descriptionCHANGED = NULL;
}

FdoSchemaElement::~FdoSchemaElement()
{
    if (m_nameCHANGED && m_name != m_nameCHANGED)
        FdoStringUtility::ClearString(m_nameCHANGED);

    if (m_descriptionCHANGED && m_description != m_descriptionCHANGED)
        FdoStringUtility::ClearString(m_descriptionCHANGED);

    //TODO:
    //we do not addref to the parent...
    //FDO_SAFE_RELEASE(m_pParent);

    FdoStringUtility::ClearString(m_name);
    FdoStringUtility::ClearString(m_description);
    FDO_SAFE_RELEASE(m_attributes);
}

FdoSchemaElement* FdoSchemaElement::GetParent()
{
    return FDO_SAFE_ADDREF(m_parent);
}


FdoFeatureSchema* FdoSchemaElement::GetFeatureSchema()
{
    FdoSchemaElement*   p = GetParent();

    if (p == NULL)
        return NULL;

    do
    {
        FdoFeatureSchema *schema = dynamic_cast<FdoFeatureSchema*>(p);
        if (schema != NULL)
            return schema;

        FdoSchemaElement*   pNext = p->GetParent();
        FDO_SAFE_RELEASE(p);
        p = pNext;
    } while(p != NULL);
    
    return NULL;
}


FdoSchemaElementState FdoSchemaElement::GetElementState()
{
    return m_state;
}

FdoSchemaAttributeDictionary* FdoSchemaElement::GetAttributes()
{
    if (!m_attributes)
        m_attributes = new FdoSchemaAttributeDictionary(*this);
    return FDO_SAFE_ADDREF(m_attributes);
}

FdoString* FdoSchemaElement::GetName()
{
    return m_name;
}

FdoStringP FdoSchemaElement::GetQualifiedName()
{
    return GetName();
}

void FdoSchemaElement::SetName(FdoString* value)
{
    VldName(value);

    _StartChanges();
    if (m_name != NULL)
    {
        if (m_nameCHANGED != m_name)
            FdoStringUtility::ClearString(m_name);
    }

    m_name = FdoStringUtility::MakeString(value);
    SetElementState(FdoSchemaElementState_Modified);
}

FdoString* FdoSchemaElement::GetDescription()
{
    return m_description;
}

void FdoSchemaElement::SetDescription(FdoString* value)
{
    _StartChanges();
    if (m_description != NULL)
    {
        if (m_descriptionCHANGED != m_description)
            FdoStringUtility::ClearString(m_description);
    }

    m_description = FdoStringUtility::MakeString(value);
    SetElementState(FdoSchemaElementState_Modified);
}

void FdoSchemaElement::Delete()
{
    SetElementState(FdoSchemaElementState_Deleted);

}

void FdoSchemaElement::Set( FdoSchemaElement* pElement, FdoSchemaXmlContext* pContext )
{
    FdoSADP     newAttributes = pElement->GetAttributes();
    FdoInt32    attCount = 0;
    FdoString** pNewAttNames = newAttributes->GetAttributeNames(attCount);
    FdoInt32    idx = 0;

    SetDescription( pElement->GetDescription() );

    FDO_SAFE_RELEASE(m_attributes);

    for ( idx = 0; idx < attCount; idx++ ) {
        FdoSADP myAttributes = GetAttributes();

        myAttributes->Add( pNewAttNames[idx], newAttributes->GetAttributeValue(pNewAttNames[idx]) );
    }
}

void FdoSchemaElement::InitFromXml(FdoSchemaXmlContext* pContext, FdoXmlAttributeCollection* attrs)
{
    // reset element to default state.

    if (m_descriptionCHANGED && m_description != m_descriptionCHANGED)
        FdoStringUtility::ClearString(m_descriptionCHANGED);
    m_descriptionCHANGED = NULL;

    FdoStringUtility::ClearString(m_description);
    FDO_SAFE_RELEASE(m_attributes);
    m_state = FdoSchemaElementState_Added;

    if ( m_name == NULL )
        // Element name must be present.
        SetName( pContext->DecodeName(FdoXmlAttributeP(attrs->GetItem( L"name" ))->GetValue()) );
}

FdoXmlSaxHandler* FdoSchemaElement::XmlStartElement(
    FdoXmlSaxContext* context, 
    FdoString* uri, 
    FdoString* name, 
    FdoString* qname, 
    FdoXmlAttributeCollection* atts
)
{
    FdoXmlSaxHandler* pRet = NULL;

    if ( wcscmp( name, L"Description" ) == 0 ) {
        // Set up a SAX handler to read description from current sub-element content
        m_XmlContentHandler = FdoXmlCharDataHandler::Create();
        pRet = m_XmlContentHandler;
    } else if ( wcscmp( name, L"Error" ) == 0 ) {
        // Set up a SAX handler to read error sub-element
        m_XmlErrorHandler = FdoSchemaXmlError::Create(atts);
        pRet = m_XmlErrorHandler.p;
    } else if ( wcscmp( name, L"SAD" ) == 0 ) {
        // Set up SAX handler to read Schema Attribute Dictionary.
        m_attributes = new FdoSchemaAttributeDictionary(*this);
        pRet = m_attributes;
    }

    return(pRet);
}

FdoBoolean FdoSchemaElement::XmlEndElement(
    FdoXmlSaxContext* context, 
    FdoString* uri, 
    FdoString* name, 
    FdoString* qname
)
{
    FdoSchemaXmlContext* fdoContext = (FdoSchemaXmlContext*) context;

    if ( (wcscmp(name, L"Description") == 0) && (m_XmlContentHandler != NULL) ) 
        SetDescription( m_XmlContentHandler->GetString() );

    if ( (wcscmp(name, L"Error") == 0) && (m_XmlErrorHandler != NULL) )
        m_XmlErrorHandler->Apply(fdoContext);

    return(false);
}

void FdoSchemaElement::SetParent(FdoSchemaElement* value)
{
    if (value)
    {
        // Verify this wouldn't make element its own parent/grandparent
        FdoSchemaElement*    parent = value;
        parent->AddRef();
        while (parent)
        {
            if (parent == this)
            {
                parent->Release();  
                throw FdoSchemaException::Create(FdoException::NLSGetMessage(FDO_NLSID(SCHEMA_11_OWNPARENT)));
            }

            FdoSchemaElement*    next = parent->GetParent();
            parent->Release();
            parent = next;
        }
    }

    _StartChanges();

    //don't addref to the parent
    //TODO: fix this
    /*
    if (m_pParent)
        m_pParent->Release();
    
    if (value)
        value->AddRef();
        */

    m_parent = value;

    if (m_parent == NULL)
        SetElementState(FdoSchemaElementState_Detached);
    else
        SetElementState(FdoSchemaElementState_Modified);
}

void FdoSchemaElement::SetElementState(FdoSchemaElementState value)
{
    // Let parent know we've been modified
    if (m_state == FdoSchemaElementState_Unchanged && m_parent)
        m_parent->SetElementState(FdoSchemaElementState_Modified);

    if (m_state == FdoSchemaElementState_Added)
    {
        // Added sticks unless deleted
        if (value == FdoSchemaElementState_Deleted)
            m_state = value;
    }
    else if (m_state == FdoSchemaElementState_Deleted)
    {
        // Deleted sticks unless added
        if (value == FdoSchemaElementState_Added)
            m_state = value;
    }
    else
        m_state = value;
}

void FdoSchemaElement::PropertyRemoved(const FdoPropertyDefinition* property)
{
    // Default doesn't do anything.  Overridden in FdoClassDefintion.
}

bool FdoSchemaElement::IsProperty(const FdoPropertyDefinition* property)
{
    return false;
}

bool FdoSchemaElement::IsSubClass()
{
    return false;
}

void FdoSchemaElement::VldName( FdoString* name )
{
    if ( name ) {
        if ( wcschr(name, ':') )
            throw FdoSchemaException::Create(
                FdoException::NLSGetMessage(
                    FDO_NLSID(SCHEMA_52_BADELEMENTNAME),
                    name,
                    ':'
                )
            );

        if ( wcschr(name, '.') )
            throw FdoSchemaException::Create(
                FdoException::NLSGetMessage(
                    FDO_NLSID(SCHEMA_52_BADELEMENTNAME),
                    name,
                    '.'
                )
            );
    }
}

void FdoSchemaElement::_StartChanges()
{
    if (!(m_changeInfoState & (CHANGEINFO_PRESENT|CHANGEINFO_PROCESSING)))
    {
        // Save initial state in case changes are rejected
        m_parentCHANGED = m_parent;   // not AddRef'ed for same reason as in SetParent()
        m_nameCHANGED = m_name;
        m_descriptionCHANGED = m_description;
        m_changeInfoState |= CHANGEINFO_PRESENT;
    }
}

void FdoSchemaElement::_RejectChanges()
{
    if (m_changeInfoState & CHANGEINFO_PROCESSED)
        return; // already processed

    m_changeInfoState |= CHANGEINFO_PROCESSED;

    if (m_changeInfoState & CHANGEINFO_PRESENT)
    {
        // restore
        m_parent = m_parentCHANGED;
        if (m_name != m_nameCHANGED)
        {
            if (m_name)
                FdoStringUtility::ClearString(m_name);
            m_name = m_nameCHANGED;
        }
        if (m_description != m_descriptionCHANGED)
        {
            if (m_description)
                FdoStringUtility::ClearString(m_description);
            m_description = m_descriptionCHANGED;
        }

        // reset
        m_parentCHANGED = NULL;
        m_nameCHANGED = NULL;
        m_descriptionCHANGED = NULL;
    }

    if (m_attributes)
        m_attributes->_RejectChanges();
    m_state = FdoSchemaElementState_Unchanged;
}

void FdoSchemaElement::_AcceptChanges()
{
    if (m_changeInfoState & CHANGEINFO_PROCESSED)
        return; // already processed

    m_changeInfoState |= CHANGEINFO_PROCESSED;

    if (m_changeInfoState & CHANGEINFO_PRESENT)
    {
        // reset
        if (m_name != m_nameCHANGED)
        {
            if (m_nameCHANGED)
                FdoStringUtility::ClearString(m_nameCHANGED);
        }
        if (m_description != m_descriptionCHANGED)
        {
            if (m_descriptionCHANGED)
                FdoStringUtility::ClearString(m_descriptionCHANGED);
        }

        m_parentCHANGED = NULL;
        m_nameCHANGED = NULL;
        m_descriptionCHANGED = NULL;
    }

    if (m_attributes)
        m_attributes->_AcceptChanges();
    m_state = FdoSchemaElementState_Unchanged;
}

void FdoSchemaElement::_BeginChangeProcessing()
{
    if (m_changeInfoState & CHANGEINFO_PROCESSING)
        return; // already marked for processing
    m_changeInfoState |= CHANGEINFO_PROCESSING;
}

void FdoSchemaElement::_EndChangeProcessing()
{
    if (!(m_changeInfoState & CHANGEINFO_PROCESSING))
        return; // processing flag already cleared

    m_changeInfoState = 0;
    m_state = FdoSchemaElementState_Unchanged;
}

void FdoSchemaElement::_writeXml( FdoSchemaXmlContext* pContext )
{
    FdoXmlWriterP       writer = pContext->GetXmlWriter();

    // Name is the only attribute to write.
    // Encode the name if asked to do so.
    writer->WriteAttribute( 
        L"name", 
        pContext->EncodeName(GetName())
    );

    // Write description as a sub-element
    if ( GetDescription() ) {
        writer->WriteStartElement( L"Description" );
        writer->WriteCharacters( GetDescription() );
        writer->WriteEndElement();
    }

    // Write the Schema Attribute Dictionary.
    if ( m_attributes ) 
        m_attributes->_writeXml(pContext);

}





