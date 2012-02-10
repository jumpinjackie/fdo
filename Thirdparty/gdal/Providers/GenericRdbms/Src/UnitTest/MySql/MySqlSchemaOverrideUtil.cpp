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

#include "Pch.h"
#include "MySqlSchemaOverrideUtil.h"

FdoRdbmsOvClassDefinition* MySqlSchemaOverrideUtil::CreateOvClassDefinition(FdoString *name)
{
    if (name == NULL)
        return FdoMySQLOvClassDefinition::Create();
    else
        return FdoMySQLOvClassDefinition::Create(name);
}

void MySqlSchemaOverrideUtil::ClassesOvAdd(FdoRdbmsOvPhysicalSchemaMapping* pSchema, FdoRdbmsOvClassDefinition* pClass)
{
    FdoMySQLOvClassesP(((FdoMySQLOvPhysicalSchemaMapping*)pSchema)->GetClasses())->Add((FdoMySQLOvClassDefinition*)pClass);
}

FdoRdbmsOvTable* MySqlSchemaOverrideUtil::CreateOvTable(FdoString* name)
{
    FdoPtr<FdoMySQLOvTable> ovTable = FdoMySQLOvTable::Create(name);

    return FDO_SAFE_ADDREF(ovTable.p);
}

FdoRdbmsOvColumn* MySqlSchemaOverrideUtil::CreateOvColumn(FdoString* name)
{
    return FdoMySQLOvColumn::Create(name);
}

FdoRdbmsOvGeometricColumn* MySqlSchemaOverrideUtil::CreateOvGeometricColumn(FdoString* name)
{
    return FdoMySQLOvGeometricColumn::Create(name);
}

FdoRdbmsOvDataPropertyDefinition* MySqlSchemaOverrideUtil::CreateOvDataPropertyDefinition(FdoString *name)
{
    return FdoMySQLOvDataPropertyDefinition::Create(name);
}

FdoRdbmsOvGeometricPropertyDefinition* MySqlSchemaOverrideUtil::CreateOvGeometricPropertyDefinition(FdoString *name)
{
    return FdoMySQLOvGeometricPropertyDefinition::Create(name);
}

void MySqlSchemaOverrideUtil::PropertiesOvAdd(FdoRdbmsOvClassDefinition* pClass, FdoRdbmsOvPropertyDefinition *pProp)
{
    FdoMySQLOvPropertiesP(((FdoMySQLOvClassDefinition*)pClass)->GetProperties())->Add(dynamic_cast<FdoMySQLOvPropertyDefinition*>(pProp));
}

void MySqlSchemaOverrideUtil::ClassOvSetTable(FdoRdbmsOvClassDefinition* pClass, FdoRdbmsOvTable* pTable)
{
    ((FdoMySQLOvClassDefinition*)pClass)->SetTable((FdoMySQLOvTable*)pTable);
}

void MySqlSchemaOverrideUtil::DataPropOvSetColumn(FdoRdbmsOvDataPropertyDefinition* pDataProp, FdoRdbmsOvColumn* pDataColumn)
{
    ((FdoMySQLOvDataPropertyDefinition*)pDataProp)->SetColumn((FdoMySQLOvColumn*)pDataColumn);
}

void MySqlSchemaOverrideUtil::GeometricPropOvSetColumn(FdoRdbmsOvGeometricPropertyDefinition* pGeomProp, FdoRdbmsOvGeometricColumn* pGeometricColumn)
{
    ((FdoMySQLOvGeometricPropertyDefinition*)pGeomProp)->SetColumn((FdoMySQLOvGeometricColumn*)pGeometricColumn);
}

FdoRdbmsOvObjectPropertyDefinition* MySqlSchemaOverrideUtil::CreateOvObjectPropertyDefinition(FdoString *name)
{
    return FdoMySQLOvObjectPropertyDefinition::Create(name);
}

FdoRdbmsOvPropertyMappingSingle* MySqlSchemaOverrideUtil::CreateOvPropertyMappingSingle()
{
    return FdoMySQLOvPropertyMappingSingle::Create();
}

FdoRdbmsOvPropertyMappingConcrete* MySqlSchemaOverrideUtil::CreateOvPropertyMappingConcrete()
{
    return FdoMySQLOvPropertyMappingConcrete::Create();
}

void MySqlSchemaOverrideUtil::ObjectPropertyOvSetMappingDefinition(FdoRdbmsOvObjectPropertyDefinition* pObProp, FdoRdbmsOvPropertyMappingDefinition* mapping)
{
    ((FdoMySQLOvObjectPropertyDefinition*)pObProp)->SetMappingDefinition(dynamic_cast<FdoMySQLOvPropertyMappingDefinition*>(mapping));
}

void MySqlSchemaOverrideUtil::PropertyMappingOvSetInternalClass(FdoRdbmsOvPropertyMappingRelation* pMapping, FdoRdbmsOvClassDefinition* pClass)
{
    (dynamic_cast<FdoMySQLOvPropertyMappingRelation*>(pMapping))->SetInternalClass((FdoMySQLOvClassDefinition*)pClass);
}

void MySqlSchemaOverrideUtil::SchemaOvSetOwner(FdoRdbmsOvPhysicalSchemaMapping *mapping, FdoString* owner)
{
    ((FdoMySQLOvPhysicalSchemaMapping*)mapping)->SetDatabase( owner );
}

