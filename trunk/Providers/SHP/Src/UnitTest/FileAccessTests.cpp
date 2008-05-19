/*
 * 
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
#include "FileAccessTests.h"

CPPUNIT_TEST_SUITE_REGISTRATION (FileAccessTests);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (FileAccessTests, "FileAccessTests");

#include <ShapeFile.h>
#include <ShapeDBF.h>
#include <ShapeIndex.h>

#ifdef _WIN32
#define SHP_FILE L"..\\..\\TestData\\Ontario\\ontario.shp"
#define SHX_FILE L"..\\..\\TestData\\Ontario\\ontario.shx"
#define DBF_FILE L"..\\..\\TestData\\Ontario\\ontario.dbf"
#define SOURCE_SHP_FILE L"..\\..\\TestData\\Ontario\\roads.shp"
#define GENERATED_SHX_FILE L"..\\..\\TestData\\Ontario\\roads2.shx"
#define REFERENCE_SHX_FILE L"..\\..\\TestData\\Ontario\\roads.shx"
#define ONLY_SHP_FILE L"..\\..\\TestData\\World"
#define ONLY_SHX_FILE L"..\\..\\TestData\\World\\world_adm0.shx"
#define ONLY_IDX_FILE L"..\\..\\TestData\\World\\world_adm0.idx"
#define ROOT_DATA_PATH L"..\\..\\TestData\\"
#else
#define SHP_FILE L"../../TestData/Ontario/ontario.shp"
#define SHX_FILE L"../../TestData/Ontario/ontario.shx"
#define DBF_FILE L"../../TestData/Ontario/ontario.dbf"
#define SOURCE_SHP_FILE L"../../TestData/Ontario/roads.shp"
#define GENERATED_SHX_FILE L"../../TestData/Ontario/roads2.shx"
#define REFERENCE_SHX_FILE L"../../TestData/Ontario/roads.shx"
#define ONLY_SHP_FILE L"../../TestData/World"
#define ONLY_SHX_FILE L"../../TestData/World/world_adm0.shx"
#define ONLY_IDX_FILE L"../../TestData/World/world_adm0.idx"
#define ROOT_DATA_PATH L"../../TestData/"
#endif

FileAccessTests::FileAccessTests (void)
{
}

FileAccessTests::~FileAccessTests (void)
{
}

void FileAccessTests::open_shp ()
{
    ShapeFile shp (SHP_FILE);

	// general information methods
	CPPUNIT_ASSERT_MESSAGE ("file code bad", 9994 == shp.GetFileCode ());
	CPPUNIT_ASSERT_MESSAGE ("length bad", 13822 == shp.GetFileLength ());
	CPPUNIT_ASSERT_MESSAGE ("version bad", 1000 == shp.GetFileVersion ());
	CPPUNIT_ASSERT_MESSAGE ("shape type bad", ePolygonShape == shp.GetFileShapeType ());

    // bounding box methods
	CPPUNIT_ASSERT_MESSAGE ("min x bad", 196983.92187500000 == shp.GetBoundingBoxMinX ());
	CPPUNIT_ASSERT_MESSAGE ("max x bad", 1766771.6250000000 == shp.GetBoundingBoxMaxX ());
	CPPUNIT_ASSERT_MESSAGE ("min y bad", 11675054.000000000 == shp.GetBoundingBoxMinY ());
	CPPUNIT_ASSERT_MESSAGE ("max y bad", 13366321.000000000 == shp.GetBoundingBoxMaxY ());
	CPPUNIT_ASSERT_MESSAGE ("min z bad", 0.0 == shp.GetBoundingBoxMinZ ());
	CPPUNIT_ASSERT_MESSAGE ("max z bad", 0.0 == shp.GetBoundingBoxMaxZ ());
	CPPUNIT_ASSERT_MESSAGE ("min m bad", 0.0 == shp.GetBoundingBoxMinM ());
	CPPUNIT_ASSERT_MESSAGE ("max m bad", 0.0 == shp.GetBoundingBoxMaxM ());
}

void FileAccessTests::open_dbf ()
{

    ShapeDBF dbf (DBF_FILE);

    // meta data
	CPPUNIT_ASSERT_MESSAGE ("number of records bad", 1 == dbf.GetNumRecords ());
	CPPUNIT_ASSERT_MESSAGE ("record size bad", 47 == dbf.GetRecordSize ());
    ColumnInfo* info = dbf.GetColumnInfo ();
    CPPUNIT_ASSERT_MESSAGE ("wrong number of columns", 4 == info->GetNumColumns ());
    CPPUNIT_ASSERT_MESSAGE ("wrong name", 0 == wcscmp (L"AREA", info->GetColumnNameAt (0)));
    CPPUNIT_ASSERT_MESSAGE ("wrong type", kColumnDecimalType == info->GetColumnTypeAt (0));
    CPPUNIT_ASSERT_MESSAGE ("wrong column width", 12 == info->GetColumnWidthAt (0));
    //TODO: verify Scale also
    CPPUNIT_ASSERT_MESSAGE ("wrong name", 0 == wcscmp (L"PERIMETER", info->GetColumnNameAt (1)));
    CPPUNIT_ASSERT_MESSAGE ("wrong type", kColumnDecimalType == info->GetColumnTypeAt (1));
    CPPUNIT_ASSERT_MESSAGE ("wrong column width", 12 == info->GetColumnWidthAt (1));
    //TODO: verify Scale also
    CPPUNIT_ASSERT_MESSAGE ("wrong name", 0 == wcscmp (L"ONTARIO_", info->GetColumnNameAt (2)));
    CPPUNIT_ASSERT_MESSAGE ("wrong type", kColumnDecimalType == info->GetColumnTypeAt (2));
    CPPUNIT_ASSERT_MESSAGE ("wrong column width", 11 == info->GetColumnWidthAt (2));
    //TODO: verify Scale also
    CPPUNIT_ASSERT_MESSAGE ("wrong name", 0 == wcscmp (L"ONTARIO_ID", info->GetColumnNameAt (3)));
    CPPUNIT_ASSERT_MESSAGE ("wrong type", kColumnDecimalType == info->GetColumnTypeAt (3));
    CPPUNIT_ASSERT_MESSAGE ("wrong column width", 11 == info->GetColumnWidthAt (3));
    //TODO: verify Scale also
}

void FileAccessTests::open_shx ()
{
    ShapeIndex shx (SHX_FILE);

	// general information methods
	CPPUNIT_ASSERT_MESSAGE ("file code bad", 9994 == shx.GetFileCode ());
	CPPUNIT_ASSERT_MESSAGE ("length bad", 54 == shx.GetFileLength ()); // only difference
	CPPUNIT_ASSERT_MESSAGE ("version bad", 1000 == shx.GetFileVersion ());
	CPPUNIT_ASSERT_MESSAGE ("shape type bad", ePolygonShape == shx.GetFileShapeType ());
	CPPUNIT_ASSERT_MESSAGE ("number of objects bad", 1 == shx.GetNumObjects ());

    // bounding box methods
	CPPUNIT_ASSERT_MESSAGE ("min x bad", 196983.92187500000 == shx.GetBoundingBoxMinX ());
	CPPUNIT_ASSERT_MESSAGE ("max x bad", 1766771.6250000000 == shx.GetBoundingBoxMaxX ());
	CPPUNIT_ASSERT_MESSAGE ("min y bad", 11675054.000000000 == shx.GetBoundingBoxMinY ());
	CPPUNIT_ASSERT_MESSAGE ("max y bad", 13366321.000000000 == shx.GetBoundingBoxMaxY ());
	CPPUNIT_ASSERT_MESSAGE ("min z bad", 0.0 == shx.GetBoundingBoxMinZ ());
	CPPUNIT_ASSERT_MESSAGE ("max z bad", 0.0 == shx.GetBoundingBoxMaxZ ());
	CPPUNIT_ASSERT_MESSAGE ("min m bad", 0.0 == shx.GetBoundingBoxMinM ());
	CPPUNIT_ASSERT_MESSAGE ("max m bad", 0.0 == shx.GetBoundingBoxMaxM ());
}

void FileAccessTests::object ()
{
    ULONG loff;
    eShapeTypes type;
    Shape* shape;
    BoundingBox* box;

    ShapeFile shp (SHP_FILE);

    shp.GetFilePointer (loff);
    shape = shp.GetObjectAt (loff, type);
	CPPUNIT_ASSERT_MESSAGE ("shape type bad", ePolygonShape == type);

	CPPUNIT_ASSERT_MESSAGE ("record number bad", 1 == shape->GetRecordNum ());

    // this only works because there is a single shape:
    box = shape->GetBoundingBox ();
	CPPUNIT_ASSERT_MESSAGE ("min x bad", box->xMin == shp.GetBoundingBoxMinX ());
    CPPUNIT_ASSERT_MESSAGE ("max x bad", box->xMax == shp.GetBoundingBoxMaxX ());
    CPPUNIT_ASSERT_MESSAGE ("min y bad", box->yMin == shp.GetBoundingBoxMinY ());
    CPPUNIT_ASSERT_MESSAGE ("max y bad", box->yMax == shp.GetBoundingBoxMaxY ());

    CPPUNIT_ASSERT_MESSAGE ("number of points bad", 1718 == shape->GetNumPoints ());
    DoublePoint* points = shape->GetPoints ();

    delete shape;
}

void FileAccessTests::create_shx ()
{
    {
        ShapeFile shp (SOURCE_SHP_FILE);
        ShapeIndex shx (GENERATED_SHX_FILE, &shp);
    }

    try
    {
        // diff
        TestCommonFileUtil::CompareFiles (GENERATED_SHX_FILE, REFERENCE_SHX_FILE);
    }
    catch (...)
    {
        FdoCommonFile::Delete (GENERATED_SHX_FILE);
        throw;
    }
    FdoCommonFile::Delete (GENERATED_SHX_FILE);
}

void FileAccessTests::only_shp ()
{
    try
    {
        FdoPtr<FdoIConnection> connection;
        int count;

        connection = ShpTests::GetConnection ();
        connection->SetConnectionString (L"DefaultFileLocation=" ONLY_SHP_FILE);
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == connection->Open ());
        FdoPtr<FdoISelect> select = (FdoISelect*)connection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (L"world_adm0");
        FdoPtr<FdoIFeatureReader> reader = select->Execute ();
        count = 0;
        while (reader->ReadNext ())
        {
            count++;
        }
        CPPUNIT_ASSERT_MESSAGE ("wrong number of features selected", 209 == count);
        connection->Close ();
        FdoCommonFile::Delete (ONLY_SHX_FILE);
        FdoCommonFile::Delete (ONLY_IDX_FILE);


        // Try again, this time specifying a temp directory in the connection string;
        // NOTE: we are being a bit sneaky in counting files to validate the proper directory
        // is being used, which generally works but isn't 100% sound:
        connection = ShpTests::GetConnection ();
        connection->SetConnectionString (L"DefaultFileLocation=" ONLY_SHP_FILE L";TemporaryFileLocation=" ROOT_DATA_PATH);
        std::vector<std::wstring> filesBefore;
        FdoCommonFile::GetAllFiles (ROOT_DATA_PATH, filesBefore);
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == connection->Open ());
        select = (FdoISelect*)connection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (L"world_adm0");
        reader = select->Execute ();

        // NOTE: we now create a DBF in the same folder as the SHP file if it doesn't exist there;
        // this is to prevent deleted records reappearing after disconnecting/reconnecting.
        //std::vector<std::wstring> filesAfter;
        //FdoCommonFile::GetAllFiles (ROOT_DATA_PATH, filesAfter);
        //CPPUNIT_ASSERT_MESSAGE("DBF file not stored in specified temp directory", filesAfter.size() > filesBefore.size());

        count = 0;
        while (reader->ReadNext ())
        {
            count++;
        }
        CPPUNIT_ASSERT_MESSAGE ("wrong number of features selected", 209 == count);
        connection->Close ();
        FdoCommonFile::Delete (ONLY_SHX_FILE);
        FdoCommonFile::Delete (ONLY_IDX_FILE);
    }
    catch (FdoException *e)
    {
        TestCommonFail(e);
    }
}

/* Test deleting a shp file. */
void FileAccessTests::delete_shp ()
{
    FdoString* NEW_SCHEMA_NAME = L"NewSchema";
    FdoString* NEW_CLASS_NAME = L"Test";

    try
    {
        FdoPtr<FdoIConnection> connection;

        connection = ShpTests::GetConnection ();
        connection->SetConnectionString (L"DefaultFileLocation=" ROOT_DATA_PATH L"Ontario");
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == connection->Open ());

        // Clean up leftovers from previous tests:
        TestCommonSchemaUtil::CleanUpClass (connection, NULL, NEW_CLASS_NAME);

        FdoPtr<FdoIApplySchema> apply = (FdoIApplySchema*)connection->CreateCommand (FdoCommandType_ApplySchema);
        FdoPtr<FdoFeatureSchema> schema = FdoFeatureSchema::Create (NEW_SCHEMA_NAME, L"");
        FdoPtr<FdoClassCollection> classes = schema->GetClasses ();

        FdoPtr<FdoDataPropertyDefinition> id = FdoDataPropertyDefinition::Create (L"Id", L"integer");
        id->SetDataType (FdoDataType_Int32);
        id->SetIsAutoGenerated (true);
        id->SetNullable (false);

        FdoPtr<FdoDataPropertyDefinition> street = FdoDataPropertyDefinition::Create (L"Street", L"text");
        street->SetDataType (FdoDataType_String);
        street->SetLength (64);

        FdoPtr<FdoDataPropertyDefinition> area = FdoDataPropertyDefinition::Create (L"Area", L"double");
        area->SetDataType (FdoDataType_Decimal);
        area->SetPrecision (20);
        area->SetScale (8);

        FdoPtr<FdoDataPropertyDefinition> vacant = FdoDataPropertyDefinition::Create (L"Vacant", L"boolean");
        vacant->SetDataType (FdoDataType_Boolean);

        FdoPtr<FdoDataPropertyDefinition> birthday = FdoDataPropertyDefinition::Create (L"Birthday", L"date");
        birthday->SetDataType (FdoDataType_DateTime);

        // build a location geometry property
        FdoPtr<FdoGeometricPropertyDefinition> location = FdoGeometricPropertyDefinition::Create (L"Geometry", L"geometry");
        location->SetGeometryTypes (FdoGeometricType_Point);
        location->SetHasElevation (true);
        location->SetHasMeasure (true);

        //// assemble the feature class
        FdoPtr<FdoFeatureClass> feature = FdoFeatureClass::Create (NEW_CLASS_NAME, L"test class created with apply schema");
        FdoPtr<FdoPropertyDefinitionCollection> properties = feature->GetProperties ();
        properties->Add (id);
        properties->Add (street);
        properties->Add (area);
        properties->Add (vacant);
        properties->Add (birthday);
        properties->Add (location);
        feature->SetGeometryProperty (location);
        FdoPtr<FdoDataPropertyDefinitionCollection> identities = feature->GetIdentityProperties ();
        identities->Add (id);

        // submit the new schema
        classes->Add (feature);
        apply->SetFeatureSchema (schema);
        apply->Execute ();

        // insert some data
        FdoPtr<FdoIInsert> insert = (FdoIInsert*)connection->CreateCommand (FdoCommandType_Insert);
        insert->SetFeatureClassName (NEW_CLASS_NAME);
        FdoPtr<FdoPropertyValueCollection> values = insert->GetPropertyValues ();
        values->Clear ();
        FdoPtr<FdoValueExpression> expression = (FdoValueExpression*)FdoExpression::Parse (L"'1147 Trafford Drive'");
        FdoPtr<FdoPropertyValue> value = FdoPropertyValue::Create (L"Street", expression);
        values->Add (value);
        // add NULL geometry value:
        FdoPtr<FdoGeometryValue> geometry = FdoGeometryValue::Create ();
        geometry->SetNullValue ();
        value = FdoPropertyValue::Create (L"Geometry", geometry);
        values->Add (value);
        FdoPtr<FdoIFeatureReader> reader = insert->Execute ();
        FdoInt32 featid;
        featid = -1;
        while (reader->ReadNext ())
        {
            if (-1 != featid)
                CPPUNIT_FAIL ("too many features inserted");
            featid = reader->GetInt32 (L"Id");
        }
        reader->Close ();
        if (-1 == featid)
            CPPUNIT_FAIL ("too few features inserted");

        // close the connection
        connection->Close ();
    }
    catch (FdoException* ge) 
    {
        TestCommonFail (ge);
    }

    // reopen, read and close the connection
    try
    {
        FdoPtr<FdoIConnection> connection;

        connection = ShpTests::GetConnection ();
#ifdef _WIN32
        connection->SetConnectionString (L"DefaultFileLocation=" ROOT_DATA_PATH L"Ontario\\Test.shp");
#else
        connection->SetConnectionString (L"DefaultFileLocation=" ROOT_DATA_PATH L"Ontario/Test.shp");
#endif
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == connection->Open ());
        FdoPtr<FdoISelect> select = (FdoISelect*)connection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (NEW_CLASS_NAME);
        // leave the reader unread:
        FdoPtr<FdoIFeatureReader> reader = select->Execute ();
        //FdoInt32 featid;
        //featid = -1;
        //while (reader->ReadNext ())
        //{
        //    if (-1 != featid)
        //        CPPUNIT_FAIL ("too many features selected");
        //    featid = reader->GetInt32 (L"FeatId");
        //}
        //reader->Close ();
        //if (-1 == featid)
        //    CPPUNIT_FAIL ("too few features selected");
        connection->Close ();
    }
    catch (FdoException* ge) 
    {
        TestCommonFail (ge);
    }

    // OK, now delete the file
#ifdef _WIN32
    CPPUNIT_ASSERT_MESSAGE("shp file not deleted", FdoCommonFile::Delete (ROOT_DATA_PATH L"Ontario\\Test.shp"));
    CPPUNIT_ASSERT_MESSAGE("dbf file not deleted", FdoCommonFile::Delete (ROOT_DATA_PATH L"Ontario\\Test.dbf"));
    CPPUNIT_ASSERT_MESSAGE("shx file not deleted", FdoCommonFile::Delete (ROOT_DATA_PATH L"Ontario\\Test.shx"));
    CPPUNIT_ASSERT_MESSAGE("idx file not deleted", FdoCommonFile::Delete (ROOT_DATA_PATH L"Ontario\\Test.idx"));
	CPPUNIT_ASSERT_MESSAGE("cpg file not deleted", FdoCommonFile::Delete (ROOT_DATA_PATH L"Ontario\\Test.cpg"));
#else
    CPPUNIT_ASSERT_MESSAGE("shp file not deleted", FdoCommonFile::Delete (ROOT_DATA_PATH L"Ontario/Test.shp"));
    CPPUNIT_ASSERT_MESSAGE("dbf file not deleted", FdoCommonFile::Delete (ROOT_DATA_PATH L"Ontario/Test.dbf"));
    CPPUNIT_ASSERT_MESSAGE("shx file not deleted", FdoCommonFile::Delete (ROOT_DATA_PATH L"Ontario/Test.shx"));
    CPPUNIT_ASSERT_MESSAGE("idx file not deleted", FdoCommonFile::Delete (ROOT_DATA_PATH L"Ontario/Test.idx"));
	CPPUNIT_ASSERT_MESSAGE("cpg file not deleted", FdoCommonFile::Delete (ROOT_DATA_PATH L"Ontario/Test.cpg"));
#endif
}


