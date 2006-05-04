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
#include "XmlContext.h"
#include <Fdo/Schema/NetworkLinkFeatureClass.h>
#include <Fdo/Schema/NetworkNodeFeatureClass.h>

FdoNetworkLinkFeatureClass* FdoNetworkLinkFeatureClass::Create()
{
    return new FdoNetworkLinkFeatureClass();
}

FdoNetworkLinkFeatureClass* FdoNetworkLinkFeatureClass::Create(FdoString* name, FdoString* description)
{
    return new FdoNetworkLinkFeatureClass( name, description );
}

FdoNetworkLinkFeatureClass::FdoNetworkLinkFeatureClass():
m_startNodeFeature(NULL),
m_endNodeFeature(NULL),
m_bStartNode(false),
m_bEndNode(false),
m_startNodeFeatureHandler(NULL),
m_endNodeFeatureHandler(NULL)
{
}

FdoNetworkLinkFeatureClass::FdoNetworkLinkFeatureClass(FdoString* name, FdoString* description): 
FdoNetworkFeatureClass( name, description ),
m_startNodeFeature(NULL),
m_endNodeFeature(NULL),
m_bStartNode(false),
m_bEndNode(false),
m_startNodeFeatureHandler(NULL),
m_endNodeFeatureHandler(NULL)
{
}

FdoNetworkLinkFeatureClass::~FdoNetworkLinkFeatureClass()
{
    FDO_SAFE_RELEASE(m_startNodeFeature);
    FDO_SAFE_RELEASE(m_endNodeFeature);
    FDO_SAFE_RELEASE(m_startNodeFeatureCHANGED);
    FDO_SAFE_RELEASE(m_endNodeFeatureCHANGED);
    FDO_SAFE_RELEASE(m_startNodeFeatureHandler);
    FDO_SAFE_RELEASE(m_endNodeFeatureHandler);
}

void FdoNetworkLinkFeatureClass::Dispose()
{
    delete this;
}

FdoClassType FdoNetworkLinkFeatureClass::GetClassType()
{
    return FdoClassType_NetworkLinkClass;
}

FdoAssociationPropertyDefinition* FdoNetworkLinkFeatureClass::GetStartNodeProperty()
{
    return FDO_SAFE_ADDREF(m_startNodeFeature);
}

void FdoNetworkLinkFeatureClass::SetStartNodeProperty(FdoAssociationPropertyDefinition* value)
{
    //
    // Make sure the associated class is a network node class
    if (value)
    {
        FdoPtr<FdoClassDefinition>assocClass = value->GetAssociatedClass();
        if( ! assocClass || assocClass->GetClassType() != FdoClassType_NetworkNodeClass )
            throw FdoSchemaException::Create(FdoException::NLSGetMessage(FDO_NLSID(SCHEMA_37_REQUIREDNODEFEATURE)));
        //
        // Make sure the associated node network calss is the same as the link network class
        FdoPtr<FdoAssociationPropertyDefinition> netwrkAssoc = GetNetworkProperty();
        FdoPtr<FdoAssociationPropertyDefinition> nodeNetwrkAssoc = (FdoPtr<FdoNetworkNodeFeatureClass>((FdoNetworkNodeFeatureClass*)value->GetAssociatedClass()))->GetNetworkProperty();
        if( netwrkAssoc && nodeNetwrkAssoc )
        {
            FdoPtr<FdoClassDefinition>networkAssocClass = netwrkAssoc->GetAssociatedClass();
            FdoPtr<FdoClassDefinition>networkNodeAssocClass = nodeNetwrkAssoc->GetAssociatedClass();
            if( networkAssocClass && networkNodeAssocClass && wcscmp( networkAssocClass->GetName(), networkNodeAssocClass->GetName() )  )
                throw FdoSchemaException::Create(FdoException::NLSGetMessage(FDO_NLSID(SCHEMA_44_LINKSTARTORENDNODENETWORKERROR)));
        }
    }

    _StartChanges();
    FDO_SAFE_RELEASE(m_startNodeFeature);
    m_startNodeFeature = FDO_SAFE_ADDREF(value);
    SetElementState(FdoSchemaElementState_Modified);
}

FdoAssociationPropertyDefinition* FdoNetworkLinkFeatureClass::GetEndNodeProperty()
{
    return FDO_SAFE_ADDREF(m_endNodeFeature);
}

void FdoNetworkLinkFeatureClass::SetEndNodeProperty(FdoAssociationPropertyDefinition* value)
{
    //
    // Make sure the associated class is a network node class
    if (value)
    {
        FdoPtr<FdoClassDefinition>assocClass = value->GetAssociatedClass();
        if( ! assocClass || assocClass->GetClassType() != FdoClassType_NetworkNodeClass )
            throw FdoSchemaException::Create(FdoException::NLSGetMessage(FDO_NLSID(SCHEMA_37_REQUIREDNODEFEATURE)));
    }
    
    //
    // Make sure the associated node network calss is the same as the link network class
    FdoPtr<FdoAssociationPropertyDefinition> netwrkAssoc = GetNetworkProperty();
    FdoPtr<FdoAssociationPropertyDefinition> nodeNetwrkAssoc = (FdoPtr<FdoNetworkNodeFeatureClass>((FdoNetworkNodeFeatureClass*)value->GetAssociatedClass()))->GetNetworkProperty();
    if( netwrkAssoc && nodeNetwrkAssoc )
    {
        FdoPtr<FdoClassDefinition>networkAssocClass = netwrkAssoc->GetAssociatedClass();
        FdoPtr<FdoClassDefinition>networkNodeAssocClass = nodeNetwrkAssoc->GetAssociatedClass();
        if( networkAssocClass && networkNodeAssocClass && wcscmp( networkAssocClass->GetName(), networkNodeAssocClass->GetName() )  )
            throw FdoSchemaException::Create(FdoException::NLSGetMessage(FDO_NLSID(SCHEMA_44_LINKSTARTORENDNODENETWORKERROR)));
    }

    _StartChanges();
    FDO_SAFE_RELEASE(m_endNodeFeature);
    m_endNodeFeature = FDO_SAFE_ADDREF(value);
    SetElementState(FdoSchemaElementState_Modified);
}

void FdoNetworkLinkFeatureClass::_StartChanges()
{
    if (!(m_changeInfoState & (CHANGEINFO_PRESENT|CHANGEINFO_PROCESSING)))
    {
        FdoNetworkFeatureClass::_StartChanges();

        m_startNodeFeatureCHANGED = FDO_SAFE_ADDREF(m_startNodeFeature);
        m_endNodeFeatureCHANGED = FDO_SAFE_ADDREF(m_endNodeFeature);
    }
}

void FdoNetworkLinkFeatureClass::_BeginChangeProcessing()
{
    if (m_changeInfoState & CHANGEINFO_PROCESSING)
        return; // already marked for processing
    FdoNetworkFeatureClass::_BeginChangeProcessing();

    m_startNodeFeatureCHANGED = FDO_SAFE_ADDREF(m_startNodeFeature);
    m_endNodeFeatureCHANGED = FDO_SAFE_ADDREF(m_endNodeFeature);
}

void FdoNetworkLinkFeatureClass::_AcceptChanges()
{
    if (m_changeInfoState & CHANGEINFO_PROCESSED)
        return; // already processed
    
    FdoNetworkFeatureClass::_AcceptChanges();

    if (m_changeInfoState & CHANGEINFO_PRESENT)
    {
        // reset
        FDO_SAFE_RELEASE(m_startNodeFeatureCHANGED);
        FDO_SAFE_RELEASE(m_endNodeFeatureCHANGED);
    }
}

void FdoNetworkLinkFeatureClass::_RejectChanges()
{
    if (m_changeInfoState & CHANGEINFO_PROCESSED)
        return; // already processed
    FdoNetworkFeatureClass::_RejectChanges();

    if (m_changeInfoState & CHANGEINFO_PRESENT)
    {
        // restore
        if (m_startNodeFeature != m_startNodeFeatureCHANGED)
        {
            FDO_SAFE_RELEASE(m_startNodeFeature);
            m_startNodeFeature = FDO_SAFE_ADDREF(m_startNodeFeatureCHANGED);
        }
        if (m_endNodeFeature != m_endNodeFeatureCHANGED)
        {
            FDO_SAFE_RELEASE(m_endNodeFeature);
            m_endNodeFeature = FDO_SAFE_ADDREF(m_endNodeFeatureCHANGED);
        }
        
        // reset
        FDO_SAFE_RELEASE(m_startNodeFeatureCHANGED);
        FDO_SAFE_RELEASE(m_endNodeFeatureCHANGED);
    }
}

void FdoNetworkLinkFeatureClass::_EndChangeProcessing()
{
    if (!(m_changeInfoState & CHANGEINFO_PROCESSING))
        return; // processing flag already cleared

    FdoNetworkFeatureClass::_EndChangeProcessing();
}
    

void FdoNetworkLinkFeatureClass::Set( FdoClassDefinition* pClass, FdoSchemaXmlContext* pContext )
{
    FdoNetworkFeatureClass::Set( pClass, pContext );

    FdoNetworkLinkFeatureClass *pNetworkLinkFeatureClass = (FdoNetworkLinkFeatureClass*) pClass;

    SetStartNodeProperty(FdoAssociationPropertyP(pNetworkLinkFeatureClass->GetStartNodeProperty()));
    SetEndNodeProperty(FdoAssociationPropertyP(pNetworkLinkFeatureClass->GetEndNodeProperty()));
}

void FdoNetworkLinkFeatureClass::InitFromXml(const FdoString* classTypeName, FdoSchemaXmlContext* pContext, FdoXmlAttributeCollection* attrs)
{
    // Make sure FDO and XML class type agree, class type change not allowed.
    if ( (wcscmp( classTypeName, L"ClassDefinition" ) != 0) && (wcscmp( classTypeName, L"NetworkLinkFeatureClass" ) != 0) )
    {
        pContext->AddError( 
            FdoSchemaExceptionP(
                FdoSchemaException::Create(
                    FdoException::NLSGetMessage(
                        FDO_NLSID(SCHEMA_24_CLASSTYPECONFLICT),
                        (FdoString*) GetQualifiedName()
                    )
                )
            )
        );

        return;
    }

    FDO_SAFE_RELEASE(m_startNodeFeature);
    FDO_SAFE_RELEASE(m_endNodeFeature);

    FdoNetworkFeatureClass::InitFromXml(pContext, attrs );

}

FdoXmlSaxHandler* FdoNetworkLinkFeatureClass::XmlStartElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname, 
        FdoXmlAttributeCollection* atts
)
{
    FdoSchemaXmlContext* fdoContext = (FdoSchemaXmlContext*) context;

    // Handle generic sub-elements
    FdoXmlSaxHandler* pRet = FdoNetworkFeatureClass::XmlStartElement(context, uri, name, qname, atts);
    FdoPtr<FdoPropertyDefinition> pProp;


    if ( !pRet )
    {
        if ( wcscmp(name, L"StartNode") == 0 )
              m_bStartNode = true;
        if ( wcscmp(name, L"EndNode") == 0 )
              m_bEndNode = true;

        if (wcscmp(name, L"AssociationProperty") == 0 )
        {
            if (m_bStartNode)
            {
                m_startNodeFeatureHandler = FdoAssociationPropertyDefinition::Create();
                m_startNodeFeatureHandler->SetParent(this);
                m_startNodeFeatureHandler->InitFromXml( name, fdoContext, atts );
                pRet = m_startNodeFeatureHandler;
            }

            if (m_bEndNode)
            {
                m_endNodeFeatureHandler = FdoAssociationPropertyDefinition::Create();
                m_endNodeFeatureHandler->SetParent(this);
                m_endNodeFeatureHandler->InitFromXml( name, fdoContext, atts );
                pRet = m_endNodeFeatureHandler;
            }
        }
    }

    return(pRet);
}

FdoBoolean FdoNetworkLinkFeatureClass::XmlEndElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname
)
{
    FdoSchemaXmlContext* fdoContext = (FdoSchemaXmlContext*) context;

    FdoNetworkFeatureClass::XmlEndElement(context, uri, name, qname);

    if (m_bStartNode == true && wcscmp(name, L"AssociationProperty") == 0 ) {
        fdoContext->AddNetworkLinkStartAssocPropRef( this, this->GetQualifiedName() + L"." + m_startNodeFeatureHandler->GetName());
        FDO_SAFE_RELEASE(m_startNodeFeatureHandler);
    }

    if (m_bEndNode == true && wcscmp(name, L"AssociationProperty") == 0 ) {
        fdoContext->AddNetworkLinkEndAssocPropRef( this, this->GetQualifiedName() + L"." + m_endNodeFeatureHandler->GetName());
        FDO_SAFE_RELEASE(m_endNodeFeatureHandler);
    }

    if ( wcscmp(name, L"StartNode") == 0 )
        m_bStartNode = false;

    if ( wcscmp(name, L"EndNode") == 0 )
        m_bEndNode = false;


    return(false);
}


void FdoNetworkLinkFeatureClass::_writeXml( FdoSchemaXmlContext* pContext )
{
    FdoXmlWriterP       writer = pContext->GetXmlWriter();

    writer->WriteStartElement( L"NetworkLinkFeatureClass" );

    if ( m_startNodeFeature )
    {
        writer->WriteAttribute( 
            L"StartNodeProperty",
            pContext->EncodeName(m_startNodeFeature->GetName())
        );
    }

    if ( m_endNodeFeature )
    {
        writer->WriteAttribute( 
            L"EndNodeProperty",
            pContext->EncodeName(m_endNodeFeature->GetName())
        );
    }

    FdoNetworkFeatureClass::_writeXml(pContext);

    if ( pContext->CheckWriteAssoc(this, m_startNodeFeature) )
    {
        writer->WriteStartElement( L"StartNode" );
        m_startNodeFeature->_writeXml(pContext);
        writer->WriteEndElement();
    }

    if ( pContext->CheckWriteAssoc(this, m_endNodeFeature) )
    {
        writer->WriteStartElement( L"EndNode" );
        m_endNodeFeature->_writeXml(pContext);
        writer->WriteEndElement();
    }

    writer->WriteEndElement();
}

