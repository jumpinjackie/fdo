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

#include <Rdbms/Override/PostGis/PostGisOvClassDefinition.h>
#include <Rdbms/Override/PostGis/PostGisOvPropertyMappingClass.h>
#include <Rdbms/Override/PostGis/PostGisOvPropertyMappingConcrete.h>
#include <Rdbms/Override/PostGis/PostGisOvPropertyMappingSingle.h>
#include <Rdbms/Override/PostGis/PostGisOvObjectPropertyDefinition.h>

FdoPostGISOvClassDefinition* FdoPostGISOvClassDefinition::Create()
{
    return (new FdoPostGISOvClassDefinition());
}

FdoPostGISOvClassDefinition* FdoPostGISOvClassDefinition::Create(
    FdoString *name)
{
    return (new FdoPostGISOvClassDefinition(name));
}

FdoPostGISOvClassDefinition::FdoPostGISOvClassDefinition()
{
    Init();
}

FdoPostGISOvClassDefinition::FdoPostGISOvClassDefinition(FdoString *name)
    : FdoRdbmsOvClassDefinition(name)
{
    Init();
}

FdoPostGISOvClassDefinition::~FdoPostGISOvClassDefinition()
{
    // idle
}

FdoPostGISOvPropertyDefinitionCollection *FdoPostGISOvClassDefinition::GetProperties()
{
    typedef FdoPostGISOvPropertyDefinitionCollection collection_t;
    collection_t* col = static_cast<collection_t*>(mPropertyDefinitions);
    FDO_SAFE_ADDREF(col);
    return col;
}

FdoPostGISOvTable *FdoPostGISOvClassDefinition::GetTable()
{
    FdoRdbmsOvTable* rdbmsTable = NULL;
    rdbmsTable = FdoRdbmsOvClassDefinition::GetTable();

    return (static_cast<FdoPostGISOvTable*>(rdbmsTable));
}

void FdoPostGISOvClassDefinition::SetTable(FdoPostGISOvTable *table)
{
    FdoRdbmsOvClassDefinition::SetTable(table);
}

void FdoPostGISOvClassDefinition::SetParent(FdoPhysicalElementMapping* parent)
{
    FdoPhysicalElementMapping::SetParent(parent);
}

void FdoPostGISOvClassDefinition::InitFromXml(
    FdoXmlSaxContext* saxContext,
    FdoXmlAttributeCollection* attrs)
{
    FdoRdbmsOvClassDefinition::InitFromXml(saxContext, attrs);

}

// Serialize the XML attributes of this class:
void FdoPostGISOvClassDefinition::_writeXmlAttributes(
    FdoXmlWriter* xmlWriter,
    const FdoXmlFlags* flags)
{
}

void FdoPostGISOvClassDefinition::Init()
{
    mPropertyDefinitions = FdoPostGISOvPropertyDefinitionCollection::Create(
        FdoRdbmsOvPropertiesP(
            FdoRdbmsOvClassDefinition::GetRdbmsProperties()
        )
    );
}

void FdoPostGISOvClassDefinition::Dispose()
{
    delete this;
}

void FdoPostGISOvClassDefinition::SetTable(
    FdoXmlSaxContext* saxContext,
    FdoXmlAttributeCollection* atts)
{
    FdoPostGISOvTableP table(FdoPostGISOvTable::Create());
    table->InitFromXml(saxContext, atts);
    SetTable(table);
}

FdoRdbmsOvDataPropertyDefinition* FdoPostGISOvClassDefinition::CreateDataProperty( 
    FdoXmlSaxContext* saxContext,
    FdoXmlAttributeCollection* propAtts,
    FdoXmlAttributeCollection* colAtts)
{
     FdoPostGISOvDataPropertyDefinition* propDef = NULL;
     propDef = FdoPostGISOvDataPropertyDefinition::Create();

     propDef->InitFromXml(saxContext, propAtts);

     // Create the column object and add to the data property.
     FdoPostGISOvColumnP column(FdoPostGISOvColumn::Create());
     propDef->SetColumn(column);
     column->InitFromXml(saxContext, colAtts);

     return propDef;
}
                
FdoRdbmsOvGeometricPropertyDefinition* FdoPostGISOvClassDefinition::CreateGeometricProperty( 
    FdoXmlSaxContext* saxContext,
    FdoXmlAttributeCollection* propAtts,
    FdoXmlAttributeCollection* colAtts)
{
     FdoPostGISOvGeometricPropertyDefinition* propDef = NULL;
     propDef = FdoPostGISOvGeometricPropertyDefinition::Create();
     propDef->InitFromXml(saxContext, propAtts);

     // Create the column  object and add to the data property.
     FdoPostGISOvGeometricColumnP column(FdoPostGISOvGeometricColumn::Create());
     propDef->SetColumn(column);
     column->InitFromXml(saxContext, colAtts);

     return propDef;
}
                
FdoRdbmsOvObjectPropertyDefinition* FdoPostGISOvClassDefinition::CreateObjectProperty( 
    FdoXmlSaxContext* saxContext,
    FdoXmlAttributeCollection* propAtts,
    FdoString* mappingType,
    FdoXmlAttributeCollection* mappingAtts)
{
     FdoPostGISOvObjectPropertyDefinition* propDef = NULL;
     propDef = FdoPostGISOvObjectPropertyDefinition::Create();
     propDef->InitFromXml(saxContext, propAtts, mappingType, mappingAtts);

     return propDef;
}
                
void FdoPostGISOvClassDefinition::AddProperty(
    FdoRdbmsOvPropertyDefinition* propDef)
{
    FdoPostGISOvPropertiesP properties(GetProperties());
    
    FdoPostGISOvPropertyDefinition* ovPropDef = NULL;
    ovPropDef = dynamic_cast<FdoPostGISOvPropertyDefinition*>(propDef);
    
    properties->Add(ovPropDef);
}

