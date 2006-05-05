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

#include "SelectTest.h"
#include "SDF/SdfCommandType.h"
#include "SDF/ICreateSDFFile.h"

#ifdef _WIN32
const wchar_t* SELECT_TEST_FILE = L"..\\..\\TestData\\SelectTest.SDX";
#else
#include <unistd.h>
const wchar_t* SELECT_TEST_FILE = L"../../TestData/Acad.SDX";
#endif

CPPUNIT_TEST_SUITE_REGISTRATION( SelectTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( SelectTest, "SelectTest");

SelectTest::SelectTest(void)
{
}

SelectTest::~SelectTest(void)
{
}

void SelectTest::setUp ()
{
}

// Tests select of only non-inherited properties
void SelectTest::TestProps()
{
    FdoDictionaryP baseProps = FdoDictionary::Create();
    FdoDictionaryP props = FdoDictionary::Create();
    props->Add( FdoDictionaryElementP(FdoDictionaryElement::Create(L"Prop9", L"")) );
    props->Add( FdoDictionaryElementP(FdoDictionaryElement::Create(L"Prop7", L"")) );

    CPPUNIT_ASSERT( baseProps->GetCount() == 0 );
    CPPUNIT_ASSERT( props->GetCount() == 2 );

    DoTest( baseProps, props, L"Prop1", L"Prop8" );
}

// Tests select of only base (inherited) properties
void SelectTest::TestBaseProps()
{
    FdoDictionaryP baseProps = FdoDictionary::Create();
    FdoDictionaryP props = FdoDictionary::Create();
    baseProps->Add( FdoDictionaryElementP(FdoDictionaryElement::Create(L"Prop2", L"")) );
    baseProps->Add( FdoDictionaryElementP(FdoDictionaryElement::Create(L"Prop4", L"")) );
    baseProps->Add( FdoDictionaryElementP(FdoDictionaryElement::Create(L"Prop6", L"")) );

    CPPUNIT_ASSERT( baseProps->GetCount() == 3 );
    CPPUNIT_ASSERT( props->GetCount() == 0 );

    DoTest( baseProps, props, L"Prop3", L"Prop7" );
}

// Tests selecting both base and non-inherited properties
void SelectTest::TestBothProps()
{
    FdoDictionaryP baseProps = FdoDictionary::Create();
    FdoDictionaryP props = FdoDictionary::Create();
    baseProps->Add( FdoDictionaryElementP(FdoDictionaryElement::Create(L"FeatId", L"")) );
    baseProps->Add( FdoDictionaryElementP(FdoDictionaryElement::Create(L"Prop1", L"")) );
    baseProps->Add( FdoDictionaryElementP(FdoDictionaryElement::Create(L"Prop2", L"")) );
    baseProps->Add( FdoDictionaryElementP(FdoDictionaryElement::Create(L"Prop3", L"")) );
    baseProps->Add( FdoDictionaryElementP(FdoDictionaryElement::Create(L"Prop4", L"")) );
    baseProps->Add( FdoDictionaryElementP(FdoDictionaryElement::Create(L"Prop6", L"")) );
    props->Add( FdoDictionaryElementP(FdoDictionaryElement::Create(L"Prop8", L"")) );

    CPPUNIT_ASSERT( baseProps->GetCount() == 6 );
    CPPUNIT_ASSERT( props->GetCount() == 1 );

    DoTest( baseProps, props, L"Prop5", L"Prop9" );
}

// Tests selecting all properties
void SelectTest::TestAllProps()
{
    FdoDictionaryP baseProps = FdoDictionary::Create();
    FdoDictionaryP props = FdoDictionary::Create();

    DoTest( baseProps, props, L"", L"" );
}

void SelectTest::DoTest (
    FdoDictionaryP baseProps, 
    FdoDictionaryP props, 
    FdoStringP exclBaseProp,
    FdoStringP exclProp
)
{
    FdoPtr<FdoIConnection> connection;
    FdoInt32 baseCount;
    FdoInt32 propCount;
    bool allSelected = false;

    if ( (baseProps->GetCount() == 0) && (props->GetCount() == 0) ) {
        baseCount = 7;
        propCount = 3;
        allSelected = true;
    }
    else {
        baseCount = baseProps->GetCount();
        propCount = props->GetCount();
    }

	try {
        connection = CreateDb();

        FdoPtr<FdoISelect> selectCommand = (FdoISelect *) connection->CreateCommand(FdoCommandType_Select);

        selectCommand->SetFeatureClassName( L"Class3" );

        FdoPtr<FdoIdentifierCollection> names = selectCommand->GetPropertyNames();
        FdoPtr<FdoIdentifier> name;
        FdoDictionaryElementP elt;
        FdoInt32 idx;

        // Add specified base properties to select list
        for ( idx = 0; idx < baseProps->GetCount(); idx ++ ) {
            elt = baseProps->GetItem(idx);
            name = FdoIdentifier::Create( elt->GetName() );
            names->Add(name);
        }

        // Add specified properties to select list
        for ( idx = 0; idx < props->GetCount(); idx ++ ) {
            elt = props->GetItem(idx);
            name = FdoIdentifier::Create( elt->GetName() );
            names->Add(name);
        }

		FdoPtr<FdoIFeatureReader> rdr = selectCommand->Execute();
        rdr->ReadNext();

        FdoClassDefinitionP classDef = rdr->GetClassDefinition();

        FdoPropertiesP propDefs = classDef->GetProperties();
        FdoPtr<FdoReadOnlyPropertyDefinitionCollection> basePropDefs = classDef->GetBaseProperties();

        // Verify that only the specified properties and base properties
        // are in the property lists for the reader's class definition.

        CPPUNIT_ASSERT( baseCount == basePropDefs->GetCount() );
        CPPUNIT_ASSERT( propCount == propDefs->GetCount() );

        if ( !allSelected ) {
            FdoPropertyP propDef;
            for ( idx = 0; idx < propDefs->GetCount(); idx++ ) {
                propDef = propDefs->GetItem(idx);
                CPPUNIT_ASSERT( props->Contains(propDef->GetName()) );
            }
            
            if ( exclProp.GetLength() > 0 )
                CPPUNIT_ASSERT( !props->Contains(exclProp) );

            for ( idx = 0; idx < basePropDefs->GetCount(); idx++ ) {
                propDef = basePropDefs->GetItem(idx);
                CPPUNIT_ASSERT( baseProps->Contains(propDef->GetName()) );
            }

            if ( exclBaseProp.GetLength() > 0 )
                CPPUNIT_ASSERT( !props->Contains(exclBaseProp) );
        }
    }
	catch ( FdoException* e ) 
	{
        connection->Close();
        printf( "Exception: %ls\n", e->GetExceptionMessage() );
	}

	connection->Close();
}

FdoIConnection* SelectTest::CreateDb() 
{
    FdoIConnection* connection;

#ifdef _WIN32
	wchar_t fullpath[1024];
	_wfullpath(fullpath, SELECT_TEST_FILE, 1024);
#else
	char cpath[1024];
	char cfullpath[1024];
	wcstombs(cpath, SELECT_TEST_FILE, 1024);
	realpath(cpath, cfullpath);
	wchar_t fullpath[1024];
	mbstowcs(fullpath, cfullpath, 1024);
#endif

    size_t len = wcstombs(NULL, SELECT_TEST_FILE, 0);
	char* mbsPath = new char[len+1];
	wcstombs(mbsPath, SELECT_TEST_FILE, len+1);

#ifdef _WIN32    
	SetFileAttributes(mbsPath, FILE_ATTRIBUTE_NORMAL);
	DeleteFile(mbsPath);
#else
	unlink(mbsPath);
#endif

	delete[] mbsPath;
    FdoPtr<IConnectionManager> manager = FdoFeatureAccessManager::GetConnectionManager ();
    connection = manager->CreateConnection (L"OSGeo.SDF.3.2");

    FdoPtr<FdoICreateSDFFile> crsdf = (FdoICreateSDFFile*)(connection->CreateCommand(SdfCommandType_CreateSDFFile));

	crsdf->SetCoordinateSystemWKT(L"[LL84]");
    crsdf->SetFileName(fullpath);
	crsdf->SetSpatialContextDescription(L"World Coordinate System, Degrees, what else do you need to know?");
	crsdf->SetSpatialContextName(L"World Geodetic Coordinate System, 1984");
	crsdf->SetXYTolerance(17.0);
	crsdf->SetZTolerance(3.14159);

	crsdf->Execute();

    std::wstring connStr = std::wstring(L"File=") + std::wstring(fullpath);
    connection->SetConnectionString(connStr.c_str());
    connection->Open();

    FdoFeatureSchemaP pSchema = FdoFeatureSchema::Create( L"Schema1", L"AutoCAD schema" );

    FdoFeatureClassP pClass1 = FdoFeatureClass::Create( L"Class1", L"AutoCAD entity base class" );
    FdoClassesP(pSchema->GetClasses())->Add(pClass1);
    pClass1->SetIsAbstract(false);

    FdoDataPropertyP pProp = FdoDataPropertyDefinition::Create( L"FeatId", L"id" );
    pProp->SetDataType( FdoDataType_Int32 );
    pProp->SetNullable(false);
    pProp->SetIsAutoGenerated(true);
    FdoPropertiesP(pClass1->GetProperties())->Add( pProp );
    FdoDataPropertiesP(pClass1->GetIdentityProperties())->Add( pProp );

    pProp = FdoDataPropertyDefinition::Create( L"Prop1", L"" );
    pProp->SetDataType( FdoDataType_Int32 );
    pProp->SetNullable(true);
    FdoPropertiesP(pClass1->GetProperties())->Add( pProp );

    pProp = FdoDataPropertyDefinition::Create( L"Prop2", L"" );
    pProp->SetDataType( FdoDataType_Int32 );
    pProp->SetNullable(true);
    FdoPropertiesP(pClass1->GetProperties())->Add( pProp );

    pProp = FdoDataPropertyDefinition::Create( L"Prop3", L"" );
    pProp->SetDataType( FdoDataType_Int32 );
    pProp->SetNullable(true);
    FdoPropertiesP(pClass1->GetProperties())->Add( pProp );

    FdoFeatureClassP pClass2 = FdoFeatureClass::Create( L"Class2", L"AutoCAD entity base class" );
    FdoClassesP(pSchema->GetClasses())->Add(pClass2);
    pClass2->SetIsAbstract(false);
    pClass2->SetBaseClass(pClass1);

    pProp = FdoDataPropertyDefinition::Create( L"Prop4", L"" );
    pProp->SetDataType( FdoDataType_Int32 );
    pProp->SetNullable(true);
    FdoPropertiesP(pClass2->GetProperties())->Add( pProp );

    pProp = FdoDataPropertyDefinition::Create( L"Prop5", L"" );
    pProp->SetDataType( FdoDataType_Int32 );
    pProp->SetNullable(true);
    FdoPropertiesP(pClass2->GetProperties())->Add( pProp );

    pProp = FdoDataPropertyDefinition::Create( L"Prop6", L"" );
    pProp->SetDataType( FdoDataType_Int32 );
    pProp->SetNullable(true);
    FdoPropertiesP(pClass2->GetProperties())->Add( pProp );

    FdoFeatureClassP pClass3 = FdoFeatureClass::Create( L"Class3", L"AutoCAD entity base class" );
    FdoClassesP(pSchema->GetClasses())->Add(pClass3);
    pClass3->SetIsAbstract(false);
    pClass3->SetBaseClass(pClass2);

    pProp = FdoDataPropertyDefinition::Create( L"Prop7", L"" );
    pProp->SetDataType( FdoDataType_Int32 );
    pProp->SetNullable(true);
    FdoPropertiesP(pClass3->GetProperties())->Add( pProp );

    pProp = FdoDataPropertyDefinition::Create( L"Prop8", L"" );
    pProp->SetDataType( FdoDataType_Int32 );
    pProp->SetNullable(true);
    FdoPropertiesP(pClass3->GetProperties())->Add( pProp );

    pProp = FdoDataPropertyDefinition::Create( L"Prop9", L"" );
    pProp->SetDataType( FdoDataType_Int32 );
    pProp->SetNullable(true);
    FdoPropertiesP(pClass3->GetProperties())->Add( pProp );

    FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);
    pCmd->SetFeatureSchema( pSchema );
    pCmd->Execute();

    FdoPtr<FdoIInsert> insertCommand = (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
    insertCommand->SetFeatureClassName(L"Schema1:Class3");
    FdoPtr<FdoPropertyValueCollection> propertyValues = insertCommand->GetPropertyValues();
    FdoPtr<FdoDataValue> dataValue;
    FdoPtr<FdoPropertyValue> propertyValue;

    dataValue = FdoDataValue::Create((FdoInt64) 1);
    propertyValue =  FdoPropertyValue::Create();
    propertyValue->SetName( L"Prop1" );
    propertyValues->Add( propertyValue );
    propertyValue->SetValue(dataValue);

    dataValue = FdoDataValue::Create((FdoInt64) 2);
    propertyValue =  FdoPropertyValue::Create();
    propertyValue->SetName( L"Prop2" );
    propertyValues->Add( propertyValue );
    propertyValue->SetValue(dataValue);

    dataValue = FdoDataValue::Create((FdoInt64) 3);
    propertyValue =  FdoPropertyValue::Create();
    propertyValue->SetName( L"Prop3" );
    propertyValues->Add( propertyValue );
    propertyValue->SetValue(dataValue);

    dataValue = FdoDataValue::Create((FdoInt64) 4);
    propertyValue =  FdoPropertyValue::Create();
    propertyValue->SetName( L"Prop4" );
    propertyValues->Add( propertyValue );
    propertyValue->SetValue(dataValue);

    FdoPtr<FdoIFeatureReader> reader = insertCommand->Execute();        

    return connection;
}
