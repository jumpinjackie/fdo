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
#include "SqlServerFdoSchemaTest.h"
#include "UnitTestUtil.h"
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvPhysicalSchemaMapping.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvPropertyDefinitionCollection.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvDataPropertyDefinition.h>

CPPUNIT_TEST_SUITE_REGISTRATION( SqlServerFdoSchemaTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( SqlServerFdoSchemaTest, "FdoSchemaTest");

void SqlServerFdoSchemaTest::set_provider()
{
	UnitTestUtil::SetProvider( "SQLServerSpatial" );
}

void SqlServerFdoSchemaTest::SQLServerOverridesTest()
{
    // Setup some overrides via API:
    //////////////////////////////////////////////////////////////////////////////////////////

    FdoPtr<FdoSqlServerOvPhysicalSchemaMapping> schemaOverrides = FdoSqlServerOvPhysicalSchemaMapping::Create(L"MySchema");
    schemaOverrides->SetDatabase(L"DefaultDatabase");
    schemaOverrides->SetIndexFilegroup(L"DefaultIndexFilegroup");
    schemaOverrides->SetOwner(L"DefaultOwner");
    schemaOverrides->SetTableFilegroup(L"DefaultTableFilegroup");
    schemaOverrides->SetTextFilegroup(L"DefaultTextFilegroup");
    schemaOverrides->SetTextInRow(SqlServerOvTextInRowOption_NotInRow);
    FdoPtr<FdoSqlServerOvClassCollection> classesOverrides = schemaOverrides->GetClasses();

    FdoPtr<FdoSqlServerOvClassDefinition> classOverrides = FdoSqlServerOvClassDefinition::Create(L"MyClass1");
//TODO:    geomColOverrides->SetGeometryStorageFormat(SqlServerOvGeometryStorageFormat_WKT);
//TODO:    geomColOverrides->SetGeometryStorageLocation(SqlServerOvGeometryStorageLocation_UserTable);
    classOverrides->SetIdentitySeed(10);
    classOverrides->SetIdentityIncrement(5);
    classOverrides->SetIdentityIsGloballyUnique(true);
    classOverrides->SetIdentityPropertyName(L"MyIdentityProperty");
    FdoPtr<FdoSqlServerOvTable> tableOverrides = FdoSqlServerOvTable::Create(L"MyTable1");
    tableOverrides->SetDatabase(L"Table1Database");
    tableOverrides->SetIndexFilegroup(L"Table1IndexFilegroup");
    tableOverrides->SetTextFilegroup(L"Table1TextFilegroup");
    tableOverrides->SetTextInRow(SqlServerOvTextInRowOption_InRow);
    classOverrides->SetTable(tableOverrides);
    FdoPtr<FdoSqlServerOvPropertyDefinitionCollection> propertiesOverrides = classOverrides->GetProperties();
    FdoPtr<FdoSqlServerOvGeometricPropertyDefinition> geomPropOverrides = FdoSqlServerOvGeometricPropertyDefinition::Create(L"Geometry");
    FdoPtr<FdoSqlServerOvGeometricColumn> geomColOverrides = FdoSqlServerOvGeometricColumn::Create(L"MyGeomCol");
    geomPropOverrides->SetColumn(geomColOverrides);
    propertiesOverrides->Add(geomPropOverrides);
    FdoPtr<FdoSqlServerOvDataPropertyDefinition> dataPropOverrides = FdoSqlServerOvDataPropertyDefinition::Create(L"MyProperty1");
    FdoPtr<FdoSqlServerOvColumn> dataColOverrides = FdoSqlServerOvColumn::Create(L"MyDataCol1");
    dataColOverrides->SetFormula(L"(MyOtherCol1 + MyOtherCol2) / MyOtherCol3");
    dataPropOverrides->SetColumn(dataColOverrides);
    propertiesOverrides->Add(dataPropOverrides);
    classesOverrides->Add(classOverrides);


    // Serialize the overrides to XML:
    //////////////////////////////////////////////////////////////////////////////////////////

    // TODO: erase file prior?
    schemaOverrides->WriteXml(UnitTestUtil::GetOutputFileName( L"FdoSchemaTest_Output.xml" ) );

    // Deserialize the overrides from XML:
    //////////////////////////////////////////////////////////////////////////////////////////

    FdoPtr<FdoPhysicalSchemaMappingCollection> schemasOverridesFromXML = FdoPhysicalSchemaMappingCollection::Create();
    schemasOverridesFromXML->ReadXml(UnitTestUtil::GetOutputFileName( L"FdoSchemaTest_Output.xml" ) );

    CPPUNIT_ASSERT_MESSAGE("Wrong schema mappings count", schemasOverridesFromXML->GetCount()==1);
    FdoPtr<FdoPhysicalSchemaMapping> schemaOverridesGenericFromXML = schemasOverridesFromXML->GetItem(0);
    FdoPtr<FdoSqlServerOvPhysicalSchemaMapping> schemaOverridesFromXML;
    schemaOverridesFromXML = FDO_SAFE_ADDREF(dynamic_cast<FdoSqlServerOvPhysicalSchemaMapping*>(schemaOverridesGenericFromXML.p));

    CPPUNIT_ASSERT_MESSAGE("Schema mapping type isnt FdoSqlServerOvPhysicalSchemaMapping", schemaOverridesFromXML != NULL);
    CPPUNIT_ASSERT_MESSAGE("Schema mapping name incorrect", 0==wcscmp(schemaOverridesFromXML->GetName(), schemaOverrides->GetName()));
    CPPUNIT_ASSERT_MESSAGE("Schema mapping database incorrect", 0==wcscmp(schemaOverridesFromXML->GetDatabase(), schemaOverrides->GetDatabase()));
    CPPUNIT_ASSERT_MESSAGE("Schema mapping index filegroup incorrect", 0==wcscmp(schemaOverridesFromXML->GetDatabase(), schemaOverrides->GetDatabase()));
    CPPUNIT_ASSERT_MESSAGE("Schema mapping owner incorrect", 0==wcscmp(schemaOverridesFromXML->GetOwner(), schemaOverrides->GetOwner()));
    CPPUNIT_ASSERT_MESSAGE("Schema mapping table filegroup incorrect", 0==wcscmp(schemaOverridesFromXML->GetTableFilegroup(), schemaOverrides->GetTableFilegroup()));
    CPPUNIT_ASSERT_MESSAGE("Schema mapping text filegroup incorrect", 0==wcscmp(schemaOverridesFromXML->GetTextFilegroup(), schemaOverrides->GetTextFilegroup()));
    CPPUNIT_ASSERT_MESSAGE("Schema mapping text-in-row option incorrect", schemaOverridesFromXML->GetTextInRow() == schemaOverrides->GetTextInRow());

    FdoPtr<FdoSqlServerOvClassCollection> classesOverridesFromXML = schemaOverrides->GetClasses();
    FdoPtr<FdoSqlServerOvClassDefinition> classOverridesFromXML = classesOverridesFromXML->GetItem(L"MyClass1");
//TODO:    CPPUNIT_ASSERT_MESSAGE("Geometry column storage format override incorrect", geomColOverridesFromXML->GetGeometryStorageFormat() == geomColOverrides->GetGeometryStorageFormat());
//TODO:    CPPUNIT_ASSERT_MESSAGE("Geometry column storage location override incorrect", geomColOverridesFromXML->GetGeometryStorageLocation() == geomColOverrides->GetGeometryStorageLocation());
    CPPUNIT_ASSERT_MESSAGE("Class identity seed override incorrect", classOverridesFromXML->GetIdentitySeed() == classOverrides->GetIdentitySeed());
    CPPUNIT_ASSERT_MESSAGE("Class identity increment override incorrect", classOverridesFromXML->GetIdentityIncrement() == classOverrides->GetIdentityIncrement());
    CPPUNIT_ASSERT_MESSAGE("Class identity global-uniqueness override incorrect", classOverridesFromXML->GetIdentityIsGloballyUnique() == classOverrides->GetIdentityIsGloballyUnique());
    CPPUNIT_ASSERT_MESSAGE("Class identity property name override incorrect", 0==wcscmp(classOverridesFromXML->GetIdentityPropertyName(), classOverrides->GetIdentityPropertyName()));

    FdoPtr<FdoSqlServerOvTable> tableOverridesFromXML = classOverridesFromXML->GetTable();
    CPPUNIT_ASSERT_MESSAGE("Table name override incorrect", 0==wcscmp(tableOverridesFromXML->GetName(), tableOverrides->GetName()));
    CPPUNIT_ASSERT_MESSAGE("Table database override incorrect", 0==wcscmp(tableOverridesFromXML->GetDatabase(), tableOverrides->GetDatabase()));
    CPPUNIT_ASSERT_MESSAGE("Table index filegroup override incorrect", 0==wcscmp(tableOverridesFromXML->GetIndexFilegroup(), tableOverrides->GetIndexFilegroup()));
    CPPUNIT_ASSERT_MESSAGE("Table text filegroup override incorrect", 0==wcscmp(tableOverridesFromXML->GetTextFilegroup(), tableOverrides->GetTextFilegroup()));
    CPPUNIT_ASSERT_MESSAGE("Table text-in-row option override incorrect", tableOverridesFromXML->GetTextInRow() == tableOverrides->GetTextInRow());

    FdoPtr<FdoSqlServerOvPropertyDefinitionCollection> propertiesOverridesFromXML = classOverridesFromXML->GetProperties();
    FdoPtr<FdoSqlServerOvPropertyDefinition> gPropOverridesFromXML = propertiesOverridesFromXML->GetItem(L"Geometry");
    FdoPtr<FdoSqlServerOvGeometricPropertyDefinition> geomPropOverridesFromXML = FDO_SAFE_ADDREF(dynamic_cast<FdoSqlServerOvGeometricPropertyDefinition*>(gPropOverridesFromXML.p));
    FdoPtr<FdoSqlServerOvGeometricColumn> geomColOverridesFromXML = geomPropOverridesFromXML->GetColumn();
    CPPUNIT_ASSERT_MESSAGE("Geometric property column name override incorrect", 0==wcscmp(geomColOverridesFromXML->GetName(), geomColOverrides->GetName()));
    FdoPtr<FdoSqlServerOvPropertyDefinition> propOverridesFromXML = propertiesOverridesFromXML->GetItem(L"MyProperty1");
    FdoPtr<FdoSqlServerOvDataPropertyDefinition> dataPropOverridesFromXML = FDO_SAFE_ADDREF(dynamic_cast<FdoSqlServerOvDataPropertyDefinition*>(propOverridesFromXML.p));
    CPPUNIT_ASSERT_MESSAGE("Data property name override incorrect", 0==wcscmp(dataPropOverridesFromXML->GetName(), dataPropOverrides->GetName()));
    FdoPtr<FdoSqlServerOvColumn> dataColOverridesFromXML = dataPropOverridesFromXML->GetColumn();
    CPPUNIT_ASSERT_MESSAGE("Data property column name override incorrect", 0==wcscmp(dataColOverridesFromXML->GetName(), dataColOverrides->GetName()));
    CPPUNIT_ASSERT_MESSAGE("Data property column formula override incorrect", 0==wcscmp(dataColOverridesFromXML->GetFormula(), dataColOverrides->GetFormula()));

}
