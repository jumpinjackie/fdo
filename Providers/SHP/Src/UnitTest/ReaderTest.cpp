// Copyright (C) 2004-2009  Autodesk, Inc.
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

#include "Pch.h"
#include "ReaderTest.h"
#include "ShpTests.h"
#include "ShpProvider.h"
#include "FdoCommonFile.h"
#include "FdoCommonSchemaUtil.h"
#include <ctime>
#include <cppunit/extensions/HelperMacros.h>
#ifndef _WIN32
#include <unistd.h>
#endif
#define LOCATION L"..\\..\\TestData\\TestingReader\\"

CPPUNIT_TEST_SUITE_REGISTRATION( ReaderTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( ReaderTest, "ReaderTest");

ReaderTest::ReaderTest(void)
{
    
}

ReaderTest::~ReaderTest(void)
{
}

void ReaderTest::setUp ()
{
    Prepare();
}

void ReaderTest::tearDown ()
{
    Done();
}

void ReaderTest::TestDataReaderWithIndex ()
{
    // Warning: SDF provider doesn't respect the order set in select->GetPropertyNames().
    // The order is fixed: SpatialExtents() -> Count()

    CPPUNIT_ASSERT(SupportsFunction(FDO_FUNCTION_COUNT));

    FdoPtr<FdoISelectAggregates> select = (FdoISelectAggregates*)m_connection->CreateCommand (FdoCommandType_SelectAggregates);
    select->SetFeatureClassName (L"IndexAccess");

    FdoPtr<FdoIdentifierCollection> identifiers = select->GetPropertyNames();
    FdoPtr<FdoIdentifier> identifier;

    identifier = FdoComputedIdentifier::Create(L"count_int32", FdoPtr<FdoExpression>(FdoExpression::Parse(L"Count(int32)")));
    identifiers->Add(identifier);

    bool bSupportSE = false; //= SupportsFunction(FDO_FUNCTION_SPATIALEXTENTS);
    if (bSupportSE)
    {
        identifier = FdoComputedIdentifier::Create(L"SpatialExtents_geometry", FdoPtr<FdoExpression>(FdoExpression::Parse(L"SpatialExtents(geometry)")));
        identifiers->Add(identifier);
    }

    FdoPtr<FdoIDataReader> reader = select->Execute();
    while( reader->ReadNext() )
    {
        if (bSupportSE)
        {
            CPPUNIT_ASSERT(reader->GetPropertyCount() == 2);

            CPPUNIT_ASSERT(reader->GetInt64(L"count_int32") == reader->GetInt64(1));

            CPPUNIT_ASSERT(reader->GetPropertyType(L"SpatialExtents_geometry") == reader->GetPropertyType(0));
            CPPUNIT_ASSERT(reader->GetPropertyType(L"count_int32")             == reader->GetPropertyType(1));
        }
        else
        {
            CPPUNIT_ASSERT(reader->GetPropertyCount() == 1);
            CPPUNIT_ASSERT(reader->GetInt64(L"count_int32") == reader->GetInt64(0));
        }
    }
}

void ReaderTest::TestFeatureReaderWithIndex ()
{
    // NOTE: the order of properties are arranged differently with different provider implementation. 
    // For Oracle: Identity property -> the left properties are ordered by the sequence they are added to class definition.
    // IndexAccess: ID -> boolean -> byte -> datetime -> double ->geometry -> int16 -> int32 -> int64 -> single -> string
    // IndexAccessChild: datetime2-> double2 -> geometry2 -> number

    // Test 1 - select base class
    {
        FdoPtr<FdoISelect> select1 = (FdoISelect*)m_connection->CreateCommand (FdoCommandType_Select);
        select1->SetFeatureClassName (L"IndexAccess");
        FdoPtr<FdoIFeatureReader> reader1 = select1->Execute();

        while( reader1->ReadNext() )
        {
            CPPUNIT_ASSERT(reader1->GetPropertyIndex(L"FeatId") == 0);
            CPPUNIT_ASSERT(reader1->GetInt32(L"FeatId") == reader1->GetInt32(0));

            CPPUNIT_ASSERT(reader1->GetPropertyIndex(L"boolean") == 1);
            CPPUNIT_ASSERT(reader1->GetBoolean(L"boolean") == true);
            CPPUNIT_ASSERT(reader1->GetBoolean(1)          == true);

            //CPPUNIT_ASSERT(reader1->GetPropertyIndex(L"byte") == 2);
            //CPPUNIT_ASSERT(reader1->GetByte(L"byte") == 1);
            //CPPUNIT_ASSERT(reader1->GetByte(2)       == 1);

            CPPUNIT_ASSERT(reader1->GetPropertyIndex(L"datetime") == 2);
            FdoDateTime dt1 = reader1->GetDateTime(L"datetime");
            //CPPUNIT_ASSERT(dt1.year == 2009 && dt1.month  == 9 && dt1.day     == 9 && 
            //               dt1.hour == 5    && dt1.minute == 5 && dt1.seconds == 5);
            FdoDateTime dt2 = reader1->GetDateTime(2);
            //CPPUNIT_ASSERT(dt2.year == 2009 && dt2.month  == 9 && dt2.day     == 9 && 
            //               dt2.hour == 5    && dt2.minute == 5 && dt2.seconds == 5);

            //CPPUNIT_ASSERT(reader1->GetDouble(L"double") == 0.123456789);
            //CPPUNIT_ASSERT(reader1->GetDouble(4)         == 0.123456789);

            //CPPUNIT_ASSERT(reader1->GetInt16(L"int16") == 2);
            //CPPUNIT_ASSERT(reader1->GetInt16(5)        == 2);

            CPPUNIT_ASSERT(reader1->GetPropertyIndex(L"int32") == 3);
            CPPUNIT_ASSERT(reader1->GetInt32(L"int32") == 3);
            CPPUNIT_ASSERT(reader1->GetInt32(3)        == 3);

            //CPPUNIT_ASSERT(reader1->GetInt64(L"int64") == 4);
            //CPPUNIT_ASSERT(reader1->GetInt64(7)        == 4);

            //CPPUNIT_ASSERT(reader1->GetSingle(L"single") == 111.2345f);
            //CPPUNIT_ASSERT(reader1->GetSingle(8)         == 111.2345f);

            CPPUNIT_ASSERT(reader1->GetPropertyIndex(L"string") == 4);
            CPPUNIT_ASSERT(wcscmp(reader1->GetString(L"string"), L"string value") == 0);
            CPPUNIT_ASSERT(wcscmp(reader1->GetString(4)       , L"string value") == 0);

            //FdoPtr<FdoLOBValue> blobValue1 = reader1->GetLOB(L"blob");
            //FdoPtr<FdoLOBValue> blobValue2 = reader1->GetLOB(11);

            //FdoPtr<FdoLOBValue> blobValue3 = reader1->GetLOB(L"clob");
            //FdoPtr<FdoLOBValue> blobValue4 = reader1->GetLOB(12);

            CPPUNIT_ASSERT(reader1->GetPropertyIndex(L"Geometry") == 5);
            FdoPtr<FdoByteArray> geomValue1 = reader1->GetGeometry(L"Geometry");
            FdoPtr<FdoByteArray> geomValue2 = reader1->GetGeometry(5);
            //CPPUNIT_ASSERT(geomValue1->GetCount() == geomValue2->GetCount());
            //for (int i = 0; i < geomValue1->GetCount(); i++ )
            //{
            //    CPPUNIT_ASSERT(geomValue1->operator [](i) == geomValue2->operator [](i));
            //}

            break;
        }

        reader1->Close();
    }

    // Test 2 - select base class with selected properties
    {
        FdoPtr<FdoISelect> select2 = (FdoISelect*)m_connection->CreateCommand (FdoCommandType_Select);
        select2->SetFeatureClassName (L"IndexAccess");
        FdoPtr<FdoIdentifierCollection> identifiers = select2->GetPropertyNames();
        FdoPtr<FdoIdentifier> identifier;

        identifier = FdoIdentifier::Create(L"int32");
        identifiers->Add(identifier);
        identifier = FdoIdentifier::Create(L"string");
        identifiers->Add(identifier);

        FdoPtr<FdoIFeatureReader> reader2 = select2->Execute();
        CPPUNIT_ASSERT( reader2->ReadNext() );
        {
            CPPUNIT_ASSERT(reader2->GetInt32(L"int32") == reader2->GetInt32(0));
            CPPUNIT_ASSERT(wcscmp(reader2->GetString(L"string"), reader2->GetString(1)) == 0);
        }

        reader2->Close();
    }

    //// Test 3 - select derived class
    //{
    //    FdoPtr<FdoISelect> select3 = (FdoISelect*)m_connection->CreateCommand (FdoCommandType_Select);
    //    select3->SetFeatureClassName (L"IndexAccessChild");
    //    FdoPtr<FdoIFeatureReader> reader3 = select3->Execute();

    //    CPPUNIT_ASSERT( reader3->ReadNext() );
    //    {
    //        CPPUNIT_ASSERT(reader3->GetInt32(L"ID") == reader3->GetInt32(0));

    //        CPPUNIT_ASSERT(reader3->IsNull(L"boolean") == reader3->IsNull(1));
    //        CPPUNIT_ASSERT(reader3->IsNull(L"byte")    == reader3->IsNull(2));
    //        CPPUNIT_ASSERT(reader3->IsNull(L"datetime") == reader3->IsNull(3));

    //        CPPUNIT_ASSERT(reader3->IsNull(L"single") == reader3->IsNull(8));
    //        CPPUNIT_ASSERT(reader3->IsNull(L"string") == reader3->IsNull(9));

    //        CPPUNIT_ASSERT(reader3->IsNull(L"double") == reader3->IsNull(4));

    //        CPPUNIT_ASSERT(reader3->IsNull(L"int16") == reader3->IsNull(5));
    //        CPPUNIT_ASSERT(reader3->IsNull(L"int32") == reader3->IsNull(6));
    //        CPPUNIT_ASSERT(reader3->IsNull(L"int64") == reader3->IsNull(7));

    //        FdoPtr<FdoByteArray> geomValue1 = reader3->GetGeometry(L"geometry");
    //        FdoPtr<FdoByteArray> geomValue2 = reader3->GetGeometry(10);
    //        CPPUNIT_ASSERT(geomValue1->GetCount() == geomValue2->GetCount());
    //        for (int i = 0; i < geomValue1->GetCount(); i++ )
    //        {
    //            CPPUNIT_ASSERT(geomValue1->operator [](i) == geomValue2->operator [](i));
    //        }

    //        //FdoPtr<FdoLOBValue> blobValue1 = reader3->GetLOB(L"blob");
    //        //FdoPtr<FdoLOBValue> blobValue2 = reader3->GetLOB(11);

    //        //FdoPtr<FdoLOBValue> blobValue3 = reader3->GetLOB(L"clob");
    //        //FdoPtr<FdoLOBValue> blobValue4 = reader3->GetLOB(12);

    //        // properties from derived class
    //        CPPUNIT_ASSERT(reader3->GetInt32(L"number") == reader3->GetInt32(11));

    //        FdoDateTime dt3 = reader3->GetDateTime(L"datetime2");
    //        FdoDateTime dt4 = reader3->GetDateTime(12);
    //        CPPUNIT_ASSERT(dt3.year == dt4.year && dt3.month == dt4.month && dt3.day == dt4.day
    //            && dt3.hour == dt4.hour && dt3.minute == dt4.minute && dt3.seconds == dt4.seconds);

    //        CPPUNIT_ASSERT(reader3->GetDouble(L"double2") == reader3->GetDouble(13));

    //        // NOTE: SqlLite Provider puts the default geometry property on the second column. 
    //        FdoPtr<FdoByteArray> geomValue3 = reader3->GetGeometry(L"geometry2");
    //        FdoPtr<FdoByteArray> geomValue4 = reader3->GetGeometry(14);
    //        CPPUNIT_ASSERT(geomValue3->GetCount() == geomValue4->GetCount());
    //        for (int i = 0; i < geomValue3->GetCount(); i++ )
    //        {
    //            CPPUNIT_ASSERT(geomValue3->operator [](i) == geomValue4->operator [](i));
    //        }
    //    }

    //    reader3->Close();
    //}

    //// Test 4 - select derived class with selected properties
    //{
    //    FdoPtr<FdoISelect> select4 = (FdoISelect*)m_connection->CreateCommand (FdoCommandType_Select);
    //    select4->SetFeatureClassName (L"IndexAccessChild");
    //    FdoPtr<FdoIdentifierCollection> identifiers = select4->GetPropertyNames();
    //    FdoPtr<FdoIdentifier> identifier;

    //    identifier = FdoIdentifier::Create(L"number");
    //    identifiers->Add(identifier);
    //    identifier = FdoIdentifier::Create(L"byte");
    //    identifiers->Add(identifier);
    //    identifier = FdoIdentifier::Create(L"double2");
    //    identifiers->Add(identifier);

    //    FdoPtr<FdoIFeatureReader> reader4 = select4->Execute();
    //    CPPUNIT_ASSERT( reader4->ReadNext() );
    //    {
    //        CPPUNIT_ASSERT(reader4->GetByte(L"byte")      == reader4->GetByte(0)  );
    //        CPPUNIT_ASSERT(reader4->GetInt32(L"number")   == reader4->GetInt32(1) );
    //        CPPUNIT_ASSERT(reader4->GetDouble(L"double2") == reader4->GetDouble(2));
    //    }

    //    reader4->Close();
    //}
}

//void ReaderTest::TestSqlDataReaderWithIndex ()
//{
//    // Test 1 - execute sql on table with primary key
//    {
//        FdoPtr<FdoISQLCommand> select1 = (FdoISQLCommand*)m_connection->CreateCommand (FdoCommandType_SQLCommand);
//        select1->SetSQLStatement(L"select * from IndexAccessSQL");
//        FdoPtr<FdoISQLDataReader> reader1 = select1->ExecuteReader();
//
//        while( reader1->ReadNext() )
//        {
//            CPPUNIT_ASSERT(reader1->GetInt64(L"ID") == 1);
//            CPPUNIT_ASSERT(reader1->GetInt64(0)     == 1);
//
//            CPPUNIT_ASSERT(reader1->GetBoolean(L"boolean") == true);
//            CPPUNIT_ASSERT(reader1->GetBoolean(1)          == true);
//
//            CPPUNIT_ASSERT(reader1->GetByte(L"byte") == 2);
//            CPPUNIT_ASSERT(reader1->GetByte(2)       == 2);
//
//            CPPUNIT_ASSERT(reader1->IsNull(L"datetime") == reader1->IsNull(3));
//
//            CPPUNIT_ASSERT(reader1->GetInt16(L"int16") == 3);
//            CPPUNIT_ASSERT(reader1->GetInt16(5)        == 3);
//
//            CPPUNIT_ASSERT(reader1->GetInt32(L"int32") == 4);
//            CPPUNIT_ASSERT(reader1->GetInt32(6)        == 4);
//
//            CPPUNIT_ASSERT(reader1->GetInt64(L"int64") == 5);
//            CPPUNIT_ASSERT(reader1->GetInt64(7)        == 5);
//
//            CPPUNIT_ASSERT(reader1->GetDouble(L"double") == 0.123456789);
//            CPPUNIT_ASSERT(reader1->GetDouble(4)         == 0.123456789);
//
//            CPPUNIT_ASSERT(reader1->IsNull(L"single") == reader1->IsNull(8));
//
//            CPPUNIT_ASSERT(wcscmp(reader1->GetString(L"string"), L"string value") == 0);
//            CPPUNIT_ASSERT(wcscmp(reader1->GetString(9)       ,  L"string value") == 0);
//
//            //// NOTE: SqlLite Provider puts the default geometry property on the second column. 
//            //FdoPtr<FdoByteArray> geomValue1 = reader1->GetGeometry(L"geometry");
//            //FdoPtr<FdoByteArray> geomValue2 = reader1->GetGeometry(10);
//            //CPPUNIT_ASSERT(geomValue1->GetCount() == geomValue2->GetCount());
//            //for (int i = 0; i < geomValue1->GetCount(); i++ )
//            //{
//            //    CPPUNIT_ASSERT(geomValue1->operator [](i) == geomValue2->operator [](i));
//            //}
//        }
//
//        reader1->Close();
//    }
//}

void ReaderTest::TestReaderMatchClassDefinition()
{
    // Test 1 - FeatureReader
    {
        FdoPtr<FdoISelect> select1 = (FdoISelect*)m_connection->CreateCommand (FdoCommandType_Select);
        select1->SetFeatureClassName (L"IndexAccess");
        FdoPtr<FdoIFeatureReader> reader1 = select1->Execute();
        CPPUNIT_ASSERT(reader1->ReadNext());

        FdoPtr<FdoClassDefinition> classDef = reader1->GetClassDefinition();
        CPPUNIT_ASSERT(classDef != NULL);

        FdoPtr<FdoPropertyDefinitionCollection> properties = classDef->GetProperties();
        for (int i = 0; i < properties->GetCount(); i++)
        {
            FdoPtr<FdoPropertyDefinition> prop = properties->GetItem(i);
            CPPUNIT_ASSERT(prop != NULL);

            FdoStringP propName = prop->GetName();
            CPPUNIT_ASSERT(wcscmp(propName, reader1->GetPropertyName(i)) == 0);
            CPPUNIT_ASSERT(reader1->GetPropertyIndex(propName) == i);
        }
    }

    // Test 2 - FeatureReader with selected properties
    {
        FdoPtr<FdoISelect> select2 = (FdoISelect*)m_connection->CreateCommand (FdoCommandType_Select);
        select2->SetFeatureClassName (L"IndexAccess");

        FdoPtr<FdoIdentifierCollection> identifiers = select2->GetPropertyNames();
        FdoPtr<FdoIdentifier> identifier;

        // NOTE: SHP provider renames identifier name from "ID" to "FeatId".
        identifier = FdoIdentifier::Create(L"FeatId");
        identifiers->Add(identifier);
        identifier = FdoIdentifier::Create(L"string");
        identifiers->Add(identifier);
        identifier = FdoIdentifier::Create(L"int32");
        identifiers->Add(identifier);
        identifier = FdoIdentifier::Create(L"boolean");
        identifiers->Add(identifier);

        FdoPtr<FdoIFeatureReader> reader2 = select2->Execute();
        CPPUNIT_ASSERT(reader2->ReadNext());

        FdoPtr<FdoClassDefinition> classDef = reader2->GetClassDefinition();
        CPPUNIT_ASSERT(classDef != NULL);

        FdoPtr<FdoPropertyDefinitionCollection> properties = classDef->GetProperties();
        for (int i = 0; i < properties->GetCount(); i++)
        {
            FdoPtr<FdoPropertyDefinition> prop = properties->GetItem(i);
            CPPUNIT_ASSERT(prop != NULL);

            FdoStringP propName = prop->GetName();
            CPPUNIT_ASSERT(wcscmp(propName, reader2->GetPropertyName(i)) == 0);
            CPPUNIT_ASSERT(reader2->GetPropertyIndex(propName) == i);
        }
    }

    // Test 3 - DataReader
    {
        CPPUNIT_ASSERT(SupportsFunction(FDO_FUNCTION_COUNT));

        FdoPtr<FdoISelectAggregates> select3 = (FdoISelectAggregates*)m_connection->CreateCommand (FdoCommandType_SelectAggregates);
        select3->SetFeatureClassName (L"IndexAccess");

        FdoPtr<FdoIdentifierCollection> identifiers = select3->GetPropertyNames();
        FdoPtr<FdoIdentifier> identifier;

        //identifier = FdoComputedIdentifier::Create(L"ID", FdoPtr<FdoExpression>(FdoExpression::Parse(L"Count(ID)")));
        //identifiers->Add(identifier);

        identifier = FdoComputedIdentifier::Create(L"count_int32", FdoPtr<FdoExpression>(FdoExpression::Parse(L"Count(int32)")));
        identifiers->Add(identifier);

        bool bSupportSE = SupportsFunction(FDO_FUNCTION_SPATIALEXTENTS);
        if (bSupportSE)
        {
            identifier = FdoComputedIdentifier::Create(L"SpatialExtents_geometry", FdoPtr<FdoExpression>(FdoExpression::Parse(L"SpatialExtents(geometry)")));
            identifiers->Add(identifier);
        }

        FdoPtr<FdoIDataReader> reader3 = select3->Execute();
        CPPUNIT_ASSERT(reader3->ReadNext());

        for (int i = 0; i < reader3->GetPropertyCount(); i++)
        {
            FdoStringP propName = reader3->GetPropertyName(i);
            CPPUNIT_ASSERT(reader3->GetPropertyIndex(propName) == i);
        }
    }
}

void ReaderTest::Prepare()
{
    PrepareConnection();
    PrepareSchema();
    PrepareData();
}

void ReaderTest::PrepareConnection()
{
    try
    {
        if (FdoCommonFile::FileExists (LOCATION))
            FdoCommonFile::RmDir(LOCATION);

        if (!FdoCommonFile::FileExists (LOCATION))
            FdoCommonFile::MkDir (LOCATION);
        m_connection = ShpTests::GetConnection ();
        ShpTests::sLocation = LOCATION;
        m_connection->SetConnectionString (L"DefaultFileLocation=" LOCATION);
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == m_connection->Open ());

        // Extra location
        if (!FdoCommonFile::FileExists (LOCATION))
            FdoCommonFile::MkDir (LOCATION);
    }
    catch (FdoException* ge)
    {
        TestCommonFail (ge);
    }
}

void ReaderTest::PrepareSchema()
{
    if (m_connection == NULL)
    {
        CPPUNIT_ASSERT(false);
        return;
    }

    // Delete "Default" schema
    try
    {
        FdoPtr<FdoIDescribeSchema> describeschema = static_cast<FdoIDescribeSchema*>(m_connection->CreateCommand(FdoCommandType_DescribeSchema));
        describeschema->SetSchemaName(L"Default");
        FdoPtr<FdoFeatureSchemaCollection> schemas = describeschema->Execute();
        if (schemas->GetCount() == 1)
        {
            // delete all features
            FdoPtr<FdoIDelete> del = (FdoIDelete*)m_connection->CreateCommand (FdoCommandType_Delete);
            del->SetFeatureClassName (L"IndexAccess");
            FdoInt32 n = del->Execute ();

            FdoPtr<FdoFeatureSchema> schema = schemas->GetItem(L"Default");
            CPPUNIT_ASSERT(schema != NULL);
            schema->Delete();

            FdoPtr<FdoIApplySchema> applyschema = static_cast<FdoIApplySchema*>(m_connection->CreateCommand(FdoCommandType_ApplySchema));
            applyschema->SetFeatureSchema(schema);
            applyschema->Execute();
        }
    }
    catch (FdoException* exc)
    {
        exc->Release();
        CPPUNIT_FAIL("\nPrepareSchema failed: ");
    }
    catch (...)
    {
        CPPUNIT_FAIL("\nPrepareSchema failed: ");
    }

    // 1. prepare schema
    FdoPtr<FdoFeatureSchema> schema = FdoFeatureSchema::Create(L"Default",L"Schema of the Spatial Data File format");

    // class 1
    FdoPtr<FdoFeatureClass> clas = FdoFeatureClass::Create(L"IndexAccess",L"");
    FdoPtr<FdoClassCollection>(schema->GetClasses())->Add(clas);
    FdoPtr<FdoPropertyDefinitionCollection> properties = clas->GetProperties();

    // indentifier property
    FdoPtr<FdoDataPropertyDefinition> indentifier = FdoDataPropertyDefinition::Create(L"ID", L"The autogenerated sequence ID of the object");
    indentifier->SetDataType(FdoDataType_Int32);
    indentifier->SetNullable(false);
    indentifier->SetIsAutoGenerated(true);
    properties->Add(indentifier);
    FdoDataPropertiesP(clas->GetIdentityProperties())->Add(indentifier);

    // boolean property
    FdoPtr<FdoDataPropertyDefinition> boolProp = FdoDataPropertyDefinition::Create(L"boolean", L"boolean property");
    boolProp->SetDataType(FdoDataType_Boolean);
    boolProp->SetDefaultValue(L"1");
    boolProp->SetNullable(true);
    properties->Add(boolProp);

    //// byte property
    //FdoPtr<FdoDataPropertyDefinition> byteProp = FdoDataPropertyDefinition::Create(L"byte", L"byte property");
    //byteProp->SetDataType(FdoDataType_Byte);
    //byteProp->SetNullable(true);
    //properties->Add(byteProp);

    // date time property
    FdoPtr<FdoDataPropertyDefinition> datetimeProp = FdoDataPropertyDefinition::Create(L"datetime", L"datetime property");
    datetimeProp->SetDataType(FdoDataType_DateTime);
    datetimeProp->SetNullable(true);
    properties->Add(datetimeProp);

    //// double property
    //FdoPtr<FdoDataPropertyDefinition> doubleProp = FdoDataPropertyDefinition::Create(L"double", L"double property");
    //doubleProp->SetDataType(FdoDataType_Double);
    //doubleProp->SetNullable(true);
    //properties->Add(doubleProp);

    //// int16 property
    //FdoPtr<FdoDataPropertyDefinition> int16Prop = FdoDataPropertyDefinition::Create(L"int16", L"int16 property");
    //int16Prop->SetDataType(FdoDataType_Int16);
    //int16Prop->SetNullable(true);
    //properties->Add(int16Prop);

    // int32 property
    FdoPtr<FdoDataPropertyDefinition> int32Prop = FdoDataPropertyDefinition::Create(L"int32", L"int32 property");
    int32Prop->SetDataType(FdoDataType_Int32);
    int32Prop->SetNullable(true);
    properties->Add(int32Prop);

    //// int64 property
    //FdoPtr<FdoDataPropertyDefinition> int64Prop = FdoDataPropertyDefinition::Create(L"int64", L"int64 property");
    //int64Prop->SetDataType(FdoDataType_Int64);
    //int64Prop->SetNullable(true);
    //properties->Add(int64Prop);

    //// single property
    //FdoPtr<FdoDataPropertyDefinition> singleProp = FdoDataPropertyDefinition::Create(L"single", L"single property");
    //singleProp->SetDataType(FdoDataType_Single);
    //singleProp->SetNullable(true);
    //properties->Add(singleProp);

    // string property
    FdoPtr<FdoDataPropertyDefinition> stringProp = FdoDataPropertyDefinition::Create(L"string", L"string property");
    stringProp->SetDataType(FdoDataType_String);
    stringProp->SetLength(50);
    stringProp->SetNullable(true);
    properties->Add(stringProp);

    // geometry property
    FdoPtr<FdoGeometricPropertyDefinition> geometryProp = FdoGeometricPropertyDefinition::Create( L"Geometry", L"geometry property");
    geometryProp->SetGeometryTypes(FdoGeometricType_Point); // Point, Line, Polygon
    properties->Add(geometryProp);

    clas->SetGeometryProperty(geometryProp);

    //// blob property
    //FdoPtr<FdoDataPropertyDefinition> blobProp = FdoDataPropertyDefinition::Create(L"blob", L"blob property");
    //blobProp->SetDataType(FdoDataType_BLOB);
    //blobProp->SetNullable(true);
    //properties->Add(blobProp);

    //// clob property
    //FdoPtr<FdoDataPropertyDefinition> clobProp = FdoDataPropertyDefinition::Create(L"clob", L"clob property");
    //clobProp->SetDataType(FdoDataType_CLOB);
    //clobProp->SetNullable(true);
    //properties->Add(clobProp);

    //// Class 2
    //FdoPtr<FdoFeatureClass> clas2 = FdoFeatureClass::Create(L"IndexAccessChild",L"");    
    //clas2->SetBaseClass(clas);
    //properties = clas2->GetProperties();

    //FdoPtr<FdoDataPropertyDefinition> numberProp = FdoDataPropertyDefinition::Create(L"number", L"A number property");
    //numberProp->SetDataType(FdoDataType_Int32);
    //numberProp->SetNullable(false);
    //properties->Add(numberProp);

    //FdoPtr<FdoDataPropertyDefinition> datetimeProp2 = FdoDataPropertyDefinition::Create(L"datetime2", L"A number property");
    //datetimeProp2->SetDataType(FdoDataType_DateTime);
    //datetimeProp2->SetNullable(false);
    //properties->Add(datetimeProp2);

    //FdoPtr<FdoDataPropertyDefinition> doubleProp2 = FdoDataPropertyDefinition::Create(L"double2", L"A double number property(1)");
    //doubleProp2->SetDataType(FdoDataType_Double);
    //doubleProp2->SetNullable(false);
    //properties->Add(doubleProp2);

    //FdoPtr<FdoGeometricPropertyDefinition> geometryProp2 = FdoGeometricPropertyDefinition::Create( L"geometry2", L"geometry property");
    //geometryProp2->SetGeometryTypes(FdoGeometricType_Curve); // Point, Line, Polygon
    //properties->Add(geometryProp2);

    //FdoPtr<FdoClassCollection>(schema->GetClasses())->Add(clas2);

    // 2. apply schema
    if (m_connection->GetConnectionState() != FdoConnectionState_Open)
    {
        m_connection->Open();
    }

    try
    {
        FdoPtr<FdoIApplySchema> applyschema = static_cast<FdoIApplySchema*>(m_connection->CreateCommand(FdoCommandType_ApplySchema));
        applyschema->SetFeatureSchema(schema);
        applyschema->Execute();
    }
    catch (FdoException* exc)
    {
        exc->Release();
        CPPUNIT_FAIL("\nPrepareSchema failed: ");
    }
    catch (...)
    {
        CPPUNIT_FAIL("\nPrepareSchema failed: ");
    }

    //// Class 3
    //try
    //{
    //    // check the default value
    //    FdoPtr<FdoISQLCommand> insertCmd = static_cast<FdoISQLCommand*>(m_connection->CreateCommand(FdoCommandType_SQLCommand));
    //    insertCmd->SetSQLStatement(L"create table IndexAccessSQL \
    //                                (ID decimal(10,0) NOT NULL, \
    //                                 boolean decimal(1,0), BYTE decimal(3,0), datetime DATE, \
    //                                 double decimal(10,9), int16 decimal(5,0), int32 decimal(10,0), \
    //                                 int64 decimal(10,0), single decimal, string VARCHAR(255))");

    //    FdoInt32 count = insertCmd->ExecuteNonQuery();
    //    CPPUNIT_ASSERT(count == 0);
    //}
    //catch(FdoException* exc)
    //{
    //    exc->Release();
    //    CPPUNIT_FAIL("\nUnexpected exception: ");
    //}

    m_connection->Close();
    m_connection->Open();
}

void ReaderTest::PrepareData()
{
    if (m_connection->GetConnectionState() != FdoConnectionState_Open)
    {
        m_connection->Open();
    }

    {
        FdoPtr<FdoIInsert> insCmd = static_cast<FdoIInsert*>(m_connection->CreateCommand(FdoCommandType_Insert));

        insCmd->SetFeatureClassName(L"IndexAccess");
        FdoPtr<FdoPropertyValueCollection> vals = insCmd->GetPropertyValues();
        FdoPtr<FdoPropertyValue> propIns;

        // add boolean value
        propIns = FdoPropertyValue::Create(L"boolean", FdoPtr<FdoBooleanValue>(FdoBooleanValue::Create(true)));
        vals->Add(propIns);

        //propIns = FdoPropertyValue::Create(L"byte", FdoPtr<FdoByteValue>(FdoByteValue::Create(1)));
        //vals->Add(propIns);

        propIns = FdoPropertyValue::Create(L"datetime", FdoPtr<FdoDateTimeValue>(FdoDateTimeValue::Create(FdoDateTime(2009,9,9,5,5,5))));
        vals->Add(propIns);

        //propIns = FdoPropertyValue::Create(L"double", FdoPtr<FdoDoubleValue>(FdoDoubleValue::Create(0.123456789)));
        //vals->Add(propIns);

        //propIns = FdoPropertyValue::Create(L"int16", FdoPtr<FdoInt16Value>(FdoInt16Value::Create(2)));
        //vals->Add(propIns);

        propIns = FdoPropertyValue::Create(L"int32", FdoPtr<FdoInt32Value>(FdoInt32Value::Create(3)));
        vals->Add(propIns);

        //propIns = FdoPropertyValue::Create(L"int64", FdoPtr<FdoInt64Value>(FdoInt64Value::Create(4)));
        //vals->Add(propIns);

        //propIns = FdoPropertyValue::Create(L"single", FdoPtr<FdoSingleValue>(FdoSingleValue::Create(111.2345f)));
        //vals->Add(propIns);

        propIns = FdoPropertyValue::Create(L"string", FdoPtr<FdoStringValue>(FdoStringValue::Create(L"string value")));
        vals->Add(propIns);

        //FdoByte lobBytes[4] = { 'a', 'b', 'c', 0 };
        //FdoPtr<FdoByteArray> lobValue = FdoByteArray::Create( lobBytes, 4 );
        //propIns = FdoPropertyValue::Create(L"blob", FdoPtr<FdoBLOBValue>(FdoBLOBValue::Create(lobValue)));
        //vals->Add(propIns);

        //propIns = FdoPropertyValue::Create(L"clob", FdoPtr<FdoCLOBValue>(FdoCLOBValue::Create(lobValue)));
        //vals->Add(propIns);

        FdoPtr<FdoGeometryValue> geometry = FdoGeometryValue::Create ();
        geometry->SetNullValue ();
        propIns = FdoPropertyValue::Create(L"Geometry", geometry);
        vals->Add(propIns);

        //FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
        //double coords[] = { 1, 1, 10, 1, 10, 10, 1,10 }; 
        //FdoPtr<FdoILinearRing> outer = gf->CreateLinearRing(0, 8, coords);
        //FdoPtr<FdoIPolygon> poly = gf->CreatePolygon(outer, NULL);
        //FdoPtr<FdoByteArray> polyfgf = gf->GetFgf(poly);
        //propIns = FdoPropertyValue::Create(L"geometry", FdoPtr<FdoGeometryValue>(FdoGeometryValue::Create(polyfgf)));
        //vals->Add(propIns);

        try
        {
            // check the default value

            FdoPtr<FdoIFeatureReader> rdr = insCmd->Execute();
            CPPUNIT_ASSERT(rdr->ReadNext());

            rdr->Close();
        }
        catch(FdoException* exc)
        {
            exc->Release();
            CPPUNIT_FAIL("\nUnexpected exception: ");
        }
    }

    //{
    //    FdoPtr<FdoIInsert> insCmd = static_cast<FdoIInsert*>(m_connection->CreateCommand(FdoCommandType_Insert));
    //    insCmd->SetFeatureClassName(L"IndexAccessChild");
    //    FdoPtr<FdoPropertyValueCollection> vals = insCmd->GetPropertyValues();
    //    FdoPtr<FdoPropertyValue> propIns;

    //    vals->Clear();

    //    propIns = FdoPropertyValue::Create(L"byte", FdoPtr<FdoByteValue>(FdoByteValue::Create(1)));
    //    vals->Add(propIns);

    //    propIns = FdoPropertyValue::Create(L"number", FdoPtr<FdoInt32Value>(FdoInt32Value::Create(10)));
    //    vals->Add(propIns);

    //    propIns = FdoPropertyValue::Create(L"datetime2", FdoPtr<FdoDateTimeValue>(FdoDateTimeValue::Create(FdoDateTime(2009,9,9,5,5,5))));
    //    vals->Add(propIns);

    //    propIns = FdoPropertyValue::Create(L"double2", FdoPtr<FdoDoubleValue>(FdoDoubleValue::Create(0.123456789)));

    //    vals->Add(propIns);

    //    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    //    double coords[] = { 1, 1, 10, 1, 10, 10, 1,10 }; 
    //    FdoPtr<FdoILinearRing> outer = gf->CreateLinearRing(0, 8, coords);
    //    FdoPtr<FdoIPolygon> poly = gf->CreatePolygon(outer, NULL);
    //    FdoPtr<FdoByteArray> polyfgf = gf->GetFgf(poly);

    //    propIns = FdoPropertyValue::Create(L"geometry", FdoPtr<FdoGeometryValue>(FdoGeometryValue::Create(polyfgf)));
    //    vals->Add(propIns);

    //    propIns = FdoPropertyValue::Create(L"geometry2", FdoPtr<FdoGeometryValue>(FdoGeometryValue::Create(polyfgf)));
    //    vals->Add(propIns);

    //    try
    //    {
    //        // check the default value
    //        FdoPtr<FdoIFeatureReader> rdr = insCmd->Execute();
    //        CPPUNIT_ASSERT(rdr->ReadNext());

    //        rdr->Close();
    //    }
    //    catch(FdoException* exc)
    //    {
    //        exc->Release();
    //        CPPUNIT_FAIL("\nUnexpected exception: ");
    //    }
    //}

    //{
    //    try
    //    {
    //        // check the default value
    //        FdoPtr<FdoISQLCommand> insertCmd = static_cast<FdoISQLCommand*>(m_connection->CreateCommand(FdoCommandType_SQLCommand));
    //        insertCmd->SetSQLStatement(L"insert into IndexAccessSQL (ID, boolean, byte, double, int16, int32, int64, string) \
    //                                    values (1, 1, 2, 0.123456789, 3, 4, 5, 'string value')");
    //        FdoInt32 count = insertCmd->ExecuteNonQuery();
    //        CPPUNIT_ASSERT(count == 1);
    //    }
    //    catch(FdoException* exc)
    //    {
    //        exc->Release();
    //        CPPUNIT_FAIL("\nUnexpected exception: ");
    //    }
    //}
}

bool ReaderTest::SupportsFunction(FdoString* functionName)
{
    FdoPtr<FdoIExpressionCapabilities> pCaps = m_connection->GetExpressionCapabilities();
    if (pCaps == NULL)
        return false;

    FdoPtr<FdoFunctionDefinitionCollection> pFuncColl = pCaps->GetFunctions();

    return pFuncColl != NULL && pFuncColl->Contains(functionName);
}

void ReaderTest::Done()
{
    if( m_connection != NULL )
        m_connection->Close();

    m_connection = NULL;
}
