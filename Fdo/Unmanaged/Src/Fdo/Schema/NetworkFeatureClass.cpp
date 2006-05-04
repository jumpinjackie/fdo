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
#include <Fdo/Schema/NetworkFeatureClass.h>

FdoNetworkFeatureClass::FdoNetworkFeatureClass():
m_costProperty(NULL),
m_network(NULL),
m_referencedFeature(NULL),
m_parentNetworkFeature(NULL),
m_costPropertyCHANGED(NULL),
m_networkCHANGED(NULL),
m_referencedFeatureCHANGED(NULL),
m_parentNetworkFeatureCHANGED(NULL),
m_bNetwork(false),
m_bReferenceFeature(false),
m_bParentNetwork(false),
m_networkHandler(NULL),
m_referencedFeatureHandler(NULL),
m_parentNetworkFeatureHandler(NULL)
{
}

FdoNetworkFeatureClass::FdoNetworkFeatureClass(FdoString* name, FdoString* description):
FdoFeatureClass(name, description),
m_costProperty(NULL),
m_network(NULL),
m_referencedFeature(NULL),
m_parentNetworkFeature(NULL),
m_costPropertyCHANGED(NULL),
m_networkCHANGED(NULL),
m_referencedFeatureCHANGED(NULL),
m_parentNetworkFeatureCHANGED(NULL),
m_bNetwork(false),
m_bReferenceFeature(false),
m_bParentNetwork(false),
m_networkHandler(NULL),
m_referencedFeatureHandler(NULL),
m_parentNetworkFeatureHandler(NULL)
{
}

FdoNetworkFeatureClass::~FdoNetworkFeatureClass()
{
    FDO_SAFE_RELEASE(m_costProperty);
    FDO_SAFE_RELEASE(m_network);
    FDO_SAFE_RELEASE(m_referencedFeature);
    FDO_SAFE_RELEASE(m_parentNetworkFeature);

    FDO_SAFE_RELEASE(m_costPropertyCHANGED);
    FDO_SAFE_RELEASE(m_networkCHANGED);
    FDO_SAFE_RELEASE(m_referencedFeatureCHANGED);
    FDO_SAFE_RELEASE(m_parentNetworkFeatureCHANGED);

    FDO_SAFE_RELEASE(m_networkHandler);
    FDO_SAFE_RELEASE(m_referencedFeatureHandler);
    FDO_SAFE_RELEASE(m_parentNetworkFeatureHandler);
}

FdoDataPropertyDefinition* FdoNetworkFeatureClass::GetCostProperty()
{
    return FDO_SAFE_ADDREF(m_costProperty);
}

void FdoNetworkFeatureClass::SetCostProperty(FdoDataPropertyDefinition* value)
{
    //
    // Make sure the property belongs to the class properties.
    if (value)
    {
        try
        {
           	FdoPtr<FdoPropertyDefinitionCollection> properties = GetProperties();
			FdoPtr<FdoPropertyDefinition>(properties->GetItem( value->GetName()));
        }
        catch( FdoSchemaException *exp )
        {
            exp->Release();
            throw FdoSchemaException::Create(FdoException::NLSGetMessage(FDO_NLSID(SCHEMA_43_COSTPROPERTYNOTFOUND)));
        }
        
        //
        // Make sure the cost property is a numeric property
        if( ! (value->GetDataType() >= FdoDataType_Decimal &&  value->GetDataType() <= FdoDataType_Single) )
            throw FdoSchemaException::Create(FdoException::NLSGetMessage(FDO_NLSID(SCHEMA_40_REQUIREDNUMERICTYPE)));
    }
    _StartChanges();
    FDO_SAFE_RELEASE(m_costProperty);
    m_costProperty = FDO_SAFE_ADDREF(value);
    SetElementState(FdoSchemaElementState_Modified);
}


FdoAssociationPropertyDefinition* FdoNetworkFeatureClass::GetNetworkProperty()
{
    return FDO_SAFE_ADDREF(m_network);
}

 
void FdoNetworkFeatureClass::SetNetworkProperty(FdoAssociationPropertyDefinition* value)
{
    //
    // Make sure the associated class is a network class
    FdoPtr<FdoClassDefinition>assocNtwrkClass;
    if (value)
    {
        assocNtwrkClass = value->GetAssociatedClass();
        if( ! assocNtwrkClass || assocNtwrkClass->GetClassType() != FdoClassType_NetworkClass )
            throw FdoSchemaException::Create(FdoException::NLSGetMessage(FDO_NLSID(SCHEMA_39_REQUIREDNETWORKFCLASS)));
    }
    //
    // Make sure the parent network class is the same as the child network class
    if( m_parentNetworkFeature )
    {
        FdoPtr<FdoClassDefinition>parentNtwrkClass = m_parentNetworkFeature->GetAssociatedClass();
        if( parentNtwrkClass && wcscmp( parentNtwrkClass->GetName(), assocNtwrkClass->GetName() )  )
            throw FdoSchemaException::Create(FdoException::NLSGetMessage(FDO_NLSID(SCHEMA_42_PARENTCHILDNETWORKNOTSAME)));
    }
    _StartChanges();
    FDO_SAFE_RELEASE(m_network);
    m_network = FDO_SAFE_ADDREF(value);
    SetElementState(FdoSchemaElementState_Modified);
}

FdoAssociationPropertyDefinition* FdoNetworkFeatureClass::GetReferencedFeatureProperty()
{
    return FDO_SAFE_ADDREF(m_referencedFeature);
}
 
void FdoNetworkFeatureClass::SetReferencedFeatureProperty(FdoAssociationPropertyDefinition* value)
{
    _StartChanges();
    FDO_SAFE_RELEASE(m_referencedFeature);
    m_referencedFeature = FDO_SAFE_ADDREF(value);
    SetElementState(FdoSchemaElementState_Modified);
}
    
FdoAssociationPropertyDefinition* FdoNetworkFeatureClass::GetParentNetworkFeatureProperty()
{
    return FDO_SAFE_ADDREF(m_parentNetworkFeature);
}
   
void FdoNetworkFeatureClass::SetParentNetworkFeatureProperty(FdoAssociationPropertyDefinition* value)
{ 
    //
    // Make sure that the parent class type is the same as this class type(i.e node/node or link/link)
    if (value)
    {
        FdoPtr<FdoClassDefinition>parentClass = value->GetAssociatedClass();
        if( ! parentClass || parentClass->GetClassType() != GetClassType() )
            throw FdoSchemaException::Create(FdoException::NLSGetMessage(FDO_NLSID(SCHEMA_41_PARENTCHILDNOTSAMETYPE)));

        //
        // Make sure the parent network class is the same as the child network class
        FdoPtr<FdoAssociationPropertyDefinition>parentAssNetwork = (FdoPtr<FdoNetworkFeatureClass>((FdoNetworkFeatureClass*)value->GetAssociatedClass()))->GetNetworkProperty();
        if( parentAssNetwork && m_network )
        {
            FdoPtr<FdoClassDefinition>parentNtwrkClass = parentAssNetwork->GetAssociatedClass();
            FdoPtr<FdoClassDefinition>ntwrkClass = m_network->GetAssociatedClass();
            if( ntwrkClass && parentNtwrkClass && wcscmp( ntwrkClass->GetName(), parentNtwrkClass->GetName() )  )
                throw FdoSchemaException::Create(FdoException::NLSGetMessage(FDO_NLSID(SCHEMA_42_PARENTCHILDNETWORKNOTSAME)));
        }
    }
    _StartChanges();
    FDO_SAFE_RELEASE(m_parentNetworkFeature);
    m_parentNetworkFeature = FDO_SAFE_ADDREF(value);
    SetElementState(FdoSchemaElementState_Modified);
}

void FdoNetworkFeatureClass::_StartChanges()
{
    if (!(m_changeInfoState & (CHANGEINFO_PRESENT|CHANGEINFO_PROCESSING)))
    {
        FdoFeatureClass::_StartChanges();
    
        m_costPropertyCHANGED = FDO_SAFE_ADDREF(m_costProperty);
        m_networkCHANGED = FDO_SAFE_ADDREF(m_network);
        m_referencedFeatureCHANGED = FDO_SAFE_ADDREF(m_referencedFeature);
        m_parentNetworkFeatureCHANGED = FDO_SAFE_ADDREF(m_parentNetworkFeature);
    }
}

void FdoNetworkFeatureClass::_BeginChangeProcessing()
{
   if (m_changeInfoState & CHANGEINFO_PROCESSING)
        return; // already marked for processing
    FdoFeatureClass::_BeginChangeProcessing();

    m_costPropertyCHANGED = FDO_SAFE_ADDREF(m_costProperty);
    m_networkCHANGED = FDO_SAFE_ADDREF(m_network);
    m_referencedFeatureCHANGED = FDO_SAFE_ADDREF(m_referencedFeature);
    m_parentNetworkFeatureCHANGED = FDO_SAFE_ADDREF(m_parentNetworkFeature);
}

void FdoNetworkFeatureClass::_AcceptChanges()
{
    if (m_changeInfoState & CHANGEINFO_PROCESSED)
        return; // already processed
    
    FdoFeatureClass::_AcceptChanges();

    if (m_changeInfoState & CHANGEINFO_PRESENT)
    {
        // reset
        FDO_SAFE_RELEASE(m_costPropertyCHANGED);
        FDO_SAFE_RELEASE(m_networkCHANGED);
        FDO_SAFE_RELEASE(m_referencedFeatureCHANGED);
        FDO_SAFE_RELEASE(m_parentNetworkFeatureCHANGED);
    }
}

void FdoNetworkFeatureClass::_RejectChanges()
{
    if (m_changeInfoState & CHANGEINFO_PROCESSED)
        return; // already processed
    FdoFeatureClass::_RejectChanges();

    if (m_changeInfoState & CHANGEINFO_PRESENT)
    {
        if (m_costProperty != m_costPropertyCHANGED)
        {
            FDO_SAFE_RELEASE(m_costProperty);
            m_costProperty = FDO_SAFE_ADDREF(m_costPropertyCHANGED);
        }
        if (m_network != m_networkCHANGED)
        {
            FDO_SAFE_RELEASE(m_network);
            m_network = FDO_SAFE_ADDREF(m_networkCHANGED);
        }
        if (m_referencedFeature != m_referencedFeatureCHANGED)
        {
            FDO_SAFE_RELEASE(m_referencedFeature);
            m_referencedFeature = FDO_SAFE_ADDREF(m_referencedFeatureCHANGED);
        }
        if (m_parentNetworkFeature != m_parentNetworkFeatureCHANGED)
        {
            FDO_SAFE_RELEASE(m_parentNetworkFeature);
            m_parentNetworkFeature = FDO_SAFE_ADDREF(m_parentNetworkFeatureCHANGED);
        }

        // reset
        FDO_SAFE_RELEASE(m_costPropertyCHANGED);
        FDO_SAFE_RELEASE(m_networkCHANGED);
        FDO_SAFE_RELEASE(m_referencedFeatureCHANGED);
        FDO_SAFE_RELEASE(m_parentNetworkFeatureCHANGED);
    }
}

void FdoNetworkFeatureClass::_EndChangeProcessing()
{
    if (!(m_changeInfoState & CHANGEINFO_PROCESSING))
        return; // processing flag already cleared

    FdoFeatureClass::_EndChangeProcessing();
}

void FdoNetworkFeatureClass::Set( FdoClassDefinition* pClass, FdoSchemaXmlContext* pContext )
{
    FdoFeatureClass::Set( pClass, pContext );

    FdoNetworkFeatureClass *pNetworkFeatureClass = (FdoNetworkFeatureClass*) pClass;

    SetCostProperty(FdoDataPropertyP(pNetworkFeatureClass->GetCostProperty()));
    SetNetworkProperty(FdoAssociationPropertyP(pNetworkFeatureClass->GetNetworkProperty()));
    SetReferencedFeatureProperty(FdoAssociationPropertyP(pNetworkFeatureClass->GetReferencedFeatureProperty()));
    SetParentNetworkFeatureProperty(FdoAssociationPropertyP(pNetworkFeatureClass->GetParentNetworkFeatureProperty()));
}

void FdoNetworkFeatureClass::InitFromXml(FdoSchemaXmlContext* pContext, FdoXmlAttributeCollection* attrs)
{
    FDO_SAFE_RELEASE(m_costProperty);
    FDO_SAFE_RELEASE(m_network);
    FDO_SAFE_RELEASE(m_referencedFeature);
    FDO_SAFE_RELEASE(m_parentNetworkFeature);

    FdoXmlAttributeP attr = attrs->FindItem( L"costProperty" );

    if (attr)
        pContext->AddNetworkFeatureCostPropRef( this, pContext->DecodeName(attr->GetValue()));

    FdoFeatureClass::InitFromXml(L"ClassDefinition", pContext, attrs );

}


FdoXmlSaxHandler* FdoNetworkFeatureClass::XmlStartElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname, 
        FdoXmlAttributeCollection* atts
)
{
    FdoSchemaXmlContext* fdoContext = (FdoSchemaXmlContext*) context;

    // Handle generic sub-elements
    FdoXmlSaxHandler* pRet = FdoFeatureClass::XmlStartElement(context, uri, name, qname, atts);
    FdoPtr<FdoPropertyDefinition> pProp;


    if ( !pRet )
    {
        if ( wcscmp(name, L"NetworkProperty") == 0 )
              m_bNetwork = true;
        if ( wcscmp(name, L"ReferenceFeatureProperty") == 0 )
              m_bReferenceFeature = true;
        if ( wcscmp(name, L"ParentNetworkProperty") == 0 )
              m_bParentNetwork = true;
        
        if ((m_bNetwork || m_bReferenceFeature || m_bParentNetwork ) && wcscmp(name, L"AssociationProperty") == 0 ) {
            if (m_bNetwork)
            {
                m_networkHandler = FdoAssociationPropertyDefinition::Create();
                m_networkHandler->SetParent(this);
                m_networkHandler->InitFromXml( name, fdoContext, atts );
                pRet = m_networkHandler;
            }
            if (m_bReferenceFeature)
            {
                m_referencedFeatureHandler = FdoAssociationPropertyDefinition::Create();
                m_referencedFeatureHandler->SetParent(this);
                m_referencedFeatureHandler->InitFromXml( name, fdoContext, atts );
                pRet = m_referencedFeatureHandler;
            }
            if (m_bParentNetwork)
            {
                m_parentNetworkFeatureHandler = FdoAssociationPropertyDefinition::Create();
                m_parentNetworkFeatureHandler->SetParent(this);
                m_parentNetworkFeatureHandler->InitFromXml( name, fdoContext, atts );
                pRet = m_parentNetworkFeatureHandler;
            }
        }
    }

    return(pRet);
}

FdoBoolean FdoNetworkFeatureClass::XmlEndElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname
)
{
    FdoSchemaXmlContext* fdoContext = (FdoSchemaXmlContext*) context;

    // End any generic sub-elements.
    FdoFeatureClass::XmlEndElement(context, uri, name, qname);

    if (m_bNetwork == true && wcscmp(name, L"AssociationProperty") == 0 ) {
        fdoContext->AddNetworkFeatureNetworkPropRef( this, this->GetQualifiedName() + L"." + m_networkHandler->GetName());
        FDO_SAFE_RELEASE(m_networkHandler);
    }

    if (m_bReferenceFeature == true && wcscmp(name, L"AssociationProperty") == 0 ) {
        fdoContext->AddNetworkFeatureRefFeatPropRef( this, this->GetQualifiedName() + L"." + m_referencedFeatureHandler->GetName());
        FDO_SAFE_RELEASE(m_referencedFeatureHandler);
    }

    if (m_bParentNetwork == true && wcscmp(name, L"AssociationProperty") == 0 ) {
        fdoContext->AddNetworkFeatureParentNetworkFeatPropRef( this, this->GetQualifiedName() + L"." + m_parentNetworkFeatureHandler->GetName());
        FDO_SAFE_RELEASE(m_parentNetworkFeatureHandler);
    }

    if ( wcscmp(name, L"NetworkProperty") == 0 )
           m_bNetwork = false;

    if ( wcscmp(name, L"ReferenceFeatureProperty") == 0 )
          m_bReferenceFeature = false;

    if ( wcscmp(name, L"ParentNetworkProperty") == 0 )
          m_bParentNetwork = false;

    return(false);
}

void FdoNetworkFeatureClass::_writeXml( FdoSchemaXmlContext* pContext  )
{
    FdoXmlWriterP       writer = pContext->GetXmlWriter();

    if ( m_costProperty ) 
    {
        writer->WriteAttribute( 
            L"costProperty",
            pContext->EncodeName(m_costProperty->GetName())
        );

    }

    if ( m_network)
    {
        writer->WriteAttribute( 
            L"network",
            pContext->EncodeName(m_network->GetName())
        );
    }

    if (m_referencedFeature)
    {
        writer->WriteAttribute( 
            L"referencedFeature",
            pContext->EncodeName(m_referencedFeature->GetName())
        );
    }

    if (m_parentNetworkFeature)
    {
        writer->WriteAttribute( 
            L"parentNetworkFeature",
            pContext->EncodeName(m_parentNetworkFeature->GetName()) 
        );
    }

    FdoFeatureClass::_writeBaseXml(pContext);

    // Write base properties used in complexType restriction
    writeXmlBaseProperties(pContext);

    if ( pContext->CheckWriteAssoc(this, m_network) )
    {
        writer->WriteStartElement(L"NetworkProperty");
        m_network->_writeXml(pContext);
        writer->WriteEndElement();
    }

    if ( pContext->CheckWriteAssoc(this, m_referencedFeature) )
    {
        writer->WriteStartElement(L"ReferenceFeatureProperty");
        m_referencedFeature->_writeXml(pContext);
        writer->WriteEndElement();
    }

    if ( pContext->CheckWriteAssoc(this, m_parentNetworkFeature) )
    {
        writer->WriteStartElement(L"ParentNetworkProperty");
        m_parentNetworkFeature->_writeXml(pContext);
        writer->WriteEndElement();
    }
}

    

