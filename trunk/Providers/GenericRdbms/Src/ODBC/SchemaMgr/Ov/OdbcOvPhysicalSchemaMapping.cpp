/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser
 * General Public License as published by the Free Software Foundation.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "stdafx.h"
#include <Rdbms/Override/ODBC/OdbcOvPhysicalSchemaMapping.h>
//TODO: need to get provider name from provider-specific header.
#include "../../Fdo/FdoRdbmsOdbcProviderInfo.h" // contains the provider name

FdoOdbcOvPhysicalSchemaMapping* FdoOdbcOvPhysicalSchemaMapping::Create()
{
    return new FdoOdbcOvPhysicalSchemaMapping();
}

FdoOdbcOvPhysicalSchemaMapping* FdoOdbcOvPhysicalSchemaMapping::Create(FdoString* name)
{
    return new FdoOdbcOvPhysicalSchemaMapping(name);
}

FdoOdbcOvPhysicalSchemaMapping::FdoOdbcOvPhysicalSchemaMapping()
{
    Init();
}

FdoOdbcOvPhysicalSchemaMapping::FdoOdbcOvPhysicalSchemaMapping(FdoString* name) : FdoRdbmsOvPhysicalSchemaMapping(name)
{
    Init();
}

FdoOdbcOvPhysicalSchemaMapping::~FdoOdbcOvPhysicalSchemaMapping()
{
}

FdoOdbcOvClassCollection* FdoOdbcOvPhysicalSchemaMapping::GetClasses()
{
    return FDO_SAFE_ADDREF( (FdoOdbcOvClassCollection*) mClasses );
}

FdoString* FdoOdbcOvPhysicalSchemaMapping::GetProvider(void)
{
    return RDBMS_ODBC_PROVIDER_NAME;
}

void FdoOdbcOvPhysicalSchemaMapping::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
    FdoRdbmsOvPhysicalSchemaMapping::InitFromXml( pContext, attrs );
}

void FdoOdbcOvPhysicalSchemaMapping::_writeXmlAttributes(FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags)
{
    xmlWriter->WriteAttribute( L"xmlns", L"http://fdoodbc.osgeo.org/schemas" );
}

FdoRdbmsOvClassDefinition* FdoOdbcOvPhysicalSchemaMapping::CreateClass( 
    FdoXmlSaxContext* context,
    FdoXmlAttributeCollection* atts
)
{
    FdoOdbcOvClassDefinition* pClass = FdoOdbcOvClassDefinition::Create();
    pClass->InitFromXml( context, atts );

    return pClass;
}
                
void FdoOdbcOvPhysicalSchemaMapping::AddClass( 
    FdoRdbmsOvClassDefinition* pClass
)
{
    FdoOdbcOvClassesP classes = GetClasses();
    classes->Add( (FdoOdbcOvClassDefinition*) pClass );
}

void FdoOdbcOvPhysicalSchemaMapping::Dispose()
{
    delete this;
}


void FdoOdbcOvPhysicalSchemaMapping::Init()
{
    mClasses = FdoOdbcOvClassCollection::Create(
        FdoRdbmsOvClassesP(
            FdoRdbmsOvPhysicalSchemaMapping::GetRdbmsClasses()
        )
    );
}
