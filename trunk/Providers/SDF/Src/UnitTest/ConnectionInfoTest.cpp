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

#include "stdafx.h"
#include "ConnectionInfoTest.h"
#include "SDF/SdfCommandType.h"
#include "SDF/ICreateSDFFile.h"

#ifdef _WIN32
const wchar_t* CI_TEST_FILE = L"..\\..\\TestData\\SelectTest.SDX";
#else
#include <unistd.h>
const wchar_t* CI_TEST_FILE = L"../../TestData/Acad.SDX";
#endif

CPPUNIT_TEST_SUITE_REGISTRATION( ConnectionInfoTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( ConnectionInfoTest, "ConnectionInfoTest");

ConnectionInfoTest::ConnectionInfoTest ()
{
}

ConnectionInfoTest::~ConnectionInfoTest ()
{
}

void ConnectionInfoTest::setUp ()
{
}

// Test whether or not the provider type and its list of dependent files is
// reported.
void ConnectionInfoTest::TestProviderInfo ()
{
    FdoPtr<FdoIConnection> connection;

    printf("Checking for Provider Type and Dependent List Info \n");
	try {
        connection = CreateDb();
        FdoPtr<FdoIConnectionInfo> connectionInfo = connection->GetConnectionInfo();
        printf(" ...> Checking for Provider Type\n");
        FdoProviderDatastoreType providerType = connectionInfo->GetProviderDatastoreType();

        switch (providerType) {

            case FdoProviderDatastoreType_DatabaseServer:
                printf(" ......> Found: 'Database Server' (unexpected result)\n");
                throw FdoException::Create(L"Unexpected provider type 'Database Server' when expecting 'File'");
                break;
            case FdoProviderDatastoreType_File:
                printf(" ......> Found: 'File' (expected result)\n");
                break;
            case FdoProviderDatastoreType_WebServer:
                printf(" ......> Found: 'Web Server' (unexpected result)\n");
                throw FdoException::Create(L"Unexpected provider type 'Web Server' when expecting 'File'");
                break;
            case FdoProviderDatastoreType_Unknown:
                printf(" ......> Found: 'Unknown' (unexpected result)\n");
                throw FdoException::Create(L"Unexpected provider type 'Unknown' when expecting 'File'");
                break;

        }



        printf(" ...> Checking for list of dependent files\n");
        FdoPtr<FdoStringCollection> stringCollection = connectionInfo->GetDependentFileNames();
        if (stringCollection == NULL)
            throw FdoException::Create(L"Unexpected empty list of dependent files");

        FdoInt32 stringCollectionCount = stringCollection->GetCount();
        if (stringCollectionCount > 0) {

            for (FdoInt32 index = 0; index < stringCollectionCount; index++) {

                FdoPtr<FdoStringElement> stringCollectionElement = stringCollection->GetItem(0);
                FdoStringP dependentFileName = stringCollectionElement->GetString();
                printf(" ......> Found: '%ls'\n", (FdoString *) dependentFileName);

            }

        }
        else
            throw FdoException::Create(L"Unexpected empty list of dependent files");

	    connection->Close();

    }
	catch ( FdoException *e ) 
	{
        connection->Close();
        printf( "Exception: %ls\n", e->GetExceptionMessage() );
	}
}

FdoIConnection *ConnectionInfoTest::CreateDb() 
{
    FdoIConnection *connection;

#ifdef _WIN32
	wchar_t fullpath[1024];
	_wfullpath(fullpath, CI_TEST_FILE, 1024);
#else
	char cpath[1024];
	char cfullpath[1024];
	wcstombs(cpath, CI_TEST_FILE, 1024);
	realpath(cpath, cfullpath);
	wchar_t fullpath[1024];
	mbstowcs(fullpath, cfullpath, 1024);
#endif

    size_t len = wcstombs(NULL, CI_TEST_FILE, 0);
	char *mbsPath = new char[len+1];
	wcstombs(mbsPath, CI_TEST_FILE, len+1);

#ifdef _WIN32    
	SetFileAttributes(mbsPath, FILE_ATTRIBUTE_NORMAL);
	DeleteFile(mbsPath);
#else
	unlink(mbsPath);
#endif

	delete[] mbsPath;
    FdoPtr<IConnectionManager> manager = FdoFeatureAccessManager::GetConnectionManager ();
    connection = manager->CreateConnection (L"OSGeo.SDF.3.3");

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
