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
#include <Std.h>
#include <Fdo/Xml/SchemaMapping.h>

FdoXmlSchemaMapping* FdoXmlSchemaMapping::Create(
    FdoString* name
)
{
    return new FdoXmlSchemaMapping( name );
}


FdoXmlSchemaMapping::FdoXmlSchemaMapping(
    FdoString* name
)
{
    SetName(name);
}

FdoXmlSchemaMapping::~FdoXmlSchemaMapping()
{
}

FdoString* FdoXmlSchemaMapping::GetProvider()
{
    return FdoXml::mGmlProviderName;
}

void FdoXmlSchemaMapping::SetTargetNamespace( FdoString* url )
{
    mTargetNamespace = url;
}

FdoString* FdoXmlSchemaMapping::GetTargetNamespace()
{
    return mTargetNamespace;
}

FdoXmlElementMappingCollection* FdoXmlSchemaMapping::GetElementMappings()
{
    if ( !mElementMappings )
        mElementMappings = FdoXmlElementMappingCollection::Create(this);

    return FDO_SAFE_ADDREF( (FdoXmlElementMappingCollection*) mElementMappings );
}

FdoXmlClassMappingCollection* FdoXmlSchemaMapping::GetClassMappings()
{
    if ( !mClassMappings )
        mClassMappings = FdoXmlClassMappingCollection::Create(this);

    return FDO_SAFE_ADDREF( (FdoXmlClassMappingCollection*) mClassMappings );
}

void FdoXmlSchemaMapping::Dispose()
{
    delete this;
}

void FdoXmlSchemaMapping::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
    FdoXmlContext* fdoContext = dynamic_cast<FdoXmlContext*>(pContext);

    FdoPhysicalSchemaMapping::InitFromXml( pContext, attrs );

    //TODO deserialize targetNamespace.
}


void FdoXmlSchemaMapping::_writeXml(
    FdoXmlWriter* xmlWriter, 
    const FdoXmlFlags* flags
)
{
    FdoInt32 idx;

    xmlWriter->WriteStartElement( L"SchemaMapping" );
    xmlWriter->WriteAttribute( L"xmlns", L"http://fdo.osgeo.org/schemas/gml/mappings" );


    FdoPhysicalSchemaMapping::_writeXml( xmlWriter, flags );

    if ( mTargetNamespace != L"" ) 
        xmlWriter->WriteAttribute( 
            L"targetNamespace", 
            mTargetNamespace 
        );

    FdoXmlElementMappingsP elemMappings = GetElementMappings();

    for ( idx = 0; idx < elemMappings->GetCount(); idx++ ) {
        FdoXmlElementMappingP( elemMappings->GetItem(idx) )->_writeXml( xmlWriter, flags );
    }

    FdoXmlClassMappingsP classMappings = GetClassMappings();

    for ( idx = 0; idx < classMappings->GetCount(); idx++ ) {
        FdoXmlClassMappingP( classMappings->GetItem(idx) )->_writeXml( xmlWriter, flags );
    }

    xmlWriter->WriteEndElement();
}


