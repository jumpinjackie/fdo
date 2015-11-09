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
#include <Fdo/Schema/Class.h>
#include "XmlContext.h"


FdoClass* FdoClass::Create()
{
    return new FdoClass();
}

FdoClass* FdoClass::Create(FdoString* name, FdoString* description)
{
    return new FdoClass(name, description);
}

FdoClass::~FdoClass()
{
}

void FdoClass::Dispose()
{
    delete this;
}

FdoClass::FdoClass() : FdoClassDefinition()
{
}

FdoClass::FdoClass(FdoString* name, FdoString* description) : FdoClassDefinition(name,description)
{
}

FdoClassType FdoClass::GetClassType()
{
    return FdoClassType_Class;
}

void FdoClass::InitFromXml(FdoString* classTypeName, FdoSchemaXmlContext* pContext, FdoXmlAttributeCollection* attrs)
{
    // Make sure FDO and XML class type agree, class type change not allowed.
    if ( (wcscmp( classTypeName, L"ClassDefinition" ) != 0) && (wcscmp( classTypeName, L"Class" ) != 0) ) { 
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

void FdoClass::_writeXml( FdoSchemaXmlContext* pContext  )
{
    FdoXmlWriterP       writer = pContext->GetXmlWriter();

    // Write the non-feature class element.
    writer->WriteStartElement( L"Class" );

    // Write the attributes and elements common to all class definitions
    FdoClassDefinition::_writeXml(pContext);

    // close the element.
    writer->WriteEndElement();
}

