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
#include "PostGisSchemaOverrideUtil.h"

FdoRdbmsOvClassDefinition* PostGisSchemaOverrideUtil::CreateOvClassDefinition(FdoString *name)
{
    if (name == NULL)
        return FdoPostGISOvClassDefinition::Create();
    else
        return FdoPostGISOvClassDefinition::Create(name);
}

void PostGisSchemaOverrideUtil::ClassesOvAdd(FdoRdbmsOvPhysicalSchemaMapping* pSchema, FdoRdbmsOvClassDefinition* pClass)
{
    FdoPostGISOvClassesP(((FdoPostGISOvPhysicalSchemaMapping*)pSchema)->GetClasses())->Add((FdoPostGISOvClassDefinition*)pClass);
}

FdoRdbmsOvTable* PostGisSchemaOverrideUtil::CreateOvTable(FdoString* name)
{
    FdoPtr<FdoPostGISOvTable> ovTable = FdoPostGISOvTable::Create(name);

    return FDO_SAFE_ADDREF(ovTable.p);
}

FdoRdbmsOvColumn* PostGisSchemaOverrideUtil::CreateOvColumn(FdoString* name)
{
    return FdoPostGISOvColumn::Create(name);
}

FdoRdbmsOvGeometricColumn* PostGisSchemaOverrideUtil::CreateOvGeometricColumn(FdoString* name)
{
    return FdoPostGISOvGeometricColumn::Create(name);
}

FdoRdbmsOvDataPropertyDefinition* PostGisSchemaOverrideUtil::CreateOvDataPropertyDefinition(FdoString *name)
{
    return FdoPostGISOvDataPropertyDefinition::Create(name);
}

FdoRdbmsOvGeometricPropertyDefinition* PostGisSchemaOverrideUtil::CreateOvGeometricPropertyDefinition(FdoString *name)
{
    return FdoPostGISOvGeometricPropertyDefinition::Create(name);
}

void PostGisSchemaOverrideUtil::PropertiesOvAdd(FdoRdbmsOvClassDefinition* pClass, FdoRdbmsOvPropertyDefinition *pProp)
{
    FdoPostGISOvPropertiesP(((FdoPostGISOvClassDefinition*)pClass)->GetProperties())->Add(dynamic_cast<FdoPostGISOvPropertyDefinition*>(pProp));
}

void PostGisSchemaOverrideUtil::ClassOvSetTable(FdoRdbmsOvClassDefinition* pClass, FdoRdbmsOvTable* pTable)
{
    ((FdoPostGISOvClassDefinition*)pClass)->SetTable((FdoPostGISOvTable*)pTable);
}

void PostGisSchemaOverrideUtil::DataPropOvSetColumn(FdoRdbmsOvDataPropertyDefinition* pDataProp, FdoRdbmsOvColumn* pDataColumn)
{
    ((FdoPostGISOvDataPropertyDefinition*)pDataProp)->SetColumn((FdoPostGISOvColumn*)pDataColumn);
}

void PostGisSchemaOverrideUtil::GeometricPropOvSetColumn(FdoRdbmsOvGeometricPropertyDefinition* pGeomProp, FdoRdbmsOvGeometricColumn* pGeometricColumn)
{
    ((FdoPostGISOvGeometricPropertyDefinition*)pGeomProp)->SetColumn((FdoPostGISOvGeometricColumn*)pGeometricColumn);
}

FdoRdbmsOvObjectPropertyDefinition* PostGisSchemaOverrideUtil::CreateOvObjectPropertyDefinition(FdoString *name)
{
    return FdoPostGISOvObjectPropertyDefinition::Create(name);
}

FdoRdbmsOvPropertyMappingSingle* PostGisSchemaOverrideUtil::CreateOvPropertyMappingSingle()
{
    return FdoPostGISOvPropertyMappingSingle::Create();
}

FdoRdbmsOvPropertyMappingConcrete* PostGisSchemaOverrideUtil::CreateOvPropertyMappingConcrete()
{
    return FdoPostGISOvPropertyMappingConcrete::Create();
}

void PostGisSchemaOverrideUtil::ObjectPropertyOvSetMappingDefinition(FdoRdbmsOvObjectPropertyDefinition* pObProp, FdoRdbmsOvPropertyMappingDefinition* mapping)
{
    ((FdoPostGISOvObjectPropertyDefinition*)pObProp)->SetMappingDefinition(dynamic_cast<FdoPostGISOvPropertyMappingDefinition*>(mapping));
}

void PostGisSchemaOverrideUtil::PropertyMappingOvSetInternalClass(FdoRdbmsOvPropertyMappingRelation* pMapping, FdoRdbmsOvClassDefinition* pClass)
{
    (dynamic_cast<FdoPostGISOvPropertyMappingRelation*>(pMapping))->SetInternalClass((FdoPostGISOvClassDefinition*)pClass);
}

void PostGisSchemaOverrideUtil::SchemaOvSetOwner(FdoRdbmsOvPhysicalSchemaMapping *mapping, FdoString* owner)
{
    ((FdoPostGISOvPhysicalSchemaMapping*)mapping)->SetOwner( owner );
}

