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
#include "SchemaTests.h"
#include "SHP/FdoShpOverrides.h"
#include "ShpProvider.h"
#include "FdoCommonFile.h"
#include "FdoCommonSchemaUtil.h"


#ifdef _WIN32
#define LOCATION L"..\\..\\TestData\\Testing\\"
#define LOCATION2 L"..\\..\\TestData\\t\x5348\x524dsting\\"
#define LOCATION3 L"..\\..\\TestData\\Ontario\\"
#define LOCATION4 L"..\\..\\TestData\\Florida\\"
#define LOCATION5 L"..\\..\\TestData\\Sheboygan"
#define LOCATION6 L"..\\..\\TestData\\Fix784301"
#define LOCATION7 L"..\\..\\TestData\\TestingSC\\"

#else
#define LOCATION L"../../TestData/Testing/"
#define LOCATION2 L"../../TestData/t\x5348\x524dsting/"
#define LOCATION3 L"../../TestData/Ontario/"
#define LOCATION4 L"../../TestData/Florida/"
#define LOCATION5 L"../../TestData/Sheboygan/"
#define LOCATION6 L"..\\..\\TestData\\Fix784301"
#define LOCATION7 L"../../TestData/TestingSC/"
#endif


CPPUNIT_TEST_SUITE_REGISTRATION (SchemaTests);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (SchemaTests, "SchemaTests");

FdoPtr<FdoIConnection> SchemaTests::mConnection;

SchemaTests::SchemaTests (void)
{
}

SchemaTests::~SchemaTests (void)
{
}

void SchemaTests::setUp ()
{
	VERBOSE = false;

    try
    {
        if (!FdoCommonFile::FileExists (LOCATION))
            FdoCommonFile::MkDir (LOCATION);
        mConnection = ShpTests::GetConnection ();
		ShpTests::sLocation = LOCATION;
        mConnection->SetConnectionString (L"DefaultFileLocation=" LOCATION);
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->Open ());

		// Extra location
		if (!FdoCommonFile::FileExists (LOCATION7))
            FdoCommonFile::MkDir (LOCATION7);
    }
    catch (FdoException* ge)
    {
        TestCommonFail (ge);
    }
}

void SchemaTests::tearDown ()
{
    try
    {
        // delete the classes, if they're there:
        TestCommonSchemaUtil::CleanUpClass(mConnection, NULL, L"Test");
        TestCommonSchemaUtil::CleanUpClass(mConnection, NULL, L"Test1");
        TestCommonSchemaUtil::CleanUpClass(mConnection, NULL, L"Test2");

        mConnection->Close ();
		FDO_SAFE_RELEASE(mConnection.p);

        if (FdoCommonFile::FileExists (LOCATION L"schema.xml"))
            FdoCommonFile::Delete (LOCATION L"schema.xml");
        if (FdoCommonFile::FileExists (LOCATION))
            FdoCommonFile::RmDir (LOCATION);
        if (FdoCommonFile::FileExists (LOCATION7))
            FdoCommonFile::RmDir (LOCATION7);
    }
    catch (FdoException *ge)
    {
        TestCommonFail (ge);
    }
}

void SchemaTests::show_schema (FdoFeatureSchemaCollection* schemas)
{
	FdoPtr<FdoISchemaCapabilities>  schemaCapabilities = mConnection->GetSchemaCapabilities();

    printf ("\tSchema Capabilities:\n");
    printf ("\t\tSupports Association Properties: %s\n", schemaCapabilities->SupportsAssociationProperties() ? "yes" : "no");
    printf ("\t\tSupports AutoId Generation: %s\n", schemaCapabilities->SupportsAutoIdGeneration() ? "yes" : "no");
    printf ("\t\tSupports Composite Unique Value Constraints: %s\n", schemaCapabilities->SupportsCompositeUniqueValueConstraints() ? "yes" : "no");
    printf ("\t\tSupports MultipleSchemas: %s\n", schemaCapabilities->SupportsMultipleSchemas() ? "yes" : "no");

	// Many others here, too boring...


    for (int i = 0; i < schemas->GetCount (); i++)
    {
        FdoFeatureSchema* schema = schemas->GetItem (i);
        printf ("Schema: %ls\n", schema->GetName());
        if ((schema->GetDescription () != NULL) && (0 != wcscmp (schema->GetDescription (), L"")))
            printf ("    Description: %ls\n", schema->GetDescription ());

        FdoClassCollection* classes = schema->GetClasses ();
        for (int j = 0; j < classes->GetCount (); j++)
        {
            FdoClassDefinition* cls = classes->GetItem (j);

            // Output basic class info:
            if (FdoClassType_FeatureClass == cls->GetClassType ())
                printf ("    Feature Class: %ls\n", cls->GetName ());
            else
                printf ("    Class: %ls\n", cls->GetName ());
            if ((cls->GetDescription () != NULL) && (0 != wcscmp (cls->GetDescription (), L"")))
                printf ("        Description: %ls\n", cls->GetDescription ());

            FdoPtr<FdoClassCapabilities> classCapabilities = cls->GetCapabilities();
            printf ("        Class Capabilities:\n");
            if (classCapabilities == NULL)
                printf ("            (Not available).\n");
            else
            {
                printf ("            Supports locking: %s\n", classCapabilities->SupportsLocking() ? "yes" : "no");
                printf ("            Supports long transactions: %s\n", classCapabilities->SupportsLongTransactions() ? "yes" : "no");
            }


            // Output identity properties:
            FdoDataPropertyDefinitionCollection* identity = cls->GetIdentityProperties ();
            for (int k = 0; k < identity->GetCount (); k++)
            {
                FdoDataPropertyDefinition* definition = identity->GetItem (k);
                printf ("        Id: %ls\n", definition->GetName ());
                if ((definition->GetDescription () != NULL) && (0 != wcscmp (definition->GetDescription (), L"")))
                    printf ("            Description: %ls\n", definition->GetDescription ());
                printf ("            Type: %ls Length: %d Precision: %d %ls\n",
                    FdoCommonMiscUtil::FdoDataTypeToString (definition->GetDataType ()),
                    definition->GetLength (),
                    definition->GetPrecision (),
                    definition->GetNullable () ? L"Nullable" : L"NotNull");
                definition->Release ();
            }

            // Output regular properties:
            FdoPropertyDefinitionCollection* properties = cls->GetProperties ();
            for (int k = 0; k < properties->GetCount (); k++)
            {
                FdoPropertyDefinition* definition = properties->GetItem (k);
                if (definition->GetPropertyType () == FdoPropertyType_DataProperty)
                {
                    FdoDataPropertyDefinition* data_definition = (FdoDataPropertyDefinition*)definition;
                    if (!identity->Contains (data_definition))
                    {
                        printf ("        Property: %ls\n", definition->GetName ());
                        if ((data_definition->GetDescription () != NULL) && (0 != wcscmp (data_definition->GetDescription (), L"")))
                            printf ("            Description: %ls\n", data_definition->GetDescription ());
                        printf ("            Type: %ls Length: %d Precision: %d %ls\n",
                            FdoCommonMiscUtil::FdoDataTypeToString (data_definition->GetDataType ()),
                            data_definition->GetLength (),
                            data_definition->GetPrecision (),
                            data_definition->GetNullable () ? L"Nullable" : L"NotNull");
                    }
                }
                else if (definition->GetPropertyType () == FdoPropertyType_ObjectProperty)
                {
                    printf ("       Object Property: %ls\n", definition->GetName ());
                    if ((definition->GetDescription () != NULL) && (0 != wcscmp (definition->GetDescription (), L"")))
                        printf ("            Description: %ls\n", definition->GetDescription ());
                }
                else if (definition->GetPropertyType () == FdoPropertyType_GeometricProperty)
                {
                    FdoGeometricPropertyDefinition* geometry_definition = (FdoGeometricPropertyDefinition*)definition;
                    printf ("        Geometric Property: %ls\n", geometry_definition->GetName ());
                    if ((geometry_definition->GetDescription () != NULL) && (0 != wcscmp (geometry_definition->GetDescription (), L"")))
                        printf ("            Description: %ls\n", geometry_definition->GetDescription ());
                    int types = geometry_definition->GetGeometryTypes ();
                    if (0 != (types & FdoGeometricType_Point))
                        printf ("            FdoGeometricType_Point types allowed\n");
                    if (0 != (types & FdoGeometricType_Curve))
                        printf ("            FdoGeometricType_Curve types allowed\n");
                    if (0 != (types & FdoGeometricType_Surface))
                        printf ("            FdoGeometricType_Surface types allowed\n");
                    if (0 != (types & FdoGeometricType_Solid))
                        printf ("            FdoGeometricType_Solid types allowed\n");
                }
                definition->Release ();
            }
            identity->Release ();
            properties->Release ();

            // Output schema attribute dictionary:
            FdoSchemaAttributeDictionary* dictionary = cls->GetAttributes ();
            int count;
            const wchar_t **names = dictionary->GetAttributeNames (count);
            if ((0 != count) && (NULL != names))
            {
                printf ("        MetaData:");
                const wchar_t *name = *names;
                for (int i = 0; i < count; i++)
                {
                    if (0 != i)
                        printf (",");
                    const wchar_t* value = dictionary->GetAttributeValue (name);
                    printf (" %ls=%ls", name, value);
                    name++;
                }
                printf ("\n");
            }
            dictionary->Release ();

            cls->Release ();
        }
        classes->Release ();
        schema->Release ();
    }
}

/* Test basic describe operation. */
void SchemaTests::describe ()
{
    try
    {
        FdoPtr<FdoIDescribeSchema> describe = (FdoIDescribeSchema*)mConnection->CreateCommand (FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> schemas = describe->Execute ();
        if (VERBOSE)
            show_schema (schemas);
    }
    catch (FdoException* ge) 
    {
        TestCommonFail (ge);
    }
}

/* Test describe operation with nonexistant schema. */
void SchemaTests::describe_bogus ()
{
    try
    {
        FdoPtr<FdoIDescribeSchema> describe = (FdoIDescribeSchema*)mConnection->CreateCommand (FdoCommandType_DescribeSchema);
        // ensure asking for a nonexistant schema yields the proper exception
        try
        {
            describe->SetSchemaName (L"Bogus");
            FdoPtr<FdoFeatureSchemaCollection> schemas = describe->Execute ();
            CPPUNIT_FAIL ("no exception asking for a nonexistant schema");
        }
        catch (FdoException* ge)
        {
            // check for expected message
            CPPUNIT_ASSERT_MESSAGE ("wrong exception", 0 == wcsncmp (L"Schema 'Bogus' not found.", ge->GetExceptionMessage (), 25));
            ge->Release ();
        }
    }
    catch (FdoException* ge) 
    {
        TestCommonFail (ge);
    }
}

/* Test basic apply operation. */
void SchemaTests::apply ()
{
    apply_helper(false);

    apply_helper(true);
}

/* Test nameless schema apply operation. */
void SchemaTests::apply_nameless ()
{
    try
    {
        FdoString* NEW_SCHEMA_NAME = L"";
        FdoString* NEW_CLASS_NAME = L"Test";

        // Clean up leftovers from previous tests:
        TestCommonSchemaUtil::CleanUpClass(mConnection, NULL, NEW_CLASS_NAME);

        FdoPtr<FdoIApplySchema> apply = (FdoIApplySchema*)mConnection->CreateCommand (FdoCommandType_ApplySchema);
        FdoPtr<FdoFeatureSchema> schema = FdoFeatureSchema::Create (NEW_SCHEMA_NAME, L"");
        FdoPtr<FdoClassCollection> classes = schema->GetClasses ();

        FdoPtr<FdoDataPropertyDefinition> id = FdoDataPropertyDefinition::Create (L"Id", L"integer");
        id->SetDataType (FdoDataType_Int32);

        FdoPtr<FdoDataPropertyDefinition> street = FdoDataPropertyDefinition::Create (L"Street", L"text");
        street->SetDataType (FdoDataType_String);
        street->SetLength (64);

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
        properties->Add (location);
        feature->SetGeometryProperty (location);
        FdoPtr<FdoDataPropertyDefinitionCollection> identities = feature->GetIdentityProperties ();
        identities->Add (id);

        bool applied = false;
        try
        {
            // submit the new schema
            classes->Add (feature);
            apply->SetFeatureSchema (schema);
            apply->Execute ();
			SaveSchema(mConnection);
            applied = true;
        }
        catch (FdoException* ge)
        {
            // check for expected message
            CPPUNIT_ASSERT_MESSAGE ("wrong exception", 0 == wcsncmp (L"No schema specified for the apply schema command.", ge->GetExceptionMessage (), 49));
            ge->Release ();
        }

        // check that the new schema doesn't show up in the list
        FdoPtr<FdoIDescribeSchema> describe = (FdoIDescribeSchema*)mConnection->CreateCommand (FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> schemas = describe->Execute ();
        FdoPtr<FdoIDisposableCollection> collection = schemas->FindClass (NEW_CLASS_NAME);
        FdoInt32 count = collection->GetCount ();
        if (0 != count)
        {
            // delete the class
            schema = schemas->GetItem (NEW_SCHEMA_NAME);
            classes = schema->GetClasses ();
            FdoPtr<FdoClassDefinition> definition = classes->GetItem (NEW_CLASS_NAME);
            definition->Delete ();
            apply->SetFeatureSchema (schema);
            apply->Execute ();
			SaveSchema(mConnection);
        }
        CPPUNIT_ASSERT_MESSAGE ("nameless schema applied", !applied);
        CPPUNIT_ASSERT_MESSAGE ("class found from nameless schema apply", 0 == count);
    }
    catch (FdoException* ge) 
    {
        TestCommonFail (ge);
    }
}

/* Test nameless class creation. */
void SchemaTests::create_nameless ()
{
    try
    {
        FdoString* NEW_SCHEMA_NAME = L"NewSchema";
        FdoString* NEW_CLASS_NAME = L"";

        // Clean up leftovers from previous tests:
        TestCommonSchemaUtil::CleanUpClass(mConnection, NULL, NEW_CLASS_NAME);

        FdoPtr<FdoIApplySchema> apply = (FdoIApplySchema*)mConnection->CreateCommand (FdoCommandType_ApplySchema);
        FdoPtr<FdoFeatureSchema> schema = FdoFeatureSchema::Create (NEW_SCHEMA_NAME, L"");
        FdoPtr<FdoClassCollection> classes = schema->GetClasses ();

        FdoPtr<FdoDataPropertyDefinition> id = FdoDataPropertyDefinition::Create (L"Id", L"integer");
        id->SetDataType (FdoDataType_Int32);

        FdoPtr<FdoDataPropertyDefinition> street = FdoDataPropertyDefinition::Create (L"Street", L"text");
        street->SetDataType (FdoDataType_String);
        street->SetLength (64);

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
        properties->Add (location);
        feature->SetGeometryProperty (location);
        FdoPtr<FdoDataPropertyDefinitionCollection> identities = feature->GetIdentityProperties ();
        identities->Add (id);

        bool applied = false;
        try
        {
            // submit the new schema
            classes->Add (feature);
            apply->SetFeatureSchema (schema);
            apply->Execute ();
			SaveSchema(mConnection);
            applied = true;
        }
        catch (FdoException* ge)
        {
            // check for expected message
            CPPUNIT_ASSERT_MESSAGE ("wrong exception", 0 == wcsncmp (L"The class name '' is invalid.", ge->GetExceptionMessage (), 29));
            ge->Release ();
        }

        // check that the new class doesn't show up in the schema
        FdoPtr<FdoIDescribeSchema> describe = (FdoIDescribeSchema*)mConnection->CreateCommand (FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> schemas = describe->Execute ();
        FdoPtr<FdoIDisposableCollection> collection = schemas->FindClass (NEW_CLASS_NAME);
        FdoInt32 count = collection->GetCount ();
        if (0 != count)
        {
            // delete the class
            schema = schemas->GetItem (NEW_SCHEMA_NAME);
            classes = schema->GetClasses ();
            FdoPtr<FdoClassDefinition> definition = classes->GetItem (NEW_CLASS_NAME);
            definition->Delete ();
            apply->SetFeatureSchema (schema);
            apply->Execute ();
			SaveSchema(mConnection);
        }
        CPPUNIT_ASSERT_MESSAGE ("nameless class created", !applied);
        CPPUNIT_ASSERT_MESSAGE ("class found from nameless schema apply", 0 == count);
    }
    catch (FdoException* ge) 
    {
        TestCommonFail (ge);
    }
}

// Executes a describe schema mapping (with default values, no config file) command.
void SchemaTests::schema_mapping_defaults_test()
{
    try
    {
        mConnection->Close ();
#ifdef _WIN32
        mConnection->SetConnectionString (L"DefaultFileLocation=" LOCATION3);
#else
        mConnection->SetConnectionString (L"DefaultFileLocation=" LOCATION3);
#endif
		ShpTests::sLocation = LOCATION3;
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->Open ());

        FdoPtr<FdoIDescribeSchemaMapping> describeSchemaMappingCmd =
            (FdoIDescribeSchemaMapping*)mConnection->CreateCommand(FdoCommandType_DescribeSchemaMapping);
        describeSchemaMappingCmd->SetIncludeDefaults(true);
        FdoSchemaMappingsP mappings = describeSchemaMappingCmd->Execute();

        FdoPtr<FdoIDescribeSchema> describeSchemaCmd =
            (FdoIDescribeSchema*)mConnection->CreateCommand(FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> logicalSchemas = describeSchemaCmd->Execute();

        // test the schema mappings:
        _TestSchemaMappings(mappings, logicalSchemas);
    }
    catch (FdoException* e)
    {
        TestCommonFail (e);
    }
}


// Executes a describe schema mapping (without default values, no config file) command.
void SchemaTests::schema_mapping_no_defaults_test()
{
    try
    {
        FdoPtr<FdoIDescribeSchemaMapping> describeSchemaMappingCmd =
            (FdoIDescribeSchemaMapping*)mConnection->CreateCommand(FdoCommandType_DescribeSchemaMapping);
        describeSchemaMappingCmd->SetIncludeDefaults(false);
        FdoSchemaMappingsP mappings = describeSchemaMappingCmd->Execute();

        CPPUNIT_ASSERT_MESSAGE("default mappings should be empty", mappings->GetCount()==0);

        // test the schema mappings:
        _TestSchemaMappings(mappings, NULL);
    }
    catch (FdoException* e)
    {
        TestCommonFail (e);
    }
}


// Helper method; iterates the given schema mappings, comparing the mappings to 
// the logical schema (if one is given).
void SchemaTests::_TestSchemaMappings(FdoPhysicalSchemaMappingCollection* mappings, FdoFeatureSchemaCollection* logicalSchemas)
{
    if (mappings == NULL)
        CPPUNIT_FAIL("FAILED - DescribeSchemaMapping returned NULL collection\n");

    FdoInt32 numMappings = mappings->GetCount();

    if (logicalSchemas != NULL)
    {
        // Match logical count to mapping count
        CPPUNIT_ASSERT_MESSAGE("Number of schema mappings != number of logical schemas", numMappings==logicalSchemas->GetCount());
    }

    for (int i=0; i<numMappings; i++)
    {
        FdoPhysicalSchemaMappingP fdoMapping = mappings->GetItem(i);
        FdoShpOvPhysicalSchemaMapping * mapping = static_cast<FdoShpOvPhysicalSchemaMapping *>(fdoMapping.p);

        FdoString* mappingName = mapping->GetName();
        FdoString* mappingProviderName = mapping->GetProvider();

        FdoPtr<FdoFeatureSchema> logicalSchema;
        FdoPtr<FdoClassCollection> logicalClasses;
        if (logicalSchemas != NULL)
        {
            logicalSchema = logicalSchemas->FindItem(mappingName);
            CPPUNIT_ASSERT_MESSAGE("Didn't find matching logical schema", logicalSchema!=NULL);
            logicalClasses = logicalSchema->GetClasses();
        }

        FdoPtr<FdoShpOvClassCollection> classes = mapping->GetClasses();

        FdoInt32 numClasses = classes->GetCount();

        if (logicalClasses != NULL)
        {
            CPPUNIT_ASSERT_MESSAGE("Logical class count doesn't match mapping class count", logicalClasses->GetCount()==numClasses);
        }

        for (int j=0; j<numClasses; j++)
        {
            FdoPtr<FdoShpOvClassDefinition> classMapping = classes->GetItem(j);

            // analyze the feature class
            FdoString* className = classMapping->GetName();
            FdoString* shapefileName = classMapping->GetShapeFile();

            // Determine default shapefile filename:
            FdoPtr<FdoIConnectionInfo> connInfo = mConnection->GetConnectionInfo();
            FdoPtr<FdoIConnectionPropertyDictionary> connProps = connInfo->GetConnectionProperties();
            FdoStringP defaultShapefileNameNoExt = connProps->GetProperty(CONNECTIONPROPERTY_DEFAULT_FILE_LOCATION);
            FdoCommonFile::DelimitPath(defaultShapefileNameNoExt);
            defaultShapefileNameNoExt += className;
			
			defaultShapefileNameNoExt += L".shp";
			FdoStringP pFullFileName = FdoCommonFile::GetAbsolutePath(defaultShapefileNameNoExt);
			FdoStringP pFullFolderName = FdoCommonFile::GetAbsolutePath(connProps->GetProperty(CONNECTIONPROPERTY_DEFAULT_FILE_LOCATION));
			defaultShapefileNameNoExt = FdoCommonFile::GetRelativePath(pFullFolderName, pFullFileName);

            FdoPtr<FdoClassDefinition> logicalClass;
            if (logicalClasses != NULL)
            {
                // match physical name to logical name:
                CPPUNIT_ASSERT_MESSAGE("Physical name didn't match logical name", 0==wcsncmp(shapefileName, defaultShapefileNameNoExt, wcslen(shapefileName)-4));

                logicalClass = logicalClasses->FindItem(className);
                CPPUNIT_ASSERT_MESSAGE("logical class not found", logicalClass!=NULL);
            }

            FdoPtr<FdoShpOvPropertyDefinitionCollection> classProps = classMapping->GetProperties();
            FdoInt32 numProps = classProps->GetCount();

            FdoPtr<FdoPropertyDefinitionCollection> logicalProperties;
            bool bHasGeometryProperty = false;
            if (logicalClass != NULL)
            {
                logicalProperties = logicalClass->GetProperties();

                for (FdoInt32 i=0; i<logicalProperties->GetCount() && !bHasGeometryProperty; i++)
                {
                    FdoPtr<FdoPropertyDefinition> logicalProp = logicalProperties->GetItem(i);
                    bHasGeometryProperty = (logicalProp->GetPropertyType() == FdoPropertyType_GeometricProperty);
                }

                // Match logical count to mapping count:
                // NOTE: logical class may have an additional geometry property that's not in the mapping,
                //       since the geometry mapping is handle in a special way:
                // NOTE: logical class will hvae a FeatId identity property that's not in the mapping either.
                CPPUNIT_ASSERT_MESSAGE("Logical property count != mapping property count", logicalProperties->GetCount()-(bHasGeometryProperty?1:0)-1 == numProps);
            }

            // Compare the data properties:
            for (int k=0; k<numProps; k++)
            {
                // NOTE: all properties in this collection are data properties, so we can do a static cast:
                FdoPtr<FdoShpOvPropertyDefinition> propDef = (FdoShpOvPropertyDefinition*)classProps->GetItem(k);
                FdoString* propName = propDef->GetName();

                FdoPtr<FdoPropertyDefinition> logicalProperty;
                if (logicalProperties != NULL)
                {
                    logicalProperty = logicalProperties->FindItem(propName);
                    FdoPtr<FdoShpOvColumnDefinition> column = propDef->GetColumn();
                    CPPUNIT_ASSERT_MESSAGE("logical property not found", logicalProperty != NULL);

                    // Match physical name to logical name:
                    CPPUNIT_ASSERT_MESSAGE("physical column name != logical property name", 0==wcscmp(propName,column->GetName()));
                }
            }
        }
    }
}


void SchemaTests::add_class ()
{
    try
    {
        TestCommonSchemaUtil::CleanUpClass (mConnection, L"Default", L"Test");

        FdoPtr<FdoIDescribeSchema> describe = (FdoIDescribeSchema*)mConnection->CreateCommand (FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> schemas = describe->Execute ();
        CPPUNIT_ASSERT_MESSAGE ("Expecting 1 schema", 1 == schemas->GetCount ());
        FdoPtr<FdoFeatureSchema> schema = schemas->GetItem (0);
        FdoPtr<FdoClassCollection> classes = schema->GetClasses ();

        FdoPtr<FdoIApplySchema> apply = (FdoIApplySchema*)mConnection->CreateCommand (FdoCommandType_ApplySchema);

        FdoPtr<FdoDataPropertyDefinition> id = FdoDataPropertyDefinition::Create (L"Id", L"integer");
        id->SetDataType (FdoDataType_Int32);

        FdoPtr<FdoDataPropertyDefinition> street = FdoDataPropertyDefinition::Create (L"Street", L"text");
        street->SetDataType (FdoDataType_String);
        street->SetLength (64);

        // build a location geometry property
        FdoPtr<FdoGeometricPropertyDefinition> location = FdoGeometricPropertyDefinition::Create (L"Geometry", L"geometry");
        location->SetGeometryTypes (FdoGeometricType_Point);
        location->SetHasElevation (true);
        location->SetHasMeasure (true);

        //// assemble the feature class
        FdoPtr<FdoFeatureClass> feature = FdoFeatureClass::Create (L"Test", L"test class created with apply schema");
        FdoPtr<FdoPropertyDefinitionCollection> properties = feature->GetProperties ();
        properties->Add (id);
        properties->Add (street);
        properties->Add (location);
        feature->SetGeometryProperty (location);
        FdoPtr<FdoDataPropertyDefinitionCollection> identities = feature->GetIdentityProperties ();
        identities->Add (id);

        // submit the new schema
        classes->Add (feature);
        apply->SetFeatureSchema (schema);
        apply->Execute ();
		SaveSchema(mConnection);

        // close and reopen the connection
        mConnection->Close ();
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->Open ());

        // check that the new schema shows up in the list
        schemas = describe->Execute ();
        CPPUNIT_ASSERT_MESSAGE ("Expecting 1 schema", 1 == schemas->GetCount ());
        schema = schemas->GetItem (0);
        classes = schema->GetClasses ();
        FdoPtr<FdoFeatureClass> cls = (FdoFeatureClass*)classes->FindItem (L"Test");
        CPPUNIT_ASSERT_MESSAGE ("Test class not found", NULL != cls.p);

        // check that the definition is the same
        CPPUNIT_ASSERT_MESSAGE ("class bad description", 0 == (wcscmp (L"test class created with apply schema", cls->GetDescription ())));
        properties = cls->GetProperties ();
        id = (FdoDataPropertyDefinition*)properties->FindItem (L"Id");
        CPPUNIT_ASSERT_MESSAGE ("id not found", NULL != id.p);
        CPPUNIT_ASSERT_MESSAGE ("id not FdoDataType_Int32", FdoDataType_Int32 == id->GetDataType ());
        CPPUNIT_ASSERT_MESSAGE ("id bad description", 0 == (wcscmp (L"integer", id->GetDescription ())));
        street = (FdoDataPropertyDefinition*)properties->FindItem (L"Street");
        CPPUNIT_ASSERT_MESSAGE ("street not found", NULL != street.p);
        CPPUNIT_ASSERT_MESSAGE ("street not FdoDataType_String", FdoDataType_String == street->GetDataType ());
        CPPUNIT_ASSERT_MESSAGE ("street bad description", 0 == (wcscmp (L"text", street->GetDescription ())));
        location = (FdoGeometricPropertyDefinition*)properties->FindItem (L"Geometry");
        CPPUNIT_ASSERT_MESSAGE ("geometry not found", NULL != location.p);
        CPPUNIT_ASSERT_MESSAGE ("geometry bad description", 0 == (wcscmp (L"geometry", location->GetDescription ())));
        CPPUNIT_ASSERT_MESSAGE ("location not points", FdoGeometricType_Point == location->GetGeometryTypes ());
        CPPUNIT_ASSERT_MESSAGE ("location no elevation", location->GetHasElevation ());
        CPPUNIT_ASSERT_MESSAGE ("location no measure", location->GetHasMeasure ());
        FdoPtr<FdoGeometricPropertyDefinition> geometry = cls->GetGeometryProperty ();
        CPPUNIT_ASSERT_MESSAGE ("wrong geometry", location == geometry);
        FdoPtr<FdoDataPropertyDefinitionCollection> ids = cls->GetIdentityProperties ();
        FdoPtr<FdoDataPropertyDefinition> identifier = ids->FindItem (L"Id");
        CPPUNIT_ASSERT_MESSAGE ("wrong id", id == identifier);
    }
    catch (FdoException* e)
    {
        TestCommonFail (e);
    }
}

/* Test apply operation with only identity and geometry. */
void SchemaTests::apply_two ()
{
    try
    {
        FdoString* NEW_SCHEMA_NAME = L"NewSchema";
        FdoString* NEW_CLASS_NAME = L"Test";

        // Clean up leftovers from previous tests:
        TestCommonSchemaUtil::CleanUpClass(mConnection, NULL, NEW_CLASS_NAME);


        FdoPtr<FdoIApplySchema> apply = (FdoIApplySchema*)mConnection->CreateCommand (FdoCommandType_ApplySchema);
        FdoPtr<FdoFeatureSchema> schema = FdoFeatureSchema::Create (NEW_SCHEMA_NAME, L"");
        FdoPtr<FdoClassCollection> classes = schema->GetClasses ();

        FdoPtr<FdoDataPropertyDefinition> id = FdoDataPropertyDefinition::Create (L"Id", L"integer");
        id->SetDataType (FdoDataType_Int32);
        FdoPtr<FdoGeometricPropertyDefinition> location = FdoGeometricPropertyDefinition::Create (L"Geometry", L"geometry");
        location->SetGeometryTypes (FdoGeometricType_Point);
        location->SetHasElevation (true);
        location->SetHasMeasure (true);

        // assemble the feature class
        FdoPtr<FdoFeatureClass> feature = FdoFeatureClass::Create (NEW_CLASS_NAME, L"test class created with apply schema");
        FdoPtr<FdoPropertyDefinitionCollection> properties = feature->GetProperties ();
        properties->Add (id);
        properties->Add (location);
        feature->SetGeometryProperty (location);
        FdoPtr<FdoDataPropertyDefinitionCollection> identities = feature->GetIdentityProperties ();
        identities->Add (id);

        // submit the new schema
        classes->Add (feature);
        apply->SetFeatureSchema (schema);
        apply->Execute ();
		SaveSchema(mConnection);

        // close and reopen the connection
        mConnection->Close ();
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->Open ());

        // check that the new schema shows up in the list
        FdoPtr<FdoIDescribeSchema> describe = (FdoIDescribeSchema*)mConnection->CreateCommand (FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> schemas = describe->Execute ();

        FdoPtr<FdoIDisposableCollection> collection = schemas->FindClass (NEW_CLASS_NAME);
        CPPUNIT_ASSERT_MESSAGE ("no class found", 1 == collection->GetCount ());
        FdoPtr<FdoFeatureClass> cls = (FdoFeatureClass *)collection->GetItem (0);
        CPPUNIT_ASSERT_MESSAGE ("wrong name", 0 == wcscmp (NEW_CLASS_NAME, cls->GetName ()));

        // check it's contents
        properties = cls->GetProperties ();
        FdoPtr<FdoDataPropertyDefinition> featid = (FdoDataPropertyDefinition*)properties->GetItem (L"Id");
        CPPUNIT_ASSERT_MESSAGE ("id wrong type", FdoDataType_Int32 == featid->GetDataType ());
        location = (FdoGeometricPropertyDefinition*)properties->GetItem (L"Geometry");
        CPPUNIT_ASSERT_MESSAGE ("wrong geometry types", FdoGeometricType_Point == location->GetGeometryTypes ());
        CPPUNIT_ASSERT_MESSAGE ("wrong elevation", location->GetHasElevation ());

        // OK, now delete the class
        schema = schemas->GetItem (NEW_SCHEMA_NAME);
        classes = schema->GetClasses ();
        FdoPtr<FdoClassDefinition> definition = classes->GetItem (NEW_CLASS_NAME);
        definition->Delete ();
        apply->SetFeatureSchema (schema);
        apply->Execute ();
		SaveSchema(mConnection);
    }
    catch (FdoException* ge) 
    {
        TestCommonFail (ge);
    }
}

/* Test apply operation with only identity and attribute, but no geometry. */
void SchemaTests::apply_three ()
{
    try
    {
        FdoString* NEW_SCHEMA_NAME = L"NewSchema";
        FdoString* NEW_CLASS_NAME = L"Test";

        // Clean up leftovers from previous tests:
        TestCommonSchemaUtil::CleanUpClass(mConnection, NULL, NEW_CLASS_NAME);

        FdoPtr<FdoIApplySchema> apply = (FdoIApplySchema*)mConnection->CreateCommand (FdoCommandType_ApplySchema);
        FdoPtr<FdoFeatureSchema> schema = FdoFeatureSchema::Create (NEW_SCHEMA_NAME, L"");
        FdoPtr<FdoClassCollection> classes = schema->GetClasses ();

        FdoPtr<FdoDataPropertyDefinition> id = FdoDataPropertyDefinition::Create (L"FeatId", L"integer");
        id->SetDataType (FdoDataType_Int32);
        FdoPtr<FdoDataPropertyDefinition> street = FdoDataPropertyDefinition::Create (L"Street", L"text");
        street->SetDataType (FdoDataType_String);
        street->SetLength (64);

        // assemble the class
        FdoPtr<FdoClass> feature = FdoClass::Create (NEW_CLASS_NAME, L"test class created with apply schema");
        FdoPtr<FdoPropertyDefinitionCollection> properties = feature->GetProperties ();
        properties->Add (id);
        properties->Add (street);
        FdoPtr<FdoDataPropertyDefinitionCollection> identities = feature->GetIdentityProperties ();
        identities->Add (id);

        // submit the new schema
        classes->Add (feature);
        apply->SetFeatureSchema (schema);
        apply->Execute ();
		SaveSchema(mConnection);

        // close and reopen the connection
        mConnection->Close ();
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->Open ());

        // check that the new schema shows up in the list
        FdoPtr<FdoIDescribeSchema> describe = (FdoIDescribeSchema*)mConnection->CreateCommand (FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> schemas = describe->Execute ();

        FdoPtr<FdoIDisposableCollection> collection = schemas->FindClass (NEW_CLASS_NAME);
        CPPUNIT_ASSERT_MESSAGE ("no class found", 1 == collection->GetCount ());
        FdoPtr<FdoClass> cls = (FdoClass *)collection->GetItem (0);
        CPPUNIT_ASSERT_MESSAGE ("wrong name", 0 == wcscmp (NEW_CLASS_NAME, cls->GetName ()));

        // check it's contents
        properties = cls->GetProperties ();
        FdoPtr<FdoDataPropertyDefinition> featid = (FdoDataPropertyDefinition*)properties->GetItem (L"FeatId");
        CPPUNIT_ASSERT_MESSAGE ("id wrong type", FdoDataType_Int32 == featid->GetDataType ());
        street = (FdoDataPropertyDefinition*)properties->GetItem (L"Street");
        CPPUNIT_ASSERT_MESSAGE ("street wrong type", FdoDataType_String == street->GetDataType ());

        // OK, now delete the class
        schema = schemas->GetItem (NEW_SCHEMA_NAME);
        classes = schema->GetClasses ();
        FdoPtr<FdoClassDefinition> definition = classes->GetItem (NEW_CLASS_NAME);
        definition->Delete ();
        apply->SetFeatureSchema (schema);
        apply->Execute ();
		SaveSchema(mConnection);
    }
    catch (FdoException* ge) 
    {
        TestCommonFail (ge);
    }
}

void SchemaTests::add_class_no_identity ()
{
    try
    {
        TestCommonSchemaUtil::CleanUpClass (mConnection, L"Default", L"Test");

        FdoPtr<FdoIDescribeSchema> describe = (FdoIDescribeSchema*)mConnection->CreateCommand (FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> schemas = describe->Execute ();
        CPPUNIT_ASSERT_MESSAGE ("Expecting 1 schema", 1 == schemas->GetCount ());
        FdoPtr<FdoFeatureSchema> schema = schemas->GetItem (0);
        FdoPtr<FdoClassCollection> classes = schema->GetClasses ();

        FdoPtr<FdoIApplySchema> apply = (FdoIApplySchema*)mConnection->CreateCommand (FdoCommandType_ApplySchema);

        FdoPtr<FdoDataPropertyDefinition> street = FdoDataPropertyDefinition::Create (L"Street", L"text");
        street->SetDataType (FdoDataType_String);
        street->SetLength (64);

        // build a location geometry property
        FdoPtr<FdoGeometricPropertyDefinition> location = FdoGeometricPropertyDefinition::Create (L"Geometry", L"geometry");
        location->SetGeometryTypes (FdoGeometricType_Point);
        location->SetHasElevation (true);
        location->SetHasMeasure (true);

        //// assemble the feature class
        FdoPtr<FdoFeatureClass> feature = FdoFeatureClass::Create (L"Test", L"test class created with apply schema");
        FdoPtr<FdoPropertyDefinitionCollection> properties = feature->GetProperties ();
        properties->Add (street);
        properties->Add (location);
        feature->SetGeometryProperty (location);

        // submit the new schema
        classes->Add (feature);
        apply->SetFeatureSchema (schema);
        apply->Execute ();
		SaveSchema(mConnection);

        // close and reopen the connection
        mConnection->Close ();
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->Open ());

        // check that the new schema shows up in the list
        describe = (FdoIDescribeSchema*)mConnection->CreateCommand (FdoCommandType_DescribeSchema);
        schemas = describe->Execute ();
        CPPUNIT_ASSERT_MESSAGE ("Expecting 1 schema", 1 == schemas->GetCount ());
        schema = schemas->GetItem (0);
        classes = schema->GetClasses ();
        FdoPtr<FdoFeatureClass> cls = (FdoFeatureClass*)classes->FindItem (L"Test");
        CPPUNIT_ASSERT_MESSAGE ("Test class not found", NULL != cls.p);

        // check that the definition is the same
        CPPUNIT_ASSERT_MESSAGE ("class bad description", 0 == (wcscmp (L"test class created with apply schema", cls->GetDescription ())));
        properties = cls->GetProperties ();
        FdoPtr<FdoDataPropertyDefinition> id = (FdoDataPropertyDefinition*)properties->FindItem (L"FeatId");
        CPPUNIT_ASSERT_MESSAGE ("featid not found", NULL != id.p);
        CPPUNIT_ASSERT_MESSAGE ("featid not FdoDataType_Int32", FdoDataType_Int32 == id->GetDataType ());
        street = (FdoDataPropertyDefinition*)properties->FindItem (L"Street");
        CPPUNIT_ASSERT_MESSAGE ("street not found", NULL != street.p);
        CPPUNIT_ASSERT_MESSAGE ("street not FdoDataType_String", FdoDataType_String == street->GetDataType ());
        CPPUNIT_ASSERT_MESSAGE ("street bad description", 0 == (wcscmp (L"text", street->GetDescription ())));
        location = (FdoGeometricPropertyDefinition*)properties->FindItem (L"Geometry");
        CPPUNIT_ASSERT_MESSAGE ("geometry not found", NULL != location.p);
        CPPUNIT_ASSERT_MESSAGE ("geometry bad description", 0 == (wcscmp (L"geometry", location->GetDescription ())));
        CPPUNIT_ASSERT_MESSAGE ("location not points", FdoGeometricType_Point == location->GetGeometryTypes ());
        CPPUNIT_ASSERT_MESSAGE ("location no elevation", location->GetHasElevation ());
        CPPUNIT_ASSERT_MESSAGE ("location no measure", location->GetHasMeasure ());
        FdoPtr<FdoGeometricPropertyDefinition> geometry = cls->GetGeometryProperty ();
        CPPUNIT_ASSERT_MESSAGE ("wrong geometry", location == geometry);
        FdoPtr<FdoDataPropertyDefinitionCollection> ids = cls->GetIdentityProperties ();
        FdoPtr<FdoDataPropertyDefinition> identifier = ids->FindItem (L"FeatId");
        CPPUNIT_ASSERT_MESSAGE ("wrong id", id == identifier);
    }
    catch (FdoException* e)
    {
        TestCommonFail (e);
    }
}

void SchemaTests::switch_schema ()
{
    try
    {
        FdoPtr<FdoIDescribeSchema> describe = (FdoIDescribeSchema*)mConnection->CreateCommand (FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> schemas = describe->Execute ();
        CPPUNIT_ASSERT_MESSAGE ("Expecting 1 schema", 1 == schemas->GetCount ());
        FdoPtr<FdoFeatureSchema> schema = schemas->GetItem (0);
        FdoPtr<FdoClassCollection> classes = schema->GetClasses ();

        // switch to another default file location (with a .prj file)
        mConnection->Close ();
#ifdef _WIN32
        mConnection->SetConnectionString (L"DefaultFileLocation=" LOCATION4);
#else
        mConnection->SetConnectionString (L"DefaultFileLocation=" LOCATION4);
#endif
		ShpTests::sLocation = LOCATION4;
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->Open ());
        describe = (FdoIDescribeSchema*)mConnection->CreateCommand (FdoCommandType_DescribeSchema);
        schemas = describe->Execute ();
        CPPUNIT_ASSERT_MESSAGE ("Expecting 1 schema", 1 == schemas->GetCount ());
        schema = schemas->GetItem (0);
        classes = schema->GetClasses ();

        // switch to yet another default file location (with a different .prj file)
        mConnection->Close ();
#ifdef _WIN32
        mConnection->SetConnectionString (L"DefaultFileLocation=" LOCATION5);
#else
        mConnection->SetConnectionString (L"DefaultFileLocation=" LOCATION5);
#endif
		ShpTests::sLocation = LOCATION5;
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->Open ());
        describe = (FdoIDescribeSchema*)mConnection->CreateCommand (FdoCommandType_DescribeSchema);
        schemas = describe->Execute ();
        CPPUNIT_ASSERT_MESSAGE ("Expecting 1 schema", 1 == schemas->GetCount ());
        schema = schemas->GetItem (0);
        classes = schema->GetClasses ();
    }
    catch (FdoException* e)
    {
        TestCommonFail (e);
    }
}

void SchemaTests::destroy_schema ()
{
    #ifdef _WIN32
    const wchar_t* file_name1 = L"..\\..\\TestData\\Ontario\\lakes";
    const wchar_t* file_name2 = L"..\\..\\TestData\\Oahu\\coundist01_n83";
    #else
    const wchar_t* file_name1 =  L"../../TestData/Ontario/lakes";
    const wchar_t* file_name2 =  L"../../TestData/Oahu/coundist01_n83";
    #endif

    try
    {
        copy (mConnection, L"Test1", file_name1);
        copy (mConnection, L"Test2", file_name2);
        mConnection->Close ();

        // destroy the schema
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->Open ());
        FdoPtr<FdoIDestroySchema> destroy = (FdoIDestroySchema*)mConnection->CreateCommand (FdoCommandType_DestroySchema);
        
		try 
		{
			destroy->SetSchemaName (L"Default");
			destroy->Execute ();
			CPPUNIT_ASSERT_MESSAGE ("Destroy schema should not succeed", false);
		}
		catch (FdoException* ge)
		{
			ge->Release();
		}

		// Delete data
		FdoPtr<FdoIDelete> del = (FdoIDelete*)mConnection->CreateCommand (FdoCommandType_Delete);

		del->SetFeatureClassName(L"Test1");
		del->Execute();
		del->SetFeatureClassName(L"Test2");
		del->Execute();

		// Now it shoud succeed
		destroy->Execute ();
		SaveSchema(mConnection);

        // check that it's not there
        FdoPtr<FdoIDescribeSchema> describe = (FdoIDescribeSchema*)mConnection->CreateCommand (FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> schemas = describe->Execute ();
        CPPUNIT_ASSERT_MESSAGE ("Expecting 0 schemas", 0 == schemas->GetCount ());
        mConnection->Close ();

        // check that it's not there even after a reopen
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->Open ());
        describe = (FdoIDescribeSchema*)mConnection->CreateCommand (FdoCommandType_DescribeSchema);
        schemas = describe->Execute ();
		// schema.xml is not generated any more during the ::copy operation, so we should find the default schema
        CPPUNIT_ASSERT_MESSAGE ("Expecting 0 schemas after reopen", 0 == schemas->GetCount ());
    }
    catch (FdoException* ge)
    {
        TestCommonFail (ge);
    }
}

void SchemaTests::non_ascii_property_name ()
{
    try
    {
        FdoString* NEW_SCHEMA_NAME = L"NewSchema";
        FdoString* NEW_CLASS_NAME = L"Test";

        // Clean up leftovers from previous tests:
        TestCommonSchemaUtil::CleanUpClass(mConnection, NULL, NEW_CLASS_NAME);


        FdoPtr<FdoIApplySchema> apply = (FdoIApplySchema*)mConnection->CreateCommand (FdoCommandType_ApplySchema);
        FdoPtr<FdoFeatureSchema> schema = FdoFeatureSchema::Create (NEW_SCHEMA_NAME, L"");
        FdoPtr<FdoClassCollection> classes = schema->GetClasses ();

        FdoPtr<FdoDataPropertyDefinition> id = FdoDataPropertyDefinition::Create (L"Id", L"integer");
        id->SetDataType (FdoDataType_Int32);
// French
        FdoPtr<FdoDataPropertyDefinition> street = FdoDataPropertyDefinition::Create (L"Str\x00E9et", L"text");
        street->SetDataType (FdoDataType_String);
        street->SetLength (64);
// Japanese
        FdoPtr<FdoDataPropertyDefinition> area = FdoDataPropertyDefinition::Create (L"Area\x5348\x524d", L"double");
        area->SetDataType (FdoDataType_Decimal);
        area->SetPrecision (20);
        area->SetScale (8);
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
        properties->Add (location);
        feature->SetGeometryProperty (location);
        FdoPtr<FdoDataPropertyDefinitionCollection> identities = feature->GetIdentityProperties ();
        identities->Add (id);

        // submit the new schema
        classes->Add (feature);
        apply->SetFeatureSchema (schema);
        apply->Execute ();
		SaveSchema(mConnection);

        // close and reopen the connection
        mConnection->Close ();
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->Open ());

        // check that the new schema shows up in the list
        FdoPtr<FdoIDescribeSchema> describe = (FdoIDescribeSchema*)mConnection->CreateCommand (FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> schemas = describe->Execute ();

        FdoPtr<FdoIDisposableCollection> collection = schemas->FindClass (NEW_CLASS_NAME);
        CPPUNIT_ASSERT_MESSAGE ("no class found", 1 == collection->GetCount ());
        FdoPtr<FdoFeatureClass> cls = (FdoFeatureClass *)collection->GetItem (0);
        CPPUNIT_ASSERT_MESSAGE ("wrong name", 0 == wcscmp (NEW_CLASS_NAME, cls->GetName ()));
        CPPUNIT_ASSERT_MESSAGE ("wrong description", 0 == wcscmp (L"test class created with apply schema", cls->GetDescription ()));

        // check it's contents
        properties = cls->GetProperties ();
        FdoPtr<FdoDataPropertyDefinition> featid = (FdoDataPropertyDefinition*)properties->GetItem (L"Id");
        CPPUNIT_ASSERT_MESSAGE ("id wrong type", FdoDataType_Int32 == featid->GetDataType ());
        street = (FdoDataPropertyDefinition*)properties->GetItem (L"Str\x00E9et");
        CPPUNIT_ASSERT_MESSAGE ("street wrong type", FdoDataType_String == street->GetDataType ());
        CPPUNIT_ASSERT_MESSAGE ("street wrong size", 64 == street->GetLength ());
        area = (FdoDataPropertyDefinition*)properties->GetItem (L"Area\x5348\x524d");
        CPPUNIT_ASSERT_MESSAGE ("area wrong type", FdoDataType_Decimal == area->GetDataType ());
        location = (FdoGeometricPropertyDefinition*)properties->GetItem (L"Geometry");
        CPPUNIT_ASSERT_MESSAGE ("wrong geometry types", FdoGeometricType_Point == location->GetGeometryTypes ());
        CPPUNIT_ASSERT_MESSAGE ("wrong elevation", location->GetHasElevation ());
        CPPUNIT_ASSERT_MESSAGE ("wrong measure", location->GetHasMeasure ());

        // OK, now delete the class
        schema = schemas->GetItem (NEW_SCHEMA_NAME);
        classes = schema->GetClasses ();
        FdoPtr<FdoClassDefinition> definition = classes->GetItem (NEW_CLASS_NAME);
        definition->Delete ();
        apply->SetFeatureSchema (schema);
        apply->Execute ();
		SaveSchema(mConnection);
    }
    catch (FdoException* ge) 
    {
        TestCommonFail (ge);
    }
}

void SchemaTests::non_ascii_class_name1 ()
{
    try
    {
        FdoString* NEW_SCHEMA_NAME = L"NewSchema";
        FdoString* NEW_CLASS_NAME = L"T\x00E9st";

        // Clean up leftovers from previous tests:
        TestCommonSchemaUtil::CleanUpClass(mConnection, NULL, NEW_CLASS_NAME);

        FdoPtr<FdoIApplySchema> apply = (FdoIApplySchema*)mConnection->CreateCommand (FdoCommandType_ApplySchema);
        FdoPtr<FdoFeatureSchema> schema = FdoFeatureSchema::Create (NEW_SCHEMA_NAME, L"");
        FdoPtr<FdoClassCollection> classes = schema->GetClasses ();

        FdoPtr<FdoDataPropertyDefinition> id = FdoDataPropertyDefinition::Create (L"Id", L"integer");
        id->SetDataType (FdoDataType_Int32);
// French
        FdoPtr<FdoDataPropertyDefinition> street = FdoDataPropertyDefinition::Create (L"Str\x00E9et", L"text");
        street->SetDataType (FdoDataType_String);
        street->SetLength (64);
// Japanese
        FdoPtr<FdoDataPropertyDefinition> area = FdoDataPropertyDefinition::Create (L"Area\x5348\x524d", L"double");
        area->SetDataType (FdoDataType_Decimal);
        area->SetPrecision (20);
        area->SetScale (8);
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
        properties->Add (location);
        feature->SetGeometryProperty (location);
        FdoPtr<FdoDataPropertyDefinitionCollection> identities = feature->GetIdentityProperties ();
        identities->Add (id);

        // submit the new schema
        classes->Add (feature);
        apply->SetFeatureSchema (schema);
        apply->Execute ();
		SaveSchema(mConnection);

        // close and reopen the connection
        mConnection->Close ();
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->Open ());

        // check that the new schema shows up in the list
        FdoPtr<FdoIDescribeSchema> describe = (FdoIDescribeSchema*)mConnection->CreateCommand (FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> schemas = describe->Execute ();

        FdoPtr<FdoIDisposableCollection> collection = schemas->FindClass (NEW_CLASS_NAME);
        CPPUNIT_ASSERT_MESSAGE ("no class found", 1 == collection->GetCount ());
        FdoPtr<FdoFeatureClass> cls = (FdoFeatureClass *)collection->GetItem (0);
        CPPUNIT_ASSERT_MESSAGE ("wrong name", 0 == wcscmp (NEW_CLASS_NAME, cls->GetName ()));
        CPPUNIT_ASSERT_MESSAGE ("wrong description", 0 == wcscmp (L"test class created with apply schema", cls->GetDescription ()));

        // check it's contents
        properties = cls->GetProperties ();
        FdoPtr<FdoDataPropertyDefinition> featid = (FdoDataPropertyDefinition*)properties->GetItem (L"Id");
        CPPUNIT_ASSERT_MESSAGE ("id wrong type", FdoDataType_Int32 == featid->GetDataType ());
        street = (FdoDataPropertyDefinition*)properties->GetItem (L"Str\x00E9et");
        CPPUNIT_ASSERT_MESSAGE ("street wrong type", FdoDataType_String == street->GetDataType ());
        CPPUNIT_ASSERT_MESSAGE ("street wrong size", 64 == street->GetLength ());
        area = (FdoDataPropertyDefinition*)properties->GetItem (L"Area\x5348\x524d");
        CPPUNIT_ASSERT_MESSAGE ("area wrong type", FdoDataType_Decimal == area->GetDataType ());
        location = (FdoGeometricPropertyDefinition*)properties->GetItem (L"Geometry");
        CPPUNIT_ASSERT_MESSAGE ("wrong geometry types", FdoGeometricType_Point == location->GetGeometryTypes ());
        CPPUNIT_ASSERT_MESSAGE ("wrong elevation", location->GetHasElevation ());
        CPPUNIT_ASSERT_MESSAGE ("wrong measure", location->GetHasMeasure ());

        // OK, now delete the class
        schema = schemas->GetItem (NEW_SCHEMA_NAME);
        classes = schema->GetClasses ();
        FdoPtr<FdoClassDefinition> definition = classes->GetItem (NEW_CLASS_NAME);
        definition->Delete ();
        apply->SetFeatureSchema (schema);
        apply->Execute ();
		SaveSchema(mConnection);
    }
    catch (FdoException* ge) 
    {
        TestCommonFail (ge);
    }
}

void SchemaTests::non_ascii_class_name2 ()
{
    try
    {
        FdoString* NEW_SCHEMA_NAME = L"NewSchema";
        FdoString* NEW_CLASS_NAME = L"T\x5348\x524dst";

        // Clean up leftovers from previous tests:
        TestCommonSchemaUtil::CleanUpClass(mConnection, NULL, NEW_CLASS_NAME);

        FdoPtr<FdoIApplySchema> apply = (FdoIApplySchema*)mConnection->CreateCommand (FdoCommandType_ApplySchema);
        FdoPtr<FdoFeatureSchema> schema = FdoFeatureSchema::Create (NEW_SCHEMA_NAME, L"");
        FdoPtr<FdoClassCollection> classes = schema->GetClasses ();

        FdoPtr<FdoDataPropertyDefinition> id = FdoDataPropertyDefinition::Create (L"Id", L"integer");
        id->SetDataType (FdoDataType_Int32);
// French
        FdoPtr<FdoDataPropertyDefinition> street = FdoDataPropertyDefinition::Create (L"Str\x00E9et", L"text");
        street->SetDataType (FdoDataType_String);
        street->SetLength (64);
// Japanese
        FdoPtr<FdoDataPropertyDefinition> area = FdoDataPropertyDefinition::Create (L"Area\x5348\x524d", L"double");
        area->SetDataType (FdoDataType_Decimal);
        area->SetPrecision (20);
        area->SetScale (8);
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
        properties->Add (location);
        feature->SetGeometryProperty (location);
        FdoPtr<FdoDataPropertyDefinitionCollection> identities = feature->GetIdentityProperties ();
        identities->Add (id);

        // submit the new schema
        classes->Add (feature);
        apply->SetFeatureSchema (schema);
        apply->Execute ();
		SaveSchema(mConnection);

        // close and reopen the connection
        mConnection->Close ();
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->Open ());

        // check that the new schema shows up in the list
        FdoPtr<FdoIDescribeSchema> describe = (FdoIDescribeSchema*)mConnection->CreateCommand (FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> schemas = describe->Execute ();

        FdoPtr<FdoIDisposableCollection> collection = schemas->FindClass (NEW_CLASS_NAME);
        CPPUNIT_ASSERT_MESSAGE ("no class found", 1 == collection->GetCount ());
        FdoPtr<FdoFeatureClass> cls = (FdoFeatureClass *)collection->GetItem (0);
        CPPUNIT_ASSERT_MESSAGE ("wrong name", 0 == wcscmp (NEW_CLASS_NAME, cls->GetName ()));
        CPPUNIT_ASSERT_MESSAGE ("wrong description", 0 == wcscmp (L"test class created with apply schema", cls->GetDescription ()));

        // check it's contents
        properties = cls->GetProperties ();
        FdoPtr<FdoDataPropertyDefinition> featid = (FdoDataPropertyDefinition*)properties->GetItem (L"Id");
        CPPUNIT_ASSERT_MESSAGE ("id wrong type", FdoDataType_Int32 == featid->GetDataType ());
        street = (FdoDataPropertyDefinition*)properties->GetItem (L"Str\x00E9et");
        CPPUNIT_ASSERT_MESSAGE ("street wrong type", FdoDataType_String == street->GetDataType ());
        CPPUNIT_ASSERT_MESSAGE ("street wrong size", 64 == street->GetLength ());
        area = (FdoDataPropertyDefinition*)properties->GetItem (L"Area\x5348\x524d");
        CPPUNIT_ASSERT_MESSAGE ("area wrong type", FdoDataType_Decimal == area->GetDataType ());
        location = (FdoGeometricPropertyDefinition*)properties->GetItem (L"Geometry");
        CPPUNIT_ASSERT_MESSAGE ("wrong geometry types", FdoGeometricType_Point == location->GetGeometryTypes ());
        CPPUNIT_ASSERT_MESSAGE ("wrong elevation", location->GetHasElevation ());
        CPPUNIT_ASSERT_MESSAGE ("wrong measure", location->GetHasMeasure ());

        // OK, now delete the class
        schema = schemas->GetItem (NEW_SCHEMA_NAME);
        classes = schema->GetClasses ();
        FdoPtr<FdoClassDefinition> definition = classes->GetItem (NEW_CLASS_NAME);
        definition->Delete ();
        apply->SetFeatureSchema (schema);
        apply->Execute ();
		SaveSchema(mConnection);
    }
    catch (FdoException* ge) 
    {
        TestCommonFail (ge);
    }
}

void SchemaTests::non_ascii_schema_name ()
{
    try
    {
        FdoString* NEW_SCHEMA_NAME = L"N\x5348\x524dwSchema";
        FdoString* NEW_CLASS_NAME = L"T\x5348\x524dst";

        // Clean up leftovers from previous tests:
        TestCommonSchemaUtil::CleanUpClass(mConnection, NULL, NEW_CLASS_NAME);

        FdoPtr<FdoIApplySchema> apply = (FdoIApplySchema*)mConnection->CreateCommand (FdoCommandType_ApplySchema);
        FdoPtr<FdoFeatureSchema> schema = FdoFeatureSchema::Create (NEW_SCHEMA_NAME, L"");
        FdoPtr<FdoClassCollection> classes = schema->GetClasses ();

        FdoPtr<FdoDataPropertyDefinition> id = FdoDataPropertyDefinition::Create (L"Id", L"integer");
        id->SetDataType (FdoDataType_Int32);
// French
        FdoPtr<FdoDataPropertyDefinition> street = FdoDataPropertyDefinition::Create (L"Str\x00E9et", L"text");
        street->SetDataType (FdoDataType_String);
        street->SetLength (64);
// Japanese
        FdoPtr<FdoDataPropertyDefinition> area = FdoDataPropertyDefinition::Create (L"Area\x5348\x524d", L"double");
        area->SetDataType (FdoDataType_Decimal);
        area->SetPrecision (20);
        area->SetScale (8);
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
        properties->Add (location);
        feature->SetGeometryProperty (location);
        FdoPtr<FdoDataPropertyDefinitionCollection> identities = feature->GetIdentityProperties ();
        identities->Add (id);

        // submit the new schema
        classes->Add (feature);
        apply->SetFeatureSchema (schema);
        apply->Execute ();
		SaveSchema(mConnection);

        // close and reopen the connection
        mConnection->Close ();
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->Open ());

        // check that the new schema shows up in the list
        FdoPtr<FdoIDescribeSchema> describe = (FdoIDescribeSchema*)mConnection->CreateCommand (FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> schemas = describe->Execute ();

        FdoPtr<FdoIDisposableCollection> collection = schemas->FindClass (NEW_CLASS_NAME);
        CPPUNIT_ASSERT_MESSAGE ("no class found", 1 == collection->GetCount ());
        FdoPtr<FdoFeatureClass> cls = (FdoFeatureClass *)collection->GetItem (0);
        CPPUNIT_ASSERT_MESSAGE ("wrong name", 0 == wcscmp (NEW_CLASS_NAME, cls->GetName ()));
        CPPUNIT_ASSERT_MESSAGE ("wrong description", 0 == wcscmp (L"test class created with apply schema", cls->GetDescription ()));

        // check it's contents
        properties = cls->GetProperties ();
        FdoPtr<FdoDataPropertyDefinition> featid = (FdoDataPropertyDefinition*)properties->GetItem (L"Id");
        CPPUNIT_ASSERT_MESSAGE ("id wrong type", FdoDataType_Int32 == featid->GetDataType ());
        street = (FdoDataPropertyDefinition*)properties->GetItem (L"Str\x00E9et");
        CPPUNIT_ASSERT_MESSAGE ("street wrong type", FdoDataType_String == street->GetDataType ());
        CPPUNIT_ASSERT_MESSAGE ("street wrong size", 64 == street->GetLength ());
        area = (FdoDataPropertyDefinition*)properties->GetItem (L"Area\x5348\x524d");
        CPPUNIT_ASSERT_MESSAGE ("area wrong type", FdoDataType_Decimal == area->GetDataType ());
        location = (FdoGeometricPropertyDefinition*)properties->GetItem (L"Geometry");
        CPPUNIT_ASSERT_MESSAGE ("wrong geometry types", FdoGeometricType_Point == location->GetGeometryTypes ());
        CPPUNIT_ASSERT_MESSAGE ("wrong elevation", location->GetHasElevation ());
        CPPUNIT_ASSERT_MESSAGE ("wrong measure", location->GetHasMeasure ());

        // OK, now delete the class
        schema = schemas->GetItem (NEW_SCHEMA_NAME);
        classes = schema->GetClasses ();
        FdoPtr<FdoClassDefinition> definition = classes->GetItem (NEW_CLASS_NAME);
        definition->Delete ();
        apply->SetFeatureSchema (schema);
        apply->Execute ();
		SaveSchema(mConnection);
    }
    catch (FdoException* ge) 
    {
        TestCommonFail (ge);
    }
}

void SchemaTests::non_ascii_directory ()
{
    try
    {
        try
        {
            mConnection->Close ();
            if (!FdoCommonFile::FileExists (LOCATION2))
                FdoCommonFile::MkDir (LOCATION2);
            mConnection = ShpTests::GetConnection ();
            mConnection->SetConnectionString (L"DefaultFileLocation=" LOCATION2);
			ShpTests::sLocation = LOCATION2;
            CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->Open ());
        }
        catch (FdoException* ge)
        {
            TestCommonFail (ge);
        }
        FdoString* NEW_SCHEMA_NAME = L"N\x5348\x524dwSchema";
        FdoString* NEW_CLASS_NAME = L"T\x5348\x524dst";

        // Clean up leftovers from previous tests:
        TestCommonSchemaUtil::CleanUpClass(mConnection, NULL, NEW_CLASS_NAME);

        FdoPtr<FdoIApplySchema> apply = (FdoIApplySchema*)mConnection->CreateCommand (FdoCommandType_ApplySchema);
        FdoPtr<FdoFeatureSchema> schema = FdoFeatureSchema::Create (NEW_SCHEMA_NAME, L"");
        FdoPtr<FdoClassCollection> classes = schema->GetClasses ();

        FdoPtr<FdoDataPropertyDefinition> id = FdoDataPropertyDefinition::Create (L"Id", L"integer");
        id->SetDataType (FdoDataType_Int32);
// French
        FdoPtr<FdoDataPropertyDefinition> street = FdoDataPropertyDefinition::Create (L"Str\x00E9et", L"text");
        street->SetDataType (FdoDataType_String);
        street->SetLength (64);
// Japanese
        FdoPtr<FdoDataPropertyDefinition> area = FdoDataPropertyDefinition::Create (L"Area\x5348\x524d", L"double");
        area->SetDataType (FdoDataType_Decimal);
        area->SetPrecision (20);
        area->SetScale (8);
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
        properties->Add (location);
        feature->SetGeometryProperty (location);
        FdoPtr<FdoDataPropertyDefinitionCollection> identities = feature->GetIdentityProperties ();
        identities->Add (id);

        // submit the new schema
        classes->Add (feature);
        apply->SetFeatureSchema (schema);
        apply->Execute ();
		SaveSchema(mConnection);

        // close and reopen the connection
        mConnection->Close ();
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->Open ());

        // check that the new schema shows up in the list
        FdoPtr<FdoIDescribeSchema> describe = (FdoIDescribeSchema*)mConnection->CreateCommand (FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> schemas = describe->Execute ();

        FdoPtr<FdoIDisposableCollection> collection = schemas->FindClass (NEW_CLASS_NAME);
        CPPUNIT_ASSERT_MESSAGE ("no class found", 1 == collection->GetCount ());
        FdoPtr<FdoFeatureClass> cls = (FdoFeatureClass *)collection->GetItem (0);
        CPPUNIT_ASSERT_MESSAGE ("wrong name", 0 == wcscmp (NEW_CLASS_NAME, cls->GetName ()));
        CPPUNIT_ASSERT_MESSAGE ("wrong description", 0 == wcscmp (L"test class created with apply schema", cls->GetDescription ()));

        // check it's contents
        properties = cls->GetProperties ();
        FdoPtr<FdoDataPropertyDefinition> featid = (FdoDataPropertyDefinition*)properties->GetItem (L"Id");
        CPPUNIT_ASSERT_MESSAGE ("id wrong type", FdoDataType_Int32 == featid->GetDataType ());
        street = (FdoDataPropertyDefinition*)properties->GetItem (L"Str\x00E9et");
        CPPUNIT_ASSERT_MESSAGE ("street wrong type", FdoDataType_String == street->GetDataType ());
        CPPUNIT_ASSERT_MESSAGE ("street wrong size", 64 == street->GetLength ());
        area = (FdoDataPropertyDefinition*)properties->GetItem (L"Area\x5348\x524d");
        CPPUNIT_ASSERT_MESSAGE ("area wrong type", FdoDataType_Decimal == area->GetDataType ());
        location = (FdoGeometricPropertyDefinition*)properties->GetItem (L"Geometry");
        CPPUNIT_ASSERT_MESSAGE ("wrong geometry types", FdoGeometricType_Point == location->GetGeometryTypes ());
        CPPUNIT_ASSERT_MESSAGE ("wrong elevation", location->GetHasElevation ());
        CPPUNIT_ASSERT_MESSAGE ("wrong measure", location->GetHasMeasure ());

        // OK, now delete the class
        schema = schemas->GetItem (NEW_SCHEMA_NAME);
        classes = schema->GetClasses ();
        FdoPtr<FdoClassDefinition> definition = classes->GetItem (NEW_CLASS_NAME);
        definition->Delete ();
        apply->SetFeatureSchema (schema);
        apply->Execute ();
		SaveSchema(mConnection);

        mConnection->Close ();
        if (FdoCommonFile::FileExists (LOCATION2 L"schema.xml"))
            FdoCommonFile::Delete (LOCATION2 L"schema.xml");
        if (FdoCommonFile::FileExists (LOCATION2))
            FdoCommonFile::RmDir (LOCATION2);
        mConnection->SetConnectionString (L"DefaultFileLocation=" LOCATION);
		ShpTests::sLocation = LOCATION;
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->Open ());
    }
    catch (FdoException* ge) 
    {
        TestCommonFail (ge);
    }
}



void SchemaTests::modify_class()
{
    try
    {
        FdoString* NEW_SCHEMA_NAME = L"NewSchemaModify";
        FdoString* NEW_CLASS_NAME = L"TestModify";

        // Clean up leftovers from previous tests:
        TestCommonSchemaUtil::CleanUpClass(mConnection, NULL, NEW_CLASS_NAME);


        // Create new schema with one class :
        ///////////////////////////////////////////////////////////////////////

        FdoPtr<FdoIApplySchema> apply = (FdoIApplySchema*)mConnection->CreateCommand (FdoCommandType_ApplySchema);
        FdoPtr<FdoFeatureSchema> schema = FdoFeatureSchema::Create (NEW_SCHEMA_NAME, L"");
        FdoPtr<FdoClassCollection> classes = schema->GetClasses ();

        FdoPtr<FdoDataPropertyDefinition> id = FdoDataPropertyDefinition::Create (L"Id", L"integer");
        id->SetDataType (FdoDataType_Int32);
        id->SetIsAutoGenerated(true);
        id->SetReadOnly(true);

        FdoPtr<FdoDataPropertyDefinition> street = FdoDataPropertyDefinition::Create (L"Street", L"text");
        street->SetDataType (FdoDataType_String);
        street->SetLength (64);

        // build a location geometry property
        FdoPtr<FdoGeometricPropertyDefinition> location = FdoGeometricPropertyDefinition::Create (L"Geometry", L"geometry");
        location->SetGeometryTypes (FdoGeometricType_Point);
        location->SetHasElevation (true);
        location->SetHasMeasure (true);

        // assemble the feature class
        FdoPtr<FdoFeatureClass> feature = FdoFeatureClass::Create (NEW_CLASS_NAME, L"test class created with apply schema");
        FdoPtr<FdoPropertyDefinitionCollection> properties = feature->GetProperties ();
        properties->Add (id);
        properties->Add (location);
        properties->Add (street);
        feature->SetGeometryProperty (location);
        FdoPtr<FdoDataPropertyDefinitionCollection> identities = feature->GetIdentityProperties ();
        identities->Add (id);

        // submit the new schema
        classes->Add (feature);
        apply->SetFeatureSchema (schema);
        apply->Execute ();
		SaveSchema(mConnection);


        // Modify the empty class (add 1 property, remove 1 property, should work with no exceptions thrown):
        ///////////////////////////////////////////////////////////////////////

        // Fetch back the new class
        FdoPtr<FdoIDescribeSchema> descSchema = (FdoIDescribeSchema*)mConnection->CreateCommand(FdoCommandType_DescribeSchema);
        descSchema->SetSchemaName(NEW_SCHEMA_NAME);
        FdoPtr<FdoFeatureSchemaCollection> schemas = descSchema->Execute();
        schema = schemas->GetItem(NEW_SCHEMA_NAME);
        classes = schema->GetClasses();
        FdoPtr<FdoClassDefinition> classDef = classes->GetItem(NEW_CLASS_NAME);
        FdoPtr<FdoPropertyDefinitionCollection> props = classDef->GetProperties();

        // Modify this class
        FdoPtr<FdoDataPropertyDefinition> addedProp = FdoDataPropertyDefinition::Create(L"NewDateTime", L"");
        addedProp->SetDataType(FdoDataType_DateTime);
        props->Add(addedProp);
        FdoPtr<FdoPropertyDefinition> removedProp = props->GetItem(L"Street");
        props->Remove(removedProp);
        apply->SetFeatureSchema(schema);
        apply->Execute();

        // Verify that it has been modified:
        descSchema->SetSchemaName(NEW_SCHEMA_NAME);
        schemas = descSchema->Execute();
        schema = schemas->GetItem(NEW_SCHEMA_NAME);
        classes = schema->GetClasses();
        classDef = classes->GetItem(NEW_CLASS_NAME);
        props = classDef->GetProperties();
        FdoPtr<FdoPropertyDefinition> addedPropCheck = props->FindItem(L"NewDateTime");
        CPPUNIT_ASSERT_MESSAGE("Added Property not found", addedPropCheck != NULL);
        FdoPtr<FdoPropertyDefinition> removedPropCheck = props->FindItem(L"Street");
        CPPUNIT_ASSERT_MESSAGE("Removed Property found", removedPropCheck == NULL);


        // Add one row of data:
        ///////////////////////////////////////////////////////////////////////

        FdoPtr<FdoIInsert> insert = (FdoIInsert*)mConnection->CreateCommand(FdoCommandType_Insert);
        insert->SetFeatureClassName(NEW_CLASS_NAME);
        FdoPtr<FdoPropertyValueCollection> propValues = insert->GetPropertyValues();
        FdoDateTime dateTime(2006, 2, (FdoInt8)3);
        FdoPtr<FdoPropertyValue> propValue = FdoPropertyValue::Create(L"NewDateTime", FdoPtr<FdoValueExpression>(FdoDateTimeValue::Create(dateTime)));
        propValues->Add(propValue);
        FdoPtr<FdoIFeatureReader> reader = insert->Execute();
        reader->Close();


        // Modify the non-empty class (expect exception):
        ///////////////////////////////////////////////////////////////////////

        // Fetch back the class
        descSchema = (FdoIDescribeSchema*)mConnection->CreateCommand(FdoCommandType_DescribeSchema);
        descSchema->SetSchemaName(NEW_SCHEMA_NAME);
        schemas = descSchema->Execute();
        schema = schemas->GetItem(NEW_SCHEMA_NAME);
        classes = schema->GetClasses();
        classDef = classes->GetItem(NEW_CLASS_NAME);
        props = classDef->GetProperties();

        // Modify this class
        FdoPtr<FdoDataPropertyDefinition> addedStringProp = FdoDataPropertyDefinition::Create(L"NewString", L"");
        addedStringProp->SetDataType(FdoDataType_String);
        addedStringProp->SetLength(50);
        props->Add(addedStringProp);
        apply->SetFeatureSchema(schema);
        try
        {
            apply->Execute();
            CPPUNIT_FAIL("Expected an exception when modifying a non-empty class, didn't get one.");
        }
        catch (FdoException *e)
        {
            e->Release();
            // we want to get an exception
        }

    }
    catch (FdoException* ge) 
    {
        TestCommonFail (ge);
    }
}


void SchemaTests::ignore_states()
{
    try
    {
        FdoString* NEW_SCHEMA_NAME = L"NewSchemaModify";
        FdoString* NEW_CLASS_NAME = L"TestModify";

        // Clean up leftovers from previous tests:
        TestCommonSchemaUtil::CleanUpClass(mConnection, NULL, NEW_CLASS_NAME);


        // Create new schema with one class :
        ///////////////////////////////////////////////////////////////////////

        FdoPtr<FdoIApplySchema> apply = (FdoIApplySchema*)mConnection->CreateCommand (FdoCommandType_ApplySchema);
        FdoPtr<FdoFeatureSchema> schema = FdoFeatureSchema::Create (NEW_SCHEMA_NAME, L"");
        FdoPtr<FdoClassCollection> classes = schema->GetClasses ();

        FdoPtr<FdoDataPropertyDefinition> id = FdoDataPropertyDefinition::Create (L"Id", L"integer");
        id->SetDataType (FdoDataType_Int32);
        id->SetIsAutoGenerated(true);
        id->SetReadOnly(true);

        FdoPtr<FdoDataPropertyDefinition> street = FdoDataPropertyDefinition::Create (L"Street", L"text");
        street->SetDataType (FdoDataType_String);
        street->SetLength (64);

        // build a location geometry property
        FdoPtr<FdoGeometricPropertyDefinition> location = FdoGeometricPropertyDefinition::Create (L"Geometry", L"geometry");
        location->SetGeometryTypes (FdoGeometricType_Point);
        location->SetHasElevation (true);
        location->SetHasMeasure (true);

        // assemble the feature class
        FdoPtr<FdoFeatureClass> feature = FdoFeatureClass::Create (NEW_CLASS_NAME, L"test class created with apply schema");
        FdoPtr<FdoPropertyDefinitionCollection> properties = feature->GetProperties ();
        properties->Add (id);
        properties->Add (location);
        properties->Add (street);
        feature->SetGeometryProperty (location);
        FdoPtr<FdoDataPropertyDefinitionCollection> identities = feature->GetIdentityProperties ();
        identities->Add (id);

        classes->Add (feature);

        // Set the element state to Unchanged by using DeepCopy:
        FdoPtr<FdoFeatureSchema> schemaCopy = FdoCommonSchemaUtil::DeepCopyFdoFeatureSchema(schema);

        classes = schemaCopy->GetClasses();
        FdoPtr<FdoClassDefinition> classDef = classes->GetItem(NEW_CLASS_NAME);
        FdoSchemaElementState newClassState = classDef->GetElementState();
        CPPUNIT_ASSERT_MESSAGE("wrong element state; this test may not run correctly", newClassState==FdoSchemaElementState_Unchanged);

        // submit the new schema, it should get created without exceptions:
        apply->SetFeatureSchema (schemaCopy);
        apply->SetIgnoreStates(true);  // ignore element states
        apply->Execute ();
		SaveSchema(mConnection);

        // Verify that the class was added, event though its element state at the time of ApplySchema was set to "Unchanged":
        FdoPtr<FdoIDescribeSchema> descSchema = (FdoIDescribeSchema*)mConnection->CreateCommand(FdoCommandType_DescribeSchema);
        descSchema->SetSchemaName(NEW_SCHEMA_NAME);
        FdoPtr<FdoFeatureSchemaCollection> schemas = descSchema->Execute();
        schema = schemas->GetItem(NEW_SCHEMA_NAME);
        classes = schema->GetClasses();
        classDef = classes->FindItem(NEW_CLASS_NAME);
        CPPUNIT_ASSERT_MESSAGE("Class failed to get created when ignorestates==true and elementstate==unchanged", classDef != NULL);
    }
    catch (FdoException* ge) 
    {
        TestCommonFail (ge);
    }
}


void SchemaTests::apply_schema()
{
    try
	{
		FdoString* fileName = L"test";

        mConnection->Close ();

		FdoPtr<FdoIConnectionInfo> cinfo = mConnection->GetConnectionInfo();
		FdoPtr<FdoIConnectionPropertyDictionary> didc = cinfo->GetConnectionProperties();
		didc->SetProperty(L"DefaultFileLocation", LOCATION);
		didc->SetProperty(L"TemporaryFileLocation",L"");

		mConnection->Open();
		//Generating default data property
		FdoPtr<FdoDataPropertyDefinition> featid = FdoDataPropertyDefinition::Create(L"FeatId",L"Default identity property");
		featid->SetDataType(::FdoDataType_Int32);
		featid->SetIsAutoGenerated(false);
		featid->SetNullable(false);
	    
		//Generating default geometric property
		FdoPtr<FdoGeometricPropertyDefinition> geometry=FdoGeometricPropertyDefinition::Create(L"Geometry",L"Default geometry property");
		geometry->SetGeometryTypes(FdoGeometricType_Point);
	    
		//Generating default feature class
		FdoPtr<FdoFeatureClass> featureclass1=FdoFeatureClass::Create(L"FeatureClass1",L"Default feature class");
		FdoPtr<FdoPropertyDefinitionCollection> properties=featureclass1->GetProperties();
		properties->Add(featid);
		properties->Add(geometry);
		FdoPtr<FdoDataPropertyDefinitionCollection> identifier=featureclass1->GetIdentityProperties();
		identifier->Add(featid);
		featureclass1->SetGeometryProperty(geometry);

		//Generating default schema
		FdoPtr<FdoFeatureSchema> schema1=FdoFeatureSchema::Create(L"Schema1",L"Default schema");
		FdoClassesP(schema1->GetClasses()->Add(featureclass1));

		//FdoPtr<FdoFeatureSchema> schema = schema1->GetItem(0);
		FdoPtr<FdoClassCollection> coll = schema1->GetClasses();
		FdoPtr<FdoClassDefinition> def = coll->GetItem(0);      
		def->SetName(fileName);               
	 
		FdoPtr<FdoIApplySchema> apply = (FdoIApplySchema*) mConnection->CreateCommand(FdoCommandType_ApplySchema);
		apply->SetFeatureSchema(schema1);
		apply->Execute();
	}
    catch (FdoException* ge) 
    {
        TestCommonFail (ge);
    }
}

void SchemaTests::test_create_PRJ()
{
	FdoString	*wkt = L"PROJCS[\"NAD_1927_StatePlane_California_I_FIPS_0401\",GEOGCS[\"GCS_North_American_1927\",DATUM[\"D_North_American_1927\",SPHEROID[\"Clarke_1866\",6378206.4,294.9786982]],PRIMEM[\"Greenwich\",0],UNIT[\"Degree\",0.017453292519943295]],PROJECTION[\"Lambert_Conformal_Conic\"],PARAMETER[\"False_Easting\",2000000],PARAMETER[\"False_Northing\",0],PARAMETER[\"Central_Meridian\",-122],PARAMETER[\"Standard_Parallel_1\",40],PARAMETER[\"Standard_Parallel_2\",41.66666666666666],PARAMETER[\"Latitude_Of_Origin\",39.33333333333334],UNIT[\"Foot_US\",0.30480060960121924]]";
    
	try
	{
        FdoString* NEW_SCHEMA_NAME = L"NewSchema";
        FdoString* NEW_CLASS_NAME = L"TestSC";

        mConnection->Close ();

		///////// STEP I - create a spatial context, a class and an geometry association
		FdoPtr<FdoIConnectionInfo> cinfo = mConnection->GetConnectionInfo();
		FdoPtr<FdoIConnectionPropertyDictionary> didc = cinfo->GetConnectionProperties();
		didc->SetProperty(L"DefaultFileLocation", LOCATION7);
		didc->SetProperty(L"TemporaryFileLocation",L"");

		mConnection->Open();

		// Clean up leftovers from previous tests:
        TestCommonSchemaUtil::CleanUpClass(mConnection, NULL, NEW_CLASS_NAME);

		//Generating default data property
		FdoPtr<FdoDataPropertyDefinition> featid = FdoDataPropertyDefinition::Create(L"FeatId",L"Default identity property");
		featid->SetDataType(::FdoDataType_Int32);
		featid->SetIsAutoGenerated(false);
		featid->SetNullable(false);
	    
		//Generating default geometric property
		FdoPtr<FdoGeometricPropertyDefinition> geometry=FdoGeometricPropertyDefinition::Create(L"Geometry",L"Default geometry property");
		geometry->SetGeometryTypes(FdoGeometricType_Point);
	    
		// Create a new spatial context
        FdoPtr<FdoICreateSpatialContext> cscCmd = (FdoICreateSpatialContext *)mConnection->CreateCommand( FdoCommandType_CreateSpatialContext );

        cscCmd->SetName(L"");	// Leave it empty to avoid confict with WKT
        cscCmd->SetDescription(L"test");
        cscCmd->SetCoordinateSystem(L""); // Leave it empty to avoid confict with WKT

        // This will try to update rather than creating
        cscCmd->SetUpdateExisting(false);
        cscCmd->SetXYTolerance(.02);
        cscCmd->SetZTolerance(.001);
        
		cscCmd->SetCoordinateSystemWkt(wkt);

        cscCmd->Execute();
		
		// Verify
		FdoPtr<FdoIGetSpatialContexts> spatialContexts = (FdoIGetSpatialContexts*)mConnection->CreateCommand (FdoCommandType_GetSpatialContexts);
        FdoPtr<FdoISpatialContextReader> pScReader = spatialContexts->Execute();
        int cnt = 0;
		FdoStringP	scname;
        while (pScReader->ReadNext())
        {
            FdoStringP pName = pScReader->GetName();
			FdoStringP wkt2 = pScReader->GetCoordinateSystemWkt();

			if ( wkt2 == wkt )
				scname = pScReader->GetName();
            cnt++;
        }
		// The default spatial context was removed since it was not used (see #792011.02). Thefore expect 2 SCs.
        CPPUNIT_ASSERT_MESSAGE ("Expecting 1 Spatial Context", 1 == cnt);
		CPPUNIT_ASSERT_MESSAGE ("Expecting existing spatial context", scname != L"");

		// Associate with the newly created SC. This will cause PRJ creation.
		geometry->SetSpatialContextAssociation(scname);

		//Generating default feature class
		FdoPtr<FdoFeatureClass> featureclass1=FdoFeatureClass::Create(NEW_CLASS_NAME, L"Default feature class");
		FdoPtr<FdoPropertyDefinitionCollection> properties=featureclass1->GetProperties();
		properties->Add(featid);
		properties->Add(geometry);
		FdoPtr<FdoDataPropertyDefinitionCollection> identifier=featureclass1->GetIdentityProperties();
		identifier->Add(featid);

		featureclass1->SetGeometryProperty(geometry);

		//Generating default schema
		FdoPtr<FdoFeatureSchema> schema1=FdoFeatureSchema::Create(NEW_SCHEMA_NAME, L"Default schema");
		FdoClassesP(schema1->GetClasses()->Add(featureclass1));

		//FdoPtr<FdoFeatureSchema> schema = schema1->GetItem(0);
		FdoPtr<FdoClassCollection> coll = schema1->GetClasses();
		FdoPtr<FdoClassDefinition> def = coll->GetItem(0);      
		def->SetName(NEW_CLASS_NAME);               
	 
		FdoPtr<FdoIApplySchema> apply = (FdoIApplySchema*) mConnection->CreateCommand(FdoCommandType_ApplySchema);
		apply->SetFeatureSchema(schema1);
		apply->Execute();

		mConnection->Close ();

		///////// STEP II - read back the spatial contexts

		FdoPtr<FdoIConnectionInfo> cinfo2 = mConnection->GetConnectionInfo();
		FdoPtr<FdoIConnectionPropertyDictionary> didc2 = cinfo->GetConnectionProperties();
		didc2->SetProperty(L"DefaultFileLocation", LOCATION7);
		didc2->SetProperty(L"TemporaryFileLocation",L"");

		mConnection->Open();

		// Verify
		FdoPtr<FdoIGetSpatialContexts> spatialContexts2 = (FdoIGetSpatialContexts*)mConnection->CreateCommand (FdoCommandType_GetSpatialContexts);
        FdoPtr<FdoISpatialContextReader> pScReader2 = spatialContexts2->Execute();
        int cnt2 = 0;
		FdoStringP	wkt2;
        while (pScReader2->ReadNext())
        {
            FdoStringP pName = pScReader2->GetName();
			wkt2 = pScReader2->GetCoordinateSystemWkt();

            cnt2++;
        }
        CPPUNIT_ASSERT_MESSAGE ("Expecting 1 Spatial Context", 1 == cnt2);
		CPPUNIT_ASSERT_MESSAGE ("Wrong value for spatial context wkt", wkt2 == wkt);

		//////  STEP III - change the spatial context of the class. The PRJ should dissapear.
        FdoPtr<FdoIDescribeSchema> descSchema = (FdoIDescribeSchema*)mConnection->CreateCommand(FdoCommandType_DescribeSchema);
        descSchema->SetSchemaName(L"Default"/*NEW_SCHEMA_NAME*/);
        FdoPtr<FdoFeatureSchemaCollection> schemas = descSchema->Execute();
        FdoPtr<FdoFeatureSchema>	schema = schemas->GetItem(L"Default"/*NEW_SCHEMA_NAME*/);
        FdoPtr<FdoClassCollection> classes = schema->GetClasses();
        FdoPtr<FdoClassDefinition> classDef = classes->GetItem(NEW_CLASS_NAME);
        FdoPtr<FdoPropertyDefinitionCollection> props = classDef->GetProperties();

        // Modify this class
        FdoPtr<FdoPropertyDefinition> removedProp = props->GetItem(L"Geometry");
        props->Remove(removedProp);
 
		// Apply changes
		FdoPtr<FdoIApplySchema> apply3 = (FdoIApplySchema*) mConnection->CreateCommand(FdoCommandType_ApplySchema);
		apply3->SetFeatureSchema(schema);
        apply3->Execute();

		// Verify
		FdoPtr<FdoIGetSpatialContexts> spatialContexts3 = (FdoIGetSpatialContexts*)mConnection->CreateCommand (FdoCommandType_GetSpatialContexts);
        FdoPtr<FdoISpatialContextReader> pScReader3 = spatialContexts3->Execute();
        int cnt3 = 0;
        while (pScReader3->ReadNext())
        {
            FdoStringP pName = pScReader2->GetName();
			FdoStringP wkt2 = pScReader2->GetCoordinateSystemWkt();

            cnt3++;
        }
		// The spatial context is still there
        CPPUNIT_ASSERT_MESSAGE ("Expecting 1 Spatial Context", 1 == cnt3);

	}
    catch (FdoException* ge) 
    {
        TestCommonFail (ge);
    }
}

/* Test basic describe operation. */
void SchemaTests::describe_Fix784301 ()
{
    try
    {
        // switch to yet another default file location.
		// 2 classes, 2 .prj files the coordinate system name is the same but diffrent WKTs.
        mConnection->Close ();
        mConnection->SetConnectionString (L"DefaultFileLocation=" LOCATION6);
        ShpTests::sLocation = LOCATION6;
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->Open ());
        FdoPtr<FdoIDescribeSchema> describe = (FdoIDescribeSchema*)mConnection->CreateCommand (FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> schemas = describe->Execute ();
        CPPUNIT_ASSERT_MESSAGE ("Expecting 1 schema", 1 == schemas->GetCount ());
        FdoPtr<FdoIGetSpatialContexts> spatialContexts = (FdoIGetSpatialContexts*)mConnection->CreateCommand (FdoCommandType_GetSpatialContexts);
        FdoPtr<FdoISpatialContextReader> pScReader = spatialContexts->Execute();
        int cnt = 0;
        while (pScReader->ReadNext())
        {
            FdoStringP pName = pScReader->GetName();
            cnt++;
        }
		// The default spatial context was removed since it was not used (see #792011.02). Thefore expect 2 SCs.
        CPPUNIT_ASSERT_MESSAGE ("Expecting 2 Spatial Context", 2 == cnt);
        FdoPtr<FdoFeatureSchema> schema = schemas->GetItem (0);
        FdoPtr<FdoClassCollection> classes = schema->GetClasses ();
        CPPUNIT_ASSERT_MESSAGE ("Expecting 2 Classes", 2 == classes->GetCount());
        FdoStringP lastSpatialContextAssociation = "";
        for (FdoInt32 i = 0; i < classes->GetCount(); i++)
        {
            FdoPtr<FdoClassDefinition> pClass = classes->GetItem(i);
            FdoPtr<FdoPropertyDefinitionCollection> pPropDefCol = pClass->GetProperties();
            FdoPtr<FdoPropertyDefinition> pPropdef = pPropDefCol->FindItem(L"Geometry");
            FdoGeometricPropertyDefinition* pGeoProp = (FdoGeometricPropertyDefinition*)(pPropdef.p);
            FdoStringP pSpatialContextAssociation = pGeoProp->GetSpatialContextAssociation();
            if (lastSpatialContextAssociation != L"")
            {
                CPPUNIT_ASSERT_MESSAGE ("Expecting different spatial contexts", lastSpatialContextAssociation != pSpatialContextAssociation);
            }
            else
            {
                lastSpatialContextAssociation = pSpatialContextAssociation;
            }
        }
    }
    catch (FdoException* ge) 
    {
        TestCommonFail (ge);
    }
}


void SchemaTests::read_only_files ()
{
    try
    {
        FdoStringP userName = FdoCommonOSUtil::GetCurrentUserName();
        if (userName == L"root")
	{
	    printf("User 'root' detected; skipping read-only SHP file test (root is able to open any file in write mode)\n");
            return;
	}

        // Make a file read-only:
        SetShpFileAccess(L"../../TestData/Ontario/roads", false);

        // re-open the connection:
        mConnection->Close ();
        mConnection->SetConnectionString (L"DefaultFileLocation=../../TestData/Ontario");
        mConnection->Open ();

        // Verify that classes are read-only:
        FdoPtr<FdoIDescribeSchema> descSchema = static_cast<FdoIDescribeSchema*>(mConnection->CreateCommand(FdoCommandType_DescribeSchema));
        FdoPtr<FdoFeatureSchemaCollection> schemas = descSchema->Execute();
        for (int s=0; s<schemas->GetCount(); s++)
        {
            FdoPtr<FdoFeatureSchema> schema = schemas->GetItem(s);
            FdoPtr<FdoClassCollection> classes = schema->GetClasses();
            for (int c=0; c<classes->GetCount(); c++)
            {
                FdoPtr<FdoClassDefinition> classDef = classes->GetItem(c);
                FdoPtr<FdoClassCapabilities> classCapabilities = classDef->GetCapabilities();
                if (0==wcscmp(classDef->GetName(), L"roads"))
                    CPPUNIT_ASSERT_MESSAGE("class should be read-only", !classCapabilities->SupportsWrite());
            }
        }

        // Make a file read-write:
        SetShpFileAccess(L"../../TestData/Ontario/roads", true);

        // re-open the connection:
        mConnection->Close ();
        mConnection->SetConnectionString (L"DefaultFileLocation=../../TestData/Ontario");
        mConnection->Open ();

        // Verify that classes are read-write:
        descSchema = static_cast<FdoIDescribeSchema*>(mConnection->CreateCommand(FdoCommandType_DescribeSchema));
        schemas = descSchema->Execute();
        for (int s=0; s<schemas->GetCount(); s++)
        {
            FdoPtr<FdoFeatureSchema> schema = schemas->GetItem(s);
            FdoPtr<FdoClassCollection> classes = schema->GetClasses();
            for (int c=0; c<classes->GetCount(); c++)
            {
                FdoPtr<FdoClassDefinition> classDef = classes->GetItem(c);
                FdoPtr<FdoClassCapabilities> classCapabilities = classDef->GetCapabilities();
                if (0==wcscmp(classDef->GetName(), L"roads"))
                    CPPUNIT_ASSERT_MESSAGE("class should be read-write", classCapabilities->SupportsWrite());
            }
        }
    }
    catch (FdoException* ge) 
    {
        TestCommonFail (ge);
    }
}


void SchemaTests::SetShpFileAccess(FdoString* shpPath, bool bReadWrite)
{
    FdoStringP file;

    file = shpPath; file += L".shp";
    FdoCommonFile::Chmod(file, bReadWrite);
    file = shpPath; file += L".dbf";
    FdoCommonFile::Chmod(file, bReadWrite);
    file = shpPath; file += L".shx";
    FdoCommonFile::Chmod(file, bReadWrite);
}


void SchemaTests::apply_helper(bool bUseSpecificGeometryTypes)
{
    try
    {
        FdoString* NEW_SCHEMA_NAME = L"NewSchema";
        FdoString* NEW_CLASS_NAME = L"Test";

        // Clean up leftovers from previous tests:
        TestCommonSchemaUtil::CleanUpClass(mConnection, NULL, NEW_CLASS_NAME);


        FdoPtr<FdoIApplySchema> apply = (FdoIApplySchema*)mConnection->CreateCommand (FdoCommandType_ApplySchema);
        FdoPtr<FdoFeatureSchema> schema = FdoFeatureSchema::Create (NEW_SCHEMA_NAME, L"");
        FdoPtr<FdoClassCollection> classes = schema->GetClasses ();

        FdoPtr<FdoDataPropertyDefinition> id = FdoDataPropertyDefinition::Create (L"Id", L"integer");
        id->SetDataType (FdoDataType_Int32);

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
        if (bUseSpecificGeometryTypes)
        {
            FdoGeometryType geomTypes[] = {FdoGeometryType_MultiPoint};
            location->SetSpecificGeometryTypes(geomTypes, 1);
        }
        else
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
		SaveSchema(mConnection);

        // Flush the schema cache:
        mConnection->Close();
        mConnection->Open();

        // check that the new schema shows up in the list
        FdoPtr<FdoIDescribeSchema> describe = (FdoIDescribeSchema*)mConnection->CreateCommand (FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> schemas = describe->Execute ();


        FdoPtr<FdoIDisposableCollection> collection = schemas->FindClass (NEW_CLASS_NAME);
        CPPUNIT_ASSERT_MESSAGE ("no class found", 1 == collection->GetCount ());
        FdoPtr<FdoFeatureClass> cls = (FdoFeatureClass *)collection->GetItem (0);
        CPPUNIT_ASSERT_MESSAGE ("wrong name", 0 == wcscmp (NEW_CLASS_NAME, cls->GetName ()));
        CPPUNIT_ASSERT_MESSAGE ("wrong description", 0 == wcscmp (L"test class created with apply schema", cls->GetDescription ()));

        // check it's contents
        properties = cls->GetProperties ();
        FdoPtr<FdoDataPropertyDefinition> featid = (FdoDataPropertyDefinition*)properties->GetItem (L"Id");
        CPPUNIT_ASSERT_MESSAGE ("id wrong type", FdoDataType_Int32 == featid->GetDataType ());
        street = (FdoDataPropertyDefinition*)properties->GetItem (L"Street");
        CPPUNIT_ASSERT_MESSAGE ("street wrong type", FdoDataType_String == street->GetDataType ());
        CPPUNIT_ASSERT_MESSAGE ("street wrong size", 64 == street->GetLength ());
        area = (FdoDataPropertyDefinition*)properties->GetItem (L"Area");
        CPPUNIT_ASSERT_MESSAGE ("area wrong type", FdoDataType_Decimal == area->GetDataType ());
        vacant = (FdoDataPropertyDefinition*)properties->GetItem (L"Vacant");
        CPPUNIT_ASSERT_MESSAGE ("vacant wrong type", FdoDataType_Boolean == vacant->GetDataType ());
        birthday = (FdoDataPropertyDefinition*)properties->GetItem (L"Birthday");
        CPPUNIT_ASSERT_MESSAGE ("birthday wrong type", FdoDataType_DateTime == birthday->GetDataType ());
        location = (FdoGeometricPropertyDefinition*)properties->GetItem (L"Geometry");
        if (bUseSpecificGeometryTypes)
        {
            FdoInt32 geomTypeCount = -1;
            FdoGeometryType* geomTypes = location->GetSpecificGeometryTypes(geomTypeCount);
            CPPUNIT_ASSERT_MESSAGE ("wrong geometry types", geomTypeCount==1 && (geomTypes[0] == FdoGeometryType_MultiPoint));
        }
        else
            CPPUNIT_ASSERT_MESSAGE ("wrong geometry types", FdoGeometricType_Point == location->GetGeometryTypes ());
        CPPUNIT_ASSERT_MESSAGE ("wrong elevation", location->GetHasElevation ());
        CPPUNIT_ASSERT_MESSAGE ("wrong measure", location->GetHasMeasure ());

        // OK, now delete the class
        schema = schemas->GetItem (NEW_SCHEMA_NAME);
        classes = schema->GetClasses ();
        FdoPtr<FdoClassDefinition> definition = classes->GetItem (NEW_CLASS_NAME);
        definition->Delete ();
        apply->SetFeatureSchema (schema);
        apply->Execute ();
		SaveSchema(mConnection);
    }
    catch (FdoException* ge) 
    {
        TestCommonFail (ge);
    }
}
