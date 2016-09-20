 //
 // Copyright (C) 2004-2006  Autodesk, Inc.
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

#ifndef CPP_UNIT_SCHEMAOVERRIDEUTIL_H
#define CPP_UNIT_SCHEMAOVERRIDEUTIL_H

#include <Rdbms/Override/RdbmsOvPhysicalSchemaMapping.h>

/* 
 * A test case for ApplySchemaCommand.
 *
 * Loads some schemas into a database.
 */

class SchemaOverrideUtil : public FdoIDisposable
{
public:
    SchemaOverrideUtil(void);
    virtual ~SchemaOverrideUtil(void);

    

    virtual FdoRdbmsOvClassDefinition* CreateOvClassDefinition(FdoString *name = NULL) { return NULL; };
    virtual void ClassesOvAdd(FdoRdbmsOvPhysicalSchemaMapping* pSchema, FdoRdbmsOvClassDefinition* pClass) {};
    virtual FdoRdbmsOvTable* CreateOvTable(FdoString* name) { return NULL; };
    virtual FdoRdbmsOvColumn* CreateOvColumn(FdoString* name) { return NULL; };
    virtual FdoRdbmsOvGeometricColumn* CreateOvGeometricColumn(FdoString* name) { return NULL; };
    virtual FdoRdbmsOvDataPropertyDefinition* CreateOvDataPropertyDefinition(FdoString *name) { return NULL; };
    virtual FdoRdbmsOvGeometricPropertyDefinition* CreateOvGeometricPropertyDefinition(FdoString *name) { return NULL; };
    virtual void PropertiesOvAdd(FdoRdbmsOvClassDefinition* pClass, FdoRdbmsOvPropertyDefinition *pProp) {};
    virtual void ClassOvSetTable(FdoRdbmsOvClassDefinition* pClass, FdoRdbmsOvTable* pTable) {};
    virtual void TableOvSetTablespace(FdoRdbmsOvTable* pTable, FdoString *tablespace) {};
    virtual void DataPropOvSetColumn(FdoRdbmsOvDataPropertyDefinition* pDataProp, FdoRdbmsOvColumn* pDataColumn) {};
    virtual void GeometricPropOvSetColumn(FdoRdbmsOvGeometricPropertyDefinition* pGeomProp, FdoRdbmsOvGeometricColumn* pGeometricColumn) {};
    virtual void ColumnOvSetSequence(FdoRdbmsOvColumn* pColumn, FdoString *sequence) {};
    virtual FdoRdbmsOvObjectPropertyDefinition* CreateOvObjectPropertyDefinition(FdoString *name) { return NULL; };
    virtual FdoRdbmsOvPropertyMappingSingle* CreateOvPropertyMappingSingle() { return NULL; };
    virtual FdoRdbmsOvPropertyMappingConcrete* CreateOvPropertyMappingConcrete() { return NULL; };
    virtual void ObjectPropertyOvSetMappingDefinition(FdoRdbmsOvObjectPropertyDefinition* pObProp, FdoRdbmsOvPropertyMappingDefinition* mapping) {};
    virtual void PropertyMappingOvSetInternalClass(FdoRdbmsOvPropertyMappingRelation* pMapping, FdoRdbmsOvClassDefinition* pClass) {};
    virtual void SchemaOvSetOwner(FdoRdbmsOvPhysicalSchemaMapping *mapping, FdoString* owner) {};

protected:
    virtual void Dispose();

};

typedef FdoPtr<SchemaOverrideUtil> SchemaOverrideUtilP;

#endif // CPP_UNIT_FDOAPPLYSCHEMATEST_H
