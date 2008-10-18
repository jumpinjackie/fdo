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

using OSGeo.FDO.Connections;
using OSGeo.FDO.Commands;
using OSGeo.FDO.Commands.Schema;
using OSGeo.FDO.Commands.Feature;
using OSGeo.FDO.Schema;
using OSGeo.FDO.Expression;
using OSGeo.FDO.Geometry;

using unit_test.Framework;

namespace unit_test.ProviderTests
{
    class SelectAggregatesTests : BaseTestWithConnection
    {
        public SelectAggregatesTests(ShpTestProvider connectionProvider)
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

        private string className = "MyClass";
        private string schemaName = "MySchema";

        public void Test_select_distinct()
        {
            try
            {
                //////////////////////////////////////////////////////////////////////
                // Create a SHP file:
                //////////////////////////////////////////////////////////////////////

                create_schema(schemaName, className, GeometricType.GeometricType_Curve, false, false);


                //////////////////////////////////////////////////////////////////////
                // Select some data via distinct:
                //////////////////////////////////////////////////////////////////////

                ISelectAggregates select = (ISelectAggregates)base.ConnectionInitialized.CreateCommand(CommandType.CommandType_SelectAggregates);
                select.SetFeatureClassName(className);
                select.Distinct = true ;
                IdentifierCollection selectedIds = select.PropertyNames ;
                selectedIds.Clear();
                selectedIds.Add(new Identifier("Street"));
                IDataReader datareader = select.Execute();
                long count = 0;
                while (datareader.ReadNext())
                {
                    string streetname = datareader.GetString("Street");
                    count++;
                }
                Debug.Assert(count == 3, "Wrong distinct street count");
                datareader.Close();
                datareader = null;

                selectedIds.Clear();
                selectedIds.Add(new Identifier("Date"));
                datareader = select.Execute();
                count = 0;
                while (datareader.ReadNext())
                {
                    // DateTime aDate = datareader.GetDateTime("Date"); /// !!!! BUG
                    count++;
                }
                Debug.Assert(count == 2, "Wrong distinct date count");
                datareader.Close();
                datareader = null;
            }
            catch (OSGeo.FDO.Common.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
            catch (System.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
        }

        public void Test_select_aggregates()
        {
            try
            {
                //////////////////////////////////////////////////////////////////////
                // Create a SHP file:
                //////////////////////////////////////////////////////////////////////

                create_schema(schemaName, className, GeometricType.GeometricType_Curve, false, false);


                //////////////////////////////////////////////////////////////////////
                // Select some data via aggregate sum:
                //////////////////////////////////////////////////////////////////////

                ISelectAggregates select = (ISelectAggregates)base.ConnectionInitialized.CreateCommand (CommandType.CommandType_SelectAggregates);
                select.SetFeatureClassName (className);
                select.Distinct = false ;
                IdentifierCollection selectedIds = select.PropertyNames ;
                selectedIds.Clear();
                selectedIds.Add(new ComputedIdentifier("HeadTally", Expression.Parse("5+Sum(HeadCount+1)-Min(HeadCount-1)")));
                IDataReader datareader = select.Execute ();
                long count = 0;
                double headcounttally = 0;
                while (datareader.ReadNext ())
                {
                    headcounttally = (double)datareader.GetDouble("HeadTally");
                    count++;
                }
                Debug.Assert(count==1, "Wrong sum rowcount");
                Debug.Assert(headcounttally==26.0, "Wrong headcount sum");
                datareader.Close();
                datareader = null;
            }
            catch (OSGeo.FDO.Common.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
            catch (System.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
        }

        public void Test_select_aggregates_null()
        {
            try
            {
                //////////////////////////////////////////////////////////////////////
                // Create a SHP file with 0 rows:
                //////////////////////////////////////////////////////////////////////

                create_schema(schemaName, className, GeometricType.GeometricType_Curve, false, false);
                ShpTests.CleanUpClass(base.ConnectionInitialized, schemaName, className, true);


                //////////////////////////////////////////////////////////////////////
                // Try select aggregates on 0 rows
                //////////////////////////////////////////////////////////////////////

                ISelectAggregates select = (ISelectAggregates)base.ConnectionInitialized.CreateCommand (CommandType.CommandType_SelectAggregates);
                select.SetFeatureClassName (className);
                select.Distinct = false ;
                IdentifierCollection selectedIds = select.PropertyNames;
                selectedIds.Clear();
                selectedIds.Add(new ComputedIdentifier("HeadTally", Expression.Parse("5+Sum(HeadCount+1)-Min(HeadCount-1)")));
                IDataReader datareader = select.Execute ();
                Debug.Assert(datareader.ReadNext(), "Expected 1 aggregate result");
                Debug.Assert(datareader.IsNull("HeadTally"), "Expected null result for aggregate on 0 rows");
                Debug.Assert(!datareader.ReadNext (), "Expected 1 aggregate result");
                datareader.Close();
                datareader = null;


                //////////////////////////////////////////////////////////////////////
                // Insert some test data (with null values):
                //////////////////////////////////////////////////////////////////////

                IInsert insert = (IInsert)base.ConnectionInitialized.CreateCommand(CommandType.CommandType_Insert);
                insert.SetFeatureClassName(className);
                PropertyValueCollection propVals = insert.PropertyValues ;
                PropertyValue propVal;
                IFeatureReader reader;
                Expression expr;

                propVals.Clear();
                expr = Expression.Parse("'Slater'");
                propVal = new PropertyValue("Street", (ValueExpression)expr);
                propVals.Add(propVal);
                expr = new DateTimeValue() ;  // null
                propVal = new PropertyValue("Date", (ValueExpression)expr);
                propVals.Add(propVal);
                expr = Expression.Parse("GEOMFROMTEXT('LINESTRING XY (1.0 1.0, 2.0 2.0, 1.0 3.0)')");
                propVal = new PropertyValue("Geometry", (ValueExpression)expr);
                propVals.Add(propVal);
                expr = ShpTests.ParseByDataType("5", DataType.DataType_Int32);
                propVal = new PropertyValue("HeadCount", (ValueExpression)expr);
                propVals.Add(propVal);
                expr = ShpTests.ParseByDataType("7500.1", DataType.DataType_Decimal);
                propVal = new PropertyValue("LotSize", (ValueExpression)expr);
                propVals.Add(propVal);
                reader = insert.Execute();

                propVals.Clear();
                expr = Expression.Parse("'Slater'");
                propVal = new PropertyValue("Street", (ValueExpression)expr);
                propVals.Add(propVal);
                expr = Expression.Parse("DATE '2003-05-21'");
                propVal = new PropertyValue("Date", (ValueExpression)expr);
                propVals.Add(propVal);
                expr = Expression.Parse("GEOMFROMTEXT('LINESTRING XY (10.0 10.0, 20.0 20.0, 10.0 30.0)')");
                propVal = new PropertyValue("Geometry", (ValueExpression)expr);
                propVals.Add(propVal);
                expr = new DecimalValue();  //null
                propVal = new PropertyValue("HeadCount", (ValueExpression)expr);
                propVals.Add(propVal);
                expr = ShpTests.ParseByDataType("5500.5", DataType.DataType_Decimal);
                propVal = new PropertyValue("LotSize", (ValueExpression)expr);
                propVals.Add(propVal);
                reader = insert.Execute();

                propVals.Clear();
                expr = Expression.Parse("'Albert'");
                propVal = new PropertyValue("Street", (ValueExpression)expr);
                propVals.Add(propVal);
                expr = Expression.Parse("DATE '2003-05-21'");
                propVal = new PropertyValue("Date", (ValueExpression)expr);
                propVals.Add(propVal);
                expr = Expression.Parse("GEOMFROMTEXT('LINESTRING XY (3.0 3.0, 4.0 4.0, 5.0 5.0)')");
                propVal = new PropertyValue("Geometry", (ValueExpression)expr);
                propVals.Add(propVal);
                expr = ShpTests.ParseByDataType("4", DataType.DataType_Int32);
                propVal = new PropertyValue("HeadCount", (ValueExpression)expr);
                propVals.Add(propVal);
                expr = ShpTests.ParseByDataType("10000.0", DataType.DataType_Decimal);
                propVal = new PropertyValue("LotSize", (ValueExpression)expr);
                propVals.Add(propVal);
                reader = insert.Execute();

                propVals.Clear();
                expr = Expression.Parse("'Queen'");
                propVal = new PropertyValue("Street", (ValueExpression)expr);
                propVals.Add(propVal);
                expr = new DateTimeValue();  // null
                propVal = new PropertyValue("Date", (ValueExpression)expr);
                propVals.Add(propVal);
                expr = Expression.Parse("GEOMFROMTEXT('LINESTRING XY (1.0 1.0, -2.0 -2.0, -1.0 -3.0)')");
                propVal = new PropertyValue("Geometry", (ValueExpression)expr);
                propVals.Add(propVal);
                expr = ShpTests.ParseByDataType("7", DataType.DataType_Int32);
                propVal = new PropertyValue("HeadCount", (ValueExpression)expr);
                propVals.Add(propVal);
                expr = new DecimalValue();  //null
                propVal = new PropertyValue("LotSize", (ValueExpression)expr);
                propVals.Add(propVal);
                reader = insert.Execute();


                //////////////////////////////////////////////////////////////////////
                // Try select aggregates on rows that contain null values
                //////////////////////////////////////////////////////////////////////

                select = (ISelectAggregates)base.ConnectionInitialized.CreateCommand (CommandType.CommandType_SelectAggregates);
                select.SetFeatureClassName (className);
                select.Distinct = false ;
                selectedIds = select.PropertyNames;
                selectedIds.Clear();
                selectedIds.Add(new ComputedIdentifier("HeadTally", Expression.Parse("5+Sum(HeadCount+1)-Min(LotSize-1)")));
                datareader = select.Execute ();
                Debug.Assert(datareader.ReadNext (), "Expected 1 aggregate result");
                Debug.Assert(datareader.GetDouble("HeadTally")==-5475.5, "Didn't get expected aggregate result"); // 5+19-5499.5 == -5475.5
                Debug.Assert(!datareader.ReadNext (), "Expected 1 aggregate result");
                datareader.Close();
                datareader = null;
            }
            catch (OSGeo.FDO.Common.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
            catch (System.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
        }

        public void Test_select_orderby()
        {
            try
            {
                //////////////////////////////////////////////////////////////////////
                // Create a SHP file:
                //////////////////////////////////////////////////////////////////////

                create_schema(schemaName, className, GeometricType.GeometricType_Curve, false, false);


                //////////////////////////////////////////////////////////////////////
                // Select some data via orderby:
                //////////////////////////////////////////////////////////////////////

                ISelectAggregates select = (ISelectAggregates)base.ConnectionInitialized.CreateCommand (CommandType.CommandType_SelectAggregates);
                select.SetFeatureClassName (className);
                select.Distinct = false ;
                select.OrderingOption = OrderingOption.OrderingOption_Descending;
                IdentifierCollection orderingIds = select.Ordering ;
                orderingIds.Add(new Identifier("Street")) ;
                IdentifierCollection selectedIds = select.PropertyNames;
                selectedIds.Clear();
                selectedIds.Add(new Identifier("Street")) ;
                IDataReader datareader = select.Execute ();
                long count = 0;
                string street = null ;
                string previousStreet;
                while (datareader.ReadNext ())
                {
                    previousStreet = street;
                    street = datareader.GetString("Street");
                    if (previousStreet != null)
                        Debug.Assert(0>=string.Compare(street, previousStreet));
                    count++;
                }
                Debug.Assert(count==4, "Wrong orderby rowcount");
                datareader.Close();
                datareader = null;
            }
            catch (OSGeo.FDO.Common.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
            catch (System.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
        }

        public void Test_count()
        {
            try
            {
                //////////////////////////////////////////////////////////////////////
                // Create a SHP file:
                //////////////////////////////////////////////////////////////////////

                create_schema(schemaName, className, GeometricType.GeometricType_Curve, false, false);


                //////////////////////////////////////////////////////////////////////
                // Try function count():
                //////////////////////////////////////////////////////////////////////

                ISelectAggregates select = (ISelectAggregates)base.ConnectionInitialized.CreateCommand (CommandType.CommandType_SelectAggregates);
                select.SetFeatureClassName (className);
                select.Distinct = false ;
                IdentifierCollection selectedIds = select.PropertyNames;
                selectedIds.Clear();
                selectedIds.Add(new ComputedIdentifier("Total", Expression.Parse("Count(\"Date\")")));
                IDataReader datareader = select.Execute ();
                long count = 0;
                while (datareader.ReadNext ())
                {
                    Int64 total = datareader.GetInt64("Total");
                    Debug.Assert(4L == total, "Count wrong");
                    count++;
                }
                Debug.Assert(count==1, "Wrong count() rowcount");
                datareader.Close();
                datareader = null;
            }
            catch (OSGeo.FDO.Common.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
            catch (System.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
        }

        public void Test_select_orderby_decimal()
        {
            try
            {
                //////////////////////////////////////////////////////////////////////
                // Create a SHP file:
                //////////////////////////////////////////////////////////////////////

                create_schema(schemaName, className, GeometricType.GeometricType_Curve, false, false);


                //////////////////////////////////////////////////////////////////////
                // Select some data via orderby:
                //////////////////////////////////////////////////////////////////////

                ISelectAggregates select = (ISelectAggregates)base.ConnectionInitialized.CreateCommand (CommandType.CommandType_SelectAggregates);
                select.SetFeatureClassName (className);
                select.Distinct = false ;
                select.OrderingOption = OrderingOption.OrderingOption_Descending ;
                IdentifierCollection orderingIds = select.Ordering ;
                orderingIds.Add(new Identifier("LotSize"));
                IdentifierCollection selectedIds = select.PropertyNames;
                selectedIds.Clear();
                selectedIds.Add(new Identifier("LotSize")) ;
                IDataReader datareader = select.Execute ();
                long count = 0;
                double size = 0.0;
                double previous_size = 0.0;
                while (datareader.ReadNext ())
                {
                    previous_size = size;
                    size = datareader.GetDouble ("LotSize");
                    if (previous_size != 0.0)
                        Debug.Assert(size < previous_size, "out of order");
                    count++;
                }
                Debug.Assert(count==4, "Wrong orderby rowcount");
                datareader.Close();
                datareader = null;
            }
            catch (OSGeo.FDO.Common.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
            catch (System.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
        }

        public void Test_select_orderby_scrambled_property_ids()
        {
            try
            {
                //////////////////////////////////////////////////////////////////////
                // Create a SHP file:
                //////////////////////////////////////////////////////////////////////

                create_schema(schemaName, className, GeometricType.GeometricType_Curve, false, false);

                //////////////////////////////////////////////////////////////////////
                // Test:
                //////////////////////////////////////////////////////////////////////

                // DescribeSchema:
                IDescribeSchema getSchemaCmd = (IDescribeSchema)base.ConnectionInitialized.CreateCommand(CommandType.CommandType_DescribeSchema);
                FeatureSchemaCollection schemas = getSchemaCmd.Execute();

                FeatureSchema schema = schemas[schemas.IndexOf(schemaName)];
                ClassCollection classes = schema.Classes ;
                ClassDefinition klass = classes[classes.IndexOf(className)] ;

                // Get a feature reader for a selection ordered by the first identy property of the class.
                ISelectAggregates select = (ISelectAggregates) base.ConnectionInitialized.CreateCommand (CommandType.CommandType_SelectAggregates);
                select.SetFeatureClassName (klass.Name);
                select.Distinct = false ;
                select.OrderingOption = OrderingOption.OrderingOption_Descending ;
                IdentifierCollection orderingIds = select.Ordering;
                DataPropertyDefinitionCollection idProps = klass.IdentityProperties ;
                DataPropertyDefinition idProp = idProps[0] ;
                string firstIdentPropName = idProp.Name;
                orderingIds.Add(new Identifier(firstIdentPropName)) ;
                IdentifierCollection selectedIds = select.PropertyNames;

                // insert property names in 'SCRAMBLED' order (actually, reverse order of ClassDefinition):
                selectedIds.Clear();
                selectedIds.Add(new Identifier("Geometry")) ;
                selectedIds.Add(new Identifier("Date")) ;
                selectedIds.Add(new Identifier("HeadCount")) ;
                selectedIds.Add(new Identifier("LotSize")) ;
                selectedIds.Add(new Identifier("Street")) ;
                selectedIds.Add(new Identifier("FeatId")) ;

                IReader reader = select.Execute ();

                int count = 0;
                while (reader.ReadNext())
                {
                    string street = reader.GetString("Street");

                    Debug.Assert((0==string.Compare(street, "Slater")) || (0==string.Compare(street, "Albert")) || (0==string.Compare(street, "Queen")),
                        "Incorrect street name") ;

                    count++;
                }
            }
            catch (OSGeo.FDO.Common.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
            catch (System.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
        }

        public void Test_select_orderby_empty_select_list()
        {
            try
            {
                //////////////////////////////////////////////////////////////////////
                // Create a SHP file:
                //////////////////////////////////////////////////////////////////////

                create_schema(schemaName, className, GeometricType.GeometricType_Curve, false, false);
                
                //////////////////////////////////////////////////////////////////////
                // Select some data via orderby:
                //////////////////////////////////////////////////////////////////////

                ISelectAggregates select = (ISelectAggregates)base.ConnectionInitialized.CreateCommand (CommandType.CommandType_SelectAggregates);
                select.SetFeatureClassName (className);
                select.Distinct = false ;
                select.OrderingOption = OrderingOption.OrderingOption_Descending ;
                IdentifierCollection orderingIds = select.Ordering ;
                orderingIds.Add(new Identifier("Street")) ;
                IdentifierCollection selectedIds = select.PropertyNames;
                selectedIds.Clear();

                // KEEP THE SELECT LIST EMPTY TO TEST THIS BEHAVIOR CORRECTLY
                IDataReader datareader = select.Execute ();

                long count = 0;
                string street = null ;
                string previousStreet;
                while (datareader.ReadNext ())
                {
                    previousStreet = street ;
                    street = datareader.GetString("Street");
                    if (previousStreet != null)
                        Debug.Assert(0>=string.Compare(street, previousStreet));

                    // DateTime datetime = datareader.GetDateTime("Date"); // !!! BUG !!!
                    double lotsize = datareader.GetDouble("LotSize");
                    double headcount = datareader.GetInt32("HeadCount");
                    datareader.GetGeometry("Geometry") ;

                    count++;
                }

                Debug.Assert(count==4, "Wrong orderby rowcount");
                datareader.Close();
                datareader = null;
            }
            catch (OSGeo.FDO.Common.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
            catch (System.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
        }

        // utility methods:
        protected void create_schema(string schemaName, string className, GeometricType type, bool elevation, bool measure)
        {
            // Clean up leftover class, if any:
            ShpTests.CleanUpClass(base.ConnectionInitialized, null, className);

            //////////////////////////////////////////////////////////////////////
            // Create the schema:
            //////////////////////////////////////////////////////////////////////

            // create the class
            FeatureSchema schema = new FeatureSchema(schemaName, " test schema") ;
            ClassCollection classes = schema.Classes ;

            DataPropertyDefinition featid = new DataPropertyDefinition("FeatId", "integer") ;
            featid.DataType = DataType.DataType_Int32 ;
            featid.IsAutoGenerated = true ;
            featid.Nullable = false ;

            DataPropertyDefinition street = new DataPropertyDefinition("Street", "text") ;
            street.DataType = DataType.DataType_String ;
            street.Length = 64;

            DataPropertyDefinition lotsize = new DataPropertyDefinition("LotSize", "size") ;
            lotsize.DataType = DataType.DataType_Decimal ;
            lotsize.Precision = 20 ;
            lotsize.Scale = 8 ;

            DataPropertyDefinition headcount = new DataPropertyDefinition("HeadCount", "size") ;
            headcount.DataType = DataType.DataType_Int32 ;

            DataPropertyDefinition date = new DataPropertyDefinition("Date", "date") ;
            date.DataType = DataType.DataType_DateTime ;

            // build a location geometry property
            GeometricPropertyDefinition location = new GeometricPropertyDefinition("Geometry", "geometry") ;
            location.GeometryTypes = (int)type ;
            location.HasElevation = elevation ;
            location.HasMeasure = measure ;

            //// assemble the feature class
            FeatureClass feature = new FeatureClass(className, "test class created with apply schema") ;
            PropertyDefinitionCollection properties = feature.Properties ;
            DataPropertyDefinitionCollection identities = feature.IdentityProperties ;
            properties.Add (featid);
            identities.Add (featid);
            properties.Add (street);
            properties.Add (date);
            properties.Add (lotsize);
            properties.Add (headcount);
            properties.Add (location);
            feature.GeometryProperty = location ;

            // submit the new schema
            classes.Add (feature);
            IApplySchema apply = (IApplySchema)base.ConnectionInitialized.CreateCommand (CommandType.CommandType_ApplySchema) ;
            apply.FeatureSchema = schema ;
            apply.Execute ();


            //////////////////////////////////////////////////////////////////////
            // Insert some test data (with redundant values):
            //////////////////////////////////////////////////////////////////////

            IInsert insert = (IInsert)base.ConnectionInitialized.CreateCommand(CommandType.CommandType_Insert);
            insert.SetFeatureClassName(className);
            PropertyValueCollection propVals = insert.PropertyValues ;
            PropertyValue propVal;
            IFeatureReader reader;
            Expression expr;

            propVals.Clear();
            expr = Expression.Parse("'Slater'");
            propVal = new PropertyValue("Street", (ValueExpression)expr);
            propVals.Add(propVal);

            expr = Expression.Parse("DATE '1999-12-31'");
            propVal = new PropertyValue("Date", (ValueExpression)expr);
            propVals.Add(propVal);

            expr = Expression.Parse("GEOMFROMTEXT('LINESTRING XY (1.0 1.0, 2.0 2.0, 1.0 3.0)')");
            propVal = new PropertyValue("Geometry", (ValueExpression)expr) ;
            propVals.Add(propVal);

            expr = ShpTests.ParseByDataType("5", DataType.DataType_Int32);
            propVal = new PropertyValue("HeadCount", (ValueExpression)expr);
            propVals.Add(propVal);

            expr = ShpTests.ParseByDataType("7500.1", DataType.DataType_Decimal);
            propVal = new PropertyValue("LotSize", (ValueExpression)expr);
            propVals.Add(propVal);

            reader = insert.Execute();


            propVals.Clear();
            expr = Expression.Parse("'Slater'");
            propVal = new PropertyValue("Street", (ValueExpression)expr);
            propVals.Add(propVal);

            expr = Expression.Parse("DATE '2003-05-21'");
            propVal = new PropertyValue("Date", (ValueExpression)expr);
            propVals.Add(propVal);

            expr = Expression.Parse("GEOMFROMTEXT('LINESTRING XY (10.0 10.0, 20.0 20.0, 10.0 30.0)')");
            propVal = new PropertyValue("Geometry", (ValueExpression)expr) ;
            propVals.Add(propVal);

            expr = ShpTests.ParseByDataType("3", DataType.DataType_Int32);
            propVal = new PropertyValue("HeadCount", (ValueExpression)expr);
            propVals.Add(propVal);

            expr = ShpTests.ParseByDataType("5500.5", DataType.DataType_Decimal);
            propVal = new PropertyValue("LotSize", (ValueExpression)expr);
            propVals.Add(propVal);

            reader = insert.Execute();


            propVals.Clear();
            expr = Expression.Parse("'Albert'");
            propVal = new PropertyValue("Street", (ValueExpression)expr);
            propVals.Add(propVal);

            expr = Expression.Parse("DATE '2003-05-21'");
            propVal = new PropertyValue("Date", (ValueExpression)expr);
            propVals.Add(propVal);

            expr = Expression.Parse("GEOMFROMTEXT('LINESTRING XY (3.0 3.0, 4.0 4.0, 5.0 5.0)')");
            propVal = new PropertyValue("Geometry", (ValueExpression)expr);
            propVals.Add(propVal);

            expr = ShpTests.ParseByDataType("4", DataType.DataType_Int32);
            propVal = new PropertyValue("HeadCount", (ValueExpression)expr);
            propVals.Add(propVal);

            expr = ShpTests.ParseByDataType("10000.0", DataType.DataType_Decimal);
            propVal = new PropertyValue("LotSize", (ValueExpression)expr);
            propVals.Add(propVal);

            reader = insert.Execute();


            propVals.Clear();
            expr = Expression.Parse("'Queen'");
            propVal = new PropertyValue("Street", (ValueExpression)expr);
            propVals.Add(propVal);

            expr = Expression.Parse("DATE '2003-05-21'");
            propVal = new PropertyValue("Date", (ValueExpression)expr);
            propVals.Add(propVal);

            expr = Expression.Parse("GEOMFROMTEXT('LINESTRING XY (1.0 1.0, -2.0 -2.0, -1.0 -3.0)')");
            propVal = new PropertyValue("Geometry", (ValueExpression)expr);
            propVals.Add(propVal);

            expr = ShpTests.ParseByDataType("7", DataType.DataType_Int32);
            propVal = new PropertyValue("HeadCount", (ValueExpression)expr);
            propVals.Add(propVal);

            expr = ShpTests.ParseByDataType("2702.7", DataType.DataType_Decimal);
            propVal = new PropertyValue("LotSize", (ValueExpression)expr);
            propVals.Add(propVal);

            reader = insert.Execute();
        }
    } // class SelectAggregatesTests
} //namespace unit_test.ProviderTests
