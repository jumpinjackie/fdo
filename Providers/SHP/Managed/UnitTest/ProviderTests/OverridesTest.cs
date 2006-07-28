using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;
using System.IO;

using OSGeo.FDO.Connections;
using OSGeo.FDO.Commands;
using OSGeo.FDO.Commands.Schema;
using OSGeo.FDO.Commands.Feature;
using OSGeo.FDO.Schema;
using OSGeo.FDO.Expression;
using OSGeo.FDO.Xml;
using OSGeo.Geometry;
using OSGeo.Common.Io;
using OSGeo.Common.Xml;

//
using ShpObjectFactory = OSGeo.FDO.Providers.SHP.Override.ObjectFactory;
using ShpPhysicalSchemaMapping = OSGeo.FDO.Providers.SHP.Override.PhysicalSchemaMapping;
using ShpClassDefinition = OSGeo.FDO.Providers.SHP.Override.ClassDefinition;
using ShpClassCollection = OSGeo.FDO.Providers.SHP.Override.ClassCollection ;
using ShpColumnDefinition = OSGeo.FDO.Providers.SHP.Override.ColumnDefinition ;
using ShpPropertyDefinition = OSGeo.FDO.Providers.SHP.Override.PropertyDefinition ;
using ShpPropertyDefinitionCollection = OSGeo.FDO.Providers.SHP.Override.PropertyDefinitionCollection ;

using unit_test.Framework;

namespace unit_test.ProviderTests
{
    class OverridesTest : BaseTestWithConnection
    {
        public OverridesTest(ShpTestProvider connectionProvider)
            : base(connectionProvider)
        { 
            schemaNames = new string[2] ;
            schemaNames[0] = "Water"  ;
            schemaNames[1] = "Water2"  ;
            
            classNames = new string[2] ;
            classNames[0] = "Lake";
            classNames[1] = "River";

            columnNames = new string[2] ;
            columnNames[0] = "SoundingDate";
            columnNames[1] = "Surveyor";

            propertyNames = new string [2] ;
            propertyNames[0] = "SNDG_DATE";
            propertyNames[1] = "SRVYR";

            fileNames = new string [2] ;
            fileNames[0] = "/Foo/Lake.shp";
            fileNames[1] = "/Foo/River.shp";
        }

        public override void SetUp()
        {
            base.SetUp() ;
        }

        public override void TearDown()
        {
            base.TearDown() ;
        }

        public enum GeometricType
        {
            GeometricType_Point = 0x01,
            GeometricType_Curve = 0x02,
            GeometricType_Surface = 0x04,
            GeometricType_Solid = 0x08
        };

        String[] schemaNames ;
        String[] classNames ;
        String[] propertyNames ;
        String[] columnNames ;
        String[] fileNames ;


        const string ROOT_LOCATION = @"..\..\..\..\TestData\" ;
        const string ONTARIO_LOCATION = ROOT_LOCATION + @"Ontario\" ;
        const string NANBOKU_LOCATION = ROOT_LOCATION + @"Nanboku\" ;
        
        // Schema constants:
        const string schemaName   = "NewSchema";

        // Ontario class constants:
        const string classNameOntario = "NewOntario";
        const string shapefileOntario = ROOT_LOCATION + "OntarioGenerated.shp";
        const string propOntarioId =        "NewFeatId";
        const string propOntarioGeom =      "NewGeometry";
        const string propOntarioPerimeter = "NewPerimeter";
        const string propOntarioArea =      "NewArea";
        const string colOntarioPerimeter = "PERIMETER";
        const string colOntarioArea =      "AREA";

        // Nanboku class constants:
        const string classNameNanboku = "NewNanboku";
        const string shapefileNanboku = ROOT_LOCATION + "NanbokuGenerated.shp";
        const string propNanbokuId =        "NewFeatId";
        const string propNanbokuGeom =      "NewGeometry";
        const string propNanbokuGL =        "NewGL";
        const string colNanbokuGL =        "GL";
        const string filterNanboku =        "(NewGL < 25.0) and (NewFeatId < 100)";

        
        public void Test_CreateSchemaOverrides() //pass
        {
            try {
                for (int i=0; i<schemaNames.Length; i++)
                {
                    // Create a new in memory schema override set
                    ShpPhysicalSchemaMapping config = new ShpPhysicalSchemaMapping() ;
                    config.Name = schemaNames[i] ;
                    ShpClassCollection classes = config.Classes ;

                    // Create class overrides
                    for (int j=0; j<classNames.Length; j++)
                    {
                        // Create a new class override
                        ShpClassDefinition classDefn = new ShpClassDefinition();
                        classDefn.Name = classNames[j];
                        classDefn.ShapeFile = fileNames[j] ;
                        ShpPropertyDefinitionCollection properties = classDefn.Properties;

                        // Create property overrides
                        for (int k=0; k<propertyNames.Length; k++)
                        {
                            // Create a new property override
                            ShpPropertyDefinition propDefn = new ShpPropertyDefinition();
                            propDefn.Name = propertyNames[k];

                            // Create column override
                            ShpColumnDefinition columnDefn = new ShpColumnDefinition() ;
                            columnDefn.Name = columnNames[k];

                            // Set the associated column
                            propDefn.Column = (columnDefn);

                            // Add the property override
                            properties.Add(propDefn);
                        }
                
                        // Add the class override
                        classes.Add(classDefn);
                    }

                    // Test the data we just created to see if we can read what we wrote
                    TestNewConfiguration(config, i);
                }
            }
	        catch (OSGeo.Common.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
        }

        public void TestNewConfiguration(ShpPhysicalSchemaMapping schemaMapping, int schemaIndex)
        {
            try {
                string name = schemaMapping.Name;
                if (name != schemaNames[schemaIndex]) {
                    Debug.Fail("FAILED - ShpPhysicalSchemaMapping.Name");
                }

                ShpClassCollection classes = schemaMapping.Classes;
                for (int j=0; j<classNames.Length; j++)
                {
                    ShpClassDefinition classDefn = classes[j];
                    name = classDefn.Name;
                    if (name != classNames[j]) {
                        Debug.Fail("FAILED - ShpClassDefinition.Name");
                    }

                    string shapeFileName = classDefn.ShapeFile;
                    if (shapeFileName != fileNames[j]) {
                        Debug.Fail("FAILED - ShpClassDefinition.GetShapeFile()");
                    }

                    ShpPropertyDefinitionCollection properties = classDefn.Properties;
                    for (int k=0; k<propertyNames.Length; k++)
                    {
                        ShpPropertyDefinition propDefn = properties[k];
                        name = propDefn.Name;
                        if (name != propertyNames[k]) {
                            Debug.Fail("FAILED - ShpPropertyDefinition.Name");
                        }

                        ShpColumnDefinition columnDefn = propDefn.Column;
                        string columnName = columnDefn.Name;
                        if (columnName != columnNames[k]) {
                            Debug.Fail("FAILED - ShpColumnDefinition.Name");
                        }
                    }
                }
            }
            catch (OSGeo.Common.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
        }

        public void TestConfiguration(ShpPhysicalSchemaMapping schemaMapping)
        {
            try {
                TestBaseProperties(schemaMapping);
                string  providerName = schemaMapping.Provider ;
                ShpClassCollection classes = schemaMapping.Classes ;
                for (int i=0; i<classes.Count; i++) {
                    ShpClassDefinition classDefn = classes[i];
                    TestBaseProperties(classDefn);
                    ShpPropertyDefinitionCollection properties = classDefn.Properties;
                    for (int j=0; j<properties.Count; j++) {
                        ShpPropertyDefinition propDefn = properties[j];
                        TestBaseProperties(propDefn);
                        ShpColumnDefinition columnDefn = propDefn.Column ;
                        TestBaseProperties(columnDefn);
                    }
                }
            }
	        catch (OSGeo.Common.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
            catch (Exception e) 
            {
                Debug.Fail(e.Message);
            }
        }

        public void TestBaseProperties(PhysicalElementMapping elementMapping)
        {
            try {
                string  name = elementMapping.Name;
                string qualifiedName = elementMapping.QualifiedName ;
                PhysicalElementMapping parentMapping = (PhysicalElementMapping)elementMapping.Parent ;
                PhysicalSchemaMapping schemaMapping = elementMapping.SchemaMapping ;
            }
	        catch (OSGeo.Common.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
            catch (Exception e)
            {
                Debug.Fail(e.Message);
            }
        }

        public void Test_TestSetConfiguration()  //pass
        {
            try
            {
                IConnection connection = base.ConnectionNew ;
                if (connection == null) {
                    Debug.Fail("FAILED - CreateConnection returned null\n");
                }

                connection.ConnectionString = ("DefaultFileLocation=" + ONTARIO_LOCATION);
                IoFileStream fileStream = new IoFileStream("ShpSchemaConfig.xml", "r");

                connection.Configuration = (fileStream);
                connection.Open();
                connection.Close();

                connection.Configuration = (fileStream);
                connection.Configuration = (fileStream);

                connection.Open();

                bool bFail = false;
                try
                {
                    connection.Configuration = (fileStream);
                }
                catch (OSGeo.Common.Exception/* ex*/)
                {
                    bFail = true;
                    //Debug.Fail(ex.Message);
                }

                if (bFail == false) {
                    Debug.Fail("FAILED - SetConfiguration() while state == Open should not be supported\n");
                }

                connection.Close();
            }
	        catch (OSGeo.Common.Exception ex)
                {
                    Debug.Fail(ex.Message);
                }
        }


        public void Test_SetEmptyConfiguration() //pass
        {
            try
            {
                // clean up after prior test
                if (File.Exists(ONTARIO_LOCATION + "schema.xml"))
                    File.Delete(ONTARIO_LOCATION + "schema.xml");
                IConnection connection = base.ConnectionNew;
                connection.ConnectionString = ("DefaultFileLocation=" + ONTARIO_LOCATION);
                IoFileStream fileStream = new IoFileStream("ShpEmptySchemaConfig.xml", "r");
                connection.Configuration = (fileStream);
                connection.Open();

                // ensure there are zero schemas
                IDescribeSchema describe = (IDescribeSchema)connection.CreateCommand(CommandType.CommandType_DescribeSchema);
                FeatureSchemaCollection schemas = describe.Execute();
                Debug.Assert(0 == schemas.Count, "wrong schema count");

                // ensure asking for a nonexistant schema yields the proper exception
                try
                {
                    describe.SchemaName = ("Bogus");
                    schemas = describe.Execute();
                    Debug.Fail("no exception asking for a nonexistant schema in an empty configuration");
                }
                catch (OSGeo.Common.Exception ex)
                {
                    //Debug.Fail(ex.Message);
                    // check for expected message
                    Debug.Assert(0 == string.Compare("Schema 'Bogus' not found. ", ex.Message), "wrong exception");
                }

                connection.Close();
            }
            catch (OSGeo.Common.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
        }

        /* Test describe/select operations with schema overrides. */
        public void Test_ReadWithOverrides() //fail
        {
            string location = ONTARIO_LOCATION ;
            
            try
            {
                // Create schema mappings for config stream:
                /////////////////////////////////////////////////////////////////

                ShpPhysicalSchemaMapping schemaMapping = new ShpPhysicalSchemaMapping();
                schemaMapping.Name = (schemaName);
                ShpClassCollection classMappings = schemaMapping.Classes;

                // Create class mappings for Ontario:
                ShpClassDefinition classMapping = new ShpClassDefinition();
                classMapping.Name = (classNameOntario);
                classMapping.ShapeFile = "ontario.shp";

                ShpPropertyDefinitionCollection propMappings = classMapping.Properties;
                ShpPropertyDefinition propMapping = new ShpPropertyDefinition();
                propMapping.Name = (propOntarioPerimeter);
                ShpColumnDefinition colMapping = new ShpColumnDefinition();
                colMapping.Name = (colOntarioPerimeter);
                propMapping.Column = (colMapping);
                propMappings.Add(propMapping);

                propMapping = new ShpPropertyDefinition();
                propMapping.Name = (propOntarioArea);
                colMapping = new ShpColumnDefinition();
                colMapping.Name = (colOntarioArea);
                propMapping.Column = (colMapping);
                propMappings.Add(propMapping);

                classMappings.Add(classMapping);

                // Create class mappings for Nanboku:
                classMapping = new ShpClassDefinition();
                classMapping.Name = (classNameNanboku);
                classMapping.ShapeFile = "..\\Nanboku\\Nanboku_3dLine.shp";

                propMappings = classMapping.Properties;

                propMapping = new ShpPropertyDefinition();
                propMapping.Name = (propNanbokuGL);
                colMapping = new ShpColumnDefinition();
                colMapping.Name = (colNanbokuGL);
                propMapping.Column = (colMapping);
                propMappings.Add(propMapping);

                classMappings.Add(classMapping);


                // Create logical schema for config stream:
                ////////////////////////////////////////////////////////////////

                FeatureSchema configSchema = new FeatureSchema(schemaName, "");
                ClassCollection configClasses = configSchema.Classes;

                // Create ontario class:
                FeatureClass configClassOntario = new FeatureClass(classNameOntario, "");
                PropertyDefinitionCollection configPropsOntario = configClassOntario.Properties;
                DataPropertyDefinitionCollection configIdPropsOntario = configClassOntario.IdentityProperties;

                DataPropertyDefinition configPropOntarioID = new DataPropertyDefinition(propOntarioId, "");
                configPropOntarioID.DataType = (DataType.DataType_Int32);
                configPropOntarioID.IsAutoGenerated = (false);
                configPropOntarioID.Nullable = (false);
                configPropOntarioID.ReadOnly = (true);
                configPropsOntario.Add(configPropOntarioID);
                configIdPropsOntario.Add(configPropOntarioID);

                DataPropertyDefinition configPropOntarioPerimeter = new DataPropertyDefinition(propOntarioPerimeter, "");
                configPropOntarioPerimeter.DataType = (DataType.DataType_Decimal);
                configPropOntarioPerimeter.Precision = (20);
                configPropOntarioPerimeter.Scale = (8);
                configPropOntarioPerimeter.Nullable = (false);
                configPropOntarioPerimeter.ReadOnly = (false);
                configPropsOntario.Add(configPropOntarioPerimeter);

                DataPropertyDefinition configPropOntarioArea = new DataPropertyDefinition(propOntarioArea, "");
                configPropOntarioArea.DataType = (DataType.DataType_Decimal);
                configPropOntarioArea.Precision = (20);
                configPropOntarioArea.Scale = (8);
                configPropOntarioArea.Nullable = (false);
                configPropOntarioArea.ReadOnly = (false);
                configPropsOntario.Add(configPropOntarioArea);

                GeometricPropertyDefinition configPropOntarioGeom = new GeometricPropertyDefinition(propOntarioGeom, "");
                configPropOntarioGeom.GeometryTypes = (int)(GeometricType.GeometricType_Surface);
                configPropOntarioGeom.HasElevation = (false);
                configPropOntarioGeom.HasMeasure = (false);
                configPropOntarioGeom.ReadOnly = (false);
                configPropsOntario.Add(configPropOntarioGeom);

                configClassOntario.GeometryProperty = (configPropOntarioGeom);

                configClasses.Add(configClassOntario);

                // Create Nanboku class:
                FeatureClass configClassNanboku = new FeatureClass(classNameNanboku, "");
                PropertyDefinitionCollection configPropsNanboku = configClassNanboku.Properties;
                DataPropertyDefinitionCollection configIdPropsNanboku = configClassNanboku.IdentityProperties;

                DataPropertyDefinition configPropNanbokuID = new DataPropertyDefinition(propNanbokuId, "");
                configPropNanbokuID.DataType = (DataType.DataType_Int32);
                configPropNanbokuID.IsAutoGenerated = (false);
                configPropNanbokuID.Nullable = (false);
                configPropNanbokuID.ReadOnly = (true);
                configPropsNanboku.Add(configPropNanbokuID);
                configIdPropsNanboku.Add(configPropNanbokuID);

                DataPropertyDefinition configPropNanbokuGL = new DataPropertyDefinition(propNanbokuGL, "");
                configPropNanbokuGL.DataType = (DataType.DataType_Decimal);
                configPropNanbokuGL.Precision = (20);
                configPropNanbokuGL.Scale = (8);
                configPropNanbokuGL.Nullable = (false);
                configPropNanbokuGL.ReadOnly = (false);
                configPropsNanboku.Add(configPropNanbokuGL);

                GeometricPropertyDefinition configPropNanbokuGeom = new GeometricPropertyDefinition(propNanbokuGeom, "");
                configPropNanbokuGeom.GeometryTypes = (int)(GeometricType.GeometricType_Curve);
                configPropNanbokuGeom.HasElevation = (false);
                configPropNanbokuGeom.HasMeasure = (false);
                configPropNanbokuGeom.ReadOnly = (false);
                configPropsNanboku.Add(configPropNanbokuGeom);

                configClassNanboku.GeometryProperty = (configPropNanbokuGeom);

                configClasses.Add(configClassNanboku);


                // Create connection:
                ////////////////////////////////////////////////////////////////

                // Output to XML stream:
                IoMemoryStream  xmlConfigStream = new IoMemoryStream() ;
                XmlWriter xmlWriter = new XmlWriter(xmlConfigStream) ;
                XmlFlags xmlFlags = new XmlFlags();
                configSchema.WriteXml(xmlWriter, xmlFlags);
                schemaMapping.WriteXml(xmlWriter, xmlFlags);
                xmlWriter.Dispose();
                xmlWriter = null;  // required to close the datastore tag
                xmlConfigStream.Reset();

                // Create a new connection, passing in the XML config stream:
                IConnection conn = base.ConnectionNew;
                conn.ConnectionString = "DefaultFileLocation=" + location ;
                conn.Configuration = xmlConfigStream ;
                conn.Open();
        
                // Verify the schema looks the way we want:
                ////////////////////////////////////////////////////////////////
                VerifySchema(conn);

                // Disconnect and reconnect (to clear schema cache/overrides):
                ////////////////////////////////////////////////////////////////
                conn.Close();
            }
            catch (OSGeo.Common.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
        }

/* TEST IS FAILING. DEBUG AND CORERECT

        // Test applyschema/insert operations with schema overrides. 
        public void Test_WriteWithOverrides()  //pass
        {
            string location = ROOT_LOCATION;
            
            // clean up after prior test
            if (File.Exists(location + "clean.cmd"))
            {
                Process.Start(location + "clean.cmd") ;
            }

            try
            {
                // Create schema mappings for config stream:
                /////////////////////////////////////////////////////////////////

                ShpPhysicalSchemaMapping schemaMapping = new ShpPhysicalSchemaMapping();
                schemaMapping.Name = (schemaName);
                ShpClassCollection classMappings = schemaMapping.Classes ;

                // Create class mappings for Ontario:
                ShpClassDefinition classMapping = new ShpClassDefinition();
                classMapping.Name = (classNameOntario);
                classMapping.ShapeFile = (shapefileOntario);

                ShpPropertyDefinitionCollection propMappings = classMapping.Properties;
                ShpPropertyDefinition propMapping = new ShpPropertyDefinition();
                propMapping.Name = (propOntarioPerimeter);
                ShpColumnDefinition colMapping = new ShpColumnDefinition();
                colMapping.Name = (colOntarioPerimeter);
                propMapping.Column = (colMapping);
                propMappings.Add(propMapping);

                propMapping = new ShpPropertyDefinition();
                propMapping.Name = (propOntarioArea);
                colMapping = new ShpColumnDefinition();
                colMapping.Name = (colOntarioArea);
                propMapping.Column = (colMapping);
                propMappings.Add(propMapping);

                classMappings.Add(classMapping);

                // Create class mappings for Nanboku:
                classMapping = new ShpClassDefinition();
                classMapping.Name = (classNameNanboku);
                classMapping.ShapeFile = (shapefileNanboku);

                propMappings = classMapping.Properties;

                propMapping = new ShpPropertyDefinition();
                propMapping.Name = (propNanbokuGL);
                colMapping = new ShpColumnDefinition();
                colMapping.Name = (colNanbokuGL);
                propMapping.Column = (colMapping);
                propMappings.Add(propMapping);

                classMappings.Add(classMapping);


                // Create logical schema for config stream:
                ////////////////////////////////////////////////////////////////

                FeatureSchema configSchema = new FeatureSchema(schemaName, "");
                ClassCollection configClasses = configSchema.Classes ;

                // Create ontario class:
                FeatureClass configClassOntario = new FeatureClass(classNameOntario, "");
                PropertyDefinitionCollection configPropsOntario = configClassOntario.Properties;
                DataPropertyDefinitionCollection configIdPropsOntario = configClassOntario.IdentityProperties;

                DataPropertyDefinition configPropOntarioID = new DataPropertyDefinition(propOntarioId, "");
                configPropOntarioID.DataType = (DataType.DataType_Int32);
                configPropOntarioID.IsAutoGenerated = (false);
                configPropOntarioID.Nullable = (false);
                configPropOntarioID.ReadOnly = (true);
                configPropsOntario.Add(configPropOntarioID);
                configIdPropsOntario.Add(configPropOntarioID);

                DataPropertyDefinition configPropOntarioPerimeter = new DataPropertyDefinition(propOntarioPerimeter, "");
                configPropOntarioPerimeter.DataType = (DataType.DataType_Decimal);
                configPropOntarioPerimeter.Precision = (20);
                configPropOntarioPerimeter.Scale = (8);
                configPropOntarioPerimeter.Nullable = (false);
                configPropOntarioPerimeter.ReadOnly = (false);
                configPropsOntario.Add(configPropOntarioPerimeter);

                DataPropertyDefinition configPropOntarioArea = new DataPropertyDefinition(propOntarioArea, "");
                configPropOntarioArea.DataType = (DataType.DataType_Decimal);
                configPropOntarioArea.Precision = (20);
                configPropOntarioArea.Scale = (8);
                configPropOntarioArea.Nullable = (false);
                configPropOntarioArea.ReadOnly = (false);
                configPropsOntario.Add(configPropOntarioArea);

                GeometricPropertyDefinition configPropOntarioGeom = new GeometricPropertyDefinition(propOntarioGeom, "");
                configPropOntarioGeom.GeometryTypes = (int)(GeometricType.GeometricType_Surface);
                configPropOntarioGeom.HasElevation = (false);
                configPropOntarioGeom.HasMeasure = (false);
                configPropOntarioGeom.ReadOnly = (false);
                configPropsOntario.Add(configPropOntarioGeom);

                configClassOntario.GeometryProperty = (configPropOntarioGeom);

                configClasses.Add(configClassOntario);

                // Create Nanboku class:
                FeatureClass configClassNanboku = new FeatureClass(classNameNanboku, "");
                PropertyDefinitionCollection configPropsNanboku = configClassNanboku.Properties;
                DataPropertyDefinitionCollection configIdPropsNanboku = configClassNanboku.IdentityProperties;

                DataPropertyDefinition configPropNanbokuID = new DataPropertyDefinition(propNanbokuId, "");
                configPropNanbokuID.DataType = (DataType.DataType_Int32);
                configPropNanbokuID.IsAutoGenerated = (false);
                configPropNanbokuID.Nullable = (false);
                configPropNanbokuID.ReadOnly = (true);
                configPropsNanboku.Add(configPropNanbokuID);
                configIdPropsNanboku.Add(configPropNanbokuID);

                DataPropertyDefinition configPropNanbokuGL = new DataPropertyDefinition(propNanbokuGL, "");
                configPropNanbokuGL.DataType = (DataType.DataType_Decimal);
                configPropNanbokuGL.Precision = (20);
                configPropNanbokuGL.Scale = (8);
                configPropNanbokuGL.Nullable = (false);
                configPropNanbokuGL.ReadOnly = (false);
                configPropsNanboku.Add(configPropNanbokuGL);

                GeometricPropertyDefinition configPropNanbokuGeom = new GeometricPropertyDefinition(propNanbokuGeom, "");
                configPropNanbokuGeom.GeometryTypes = (int)(GeometricType.GeometricType_Curve);
                configPropNanbokuGeom.HasElevation = (false);
                configPropNanbokuGeom.HasMeasure = (false);
                configPropNanbokuGeom.ReadOnly = (false);
                configPropsNanboku.Add(configPropNanbokuGeom);

                configClassNanboku.GeometryProperty = (configPropNanbokuGeom);

                configClasses.Add(configClassNanboku);


                // Create connection:
                ////////////////////////////////////////////////////////////////

                // Create a new connection, with NO configuration file:
                IConnection conn = base.ConnectionNew;
                conn.ConnectionString = ("DefaultFileLocation=" + location);
                conn.Open();

                // Clean up leftovers from previous tests:
                ShpTests.CleanUpClass(conn, null, classNameOntario);
                ShpTests.CleanUpClass(conn, null, classNameNanboku);


                // Create the new classes using ApplySchema and the overrides:
                ////////////////////////////////////////////////////////////////

                IApplySchema apply = (IApplySchema)conn.CreateCommand(CommandType.CommandType_ApplySchema);
                apply.FeatureSchema = (configSchema);
                apply.PhysicalMapping = (schemaMapping);
                apply.Execute();

                // Verify the schema looks the way we want:
                ////////////////////////////////////////////////////////////////
                VerifySchema (conn);

                // Disconnect and reconnect (to clear schema cache/overrides):
                ////////////////////////////////////////////////////////////////
                conn.Close ();

                conn.Open();
                VerifySchema (conn);
                conn.Close ();

                // Try a completely different connection:
                ////////////////////////////////////////////////////////////////
                IConnection connection = base.ConnectionNew;
                connection.ConnectionString = ("DefaultFileLocation=" + location);
                connection.Open();
            }
            catch (OSGeo.Common.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
        }

*/

        void VerifySchema (IConnection conn)
        {
            IDescribeSchema describe = (IDescribeSchema)conn.CreateCommand (CommandType.CommandType_DescribeSchema);
            FeatureSchemaCollection schemas = describe.Execute ();
            FeatureSchema schema = schemas[schemas.IndexOf(schemaName)];
            ClassCollection classes = schema.Classes ;
            Debug.Assert(2==classes.Count, "wrong class count");

            // validate Ontario class:
            ClassDefinition aClass = classes[classes.IndexOf(classNameOntario)] ;
            Debug.Assert(ClassType.ClassType_FeatureClass == aClass.ClassType, "wrong class type");
            PropertyDefinitionCollection properties = aClass.Properties;
            Debug.Assert(4 == properties.Count, "wrong property count");
            PropertyDefinition property = properties[properties.IndexOf(propOntarioId)] ;
            DataPropertyDefinition dataProperty = (DataPropertyDefinition)property;
            Debug.Assert(null!=dataProperty, "wrong property type");
            Debug.Assert(DataType.DataType_Int32==dataProperty.DataType, "wrong data type");
            property = properties[properties.IndexOf(propOntarioPerimeter)] ;
            dataProperty = (DataPropertyDefinition)property;
            Debug.Assert(null!=dataProperty, "wrong property type");
            Debug.Assert(DataType.DataType_Decimal==dataProperty.DataType, "wrong data type");
            property = properties[properties.IndexOf(propOntarioArea)] ;
            dataProperty = (DataPropertyDefinition)property;
            Debug.Assert(null!=dataProperty, "wrong property type");
            Debug.Assert(DataType.DataType_Decimal==dataProperty.DataType, "wrong data type");
            property = properties[properties.IndexOf(propOntarioGeom)] ;
            GeometricPropertyDefinition geomProperty = (GeometricPropertyDefinition)property;
            Debug.Assert(null!=geomProperty, "wrong property type");
            Debug.Assert((int)GeometricType.GeometricType_Surface == geomProperty.GeometryTypes, "wrong geom types");

            // validate Nanboku class:
            aClass = classes[classes.IndexOf(classNameNanboku)] ;
            Debug.Assert(ClassType.ClassType_FeatureClass == aClass.ClassType, "wrong class type");
            properties = aClass.Properties;
            Debug.Assert(3==properties.Count, "wrong property count");
            property = properties[properties.IndexOf(propNanbokuId)];
            dataProperty = (DataPropertyDefinition)property;
            Debug.Assert(null!=dataProperty, "wrong property type");
            Debug.Assert(DataType.DataType_Int32==dataProperty.DataType, "wrong data type");
            property = properties[properties.IndexOf(propNanbokuGL)];
            dataProperty = (DataPropertyDefinition)property;
            Debug.Assert(null!=dataProperty, "wrong property type");
            Debug.Assert(DataType.DataType_Decimal==dataProperty.DataType, "wrong data type");
            property = properties[properties.IndexOf(propNanbokuGeom)];
            geomProperty = (GeometricPropertyDefinition)property;
            Debug.Assert(null!=geomProperty, "wrong property type");
            Debug.Assert((int)GeometricType.GeometricType_Curve == geomProperty.GeometryTypes, "wrong geom types");

            // Select from Ontario class (no filter, no property subset):
            ISelect select = (ISelect)conn.CreateCommand(CommandType.CommandType_Select);
            select.SetFeatureClassName(classNameOntario);
            IFeatureReader reader = select.Execute();
            while (reader.ReadNext())
            {
                int id = reader.GetInt32(propOntarioId);
                reader.GetGeometry(propOntarioGeom);
                double perimeter = reader.GetDouble(propOntarioPerimeter);
                double area = reader.GetDouble(propOntarioArea);
            }

            // Select from Nanboku class (with filter and property subset):
            select = (ISelect)conn.CreateCommand(CommandType.CommandType_Select);
            select.SetFeatureClassName(classNameNanboku);
            select.SetFilter(filterNanboku);
            IdentifierCollection selectedProperties = select.PropertyNames ;
            Identifier selectedProperty = new Identifier(propNanbokuId);
            selectedProperties.Add(selectedProperty);
            selectedProperty = new Identifier(propNanbokuGL);
            selectedProperties.Add(selectedProperty);
            reader = select.Execute();
            while (reader.ReadNext())
            {
                int id = reader.GetInt32(propNanbokuId);
                double gl = reader.GetDouble(propNanbokuGL);

                Debug.Assert(id < 100, "NewFeatId not filtered out as expected");
                Debug.Assert(gl<25.0, "NewGL not filtered out as expected");

                // should throw exception (not selected):
                try
                {
                    reader.GetGeometry(propNanbokuGeom);
                    Debug.Fail("Didn't get expected exception when fetching property not in select set");
                }
                catch (OSGeo.Common.Exception ex)
                {
                    // We expect to get here, so don't re-throw exception
                }
            }
        }


    }
}
