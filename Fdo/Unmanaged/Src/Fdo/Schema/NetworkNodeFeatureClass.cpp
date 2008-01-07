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
#include <Fdo/Schema/NetworkNodeFeatureClass.h>

FdoNetworkNodeFeatureClass* FdoNetworkNodeFeatureClass::Create()
{
    return new FdoNetworkNodeFeatureClass();
}

FdoNetworkNodeFeatureClass* FdoNetworkNodeFeatureClass::Create(FdoString* name, FdoString* description)
{
    return new FdoNetworkNodeFeatureClass( name, description );
}

FdoNetworkNodeFeatureClass::FdoNetworkNodeFeatureClass(): 
m_layer(NULL),
m_layerCHANGED(NULL),
m_bStartLayer(false),
m_layerHandler(NULL)
{
}

FdoNetworkNodeFeatureClass::FdoNetworkNodeFeatureClass(FdoString* name, FdoString* description):
FdoNetworkFeatureClass(name, description),
m_layer(NULL),
m_layerCHANGED(NULL),
m_bStartLayer(false),
m_layerHandler(NULL)
{
}

FdoNetworkNodeFeatureClass::~FdoNetworkNodeFeatureClass()
{
    FDO_SAFE_RELEASE(m_layer);
    FDO_SAFE_RELEASE(m_layerCHANGED);
    FDO_SAFE_RELEASE(m_layerHandler);
}

void FdoNetworkNodeFeatureClass::Dispose()
{
    delete this;
}

FdoClassType FdoNetworkNodeFeatureClass::GetClassType()
{
    return FdoClassType_NetworkNodeClass;
}

FdoAssociationPropertyDefinition* FdoNetworkNodeFeatureClass::GetLayerProperty()
{
    return FDO_SAFE_ADDREF(m_layer);
}

void FdoNetworkNodeFeatureClass::SetLayerProperty(FdoAssociationPropertyDefinition* value)
{
    //
    // Make sure the associated class is a network layer class
    if (value)
    {
        FdoPtr<FdoClassDefinition>assocClass = value->GetAssociatedClass();
        if( ! assocClass || assocClass->GetClassType() != FdoClassType_NetworkLayerClass )
            throw FdoSchemaException::Create(FdoException::NLSGetMessage(FDO_NLSID(SCHEMA_38_REQUIREDLAYERCLASS)));
    }
    
    _StartChanges();
    FDO_SAFE_RELEASE(m_layer);
    m_layer = FDO_SAFE_ADDREF(value);
    SetElementState(FdoSchemaElementState_Modified);
}

void FdoNetworkNodeFeatureClass::_StartChanges()
{
    if (!(m_changeInfoState & (CHANGEINFO_PRESENT|CHANGEINFO_PROCESSING)))
    {
        FdoNetworkFeatureClass::_StartChanges();

        m_layerCHANGED = FDO_SAFE_ADDREF(m_layer);
    }
}

void FdoNetworkNodeFeatureClass::_BeginChangeProcessing()
{
    if (m_changeInfoState & CHANGEINFO_PROCESSING)
        return; // already marked for processing
    FdoNetworkFeatureClass::_BeginChangeProcessing();

    m_layerCHANGED = FDO_SAFE_ADDREF(m_layer);
}

void FdoNetworkNodeFeatureClass::_AcceptChanges()
{
    if (m_changeInfoState & CHANGEINFO_PROCESSED)
        return; // already processed
    
    FdoNetworkFeatureClass::_AcceptChanges();

    if (m_changeInfoState & CHANGEINFO_PRESENT)
    {
        // reset
        FDO_SAFE_RELEASE(m_layerCHANGED);
    }
}

void FdoNetworkNodeFeatureClass::_RejectChanges()
{
    if (m_changeInfoState & CHANGEINFO_PROCESSED)
        return; // already processed
    FdoNetworkFeatureClass::_RejectChanges();

    if (m_changeInfoState & CHANGEINFO_PRESENT)
    {
        // restore
        if (m_layer != m_layerCHANGED)
        {
            FDO_SAFE_RELEASE(m_layer);
            m_layer = FDO_SAFE_ADDREF(m_layerCHANGED);
        }
        
        // reset
        FDO_SAFE_RELEASE(m_layerCHANGED);
    }
}

void FdoNetworkNodeFeatureClass::_EndChangeProcessing()
{
    if (!(m_changeInfoState & CHANGEINFO_PROCESSING))
        return; // processing flag already cleared

    FdoNetworkFeatureClass::_EndChangeProcessing();
}


void FdoNetworkNodeFeatureClass::Set( FdoClassDefinition* pClass, FdoSchemaMergeContext* pContext )
{
    FdoNetworkFeatureClass::Set( pClass, pContext );

    // Base function catches class type mismatch so silently quit on type mismatch
    if ( GetClassType() == pClass->GetClassType() ) {
        if ( pContext->GetIgnoreStates() || (GetElementState() == FdoSchemaElementState_Added) || (pClass->GetElementState() == FdoSchemaElementState_Modified) ) {
            FdoNetworkNodeFeatureClass *pNetworkNodeFeatureClass = (FdoNetworkNodeFeatureClass*) pClass;

            // Update this class from the given class. The same pattern is followed 
            // for each member:
            //
            // If new and old values differ
            //    If modification allowed (always allowed if this is a new property).
            //      If value is an object
            //        Add a reference to be resolved later (when we're sure that referenced
            //          object exists).
            //      else
            //        Perform the modification
            //      end if
            //    else
            //      log an error
            //    end if
            //  end if

            // Layer

            FdoAssociationPropertyP newLayerProp = pNetworkNodeFeatureClass->GetLayerProperty();
            FdoStringP oldLayerPropName = m_layer ? m_layer->GetName() : L"";
            FdoStringP newLayerPropName = newLayerProp ? newLayerProp->GetName() : L"";

            if ( oldLayerPropName != newLayerPropName ) {
                if ( (GetElementState() == FdoSchemaElementState_Added) || (pContext->CanModNodeLayer(pNetworkNodeFeatureClass)) ) 
                    pContext->AddNetworkNodeAssocPropRef( 
                        this, 
                        newLayerProp ? pNetworkNodeFeatureClass->GetQualifiedName() + L"." + newLayerProp->GetName() : L""
                    );
                else
                    pContext->AddError( 
                        FdoSchemaExceptionP(
                            FdoSchemaException::Create(
                                FdoException::NLSGetMessage(
                                    FDO_NLSID(SCHEMA_86_MODNODELAYER),
                                    (FdoString*) GetQualifiedName()
                                )
                            )
                        )
                    );
            }
        }
    }
}

void FdoNetworkNodeFeatureClass::CheckReferences( FdoSchemaMergeContext* pContext )
{
    // No need to check references if this element is going away.
    if ( GetElementState() != FdoSchemaElementState_Deleted ) {
        FdoSchemaElement::CheckReferences(pContext);

        // Check if layer property marked for delete.        

        FdoAssociationPropertyP layerProp = GetLayerProperty();

        if ( layerProp && (layerProp->GetElementState() == FdoSchemaElementState_Deleted) ) {
            pContext->AddError( 
                FdoSchemaExceptionP(
                    FdoSchemaException::Create(
                        FdoException::NLSGetMessage(
                            FDO_NLSID(SCHEMA_140_DELNODELAYER),
                            (FdoString*) layerProp->GetQualifiedName(),
                            (FdoString*) GetQualifiedName()
                        )
                    )
                )
            );
        }
    }
}

void FdoNetworkNodeFeatureClass::InitFromXml(FdoString* classTypeName, FdoSchemaXmlContext* pContext, FdoXmlAttributeCollection* attrs)
{
    // Make sure FDO and XML class type agree, class type change not allowed.
    if ( (wcscmp( classTypeName, L"ClassDefinition" ) != 0) && (wcscmp( classTypeName, L"NetworkNodeFeatureClass" ) != 0) )
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

    FDO_SAFE_RELEASE(m_layer);

    FdoNetworkFeatureClass::InitFromXml(pContext, attrs );

}

FdoXmlSaxHandler* FdoNetworkNodeFeatureClass::XmlStartElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname, 
        FdoXmlAttributeCollection* atts
)
{
    FdoSchemaXmlContext* fdoContext = (FdoSchemaXmlContext*) context;

    FdoXmlSaxHandler* pRet = FdoNetworkFeatureClass::XmlStartElement(context, uri, name, qname, atts);
    if ( !pRet )
    {
        if (wcscmp(name, L"AssociationProperty") == 0 )
        {
            m_bStartLayer = true;
            m_layerHandler = FdoAssociationPropertyDefinition::Create();
            m_layerHandler->SetParent(this);
            m_layerHandler->InitFromXml( name, fdoContext, atts );
            pRet = m_layerHandler;
        }
    }

    return(pRet);
}

FdoBoolean FdoNetworkNodeFeatureClass::XmlEndElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname
)
{
    FdoSchemaXmlContext* fdoContext = (FdoSchemaXmlContext*) context;

    FdoNetworkFeatureClass::XmlEndElement(context, uri, name, qname);

    if (m_bStartLayer == true && wcscmp(name, L"AssociationProperty") == 0 )
    {
        fdoContext->GetMergeContext()->AddNetworkNodeAssocPropRef( this, this->GetQualifiedName() + L"." + m_layerHandler->GetName());
        FDO_SAFE_RELEASE(m_layerHandler);
        m_bStartLayer = false;
    }

    return(false);
}


void FdoNetworkNodeFeatureClass::_writeXml( FdoSchemaXmlContext* pContext )
{
    FdoXmlWriterP       writer = pContext->GetXmlWriter();

    writer->WriteStartElement( L"NetworkNodeFeatureClass" );

    if ( m_layer )
    {
        writer->WriteAttribute( 
            L"LayerProperty",
            pContext->EncodeName(m_layer->GetName())
        );
    }

    FdoNetworkFeatureClass::_writeXml(pContext);

    if ( pContext->CheckWriteAssoc(this, m_layer) )
        m_layer->_writeXml(pContext);

    writer->WriteEndElement();
}


