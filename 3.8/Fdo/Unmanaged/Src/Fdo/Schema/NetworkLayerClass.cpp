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
#include <Fdo/Schema/NetworkLayerClass.h>

FdoNetworkLayerClass* FdoNetworkLayerClass::Create()
{
    return new FdoNetworkLayerClass();
}

FdoNetworkLayerClass* FdoNetworkLayerClass::Create(FdoString* name, FdoString* description)
{
    return new FdoNetworkLayerClass( name, description );
}

FdoNetworkLayerClass::FdoNetworkLayerClass()
{

}

FdoNetworkLayerClass::FdoNetworkLayerClass(FdoString* name, FdoString* description):
FdoClassDefinition( name, description)
{

}

FdoNetworkLayerClass::~FdoNetworkLayerClass()
{

}

void FdoNetworkLayerClass::Dispose()
{
    delete this;
}


FdoClassType FdoNetworkLayerClass::GetClassType()
{
    return FdoClassType_NetworkLayerClass;
}

void FdoNetworkLayerClass::_StartChanges()
{
    if (!(m_changeInfoState & (CHANGEINFO_PRESENT|CHANGEINFO_PROCESSING)))
    {
        FdoClassDefinition::_StartChanges();

    }
}

void FdoNetworkLayerClass::_BeginChangeProcessing()
{
   if (m_changeInfoState & CHANGEINFO_PROCESSING)
        return; // already marked for processing
    FdoClassDefinition::_BeginChangeProcessing();

    
}

void FdoNetworkLayerClass::_AcceptChanges()
{
    if (m_changeInfoState & CHANGEINFO_PROCESSED)
        return; // already processed
    
    FdoClassDefinition::_AcceptChanges();

    if (m_changeInfoState & CHANGEINFO_PRESENT)
    {
        
    }
}

void FdoNetworkLayerClass::_RejectChanges()
{
    if (m_changeInfoState & CHANGEINFO_PROCESSED)
        return; // already processed
    FdoClassDefinition::_RejectChanges();

    if (m_changeInfoState & CHANGEINFO_PRESENT)
    {
        
    }
}

void FdoNetworkLayerClass::_EndChangeProcessing()
{
    if (!(m_changeInfoState & CHANGEINFO_PROCESSING))
        return; // processing flag already cleared

    FdoClassDefinition::_EndChangeProcessing();
}

void FdoNetworkLayerClass::Set( FdoClassDefinition* pClass, FdoSchemaMergeContext* pContext )
{
    FdoClassDefinition::Set( pClass, pContext );
}

void FdoNetworkLayerClass::InitFromXml(FdoString* classTypeName, FdoSchemaXmlContext* pContext, FdoXmlAttributeCollection* attrs)
{
    // Make sure FDO and XML class type agree, class type change not allowed.
    if ( (wcscmp( classTypeName, L"ClassDefinition" ) != 0) && (wcscmp( classTypeName, L"NetworkLayerClass" ) != 0) ) {
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

    FdoClassDefinition::InitFromXml(pContext, attrs );

}

void FdoNetworkLayerClass::_writeXml( FdoSchemaXmlContext* pContext )
{
    FdoXmlWriterP       writer = pContext->GetXmlWriter();

    // Write element for feature class.
    writer->WriteStartElement( L"NetworkLayerClass" );

    // Write ClassDefinition parts
    FdoClassDefinition::_writeXml(pContext);

    // Write base properties used in complexType restriction
    writeXmlBaseProperties(pContext);

    // close the element.
    writer->WriteEndElement();
}


