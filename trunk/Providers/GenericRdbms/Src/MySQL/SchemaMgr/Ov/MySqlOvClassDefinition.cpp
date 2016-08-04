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

#include <Rdbms/Override/MySQL/MySqlOvClassDefinition.h>
#include <Rdbms/Override/MySQL/MySqlOvPropertyMappingClass.h>
#include <Rdbms/Override/MySQL/MySqlOvPropertyMappingConcrete.h>
#include <Rdbms/Override/MySQL/MySqlOvPropertyMappingSingle.h>
#include <Rdbms/Override/MySQL/MySqlOvObjectPropertyDefinition.h>

FdoMySQLOvClassDefinition* FdoMySQLOvClassDefinition::Create()
{
    return new FdoMySQLOvClassDefinition();
}

FdoMySQLOvClassDefinition* FdoMySQLOvClassDefinition::Create(FdoString *name)
{
    return new FdoMySQLOvClassDefinition(name);
}

FdoMySQLOvClassDefinition::FdoMySQLOvClassDefinition()
{
    Init();
}

FdoMySQLOvClassDefinition::FdoMySQLOvClassDefinition(FdoString *name) :
    FdoRdbmsOvClassDefinition( name )
{
    Init();
}

FdoMySQLOvClassDefinition::~FdoMySQLOvClassDefinition()
{
}

FdoMySQLOvPropertyDefinitionCollection *FdoMySQLOvClassDefinition::GetProperties()
{
    return FDO_SAFE_ADDREF( (FdoMySQLOvPropertyDefinitionCollection*) mPropertyDefinitions );
}

FdoMySQLOvTable *FdoMySQLOvClassDefinition::GetTable()
{
    return (FdoMySQLOvTable*) FdoRdbmsOvClassDefinition::GetTable();
}

void FdoMySQLOvClassDefinition::SetTable(FdoMySQLOvTable *table)
{
    FdoRdbmsOvClassDefinition::SetTable(table);
}

void FdoMySQLOvClassDefinition::SetAutoIncrementPropertyName(FdoString* autoIncrementPropertyName)
{
    mAutoIncrementPropertyName = autoIncrementPropertyName;
}

FdoString* FdoMySQLOvClassDefinition::GetAutoIncrementPropertyName(void)
{
    return mAutoIncrementPropertyName;
}

FdoInt64 FdoMySQLOvClassDefinition::GetAutoIncrementSeed(void)
{
    return mAutoIncrementSeed;
}

void FdoMySQLOvClassDefinition::SetAutoIncrementSeed(FdoInt64 autoIncrementSeed)
{
    mAutoIncrementSeed = autoIncrementSeed;
}


void FdoMySQLOvClassDefinition::SetParent(FdoPhysicalElementMapping* value)
{
    FdoPhysicalElementMapping::SetParent(value);
}

void FdoMySQLOvClassDefinition::Init()
{
    mPropertyDefinitions = FdoMySQLOvPropertyDefinitionCollection::Create(
        FdoRdbmsOvPropertiesP(
            FdoRdbmsOvClassDefinition::GetRdbmsProperties()
        )
    );

    mAutoIncrementSeed = 1;
}

FdoRdbmsOvDataPropertyDefinition* FdoMySQLOvClassDefinition::CreateDataProperty( 
    FdoXmlSaxContext* context,
    FdoXmlAttributeCollection* propAtts,
    FdoXmlAttributeCollection* colAtts
)
{
     FdoMySQLOvDataPropertyDefinition* pProp = FdoMySQLOvDataPropertyDefinition::Create();
     pProp->InitFromXml( context, propAtts );

     // Create the column  object and add to the data property.
     FdoMySQLOvColumnP pColumn = FdoMySQLOvColumn::Create();
     pProp->SetColumn( pColumn );
     pColumn->InitFromXml( context, colAtts );

     return pProp;
}
                
FdoRdbmsOvGeometricPropertyDefinition* FdoMySQLOvClassDefinition::CreateGeometricProperty( 
    FdoXmlSaxContext* context,
    FdoXmlAttributeCollection* propAtts,
    FdoXmlAttributeCollection* colAtts
)
{
     FdoMySQLOvGeometricPropertyDefinition* pProp = FdoMySQLOvGeometricPropertyDefinition::Create();
     pProp->InitFromXml( context, propAtts );

     // Create the column  object and add to the data property.
     FdoMySQLOvGeometricColumnP pColumn = FdoMySQLOvGeometricColumn::Create();
     pProp->SetColumn( pColumn );
     pColumn->InitFromXml( context, colAtts );

     return pProp;
}
                
FdoRdbmsOvObjectPropertyDefinition* FdoMySQLOvClassDefinition::CreateObjectProperty( 
    FdoXmlSaxContext* context,
    FdoXmlAttributeCollection* propAtts,
    FdoString* mappingType,
    FdoXmlAttributeCollection* mappingAtts
)
{
     FdoMySQLOvObjectPropertyDefinition* pProp = FdoMySQLOvObjectPropertyDefinition::Create();
     pProp->InitFromXml( context, propAtts, mappingType, mappingAtts );

     return pProp;
}
                
void FdoMySQLOvClassDefinition::AddProperty( 
    FdoRdbmsOvPropertyDefinition* pProp
)
{
    FdoMySQLOvPropertiesP properties = GetProperties();
    properties->Add( dynamic_cast<FdoMySQLOvPropertyDefinition*>(pProp) );
}

void FdoMySQLOvClassDefinition::SetTable(
    FdoXmlSaxContext* context,
    FdoXmlAttributeCollection* atts
)
{
    FdoMySQLOvTableP table = FdoMySQLOvTable::Create();
    table->InitFromXml( context, atts );
    SetTable( table );
}

void FdoMySQLOvClassDefinition::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
    FdoRdbmsOvClassDefinition::InitFromXml(pContext, attrs);

    FdoXmlAttributeP att = attrs->FindItem(L"autoIncrementSeed");
    if (att) {
        mAutoIncrementSeed = (FdoInt32)FdoStringP(att->GetValue()).ToLong();
    }

    // TODO: call FdoXmlContext::DecodeName() here?
    att = attrs->FindItem(L"autoIncrementPropertyName");
    if (att) {
        mAutoIncrementPropertyName = att->GetValue();
    }
}

// Serialize the XML attributes of this class:
void FdoMySQLOvClassDefinition::_writeXmlAttributes(FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags)
{
    FdoStringP sAutoIncrementSeed = FdoStringP::Format(L"%d", mAutoIncrementSeed);
    xmlWriter->WriteAttribute(L"autoIncrementSeed", sAutoIncrementSeed);

    // TODO: call FdoXmlContext::EncodeName() here?
    xmlWriter->WriteAttribute(L"autoIncrementPropertyName", mAutoIncrementPropertyName);
}


void FdoMySQLOvClassDefinition::Dispose()
{
    delete this;
}
