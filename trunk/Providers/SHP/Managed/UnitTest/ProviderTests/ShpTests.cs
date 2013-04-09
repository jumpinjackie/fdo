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
using OSGeo.FDO.Expression;
using OSGeo.FDO.Commands.Schema;
using OSGeo.FDO.Commands;
using OSGeo.FDO.Schema;
using OSGeo.FDO.Commands.Feature;
using OSGeo.FDO.Geometry;
using OSGeo.FDO.Spatial;
using OSGeo.FDO.Common;

namespace unit_test.ProviderTests
{
    class ShpTests
    {

        // Delete a class (if bDeleteRowsOnly==false) or delete the rows of a class (if bDeleteRowsOnly==true):
        public static void CleanUpClass(IConnection connection, string schema_name, string class_name, bool bDeleteRowsOnly)
        {
            try
            {
                Identifier identifier = new Identifier(class_name);
                IDescribeSchema descSchema = (IDescribeSchema)connection.CreateCommand(CommandType.CommandType_DescribeSchema);
                if (schema_name != null)
                {
                    descSchema.SchemaName = schema_name;
                }
                FeatureSchemaCollection schemas = descSchema.Execute();

                bool bFound = false;
                for (int iSchemaIndex = 0; iSchemaIndex < schemas.Count; iSchemaIndex++)
                {
                    FeatureSchema schema = schemas[iSchemaIndex];
                    if ((schema_name == null) || (schema.Name == schema_name))
                    {
                        ClassCollection classes = schema.Classes;
                        for (int iClassIndex = 0; iClassIndex < classes.Count; iClassIndex++)
                        {
                            ClassDefinition classDef = classes[iClassIndex];
                            if (classDef.Name == identifier.Name)
                            {
                                bFound = true;

                                if (bDeleteRowsOnly)
                                {
                                    // delete the rows of this class, but not the class itself:
                                    IDelete deleteCmd = (IDelete)connection.CreateCommand(CommandType.CommandType_Delete);
                                    deleteCmd.SetFeatureClassName(classDef.Name);
                                    deleteCmd.Execute();
                                }
                                else
                                {
                                    // delete the rows of this class, but not the class itself:
                                    IDelete deleteCmd = (IDelete)connection.CreateCommand(CommandType.CommandType_Delete);
                                    deleteCmd.SetFeatureClassName(classDef.Name);
                                    deleteCmd.Execute();
                                    
                                    // delete the entire class:
                                    classDef.Delete();
                                    IApplySchema applySchema = (IApplySchema)connection.CreateCommand(CommandType.CommandType_ApplySchema);
                                    applySchema.FeatureSchema = schema;
                                    applySchema.Execute();
                                }
                                break;
                            }
                        }
                    }
                    if (bFound)
                        break;
                }
                // recurse with default schema, if class not found:
                if (!bFound && (schema_name != null) && (schema_name.Length > 0))
                    CleanUpClass(connection, null, class_name);
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

        public static void Fail(OSGeo.FDO.Common.Exception e)
        {
            Fail(e.Message) ;
        }

        public static void Fail(string error)
        {
            Debug.Fail("[FAILURE]  " + error);
        }

        public static void CleanUpClass(IConnection connection, string schema_name, string class_name)
        {
            CleanUpClass(connection, schema_name, class_name, false);
        }

        public static Expression ParseByDataType(string data, DataType dataType)
        {
            Expression expr = null;
            bool bIsNull = false;

            // NOTE: blob parsing doesn't work yet (ever?) in FDO:
            if (dataType != DataType.DataType_BLOB)
            {
                expr = Expression.Parse(data);

                if (expr is BooleanValue)
                {
                    bIsNull = true;
                }
                else
                {
                    bIsNull = false;
                }
            }

            switch (dataType)
            {
                case DataType.DataType_Boolean:
                    {
                        if (bIsNull)
                        {
                            BooleanValue val = new BooleanValue();
                            val.SetNull();
                            expr = val;
                        }
                        else
                        {
                            BooleanValue value = (BooleanValue)(expr);
                            if (value == null)
                            {
                                Debug.Fail("Wrong data type!");
                            }
                        }
                    }
                    break;

                case DataType.DataType_Byte:
                    {
                        if (bIsNull)
                        {
                            ByteValue val = new ByteValue();
                            val.SetNull();
                            expr = val;
                        }
                        else
                        {
                            Int32Value value = (Int32Value)(expr);
                            if (value == null)
                            {
                                Debug.Fail("Wrong data type!");
                            }
                            expr = new ByteValue((byte)value.Int32);
                        }
                    }
                    break;

                case DataType.DataType_Int16:
                    {
                        if (bIsNull)
                        {
                            Int16Value val = new Int16Value();
                            val.SetNull();
                            expr = val;
                        }
                        else
                        {
                            Int32Value value = (Int32Value)(expr);
                            if (value == null)
                            {
                                Debug.Fail("Wrong data type!");
                            }
                            expr = new Int16Value((Int16)value.Int32);
                        }
                    }
                    break;

                case DataType.DataType_Int32:
                    {
                        if (bIsNull)
                        {
                            Int32Value val = new Int32Value();
                            val.SetNull();
                            expr = val;
                        }
                        else
                        {
                            Int32Value value = (Int32Value)(expr);
                            if (value == null)
                            {
                                Debug.Fail("Wrong data type!");
                            }
                        }
                    }
                    break;

                case DataType.DataType_Int64:
                    {
                        if (bIsNull)
                        {
                            Int64Value val = new Int64Value();
                            val.SetNull();
                            expr = val;
                        }
                        else
                        {
                            Int64Value value = (Int64Value)(expr);
                            if (value == null)
                            {
                                Debug.Fail("Wrong data type!");
                            }
                            expr = new Int64Value((Int64)value.Int64);
                        }
                    }
                    break;

                case DataType.DataType_Single:
                    {
                        if (bIsNull)
                        {
                            SingleValue val = new SingleValue();
                            val.SetNull();
                            expr = val;
                        }
                        else
                        {
                            DoubleValue value = (DoubleValue)(expr);
                            if (value == null)
                            {
                                Debug.Fail("Wrong data type!");
                            }
                            expr = new SingleValue((float)value.Double);
                        }
                    }
                    break;

                case DataType.DataType_Double:
                    {
                        if (bIsNull)
                        {
                            DoubleValue val = new DoubleValue();
                            val.SetNull();
                            expr = val;
                        }
                        else
                        {
                            DoubleValue value = (DoubleValue)(expr);
                            if (value == null)
                            {
                                Debug.Fail("Wrong data type!");
                            }
                        }
                    }
                    break;

                case DataType.DataType_DateTime:
                    {
                        if (bIsNull)
                        {
                            DateTimeValue val = new DateTimeValue();
                            val.SetNull();
                            expr = val;
                        }
                        else
                        {
                            DateTimeValue value = (DateTimeValue)expr;
                            if (value == null)
                            {
                                Debug.Fail("Wrong data type!");
                            }
                        }
                    }
                    break;

                case DataType.DataType_Decimal:
                    {
                        if (bIsNull)
                        {
                            DecimalValue val = new DecimalValue();
                            val.SetNull();
                            expr = val;
                        }
                        else
                        {
                            DoubleValue valueDouble = (DoubleValue)expr;
                            if (valueDouble != null)
                                expr = new DecimalValue((double)valueDouble.Double);
                            else
                            {
                                Int32Value valueInt32 = (Int32Value)expr;
                                if (valueInt32 != null)
                                    expr = new DecimalValue((double)valueInt32.Int32);
                                else
                                    Debug.Fail("Wrong data type!");
                            }
                        }
                    }
                    break;

                case DataType.DataType_String:
                    {
                        if (bIsNull)
                        {
                            StringValue val = new StringValue();
                            val.SetNull();
                            expr = val;
                        }
                        else
                        {
                            StringValue value = (StringValue)expr;
                            if (value == null)
                            {
                                Debug.Fail("Wrong data type!");
                            }
                        }
                    }
                    break;
                default:
                    Debug.Fail("Unhandled data type!");
                    break;
            }

            return expr;
        }

        public static void AnalyzeGeometry(string class_name, int index, byte[] geom_fgf, double length0, double area0)
        {
            FgfGeometryFactory gf = new FgfGeometryFactory();
            IGeometry geom = gf.CreateGeometryFromFgf(geom_fgf);

            switch (geom.DerivedType)
            {
                case GeometryType.GeometryType_LineString:
#if DEBUG
                    Console.WriteLine(class_name + " - " + index + ". FdoGeometryType_LineString!");
#endif
                    break;

                case GeometryType.GeometryType_Point:
#if DEBUG
                    Console.WriteLine(class_name + " - " + index + ". FdoGeometryType_Point!");
#endif
                    break;

                case GeometryType.GeometryType_Polygon:
                    {
                        IPolygon poly = (IPolygon)geom;
                        ILinearRing extRing = poly.ExteriorRing;
                        double area = SpatialUtility.ComputeLinearRingArea(extRing);
                        double length = SpatialUtility.ComputeLinearRingLength(extRing);
#if DEBUG
                        Console.WriteLine(class_name + " - " + index + ". FdoGeometryType_Polygon  length=" + length.ToString() + " (" + length0.ToString() + ") area=" + area.ToString() + " (" + area0.ToString() + ")");
#endif
                        for (int j = 0; j < poly.InteriorRingCount; j++)
                        {
                            ILinearRing intRing = poly.GetInteriorRing(j);
                            area = SpatialUtility.ComputeLinearRingArea(intRing);
#if DEBUG
                            Console.WriteLine(j.ToString() + "    . intPolygon  area=" + area.ToString());
#endif
                        }
                    }
                    break;

                case GeometryType.GeometryType_MultiPoint:
#if DEBUG
                    Console.WriteLine(class_name + " - " + index + ". FdoGeometryType_MultiPoint!");
#endif
                    break;

                case GeometryType.GeometryType_MultiGeometry:
#if DEBUG
                    Console.WriteLine(class_name + " - " + index + ". FdoGeometryType_MultiGeometry!");
#endif
                    break;

                case GeometryType.GeometryType_MultiLineString:
#if DEBUG
                    Console.WriteLine(class_name + " - " + index + ". FdoGeometryType_MultiLineString!");
#endif
                    break;

                case GeometryType.GeometryType_MultiPolygon:
                    {
#if DEBUG
                        Console.WriteLine(class_name + " - " + index + ". FdoGeometryType_MultiPolygon (" + area0.ToString() + ")", area0);
#endif

                        IMultiPolygon mpoly = (IMultiPolygon)geom;
                        for (int i = 0; i < mpoly.Count; i++)
                        {
                            IPolygon poly = mpoly.get_Item(i);
                            ILinearRing extRing = poly.ExteriorRing;
                            double area = SpatialUtility.ComputeLinearRingArea(extRing);
                            double length = SpatialUtility.ComputeLinearRingLength(extRing);
#if DEBUG
                            Console.WriteLine(i.ToString() + ". extPolygon length=" + length.ToString() + " (" + length0.ToString() + ") area=" + area.ToString() + " (" + area0.ToString() + ")");
#endif

                            for (int j = 0; j < poly.InteriorRingCount; j++)
                            {
                                ILinearRing intRing = poly.GetInteriorRing(j);
                                area = SpatialUtility.ComputeLinearRingArea(intRing);
#if DEBUG
                                Console.WriteLine(j.ToString() + ". intPolygon  area=" + area.ToString());
#endif
                            }
                        }
                    }
                    break;

                case GeometryType.GeometryType_CurveString:
#if DEBUG
                    Console.WriteLine(class_name + " - " + index + ". FdoGeometryType_CurveString!");
#endif
                    break;

                case GeometryType.GeometryType_MultiCurveString:
#if DEBUG
                    Console.WriteLine(class_name + " - " + index + ". FdoGeometryType_MultiCurveString!");
#endif
                    break;

                case GeometryType.GeometryType_CurvePolygon:
#if DEBUG
                    Console.WriteLine(class_name + " - " + index + ". FdoGeometryType_CurvePolygon!");
#endif
                    break;

                case GeometryType.GeometryType_MultiCurvePolygon:
#if DEBUG
                    Console.WriteLine(class_name + " - " + index + ". FdoGeometryType_MultiCurvePolygon!");
#endif
                    break;
            }
        }
    }
}
