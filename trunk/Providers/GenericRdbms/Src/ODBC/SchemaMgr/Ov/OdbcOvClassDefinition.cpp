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

#include <Rdbms/Override/ODBC/OdbcOvClassDefinition.h>

FdoOdbcOvClassDefinition* FdoOdbcOvClassDefinition::Create()
{
    return new FdoOdbcOvClassDefinition();
}

FdoOdbcOvClassDefinition* FdoOdbcOvClassDefinition::Create(FdoString *name)
{
    return new FdoOdbcOvClassDefinition(name);
}

FdoOdbcOvClassDefinition::FdoOdbcOvClassDefinition()
{
    Init();
}

FdoOdbcOvClassDefinition::FdoOdbcOvClassDefinition(FdoString *name) :
    FdoRdbmsOvClassDefinition( name )
{
    Init();
}

FdoOdbcOvClassDefinition::~FdoOdbcOvClassDefinition()
{
}

FdoOdbcOvPropertyDefinitionCollection *FdoOdbcOvClassDefinition::GetProperties()
{
    return FDO_SAFE_ADDREF( (FdoOdbcOvPropertyDefinitionCollection*) mPropertyDefinitions );
}

FdoOdbcOvTable *FdoOdbcOvClassDefinition::GetTable()
{
    return (FdoOdbcOvTable*) FdoRdbmsOvClassDefinition::GetTable();
}

void FdoOdbcOvClassDefinition::SetTable(FdoOdbcOvTable *table)
{
    FdoRdbmsOvClassDefinition::SetTable(table);
}

void FdoOdbcOvClassDefinition::SetParent(FdoPhysicalElementMapping* value)
{
    FdoPhysicalElementMapping::SetParent(value);
}

void FdoOdbcOvClassDefinition::Init()
{
    mPropertyDefinitions = FdoOdbcOvPropertyDefinitionCollection::Create(
        FdoRdbmsOvPropertiesP(
            FdoRdbmsOvClassDefinition::GetRdbmsProperties()
        )
    );
}

FdoRdbmsOvDataPropertyDefinition* FdoOdbcOvClassDefinition::CreateDataProperty( 
    FdoXmlSaxContext* context,
    FdoXmlAttributeCollection* propAtts,
    FdoXmlAttributeCollection* colAtts
)
{
     FdoOdbcOvDataPropertyDefinition* pProp = FdoOdbcOvDataPropertyDefinition::Create();
     pProp->InitFromXml( context, propAtts );

     // Create the column  object and add to the data property.
     FdoOdbcOvColumnP pColumn = FdoOdbcOvColumn::Create();
     pProp->SetColumn( pColumn );
     pColumn->InitFromXml( context, colAtts );

     return pProp;
}
                
FdoRdbmsOvGeometricPropertyDefinition* FdoOdbcOvClassDefinition::CreateGeometricProperty( 
    FdoXmlSaxContext* context,
    FdoXmlAttributeCollection* propAtts,
    FdoXmlAttributeCollection* colAtts
)
{
     FdoOdbcOvGeometricPropertyDefinition* pProp = FdoOdbcOvGeometricPropertyDefinition::Create();
     pProp->InitFromXml( context, propAtts );

     // There are no attributes or sub-elements to process specifically for ODBC.

     return pProp;
}
                
void FdoOdbcOvClassDefinition::AddProperty( 
    FdoRdbmsOvPropertyDefinition* pProp
)
{
    FdoOdbcOvPropertiesP properties = GetProperties();
    properties->Add( dynamic_cast<FdoOdbcOvPropertyDefinition*>(pProp) );
}

void FdoOdbcOvClassDefinition::SetTable(
    FdoXmlSaxContext* context,
    FdoXmlAttributeCollection* atts
)
{
    FdoOdbcOvTableP table = FdoOdbcOvTable::Create();
    table->InitFromXml( context, atts );
    SetTable( table );
}

void FdoOdbcOvClassDefinition::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
    FdoRdbmsOvClassDefinition::InitFromXml(pContext, attrs);
}

// Serialize the XML attributes of this class:
void FdoOdbcOvClassDefinition::_writeXmlAttributes(FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags)
{
}


void FdoOdbcOvClassDefinition::Dispose()
{
    delete this;
}
