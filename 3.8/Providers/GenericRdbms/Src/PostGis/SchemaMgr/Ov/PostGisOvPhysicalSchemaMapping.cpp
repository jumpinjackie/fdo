//
// Copyright (C) 2006 Refractions Research, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
#include "stdafx.h"
#include <Rdbms/Override/PostGis/PostGisOvPhysicalSchemaMapping.h>
//TODO: need to get provider name from provider-specific header.
#include "../../Fdo/FdoRdbmsPostGisProviderInfo.h" // contains the provider name

FdoPostGISOvPhysicalSchemaMapping* FdoPostGISOvPhysicalSchemaMapping::Create()
{
    return (new FdoPostGISOvPhysicalSchemaMapping());
}

FdoPostGISOvPhysicalSchemaMapping* FdoPostGISOvPhysicalSchemaMapping::Create(
    FdoString* name)
{
    return (new FdoPostGISOvPhysicalSchemaMapping(name));
}

FdoPostGISOvPhysicalSchemaMapping::FdoPostGISOvPhysicalSchemaMapping()
{
    Init();
}

FdoPostGISOvPhysicalSchemaMapping::FdoPostGISOvPhysicalSchemaMapping(
    FdoString* name) : FdoRdbmsOvPhysicalSchemaMapping(name)
{
    Init();
}

FdoPostGISOvPhysicalSchemaMapping::~FdoPostGISOvPhysicalSchemaMapping()
{
    // idle
}

FdoPostGISOvClassCollection* FdoPostGISOvPhysicalSchemaMapping::GetClasses()
{
    FdoPostGISOvClassCollection* collection = NULL;
    collection = static_cast<FdoPostGISOvClassCollection*>(mClasses);

    FDO_SAFE_ADDREF(collection);
    return collection;
}

FdoString *FdoPostGISOvPhysicalSchemaMapping::GetOwner()
{
    return mOwner;
}

void FdoPostGISOvPhysicalSchemaMapping::SetOwner(FdoString* owner)
{
    mOwner = owner;
}

FdoString* FdoPostGISOvPhysicalSchemaMapping::GetProvider()
{
    return RDBMS_POSTGIS_PROVIDER_NAME;
}

void FdoPostGISOvPhysicalSchemaMapping::InitFromXml(
    FdoXmlSaxContext* saxContext,
    FdoXmlAttributeCollection* attrs)
{
    FdoRdbmsOvPhysicalSchemaMapping::InitFromXml(saxContext, attrs);

    FdoXmlAttributeP xmlAttr = attrs->FindItem(L"owner");
    if (xmlAttr)
    {
        mOwner = xmlAttr->GetValue();
    }

}

void FdoPostGISOvPhysicalSchemaMapping::_writeXmlAttributes(
    FdoXmlWriter* xmlWriter,
    const FdoXmlFlags* flags)
{
    xmlWriter->WriteAttribute(
        L"xmlns",
        L"http://fdopostgresql.osgeo.org/schemas");

    if (mOwner.GetLength() > 0)
        xmlWriter->WriteAttribute(L"owner", mOwner);
}

FdoRdbmsOvClassDefinition* FdoPostGISOvPhysicalSchemaMapping::CreateClass( 
    FdoXmlSaxContext* saxContext,
    FdoXmlAttributeCollection* atts)
{
    FdoPostGISOvClassDefinition* classDef = NULL;
    classDef = FdoPostGISOvClassDefinition::Create();

    classDef->InitFromXml(saxContext, atts);

    return classDef;
}
                
void FdoPostGISOvPhysicalSchemaMapping::AddClass(
    FdoRdbmsOvClassDefinition* classDef)
{
    FdoPostGISOvClassDefinition* postgisClassDef = NULL;
    postgisClassDef = static_cast<FdoPostGISOvClassDefinition*>(classDef);

    FdoPtr<FdoPostGISOvClassCollection> classes(GetClasses());
    classes->Add(postgisClassDef);
}

void FdoPostGISOvPhysicalSchemaMapping::Dispose()
{
    delete this;
}

void FdoPostGISOvPhysicalSchemaMapping::Init()
{
    mClasses = FdoPostGISOvClassCollection::Create(
        FdoRdbmsOvClassesP(
            FdoRdbmsOvPhysicalSchemaMapping::GetRdbmsClasses()));
}
