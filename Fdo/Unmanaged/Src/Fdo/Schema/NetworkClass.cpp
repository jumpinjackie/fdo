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
#include <Fdo/Schema/NetworkClass.h>

FdoNetworkClass* FdoNetworkClass::Create()
{
    return new FdoNetworkClass();
}

FdoNetworkClass* FdoNetworkClass::Create(FdoString* name, FdoString* description)
{
    return new FdoNetworkClass(name, description);
}

FdoNetworkClass::FdoNetworkClass():
m_layerClass(NULL)
{

}

FdoNetworkClass::FdoNetworkClass(FdoString* name, FdoString* description):
FdoClassDefinition( name, description ),
m_layerClass(NULL)
{

}

FdoNetworkClass::~FdoNetworkClass()
{
    FDO_SAFE_RELEASE(m_layerClass);
    FDO_SAFE_RELEASE(m_layerClassCHANGED);
}

void FdoNetworkClass::Dispose()
{
    delete this;
}

FdoClassType FdoNetworkClass::GetClassType()
{
    return FdoClassType_NetworkClass;
}

FdoNetworkLayerClass* FdoNetworkClass::GetLayerClass()
{
    return FDO_SAFE_ADDREF(m_layerClass);
}

void FdoNetworkClass::SetLayerClass(FdoNetworkLayerClass* value)
{
    _StartChanges();
    FDO_SAFE_RELEASE(m_layerClass);
    m_layerClass = FDO_SAFE_ADDREF(value);
    SetElementState(FdoSchemaElementState_Modified);
}


void FdoNetworkClass::_StartChanges()
{
    if (!(m_changeInfoState & (CHANGEINFO_PRESENT|CHANGEINFO_PROCESSING)))
    {
        FdoClassDefinition::_StartChanges();

        m_layerClassCHANGED = FDO_SAFE_ADDREF(m_layerClass);
    }
}

void FdoNetworkClass::_BeginChangeProcessing()
{
    if (m_changeInfoState & CHANGEINFO_PROCESSING)
        return; // already marked for processing

    FdoClassDefinition::_BeginChangeProcessing();

    m_layerClassCHANGED = FDO_SAFE_ADDREF(m_layerClass);
}

void FdoNetworkClass::_AcceptChanges()
{
    if (m_changeInfoState & CHANGEINFO_PROCESSED)
        return; // already processed
    
    FdoClassDefinition::_AcceptChanges();

    if (m_changeInfoState & CHANGEINFO_PRESENT)
    {
        // reset
        FDO_SAFE_RELEASE(m_layerClassCHANGED);
    }
}

void FdoNetworkClass::_RejectChanges()
{
    if (m_changeInfoState & CHANGEINFO_PROCESSED)
        return; // already processed
    FdoClassDefinition::_RejectChanges();

    if (m_changeInfoState & CHANGEINFO_PRESENT)
    {
        // restore
        if (m_layerClass != m_layerClassCHANGED)
        {
            FDO_SAFE_RELEASE(m_layerClass);
            m_layerClass = FDO_SAFE_ADDREF(m_layerClassCHANGED);
        }
        
        // reset
        FDO_SAFE_RELEASE(m_layerClassCHANGED);
    }
}

void FdoNetworkClass::_EndChangeProcessing()
{
    if (!(m_changeInfoState & CHANGEINFO_PROCESSING))
        return; // processing flag already cleared

    FdoClassDefinition::_EndChangeProcessing();
}

void FdoNetworkClass::Set( FdoClassDefinition* pClass, FdoSchemaMergeContext* pContext )
{
    FdoClassDefinition::Set( pClass, pContext );

    // Base function catches class type mismatch so silently quit on type mismatch
    if ( GetClassType() == pClass->GetClassType() ) {
        if ( pContext->GetIgnoreStates() || (GetElementState() == FdoSchemaElementState_Added) || (pClass->GetElementState() == FdoSchemaElementState_Modified) ) {
            FdoNetworkLayerClassP newLayerClass = ((FdoNetworkClass*) pClass)->GetLayerClass();

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

            // Network Layer Class

            FdoSchemaElementP newLayerParent = newLayerClass ? newLayerClass->GetParent() : (FdoSchemaElement*) NULL;
            if ( newLayerClass && !newLayerParent ) {
                // Error: network layer class is orphan.
                pContext->AddError( 
                    FdoSchemaExceptionP(
                        FdoSchemaException::Create(
                            FdoException::NLSGetMessage(
                                FDO_NLSID(SCHEMA_51_CLASSNOSCHEMA),
                                (FdoString*) GetQualifiedName(),
                                newLayerClass->GetName()
                            )
                        )
                    )
                );
            }
            else {
                FdoStringP oldLayerClassName = m_layerClass ? m_layerClass->GetQualifiedName() : FdoStringP();
                FdoStringP newLayerClassName = newLayerClass ? newLayerClass->GetQualifiedName() : FdoStringP();
                if ( oldLayerClassName != newLayerClassName ) { 
                    if ( (GetElementState() == FdoSchemaElementState_Added) || (pContext->CanModNetLayer((FdoNetworkClass*)pClass)) ) {
                        pContext->AddNetworkClassRef( 
                            this, 
                            newLayerParent ? newLayerParent->GetName() : L"",
                            newLayerClass ? newLayerClass->GetName() : L""
                        );
                    }
                    else {
                        pContext->AddError( 
                            FdoSchemaExceptionP(
                                FdoSchemaException::Create(
                                    FdoException::NLSGetMessage(
                                        FDO_NLSID(SCHEMA_79_MODNETLAYER),
                                        (FdoString*) GetQualifiedName()
                                    )
                                )
                            )
                        );
                    }
                }
            }
        }
    }
}

void FdoNetworkClass::CheckReferences( FdoSchemaMergeContext* pContext )
{
    // No need to check references if this element is going away.
    if ( GetElementState() != FdoSchemaElementState_Deleted ) {
        FdoSchemaElement::CheckReferences(pContext);

        // Check if network layer class marked for delete.        

        FdoNetworkLayerClassP layerClass = GetLayerClass();

        if ( layerClass && (layerClass->GetElementState() == FdoSchemaElementState_Deleted) ) {
            pContext->AddError( 
                FdoSchemaExceptionP(
                    FdoSchemaException::Create(
                        FdoException::NLSGetMessage(
                            FDO_NLSID(SCHEMA_133_DELLAYERCLASS),
                            (FdoString*) layerClass->GetQualifiedName(),
                            (FdoString*) GetQualifiedName()
                        )
                    )
                )
            );
        }
    }
}

void FdoNetworkClass::InitFromXml(const FdoString* classTypeName, FdoSchemaXmlContext* pContext, FdoXmlAttributeCollection* attrs)
{
    // Make sure FDO and XML class type agree, class type change not allowed.
    if ( (wcscmp( classTypeName, L"ClassDefinition" ) != 0) && (wcscmp( classTypeName, L"NetworkClass" ) != 0) )
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

    FDO_SAFE_RELEASE(m_layerClass);

    FdoXmlAttributeP schema = attrs->FindItem( L"classSchema" );
    FdoXmlAttributeP cls = attrs->FindItem( L"class" );
    if (schema != NULL && cls != NULL)
        pContext->GetMergeContext()->AddNetworkClassRef( this, pContext->DecodeName(schema->GetValue()), pContext->DecodeName(cls->GetValue()) );

    FdoClassDefinition::InitFromXml(pContext, attrs );

}

void FdoNetworkClass::_writeXml( FdoSchemaXmlContext* pContext )
{
    FdoXmlWriterP       writer = pContext->GetXmlWriter();

    // Write element for feature class.
    writer->WriteStartElement( L"NetworkClass" );

    // Write layerClass as attribute
    if ( m_layerClass )
    { 
        writer->WriteAttribute( 
            L"classSchema", 
            pContext->EncodeName( pContext->RefClass2SchemaName(this, m_layerClass) )
        );
        writer->WriteAttribute( 
            L"class",
            pContext->EncodeName(m_layerClass->GetName())
        );
    }

    // Write ClassDefinition parts
    FdoClassDefinition::_writeXml(pContext);

    // Write base properties used in complexType restriction
    writeXmlBaseProperties(pContext);

    // close the element.
    writer->WriteEndElement();
}

