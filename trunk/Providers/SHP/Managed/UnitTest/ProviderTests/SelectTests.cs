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

using OSGeo.FDO.Connections;
using OSGeo.FDO.Commands;
using OSGeo.FDO.Commands.Schema;
using OSGeo.FDO.Commands.Feature;
using OSGeo.FDO.Commands.SpatialContext;
using OSGeo.FDO.Schema;
using OSGeo.FDO.Expression;
using OSGeo.Geometry;
using OSGeo.Common;

using unit_test.Framework;

namespace unit_test.ProviderTests
{
    class SelectTests : BaseTestWithConnection
    {
        public SelectTests(ShpTestProvider connectProvider)
            : base(connectProvider)
        { }

        public void Test_select()
        {
            double area = 0;
            double length = 0;
            int count = 0;
            IConnection mConnection = base.ConnectionInitialized;
            try
            {
                ISelect select = (ISelect)mConnection.CreateCommand(CommandType.CommandType_Select);
                select.SetFeatureClassName("ontario");
                IFeatureReader reader = select.Execute(); 
                count = 0;
                while (reader.ReadNext())
                {
                    count++;
                    reader.GetInt32("FeatId");
                    area = reader.GetDouble("AREA");
                    length = reader.GetDouble("PERIMETER");
                    reader.GetDouble("ONTARIO_");
                    reader.GetDouble("ONTARIO_ID");
                    byte[] geometry = reader.GetGeometry("Geometry");

                    ShpTests.AnalyzeGeometry("ontario", count, geometry, length, area);
                }
                Debug.Assert(count != 0, "no ontario features selected");

                select.SetFeatureClassName("roads");
                reader = select.Execute();
                count = 0;
                while (reader.ReadNext())
                {
                    count++;
                    reader.GetInt32("FeatId");
                    reader.GetDouble("FNODE_");
                    reader.GetDouble("TNODE_");
                    reader.GetDouble("LPOLY_");
                    reader.GetDouble("RPOLY_");
                    length = reader.GetDouble("LENGTH");
                    reader.GetDouble("ROADWAYS_");
                    reader.GetDouble("ROADWAYS_I");
                    reader.GetDouble("MNRCODE");
                    reader.GetString("FCODE");
                    byte[] geometry = reader.GetGeometry("Geometry");

                    ShpTests.AnalyzeGeometry("ontario", count, geometry, length, area);
                }
                Debug.Assert(count != 0, "no roads features selected");

                select.SetFeatureClassName("lakes");
                reader = select.Execute();
                count = 0;
                while (reader.ReadNext())
                {
                    count++;
                    reader.GetInt32("FeatId");
                    area = reader.GetDouble("AREA");
                    length = reader.GetDouble("PERIMETER");
                    reader.GetDouble("DRAINAGE_");
                    reader.GetDouble("DRAINAGE_I");
                    if (!reader.IsNull("HOTLINK"))
                        reader.GetString("HOTLINK");
                    byte[] geometry = reader.GetGeometry("Geometry");

                    ShpTests.AnalyzeGeometry("ontario", count, geometry, length, area);
                }
                Debug.Assert(count != 0, "no lakes features selected");
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

        public void Test_get_spatial_contexts()
        {
            try
            {
                // Test #1. No PRJ file
                get_spatial_context(base.ConnectionInitialized, "Default");

                // Test #2. PRJ file present
                IConnection connection = base.ConnectionNew;

                connection.ConnectionString = @"DefaultFileLocation=..\..\..\..\TestData\Florida";

                Debug.Assert(connection.Open() == ConnectionState.ConnectionState_Open, "connection state not open");

                get_spatial_context(connection, "NAD_1983_HARN_UTM_Zone_17N");

                connection.Close();
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

        //**********************Runtime error, debug later*********************
        private void get_spatial_context(IConnection connection, string cs_name)
        {
            IGetSpatialContexts get;
            ISpatialContextReader reader;
            int count;

            try
            {
                get = (IGetSpatialContexts)connection.CreateCommand(CommandType.CommandType_GetSpatialContexts);
                reader = get.Execute();
                count = 0;
                while (reader.ReadNext())
                    count++;
                Debug.Assert(count != 0, "no spatial context");

                get.ActiveOnly = true; // BUG !!!!! ActiveOnly not respected
                reader = get.Execute();
                count = 0;
                while (reader.ReadNext())
                {
                    string name = reader.GetName();
                    if (name == cs_name)
                    {
                        Debug.Assert(reader.GetName() == cs_name, "Coordinate System not active");

                        string wkt = reader.GetCoordinateSystemWkt();
                        string cs = reader.GetCoordinateSystem();

                        if (cs_name != "Default")
                            Debug.Assert(wkt != "", "WKT Coordinate System is not set");
                        else
                            Debug.Assert(wkt == "", "WKT Coordinate System is set");

                        count++;
                    }
                }
                Debug.Assert(count != 0, "no spatial context");
                Debug.Assert(count == 1, "too many spatial contexts");
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

        public void Test_get_class_definition()
        {
            ISelect select = (ISelect)base.ConnectionInitialized.CreateCommand(CommandType.CommandType_Select);
            select.SetFeatureClassName("ontario");
            IFeatureReader reader = select.Execute();
            reader.ReadNext();
            ClassDefinition definition = reader.GetClassDefinition();
            Debug.Assert(definition.ClassType == ClassType.ClassType_FeatureClass, "wrong class type");
            Debug.Assert(definition.Name == "ontario", "wrong class name");
            ClassCapabilities caps = definition.Capabilities;
            Debug.Assert(!caps.SupportsLocking, "supports locking?");
            Debug.Assert(!caps.SupportsLongTransactions, "supports long transactions?");
            DataPropertyDefinitionCollection identities = definition.IdentityProperties;
            Debug.Assert(identities.Count == 1, "too many id properties");
            DataPropertyDefinition id = identities[0];
            Debug.Assert(id.Name == "FeatId", "id property wrong name");
            Debug.Assert(id.DataType == DataType.DataType_Int32, "id property wrong type");
            Debug.Assert(!reader.IsNull(id.Name), "id property is nul");
            // ToDo: check regular properties..
            GeometricPropertyDefinition geometry = ((FeatureClass)definition).GeometryProperty;
            Debug.Assert(geometry.Name == "Geometry", "geometry property wrong name");
            Debug.Assert(geometry.GeometryTypes == 0x04, "geometry property wrong types");
        }

        public void Test_requery()
        {
            try
            {
                ISelect select = (ISelect)base.ConnectionInitialized.CreateCommand(CommandType.CommandType_Select);
                select.SetFeatureClassName("lakes");
                IFeatureReader reader = select.Execute();
                int count1 = 0;
                while (reader.ReadNext())
                {
                    count1++;
                }
                reader = select.Execute();
                int count2 = 0;
                while (reader.ReadNext())
                {
                    count2++;
                }
                Debug.Assert(count1 == count2, "feature counts differ");
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

        public void Test_select_subset()
        {
            ISelect select = (ISelect)base.ConnectionInitialized.CreateCommand(CommandType.CommandType_Select);
            select.SetFeatureClassName("ontario");
            IdentifierCollection ids = select.PropertyNames;
            Identifier id = new Identifier("PERIMETER");
            ids.Add(id);
            IFeatureReader reader = select.Execute();
            reader.ReadNext();
            ClassDefinition definition = reader.GetClassDefinition();
            PropertyDefinitionCollection properties = definition.Properties;
            Debug.Assert(properties.Count == 1, "properties has wrong number of values");  // 2 == the selected property (PERIMITER) + the identity properties (FEATID; they never get filtered out)
            PropertyDefinition property = properties["PERIMETER"];
            Debug.Assert(property.PropertyType == PropertyType.PropertyType_DataProperty, "property is not a data property");
            Debug.Assert(property.Name == "PERIMETER", "property is wrong");
            double d = reader.GetDouble("PERIMETER");
            bool bad;
            try
            {
                d = reader.GetDouble("AREA");
                bad = true;
            }
            catch (OSGeo.Common.Exception)
            {
                bad = false;
            }
            Debug.Assert(!bad, "non.selected property is accessible");
        }

        public void Test_select_all()
        {
            double area;
            double length;
            int count = 0;

            ISelect select = (ISelect)base.ConnectionInitialized.CreateCommand(CommandType.CommandType_Select);
            select.SetFeatureClassName("ontario");
            IdentifierCollection ids = select.PropertyNames;
            IFeatureReader reader = select.Execute();
            while (reader.ReadNext())
            {
                reader.GetInt32("FeatId");
                area = reader.GetDouble("AREA");
                length = reader.GetDouble("PERIMETER");
                reader.GetDouble("ONTARIO_");
                reader.GetDouble("ONTARIO_ID");
                byte[] geometry = reader.GetGeometry("Geometry");

                ShpTests.AnalyzeGeometry("ontario", count++, geometry, length, area);
            }
        }

        public void Test_select_mixed_decimals()
        {
            try
            {
                double myfloat;
                double mydouble;
                double mysmallint;
                double mylargeint;
                double myobjectid;
                int count;

                ISelect select = (ISelect)base.ConnectionInitialized.CreateCommand(CommandType.CommandType_Select);
                select.SetFeatureClassName("ShpMixedDecimals");
                IFeatureReader reader = select.Execute();
                count = 0;
                while (reader.ReadNext())
                {
                    count++;
                    mylargeint = reader.GetDouble("MYLARGEINT");
                    myfloat = reader.GetDouble("MYFLOAT");
                    mydouble = reader.GetDouble("MYDOUBLE");
                    mysmallint = reader.GetDouble("MYSMALLINT");
                    myobjectid = reader.GetDouble("OBJECTID");
                    byte[] geometry = reader.GetGeometry("Geometry");
                }
                Debug.Assert(count != 0, "no features selected");
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

        public void Test_get_spatial_context_text()
        {
            IGetSpatialContexts get;
            ISpatialContextReader reader;
            IConnection mConnection = base.ConnectionInitialized;
            try
            {
                mConnection.Close();

                mConnection.ConnectionString = @"DefaultFileLocation=..\..\..\..\TestData\Oahu";

                Debug.Assert(mConnection.Open() == ConnectionState.ConnectionState_Open, "connection state not open");

                get = (IGetSpatialContexts)mConnection.CreateCommand(CommandType.CommandType_GetSpatialContexts);
                reader = get.Execute();
                string text = "PROJCS[\"NAD_1983_UTM_Zone_4N\",GEOGCS[\"GCS_North_American_1983\",DATUM[\"D_North_American_1983\",SPHEROID[\"GRS_1980\",6378137.0,298.257222101]],PRIMEM[\"Greenwich\",0.0],UNIT[\"Degree\",0.0174532925199433]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"False_Easting\",500000.0],PARAMETER[\"False_Northing\",0.0],PARAMETER[\"Central_Meridian\",-159.0],PARAMETER[\"Scale_Factor\",0.9996],PARAMETER[\"Latitude_Of_Origin\",0.0],UNIT[\"Meter\",1.0]]";
                while (reader.ReadNext())
                {
                    string name = reader.GetName();
                    if (name == "NAD_1983_UTM_Zone_4N")
                    {
                        string wkt = reader.GetCoordinateSystemWkt();
                        Debug.Assert(text.Length == wkt.Length, "wrong wkt text length");
                        if (wkt != text)
                        {
                            Debug.Fail("wrong wkt text");
                        }
                    }
                }
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

        public void Test_select_only_geometry()
        {
            int count;

            IConnection mConnection = base.ConnectionInitialized;
            try
            {
                mConnection.Close();
                mConnection.ConnectionString = @"DefaultFileLocation=..\..\..\..\TestData\Ontario";
                Debug.Assert(mConnection.Open() == ConnectionState.ConnectionState_Open, "connection state not open");

                ISelect select = (ISelect)mConnection.CreateCommand(CommandType.CommandType_Select);
                select.SetFeatureClassName("lakes");
                IdentifierCollection ids = select.PropertyNames;
                Identifier id = new Identifier("Geometry");
                ids.Add(id);
                IFeatureReader reader = select.Execute();
                ClassDefinition definition = reader.GetClassDefinition();
                PropertyDefinitionCollection properties = definition.Properties;
                count = properties.Count;
                Debug.Assert(count == 1, "wrong number of properties");
                PropertyDefinition property = properties[0];
                Debug.Assert(property.Name == "Geometry", "wrong property");
                count = 0;
                while (reader.ReadNext())
                {
                    count++;
                    byte[] geometry = reader.GetGeometry("Geometry");
                    try
                    {
                        reader.GetInt32("FeatId");
                        Debug.Fail("FeatId present");
                    }
                    catch (OSGeo.Common.Exception)
                    {
                    }
                }
                Debug.Assert(count != 0, "no lakes features selected");
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

        private static string GeometryTypeToString (GeometryType type)
        {
            string ret;

            switch (type)
            {
                case GeometryType.GeometryType_None:
                    ret = "GeometryType.GeometryType_None";
                    break;
                case GeometryType.GeometryType_Point:
                    ret = "GeometryType.GeometryType_Point";
                    break;
                case GeometryType.GeometryType_LineString:
                    ret = "GeometryType.GeometryType_LineString";
                    break;
                case GeometryType.GeometryType_Polygon:
                    ret = "GeometryType.GeometryType_Polygon";
                    break;
                case GeometryType.GeometryType_MultiPoint:
                    ret = "GeometryType.GeometryType_MultiPoint";
                    break;
                case GeometryType.GeometryType_MultiLineString:
                    ret = "GeometryType.GeometryType_MultiLineString";
                    break;
                case GeometryType.GeometryType_MultiPolygon:
                    ret = "GeometryType.GeometryType_MultiPolygon";
                    break;
                case GeometryType.GeometryType_MultiGeometry:
                    ret = "GeometryType.GeometryType_MultiGeometry";
                    break;
                case GeometryType.GeometryType_CurveString:
                    ret = "GeometryType.GeometryType_CurveString";
                    break;
                case GeometryType.GeometryType_CurvePolygon:
                    ret = "GeometryType.GeometryType_CurvePolygon";
                    break;
                case GeometryType.GeometryType_MultiCurveString:
                    ret = "GeometryType.GeometryType_MultiCurveString";
                    break;
                case GeometryType.GeometryType_MultiCurvePolygon:
                    ret = "GeometryType.GeometryType_MultiCurvePolygon";
                    break;
                default:
                    ret = type.ToString();
                    break;
            }

            return (ret);
        }

        public void Test_select_hawaii()
        {
            int count;

            try
            {
                IConnection mConnection = base.ConnectionInitialized;

                mConnection.Close();
                mConnection.ConnectionString = @"DefaultFileLocation=..\..\..\..\TestData\Hawaii";

                Debug.Assert(mConnection.Open() == ConnectionState.ConnectionState_Open, "connection state not open");

                ISelect select = (ISelect)mConnection.CreateCommand(CommandType.CommandType_Select);
                select.SetFeatureClassName("coast_n83");
                IFeatureReader reader = select.Execute();
                FgfGeometryFactory factory = new FgfGeometryFactory();
                count = 0;
                while (reader.ReadNext())
                {
                    count++;
                    int id = reader.GetInt32("FeatId");
                    byte[] bytes = reader.GetGeometry("Geometry");
                    Debug.Assert(bytes != null, "null bytes");
                    IGeometry geometry = factory.CreateGeometryFromFgf(bytes);
                    GeometryType type = geometry.DerivedType;
#if DEBUG
                    Console.WriteLine(id.ToString() + " " + GeometryTypeToString(type));
#endif
                    Debug.Assert(type == GeometryType.GeometryType_Polygon, "not a polygon");
                    IPolygon poly = (IPolygon)geometry;
                    ILinearRing ring = poly.ExteriorRing;
                    IEnvelope envelope = ring.Envelope;
#if DEBUG
                    {
                        Console.WriteLine(" [" + envelope.MinX.ToString() + "," + envelope.MinY.ToString() + " " + envelope.MaxX.ToString() + "," + envelope.MaxY.ToString() + "]");
                        Console.WriteLine(poly.InteriorRingCount.ToString());
                    }
#endif
                    for (int i = 0; i < poly.InteriorRingCount; i++)
                    {
                        ring = poly.GetInteriorRing(i);
                        envelope = ring.Envelope;
#if DEBUG
                        Console.WriteLine(" [" + envelope.MinX.ToString() + "," + envelope.MinY.ToString() + " " + envelope.MaxX.ToString() + "," + envelope.MaxY.ToString() + "]");
#endif
                    }
                }
                Debug.Assert(count != 0, "no coast_n83 features selected");
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

        public void Test_select_ontario ()
        {
            int count;

            try
            {
                IConnection mConnection = base.ConnectionInitialized;

                mConnection.Close ();
                mConnection.ConnectionString = @"DefaultFileLocation=..\..\..\..\TestData\Ontario";

                Debug.Assert (mConnection.Open() == ConnectionState.ConnectionState_Open, "connection state not open");

                ISelect select = (ISelect)mConnection.CreateCommand (CommandType.CommandType_Select);
                select.SetFeatureClassName("ontario");
                IFeatureReader reader = select.Execute ();
                FgfGeometryFactory factory = new FgfGeometryFactory();
                count = 0;
                while (reader.ReadNext ())
                {
                    count++;
                    int id = reader.GetInt32 ("FeatId");
                    byte [] bytes = reader.GetGeometry ("Geometry");
                    
                    Debug.Assert (bytes != null, "null bytes");
                }
                Debug.Assert (count != 0, "no ontario features selected");
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

        //**********************Runtime error, debug later*********************
        public void Test_select_date_time()
        {
            try
            {
                IConnection mConnection = base.ConnectionInitialized;

                mConnection.Close();
                mConnection.ConnectionString = @"DefaultFileLocation=..\..\..\..\TestData\Zones";

                Debug.Assert(mConnection.Open() == ConnectionState.ConnectionState_Open, "connection state not open");
                ISelect select = (ISelect)mConnection.CreateCommand(CommandType.CommandType_Select);
                select.SetFeatureClassName("ZONES");
                IFeatureReader reader = select.Execute();
                long count = 0;
                long nonnull = 0;
                while (reader.ReadNext())
                {
                    if (!reader.IsNull("REZONEDATE"))
                    {
                        // DateTime when = reader.GetDateTime("REZONEDATE"); !!!  THIS IS A BUG
                        nonnull++;
                    }
                    count++;
                }
                Debug.Assert(524 == count, "Wrong record count");
                Debug.Assert(25 == nonnull, "Wrong non.null record count");
                reader.Close();
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
