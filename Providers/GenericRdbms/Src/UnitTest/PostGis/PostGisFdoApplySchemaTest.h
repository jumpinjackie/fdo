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

#ifndef POSTGIS_FDOAPPLYSCHEMATEST_H
#define POSTGIS_FDOAPPLYSCHEMATEST_H

#include "FdoApplySchemaTest.h"
#include <Rdbms/Override/PostGis/PostGisOvPropertyMappingSingle.h>
#include <Rdbms/Override/PostGis/PostGisOvPropertyMappingConcrete.h>
#include <Rdbms/Override/PostGis/PostGisOvClassDefinition.h>
#include <Rdbms/Override/PostGis/PostGisOvObjectPropertyDefinition.h>
#include <Rdbms/Override/PostGis/PostGisOvPhysicalSchemaMapping.h>

class PostGisFdoApplySchemaTest : public FdoApplySchemaTest
{
public:
    CPPUNIT_TEST_SUB_SUITE (PostGisFdoApplySchemaTest, FdoApplySchemaTest);
    CPPUNIT_TEST_SUITE_END ();
	
public:
	void  set_provider();
	
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
    virtual void TableOvSetTablespace(FdoRdbmsOvTable* pTable, FdoString *tablespace);
    virtual void DataPropOvSetColumn(FdoRdbmsOvDataPropertyDefinition* pDataProp, FdoRdbmsOvColumn* pDataColumn);
    virtual void GeometricPropOvSetColumn(FdoRdbmsOvGeometricPropertyDefinition* pGeomProp, FdoRdbmsOvGeometricColumn* pGeometricColumn);
    virtual FdoRdbmsOvObjectPropertyDefinition* CreateOvObjectPropertyDefinition(FdoString *name);
    virtual FdoRdbmsOvPropertyMappingSingle* CreateOvPropertyMappingSingle();
    virtual FdoRdbmsOvPropertyMappingConcrete* CreateOvPropertyMappingConcrete();
    virtual void ObjectPropertyOvSetMappingDefinition(FdoRdbmsOvObjectPropertyDefinition* pObProp, FdoRdbmsOvPropertyMappingDefinition* mapping);
    virtual void PropertyMappingOvSetInternalClass(FdoRdbmsOvPropertyMappingRelation* pMapping, FdoRdbmsOvClassDefinition* pClass);
    virtual void ShemaOvSetOwner(FdoRdbmsOvPhysicalSchemaMapping *mapping, FdoString* owner);

    virtual void VldClassCapabilities( int ltMode, int lckMode, FdoClassDefinition* pClass );

    virtual bool CanApplyWithoutMetaSchema();
    virtual bool CanCreateSchemaWithoutMetaSchema();
    virtual bool SupportsCrossDatastoreDependencies() {return false;};

    virtual FdoStringP SchemaTestErrFile( int fileNum, bool isMaster );
    virtual FdoStringP SchemaOvErrFile( int fileNum, bool isMaster );
    virtual FdoStringP LogicalPhysicalBend( FdoString* inFile );
    virtual FdoStringP LogicalPhysicalFormat( FdoString* inFile );

    virtual FdoStringP GetDefaultSchemaName();
    virtual FdoStringP GetValueColumnName();

};


#endif // PostGis_FDOAPPLYSCHEMATEST_H
