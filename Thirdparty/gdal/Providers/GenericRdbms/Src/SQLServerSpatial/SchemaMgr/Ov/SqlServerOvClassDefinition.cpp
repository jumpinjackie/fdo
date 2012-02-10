/*
 * Copyright (C) 2004-2008  Autodesk, Inc.
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

#include <Rdbms/Override/SQLServerSpatial/SqlServerOvClassDefinition.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvPropertyMappingClass.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvPropertyMappingConcrete.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvPropertyMappingSingle.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvObjectPropertyDefinition.h>

FdoSqlServerOvClassDefinition* FdoSqlServerOvClassDefinition::Create()
{
    return new FdoSqlServerOvClassDefinition();
}

FdoSqlServerOvClassDefinition* FdoSqlServerOvClassDefinition::Create(FdoString *name)
{
    return new FdoSqlServerOvClassDefinition(name);
}

FdoSqlServerOvClassDefinition::FdoSqlServerOvClassDefinition()
{
    Init();
}

FdoSqlServerOvClassDefinition::FdoSqlServerOvClassDefinition(FdoString *name) :
    FdoRdbmsOvClassDefinition( name )
{
    Init();
}

FdoSqlServerOvClassDefinition::~FdoSqlServerOvClassDefinition()
{
}

FdoSqlServerOvPropertyDefinitionCollection *FdoSqlServerOvClassDefinition::GetProperties()
{
    return FDO_SAFE_ADDREF( (FdoSqlServerOvPropertyDefinitionCollection*) mPropertyDefinitions );
}

FdoSqlServerOvTable *FdoSqlServerOvClassDefinition::GetTable()
{
    return (FdoSqlServerOvTable*) FdoRdbmsOvClassDefinition::GetTable();
}

void FdoSqlServerOvClassDefinition::SetTable(FdoSqlServerOvTable *table)
{
    FdoRdbmsOvClassDefinition::SetTable(table);
}

void FdoSqlServerOvClassDefinition::SetIdentityPropertyName(FdoString* identityPropertyName)
{
    mIdentityPropertyName = identityPropertyName;
}

FdoString* FdoSqlServerOvClassDefinition::GetIdentityPropertyName(void)
{
    return mIdentityPropertyName;
}

bool FdoSqlServerOvClassDefinition::GetIdentityIsGloballyUnique(void)
{
    return mIdentityIsGloballyUnique;
}

void FdoSqlServerOvClassDefinition::SetIdentityIsGloballyUnique(bool identityIsGloballyUnique)
{
    mIdentityIsGloballyUnique = identityIsGloballyUnique;
}

FdoInt32 FdoSqlServerOvClassDefinition::GetIdentitySeed(void)
{
    return mIdentitySeed;
}

void FdoSqlServerOvClassDefinition::SetIdentitySeed(FdoInt32 identitySeed)
{
    mIdentitySeed = identitySeed;
}

FdoInt32 FdoSqlServerOvClassDefinition::GetIdentityIncrement(void)
{
    return mIdentityIncrement;
}

void FdoSqlServerOvClassDefinition::SetIdentityIncrement(FdoInt32 identityIncrement)
{
    mIdentityIncrement = identityIncrement;
}


void FdoSqlServerOvClassDefinition::SetParent(FdoPhysicalElementMapping* value)
{
    FdoPhysicalElementMapping::SetParent(value);
}

void FdoSqlServerOvClassDefinition::Init()
{
    mPropertyDefinitions = FdoSqlServerOvPropertyDefinitionCollection::Create(
        FdoRdbmsOvPropertiesP(
            FdoRdbmsOvClassDefinition::GetRdbmsProperties()
        )
    );

    mIdentityIsGloballyUnique = false;
    mIdentitySeed = 0;
    mIdentityIncrement = 1;
}

FdoRdbmsOvDataPropertyDefinition* FdoSqlServerOvClassDefinition::CreateDataProperty( 
    FdoXmlSaxContext* context,
    FdoXmlAttributeCollection* propAtts,
    FdoXmlAttributeCollection* colAtts
)
{
     FdoSqlServerOvDataPropertyDefinition* pProp = FdoSqlServerOvDataPropertyDefinition::Create();
     pProp->InitFromXml( context, propAtts );

     // Create the column  object and add to the data property.
     FdoSqlServerOvColumnP pColumn = FdoSqlServerOvColumn::Create();
     pProp->SetColumn( pColumn );
     pColumn->InitFromXml( context, colAtts );

     return pProp;
}
                
FdoRdbmsOvGeometricPropertyDefinition* FdoSqlServerOvClassDefinition::CreateGeometricProperty( 
    FdoXmlSaxContext* context,
    FdoXmlAttributeCollection* propAtts,
    FdoXmlAttributeCollection* colAtts
)
{
     FdoSqlServerOvGeometricPropertyDefinition* pProp = FdoSqlServerOvGeometricPropertyDefinition::Create();
     pProp->InitFromXml( context, propAtts );

     // Create the column  object and add to the data property.
     FdoSqlServerOvGeometricColumnP pColumn = FdoSqlServerOvGeometricColumn::Create();
     pProp->SetColumn( pColumn );
     pColumn->InitFromXml( context, colAtts );

     return pProp;
}
                
FdoRdbmsOvObjectPropertyDefinition* FdoSqlServerOvClassDefinition::CreateObjectProperty( 
    FdoXmlSaxContext* context,
    FdoXmlAttributeCollection* propAtts,
    FdoString* mappingType,
    FdoXmlAttributeCollection* mappingAtts
)
{
     FdoSqlServerOvObjectPropertyDefinition* pProp = FdoSqlServerOvObjectPropertyDefinition::Create();
     pProp->InitFromXml( context, propAtts, mappingType, mappingAtts );

     return pProp;
}
                
void FdoSqlServerOvClassDefinition::AddProperty( 
    FdoRdbmsOvPropertyDefinition* pProp
)
{
    FdoSqlServerOvPropertiesP(GetProperties())->Add( dynamic_cast<FdoSqlServerOvPropertyDefinition*>(pProp) );
}

void FdoSqlServerOvClassDefinition::SetTable(
    FdoXmlSaxContext* context,
    FdoXmlAttributeCollection* atts
)
{
    FdoSqlServerOvTableP table = FdoSqlServerOvTable::Create();
    table->InitFromXml( context, atts );
    SetTable( table );
}

void FdoSqlServerOvClassDefinition::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
    FdoRdbmsOvClassDefinition::InitFromXml(pContext, attrs);

    FdoXmlAttributeP att = attrs->FindItem(L"identityIsGloballyUnique");
    if (att) {
        mIdentityIsGloballyUnique = FdoStringP(att->GetValue()).ToBoolean();
    }

    att = attrs->FindItem(L"identitySeed");
    if (att) {
        mIdentitySeed = (FdoInt32)FdoStringP(att->GetValue()).ToLong();
    }

    att = attrs->FindItem(L"identityIncrement");
    if (att) {
        mIdentityIncrement = (FdoInt32)FdoStringP(att->GetValue()).ToLong();
    }

    // TODO: call FdoXmlContext::DecodeName() here?
    att = attrs->FindItem(L"identityPropertyName");
    if (att) {
        mIdentityPropertyName = att->GetValue();
    }
}

// Serialize the XML attributes of this class:
void FdoSqlServerOvClassDefinition::_writeXmlAttributes(FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags)
{
    xmlWriter->WriteAttribute(L"identityIsGloballyUnique", mIdentityIsGloballyUnique ? L"true" : L"false");

    FdoStringP sIdentitySeed = FdoStringP::Format(L"%d", mIdentitySeed);
    xmlWriter->WriteAttribute(L"identitySeed", sIdentitySeed);

    FdoStringP sIdentityIncrement = FdoStringP::Format(L"%d", mIdentityIncrement);
    xmlWriter->WriteAttribute(L"identityIncrement", sIdentityIncrement);

    // TODO: call FdoXmlContext::EncodeName() here?
    xmlWriter->WriteAttribute(L"identityPropertyName", mIdentityPropertyName);
}


void FdoSqlServerOvClassDefinition::Dispose()
{
    delete this;
}
