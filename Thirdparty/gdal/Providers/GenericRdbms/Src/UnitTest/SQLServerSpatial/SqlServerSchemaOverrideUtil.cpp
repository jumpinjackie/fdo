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
#include "SqlServerSchemaOverrideUtil.h"

FdoRdbmsOvClassDefinition* SqlServerSchemaOverrideUtil::CreateOvClassDefinition(FdoString *name)
{
    if (name == NULL)
        return FdoSqlServerOvClassDefinition::Create();
    else
        return FdoSqlServerOvClassDefinition::Create(name);
}

void SqlServerSchemaOverrideUtil::ClassesOvAdd(FdoRdbmsOvPhysicalSchemaMapping* pSchema, FdoRdbmsOvClassDefinition* pClass)
{
    FdoSqlServerOvClassesP(((FdoSqlServerOvPhysicalSchemaMapping*)pSchema)->GetClasses())->Add((FdoSqlServerOvClassDefinition*)pClass);
}

FdoRdbmsOvTable* SqlServerSchemaOverrideUtil::CreateOvTable(FdoString* name)
{
    FdoPtr<FdoSqlServerOvTable> ovTable = FdoSqlServerOvTable::Create(name);

    return FDO_SAFE_ADDREF(ovTable.p);
}

FdoRdbmsOvColumn* SqlServerSchemaOverrideUtil::CreateOvColumn(FdoString* name)
{
    return FdoSqlServerOvColumn::Create(name);
}

FdoRdbmsOvGeometricColumn* SqlServerSchemaOverrideUtil::CreateOvGeometricColumn(FdoString* name)
{
    return FdoSqlServerOvGeometricColumn::Create(name);
}

FdoRdbmsOvDataPropertyDefinition* SqlServerSchemaOverrideUtil::CreateOvDataPropertyDefinition(FdoString *name)
{
    return FdoSqlServerOvDataPropertyDefinition::Create(name);
}

FdoRdbmsOvGeometricPropertyDefinition* SqlServerSchemaOverrideUtil::CreateOvGeometricPropertyDefinition(FdoString *name)
{
    return FdoSqlServerOvGeometricPropertyDefinition::Create(name);
}

void SqlServerSchemaOverrideUtil::PropertiesOvAdd(FdoRdbmsOvClassDefinition* pClass, FdoRdbmsOvPropertyDefinition *pProp)
{
    FdoSqlServerOvPropertiesP(((FdoSqlServerOvClassDefinition*)pClass)->GetProperties())->Add(dynamic_cast<FdoSqlServerOvPropertyDefinition*>(pProp));
}

void SqlServerSchemaOverrideUtil::ClassOvSetTable(FdoRdbmsOvClassDefinition* pClass, FdoRdbmsOvTable* pTable)
{
    ((FdoSqlServerOvClassDefinition*)pClass)->SetTable((FdoSqlServerOvTable*)pTable);
}

void SqlServerSchemaOverrideUtil::DataPropOvSetColumn(FdoRdbmsOvDataPropertyDefinition* pDataProp, FdoRdbmsOvColumn* pDataColumn)
{
    ((FdoSqlServerOvDataPropertyDefinition*)pDataProp)->SetColumn((FdoSqlServerOvColumn*)pDataColumn);
}

void SqlServerSchemaOverrideUtil::GeometricPropOvSetColumn(FdoRdbmsOvGeometricPropertyDefinition* pGeomProp, FdoRdbmsOvGeometricColumn* pGeometricColumn)
{
    ((FdoSqlServerOvGeometricPropertyDefinition*)pGeomProp)->SetColumn((FdoSqlServerOvGeometricColumn*)pGeometricColumn);
}

FdoRdbmsOvObjectPropertyDefinition* SqlServerSchemaOverrideUtil::CreateOvObjectPropertyDefinition(FdoString *name)
{
    return FdoSqlServerOvObjectPropertyDefinition::Create(name);
}

FdoRdbmsOvPropertyMappingSingle* SqlServerSchemaOverrideUtil::CreateOvPropertyMappingSingle()
{
    return FdoSqlServerOvPropertyMappingSingle::Create();
}

FdoRdbmsOvPropertyMappingConcrete* SqlServerSchemaOverrideUtil::CreateOvPropertyMappingConcrete()
{
    return FdoSqlServerOvPropertyMappingConcrete::Create();
}

void SqlServerSchemaOverrideUtil::ObjectPropertyOvSetMappingDefinition(FdoRdbmsOvObjectPropertyDefinition* pObProp, FdoRdbmsOvPropertyMappingDefinition* mapping)
{
    ((FdoSqlServerOvObjectPropertyDefinition*)pObProp)->SetMappingDefinition(dynamic_cast<FdoSqlServerOvPropertyMappingDefinition*>(mapping));
}

void SqlServerSchemaOverrideUtil::PropertyMappingOvSetInternalClass(FdoRdbmsOvPropertyMappingRelation* pMapping, FdoRdbmsOvClassDefinition* pClass)
{
    (dynamic_cast<FdoSqlServerOvPropertyMappingRelation*>(pMapping))->SetInternalClass((FdoSqlServerOvClassDefinition*)pClass);
}

void SqlServerSchemaOverrideUtil::SchemaOvSetOwner(FdoRdbmsOvPhysicalSchemaMapping *mapping, FdoString* owner)
{
    ((FdoSqlServerOvPhysicalSchemaMapping*)mapping)->SetOwner( owner );
}

