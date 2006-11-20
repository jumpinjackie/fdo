using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Diagnostics;

using OSGeo.FDO;
using OSGeo.FDO.Schema;
using OSGeo.FDO.Expression;
using OSGeo.FDO.Commands;
using OSGeo.FDO.Commands.DataStore;
using OSGeo.FDO.Connections;
using OSGeo.FDO.Xml;
using OSGeo.FDO.Commands.Schema;
using OSGeo.FDO.Commands.SpatialContext;

using unit_test.Framework;

namespace unit_test.ProviderTests
{
    class SchemaTests : BaseTestWithConnection
    {
        public SchemaTests(TestProvider connectProvider)
            : base(connectProvider)
        {
            Console.WriteLine("start SchemaTests");
        }

        public override void SetUp()
        {
            base.SetUp();
        }

        public override void TearDown()
        {
            base.TearDown();
        }

        public void Test_Constraints()
        {
            IConnection mConnection = base.ConnectionInitialized;

            string fullpath = @"..\..\..\..\TestData\PARCEL_NEW.SDF";

            File.Delete(fullpath);

            ICreateDataStore CreateCmd = (ICreateDataStore)mConnection.CreateCommand(CommandType.CommandType_CreateDataStore);

            IDataStorePropertyDictionary dictionary = CreateCmd.DataStoreProperties;

            string[] names = dictionary.PropertyNames;

            dictionary.SetProperty(names[0], fullpath);

            CreateCmd.Execute();

            mConnection.ConnectionString = @"File=..\..\..\..\TestData\PARCEL_NEW.SDF";
            Debug.Assert(mConnection.ConnectionState == ConnectionState.ConnectionState_Closed, "connection state not closed");
            ConnectionState state = mConnection.Open();
            Debug.Assert(state == ConnectionState.ConnectionState_Open, "connection state not open");
            Debug.Assert(mConnection.ConnectionState == ConnectionState.ConnectionState_Open, "connection state not open");

            ICreateSpatialContext CreateCreateSpatialContext = (ICreateSpatialContext)mConnection.CreateCommand(CommandType.CommandType_CreateSpatialContext);
            CreateCreateSpatialContext.CoordinateSystemWkt = "[LL84]";
            CreateCreateSpatialContext.Description = "World Coordinate System, Degrees";
            CreateCreateSpatialContext.Name = "World Geodetic Coordinate System, 1984";
            CreateCreateSpatialContext.XYTolerance = 17.0;
            CreateCreateSpatialContext.ZTolerance = 3.14159;
            CreateCreateSpatialContext.Execute();

            IApplySchema applyschema = (IApplySchema)mConnection.CreateCommand(CommandType.CommandType_ApplySchema);

            FeatureSchema schema = new FeatureSchema("MySchema", "");
            FeatureClass clas = new FeatureClass("MyClass", "");
            ClassCollection classes = schema.Classes;
            classes.Add(clas);
            PropertyDefinitionCollection properties = clas.Properties;

            GeometricPropertyDefinition geometry = new GeometricPropertyDefinition("Geom", "");
            geometry.GeometryTypes = 7; // Point, Line, Polygon
            properties.Add(geometry);
            clas.GeometryProperty = geometry;

            DataPropertyDefinition dpd = new DataPropertyDefinition("NotNullString", "");
            dpd.Nullable = false;
            dpd.DataType = DataType.DataType_String;
            properties.Add(dpd);

            dpd = new DataPropertyDefinition("StringRange", "");
            PropertyValueConstraintRange rangeConstraint = new PropertyValueConstraintRange();
            rangeConstraint.MaxInclusive = false;
            rangeConstraint.MaxValue = new StringValue("Rich");
            rangeConstraint.MinInclusive = true;
            rangeConstraint.MinValue = new StringValue("Poor");
            dpd.ValueConstraint = rangeConstraint;
            dpd.Nullable = true;
            dpd.DataType = DataType.DataType_String;
            properties.Add(dpd);

            dpd = new DataPropertyDefinition("Int32Range", "The key of the object");
            dpd.DataType = DataType.DataType_Int32;
            rangeConstraint = new PropertyValueConstraintRange();
            rangeConstraint.MaxInclusive = true;
            rangeConstraint.MaxValue = new Int32Value(10000);
            rangeConstraint.MinInclusive = true;
            rangeConstraint.MinValue = new Int32Value(0);
            dpd.Nullable = true;
            dpd.ValueConstraint = rangeConstraint;
            properties.Add(dpd);
            DataPropertyDefinitionCollection idProps = clas.IdentityProperties;
            idProps.Add(dpd); 

            dpd = new DataPropertyDefinition("StringEnum", "");
            PropertyValueConstraintList listConstraint = new PropertyValueConstraintList();
            DataValueCollection listValues = listConstraint.ConstraintList;
            listValues.Add(new StringValue("Up"));
            listValues.Add(new StringValue("Down"));
            listValues.Add(new StringValue("Left"));
            listValues.Add(new StringValue("Right"));
            dpd.Nullable = true;
            dpd.ValueConstraint = listConstraint;
            dpd.DataType = DataType.DataType_String;
            properties.Add(dpd);

            dpd = new DataPropertyDefinition("Int32Enum", "");
            listConstraint = new PropertyValueConstraintList();
            listValues = listConstraint.ConstraintList;
            listValues.Add(new Int32Value(1));
            listValues.Add(new Int32Value(2));
            listValues.Add(new Int32Value(3));
            listValues.Add(new Int32Value(5));
            listValues.Add(new Int32Value(7));
            listValues.Add(new Int32Value(9));
            dpd.Nullable = true;
            dpd.ValueConstraint = listConstraint;
            dpd.DataType = DataType.DataType_Int32;
            properties.Add(dpd);

            applyschema.FeatureSchema = schema;
            applyschema.Execute();

            IDescribeSchema descSchema = (IDescribeSchema)mConnection.CreateCommand(CommandType.CommandType_DescribeSchema);
            FeatureSchemaCollection schemas = descSchema.Execute();
            Debug.Assert(schemas.Count == 1, "Expecting 1 schema");
            schema = schemas[0];
            Debug.Assert(schema.Name == "MySchema", "Expecting 'MySchema' schema");
            classes = schema.Classes;
            Debug.Assert(classes.Count == 1, "Expecting 1 class");
            ClassDefinition classDef = classes[0];
            Debug.Assert(classDef.Name == "MyClass", "Expecting 'MyClass' class");

            dpd = (DataPropertyDefinition)classDef.Properties["Int32Range"];
            rangeConstraint = (PropertyValueConstraintRange)dpd.ValueConstraint;
            Int32Value minVal = (Int32Value)rangeConstraint.MinValue;
            Int32Value maxVal = (Int32Value)rangeConstraint.MaxValue;

            Debug.Assert(minVal.Int32 == 0);
            Debug.Assert(maxVal.Int32 == 10000);

            mConnection.Close();
            Debug.Assert(mConnection.ConnectionState == ConnectionState.ConnectionState_Closed, "connection state not closed");

            File.Delete(fullpath);

            Console.WriteLine("Test_Constraints runs successfully !");
        }

    }
}
