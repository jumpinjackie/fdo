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

using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;
using System.IO;

using OSGeo.FDO.Schema;
using OSGeo.FDO.Connections;
using OSGeo.FDO.Commands.Schema;
using OSGeo.FDO.Commands;
using OSGeo.FDO;

using unit_test.Framework;

namespace unit_test.ProviderTests
{
    class SchemaTests : BaseTestWithConnection
    {
        const string LOCATION = @"..\..\..\..\..\TestData\Testing";
        const string LOCATION2 = @"..\..\..\..\..\TestData\t\x5348\x524dsting\";

        public SchemaTests(ShpTestProvider connectProvider)
            : base(connectProvider)
        { }

        private void show_schema(FeatureSchemaCollection schemas)
        {
            for (int i = 0; i < schemas.Count; i++)
            {
                FeatureSchema schema = schemas[i];

                Console.WriteLine("Schema: " + schema.Name);
                if ((schema.Description != null) && (schema.Description != ""))
                    Console.WriteLine("    Description: " + schema.Description);
                ClassCollection classes = schema.Classes;
                for (int j = 0; j < classes.Count; j++)
                {
                    ClassDefinition cls = classes[j];

                    // Output basic class info:
                    if (cls.ClassType == ClassType.ClassType_FeatureClass)
                    {
                        string name = cls.Name;
                        Console.WriteLine("    Feature Class: " + name);
                    }
                    //Console.WriteLine ("    Feature Class: " + cls.Name);
                    else
                        Console.WriteLine("    Class: " + cls.Name);
                    if ((cls.Description != null) && (cls.Description != ""))
                        Console.WriteLine("        Description: " + cls.Description);

                    ClassCapabilities classCapabilities = cls.Capabilities;
                    Console.WriteLine("        Class Capabilities:");
                    if (classCapabilities == null)
                        Console.WriteLine("            (Not available).");
                    else
                    {
                        Console.WriteLine("            Supports locking: " + (classCapabilities.SupportsLocking ? "yes" : "no"));
                        Console.WriteLine("            Supports long transactions: " + (classCapabilities.SupportsLongTransactions ? "yes" : "no"));
                    }


                    // Output identity properties:
                    DataPropertyDefinitionCollection identity = cls.IdentityProperties;
                    for (int k = 0; k < identity.Count; k++)
                    {
                        DataPropertyDefinition definition = identity[k];
                        Console.WriteLine("        Id: " + definition.Name);
                        if ((definition.Description != null) && (definition.Description != ""))
                            Console.WriteLine("            Description: " + definition.Description);
                        //Console.WriteLine ("            Type: " + ShpTests.%ls Length: %d Precision: %d " +
                        //    ShpTests::GetDataTypeString (definition.DataType),
                        //    definition.Length,
                        //    definition.GetPrecision (),
                        //    definition.Getnullable () ? "nullable" : "Notnul");                
                    }

                    // Output regular properties:
                    PropertyDefinitionCollection properties = cls.Properties;
                    for (int k = 0; k < properties.Count; k++)
                    {
                        PropertyDefinition definition = properties[k];
                        if (definition.PropertyType == PropertyType.PropertyType_DataProperty)
                        {
                            DataPropertyDefinition data_definition = (DataPropertyDefinition)definition;
                            if (!identity.Contains(data_definition))
                            {
                                Console.WriteLine("        Property: " + definition.Name);
                                if ((data_definition.Description != null) && (data_definition.Description != ""))
                                    Console.WriteLine("            Description: " + data_definition.Description);
                                //Console.WriteLine ("            Type: %ls Length: %d Precision: %d " +
                                //    ShpTests::GetDataTypeString (data_definition.DataType),
                                //    data_definition.Length,
                                //    data_definition.GetPrecision (),
                                //    data_definition.Getnullable () ? "nullable" : "Notnul");
                            }
                        }
                        else if (definition.PropertyType == PropertyType.PropertyType_ObjectProperty)
                        {
                            Console.WriteLine("       Object Property: " + definition.Name);
                            if ((definition.Description != null) && (definition.Description != ""))
                                Console.WriteLine("            Description: " + definition.Description);
                        }
                        else if (definition.PropertyType == PropertyType.PropertyType_GeometricProperty)
                        {
                            GeometricPropertyDefinition geometry_definition = (GeometricPropertyDefinition)definition;
                            Console.WriteLine("        Geometric Property: " + geometry_definition.Name);
                            if ((geometry_definition.Description != null) && (geometry_definition.Description != ""))
                                Console.WriteLine("            Description: " + geometry_definition.Description);
                            int types = geometry_definition.GeometryTypes;
                            if (0 != (types & 0x01))
                                Console.WriteLine("            GeometricType_Point types allowed");
                            if (0 != (types & 0x02))
                                Console.WriteLine("            GeometricType_Curve types allowed");
                            if (0 != (types & 0x04))
                                Console.WriteLine("            GeometricType_Surface types allowed");
                            if (0 != (types & 0x08))
                                Console.WriteLine("            GeometricType_Solid types allowed");
                        }
                    }

                    // Output schema attribute dictionary:
                    SchemaAttributeDictionary dictionary = cls.Attributes;
                    int count = dictionary.AttributeNames.Length;
                    string[] names = dictionary.AttributeNames;
                    if ((0 != count) && (null != names))
                    {
                        Console.WriteLine("        MetaData:");
                        string name;
                        for (int t = 0; t < count; t++)
                        {
                            if (0 != t)
                                Console.Write(",");
                            name = names[t];
                            string value = dictionary.GetAttributeValue(name);
                            Console.WriteLine(name + " = " + value);
                        }
                        Console.WriteLine();
                    }
                }
            }
        }

        //**********************Runtime error, debug later*********************
//        /// Test basic describe operation. /
//        public void Test_describe ()
//        {
//            try
//            {
//                IConnection connection =  base.ConnectionNew;
//                connection.ConnectionString = @"DefaultFileLocation=..\..\TestData\Ontario2";
//                connection.Open();

//                IDescribeSchema describe = (IDescribeSchema)connection.CreateCommand(CommandType.CommandType_DescribeSchema);
//                FeatureSchemaCollection schemas = describe.Execute ();
//#if DEBUG
//                    show_schema (schemas);
//#endif
//                connection.Close();

//                FeatureSchema oldSchema = schemas[0];
//                ClassCollection oldClasses = oldSchema.Classes;
//                ClassDefinition oldClass = oldClasses[0];

//                IConnection connection1 = base.ConnectionNew;
//                connection1.ConnectionString = @"DefaultFileLocation=..\..\TestData\Ontario2";
//                connection1.Open();

//                string NEW_SCHEMA_NAME = "NewSchema";
//                string NEW_CLASS_NAME = oldClass.Name;

//                FeatureSchema schema = new FeatureSchema(NEW_SCHEMA_NAME, "");
//                ClassCollection classes = schema.Classes;

//                DataPropertyDefinition id = new DataPropertyDefinition("Id", "integer");
//                id.DataType = DataType.DataType_Int32;

//                DataPropertyDefinition street = new DataPropertyDefinition ("Street", "text");
//                street.DataType = DataType.DataType_String;
//                street.Length = 64;

//                DataPropertyDefinition area = new DataPropertyDefinition("Area", "double");
//                area.DataType = DataType.DataType_Decimal;
//                area.Precision =20;
//                area.Scale = 8;

//                DataPropertyDefinition vacant = new DataPropertyDefinition("Vacant", "boolean");
//                vacant.DataType = DataType.DataType_Boolean;

//                DataPropertyDefinition birthday = new DataPropertyDefinition("Birthday", "date");
//                birthday.DataType = DataType.DataType_DateTime;

//                // build a location geometry property
//                GeometricPropertyDefinition location = new GeometricPropertyDefinition ("Geometry", "geometry");
//                location.GeometryTypes = 0x01;
//                location.HasElevation = true;
//                location.HasMeasure = true;

//                //// assemble the feature class
//                FeatureClass feature = new FeatureClass (NEW_CLASS_NAME, "test class created with apply schema");
//                PropertyDefinitionCollection properties = feature.Properties;
//                properties.Add (id);
//                properties.Add (street);
//                properties.Add (area);
//                properties.Add (vacant);
//                properties.Add (birthday);
//                properties.Add (location);
//                feature.GeometryProperty = location;
//                DataPropertyDefinitionCollection identities = feature.IdentityProperties;
//                identities.Add (id);

//                // submit the new schema
//                classes.Add (feature);

//                IApplySchema apply = (IApplySchema)connection1.CreateCommand(CommandType.CommandType_ApplySchema);
//                apply.FeatureSchema = schema;
//                apply.Execute();

//                connection1.Close();
//            }
//            catch (OSGeo.Common.Exception ex)
//            {
//                Debug.Fail(ex.Message);
//            }
//            catch (System.Exception ex)
//            {
//                Debug.Fail(ex.Message);
//            }
//        }

        //**********************Runtime error, debug later*********************
        ///// Test describe operation with nonexistant schema. /
        //public void Test_describe_bogus()
        //{
        //    try
        //    {
        //        IDescribeSchema describe = (IDescribeSchema)base.ConnectionInitialized.CreateCommand(CommandType.CommandType_DescribeSchema);
        //        // ensure asking for a nonexistant schema yields the proper exception
        //        try
        //        {
        //            describe.SchemaName = "Bogus";
        //            FeatureSchemaCollection schemas = describe.Execute();
        //            Debug.Fail("no exception asking for a nonexistant schema");
        //        }
        //        catch (OSGeo.Common.Exception e)
        //        {
        //            // check for expected message                    
        //            Debug.Assert(e.Message.IndexOf("Schema 'Bogus' not found. ") != -1, "wrong exception");
        //        }
        //    }
        //    catch (OSGeo.Common.Exception ex)
        //    {
        //        Debug.Fail(ex.Message);
        //    }
        //    catch (System.Exception ex)
        //    {
        //        Debug.Fail(ex.Message);
        //    }
        //}

        /// Test basic apply operation. /
        public void Tests_apply ()
        {
            try
            {
                string NEW_SCHEMA_NAME = "NewSchema";
                string NEW_CLASS_NAME = "Test";

                // Clean up leftovers from previous tests:
                ShpTests.CleanUpClass(base.ConnectionInitialized, null, NEW_CLASS_NAME);


                IApplySchema apply = (IApplySchema)base.ConnectionInitialized.CreateCommand (CommandType.CommandType_ApplySchema);
                FeatureSchema schema = new FeatureSchema (NEW_SCHEMA_NAME, "");
                ClassCollection classes = schema.Classes;

                DataPropertyDefinition id = new DataPropertyDefinition("Id", "integer");
                id.DataType = DataType.DataType_Int32;

                DataPropertyDefinition street = new DataPropertyDefinition("Street", "text");
                street.DataType = DataType.DataType_String;
                street.Length = 64;

                DataPropertyDefinition area = new DataPropertyDefinition("Area", "double");
                area.DataType = DataType.DataType_Decimal;
                area.Precision = 20;
                area.Scale = 8;

                DataPropertyDefinition vacant = new DataPropertyDefinition("Vacant", "boolean");
                vacant.DataType = DataType.DataType_Boolean;

                DataPropertyDefinition birthday = new DataPropertyDefinition("Birthday", "date");
                birthday.DataType = DataType.DataType_DateTime;

                // build a location geometry property
                GeometricPropertyDefinition location = new GeometricPropertyDefinition ("Geometry", "geometry");
                location.GeometryTypes = 0x01;
                location.HasElevation = true;
                location.HasMeasure = true;

                //// assemble the feature class
                FeatureClass feature = new FeatureClass (NEW_CLASS_NAME, "test class created with apply schema");
                PropertyDefinitionCollection properties = feature.Properties;
                properties.Add (id);
                properties.Add (street);
                properties.Add (area);
                properties.Add (vacant);
                properties.Add (birthday);
                properties.Add (location);
                feature.GeometryProperty = location;
                DataPropertyDefinitionCollection identities = feature.IdentityProperties;
                identities.Add (id);

                // submit the new schema
                classes.Add (feature);
                apply.FeatureSchema = schema;
                apply.Execute ();

                // check that the new schema shows up in the list
                IDescribeSchema describe = (IDescribeSchema)base.ConnectionInitialized.CreateCommand (CommandType.CommandType_DescribeSchema);
                FeatureSchemaCollection schemas = describe.Execute ();

                IDisposableCollection collection = schemas.FindClass (NEW_CLASS_NAME);
                Debug.Assert (collection.Count == 1, "no class found");
                FeatureClass cls = (FeatureClass )collection[0];

                Debug.Assert (NEW_CLASS_NAME == cls.Name, "wrong name");
                Debug.Assert (cls.Description == "test class created with apply schema", "wrong description");

                // check it's contents
                properties = cls.Properties;                
                DataPropertyDefinition featid = (DataPropertyDefinition)properties["Id"]; 
                Debug.Assert (featid.DataType == DataType.DataType_Int32, "id wrong type");
                street = (DataPropertyDefinition)properties["Street"];
                Debug.Assert (street.DataType == DataType.DataType_String, "street wrong type" );
                Debug.Assert (street.Length == 64, "street wrong size" );
                area = (DataPropertyDefinition)properties["Area"];
                Debug.Assert (area.DataType == DataType.DataType_Decimal, "area wrong type");
                vacant = (DataPropertyDefinition)properties["Vacant"];
                Debug.Assert (vacant.DataType == DataType.DataType_Boolean, "vacant wrong type" );
                birthday = (DataPropertyDefinition)properties["Birthday"];
                Debug.Assert (birthday.DataType == DataType.DataType_DateTime, "birthday wrong type");
                location = (GeometricPropertyDefinition)properties["Geometry"];
                Debug.Assert (location.GeometryTypes== 0x01, "wrong geometry types");
                Debug.Assert (location.HasElevation, "wrong elevation");
                Debug.Assert (location.HasMeasure, "wrong measure");

                // OK, now delete the class
                schema = schemas[NEW_SCHEMA_NAME];
                classes = schema.Classes;
                ClassDefinition definition = classes[NEW_CLASS_NAME];
                definition.Delete ();
                apply.FeatureSchema = schema;
                apply.Execute ();
            }
            catch (OSGeo.Common.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
            catch (System.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
        }

        /// Test nameless schema apply operation. /
        public void Test_apply_nameless ()
        {
            try
            {
                string NEW_SCHEMA_NAME = "";
                string NEW_CLASS_NAME = "Test";

                // Clean up leftovers from previous tests:
                ShpTests.CleanUpClass(base.ConnectionInitialized, null, NEW_CLASS_NAME);

                IApplySchema apply = (IApplySchema)base.ConnectionInitialized.CreateCommand (CommandType.CommandType_ApplySchema);
                FeatureSchema schema = new FeatureSchema (NEW_SCHEMA_NAME, "");
                ClassCollection classes = schema.Classes;

                DataPropertyDefinition id = new DataPropertyDefinition("Id", "integer");
                id.DataType = DataType.DataType_Int32;

                DataPropertyDefinition street = new DataPropertyDefinition("Street", "text");
                street.DataType = DataType.DataType_String;
                street.Length = 64;

                // build a location geometry property
                GeometricPropertyDefinition location = new GeometricPropertyDefinition ("Geometry", "geometry");
                location.GeometryTypes = 0x01;
                location.HasElevation = true;
                location.HasMeasure = true;

                //// assemble the feature class
                FeatureClass feature = new FeatureClass (NEW_CLASS_NAME, "test class created with apply schema");
                PropertyDefinitionCollection properties = feature.Properties;
                properties.Add (id);
                properties.Add (street);
                properties.Add (location);
                feature.GeometryProperty = location;
                DataPropertyDefinitionCollection identities = feature.IdentityProperties;
                identities.Add (id);

                bool applied = false;
                try
                {
                    // submit the new schema
                    classes.Add (feature);
                    apply.FeatureSchema = schema;
                    apply.Execute ();
                    applied = true;
                }
                catch (OSGeo.Common.Exception ge)
                {
                    // check for expected message
                    Debug.Assert (ge.Message.IndexOf("No schema specified for the apply schema command.") != -1, "wrong exception");
                }

                // check that the new schema doesn't show up in the list
                IDescribeSchema describe = (IDescribeSchema)base.ConnectionInitialized.CreateCommand (CommandType.CommandType_DescribeSchema);
                FeatureSchemaCollection schemas = describe.Execute ();
                IDisposableCollection collection = schemas.FindClass (NEW_CLASS_NAME);
                int count = collection.Count;
                if (0 != count)
                {                    
                    // delete the class
                    schema = schemas[NEW_SCHEMA_NAME];
                    classes = schema.Classes;
                    ClassDefinition definition = classes[NEW_CLASS_NAME];
                    definition.Delete();
                    apply.FeatureSchema = schema;
                    apply.Execute();
                }
                Debug.Assert (!applied, "nameless schema applied");
                Debug.Assert (0 == count, "class found from nameless schema apply");
            }
            catch (OSGeo.Common.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
            catch (System.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
        }

        /// Test nameless class creation. /
        public void Test_create_nameless ()
        {
            try
            {
                string NEW_SCHEMA_NAME = "NewSchema";
                string NEW_CLASS_NAME = "";

                // Clean up leftovers from previous tests:
                ShpTests.CleanUpClass(base.ConnectionInitialized, null, NEW_CLASS_NAME);

                IApplySchema apply = (IApplySchema)base.ConnectionInitialized.CreateCommand (CommandType.CommandType_ApplySchema);
                FeatureSchema schema = new FeatureSchema (NEW_SCHEMA_NAME, "");
                ClassCollection classes = schema.Classes;

                DataPropertyDefinition id = new DataPropertyDefinition("Id", "integer");
                id.DataType = DataType.DataType_Int32;

                DataPropertyDefinition street = new DataPropertyDefinition("Street", "text");
                street.DataType = DataType.DataType_String;
                street.Length = 64;

                // build a location geometry property
                GeometricPropertyDefinition location = new GeometricPropertyDefinition ("Geometry", "geometry");
                location.GeometryTypes = 0x01;
                location.HasElevation = true;
                location.HasMeasure = true;

                //// assemble the feature class
                FeatureClass feature = new FeatureClass (NEW_CLASS_NAME, "test class created with apply schema");
                PropertyDefinitionCollection properties = feature.Properties;
                properties.Add (id);
                properties.Add (street);
                properties.Add (location);
                feature.GeometryProperty = location;
                DataPropertyDefinitionCollection identities = feature.IdentityProperties;
                identities.Add (id);

                bool applied = false;
                try
                {
                    // submit the new schema
                    classes.Add (feature);
                    apply.FeatureSchema = schema;
                    apply.Execute ();
                    applied = true;
                }
                catch (OSGeo.Common.Exception ge)
                {
                    // check for expected message
                    Debug.Assert (ge.Message.IndexOf("The class name '' is invalid.") != -1, "wrong exception");
                }

                // check that the new class doesn't show up in the schema
                IDescribeSchema describe = (IDescribeSchema)base.ConnectionInitialized.CreateCommand (CommandType.CommandType_DescribeSchema);
                FeatureSchemaCollection schemas = describe.Execute ();
                IDisposableCollection collection = schemas.FindClass (NEW_CLASS_NAME);
                int count = collection.Count;
                if (0 != count)
                {
                    // delete the class                  
                    schema = schemas[NEW_SCHEMA_NAME];
                    classes = schema.Classes;
                    ClassDefinition definition = classes[NEW_CLASS_NAME];
                    definition.Delete ();
                    apply.FeatureSchema = schema;
                    apply.Execute ();
                }
                Debug.Assert (!applied, "nameless class created");
                Debug.Assert (0 == count, "class found from nameless schema apply");
            }
            catch (OSGeo.Common.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
            catch (System.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
        }

        //// Executes a describe schema mapping (with default values, no config file) command.
        public void Test_schema_mapping_defaults_test()
        {
            try
            {
                IConnection mConnection = base.ConnectionInitialized;
                mConnection.Close ();
                mConnection.ConnectionString = @"DefaultFileLocation=..\..\..\..\..\TestData\Testing";
       
                Debug.Assert (mConnection.Open() == ConnectionState.ConnectionState_Open, "connection state not open");

                IDescribeSchemaMapping describeSchemaMappingCmd =
                    (IDescribeSchemaMapping)mConnection.CreateCommand(CommandType.CommandType_DescribeSchemaMapping);
                describeSchemaMappingCmd.IncludeDefaults = true;
                PhysicalSchemaMappingCollection mappings = describeSchemaMappingCmd.Execute();

                IDescribeSchema describeSchemaCmd =
                    (IDescribeSchema)mConnection.CreateCommand(CommandType.CommandType_DescribeSchema);
                FeatureSchemaCollection logicalSchemas = describeSchemaCmd.Execute();

                // test the schema mappings:
                //***************cann't implement***************************
                //_TestSchemaMappings(mappings, logicalSchemas);
            }
            catch (OSGeo.Common.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
            catch (System.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
        }


        //// Executes a describe schema mapping (without default values, no config file) command.
        public void Test_schema_mapping_no_defaults_test()
        {
            try
            {
                IConnection mConnection = base.ConnectionInitialized;
                IDescribeSchemaMapping describeSchemaMappingCmd =
                    (IDescribeSchemaMapping)mConnection.CreateCommand(CommandType.CommandType_DescribeSchemaMapping);
                describeSchemaMappingCmd.IncludeDefaults = false;
                PhysicalSchemaMappingCollection mappings = describeSchemaMappingCmd.Execute();

                Debug.Assert(mappings.Count==0, "default mappings should be empty");

                // test the schema mappings:
                //***************cann't implement***************************
                //_TestSchemaMappings(mappings, null);
            }
            catch (OSGeo.Common.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
            catch (System.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
        }


        // Helper method; iterates the given schema mappings, comparing the mappings to 
        // the logical schema (if one is given).
        //private void _TestSchemaMappings(PhysicalSchemaMappingCollection mappings, FeatureSchemaCollection logicalSchemas)
        //{
        //    if (mappings == null)
        //        Debug.Fail("FAILED - DescribeSchemaMapping returned null collection");

        //    int numMappings = mappings.Count;

        //    if (logicalSchemas != null)
        //    {
        //        // Match logical count to mapping count
        //        Debug.Assert(numMappings==logicalSchemas.Count, "Number of schema mappings != number of logical schemas");
        //    }

        //    for (int i=0; i<numMappings; i++)
        //    {
        //        PhysicalSchemaMapping fdoMapping = mappings[i];
        //        //********************** no ShpPhysicalSchemaMapping type ******************************
        //        //ShpPhysicalSchemaMapping  mapping = static_cast<ShpPhysicalSchemaMapping (fdoMapping.p);

        //        //string mappingName = mapping.Name;
        //        //string mappingProviderName = mapping.GetProvider();

        //        FeatureSchema logicalSchema;
        //        ClassCollection logicalClasses;
        //        if (logicalSchemas != null)
        //        {
        //            //logicalSchema = logicalSchemas.FindItem(mappingName);
        //            Debug.Assert(logicalSchema!=null, "Didn't find matching logical schema");
        //            logicalClasses = logicalSchema.Classes;
        //        }

        //        //ShpClassCollection classes = mapping.Classes;

        //        //int numClasses = classes.Count;

        //        if (logicalClasses != null)
        //        {
        //            Debug.Assert(logicalClasses.Count==numClasses, "Logical class count doesn't match mapping class count");
        //        }

        //        for (int j=0; j<numClasses; j++)
        //        {
        //            ShpClassDefinition classMapping = classes.GetItem(j);

        //            // analyze the feature class
        //            string className = classMapping.Name;
        //            string shapefileName = classMapping.GetShapeFile();

        //            // Determine default shapefile filename:
        //            IConnectionInfo connInfo = mConnection.GetConnectionInfo();
        //            IConnectionPropertyDictionary connProps = connInfo.GetConnectionProperties();
        //            stringP defaultShapefileNameNoExt = connProps.GetProperty(CONNECTIONPROPERTY_DEFAULT_FILE_LOCATION);
        //            CommonFile::DelimitPath(defaultShapefileNameNoExt);
        //            defaultShapefileNameNoExt += className;

        //            ClassDefinition logicalClass;
        //            if (logicalClasses != null)
        //            {
        //                // match physical name to logical name:
        //                Debug.Assert("Physical name didn't match logical name", 0==wcsncmp(shapefileName, defaultShapefileNameNoExt, wcslen(shapefileName)-4));

        //                logicalClass = logicalClasses.FindItem(className);
        //                Debug.Assert("logical class not found", logicalClass!=null);
        //            }

        //            ShpPropertyDefinitionCollection classProps = classMapping.GetProperties();
        //            int numProps = classProps.Count;

        //            PropertyDefinitionCollection logicalProperties;
        //            bool bHasGeometryProperty = false;
        //            if (logicalClass != null)
        //            {
        //                logicalProperties = logicalClass.GetProperties();

        //                for (int i=0; i<logicalProperties.Count && !bHasGeometryProperty; i++)
        //                {
        //                    PropertyDefinition logicalProp = logicalProperties.GetItem(i);
        //                    bHasGeometryProperty = (logicalProp.GetPropertyType() == PropertyType_GeometricProperty);
        //                }

        //                // Match logical count to mapping count:
        //                // NOTE: logical class may have an additional geometry property that's not in the mapping,
        //                //       since the geometry mapping is handle in a special way:
        //                // NOTE: logical class will hvae a FeatId identity property that's not in the mapping either.
        //                Debug.Assert("Logical property count != mapping property count", logicalProperties.Count-(bHasGeometryProperty?1:0)-1 == numProps);
        //            }

        //            // Compare the data properties:
        //            for (int k=0; k<numProps; k++)
        //            {
        //                // NOTE: all properties in this collection are data properties, so we can do a static cast:
        //                ShpPropertyDefinition propDef = (ShpPropertyDefinition)classProps.GetItem(k);
        //                string propName = propDef.Name;

        //                PropertyDefinition logicalProperty;
        //                if (logicalProperties != null)
        //                {
        //                    logicalProperty = logicalProperties.FindItem(propName);
        //                    ShpColumnDefinition column = propDef.GetColumn();
        //                    Debug.Assert("logical property not found", logicalProperty != null);

        //                    // Match physical name to logical name:
        //                    Debug.Assert("physical column name != logical property name", 0==wcscmp(propName,column.Name));
        //                }
        //            }
        //        }
        //    }
        //}

        public void Test_add_class ()
        {
            try
            {
                IConnection mConnection = base.ConnectionInitialized;
                ShpTests.CleanUpClass (mConnection, "Default", "Test");

                IDescribeSchema describe = (IDescribeSchema)mConnection.CreateCommand (CommandType.CommandType_DescribeSchema);
                FeatureSchemaCollection schemas = describe.Execute ();
                Debug.Assert (1 == schemas.Count, "Expecting 1 schema");
                FeatureSchema schema = schemas[0];
                ClassCollection classes = schema.Classes;

                IApplySchema apply = (IApplySchema)mConnection.CreateCommand (CommandType.CommandType_ApplySchema);

                DataPropertyDefinition id = new DataPropertyDefinition("Id", "integer");
                id.DataType = DataType.DataType_Int32;

                DataPropertyDefinition street = new DataPropertyDefinition("Street", "text");
                street.DataType = DataType.DataType_String;
                street.Length = 64;

                // build a location geometry property
                GeometricPropertyDefinition location = new GeometricPropertyDefinition ("Geometry", "geometry");
                location.GeometryTypes = 0x01;
                location.HasElevation = true;
                location.HasMeasure = true;

                //// assemble the feature class
                FeatureClass feature = new FeatureClass ("Test", "test class created with apply schema");
                PropertyDefinitionCollection properties = feature.Properties;
                properties.Add (id);
                properties.Add (street);
                properties.Add (location);
                feature.GeometryProperty = location;
                DataPropertyDefinitionCollection identities = feature.IdentityProperties;
                identities.Add (id);

                // submit the new schema
                classes.Add (feature);
                apply.FeatureSchema = schema;
                apply.Execute ();

                // close and reopen the connection
                mConnection.Close ();
                Debug.Assert (mConnection.Open () == ConnectionState.ConnectionState_Open, "connection state not open");

                // check that the new schema shows up in the list
                schemas = describe.Execute ();
                Debug.Assert (1 == schemas.Count, "Expecting 1 schema");
                schema = schemas[0];
                classes = schema.Classes;
                FeatureClass cls = (FeatureClass)classes["Test"];
                Debug.Assert (cls != null, "Test class not found");

                // check that the definition is the same
                Debug.Assert (cls.Description == "test class created with apply schema", "class bad description");
                properties = cls.Properties;
                id = (DataPropertyDefinition)properties["Id"];
                Debug.Assert (id != null, "id not found");
                Debug.Assert (id.DataType == DataType.DataType_Int32, "id not DataType_Int32");
                Debug.Assert (id.Description == "integer", "id bad description");
                street = (DataPropertyDefinition)properties["Street"];
                Debug.Assert (street != null, "street not found");
                Debug.Assert (street.DataType == DataType.DataType_String, "street not DataType_String");
                Debug.Assert (street.Description == "text", "street bad description");
                location = (GeometricPropertyDefinition)properties["Geometry"];
                Debug.Assert (location != null, "geometry not found");
                Debug.Assert (location.Description != "geometry", "geometry bad description");
                Debug.Assert (location.GeometryTypes == 0x01, "location not points");
                Debug.Assert (location.HasElevation, "location no elevation");
                Debug.Assert (location.HasMeasure, "location no measure");
                GeometricPropertyDefinition geometry = cls.GeometryProperty;
                Debug.Assert (location == geometry, "wrong geometry");
                DataPropertyDefinitionCollection ids = cls.IdentityProperties;
                DataPropertyDefinition identifier = ids["Id"];
                Debug.Assert (id == identifier, "wrong id");
            }
            catch (OSGeo.Common.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
            catch (System.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
        }

        /// Test apply operation with only identity and geometry. /
        public void Test_apply_two ()
        {
            try
            {
                IConnection mConnection = base.ConnectionInitialized;
                string NEW_SCHEMA_NAME = "NewSchema";
                string NEW_CLASS_NAME = "Test";

                // Clean up leftovers from previous tests:
                ShpTests.CleanUpClass(mConnection, null, NEW_CLASS_NAME);

                IApplySchema apply = (IApplySchema)mConnection.CreateCommand (CommandType.CommandType_ApplySchema);
                FeatureSchema schema = new FeatureSchema (NEW_SCHEMA_NAME, "");
                ClassCollection classes = schema.Classes;

                DataPropertyDefinition id = new DataPropertyDefinition("Id", "integer");
                id.DataType = DataType.DataType_Int32;
                GeometricPropertyDefinition location = new GeometricPropertyDefinition ("Geometry", "geometry");
                location.GeometryTypes = 0x01;
                location.HasElevation = true;
                location.HasMeasure = true;

                // assemble the feature class
                FeatureClass feature = new FeatureClass (NEW_CLASS_NAME, "test class created with apply schema");
                PropertyDefinitionCollection properties = feature.Properties;
                properties.Add (id);
                properties.Add (location);
                feature.GeometryProperty = location;
                DataPropertyDefinitionCollection identities = feature.IdentityProperties;
                identities.Add (id);

                // submit the new schema
                classes.Add (feature);
                apply.FeatureSchema = schema;
                apply.Execute ();

                // close and reopen the connection
                mConnection.Close ();
                Debug.Assert (mConnection.Open() == ConnectionState.ConnectionState_Open, "connection state not open");

                // check that the new schema shows up in the list
                IDescribeSchema describe = (IDescribeSchema)mConnection.CreateCommand (CommandType.CommandType_DescribeSchema);
                FeatureSchemaCollection schemas = describe.Execute ();

                IDisposableCollection collection = schemas.FindClass (NEW_CLASS_NAME);
                Debug.Assert (collection.Count == 1, "no class found");
                FeatureClass cls = (FeatureClass )collection[0];
                Debug.Assert (cls.Name == NEW_CLASS_NAME, "wrong name");

                // check it's contents
                properties = cls.Properties;
                DataPropertyDefinition featid = (DataPropertyDefinition)properties["Id"];
                Debug.Assert (featid.DataType == DataType.DataType_Int32, "id wrong type");
                location = (GeometricPropertyDefinition)properties["Geometry"];
                Debug.Assert (location.GeometryTypes == 0x01, "wrong geometry types");
                Debug.Assert (location.HasElevation, "wrong elevation");

                // OK, now delete the class
                schema = schemas[NEW_SCHEMA_NAME];
                classes = schema.Classes;
                ClassDefinition definition = classes[NEW_CLASS_NAME];
                definition.Delete ();
                apply.FeatureSchema = schema;
                apply.Execute ();
            }
            catch (OSGeo.Common.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
            catch (System.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
        }

        /// Test apply operation with only identity and attribute, but no geometry. /
        public void Test_apply_three ()
        {
            try
            {
                IConnection mConnection = base.ConnectionInitialized;

                string NEW_SCHEMA_NAME = "NewSchema";
                string NEW_CLASS_NAME = "Test";

                // Clean up leftovers from previous tests:
                ShpTests.CleanUpClass(mConnection, null, NEW_CLASS_NAME);

                IApplySchema apply = (IApplySchema)mConnection.CreateCommand (CommandType.CommandType_ApplySchema);
                FeatureSchema schema = new FeatureSchema (NEW_SCHEMA_NAME, "");
                ClassCollection classes = schema.Classes;

                DataPropertyDefinition id = new DataPropertyDefinition("FeatId", "integer");
                id.DataType = DataType.DataType_Int32;
                DataPropertyDefinition street = new DataPropertyDefinition("Street", "text");
                street.DataType = DataType.DataType_String;
                street.Length = 64;

                // assemble the class
                Class feature = new Class (NEW_CLASS_NAME, "test class created with apply schema");
                PropertyDefinitionCollection properties = feature.Properties;
                properties.Add (id);
                properties.Add (street);
                DataPropertyDefinitionCollection identities = feature.IdentityProperties;
                identities.Add (id);

                // submit the new schema
                classes.Add (feature);
                apply.FeatureSchema = schema;
                apply.Execute ();

                // close and reopen the connection
                mConnection.Close ();
                Debug.Assert (mConnection.Open() == ConnectionState.ConnectionState_Open, "connection state not open");

                // check that the new schema shows up in the list
                IDescribeSchema describe = (IDescribeSchema)mConnection.CreateCommand (CommandType.CommandType_DescribeSchema);
                FeatureSchemaCollection schemas = describe.Execute ();

                IDisposableCollection collection = schemas.FindClass (NEW_CLASS_NAME);
                Debug.Assert (collection.Count == 1, "no class found");
                FeatureClass cls = (FeatureClass )collection[0];
                Debug.Assert (cls.Name == NEW_CLASS_NAME, "wrong name");

                // check it's contents
                properties = cls.Properties;
                DataPropertyDefinition featid = (DataPropertyDefinition)properties["FeatId"];
                Debug.Assert (featid.DataType == DataType.DataType_Int32, "id wrong type");
                street = (DataPropertyDefinition)properties["Street"];
                Debug.Assert (street.DataType == DataType.DataType_String, "street wrong type");

                // OK, now delete the class
                schema = schemas[NEW_SCHEMA_NAME];
                classes = schema.Classes;
                ClassDefinition definition = classes[NEW_CLASS_NAME];
                definition.Delete ();
                apply.FeatureSchema = schema;
                apply.Execute ();
            }
            catch (OSGeo.Common.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
            catch (System.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
        }

        public void Test_add_class_no_identity ()
        {
            try
            {
                IConnection mConnection = base.ConnectionInitialized;

                ShpTests.CleanUpClass (mConnection, "Default", "Test");

                IDescribeSchema describe = (IDescribeSchema)mConnection.CreateCommand (CommandType.CommandType_DescribeSchema);
                FeatureSchemaCollection schemas = describe.Execute ();
                Debug.Assert (schemas.Count == 1, "Expecting 1 schema");
                FeatureSchema schema = schemas[0];
                ClassCollection classes = schema.Classes;

                IApplySchema apply = (IApplySchema)mConnection.CreateCommand (CommandType.CommandType_ApplySchema);

                DataPropertyDefinition street = new DataPropertyDefinition("Street", "text");
                street.DataType = DataType.DataType_String;
                street.Length = 64;

                // build a location geometry property
                GeometricPropertyDefinition location = new GeometricPropertyDefinition ("Geometry", "geometry");
                location.GeometryTypes = 0x01;
                location.HasElevation = true;
                location.HasMeasure = true;

                // assemble the feature class
                FeatureClass feature = new FeatureClass ("Test", "test class created with apply schema");
                PropertyDefinitionCollection properties = feature.Properties;
                properties.Add (street);
                properties.Add (location);
                feature.GeometryProperty = location;

                // submit the new schema
                classes.Add (feature);
                apply.FeatureSchema = schema;
                apply.Execute ();

                // close and reopen the connection
                mConnection.Close ();
                Debug.Assert (mConnection.Open() == ConnectionState.ConnectionState_Open, "connection state not open");

                // check that the new schema shows up in the list
                describe = (IDescribeSchema)mConnection.CreateCommand (CommandType.CommandType_DescribeSchema);
                schemas = describe.Execute ();
                Debug.Assert (schemas.Count == 1, "Expecting 1 schema");
                schema = schemas[0];
                classes = schema.Classes;
                FeatureClass cls = (FeatureClass)classes["Test"];
                Debug.Assert (cls != null, "Test class not found");

                // check that the definition is the same
                Debug.Assert (cls.Description == "test class created with apply schema", "class bad description");
                properties = cls.Properties;
                DataPropertyDefinition id = (DataPropertyDefinition)properties["FeatId"];
                Debug.Assert (id != null, "featid not found");
                Debug.Assert (id.DataType == DataType.DataType_Int32, "featid not DataType_Int32");
                street = (DataPropertyDefinition)properties["Street"];
                Debug.Assert (street!= null, "street not found");
                Debug.Assert (street.DataType == DataType.DataType_String, "street not DataType_String");
                Debug.Assert (street.Description == "text", "street bad description");
                location = (GeometricPropertyDefinition)properties["Geometry"];
                Debug.Assert (location != null, "geometry not found");
                Debug.Assert (location.Description == "geometry", "geometry bad description");
                Debug.Assert (location.GeometryTypes == 0x01, "location not points");
                Debug.Assert (location.HasElevation, "location no elevation");
                Debug.Assert (location.HasMeasure, "location no measure");
                GeometricPropertyDefinition geometry = cls.GeometryProperty;
                Debug.Assert (location == geometry, "wrong geometry");
                DataPropertyDefinitionCollection ids = cls.IdentityProperties;
                DataPropertyDefinition identifier = ids["FeatId"];
                Debug.Assert (id == identifier,  "wrong id");
            }
             catch (OSGeo.Common.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
            catch (System.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
        }

        void Test_switch_schema ()
        {
            try
            {
                IConnection mConnection = base.ConnectionInitialized;

                IDescribeSchema describe = (IDescribeSchema)mConnection.CreateCommand (CommandType.CommandType_DescribeSchema);
                FeatureSchemaCollection schemas = describe.Execute ();
                Debug.Assert (schemas.Count == 1, "Expecting 1 schema");
                FeatureSchema schema = schemas[0];
                ClassCollection classes = schema.Classes;

                // switch to another default file location (with a .prj file)
                mConnection.Close ();
                mConnection.ConnectionString = @"DefaultFileLocation=..\..\..\..\..\TestData\Florida"; 
       
                Debug.Assert (mConnection.Open() == ConnectionState.ConnectionState_Open, "connection state not open");
                describe = (IDescribeSchema)mConnection.CreateCommand (CommandType.CommandType_DescribeSchema);
                schemas = describe.Execute ();
                Debug.Assert (schemas.Count == 1, "Expecting 1 schema");
                schema = schemas[0];
                classes = schema.Classes;

                // switch to yet another default file location (with a different .prj file)
                mConnection.Close ();
                mConnection.ConnectionString = @"DefaultFileLocation=..\..\..\..\..\TestData\Sheboygan";
                Debug.Assert (mConnection.Open() == ConnectionState.ConnectionState_Open, "connection state not open");
                describe = (IDescribeSchema)mConnection.CreateCommand (CommandType.CommandType_DescribeSchema);
                schemas = describe.Execute ();
                Debug.Assert (schemas.Count == 1, "Expecting 1 schema");
                schema = schemas[0];
                classes = schema.Classes;
            }
            catch (OSGeo.Common.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
            catch (System.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
        }

        public void Test_destroy_schema ()
        {
            //string file_name1 = @"..\..\..\..\..\TestData\Ontario\lakes";
            //string file_name2 = @"..\..\..\..\..\TestData\Oahu\coundist01_n83";
          
            try
            {
                IConnection mConnection = base.ConnectionInitialized;

                //copy (mConnection, "Test1", file_name1);
                //copy (mConnection, "Test2", file_name2);
                mConnection.Close ();

                // destroy the schema
                Debug.Assert (mConnection.Open() == ConnectionState.ConnectionState_Open, "connection state not open");
                IDestroySchema destroy = (IDestroySchema)mConnection.CreateCommand (CommandType.CommandType_DestroySchema);
                destroy.SchemaName = "Default";
                destroy.Execute ();

                // check that it's not there
                IDescribeSchema describe = (IDescribeSchema)mConnection.CreateCommand (CommandType.CommandType_DescribeSchema);
                FeatureSchemaCollection schemas = describe.Execute ();
                Debug.Assert (schemas.Count == 0,  "Expecting 0 schemas");
                mConnection.Close ();

                // check that it's not there even after a reopen
                Debug.Assert (mConnection.Open() == ConnectionState.ConnectionState_Open, "connection state not open");
                describe = (IDescribeSchema)mConnection.CreateCommand (CommandType.CommandType_DescribeSchema);
                schemas = describe.Execute ();
                Debug.Assert (schemas.Count == 0, "Expecting 0 schemas after reopen");
            }
           catch (OSGeo.Common.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
            catch (System.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
        }

        public void Test_non_ascii_property_name ()
        {
            try
            {
                IConnection mConnection = base.ConnectionInitialized;

                string NEW_SCHEMA_NAME = "NewSchema";
                string NEW_CLASS_NAME = "Test";

                // Clean up leftovers from previous tests:
                ShpTests.CleanUpClass(mConnection, null, NEW_CLASS_NAME);
                
                IApplySchema apply = (IApplySchema)mConnection.CreateCommand (CommandType.CommandType_ApplySchema);
                FeatureSchema schema = new FeatureSchema (NEW_SCHEMA_NAME, "");
                ClassCollection classes = schema.Classes;

                DataPropertyDefinition id = new DataPropertyDefinition("Id", "integer");
                id.DataType = DataType.DataType_Int32;
        // French
                DataPropertyDefinition street = new DataPropertyDefinition("Stréet", "text");
                street.DataType = DataType.DataType_String;
                street.Length = 64;
        // Japanese
                DataPropertyDefinition area = new DataPropertyDefinition("Area\x5348\x524d", "double");
                area.DataType = DataType.DataType_Decimal;
                area.Precision = 20;
                area.Scale = 8;
                // build a location geometry property
                GeometricPropertyDefinition location = new GeometricPropertyDefinition ("Geometry", "geometry");
                location.GeometryTypes = 0x01;
                location.HasElevation = true;
                location.HasMeasure = true;

                //// assemble the feature class
                FeatureClass feature = new FeatureClass (NEW_CLASS_NAME, "test class created with apply schema");
                PropertyDefinitionCollection properties = feature.Properties;
                properties.Add (id);
                properties.Add (street);
                properties.Add (area);
                properties.Add (location);
                feature.GeometryProperty = location;
                DataPropertyDefinitionCollection identities = feature.IdentityProperties;
                identities.Add (id);

                // submit the new schema
                classes.Add (feature);
                apply.FeatureSchema = schema;
                apply.Execute ();

                // close and reopen the connection
                mConnection.Close ();
                Debug.Assert (mConnection.Open() == ConnectionState.ConnectionState_Open, "connection state not open");

                // check that the new schema shows up in the list
                IDescribeSchema describe = (IDescribeSchema)mConnection.CreateCommand (CommandType.CommandType_DescribeSchema);
                FeatureSchemaCollection schemas = describe.Execute ();

                IDisposableCollection collection = schemas.FindClass (NEW_CLASS_NAME);
                Debug.Assert (collection.Count == 1, "no class found");
                FeatureClass cls = (FeatureClass )collection[0];
                Debug.Assert (cls.Name == NEW_CLASS_NAME, "wrong name");
                Debug.Assert (cls.Description == "test class created with apply schema", "wrong description");

                // check it's contents
                properties = cls.Properties;
                DataPropertyDefinition featid = (DataPropertyDefinition)properties["Id"];
                Debug.Assert (featid.DataType == DataType.DataType_Int32, "id wrong type");
                street = (DataPropertyDefinition)properties["Stréet"];
                Debug.Assert (street.DataType == DataType.DataType_String, "street wrong type");
                Debug.Assert (street.Length == 64, "street wrong size");
                area = (DataPropertyDefinition)properties["Area\x5348\x524d"];
                Debug.Assert (area.DataType == DataType.DataType_Decimal, "area wrong type");
                location = (GeometricPropertyDefinition)properties["Geometry"];
                Debug.Assert (location.GeometryTypes == 0x01, "wrong geometry types");
                Debug.Assert (location.HasElevation, "wrong elevation");
                Debug.Assert (location.HasMeasure, "wrong measure");

                // OK, now delete the class
                schema = schemas[NEW_SCHEMA_NAME];
                classes = schema.Classes;
                ClassDefinition definition = classes[NEW_CLASS_NAME];
                definition.Delete ();
                apply.FeatureSchema = schema;
                apply.Execute ();
            }
            catch (OSGeo.Common.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
            catch (System.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
        }

        public void Test_non_ascii_class_name1()
        {
            try
            {
                IConnection mConnection = base.ConnectionInitialized;

                string NEW_SCHEMA_NAME = "NewSchema";
                string NEW_CLASS_NAME = "Tést";

                // Clean up leftovers from previous tests:
                ShpTests.CleanUpClass(mConnection, null, NEW_CLASS_NAME);

                IApplySchema apply = (IApplySchema)mConnection.CreateCommand (CommandType.CommandType_ApplySchema);
                FeatureSchema schema = new FeatureSchema (NEW_SCHEMA_NAME, "");
                ClassCollection classes = schema.Classes;

                DataPropertyDefinition id = new DataPropertyDefinition("Id", "integer");
                id.DataType = DataType.DataType_Int32;
        // French
                DataPropertyDefinition street = new DataPropertyDefinition("Stréet", "text");
                street.DataType = DataType.DataType_String;
                street.Length = 64;
        // Japanese
                DataPropertyDefinition area = new DataPropertyDefinition("Area\x5348\x524d", "double");
                area.DataType = DataType.DataType_Decimal;
                area.Precision = 20;
                area.Scale = 8;
                // build a location geometry property
                GeometricPropertyDefinition location = new GeometricPropertyDefinition ("Geometry", "geometry");
                location.GeometryTypes = 0x01;
                location.HasElevation = true;
                location.HasMeasure = true;

                //// assemble the feature class
                FeatureClass feature = new FeatureClass (NEW_CLASS_NAME, "test class created with apply schema");
                PropertyDefinitionCollection properties = feature.Properties;
                properties.Add (id);
                properties.Add (street);
                properties.Add (area);
                properties.Add (location);
                feature.GeometryProperty = location;
                DataPropertyDefinitionCollection identities = feature.IdentityProperties;
                identities.Add (id);

                // submit the new schema
                classes.Add (feature);
                apply.FeatureSchema = schema;
                apply.Execute ();

                // close and reopen the connection
                mConnection.Close ();
                Debug.Assert (mConnection.Open() == ConnectionState.ConnectionState_Open, "connection state not open");

                // check that the new schema shows up in the list
                IDescribeSchema describe = (IDescribeSchema)mConnection.CreateCommand (CommandType.CommandType_DescribeSchema);
                FeatureSchemaCollection schemas = describe.Execute ();

                IDisposableCollection collection = schemas.FindClass (NEW_CLASS_NAME);
                Debug.Assert (collection.Count == 1, "no class found");
                FeatureClass cls = (FeatureClass )collection[0];
                 Debug.Assert (cls.Name == NEW_CLASS_NAME, "wrong name");
                Debug.Assert (cls.Description == "test class created with apply schema", "wrong description");

                // check it's contents
                properties = cls.Properties;
                DataPropertyDefinition featid = (DataPropertyDefinition)properties["Id"];
                Debug.Assert (featid.DataType == DataType.DataType_Int32, "id wrong type");
                street = (DataPropertyDefinition)properties["Stréet"];
                Debug.Assert (street.DataType == DataType.DataType_String, "street wrong type");
                Debug.Assert (street.Length == 64, "street wrong size");
                area = (DataPropertyDefinition)properties["Area\x5348\x524d"];
                Debug.Assert (area.DataType == DataType.DataType_Decimal, "area wrong type");
                location = (GeometricPropertyDefinition)properties["Geometry"];
                Debug.Assert (location.GeometryTypes == 0x01, "wrong geometry types");
                Debug.Assert (location.HasElevation, "wrong elevation");
                Debug.Assert (location.HasMeasure, "wrong measure");

                // OK, now delete the class
                schema = schemas[NEW_SCHEMA_NAME];
                classes = schema.Classes;
                ClassDefinition definition = classes[NEW_CLASS_NAME];
                definition.Delete ();
                apply.FeatureSchema = schema;
                apply.Execute ();
            }
            catch (OSGeo.Common.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
            catch (System.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
        }

        public void Test_non_ascii_class_name2()
        {
            try
            {
                IConnection mConnection = base.ConnectionInitialized;

                string NEW_SCHEMA_NAME = "NewSchema";
                string NEW_CLASS_NAME = "T\x5348\x524dst";

                // Clean up leftovers from previous tests:
                ShpTests.CleanUpClass(mConnection, null, NEW_CLASS_NAME);

                IApplySchema apply = (IApplySchema)mConnection.CreateCommand (CommandType.CommandType_ApplySchema);
                FeatureSchema schema = new FeatureSchema (NEW_SCHEMA_NAME, "");
                ClassCollection classes = schema.Classes;

                DataPropertyDefinition id = new DataPropertyDefinition("Id", "integer");
                id.DataType = DataType.DataType_Int32;
        // French
                DataPropertyDefinition street = new DataPropertyDefinition("Stréet", "text");
                street.DataType = DataType.DataType_String;
                street.Length = 64;
        // Japanese
                DataPropertyDefinition area = new DataPropertyDefinition("Area\x5348\x524d", "double");
                area.DataType = DataType.DataType_Decimal;
                area.Precision = 20;
                area.Scale = 8;
                // build a location geometry property
                GeometricPropertyDefinition location = new GeometricPropertyDefinition ("Geometry", "geometry");
                location.GeometryTypes = 0x01;
                location.HasElevation = true;
                location.HasMeasure = true;

                //// assemble the feature class
                FeatureClass feature = new FeatureClass (NEW_CLASS_NAME, "test class created with apply schema");
                PropertyDefinitionCollection properties = feature.Properties;
                properties.Add (id);
                properties.Add (street);
                properties.Add (area);
                properties.Add (location);
                feature.GeometryProperty = location;
                DataPropertyDefinitionCollection identities = feature.IdentityProperties;
                identities.Add (id);

                // submit the new schema
                classes.Add (feature);
                apply.FeatureSchema = schema;
                apply.Execute ();

                // close and reopen the connection
                mConnection.Close ();
                Debug.Assert (mConnection.Open() == ConnectionState.ConnectionState_Open, "connection state not open");

                // check that the new schema shows up in the list
                IDescribeSchema describe = (IDescribeSchema)mConnection.CreateCommand (CommandType.CommandType_DescribeSchema);
                FeatureSchemaCollection schemas = describe.Execute ();

                IDisposableCollection collection = schemas.FindClass (NEW_CLASS_NAME);
                Debug.Assert (collection.Count == 1, "no class found");
                FeatureClass cls = (FeatureClass )collection[0];
                Debug.Assert (cls.Name == NEW_CLASS_NAME, "wrong name");
                Debug.Assert (cls.Description == "test class created with apply schema", "wrong description");

                // check it's contents
                properties = cls.Properties;
                DataPropertyDefinition featid = (DataPropertyDefinition)properties["Id"];
                Debug.Assert (featid.DataType == DataType.DataType_Int32, "id wrong type");
                street = (DataPropertyDefinition)properties["Stréet"];
                Debug.Assert (street.DataType == DataType.DataType_String, "street wrong type");
                Debug.Assert (street.Length == 64, "street wrong size");
                area = (DataPropertyDefinition)properties["Area\x5348\x524d"];
                Debug.Assert (area.DataType == DataType.DataType_Decimal, "area wrong type");
                location = (GeometricPropertyDefinition)properties["Geometry"];
                Debug.Assert (location.GeometryTypes == 0x01, "wrong geometry types");
                Debug.Assert (location.HasElevation, "wrong elevation");
                Debug.Assert (location.HasMeasure, "wrong measure");

                // OK, now delete the class
                schema = schemas[NEW_SCHEMA_NAME];
                classes = schema.Classes;
                ClassDefinition definition = classes[NEW_CLASS_NAME];
                definition.Delete ();
                apply.FeatureSchema = schema;
                apply.Execute ();
            }
            catch (OSGeo.Common.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
            catch (System.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
        }

        public void Test_non_ascii_schema_name()
        {
            try
            {
                IConnection mConnection = base.ConnectionInitialized;

                string NEW_SCHEMA_NAME = "N\x5348\x524dwSchema";
                string NEW_CLASS_NAME = "T\x5348\x524dst";

                // Clean up leftovers from previous tests:
                ShpTests.CleanUpClass(mConnection, null, NEW_CLASS_NAME);

                IApplySchema apply = (IApplySchema)mConnection.CreateCommand (CommandType.CommandType_ApplySchema);
                FeatureSchema schema = new FeatureSchema (NEW_SCHEMA_NAME, "");
                ClassCollection classes = schema.Classes;

                DataPropertyDefinition id = new DataPropertyDefinition("Id", "integer");
                id.DataType = DataType.DataType_Int32;
        // French
                DataPropertyDefinition street = new DataPropertyDefinition("Stréet", "text");
                street.DataType = DataType.DataType_String;
                street.Length = 64;
        // Japanese
                DataPropertyDefinition area = new DataPropertyDefinition("Area\x5348\x524d", "double");
                area.DataType = DataType.DataType_Decimal;
                area.Precision = 20;
                area.Scale = 8;
                // build a location geometry property
                GeometricPropertyDefinition location = new GeometricPropertyDefinition ("Geometry", "geometry");
                location.GeometryTypes = 0x01;
                location.HasElevation = true;
                location.HasMeasure = true;

                //// assemble the feature class
                FeatureClass feature = new FeatureClass (NEW_CLASS_NAME, "test class created with apply schema");
                PropertyDefinitionCollection properties = feature.Properties;
                properties.Add (id);
                properties.Add (street);
                properties.Add (area);
                properties.Add (location);
                feature.GeometryProperty = location;
                DataPropertyDefinitionCollection identities = feature.IdentityProperties;
                identities.Add (id);

                // submit the new schema
                classes.Add (feature);
                apply.FeatureSchema = schema;
                apply.Execute ();

                // close and reopen the connection
                mConnection.Close ();
                Debug.Assert(mConnection.Open() == ConnectionState.ConnectionState_Open, "connection state not open");

                // check that the new schema shows up in the list
                IDescribeSchema describe = (IDescribeSchema)mConnection.CreateCommand (CommandType.CommandType_DescribeSchema);
                FeatureSchemaCollection schemas = describe.Execute ();

                IDisposableCollection collection = schemas.FindClass (NEW_CLASS_NAME);
                Debug.Assert (collection.Count == 1, "no class found");
                FeatureClass cls = (FeatureClass )collection[0];
                Debug.Assert (cls.Name == NEW_CLASS_NAME, "wrong name");
                Debug.Assert (cls.Description == "test class created with apply schema", "wrong description");

                // check it's contents
                properties = cls.Properties;
                DataPropertyDefinition featid = (DataPropertyDefinition)properties["Id"];
                Debug.Assert (featid.DataType == DataType.DataType_Int32, "id wrong type");
                street = (DataPropertyDefinition)properties["Stréet"];
                Debug.Assert (street.DataType == DataType.DataType_String, "street wrong type");
                Debug.Assert (street.Length == 64, "street wrong size");
                area = (DataPropertyDefinition)properties["Area\x5348\x524d"];
                Debug.Assert (area.DataType == DataType.DataType_Decimal, "area wrong type");
                location = (GeometricPropertyDefinition)properties["Geometry"];
                Debug.Assert (location.GeometryTypes == 0x01, "wrong geometry types");
                Debug.Assert (location.HasElevation, "wrong elevation");
                Debug.Assert (location.HasMeasure, "wrong measure");

                // OK, now delete the class
                schema = schemas[NEW_SCHEMA_NAME];
                classes = schema.Classes;
                ClassDefinition definition = classes[NEW_CLASS_NAME];
                definition.Delete ();
                apply.FeatureSchema = schema;
                apply.Execute ();
            }
           catch (OSGeo.Common.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
            catch (System.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
        }

        public void Test_non_ascii_directory()
        {
            IConnection mConnection = base.ConnectionNew;
            try
            {
                try
                {
                    mConnection.Close ();
                    if(!Directory.Exists(LOCATION2))
                    {
                        Directory.CreateDirectory(LOCATION2);
                    }
                    
                    mConnection.ConnectionString = @"DefaultFileLocation=" + LOCATION2;
                    Debug.Assert (mConnection.Open() == ConnectionState.ConnectionState_Open, "connection state not open");
                }
                catch (OSGeo.Common.Exception ge)
                {
                    Debug.Fail(ge.Message);
                }
                string NEW_SCHEMA_NAME = "N\x5348\x524dwSchema";
                string NEW_CLASS_NAME = "T\x5348\x524dst";

                // Clean up leftovers from previous tests:
                ShpTests.CleanUpClass(mConnection, null, NEW_CLASS_NAME);

                IApplySchema apply = (IApplySchema)mConnection.CreateCommand (CommandType.CommandType_ApplySchema);
                FeatureSchema schema = new FeatureSchema (NEW_SCHEMA_NAME, "");
                ClassCollection classes = schema.Classes;

                DataPropertyDefinition id = new DataPropertyDefinition("Id", "integer");
                id.DataType = DataType.DataType_Int32;
        // French
                DataPropertyDefinition street = new DataPropertyDefinition("Stréet", "text");
                street.DataType = DataType.DataType_String;
                street.Length = 64;
        // Japanese
                DataPropertyDefinition area = new DataPropertyDefinition("Area\x5348\x524d", "double");
                area.DataType = DataType.DataType_Decimal;
                area.Precision = 20;
                area.Scale = 8;
                // build a location geometry property
                GeometricPropertyDefinition location = new GeometricPropertyDefinition ("Geometry", "geometry");
                location.GeometryTypes = 0x01;
                location.HasElevation = true;
                location.HasMeasure = true;

                //// assemble the feature class
                FeatureClass feature = new FeatureClass (NEW_CLASS_NAME, "test class created with apply schema");
                PropertyDefinitionCollection properties = feature.Properties;
                properties.Add (id);
                properties.Add (street);
                properties.Add (area);
                properties.Add (location);
                feature.GeometryProperty = location;
                DataPropertyDefinitionCollection identities = feature.IdentityProperties;
                identities.Add (id);

                // submit the new schema
                classes.Add (feature);
                apply.FeatureSchema = schema;
                apply.Execute ();

                // close and reopen the connection
                mConnection.Close ();
                Debug.Assert (mConnection.Open() == ConnectionState.ConnectionState_Open, "connection state not open");

                // check that the new schema shows up in the list
                IDescribeSchema describe = (IDescribeSchema)mConnection.CreateCommand (CommandType.CommandType_DescribeSchema);
                FeatureSchemaCollection schemas = describe.Execute ();

                IDisposableCollection collection = schemas.FindClass (NEW_CLASS_NAME);
                Debug.Assert (collection.Count == 1, "no class found");
                FeatureClass cls = (FeatureClass )collection[0];
                Debug.Assert (cls.Name == NEW_CLASS_NAME, "wrong name");
                Debug.Assert (cls.Description == "test class created with apply schema", "wrong description");

                // check it's contents
                properties = cls.Properties;
                DataPropertyDefinition featid = (DataPropertyDefinition)properties["Id"];
                Debug.Assert (featid.DataType == DataType.DataType_Int32, "id wrong type");
                street = (DataPropertyDefinition)properties["Stréet"];
                Debug.Assert (street.DataType == DataType.DataType_String, "street wrong type");
                Debug.Assert (street.Length == 64, "street wrong size");
                area = (DataPropertyDefinition)properties["Area\x5348\x524d"];
                Debug.Assert (area.DataType == DataType.DataType_Decimal, "area wrong type");
                location = (GeometricPropertyDefinition)properties["Geometry"];
                Debug.Assert (location.GeometryTypes == 0x01, "wrong geometry types");
                Debug.Assert (location.HasElevation, "wrong elevation");
                Debug.Assert (location.HasMeasure, "wrong measure");

                // OK, now delete the class
                schema = schemas[NEW_SCHEMA_NAME];
                classes = schema.Classes;
                ClassDefinition definition = classes[NEW_CLASS_NAME];
                definition.Delete ();
                apply.FeatureSchema = schema;
                apply.Execute ();

                mConnection.Close ();
                mConnection.ConnectionString = @"DefaultFileLocation=" + LOCATION;
                Debug.Assert (mConnection.Open() == ConnectionState.ConnectionState_Open, "connection state not open");
            }
            catch (OSGeo.Common.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
            catch (System.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
        }
    }
}
