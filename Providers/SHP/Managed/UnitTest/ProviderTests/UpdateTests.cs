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
using OSGeo.Geometry;

using unit_test.Framework;

namespace unit_test.ProviderTests
{
    class UpdateTests : BaseTestWithConnection
    {
        public UpdateTests(ShpTestProvider connectionProvider)
            : base(connectionProvider)
        { }

        public override void SetUp()
        {
            //
            if(!Directory.Exists(GetLocation()))
            {
                Directory.CreateDirectory(GetLocation());
            }

            base.SetUp() ;
        }

        public override void TearDown()
        {
            // Clean up leftover class, if any:
            ShpTests.CleanUpClass(base.ConnectionInitialized, null, className);

            base.TearDown() ;

            //
            if (File.Exists(GetLocation() + "schema.xml"))
            {
                File.Delete(GetLocation() + "schema.xml");
            }
        }

        public enum GeometricType
        {
            GeometricType_Point = 0x01,
            GeometricType_Curve = 0x02,
            GeometricType_Surface = 0x04,
            GeometricType_Solid = 0x08
        };

        private string className = "Test";
        private string schemaName = "TheSchema";

        public void Test_update ()
        {
            try
            {
                create_schema (schemaName, className, GeometricType.GeometricType_Point, true, true);

                IInsert insert = (IInsert)base.ConnectionInitialized.CreateCommand (CommandType.CommandType_Insert);
                insert.SetFeatureClassName(className);
                PropertyValueCollection values = insert.PropertyValues ;
                values.Clear() ;

                ValueExpression expression = (ValueExpression)ShpTests.ParseByDataType("24", DataType.DataType_Decimal); 
                PropertyValue value = new PropertyValue("Id", expression);
                values.Add (value);

                expression = (ValueExpression)Expression.Parse ("'1147 Trafford Drive'");
                value = new PropertyValue("Street", expression);
                values.Add (value);

                // add NULL geometry value:
                GeometryValue geometry = new GeometryValue();
                geometry.SetNull();
                value = new PropertyValue("Geometry", geometry);
                values.Add (value);

                IFeatureReader reader = insert.Execute ();
                int featid = -1;
                while (reader.ReadNext ())
                {
                    if (-1 != featid)
                        Debug.Fail("too many features inserted");
                    featid = reader.GetInt32 ("FeatId");
                }
                reader.Close ();
                if (-1 == featid)
                    Debug.Fail("too few features inserted");

                ISelect select = (ISelect)base.ConnectionInitialized.CreateCommand(CommandType.CommandType_Select);
                select.SetFeatureClassName(className);
                reader = select.Execute();
                while (reader.ReadNext())
                {
                    Debug.Assert(featid == reader.GetInt32("FeatId"), "incorrect featid value");
                    Debug.Assert(24 == reader.GetDouble("Id"), "incorrect id value");
                    Debug.Assert(0 == string.Compare("1147 Trafford Drive", reader.GetString("Street")), "incorrect street value");
                    Debug.Assert(reader.IsNull("Area"), "incorrect area value");
                    Debug.Assert(reader.IsNull("Vacant"), "incorrect vacant value");
                    Debug.Assert(reader.IsNull("Birthday"), "incorrect birthday year value");
                    Debug.Assert(reader.IsNull("Geometry"), "incorrect geometry value");
                }
                reader.Close();

                // update it
                IUpdate update = (IUpdate)base.ConnectionInitialized.CreateCommand (CommandType.CommandType_Update);
                update.SetFeatureClassName (className);
                
                string filter = "FeatId = " + featid.ToString() ;
                update.SetFilter (filter);
	            values = update.PropertyValues ;
                expression = (ValueExpression)Expression.Parse("'88 Noodle Court'");
                value = new PropertyValue("Street", expression);
                values.Add (value);
                if (1 != update.Execute ())
                    Debug.Fail("update execute failed");

                // check by doing a select
                select = (ISelect)base.ConnectionInitialized.CreateCommand (CommandType.CommandType_Select);
                select.SetFeatureClassName(className);
                reader = select.Execute ();
                while (reader.ReadNext ())
                {
                    Debug.Assert (featid == reader.GetInt32 ("FeatId"), "incorrect featid value");
                    Debug.Assert (24 == reader.GetDouble ("Id"), "incorrect id value");
                    Debug.Assert (0 == string.Compare("88 Noodle Court", reader.GetString ("Street")), "incorrect street value");
                    Debug.Assert (reader.IsNull("Area"), "incorrect area value");
                    Debug.Assert (reader.IsNull("Vacant"), "incorrect vacant value");
                    Debug.Assert (reader.IsNull ("Birthday"), "incorrect birthday year value");
                    Debug.Assert (reader.IsNull ("Geometry"), "incorrect geometry value");
                }
                reader.Close ();
            }
            catch (OSGeo.Common.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
        }

        // utility methods:
        protected void create_schema(string schemaName, string className, GeometricType type, bool elevation, bool measure)
        {
            // Clean up leftovers from previous tests:
            ShpTests.CleanUpClass(base.ConnectionInitialized, null, className);
            
            IApplySchema apply = (IApplySchema)base.ConnectionInitialized.CreateCommand(CommandType.CommandType_ApplySchema);
            FeatureSchema schema = new FeatureSchema(schemaName, "test schema");
            ClassCollection classes = schema.Classes;

            DataPropertyDefinition featid = new DataPropertyDefinition("FeatId", "integer");
            featid.DataType = DataType.DataType_Int32 ;
            featid.IsAutoGenerated = true ;
            featid.Nullable = false ;

            DataPropertyDefinition id = new DataPropertyDefinition("Id", "decimal");
            id.DataType = DataType.DataType_Decimal;
            id.Precision = 10;
            id.Scale = 10 ;
         
            DataPropertyDefinition street = new DataPropertyDefinition("Street", "text");
            street.DataType = DataType.DataType_String;
            street.Length = 64;

            DataPropertyDefinition area = new DataPropertyDefinition("Area", "double");
            area.DataType = DataType.DataType_Decimal;
            area.Precision = 20;
            area.Scale = 20;

            DataPropertyDefinition vacant = new DataPropertyDefinition("Vacant", "boolean");
            vacant.DataType = DataType.DataType_Boolean;

            DataPropertyDefinition birthday = new DataPropertyDefinition("Birthday", "date");
            birthday.DataType = DataType.DataType_DateTime;

            // build a location geometry property
            GeometricPropertyDefinition location = new GeometricPropertyDefinition("Geometry", "geometry");
            location.GeometryTypes = (int)type;
            location.HasElevation = elevation;
            location.HasMeasure = measure;

            //// assemble the feature class
            FeatureClass feature = new FeatureClass(className, "test class created with apply schema");
            PropertyDefinitionCollection properties = feature.Properties;
            properties.Add(featid);
            properties.Add(id);
            properties.Add(street);
            properties.Add(area);
            properties.Add(vacant);
            properties.Add(birthday);
            properties.Add(location);
            feature.GeometryProperty = location;
            DataPropertyDefinitionCollection identities = feature.IdentityProperties;
            identities.Add(featid);

            // submit the new schema
            classes.Add(feature);
            apply.FeatureSchema = schema;
            apply.Execute();
        }
    }
}
