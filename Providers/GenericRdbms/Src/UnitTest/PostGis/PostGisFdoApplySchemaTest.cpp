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
#include "PostGisFdoApplySchemaTest.h"
#include "UnitTestUtil.h"

CPPUNIT_TEST_SUITE_REGISTRATION( PostGisFdoApplySchemaTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( PostGisFdoApplySchemaTest, "FdoApplySchemaTest");

void PostGisFdoApplySchemaTest::set_provider()
{
	UnitTestUtil::SetProvider( "PostGIS" );
	mIsLowerDatastoreName = true;
}


FdoRdbmsOvClassDefinition* PostGisFdoApplySchemaTest::CreateOvClassDefinition(FdoString *name)
{
    if (name == NULL)
        return FdoPostGISOvClassDefinition::Create();
    else
    {
        FdoPtr<FdoPostGISOvClassDefinition> classOv = FdoPostGISOvClassDefinition::Create(name);

        return FDO_SAFE_ADDREF(classOv.p);
    }
}

void PostGisFdoApplySchemaTest::ClassesOvAdd(FdoRdbmsOvPhysicalSchemaMapping* pSchema, FdoRdbmsOvClassDefinition* pClass)
{
    FdoPostGISOvClassesP(((FdoPostGISOvPhysicalSchemaMapping*)pSchema)->GetClasses())->Add((FdoPostGISOvClassDefinition*)pClass);
}

FdoRdbmsOvTable* PostGisFdoApplySchemaTest::CreateOvTable(FdoString* name)
{
    FdoPostGISOvTable* table = FdoPostGISOvTable::Create(name);

    if (0==wcscmp(name, L"oneforeign"))
    {
        table->SetOwner( UnitTestUtil::GetEnviron("datastore", DB_NAME_OVERRIDE_SUFFIX) );
    }

    return table;
}

FdoRdbmsOvColumn* PostGisFdoApplySchemaTest::CreateOvColumn(FdoString* name)
{
    return FdoPostGISOvColumn::Create(name);
}

FdoRdbmsOvGeometricColumn* PostGisFdoApplySchemaTest::CreateOvGeometricColumn(FdoString* name)
{
    return FdoPostGISOvGeometricColumn::Create(name);
}

FdoRdbmsOvDataPropertyDefinition* PostGisFdoApplySchemaTest::CreateOvDataPropertyDefinition(FdoString *name)
{
    return FdoPostGISOvDataPropertyDefinition::Create(name);
}

FdoRdbmsOvGeometricPropertyDefinition* PostGisFdoApplySchemaTest::CreateOvGeometricPropertyDefinition(FdoString *name)
{
    return FdoPostGISOvGeometricPropertyDefinition::Create(name);
}

void PostGisFdoApplySchemaTest::PropertiesOvAdd(FdoRdbmsOvClassDefinition* pClass, FdoRdbmsOvPropertyDefinition *pProp)
{
    FdoPostGISOvPropertiesP(((FdoPostGISOvClassDefinition*)pClass)->GetProperties())->Add(dynamic_cast<FdoPostGISOvPropertyDefinition*>(pProp));
}

void PostGisFdoApplySchemaTest::ClassOvSetTable(FdoRdbmsOvClassDefinition* pClass, FdoRdbmsOvTable* pTable)
{
    ((FdoPostGISOvClassDefinition*)pClass)->SetTable((FdoPostGISOvTable*)pTable);
}

void PostGisFdoApplySchemaTest::TableOvSetTablespace(FdoRdbmsOvTable* pTable, FdoString *tablespace)
{
    // do nothing (PostGis doesn't support tablespace per se)
}

void PostGisFdoApplySchemaTest::DataPropOvSetColumn(FdoRdbmsOvDataPropertyDefinition* pDataProp, FdoRdbmsOvColumn* pDataColumn)
{
    ((FdoPostGISOvDataPropertyDefinition*)pDataProp)->SetColumn((FdoPostGISOvColumn*)pDataColumn);
}

void PostGisFdoApplySchemaTest::GeometricPropOvSetColumn(FdoRdbmsOvGeometricPropertyDefinition* pGeomProp, FdoRdbmsOvGeometricColumn* pGeometricColumn)
{
    ((FdoPostGISOvGeometricPropertyDefinition*)pGeomProp)->SetColumn((FdoPostGISOvGeometricColumn*)pGeometricColumn);
}

FdoRdbmsOvObjectPropertyDefinition* PostGisFdoApplySchemaTest::CreateOvObjectPropertyDefinition(FdoString *name)
{
    return FdoPostGISOvObjectPropertyDefinition::Create(name);
}

FdoRdbmsOvPropertyMappingSingle* PostGisFdoApplySchemaTest::CreateOvPropertyMappingSingle()
{
    return FdoPostGISOvPropertyMappingSingle::Create();
}

FdoRdbmsOvPropertyMappingConcrete* PostGisFdoApplySchemaTest::CreateOvPropertyMappingConcrete()
{
    return FdoPostGISOvPropertyMappingConcrete::Create();
}

void PostGisFdoApplySchemaTest::ObjectPropertyOvSetMappingDefinition(FdoRdbmsOvObjectPropertyDefinition* pObProp, FdoRdbmsOvPropertyMappingDefinition* mapping)
{
    ((FdoPostGISOvObjectPropertyDefinition*)pObProp)->SetMappingDefinition(dynamic_cast<FdoPostGISOvPropertyMappingDefinition*>(mapping));
}

void PostGisFdoApplySchemaTest::PropertyMappingOvSetInternalClass(FdoRdbmsOvPropertyMappingRelation* pMapping, FdoRdbmsOvClassDefinition* pClass)
{
    (dynamic_cast<FdoPostGISOvPropertyMappingRelation*>(pMapping))->SetInternalClass((FdoPostGISOvClassDefinition*)pClass);
}

void PostGisFdoApplySchemaTest::ShemaOvSetOwner(FdoRdbmsOvPhysicalSchemaMapping *mapping, FdoString* owner)
{
    //FDO owner corresponds to a PostGis database.
    ((FdoPostGISOvPhysicalSchemaMapping*)mapping)->SetOwner( owner );
}

void PostGisFdoApplySchemaTest::VldClassCapabilities( int ltMode, int lckMode, FdoClassDefinition* pClass )
{
    // The following verifies that the AcDbEntity class capabilities are valid.
    FdoClassCapabilitiesP cc = pClass->GetCapabilities();
    CPPUNIT_ASSERT( cc != NULL );
    CPPUNIT_ASSERT( !(cc->SupportsLocking()) );
    CPPUNIT_ASSERT( !(cc->SupportsLongTransactions()) );

    FdoInt32 lockTypeCount;
    FdoLockType* lockTypes = cc->GetLockTypes( lockTypeCount );

    CPPUNIT_ASSERT( lockTypeCount == 0 );
}

bool PostGisFdoApplySchemaTest::CanApplyWithoutMetaSchema()
{
    return true;
}

bool PostGisFdoApplySchemaTest::CanCreateSchemaWithoutMetaSchema()
{
    return true;
}

FdoStringP PostGisFdoApplySchemaTest::LogicalPhysicalBend( FdoString* inFile )
{
    FdoStringP outFile = UnitTestUtil::GetOutputFileName( inFile );

    _logicalPhysicalBend( inFile, outFile, L"PostGIS" );

    return outFile;
}

FdoStringP PostGisFdoApplySchemaTest::LogicalPhysicalFormat( FdoString* inFile )
{
    FdoStringP outFile = FdoStringP(inFile).Replace( L".", L"_LPhF." );

    _logicalPhysicalFormat( inFile, outFile, L"PostGIS" );

    return outFile;
}

FdoStringP PostGisFdoApplySchemaTest::SchemaTestErrFile( int fileNum, bool isMaster ) 
{
    if ( isMaster ) {
        if ( (fileNum == 3) || (fileNum == 6) || (fileNum == 9) ) 
            return L"";
		return FdoStringP::Format( L"apply_schema_err%d%ls%ls.txt", fileNum,
			(fileNum == 2 || fileNum == 5 || fileNum == 8) ? L"M" : L"", L"_master");
    }
    return UnitTestUtil::GetOutputFileName( FdoStringP::Format( L"apply_schema_err%d%ls.txt",fileNum,
        (fileNum == 2 || fileNum == 3 || fileNum == 5 || fileNum == 6 || fileNum == 8 || fileNum == 9) ? L"M" : L"") );
}

FdoStringP PostGisFdoApplySchemaTest::GetDefaultSchemaName()
{
	return L"public";
}

