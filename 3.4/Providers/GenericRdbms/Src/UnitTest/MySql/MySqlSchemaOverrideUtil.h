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

#ifndef MYSQL_SCHEMAOVERRIDEUTIL_H
#define MYSQL_SCHEMAOVERRIDEUTIL_H

#include "SchemaOverrideUtil.h"
#include <Rdbms/Override/MySQL/MySqlOvPropertyMappingSingle.h>
#include <Rdbms/Override/MySQL/MySqlOvPropertyMappingConcrete.h>
#include <Rdbms/Override/MySQL/MySqlOvPhysicalSchemaMapping.h>

class MySqlSchemaOverrideUtil : public SchemaOverrideUtil
{
    // Helper methods for overrides tests; overriden by each provider:
    virtual FdoRdbmsOvClassDefinition* CreateOvClassDefinition(FdoString *name = NULL);
    virtual void ClassesOvAdd(FdoRdbmsOvPhysicalSchemaMapping* pSchema, FdoRdbmsOvClassDefinition* pClass);
    virtual FdoRdbmsOvTable* CreateOvTable(FdoString* name);
    virtual FdoRdbmsOvColumn* CreateOvColumn(FdoString* name);
    virtual FdoRdbmsOvGeometricColumn* CreateOvGeometricColumn(FdoString* name);
    virtual FdoRdbmsOvDataPropertyDefinition* CreateOvDataPropertyDefinition(FdoString *name);
    virtual FdoRdbmsOvGeometricPropertyDefinition* CreateOvGeometricPropertyDefinition(FdoString *name);
    virtual void PropertiesOvAdd(FdoRdbmsOvClassDefinition* pClass, FdoRdbmsOvPropertyDefinition *pProp);
    virtual void ClassOvSetTable(FdoRdbmsOvClassDefinition* pClass, FdoRdbmsOvTable* pTable);
    virtual void DataPropOvSetColumn(FdoRdbmsOvDataPropertyDefinition* pDataProp, FdoRdbmsOvColumn* pDataColumn);
    virtual void GeometricPropOvSetColumn(FdoRdbmsOvGeometricPropertyDefinition* pGeomProp, FdoRdbmsOvGeometricColumn* pGeometricColumn);
    virtual FdoRdbmsOvObjectPropertyDefinition* CreateOvObjectPropertyDefinition(FdoString *name);
    virtual FdoRdbmsOvPropertyMappingSingle* CreateOvPropertyMappingSingle();
    virtual FdoRdbmsOvPropertyMappingConcrete* CreateOvPropertyMappingConcrete();
    virtual void ObjectPropertyOvSetMappingDefinition(FdoRdbmsOvObjectPropertyDefinition* pObProp, FdoRdbmsOvPropertyMappingDefinition* mapping);
    virtual void PropertyMappingOvSetInternalClass(FdoRdbmsOvPropertyMappingRelation* pMapping, FdoRdbmsOvClassDefinition* pClass);
    virtual void SchemaOvSetOwner(FdoRdbmsOvPhysicalSchemaMapping *mapping, FdoString* owner);
};

#endif // MySql_FDOAPPLYSCHEMATEST_H
