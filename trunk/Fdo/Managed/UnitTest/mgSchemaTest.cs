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
using System.Diagnostics;
using OSGeo.FDO;
using OSGeo.FDO.Schema;
using OSGeo.FDO.Expression;
using OSGeo.FDO.Commands;
using OSGeo.FDO.Raster;
using OSGeo.FDO.Xml;
using OSGeo.Common.Io;
using OSGeo.Common.Xml;
using OSGeo.Common.Gml212;
using OSGeo.FDO.Commands.Schema;

namespace Fdo_Test
{
    /// <summary>
    /// Summary description for mgSchemaTest.
    /// </summary>
    public class mgSchemaTest
    {
        public mgSchemaTest()
        {
            Console.WriteLine("start mgSchemaTest");
        }

        private void testSchemaElement(SchemaElement pElement, string pszName, string pszDesc)
        {
            // check constructor created things OK
            Debug.Assert(pElement.Name == pszName);
            Debug.Assert(pElement.Description == pszDesc);

            // check setting to null
            pElement.Name = string.Empty; //SetName(null);
            pElement.Description = string.Empty; //SetDescription(null);
            Debug.Assert(pElement.Name == string.Empty);
            Debug.Assert(pElement.Description == string.Empty);

            // check setters and getters
            pElement.Name = pszName;
            pElement.Description = pszDesc;
            Debug.Assert(pElement.Name == pszName);
            Debug.Assert(pElement.Description == pszDesc);


            // check Schema Attribute Dictionary
            SchemaAttributeDictionary pDict = pElement.Attributes;
            Debug.Assert(pDict.Count == 0);

            pDict.Add("Test", "test");
            Debug.Assert(pDict.Count == 1);
            Debug.Assert(pDict.GetAttributeValue("Test") == "test");

            try
            {
                pDict.Add("Test", "this");
                Debug.Assert(false);         // should have failed and throw exception
            }
            catch (OSGeo.Common.Exception)
            {
            }
            pDict.Add("test", "this");     // case sensitive, so should succeed
            Debug.Assert(pDict.Count == 2);

            try
            {
                pDict.SetAttributeValue("NoMatch", "other");
                Debug.Assert(false);         // should have failed and throw exception
            }
            catch (OSGeo.Common.Exception)
            {
            }

            pDict.SetAttributeValue("Test", "me");
            pDict.SetAttributeValue("Test", "other");
            Debug.Assert(pDict.Count == 2);
            Debug.Assert(pDict.GetAttributeValue("Test") == "other");

            pDict.Add("3", null);
            pDict.Add("4", "");
            Debug.Assert(pDict.GetAttributeValue("3") == null);

            // null value and null string aren't the same.
            Debug.Assert(pDict.GetAttributeValue("4") != null);
            Debug.Assert(pDict.GetAttributeValue("4") == "");
            pDict.SetAttributeValue("4", null);
            Debug.Assert(pDict.GetAttributeValue("4") == null);

            pDict.Add("5", "5lab");
            pDict.Add("6", "b6ab");
            pDict.Add("7", "bl7b");
            pDict.Add("8", "bla8");
            pDict.Add("9", "9lab");
            pDict.Add("10", "b10ab");
            Debug.Assert(pDict.Count == 10);

            Debug.Assert(pDict.Count == 10);
            pDict.Remove("4");
            Debug.Assert(pDict.Count == 9);

            try
            {
                pDict.Remove("4");
                Debug.Assert(false);         // should have failed and throw exception
            }
            catch (OSGeo.Common.Exception)
            {
            }

            try
            {
                pDict.Remove("b10ab");
                Debug.Assert(false);         // should have failed and throw exception
            }
            catch (OSGeo.Common.Exception)
            {
            }

            Int32 cLength = pDict.AttributeNames.Length;
            string[] aNames = pDict.AttributeNames;
            Debug.Assert(cLength == 9);
            Debug.Assert(aNames[0] == "Test");
            Debug.Assert(aNames[1] == "test");
            Debug.Assert(aNames[2] == "3");
            Debug.Assert(aNames[3] == "5");
            Debug.Assert(aNames[4] == "6");
            Debug.Assert(aNames[5] == "7");
            Debug.Assert(aNames[6] == "8");
            Debug.Assert(aNames[7] == "9");
            Debug.Assert(aNames[8] == "10");
        }

        private void testClassDefinition(ClassDefinition pClassDef, string pszName, string pszDesc)
        {
            testSchemaElement(pClassDef, pszName, pszDesc);

            pClassDef.IsAbstract = true;
            Debug.Assert(pClassDef.IsAbstract);

            pClassDef.IsAbstract = false;
            Debug.Assert(!pClassDef.IsAbstract);
        }

        public void testPropertyValue()
        {
            Console.WriteLine("start mgSchemaTest.testPropertyValue");

            // create some data for the test
            string pszIdent = "Prop.Value.1";
            string pszDVal = "DataValue1";

            DataValue pDV = new StringValue(pszDVal);
            Identifier pIdent = new Identifier(pszIdent);

            PropertyValue pPropVal = new PropertyValue(pIdent, pDV);

            // check constructor created things OK
            Identifier pIdentTest = pPropVal.Name;
            Debug.Assert(pIdentTest == pIdent);

            ValueExpression pVETest = pPropVal.Value;
            Debug.Assert(pVETest == pDV);

            // test getting and setting nulls
            pPropVal.Name = (Identifier)null;
            pPropVal.Value = (DataValue)null;
            Debug.Assert(pPropVal.Name == null);
            Debug.Assert(pPropVal.Value == null);

            // test setters and getters
            pPropVal.SetName(pszIdent);

            StringValue pszDValExpression = new StringValue(pszDVal);
            pPropVal.Value = pszDValExpression;
            pIdentTest = pPropVal.Name;
            Debug.Assert(pIdentTest.Text == pszIdent);

            StringValue pDVTest = (StringValue)(pPropVal.Value);
            Debug.Assert(pDVTest.String == pszDVal);

            //Console.WriteLine("finish mgSchemaTest.testPropertyValue");
            
        }


        public void testMultiSchema()
        {
            Console.WriteLine("start mgSchemaTest.testMultiSchema");

            FeatureSchemaCollection pSchemas = new FeatureSchemaCollection(null);
            FeatureSchema pFSchema1 = new FeatureSchema("SchemaOne", "Schema One Desc");
            ClassCollection pClasses1 = pFSchema1.Classes;
            pSchemas.Add(pFSchema1);

            FeatureSchema pFSchema2 = new FeatureSchema("SchemaTwo", "Schema two Desc");
            ClassCollection pClasses2 = pFSchema2.Classes;
            pSchemas.Add(pFSchema2);

            Class pclass1 = new Class("Class1", "Class1 Desc");
            Class pclass2 = new Class("Class2", "Class2 Desc");
            pClasses1.Add(pclass1);
            pClasses1.Add(pclass2);

            Class pclass3 = new Class("Class3", "Class3 Desc");
            Class pclass4 = new Class("Class1", "Class1 Desc");
            pClasses2.Add(pclass3);
            pClasses2.Add(pclass4);

            IDisposableCollection result = pSchemas.FindClass("Class1");
            Debug.Assert(result.Count == 2);

            result = pSchemas.FindClass("SchemaTwo:Class1");
            Debug.Assert(result.Count == 1);
            result = pSchemas.FindClass("SchemaOne:Bogus");
            Debug.Assert(result.Count == 0);
            result = pSchemas.FindClass("Bogus");
            Debug.Assert(result.Count == 0);

            //Console.WriteLine("finish mgSchemaTest.testMultiSchema");
            
        }

        public void testFeatureSchema()
        {
            Console.WriteLine("start mgSchemaTest.testFeatureSchema");

            string pName = "FSClassName";
            string pDesc = "FeatureSchema Description";
            FeatureSchema pFSchema = new FeatureSchema(pName, pDesc);
            ClassCollection pClasses = pFSchema.Classes;
            SchemaAttributeDictionary pattrib;
            PropertyDefinitionCollection pfeatureclassprops;
            DataPropertyDefinitionCollection pfeatureclassidprops;

            // check base class functionality
            testSchemaElement(pFSchema, pName, pDesc);

            // Build a small schema with one of each element
            //
            //
            // FeatureSchema
            //  + Class (Class,Class Desc)
            //      + Attrib (Attribute1, Value1)
            //      + Attrib (Attribute2, Value2)
            //  
            //  + FeatureClass (Feature Class, Feature Class Desc)   
            //      + Attrib (FC Attribute1, FC Value1)
            //      + Attrib (FC Attribute2, FC Value2)
            //      Props = [pgeomprop, pobjprop]
            //      BaseProps = [pgeomprop]
            //      GeometryProp = pgeomprop
            // 
            // 
            //  pgeomprop = GeometricPropertyDefinition (Gemo Prop, Geom Prop Desc)
            //      GeometryType = Curve
            //      Readonly = True
            //      HasMeasure = True
            //      + Attrib (Geom Prop Attrib1, Value1)
            // 
            //  pdataprop = DataPropertyDefinition (Data Prop, Data Prop Desc)
            //      DataType = Boolean
            //      DefaultValue = 1
            //      Nullable = false
            //      Precision = 2
            //      ReadOnly = false
            //      Scale = 2
            //
            //  pobjprop = ObjectPropertyDefinition (Object Prop, Object Prop Desc)
            //      Class = Class (Class, Class Desc)
            //      identityProperty = pdataprop

            Class pclass = new Class("Class", "Class Desc");
            pattrib = pclass.Attributes;
            pClasses.Add(pclass);
            pattrib.Add("Attribute1", "Value1");
            pattrib.Add("Attribute2", "Value2");

            GeometricPropertyDefinition pgeomprop = new GeometricPropertyDefinition("Geom Prop", "Geom Prop Desc");
            pgeomprop.GeometryTypes = 0x02;
            pgeomprop.ReadOnly = true;
            pgeomprop.HasMeasure = true;
            pgeomprop.HasElevation = true;
            pattrib = pgeomprop.Attributes;
            pattrib.Add("Geom Prop Attr1", "Value1");

            DataPropertyDefinition pdataprop = new DataPropertyDefinition("Data Prop", "Data Prop Desc");
            pdataprop.DataType = DataType.DataType_Boolean;
            pdataprop.DefaultValue = "1";
            pdataprop.Nullable = false;

            // test incorrect precision:
            bool bCaughtPrecisionException = false;
            try
            {
                pdataprop.Precision = -1;
            }
            catch (OSGeo.Common.Exception)
            {
                bCaughtPrecisionException = true;
                pdataprop.Precision = 10;
            }
            Debug.Assert(bCaughtPrecisionException);

            // test incorrect length:
            bool bCaughtLengthException = false;
            try
            {
                pdataprop.Length = -1;
            }
            catch (OSGeo.Common.Exception)
            {
                bCaughtLengthException = true;
                pdataprop.Length = 20;
            }
            Debug.Assert(bCaughtLengthException);

            Boolean bFailed = false;
            try
            {
                FeatureClass pFClass = new FeatureClass("bad:name", pDesc);
            }
            catch (OSGeo.Common.Exception e)
            {
                Debug.Assert(e.Message.Substring(e.Message.LastIndexOf(") ") + 1) == " Invalid Feature schema element name 'bad:name'; must not contain ':'. ");
                bFailed = true;
            }
            Debug.Assert(bFailed, "Creating 'bad.name' feature class should have failed.");

            pdataprop.Precision = 2;
            pdataprop.ReadOnly = false;
            pdataprop.Scale = 2;

            DataPropertyDefinition pAutogenProp = new DataPropertyDefinition("AutoGen Prop", "AutoGen Prop Desc");
            pAutogenProp.DataType = DataType.DataType_Int64;
            pAutogenProp.Nullable = false;
            pAutogenProp.ReadOnly = false;
            pAutogenProp.IsAutoGenerated = true;

            // Turning on autogeneration causes readonly to flip to true.
            Debug.Assert(pAutogenProp.ReadOnly);

            // test making autogenerated property writable, must fail. 
            bool bCaughtAutoGenException = false;
            try
            {
                pAutogenProp.ReadOnly = false;
            }
            catch (OSGeo.Common.Exception)
            {
                bCaughtAutoGenException = true;
            }
            Debug.Assert(bCaughtAutoGenException);

            // Test setting it back to non-autogenerated
            pAutogenProp.IsAutoGenerated = false;
            // Readonly should not change this time.
            Debug.Assert(pAutogenProp.ReadOnly);
            pAutogenProp.ReadOnly = false;
            Debug.Assert(!pAutogenProp.ReadOnly);

            ObjectPropertyDefinition pobjprop = new ObjectPropertyDefinition("Object Prop", "Object Prop Desc");
            pobjprop.Class = pclass;
            pobjprop.IdentityProperty = pdataprop;
            pobjprop.ObjectType = ObjectType.ObjectType_Value;

            FeatureClass pfeatureclass = new FeatureClass("FeatureClass", "FeatureClass Desc");
            pattrib = pfeatureclass.Attributes;
            pClasses.Add(pfeatureclass);
            pattrib.Add("FC Attribute1", "FC Value1");
            pattrib.Add("FC Attribute2", "FC Value2");
            pfeatureclassprops = pfeatureclass.Properties;
            pfeatureclassidprops = pfeatureclass.IdentityProperties;
            pfeatureclassprops.Add(pgeomprop);
            pfeatureclass.GeometryProperty = pgeomprop;
            pfeatureclassprops.Add(pobjprop);

            Debug.Assert(pfeatureclass.Capabilities == null);

            try
            {
                pfeatureclassidprops.Add(pdataprop);   // should fail, not member of Properties yet
                Debug.Assert(false);
            }
            catch (OSGeo.Common.Exception)
            {
            }

            pfeatureclassprops.Add(pdataprop);
            pfeatureclassidprops.Add(pdataprop);

            PropertyDefinitionCollection baseProps = new PropertyDefinitionCollection(null);
            baseProps.Add(pgeomprop);
            pfeatureclass.SetBaseProperties(baseProps);

            // Test modifying/accepting/changing schema
            Debug.Assert(pclass.ElementState == SchemaElementState.SchemaElementState_Added);
            Debug.Assert(pfeatureclass.ElementState == SchemaElementState.SchemaElementState_Added);
            Debug.Assert(pgeomprop.ElementState == SchemaElementState.SchemaElementState_Added);

            // initially reject all changes
            pFSchema.RejectChanges();
            Debug.Assert(pFSchema.ElementState == SchemaElementState.SchemaElementState_Unchanged);

            // now redo and accept all changes        
            pClasses.Add(pclass);
            Debug.Assert(pFSchema.ElementState == SchemaElementState.SchemaElementState_Modified);
            pattrib = pclass.Attributes;
            pattrib.Add("Attribute1", "Value1");
            pattrib.Add("Attribute2", "Value2");

            pgeomprop.GeometryTypes = 0x02;
            pgeomprop.ReadOnly = true;
            pgeomprop.HasMeasure = true;
            pgeomprop.HasElevation = true;
            pattrib = pgeomprop.Attributes;
            pattrib.Add("Geom Prop Attr1", "Value1");

            pdataprop.DataType = DataType.DataType_Boolean;
            pdataprop.DefaultValue = "1";
            pdataprop.Nullable = false;
            pdataprop.Precision = 2;
            pdataprop.ReadOnly = false;
            pdataprop.Scale = 2;

            pobjprop.Class = pclass;
            pobjprop.IdentityProperty = pdataprop;
            pobjprop.ObjectType = ObjectType.ObjectType_Value;

            pattrib = pfeatureclass.Attributes;
            pClasses.Add(pfeatureclass);
            pattrib.Add("FC Attribute1", "FC Value1");
            pattrib.Add("FC Attribute2", "FC Value2");
            pfeatureclassprops = pfeatureclass.Properties;
            pfeatureclassidprops = pfeatureclass.IdentityProperties;
            pfeatureclassprops.Add(pgeomprop);
            pfeatureclass.GeometryProperty = pgeomprop;
            pfeatureclassprops.Add(pobjprop);

            pfeatureclassprops.Add(pdataprop);
            pfeatureclassidprops.Add(pdataprop);

            baseProps = new PropertyDefinitionCollection(null);
            baseProps.Add(pgeomprop);
            pfeatureclass.SetBaseProperties(baseProps);
            pFSchema.AcceptChanges();
            Debug.Assert(pFSchema.ElementState == SchemaElementState.SchemaElementState_Unchanged);
            Debug.Assert(pclass.ElementState == SchemaElementState.SchemaElementState_Unchanged);
            Debug.Assert(pfeatureclass.ElementState == SchemaElementState.SchemaElementState_Unchanged);
            Debug.Assert(pgeomprop.ElementState == SchemaElementState.SchemaElementState_Unchanged);
            Debug.Assert(pobjprop.ElementState == SchemaElementState.SchemaElementState_Unchanged);
            Debug.Assert(pdataprop.ElementState == SchemaElementState.SchemaElementState_Unchanged);

            // Test changing and rejecting change to a class
            pclass.Name = "New Name";
            Debug.Assert(pFSchema.ElementState == SchemaElementState.SchemaElementState_Modified);
            pclass.Description = "New Description";

            pFSchema.RejectChanges();
            Debug.Assert(pFSchema.ElementState == SchemaElementState.SchemaElementState_Unchanged);
            Debug.Assert(pclass.ElementState == SchemaElementState.SchemaElementState_Unchanged);
            Debug.Assert(pfeatureclass.ElementState == SchemaElementState.SchemaElementState_Unchanged);
            Debug.Assert(pgeomprop.ElementState == SchemaElementState.SchemaElementState_Unchanged);
            Debug.Assert(pobjprop.ElementState == SchemaElementState.SchemaElementState_Unchanged);
            Debug.Assert(pdataprop.ElementState == SchemaElementState.SchemaElementState_Unchanged);

            // Test deleting class from a collection via class.Delete()
            Debug.Assert(pClasses.Count == 2);
            pclass.Delete();
            Debug.Assert(pFSchema.ElementState == SchemaElementState.SchemaElementState_Modified);
            Debug.Assert(pClasses.Count == 2);
            pFSchema.AcceptChanges();
            Debug.Assert(pClasses.Count == 1);
            Debug.Assert(pFSchema.ElementState == SchemaElementState.SchemaElementState_Unchanged);
            Debug.Assert(pclass.ElementState == SchemaElementState.SchemaElementState_Unchanged);
            Debug.Assert(pfeatureclass.ElementState == SchemaElementState.SchemaElementState_Unchanged);
            Debug.Assert(pgeomprop.ElementState == SchemaElementState.SchemaElementState_Unchanged);
            Debug.Assert(pobjprop.ElementState == SchemaElementState.SchemaElementState_Unchanged);
            Debug.Assert(pdataprop.ElementState == SchemaElementState.SchemaElementState_Unchanged);

            // Test rejecting simple Description change in a property
            pgeomprop.Description = "Something else";
            Debug.Assert(pgeomprop.Description == "Something else");
            Debug.Assert(pfeatureclass.ElementState == SchemaElementState.SchemaElementState_Modified);
            Debug.Assert(pFSchema.ElementState == SchemaElementState.SchemaElementState_Modified);
            pFSchema.RejectChanges();
            Debug.Assert(pgeomprop.Description == "Geom Prop Desc");
            Debug.Assert(pFSchema.ElementState == SchemaElementState.SchemaElementState_Unchanged);
            Debug.Assert(pclass.ElementState == SchemaElementState.SchemaElementState_Unchanged);
            Debug.Assert(pfeatureclass.ElementState == SchemaElementState.SchemaElementState_Unchanged);
            Debug.Assert(pgeomprop.ElementState == SchemaElementState.SchemaElementState_Unchanged);
            Debug.Assert(pobjprop.ElementState == SchemaElementState.SchemaElementState_Unchanged);
            Debug.Assert(pdataprop.ElementState == SchemaElementState.SchemaElementState_Unchanged);

            // Test removing property (which should also remove from idProperties)
            Debug.Assert(pfeatureclassidprops.Count == 1);
            Debug.Assert(pfeatureclassprops.Count == 3);
            pfeatureclassprops.Remove(pdataprop);
            Debug.Assert(pfeatureclassprops.Count == 2);
            Debug.Assert(pfeatureclassidprops.Count == 0);    // removing property should also remove from identity properties
            pFSchema.RejectChanges();
            Debug.Assert(pfeatureclassprops.Count == 3);
            Debug.Assert(pfeatureclassidprops.Count == 1);
            Debug.Assert(pFSchema.ElementState == SchemaElementState.SchemaElementState_Unchanged);
            Debug.Assert(pclass.ElementState == SchemaElementState.SchemaElementState_Unchanged);
            Debug.Assert(pfeatureclass.ElementState == SchemaElementState.SchemaElementState_Unchanged);
            Debug.Assert(pgeomprop.ElementState == SchemaElementState.SchemaElementState_Unchanged);
            Debug.Assert(pobjprop.ElementState == SchemaElementState.SchemaElementState_Unchanged);
            Debug.Assert(pdataprop.ElementState == SchemaElementState.SchemaElementState_Unchanged);

            Debug.Assert(pfeatureclass.GetBaseProperties().Count == 1);
            Debug.Assert((SchemaElement)((pfeatureclass.GetBaseProperties())[0]) == pgeomprop);
            Debug.Assert((SchemaElement)((pfeatureclass.GetBaseProperties())[pgeomprop.Name]) == pgeomprop);
            Debug.Assert(pfeatureclass.GetBaseProperties().IndexOf(pgeomprop) == 0);
            Debug.Assert(pfeatureclass.GetBaseProperties().Contains(pgeomprop));
            Debug.Assert(!pfeatureclass.GetBaseProperties().Contains(pobjprop));

            // Test inserting a property -- make sure ElementState is propogated up properly
            Debug.Assert(pFSchema.ElementState == SchemaElementState.SchemaElementState_Unchanged);
            DataPropertyDefinition pdataprop2 = new DataPropertyDefinition("Data Prop 2", "Data Prop 2 Desc");
            pdataprop2.DataType = DataType.DataType_Boolean;
            pdataprop2.DefaultValue = "1";
            pdataprop2.Nullable = false;
            pdataprop2.Precision = 2;
            pdataprop2.ReadOnly = false;
            pdataprop2.Scale = 2;
            pfeatureclassprops.Add(pdataprop2);
            Debug.Assert(pFSchema.ElementState == SchemaElementState.SchemaElementState_Modified);
            pFSchema.AcceptChanges();
            Debug.Assert(pFSchema.ElementState == SchemaElementState.SchemaElementState_Unchanged);
            Debug.Assert(pclass.ElementState == SchemaElementState.SchemaElementState_Unchanged);
            Debug.Assert(pfeatureclass.ElementState == SchemaElementState.SchemaElementState_Unchanged);
            Debug.Assert(pgeomprop.ElementState == SchemaElementState.SchemaElementState_Unchanged);
            Debug.Assert(pobjprop.ElementState == SchemaElementState.SchemaElementState_Unchanged);
            Debug.Assert(pdataprop.ElementState == SchemaElementState.SchemaElementState_Unchanged);

            // try inserting a Class -- make sure ElementState is propogated up properly
            Class pclass2 = new Class("Class 2", "Class 2 Desc");
            pClasses.Add(pclass2);
            Debug.Assert(pclass2.ElementState == SchemaElementState.SchemaElementState_Added);
            Debug.Assert(pFSchema.ElementState == SchemaElementState.SchemaElementState_Modified);
            pFSchema.AcceptChanges();

            // Test reject change for feature class
            GeometricPropertyDefinition pgeomprop2 = new GeometricPropertyDefinition("Geom Prop", "Geom Prop Desc");
            pfeatureclass.GeometryProperty = pgeomprop2;
            pFSchema.RejectChanges();

            // Try to create circular base-class references (infinite loop)
            pFSchema.AcceptChanges();

            // NOTE: This is semi-circular because pfeatureclass contains pobjprop, and pobjprop refers to pclass
            FeatureClass pfeatureclass2 = new FeatureClass("FeatureClass2", "FeatureClass2 Desc");
            pfeatureclass2.BaseClass = pfeatureclass;
            try
            {
                pfeatureclass.BaseClass = pfeatureclass2;
                Debug.Assert(false);  // should never reach this, an exception should be thrown because pclass would become its own grandparent
            }
            catch (OSGeo.Common.Exception)
            {
            }
            pFSchema.AcceptChanges();

            // TODO: If we leave the semi-circular reference, some memory doesn't get released
            //       when we release the pFSchema.  Do we need a way to break this reference count deadlock?
            pfeatureclass2.BaseClass = null;

            FeatureClass pFeatClass3 = new FeatureClass("FeatureClass3", "");
            pFeatClass3.IsAbstract = false;

            DataPropertyDefinition pPartNum = new DataPropertyDefinition("PartNum", "");
            pPartNum.DataType = DataType.DataType_Int16;
            pPartNum.Nullable = true;
            pFeatClass3.Properties.Add(pPartNum);

            DataValue minValue = new DataValue(-20000);
            DataValue maxValue = new DataValue(20000);
            PropertyValueConstraintRange pRange = new PropertyValueConstraintRange(minValue, maxValue);
            pPartNum.ValueConstraint = pRange;

            DataPropertyDefinition pProp = new DataPropertyDefinition("Volume", "");
            pProp.DataType = DataType.DataType_Single;
            pProp.Nullable = false;
            pProp.ValueConstraint = null;
            pFeatClass3.Properties.Add(pProp);

            Debug.Assert(((PropertyValueConstraintRange)pPartNum.ValueConstraint).MinValue.ToString() == "-20000");
            Debug.Assert(((PropertyValueConstraintRange)pPartNum.ValueConstraint).MaxValue.ToString() == "20000");
            Debug.Assert(pProp.ValueConstraint == null);


            pFSchema.AcceptChanges();

            // Test Enumerator
            System.Collections.IEnumerator pEnum = pfeatureclassprops.GetEnumerator();
            while (pEnum.MoveNext())
            {
                PropertyDefinition primaryProp = (PropertyDefinition)(pEnum.Current);
            }

            // Force smart pointers to release
            pfeatureclassprops = null;
            pfeatureclassidprops = null;
            pattrib = null;
                     
            bFailed = false;
            try
            {
                pFSchema = new FeatureSchema("bad.name", pDesc);
            }
            catch (OSGeo.Common.Exception e)
            {
                Debug.Assert(e.Message.Substring(e.Message.LastIndexOf(") ") + 1) == " Invalid Feature schema element name 'bad.name'; must not contain '.'. ");
                bFailed = true;
            }
            Debug.Assert(bFailed, "Creating 'bad.name' feature schema should have failed.");

            bFailed = false;
            try
            {
                pdataprop = new DataPropertyDefinition("bad.name", pDesc);
            }
            catch (OSGeo.Common.Exception e)
            {
                Debug.Assert(e.Message.Substring(e.Message.LastIndexOf(") ") + 1) == " Invalid Feature schema element name 'bad.name'; must not contain '.'. ");
                bFailed = true;
            }
            Debug.Assert(bFailed, "Creating 'bad.name' property should have failed.");

            //Console.WriteLine("finish mgSchemaTest.testFeatureSchema");
            
        }

        public void testClass()
        {
            Console.WriteLine("start mgSchemaTest.testClass");

            string pName = "ClassName";
            string pDesc = "Class Description";

            Class pClass = new Class(pName, pDesc);

            // check constructor created things OK
            Debug.Assert(pClass.ClassType == ClassType.ClassType_Class);

            // check base class functionality
            testClassDefinition(pClass, pName, pDesc);

            //Console.WriteLine("finish mgSchemaTest.testClass");
            
        }

        public void testFeatureClass()
        {
            Console.WriteLine("start mgSchemaTest.testFeatureClass");

            string pName = "FClassName";
            string pDesc = "FeatureClass Description";

            FeatureClass pFClass = new FeatureClass(pName, pDesc);

            // check constructor created things OK
            Debug.Assert(pFClass.ClassType == ClassType.ClassType_FeatureClass);

            Boolean bFailed = false;
            try
            {
                pFClass = new FeatureClass("bad:name", pDesc);
            }
            catch (OSGeo.Common.Exception e)
            {
                //  string pMessage = wcschr( e.GetExceptionMessage(), ')' ) + 1;
                Debug.Assert(e.Message.Substring(e.Message.LastIndexOf(") ") + 1) == " Invalid Feature schema element name 'bad:name'; must not contain ':'. ");
                bFailed = true;
            }
            Debug.Assert(bFailed, "Creating 'bad:name' feature class should have failed.");

            // check base class functionality
            testClassDefinition(pFClass, pName, pDesc);

            // TODO: test Get/SetGeometryProperty()

            bFailed = false;
            try
            {
                pFClass = new FeatureClass("bad.name", pDesc);
            }
            catch (OSGeo.Common.Exception e)
            {
                //Todo: get this working for Linux (likely need to set up message catalogue
                Debug.Assert(e.Message.Substring(e.Message.LastIndexOf(") ") + 1) == " Invalid Feature schema element name 'bad.name'; must not contain '.'. ");
                bFailed = true;
            }
            Debug.Assert(bFailed, "Creating 'bad.name' feature class should have failed.");

            //Console.WriteLine("finish mgSchemaTest.testFeatureClass");
            
        }

        public void testSubClass()
        {
            Console.WriteLine("start mgSchemaTest.testSubClass");

            FeatureSchema pSchema = new FeatureSchema("TestSchema", "");

            // A non-abstract base class
            FeatureClass pBaseClass = new FeatureClass("Base Class", "");
            pBaseClass.IsAbstract = false;

            DataPropertyDefinition pProp = new DataPropertyDefinition("FeatId", "");
            pProp.DataType = DataType.DataType_Int64;
            pProp.Nullable = false;
            pBaseClass.Properties.Add(pProp);
            pBaseClass.IdentityProperties.Add(pProp);

            pSchema.Classes.Add(pBaseClass);

            // Try to define an identity property on a subclass

            FeatureClass pClass = new FeatureClass("ClassA", "");
            pClass.IsAbstract = false;
            pClass.BaseClass = pBaseClass;

            pProp = new DataPropertyDefinition("Id", "");
            pProp.DataType = DataType.DataType_Int64;
            pProp.Nullable = false;
            pClass.Properties.Add(pProp);
            try
            {
                pClass.IdentityProperties.Add(pProp);
                Debug.Assert(false);  // should never reach this, an exception should be thrown because tried to add an id property to class with base class
            }
            catch (OSGeo.Common.Exception)
            {
            }

            // Try to set the base class for a class with identity properties        
            pClass = new FeatureClass("ClassB", "");
            pClass.IsAbstract = false;

            pProp = new DataPropertyDefinition("Id", "");
            pProp.DataType = DataType.DataType_Int64;
            pProp.Nullable = false;
            pClass.Properties.Add(pProp);
            pClass.IdentityProperties.Add(pProp);
            try
            {
                pClass.BaseClass = pBaseClass;
                Debug.Assert(false);  // should never reach this, an exception should be thrown because tried to base class for class with id properties
            }
            catch (OSGeo.Common.Exception)
            {
            }

            // Create a valid subclass.        
            pClass = new FeatureClass("ClassC", "");
            pClass.IsAbstract = false;
            pClass.BaseClass = pBaseClass;

            pProp = new DataPropertyDefinition("Id", "");
            pProp.DataType = DataType.DataType_Int64;
            pProp.Nullable = false;
            pClass.Properties.Add(pProp);
            pSchema.Classes.Add(pClass);

            //Console.WriteLine("finish mgSchemaTest.testSubClass");
            
        }

        public void testRasterClass()
        {
            Console.WriteLine("start mgSchemaTest.testRasterClass");

            string class_name = "Raster Class Name";
            string class_description = "Raster Class Description";
            string raster_property_name = "Raster Property Name";
            string raster_property_description = "Raster Property Description";

            FeatureClass cls = new FeatureClass(class_name, class_description);
            Debug.Assert(cls.ClassType == ClassType.ClassType_FeatureClass);
            PropertyDefinitionCollection properties = cls.Properties;
            RasterPropertyDefinition raster = new RasterPropertyDefinition(raster_property_name, raster_property_description);
            properties.Add(raster);

            //Console.WriteLine("finish mgSchemaTest.testRasterClass");
            
        }

        public void testAssociationOK()
        {
            Console.WriteLine("start mgSchemaTest.testAssociationOK");

            string pName = "FSClassName";
            string pDesc = "FeatureSchema Description";
            FeatureSchema pFSchema = new FeatureSchema(pName, pDesc);
            ClassCollection pClasses = pFSchema.Classes;
            DataPropertyDefinition pProp;

            // Create a feature class
            FeatureClass pfeatureclass = new FeatureClass("FeatureClass", "FeatureClass Desc");
            // Add identity property
            pProp = new DataPropertyDefinition("FeatId", "Id Prop");
            pProp.DataType = DataType.DataType_Int64;
            pProp.Nullable = false;
            pfeatureclass.Properties.Add(pProp);
            pfeatureclass.IdentityProperties.Add(pProp);
            // Add first name and last name properties
            pProp = new DataPropertyDefinition("FirstName", "First Name");
            pProp.DataType = DataType.DataType_String;
            pProp.Nullable = false;
            pfeatureclass.Properties.Add(pProp);
            pProp = new DataPropertyDefinition("LastName", "Last Name");
            pProp.DataType = DataType.DataType_String;
            pProp.Nullable = false;
            pfeatureclass.Properties.Add(pProp);

            // Create a class
            Class pclass = new Class("Class", "Class Desc");
            // Add identity property
            pProp = new DataPropertyDefinition("Id", "Id Prop");
            pProp.DataType = DataType.DataType_Int64;
            pProp.Nullable = false;
            pclass.Properties.Add(pProp);
            pclass.IdentityProperties.Add(pProp);
            // Add name one and name two properties
            pProp = new DataPropertyDefinition("NameOne", "Name One");
            pProp.DataType = DataType.DataType_String;
            pProp.Nullable = false;
            pclass.Properties.Add(pProp);
            pProp = new DataPropertyDefinition("NameTwo", "Name Two");
            pProp.DataType = DataType.DataType_String;
            pProp.Nullable = false;
            pclass.Properties.Add(pProp);

            // Add the classes to the schema class collection
            pClasses.Add(pfeatureclass);
            pClasses.Add(pclass);

            // Create a valid association property
            AssociationPropertyDefinition passprop = new AssociationPropertyDefinition("Association Prop1", "Association Prop Desc");
            passprop.AssociatedClass = pclass;
            passprop.LockCascade = true;
            passprop.DeleteRule = DeleteRule.DeleteRule_Prevent;
            passprop.ReverseName = "Reverse Name";

            DataPropertyDefinition pprop = (DataPropertyDefinition)pclass.Properties["NameOne"];
            passprop.IdentityProperties.Add(pprop);

            pProp = (DataPropertyDefinition)pclass.Properties["NameTwo"];
            passprop.IdentityProperties.Add(pProp);

            pProp = (DataPropertyDefinition)pfeatureclass.Properties["FirstName"];
            passprop.ReverseIdentityProperties.Add(pProp);

            pProp = (DataPropertyDefinition)pfeatureclass.Properties["LastName"];
            passprop.ReverseIdentityProperties.Add(pProp);

            //Add the association property to the feature class property collection
            pfeatureclass.Properties.Add(passprop);

            pFSchema.AcceptChanges();

            //Console.WriteLine("finish mgSchemaTest.testAssociationOK");
            
        }

        public void testAssociationFail1()
        {
            Console.WriteLine("start mgSchemaTest.testAssociationFail1");

            string pName = "FSClassName";
            string pDesc = "FeatureSchema Description";
            FeatureSchema pFSchema = new FeatureSchema(pName, pDesc);
            ClassCollection pClasses = pFSchema.Classes;

            // Create a feature class
            FeatureClass pfeatureclass = new FeatureClass("FeatureClass", "FeatureClass Desc");
            // Add identity property
            DataPropertyDefinition pProp = new DataPropertyDefinition("FeatId", "Id Prop");
            pProp.DataType = DataType.DataType_Int64;
            pProp.Nullable = false;
            pfeatureclass.Properties.Add(pProp);
            pfeatureclass.IdentityProperties.Add(pProp);
            // Add first name and last name properties
            pProp = new DataPropertyDefinition("FirstName", "First Name");
            pProp.DataType = DataType.DataType_String;
            pProp.Nullable = false;
            pfeatureclass.Properties.Add(pProp);
            pProp = new DataPropertyDefinition("LastName", "Last Name");
            pProp.DataType = DataType.DataType_String;
            pProp.Nullable = false;
            pfeatureclass.Properties.Add(pProp);

            // Create a class
            Class pclass = new Class("Class", "Class Desc");
            // Add identity property
            pProp = new DataPropertyDefinition("Id", "Id Prop");
            pProp.DataType = DataType.DataType_Int64;
            pProp.Nullable = false;
            pclass.Properties.Add(pProp);
            pclass.IdentityProperties.Add(pProp);
            // Add name one and name two properties
            pProp = new DataPropertyDefinition("NameOne", "Name One");
            pProp.DataType = DataType.DataType_String;
            pProp.Nullable = false;
            pclass.Properties.Add(pProp);
            pProp = new DataPropertyDefinition("NameTwo", "Name Two");
            pProp.DataType = DataType.DataType_String;
            pProp.Nullable = false;
            pclass.Properties.Add(pProp);

            // Add the classes to the schema class collection
            pClasses.Add(pfeatureclass);
            pClasses.Add(pclass);

            // Create an invalid association property( identity and reverse identity are not of the same types
            AssociationPropertyDefinition passprop = new AssociationPropertyDefinition("Association Prop2", "Association Prop 2 Desc");
            passprop.AssociatedClass = pclass;
            passprop.ReverseName = "Reverse";

            passprop.IdentityProperties.Add((DataPropertyDefinition)pclass.Properties["NameOne"]);

            passprop.IdentityProperties.Add((DataPropertyDefinition)pclass.Properties["NameTwo"]);
            passprop.ReverseIdentityProperties.Add((DataPropertyDefinition)pfeatureclass.Properties["FirstName"]);
            passprop.ReverseIdentityProperties.Add((DataPropertyDefinition)pfeatureclass.Properties["FeatId"]);

            // Add the invalid association property to the feature class property collection
            pfeatureclass.Properties.Add(passprop);
            bool failed = false;
            try
            {
                pFSchema.AcceptChanges();
            }
            catch (OSGeo.Common.Exception)
            {
                // Expected exception
                pFSchema.RejectChanges();
                failed = true;
            }
            Debug.Assert(failed, "testAssociationFail1 did not fail");

            //Console.WriteLine("finish mgSchemaTest.testAssociationFail1");
            
        }

        public void testAssociationFail2()
        {
            Console.WriteLine("start mgSchemaTest.testAssociationFail2");

            string pName = "FSClassName";
            string pDesc = "FeatureSchema Description";
            FeatureSchema pFSchema = new FeatureSchema(pName, pDesc);
            ClassCollection pClasses = pFSchema.Classes;

            // Create a feature class
            FeatureClass pfeatureclass = new FeatureClass("FeatureClass", "FeatureClass Desc");
            // Add identity property
            DataPropertyDefinition pProp = new DataPropertyDefinition("FeatId", "Id Prop");
            pProp.DataType = DataType.DataType_Int64;
            pProp.Nullable = false;
            pfeatureclass.Properties.Add(pProp);
            pfeatureclass.IdentityProperties.Add(pProp);
            // Add first name and last name properties
            pProp = new DataPropertyDefinition("FirstName", "First Name");
            pProp.DataType = DataType.DataType_String;
            pProp.Nullable = false;
            pfeatureclass.Properties.Add(pProp);
            pProp = new DataPropertyDefinition("LastName", "Last Name");
            pProp.DataType = DataType.DataType_String;
            pProp.Nullable = false;
            pfeatureclass.Properties.Add(pProp);

            // Create a class
            Class pclass = new Class("Class", "Class Desc");
            // Add identity property
            pProp = new DataPropertyDefinition("Id", "Id Prop");
            pProp.DataType = DataType.DataType_Int64;
            pProp.Nullable = false;
            pclass.Properties.Add(pProp);
            pclass.IdentityProperties.Add(pProp);
            // Add name one and name two properties
            pProp = new DataPropertyDefinition("NameOne", "Name One");
            pProp.DataType = DataType.DataType_String;
            pProp.Nullable = false;
            pclass.Properties.Add(pProp);
            pProp = new DataPropertyDefinition("NameTwo", "Name Two");
            pProp.DataType = DataType.DataType_String;
            pProp.Nullable = false;
            pclass.Properties.Add(pProp);

            // Add the classes to the schema class collection
            pClasses.Add(pfeatureclass);
            pClasses.Add(pclass);

            // Create an invalid association by not setting the associated class
            AssociationPropertyDefinition passprop = new AssociationPropertyDefinition("Association Prop3", "Association Prop 3 Desc");
            passprop.IdentityProperties.Add((DataPropertyDefinition)pfeatureclass.Properties["FirstName"]);
            passprop.ReverseIdentityProperties.Add((DataPropertyDefinition)pfeatureclass.Properties["FirstName"]);

            // Add the invalid association property to the feature class property collection
            pfeatureclass.Properties.Add(passprop);
            bool failed = false;
            try
            {
                pFSchema.AcceptChanges();
            }
            catch (OSGeo.Common.Exception)
            {
                // Expected exception
                pFSchema.RejectChanges();
                failed = true;
            }
            Debug.Assert(failed, "testAssociationFail2 did not fail");

            //Console.WriteLine("finish mgSchemaTest.testAssociationFail2");
            
        }

        public void testAssociationFail3()
        {
            Console.WriteLine("start mgSchemaTest.testAssociationFail3");

            string pName = "FSClassName";
            string pDesc = "FeatureSchema Description";
            FeatureSchema pFSchema = new FeatureSchema(pName, pDesc);
            ClassCollection pClasses = pFSchema.Classes;

            // Create a feature class
            FeatureClass pfeatureclass = new FeatureClass("FeatureClass", "FeatureClass Desc");
            // Add identity property
            DataPropertyDefinition pProp = new DataPropertyDefinition("FeatId", "Id Prop");
            pProp.DataType = DataType.DataType_Int64;
            pProp.IsAutoGenerated = true;
            pProp.Nullable = false;
            pfeatureclass.Properties.Add(pProp);
            pfeatureclass.IdentityProperties.Add(pProp);
            // Add first name and last name properties
            pProp = new DataPropertyDefinition("FirstName", "First Name");
            pProp.DataType = DataType.DataType_String;
            pProp.Nullable = false;
            pfeatureclass.Properties.Add(pProp);
            pProp = new DataPropertyDefinition("LastName", "Last Name");
            pProp.DataType = DataType.DataType_String;
            pProp.Nullable = false;
            pfeatureclass.Properties.Add(pProp);

            // Create a class
            Class pclass = new Class("Class", "Class Desc");
            // Add identity property
            pProp = new DataPropertyDefinition("Id", "Id Prop");
            pProp.DataType = DataType.DataType_Int64;
            pProp.IsAutoGenerated = true;
            pProp.Nullable = false;
            pclass.Properties.Add(pProp);
            pclass.IdentityProperties.Add(pProp);
            // Add name one and name two properties
            pProp = new DataPropertyDefinition("NameOne", "Name One");
            pProp.DataType = DataType.DataType_String;
            pProp.Nullable = false;
            pclass.Properties.Add(pProp);
            pProp = new DataPropertyDefinition("NameTwo", "Name Two");
            pProp.DataType = DataType.DataType_String;
            pProp.Nullable = false;
            pclass.Properties.Add(pProp);

            // Add the classes to the schema class collection
            pClasses.Add(pfeatureclass);
            pClasses.Add(pclass);

            // Create an invalid a reverse idenitity property with an autogenerated property(FeatId)(should fail)
            AssociationPropertyDefinition passprop = new AssociationPropertyDefinition("Association Prop3", "Association Prop 3 Desc");
            passprop.AssociatedClass = pclass;
            passprop.IdentityProperties.Add((DataPropertyDefinition)pclass.Properties["Id"]);
            passprop.ReverseIdentityProperties.Add((DataPropertyDefinition)pfeatureclass.Properties["FeatId"]);

            // Add the invalid association property to the feature class property collection
            pfeatureclass.Properties.Add(passprop);
            bool failed = false;
            try
            {
                pFSchema.AcceptChanges();
            }
            catch (OSGeo.Common.Exception)
            {
                // Expected exception
                pFSchema.RejectChanges();
                failed = true;
            }
            Debug.Assert(failed, "testAssociationFail3 did not fail");

            //Console.WriteLine("finish mgSchemaTest.testAssociationFail3");
            
        }

        public void testNetworkOK()
        {
            Console.WriteLine("start mgSchemaTest.testNetworkOK");

            FeatureSchema pFSchema = new FeatureSchema("FSClassName", "FeatureSchema Description");
            ClassCollection pClasses = pFSchema.Classes;

            // Create a network layer class
            NetworkLayerClass layer = new NetworkLayerClass("TestLayer", "A network test Layer class");
            // Add property to the layer class
            DataPropertyDefinition pProp = new DataPropertyDefinition("LayerPropertyOne", "A layer test property");
            pProp.DataType = DataType.DataType_String;
            pProp.Nullable = false;
            layer.Properties.Add(pProp);

            // Create a network class
            NetworkClass network = new NetworkClass("TestNetwork", "A network test class");
            // Add property to the layer class
            pProp = new DataPropertyDefinition("NetworkPropertyOne", "A network test property");
            pProp.DataType = DataType.DataType_String;
            pProp.Nullable = false;
            network.Properties.Add(pProp);
            // Set the layer class
            network.LayerClass = layer;

            // Create a parent network node class
            NetworkNodeFeatureClass parentNetworkNode = new NetworkNodeFeatureClass("TestNetworkParentNode", "A parent network node test class");
            // Set the layer, network and cost properties
            AssociationPropertyDefinition layerAssoc = new AssociationPropertyDefinition("Layer Association Prop", "The layer association prop");
            layerAssoc.AssociatedClass = layer;
            AssociationPropertyDefinition networkAssoc = new AssociationPropertyDefinition("Network Association Prop", "The network association prop");
            networkAssoc.AssociatedClass = network;
            parentNetworkNode.LayerProperty = layerAssoc;
            parentNetworkNode.NetworkProperty = networkAssoc;

            // Create a network node class
            NetworkNodeFeatureClass networkNode = new NetworkNodeFeatureClass("TestNetworkNode", "A network node test class");
            // Add property to the layer class
            pProp = new DataPropertyDefinition("Speed", "A speed property");
            pProp.DataType = DataType.DataType_Double;
            pProp.Nullable = false;
            networkNode.Properties.Add(pProp);
            // Set the layer, network and cost properties
            layerAssoc = new AssociationPropertyDefinition("Layer Association Prop", "The layer association prop");
            layerAssoc.AssociatedClass = layer;
            networkAssoc = new AssociationPropertyDefinition("Network Association Prop", "The network association prop");
            networkAssoc.AssociatedClass = network;
            networkNode.LayerProperty = layerAssoc;
            networkNode.NetworkProperty = networkAssoc;
            networkNode.CostProperty = pProp;
            AssociationPropertyDefinition parentAssoc = new AssociationPropertyDefinition("Parent Association Prop", "The parent association prop");
            parentAssoc.AssociatedClass = parentNetworkNode;
            networkNode.ParentNetworkFeatureProperty = parentAssoc;

            // Create a network link class
            NetworkLinkFeatureClass networkLink = new NetworkLinkFeatureClass("TestNetworkLink", "A network link test class");
            // Add property to the layer class
            pProp = new DataPropertyDefinition("Speed", "A speed property");
            pProp.DataType = DataType.DataType_Double;
            pProp.Nullable = false;
            networkLink.Properties.Add(pProp);
            // Set the network and cost properties
            networkLink.NetworkProperty = networkAssoc;
            networkLink.CostProperty = pProp;
            // Set the start and end nodes
            AssociationPropertyDefinition nodeAssoc = new AssociationPropertyDefinition("Network node Association Prop", "The network node association prop");
            nodeAssoc.AssociatedClass = networkNode;
            networkLink.StartNodeProperty = nodeAssoc;
            networkLink.EndNodeProperty = nodeAssoc;

            // Add the classes to the schema class collection
            pClasses.Add(layer);
            pClasses.Add(network);
            pClasses.Add(networkNode);
            pClasses.Add(networkLink);
            pClasses.Add(parentNetworkNode);

            try
            {
                pFSchema.AcceptChanges();
            }
            catch (OSGeo.Common.Exception)
            {
            }

            //Console.WriteLine("finish mgSchemaTest.testNetworkOK");
            
        }

        //
        // This test will attemp to create a link and set the start node and end node from different network
        // than the one used to create the link.
        public void testNetworkFail1()
        {
            Console.WriteLine("start mgSchemaTest.testNetworkFail1");

            // Create a network layer class
            NetworkLayerClass layer = new NetworkLayerClass("TestLayer", "A network test Layer class");
            // Add property to the layer class
            DataPropertyDefinition pProp = new DataPropertyDefinition("LayerPropertyOne", "A layer test property");
            pProp.DataType = DataType.DataType_String;
            pProp.Nullable = false;
            layer.Properties.Add(pProp);

            // Create the first network class
            NetworkClass network1 = new NetworkClass("TestNetwork1", "A network test class");
            // Add property to the layer class
            pProp = new DataPropertyDefinition("NetworkPropertyOne", "A network test property");
            pProp.DataType = DataType.DataType_String;
            pProp.Nullable = false;
            network1.Properties.Add(pProp);
            // Set the layer class
            network1.LayerClass = layer;

            // Create a second network class
            NetworkClass network2 = new NetworkClass("TestNetwork2", "A network test class");
            // Add property to the layer class
            pProp = new DataPropertyDefinition("NetworkPropertyOne", "A network test property");
            pProp.DataType = DataType.DataType_String;
            pProp.Nullable = false;
            network2.Properties.Add(pProp);
            // Set the layer class
            network2.LayerClass = layer;

            // Create the first network node class
            NetworkNodeFeatureClass networkNode1 = new NetworkNodeFeatureClass("TestNetworkNode", "A network node test class");
            // Add property to the layer class
            pProp = new DataPropertyDefinition("Speed", "A speed property");
            pProp.DataType = DataType.DataType_Double;
            pProp.Nullable = false;
            networkNode1.Properties.Add(pProp);
            // Set the layer, network and cost properties
            AssociationPropertyDefinition layerAssoc = new AssociationPropertyDefinition("Layer Association Prop", "The layer association prop");
            layerAssoc.AssociatedClass = layer;
            AssociationPropertyDefinition networkAssoc = new AssociationPropertyDefinition("Network Association Prop", "The network association prop");
            networkAssoc.AssociatedClass = network1;
            networkNode1.LayerProperty = layerAssoc;
            networkNode1.NetworkProperty = networkAssoc;
            networkNode1.CostProperty = pProp;

            // Create the second network node class
            NetworkNodeFeatureClass networkNode2 = new NetworkNodeFeatureClass("TestNetworkNode", "A network node test class");
            // Add property to the layer class
            pProp = new DataPropertyDefinition("Speed", "A speed property");
            pProp.DataType = DataType.DataType_Double;
            pProp.Nullable = false;
            networkNode2.Properties.Add(pProp);
            // Set the layer, network and cost properties
            layerAssoc = new AssociationPropertyDefinition("Layer Association Prop", "The layer association prop");
            layerAssoc.AssociatedClass = layer;
            networkAssoc = new AssociationPropertyDefinition("Network Association Prop", "The network association prop");
            networkAssoc.AssociatedClass = network2;
            networkNode2.LayerProperty = layerAssoc;
            networkNode2.NetworkProperty = networkAssoc;
            networkNode2.CostProperty = pProp;

            // Create a network link class
            NetworkLinkFeatureClass networkLink = new NetworkLinkFeatureClass("TestNetworkLink", "A network link test class");
            // Add property to the layer class
            pProp = new DataPropertyDefinition("Speed", "A speed property");
            pProp.DataType = DataType.DataType_Double;
            pProp.Nullable = false;
            networkLink.Properties.Add(pProp);
            // Set the network and cost properties
            networkLink.NetworkProperty = networkAssoc;
            networkLink.CostProperty = pProp;
            // Set the start and end nodes
            AssociationPropertyDefinition nodeAssoc = new AssociationPropertyDefinition("Network node Association Prop", "The network node association prop");
            nodeAssoc.AssociatedClass = networkNode1;
            bool failed = false;
            try
            {
                networkLink.StartNodeProperty = nodeAssoc;
                networkLink.EndNodeProperty = nodeAssoc;
            }
            catch (OSGeo.Common.Exception)
            {
                // Excepted exception
                failed = true;
            }
            Debug.Assert(failed, "testNetworkFail1 failed to fail!!");

            //Console.WriteLine("finish mgSchemaTest.testNetworkFail1");
            
        }

        public void testNetworkFail2()
        {
            Console.WriteLine("start mgSchemaTest.testNetworkFail2");

            // Create a network layer class
            NetworkLayerClass layer = new NetworkLayerClass("TestLayer", "A network test Layer class");
            // Add property to the layer class
            DataPropertyDefinition pProp = new DataPropertyDefinition("LayerPropertyOne", "A layer test property");
            pProp.DataType = DataType.DataType_String;
            pProp.Nullable = false;
            layer.Properties.Add(pProp);

            // Create a network class
            NetworkClass network = new NetworkClass("TestNetwork", "A network test class");
            // Add property to the layer class
            pProp = new DataPropertyDefinition("NetworkPropertyOne", "A network test property");
            pProp.DataType = DataType.DataType_String;
            pProp.Nullable = false;
            network.Properties.Add(pProp);
            // Set the layer class
            network.LayerClass = layer;

            // Create a network node class
            NetworkNodeFeatureClass networkNode = new NetworkNodeFeatureClass("TestNetworkNode", "A network node test class");
            // Add property to the layer class
            pProp = new DataPropertyDefinition("Speed", "A speed property");
            pProp.DataType = DataType.DataType_Double;
            pProp.Nullable = false;
            // Set the layer, network and cost properties
            AssociationPropertyDefinition layerAssoc = new AssociationPropertyDefinition("Layer Association Prop", "The layer association prop");
            layerAssoc.AssociatedClass = layer;
            AssociationPropertyDefinition networkAssoc = new AssociationPropertyDefinition("Network Association Prop", "The network association prop");
            networkAssoc.AssociatedClass = network;
            networkNode.LayerProperty = layerAssoc;
            networkNode.NetworkProperty = networkAssoc;
            bool failed = false;
            try
            {
                networkNode.CostProperty = pProp;
            }
            catch (OSGeo.Common.Exception)
            {
                // Excepted exception
                failed = true;
            }
            Debug.Assert(failed, "testNetworkFail1 failed to fail!!");

            //Console.WriteLine("finish mgSchemaTest.testNetworkFail2");
            
        }

        public void testXmlAll()
        {
            Console.WriteLine("start mgSchemaTest.testXmlAll");

            FeatureSchemaCollection schemas = new FeatureSchemaCollection(null);
            FeatureSchemaCollection schemas2;

            try
            {
                // Create 2 schemas: electric references acad.
                FeatureSchema acadSchema = createAcadSchema();
                schemas.Add(acadSchema);
                schemas.Add(createElectricSchema(acadSchema));

                // Write both schemas to an XML document.
                schemas.WriteXml("TestFiles\\schema_all1.xml", new XmlFlags("www.autodesk.com"));

                // Read them back into an empty collection.
                schemas2 = new FeatureSchemaCollection(null);
                schemas2.ReadXml("TestFiles\\schema_all1.xml", new XmlFlags("www.autodesk.com", XmlFlags.ErrorLevel.ErrorLevel_High));

                // Write them out again to check for loss of data on round-trip
                schemas2.WriteXml("TestFiles\\schema_all2.xml", new XmlFlags("www.autodesk.com"));

                // Format output files for comparison. Adds carriage returns.
                IoFileStream reader = new IoFileStream("TestFiles\\schema_all1.xml", "r");
                IoFileStream writer = new IoFileStream("TestFiles\\schema_all1.txt", "w+t");
                mgXmlFormatter formatter1 = new mgXmlFormatter(reader, writer);
                formatter1.Format();
                reader.Close();
                writer.Close();

                reader = new IoFileStream("TestFiles\\schema_all2.xml", "r");
                writer = new IoFileStream("TestFiles\\schema_all2.txt", "w+t");
                mgXmlFormatter formatter2 = new mgXmlFormatter(reader, writer);
                formatter2.Format();
                //reader.Close();
                //writer.Close();

                reader.Dispose();
                writer.Dispose();

                Debug.Assert(mgUnitTestUtil.CheckOutput("TestFiles\\schema_all1_master.txt", "TestFiles\\schema_all1.txt"), "schema_all1_master.txt", "schema_all1.txt");
                Debug.Assert(mgUnitTestUtil.CheckOutput("TestFiles\\schema_all1_master.txt", "TestFiles\\schema_all2.txt"), "schema_all1_master.txt", "schema_all2.txt");                
            }
            catch (OSGeo.Common.Exception ex)
            {
                Debug.Assert(false, ex.Message);
            }
            catch (System.Exception ex)
            {
                Debug.Assert(false, ex.Message);
            }

            //Console.WriteLine("finish mgSchemaTest.testXmlAll");
            
        }

        public void testXmlRefs()
        {
            Console.WriteLine("start mgSchemaTest.testXmlRefs");

            FeatureSchemaCollection schemas = new FeatureSchemaCollection(null);

            try
            {
                // Create a number of schemas. Each schema has base class and
                // object property class references to the previous schema
                FeatureSchema schema1 = createRefSchema(1);
                FeatureSchema schema2 = createRefSchema(2, schema1);
                FeatureSchema schema3 = createRefSchema(3, schema2);
                FeatureSchema schema4 = createRefSchema(4, schema3);
                FeatureSchema schema5 = createRefSchema(5, schema4);

                // Write them out to 3 XML documents in memory
                IoMemoryStream schemaStreamA = new IoMemoryStream();
                IoMemoryStream schemaStreamB = new IoMemoryStream();
                IoMemoryStream schemaStreamC = new IoMemoryStream();
                IoMemoryStream schemaStream2 = new IoMemoryStream();

                // 4 and 5 go into the 3rd document
                // This creates an XML document since it contains multiple schemas
                schemas.Add(schema5);
                schemas.Add(schema4);
                schemas.WriteXml(schemaStreamC);

                // 3 into the 2nd document
                // This creates an XML Schema document since there is only one schema.
                schema3.WriteXml(schemaStreamB);

                // 1 and 2 into the first document.
                // This creates an XML document since it contains multiple schemas
                schemas = new FeatureSchemaCollection(null);
                schemas.Add(schema2);
                schemas.Add(schema1);
                schemas.WriteXml(schemaStreamA);

                schemaStreamA.Reset();
                schemaStreamB.Reset();
                schemaStreamC.Reset();

                schemas = new FeatureSchemaCollection(null);
                // Read 1st document into empty collection.
                // This tests the resolving of references between schemas in same document.
                schemas.ReadXml(schemaStreamA); 
                // Read 2nd document into collection.
                // This tests the resolving of references from a schema in an XML document
                // to schemas outside the document, but in the collection being read into.
                schemas.ReadXml(schemaStreamB);

                // Read 3rd document. Tests both types of references, cross-schema and
                // cross-document.
                schemas.ReadXml(schemaStreamC);

                // Write them all back out again. Tests round-tripping
                schemas.WriteXml(schemaStream2);

                // Write the documents to files, with carriage return formatting.
                IoFileStream f1a = new IoFileStream("TestFiles\\schema_refs1a.xml", "w+t");
                IoFileStream f1b = new IoFileStream("TestFiles\\schema_refs1b.xsd", "w+t");
                IoFileStream f1c = new IoFileStream("TestFiles\\schema_refs1c.xml", "w+t");
                IoFileStream f2 = new IoFileStream("TestFiles\\schema_refs2.xml", "w+t");

                schemaStreamA.Reset();
                mgXmlFormatter fmt1a = new mgXmlFormatter(schemaStreamA, f1a);
                fmt1a.Format();
                f1a.Close();

                schemaStreamB.Reset();
                mgXmlFormatter fmt1b = new mgXmlFormatter(schemaStreamB, f1b);
                fmt1b.Format();
                f1b.Close();

                schemaStreamC.Reset();
                mgXmlFormatter fmt1c = new mgXmlFormatter(schemaStreamC, f1c);
                fmt1c.Format();
                f1c.Close();

                schemaStream2.Reset();
                mgXmlFormatter fmt2 = new mgXmlFormatter(schemaStream2, f2);
                fmt2.Format();
                f2.Close();

                schemaStreamC.Reset();
                schemas = new FeatureSchemaCollection(null);

                // The following tests reading a schema with cross-document
                // references into an empty collection. This test will fail 
                // because these references will be unresolvable.
                bool bFailed = false;
                try
                {
                    schemas.ReadXml(schemaStreamC);
                }
                catch (OSGeo.Common.Exception)
                {
                    bFailed = true;
                }
                Debug.Assert(bFailed, "Loading of schemas 4 and 5 into empty feature schema collection should have failed");

                // Compare output against expected results.
                Debug.Assert(mgUnitTestUtil.CheckOutput("TestFiles\\schema_refs1a_master.txt", "TestFiles\\schema_refs1a.xml"), "schema_refs1a_master.txt", "schema_refs1a.xml");
                Debug.Assert(mgUnitTestUtil.CheckOutput("TestFiles\\schema_refs1b_master.txt", "TestFiles\\schema_refs1b.xsd"), "schema_refs1b_master.txt", "schema_refs1b.xsd");
                Debug.Assert(mgUnitTestUtil.CheckOutput("TestFiles\\schema_refs1c_master.txt", "TestFiles\\schema_refs1c.xml"), "schema_refs1c_master.txt", "schema_refs1c.xml");
                Debug.Assert(mgUnitTestUtil.CheckOutput("TestFiles\\schema_refs2_master.txt", "TestFiles\\schema_refs2.xml"), "schema_refs2_master.txt", "schema_refs2.xml");
            }
            catch (OSGeo.Common.Exception ex)
            {
                Debug.Assert(false, ex.Message);
            }
            catch (System.Exception ex)
            {
                Debug.Assert(false, ex.Message);
            }

            //Console.WriteLine("finish mgSchemaTest.testXmlRefs");
            
        }

        public void testXmlMerge()
        {
            Console.WriteLine("start mgSchemaTest.testXmlMerge");

            FeatureSchemaCollection schemas = new FeatureSchemaCollection(null);

            try
            {
                // Initialize a schema collection with 2 schemas.
                FeatureSchema acadSchema = createAcadSchema();
                schemas.Add(acadSchema);
                schemas.Add(createElectricSchema(acadSchema));

                // schema_merge1a.xml contains the above schemas, with some modifications.
                // The following tests merging the modifications into the schema collection.
                schemas.ReadXml("TestFiles\\schema_merge1a.xml", new XmlFlags("www.autodesk.com", XmlFlags.ErrorLevel.ErrorLevel_Normal));

                // Write out the schemas for verification
                schemas.WriteXml("TestFiles\\schema_merge2a.xml", new XmlFlags("www.autodesk.com"));

                // Try merging into schemas with lots of references.
                // Create schema collection with 5 schema, each schema has
                // base class and object property class references to previous
                // schema.
                FeatureSchema schema1 = createRefSchema(1);
                FeatureSchema schema2 = createRefSchema(2, schema1);
                FeatureSchema schema3 = createRefSchema(3, schema2);
                FeatureSchema schema4 = createRefSchema(4, schema3);
                FeatureSchema schema5 = createRefSchema(5, schema4);

                schemas = new FeatureSchemaCollection(null);
                schemas.Add(schema1);
                schemas.Add(schema2);
                schemas.Add(schema3);
                schemas.Add(schema4);
                schemas.Add(schema5);

                // schema_merge1b.xml contains modifications to the 5 schemas.
                // The following tests the merging of these mods into the schema collection.
                schemas.ReadXml("TestFiles\\schema_merge1b.xml", new XmlFlags("fdo.osgeo.org/schemas/feature", XmlFlags.ErrorLevel.ErrorLevel_Normal));

                // Write out the schemas for verification
                schemas.WriteXml("TestFiles\\schema_merge2b.xml", new XmlFlags("www.autodesk.com"));

                // Test merging invalid modifications (changes to class or property type).
                bool bFailed = false;
                try
                {
                    schemas.ReadXml("TestFiles\\schema_merge_err.xml", new XmlFlags("www.autodesk.com", XmlFlags.ErrorLevel.ErrorLevel_High));
                }
                catch (OSGeo.Common.Exception e)
                {
                    Debug.Assert(e.Message.IndexOf("Failed to open file 'TestFiles\\schema_merge_err.xml' with access modes: 'rt'. ") != -1);
                    bFailed = true;
                }
                Debug.Assert(bFailed, "Loading of schema_merge_err.xml should have failed");

                // Write out the schemas for verification. In this case the schemas
                // should not have changed, since the last read had errors.
                schemas.WriteXml("TestFiles\\schema_merge3b.xml", new XmlFlags("www.autodesk.com"));

                // Format the output files with carriage returns so they can 
                // be compared with masters.
                IoFileStream reader = new IoFileStream("TestFiles\\schema_merge2a.xml", "r");
                IoFileStream writer = new IoFileStream("TestFiles\\schema_merge2a.txt", "w+t");
                mgXmlFormatter formatter1 = new mgXmlFormatter(reader, writer);
                formatter1.Format();
                writer.Close();

                reader = new IoFileStream("TestFiles\\schema_merge2b.xml", "r");
                writer = new IoFileStream("TestFiles\\schema_merge2b.txt", "w+t");
                mgXmlFormatter formatter2 = new mgXmlFormatter(reader, writer);
                formatter2.Format();
                writer.Close();

                reader = new IoFileStream("TestFiles\\schema_merge3b.xml", "r");
                writer = new IoFileStream("TestFiles\\schema_merge3b.txt", "w+t");
                mgXmlFormatter formatter3 = new mgXmlFormatter(reader, writer);
                formatter3.Format();
                writer.Close();

                // Compare output against expected results.              
                Debug.Assert(mgUnitTestUtil.CheckOutput("TestFiles\\schema_merge2a_master.txt", "TestFiles\\schema_merge2a.txt"), "schema_merge2a_master.txt", "schema_merge2a.txt");
               Debug.Assert(mgUnitTestUtil.CheckOutput("TestFiles\\schema_merge2b_master.txt", "TestFiles\\schema_merge2b.txt"), "schema_merge2b_master.txt", "schema_merge2b.txt");
               Debug.Assert(mgUnitTestUtil.CheckOutput("TestFiles\\schema_merge2b_master.txt", "TestFiles\\schema_merge3b.txt"), "schema_merge2b_master.txt", "schema_merge3b.txt");               
            }
            catch (OSGeo.Common.Exception ex)
            {
                Debug.Assert(false, ex.Message);
            }
            catch (System.Exception ex)
            {
                Debug.Assert(false, ex.Message);
            }

            //Console.WriteLine("finish mgSchemaTest.testXmlMerge");
            
        }

        public void testXmlUnsupported()
        {
            Console.WriteLine("start mgSchemaTest.testXmlUnsupported");

            try
            {
                // The following reads in a schema that has GML elements not supported by FDO.
                // A number of errors are generated. The errors depend on the 
                // error level. All 4 error levels are tested.
                eachUnsupported(XmlFlags.ErrorLevel.ErrorLevel_High, "TestFiles\\schema_uns_high_master.txt", "TestFiles\\schema_uns_high.txt");
                eachUnsupported(XmlFlags.ErrorLevel.ErrorLevel_Normal, "TestFiles\\schema_uns_norm_master.txt", "TestFiles\\schema_uns_norm.txt");
                eachUnsupported(XmlFlags.ErrorLevel.ErrorLevel_Low, "TestFiles\\schema_uns_low_master.txt", "TestFiles\\schema_uns_low.txt");
                eachUnsupported(XmlFlags.ErrorLevel.ErrorLevel_VeryLow, "TestFiles\\schema_uns_vlow_master.txt", "TestFiles\\schema_uns_vlow.txt");
            }
            catch (OSGeo.Common.Exception ex)
            {
                Debug.Assert(false, ex.Message);
            }
            catch (System.Exception ex)
            {
                Debug.Assert(false, ex.Message);
            }

            //Console.WriteLine("finish mgSchemaTest.testXmlUnsupported");
            
        }

        public void testRefErrors()
        {
            Console.WriteLine("start mgSchemaTest.testRefErrors");


            FeatureSchemaCollection schemas = new FeatureSchemaCollection(null);
            Int32 i;

            try
            {
                // Create classes that reference classes in other schemas.
                FeatureSchema schema1 = createRefSchema(1);
                FeatureSchema schema2 = createRefSchema(2, schema1);
                FeatureSchema schema3 = createRefSchema(3, schema2);
                FeatureSchema schema4 = createRefSchema(4, schema3);

                schemas = new FeatureSchemaCollection(null);
                schemas.Add(schema1);
                schemas.Add(schema2);
                schemas.Add(schema3);
                schemas.Add(schema4);

                // Detach some referenced classes from their schemas.
                ClassCollection classes = schema1.Classes;
                for (i = (classes.Count - 1); i >= 0; i--)
                {
                    classes.RemoveAt(i);
                }

                classes = schema3.Classes;
                for (i = (classes.Count - 1); i >= 0; i--)
                {
                    classes.RemoveAt(i);
                }

                // Trap the bad references by writing to XML
                IoMemoryStream schemaStream = new IoMemoryStream();

                bool bFailed = false;

                // Try just one schema
                try
                {
                    schema2.WriteXml(schemaStream);
                }
                catch (OSGeo.Common.Exception)
                {
                    bFailed = true;
                }

                Debug.Assert(bFailed, "WriteXml of schema should have failed");

                schemaStream.Reset();

                // try all schemas.
                try
                {
                    schemas.WriteXml(schemaStream);
                }
                catch (OSGeo.Common.Exception)
                {
                    bFailed = true;
                }

                Debug.Assert(bFailed, "WriteXml of schema collection should have failed");
            }
            catch (OSGeo.Common.Exception ex)
            {
                Debug.Assert(false, ex.Message);
            }
            catch (System.Exception ex)
            {
                Debug.Assert(false, ex.Message);
            }

            //Console.WriteLine("finish mgSchemaTest.testRefErrors");
            
        }

        public void testXmlExternal()
        {
            Console.WriteLine("start mgSchemaTest.testXmlExternal");

            FeatureSchemaCollection schemas = new FeatureSchemaCollection(null);
            FeatureSchemaCollection schemas2 = new FeatureSchemaCollection(null);

            try
            {
                /* Read the "externally created" schema */
                schemas.ReadXml("TestFiles\\schema_ext1.xml", new XmlFlags("www.autodesk.com", XmlFlags.ErrorLevel.ErrorLevel_VeryLow));

                // Write the schema back to an XML document.
                schemas.WriteXml("TestFiles\\schema_ext2a.xml", new XmlFlags("www.autodesk.com"));

                // Format output files for comparison. Adds carriage returns.
                IoFileStream reader = new IoFileStream("TestFiles\\schema_ext2a.xml", "r");
                IoFileStream writer = new IoFileStream("TestFiles\\schema_ext2a.txt", "w+t");
                mgXmlFormatter formatter1 = new mgXmlFormatter(reader, writer);
                formatter1.Format();
                reader.Close();
                writer.Close();

                // Compare output and expected results.
                Debug.Assert(mgUnitTestUtil.CheckOutput("TestFiles\\schema_ext_a_master.txt", "TestFiles\\schema_ext2a.txt"));
            }
            catch (OSGeo.Common.Exception ex)
            {
                Debug.Assert(false, ex.Message);
            }
            catch (System.Exception ex)
            {
                Debug.Assert(false, ex.Message);
            }

            //Console.WriteLine("finish mgSchemaTest.testXmlExternal");
            
        }

        public void testXmlSchemaMappings()
        {
            Console.WriteLine("start mgSchemaTest.testXmlExternal");
            
            int i;
            FeatureSchemaCollection schemas = new FeatureSchemaCollection(null);

            try
            {
                XmlFlags flags = new XmlFlags("fdo.osgeo.org/schemas/feature", XmlFlags.ErrorLevel.ErrorLevel_VeryLow);
                flags.SchemaNameAsPrefix = true;
                flags.UseGmlId = true;

                XmlReader gmlReader = Schema.CreateReader();
                XmlReader extReader = new XmlReader("TestFiles\\schema_ext1.xml");

                IoMemoryStream mergedStream = new IoMemoryStream();
                XmlWriter mergedWriter = new XmlWriter(mergedStream);
                // RefHandler performs copy and accumulates any include or import
                // references.
                XmlCopyHandler copier = new XmlCopyHandler(mergedWriter);

                gmlReader.Parse(copier);
                extReader.Parse(copier);
                                
                mergedWriter.Close();
                mergedStream.Reset();

                XmlReader mergedReader = new XmlReader(mergedStream);

                schemas.ReadXml(mergedReader, flags);
                PhysicalSchemaMappingCollection mappings = schemas.XmlSchemaMappings;
                
                XmlWriter writer = new XmlWriter("TestFiles\\schema_mappings_212.xml", true, XmlWriter.LineFormat.LineFormat_Indent);
                mappings.WriteXml(writer);
                writer.Close();

                // Compare output and expected results.
                Debug.Assert(mgUnitTestUtil.CheckOutput("TestFiles\\schema_mappings_212_master.txt", "TestFiles\\schema_mappings_212.xml"));

                XmlSchemaMapping mapping = null;
                for (i = 0; i < mappings.Count; i++)
                {
                    mapping = (XmlSchemaMapping)mappings[i];
                    if (mapping.TargetNamespace == "http://www.autodesk.com/External")
                        break;
                    mapping = null;
                }

                Debug.Assert(mapping != null);

                XmlElementMapping elemMapping = mapping.GetElementMappings()["TestFeat1"];
                Debug.Assert(elemMapping != null);

                Debug.Assert( elemMapping.GmlUri == "http://www.autodesk.com/External");
                Debug.Assert( elemMapping.GmlLocalName == "TestFeat1");

                XmlClassMapping classMapping = elemMapping.ClassMapping;
                Debug.Assert(classMapping != null);

                XmlElementMapping subElemMapping =  classMapping.ElementMappings["ext/LineProperty"];
                Debug.Assert(subElemMapping != null);

                Debug.Assert( subElemMapping.GmlUri == "http://www.autodesk.com/External");
                Debug.Assert( subElemMapping.GmlLocalName == "LineProperty");

                XmlClassMapping propClassMapping = subElemMapping.ClassMapping;
                Debug.Assert(classMapping != null);

                Debug.Assert(propClassMapping.WkBaseName == "CurveProperty");

                subElemMapping = classMapping.ElementMappings["GrpDouble"];
                Debug.Assert(subElemMapping != null);

                Debug.Assert( subElemMapping.GmlUri == "http://www.autodesk.com/External");
                Debug.Assert( subElemMapping.GmlLocalName =="GrpDouble");
            }
            catch (OSGeo.Common.Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
            catch (System.Exception ex)
            {
                Console.WriteLine(ex.Message);
            }

            //Console.WriteLine("finish mgSchemaTest.testXmlExternal");
            
        }

        private void eachUnsupported(XmlFlags.ErrorLevel level, string masterFile, string outFile)
        {
            FeatureSchemaCollection schemas = new FeatureSchemaCollection(null);
            bool bFailed = false;

            try
            {
                schemas.ReadXml("TestFiles\\schema_unsupported.xsd", new XmlFlags("fdo.osgeo.org/schemas/feature", level));
            }
            catch (OSGeo.Common.Exception e)
            {
                mgUnitTestUtil.PrintException(e.Message, outFile);
                if (level == XmlFlags.ErrorLevel.ErrorLevel_VeryLow)
                {
                    Debug.Assert(false);
                    bFailed = true;
                }

                if (level != XmlFlags.ErrorLevel.ErrorLevel_VeryLow)
                {
                    Debug.Assert(!bFailed, "Loading of schema_unsupported.xsd should have failed");                    
                }
                mgUnitTestUtil.CheckInClude(masterFile, outFile);
            }
        }

        private FeatureSchema createAcadSchema()
        {
            FeatureSchema pSchema = new FeatureSchema("Acad", "AutoCAD schema");

            // Id'less class for Object Properties only

            Class pXData = new Class("AcXData", "Xdata");
            pXData.IsAbstract = false;

            DataPropertyDefinition pXDataSeq = new DataPropertyDefinition("Seq", "seq");
            pXDataSeq.DataType = DataType.DataType_Int32;
            pXDataSeq.Nullable = false;
            pXData.Properties.Add(pXDataSeq);

            DataPropertyDefinition pProp = new DataPropertyDefinition("Datavalue", "datavalue");
            pProp.DataType = DataType.DataType_String;
            pProp.Length = 4000;
            pProp.Nullable = false;
            pXData.Properties.Add(pProp);
            pSchema.Classes.Add(pXData);

            // More Id'less classes to test nested object properties        
            Class pCoordVal = new Class("AcDbVertexCoordinateValue", "");
            pCoordVal.IsAbstract = false;

            DataPropertyDefinition pCoordValSeq = new DataPropertyDefinition("Seq", "seq");
            pCoordValSeq.DataType = DataType.DataType_Int32;
            pCoordValSeq.Nullable = false;
            pCoordVal.Properties.Add(pCoordValSeq);

            pProp = new DataPropertyDefinition("Value", "");
            pProp.DataType = DataType.DataType_Double;
            pProp.Nullable = false;
            pCoordVal.Properties.Add(pProp);

            pSchema.Classes.Add(pCoordVal);

            Class pVertex = new Class("AcDbVertexData", "");
            pVertex.IsAbstract = false;

            DataPropertyDefinition pVertexSeq = new DataPropertyDefinition("Seq", "seq");
            pVertexSeq.IsAutoGenerated = true;
            pVertexSeq.DataType = DataType.DataType_Int32;
            pVertexSeq.Nullable = false;
            pVertex.Properties.Add(pVertexSeq);

            ObjectPropertyDefinition pObjProp = new ObjectPropertyDefinition("point", "");
            pObjProp.Class = pCoordVal;
            pObjProp.IdentityProperty = pCoordValSeq;
            pObjProp.ObjectType = ObjectType.ObjectType_Collection;
            pVertex.Properties.Add(pObjProp);

            pSchema.Classes.Add(pVertex);

            // A non-abstract base class

            FeatureClass pEntClass = new FeatureClass("AcDbEntity", "AutoCAD entity base class");
            pEntClass.IsAbstract = false;

            pProp = new DataPropertyDefinition("FeatId", "id");
            pProp.DataType = DataType.DataType_Int64;
            pProp.Nullable = false;
            pProp.ReadOnly = true;
            pEntClass.Properties.Add(pProp);
            pEntClass.IdentityProperties.Add(pProp);

            pProp = new DataPropertyDefinition("Layer", "Acad layer");
            pProp.DataType = DataType.DataType_String;
            pProp.Length = 10;
            pProp.Nullable = true;
            pEntClass.Properties.Add(pProp);

            pProp = new DataPropertyDefinition("ColourIndex", "Acad Colour");
            pProp.DataType = DataType.DataType_Byte;
            pProp.Nullable = true;
            pEntClass.Properties.Add(pProp);

            pObjProp = new ObjectPropertyDefinition("xdata", "xdata");
            pObjProp.Class = pXData;
            pObjProp.IdentityProperty = pXDataSeq;
            pObjProp.ObjectType = ObjectType.ObjectType_OrderedCollection;
            pObjProp.OrderType = OrderType.OrderType_Ascending;
            pEntClass.Properties.Add(pObjProp);
            pSchema.Classes.Add(pEntClass);

            // A sub-class to test Object and Data property inheritance.        
            FeatureClass pPlineClass = new FeatureClass("AcDb3dPolyline", "AutoCAD 3d polyline");
            pPlineClass.IsAbstract = false;
            pPlineClass.BaseClass = pEntClass;
            pSchema.Classes.Add(pPlineClass);

            pProp = new DataPropertyDefinition("Width", "line width");
            pProp.DataType = DataType.DataType_Double;
            pProp.Precision = 10;
            pProp.Scale = 5;
            pProp.Nullable = false;
            pPlineClass.Properties.Add(pProp);

            pProp = new DataPropertyDefinition("Closed", "is first and last points the same");
            pProp.DataType = DataType.DataType_Boolean;
            pProp.Nullable = false;
            pPlineClass.Properties.Add(pProp);

            pObjProp = new ObjectPropertyDefinition("vertices", "");
            pObjProp.Class = pVertex;
            pObjProp.IdentityProperty = pVertexSeq;
            pObjProp.ObjectType = ObjectType.ObjectType_OrderedCollection;
            pObjProp.OrderType = OrderType.OrderType_Descending;
            pPlineClass.Properties.Add(pObjProp);

            // The following tests object property nesting to 3 levels.

            FeatureClass pHatchClass = new FeatureClass("AcDbHatch", "AutoCAD hatched polygon");
            pHatchClass.IsAbstract = false;
            pHatchClass.BaseClass = pEntClass;

            pObjProp = new ObjectPropertyDefinition("edges", "");
            pObjProp.Class = pPlineClass;
            pObjProp.ObjectType = ObjectType.ObjectType_Collection;
            pHatchClass.Properties.Add(pObjProp);
            pSchema.Classes.Add(pHatchClass);

            /* Create a number of interrelated network classes, that have no base classes */

            // Create a network layer class
            NetworkLayerClass layer = new NetworkLayerClass("TestLayer", "A network test Layer class");
            // Add property to the layer class
            pProp = new DataPropertyDefinition("LayerPropertyOne", "A layer test property");
            pProp.DataType = DataType.DataType_String;
            pProp.Length = 50;
            pProp.Nullable = false;
            layer.Properties.Add(pProp);
            pSchema.Classes.Add(layer);

            // Create a network class
            NetworkClass network = new NetworkClass("TestNetwork", "A network test class");
            // Add property to the layer class
            pProp = new DataPropertyDefinition("NetworkPropertyOne", "A network test property");
            pProp.DataType = DataType.DataType_String;
            pProp.Length = 100;
            pProp.Nullable = false;
            network.Properties.Add(pProp);
            // Set the layer class
            network.LayerClass = layer;
            pSchema.Classes.Add(network);

            // Create a parent network node class
            NetworkNodeFeatureClass parentNetworkNode = new NetworkNodeFeatureClass("TestNetworkParentNode", "A parent network node test class");
            // Set the layer, network and cost properties
            AssociationPropertyDefinition layerAssoc = new AssociationPropertyDefinition("Layer Association Prop", "The layer association prop");
            layerAssoc.AssociatedClass = layer;
            AssociationPropertyDefinition networkAssoc = new AssociationPropertyDefinition("Network Association Prop", "The network association prop");
            networkAssoc.AssociatedClass = network;
            parentNetworkNode.LayerProperty = layerAssoc;
            parentNetworkNode.NetworkProperty = networkAssoc;
            pSchema.Classes.Add(parentNetworkNode);

            // Create a network node class
            NetworkNodeFeatureClass networkNode = new NetworkNodeFeatureClass("TestNetworkNode", "A network node test class");
            // Add property to the layer class
            pProp = new DataPropertyDefinition("Speed", "A speed property");
            pProp.DataType = DataType.DataType_Double;
            pProp.Nullable = false;
            networkNode.Properties.Add(pProp);
            // Set the layer, network and cost properties
            layerAssoc = new AssociationPropertyDefinition("Layer Association Prop", "The layer association prop");
            layerAssoc.AssociatedClass = layer;
            networkAssoc = new AssociationPropertyDefinition("Network Association Prop", "The network association prop");
            networkAssoc.AssociatedClass = network;
            networkNode.LayerProperty = layerAssoc;
            networkNode.NetworkProperty = networkAssoc;
            networkNode.CostProperty = pProp;
            AssociationPropertyDefinition parentAssoc = new AssociationPropertyDefinition("Parent Association Prop", "The parent association prop");
            parentAssoc.AssociatedClass = parentNetworkNode;
            networkNode.ParentNetworkFeatureProperty = parentAssoc;
            AssociationPropertyDefinition nodeFeatAssoc = new AssociationPropertyDefinition("Feature Association Prop", "The feature association prop");
            nodeFeatAssoc.AssociatedClass = pHatchClass;
            networkNode.ReferencedFeatureProperty = nodeFeatAssoc;
            pSchema.Classes.Add(networkNode);

            // Create a network link class
            NetworkLinkFeatureClass networkLink = new NetworkLinkFeatureClass("TestNetworkLink", "A network link test class");
            // Add property to the layer class
            pProp = new DataPropertyDefinition("Speed", "A speed property");
            pProp.DataType = DataType.DataType_Double;
            pProp.Nullable = false;
            networkLink.Properties.Add(pProp);
            // Set the network and cost properties
            networkLink.NetworkProperty = networkAssoc;
            networkLink.CostProperty = pProp;
            // Set the start and end nodes
            AssociationPropertyDefinition nodeAssoc = new AssociationPropertyDefinition("Network node Association Prop", "The network node association prop");
            nodeAssoc.AssociatedClass = networkNode;
            networkLink.StartNodeProperty = nodeAssoc;
            networkLink.EndNodeProperty = nodeAssoc;
            AssociationPropertyDefinition linkFeatAssoc = new AssociationPropertyDefinition("Feature Association Prop", "The feature association prop");
            linkFeatAssoc.AssociatedClass = pPlineClass;
            networkLink.ReferencedFeatureProperty = linkFeatAssoc;
            pSchema.Classes.Add(networkLink);

            /* Create a number of non-interrelated network classes, that have base classes */

            // Create a network layer class
            NetworkLayerClass layer2 = new NetworkLayerClass("TestLayer2", "A network test Layer class");
            layer2.BaseClass = layer;
            // Add property to the layer class
            pProp = new DataPropertyDefinition("LayerPropertyTwo", "A layer test property");
            pProp.DataType = DataType.DataType_String;
            pProp.Length = 52;
            pProp.Nullable = true;
            layer2.Properties.Add(pProp);
            pSchema.Classes.Add(layer2);

            // Create a network class
            NetworkClass network2 = new NetworkClass("TestNetwork2", "A network test class");
            network2.BaseClass = network;
            // Add property to the layer class
            pProp = new DataPropertyDefinition("NetworkPropertyTwo", "A network test property");
            pProp.DataType = DataType.DataType_String;
            pProp.Length = 102;
            pProp.Nullable = true;
            network2.Properties.Add(pProp);
            pSchema.Classes.Add(network2);

            // Create a parent network node class
            NetworkNodeFeatureClass parentNetworkNode2 = new NetworkNodeFeatureClass("TestNetworkParentNode2", "A parent network node test class");
            parentNetworkNode2.BaseClass = parentNetworkNode;
            pSchema.Classes.Add(parentNetworkNode2);

            // Create a network node class
            NetworkNodeFeatureClass networkNode2 = new NetworkNodeFeatureClass("TestNetworkNode2", "A network node test class");
            networkNode2.BaseClass = networkNode;
            // Add property to the layer class
            pProp = new DataPropertyDefinition("Closed", "");
            pProp.DataType = DataType.DataType_Boolean;
            pProp.Nullable = false;
            networkNode2.Properties.Add(pProp);
            pSchema.Classes.Add(networkNode2);

            // Create a network link class
            NetworkLinkFeatureClass networkLink2 = new NetworkLinkFeatureClass("TestNetworkLink2", "A network link test class");
            networkLink2.BaseClass = networkLink;
            // Add property to the layer class
            pProp = new DataPropertyDefinition("Material", "");
            pProp.DataType = DataType.DataType_String;
            pProp.Length = 52;
            pProp.Nullable = false;
            networkLink2.Properties.Add(pProp);
            pSchema.Classes.Add(networkLink2);

            return (pSchema);
        }

        private FeatureSchema createElectricSchema(FeatureSchema pAcadSchema)
        {
            ClassDefinition pEntClass = pAcadSchema.Classes["AcDbEntity"];
            ClassDefinition pPlineClass = pAcadSchema.Classes["AcDb3dPolyline"];

            FeatureSchema pSchema = new FeatureSchema("Electrical", "Electrical schema");

            pSchema.Attributes.Add("Author", "Thomas Edison");

            /* An abstract base class */
            FeatureClass pDevClass = new FeatureClass("Electric Device", "electic base class");
            pDevClass.IsAbstract = true;
            pDevClass.BaseClass = pEntClass;

            // Test nested object properties (ElectricDevice.furniture.xdata) where graphic's class has an id.        
            ObjectPropertyDefinition pObjProp = new ObjectPropertyDefinition("furniture", "Acad entity");
            pObjProp.ObjectType = ObjectType.ObjectType_Value;
            pObjProp.Class = pEntClass;
            pDevClass.Properties.Add(pObjProp);

            // Test geometry property        
            GeometricPropertyDefinition pGeomProp = new GeometricPropertyDefinition("Geometry", "location and shape");
            pGeomProp.GeometryTypes = 0x01 | 0x02 | 0x04 | 0x08;
            pGeomProp.HasMeasure = true;
            pGeomProp.ReadOnly = true;
            pGeomProp.SpatialContextAssociation = "SC_0";
            pGeomProp.Attributes.Add("Measure Units", "metres");
            pDevClass.Properties.Add(pGeomProp);

            pDevClass.GeometryProperty = pGeomProp;

            pSchema.Classes.Add(pDevClass);

            /* Subclass with dictionary */
            FeatureClass pTransClass = new FeatureClass("Transformer", "");
            pTransClass.IsAbstract = false;
            pTransClass.BaseClass = pDevClass;
            pTransClass.Attributes.Add("Rules DLL", "transformer.dll");
            pTransClass.Attributes.Add("Entrypoint", "start_transformer");

            // Add data properties of various types
            DataPropertyDefinition pProp = new DataPropertyDefinition("Voltage", "voltage");
            pProp.Attributes.Add("Calculable", "yes");
            pProp.DataType = DataType.DataType_Decimal;
            pProp.Precision = 10;
            pProp.Scale = -2;
            pProp.Nullable = false;
            pTransClass.Properties.Add(pProp);

            DataPropertyDefinition pTransPhase = new DataPropertyDefinition("Phase", "A, B or C");
            pTransPhase.DataType = DataType.DataType_String;
            pTransPhase.Length = 1;
            pTransPhase.Nullable = false;
            pTransClass.Properties.Add(pTransPhase);

            pProp = new DataPropertyDefinition("InstallDate", "");
            pProp.DataType = DataType.DataType_DateTime;
            pProp.Nullable = true;
            pTransClass.Properties.Add(pProp);

            DataPropertyDefinition pTransPartNum = new DataPropertyDefinition("PartNum", "");
            pTransPartNum.DataType = DataType.DataType_Int16;
            pTransPartNum.Nullable = true;
            pTransClass.Properties.Add(pTransPartNum);

            pProp = new DataPropertyDefinition("Volume", "");
            pProp.DataType = DataType.DataType_Single;
            pProp.Nullable = false;
            pTransClass.Properties.Add(pProp);

            pProp = new DataPropertyDefinition("Picture", "");
            pProp.DataType = DataType.DataType_BLOB;
            pProp.Nullable = true;
            pTransClass.Properties.Add(pProp);

            pProp = new DataPropertyDefinition("Specifications", "");
            pProp.DataType = DataType.DataType_CLOB;
            pProp.Nullable = false;
            pTransClass.Properties.Add(pProp);

            // Raster property (all defaults)
            RasterPropertyDefinition pRasterProp = new RasterPropertyDefinition("Maintenance Notes", "");
            pTransClass.Properties.Add(pRasterProp);
            pSchema.Classes.Add(pTransClass);

            // Test class that is already suffixed by "Type". GML ComplexType should end up
            // as TransformerTypeType
            Class pTransTypeClass = new Class("TransformerType", "");
            pTransTypeClass.IsAbstract = false;

            pProp = new DataPropertyDefinition("Name", "type name");
            pProp.DataType = DataType.DataType_String;
            pProp.Length = 30;
            pProp.Nullable = false;
            pTransTypeClass.Properties.Add(pProp);
            pTransTypeClass.IdentityProperties.Add(pProp);
            pSchema.Classes.Add(pTransTypeClass);

            // Test class that is called "Type". GML ComplexType should end up
            // as TypeType
            Class pTypeClass = new Class("Type", "");
            pTypeClass.IsAbstract = false;

            pProp = new DataPropertyDefinition("Name", "type name");
            pProp.DataType = DataType.DataType_String;
            pProp.Length = 30;
            pProp.Nullable = false;
            pTypeClass.Properties.Add(pProp);
            pTypeClass.IdentityProperties.Add(pProp);
            pSchema.Classes.Add(pTypeClass);

            // Add a new non-inherited class that will later be removed.
            FeatureClass pPoleClass = new FeatureClass("PoleTypeStructure", "");
            pPoleClass.IsAbstract = false;

            pProp = new DataPropertyDefinition("Street", "");
            pProp.DataType = DataType.DataType_String;
            pProp.Length = 20;
            pProp.Nullable = false;
            pPoleClass.Properties.Add(pProp);
            pPoleClass.IdentityProperties.Add(pProp);

            pProp = new DataPropertyDefinition("ID", "");
            pProp.DataType = DataType.DataType_Double;
            pProp.Nullable = false;
            pPoleClass.Properties.Add(pProp);
            pPoleClass.IdentityProperties.Add(pProp);

            pProp = new DataPropertyDefinition("Height", "");
            pProp.DataType = DataType.DataType_Double;
            pProp.Nullable = false;
            pProp.DefaultValue = "15";
            pPoleClass.Properties.Add(pProp);

            pGeomProp = new GeometricPropertyDefinition("Location", "point location");
            pGeomProp.GeometryTypes = 0x01;
            pGeomProp.HasElevation = false;
            pPoleClass.Properties.Add(pGeomProp);
            pPoleClass.GeometryProperty = pGeomProp;

            pGeomProp = new GeometricPropertyDefinition("Shape", "pole shape");
            pGeomProp.GeometryTypes = 0x02;
            pGeomProp.SpatialContextAssociation = "SC_1";
            pPoleClass.Properties.Add(pGeomProp);

            // Raster property (no defaults)
            pRasterProp = new RasterPropertyDefinition("Picture", "");
            pRasterProp.Nullable = true;
            pRasterProp.ReadOnly = true;
            pRasterProp.DefaultImageXSize = 4000;
            pRasterProp.DefaultImageYSize = 2000;

            RasterDataModel model = new RasterDataModel();
            model.DataModelType = RasterDataModelType.RasterDataModelType_Bitonal;
            
            
            model.Organization = RasterDataOrganization.RasterDataOrganization_Pixel;
            model.BitsPerPixel = 24;
            model.TileSizeX = 500;
            model.TileSizeY = 250;
            pRasterProp.DefaultDataModel = model;

            pPoleClass.Properties.Add(pRasterProp);

            // Association property (all defaults)
            AssociationPropertyDefinition pAssocProp = new AssociationPropertyDefinition("Mounted Transformer", "sits on pole");
            pAssocProp.AssociatedClass = pTransClass;

            pPoleClass.Properties.Add(pAssocProp);

            // Raster property (all defaults except one model attribute).
            pRasterProp = new RasterPropertyDefinition("Infrared", "");
            pRasterProp.Nullable = false;

            model = new RasterDataModel();
            model.BitsPerPixel = 16;
            pRasterProp.DefaultDataModel = model;

            pPoleClass.Properties.Add(pRasterProp);
            pSchema.Classes.Add(pPoleClass);

            // The rest of the classes test various types of association properties
            Class pMaintClass = new Class("Maint Hist", "");
            pMaintClass.IsAbstract = false;

            pProp = new DataPropertyDefinition("id", "");
            pProp.DataType = DataType.DataType_Int64;
            pProp.Nullable = false;
            pMaintClass.Properties.Add(pProp);
            pMaintClass.IdentityProperties.Add(pProp);

            DataPropertyDefinition pMaintPhase = new DataPropertyDefinition("Phase", "");
            pMaintPhase.DataType = DataType.DataType_String;
            pMaintPhase.Length = 1;
            pMaintPhase.Nullable = false;
            pMaintClass.Properties.Add(pMaintPhase);

            DataPropertyDefinition pMaintPartNum = new DataPropertyDefinition("PartNum", "");
            pMaintPartNum.DataType = DataType.DataType_Int16;
            pMaintPartNum.Nullable = true;
            pMaintClass.Properties.Add(pMaintPartNum);

            // Association property with multiple explicit identity properties and
            // reverse name.
            pAssocProp = new AssociationPropertyDefinition("Transformer", "");
            pAssocProp.AssociatedClass = pTransClass;
            pAssocProp.ReverseName = "Maintenance";
            pAssocProp.Multiplicity = "1";

            pAssocProp.IdentityProperties.Add(pTransPhase);
            pAssocProp.IdentityProperties.Add(pTransPartNum);
            pAssocProp.ReverseIdentityProperties.Add(pMaintPhase);
            pAssocProp.ReverseIdentityProperties.Add(pMaintPartNum);

            pMaintClass.Properties.Add(pAssocProp);
            pSchema.Classes.Add(pMaintClass);

            Class pOutClass = new Class("Outage Hist", "");
            pOutClass.IsAbstract = false;

            pProp = new DataPropertyDefinition("id", "");
            pProp.DataType = DataType.DataType_Int64;
            pProp.Nullable = false;
            pOutClass.Properties.Add(pProp);
            pOutClass.IdentityProperties.Add(pProp);

            DataPropertyDefinition pOutPhase = new DataPropertyDefinition("Phase", "");
            pOutPhase.DataType = DataType.DataType_String;
            pOutPhase.Length = 1;
            pOutPhase.Nullable = false;
            pOutClass.Properties.Add(pOutPhase);

            DataPropertyDefinition pOutPartNum = new DataPropertyDefinition("PartNum", "");
            pOutPartNum.DataType = DataType.DataType_Int16;
            pOutPartNum.Nullable = true;
            pOutClass.Properties.Add(pOutPartNum);

            // Association property with multiple explicit identity properties, but no
            // reverse name.
            pAssocProp = new AssociationPropertyDefinition("Transformer", "");
            pAssocProp.AssociatedClass = pTransClass;
            pAssocProp.Multiplicity = "1";

            pAssocProp.IdentityProperties.Add(pTransPhase);
            pAssocProp.IdentityProperties.Add(pTransPartNum);
            pAssocProp.ReverseIdentityProperties.Add(pOutPhase);
            pAssocProp.ReverseIdentityProperties.Add(pOutPartNum);

            pOutClass.Properties.Add(pAssocProp);
            pSchema.Classes.Add(pOutClass);

            Class pTarClass = new Class("Tarrings", "");
            pOutClass.IsAbstract = false;

            pProp = new DataPropertyDefinition("id", "");
            pProp.DataType = DataType.DataType_Int64;
            pProp.Nullable = false;
            pTarClass.Properties.Add(pProp);
            pTarClass.IdentityProperties.Add(pProp);

            // Association property with multiple implicit identity properties.
            pAssocProp = new AssociationPropertyDefinition("Pole", "");
            pAssocProp.AssociatedClass = pPoleClass;
            pAssocProp.ReverseName = "Tar Applications";

            pTarClass.Properties.Add(pAssocProp);
            pSchema.Classes.Add(pTarClass);
                        
            // Create a network layer class
            NetworkLayerClass layer = new NetworkLayerClass("TestLayer", "A network test Layer class");
            // Add property to the layer class
            pProp = new DataPropertyDefinition("LayerPropertyOne", "A layer test property");
            pProp.DataType = DataType.DataType_String;
            pProp.Length = 50;
            pProp.Nullable = false;
            layer.Properties.Add(pProp);
            pSchema.Classes.Add(layer);

            // Create a network class
            NetworkClass network = new NetworkClass("TestNetwork", "A network test class");
            // Add property to the layer class
            pProp = new DataPropertyDefinition("NetworkPropertyOne", "A network test property");
            pProp.DataType = DataType.DataType_String;
            pProp.Length = 100;
            pProp.Nullable = false;
            network.Properties.Add(pProp);
            pSchema.Classes.Add(network);

            // Create a parent network node class
            NetworkNodeFeatureClass parentNetworkNode = new NetworkNodeFeatureClass("TestNetworkParentNode", "A parent network node test class");
            pSchema.Classes.Add(parentNetworkNode);

            // Create a network node class
            NetworkNodeFeatureClass networkNode = new NetworkNodeFeatureClass("TestNetworkNode", "A network node test class");
            // Add property to the layer class
            pProp = new DataPropertyDefinition("Speed", "A speed property");
            pProp.DataType = DataType.DataType_Double;
            pProp.Nullable = false;
            networkNode.Properties.Add(pProp);
            // Set the layer, network and cost properties
            pSchema.Classes.Add(networkNode);

            // Create a network link class
            NetworkLinkFeatureClass networkLink = new NetworkLinkFeatureClass("TestNetworkLink", "A network link test class");
            // Add property to the layer class
            pProp = new DataPropertyDefinition("Speed", "A speed property");
            pProp.DataType = DataType.DataType_Double;
            pProp.Nullable = false;
            networkLink.Properties.Add(pProp);
            pSchema.Classes.Add(networkLink);

            // Create a network layer class
            NetworkLayerClass layer2 = new NetworkLayerClass("TestLayer2", "A network test Layer class");
            layer2.BaseClass = layer;
            // Add property to the layer class
            pProp = new DataPropertyDefinition("LayerPropertyTwo", "A layer test property");
            pProp.DataType = DataType.DataType_String;
            pProp.Length = 52;
            pProp.Nullable = true;
            layer2.Properties.Add(pProp);
            pSchema.Classes.Add(layer2);

            // Create a network class
            NetworkClass network2 = new NetworkClass("TestNetwork2", "A network test class");
            network2.BaseClass = network;
            // Add property to the layer class
            pProp = new DataPropertyDefinition("NetworkPropertyTwo", "A network test property");
            pProp.DataType = DataType.DataType_String;
            pProp.Length = 102;
            pProp.Nullable = true;
            network2.Properties.Add(pProp);
            // Set the layer class
            network2.LayerClass = layer2;
            pSchema.Classes.Add(network2);

            // Create a parent network node class
            NetworkNodeFeatureClass parentNetworkNode2 = new NetworkNodeFeatureClass("TestNetworkParentNode2", "A parent network node test class");
            parentNetworkNode2.BaseClass = parentNetworkNode;
            // Set the layer, network and cost properties
            AssociationPropertyDefinition layerAssoc = new AssociationPropertyDefinition("Layer Association Prop", "The layer association prop");
            layerAssoc.AssociatedClass = layer2;
            layerAssoc.DeleteRule = DeleteRule.DeleteRule_Break;
            AssociationPropertyDefinition networkAssoc = new AssociationPropertyDefinition("Network Association Prop", "The network association prop");
            networkAssoc.AssociatedClass = network2;
            networkAssoc.ReverseName = "Primitive";
            parentNetworkNode2.LayerProperty = layerAssoc;
            parentNetworkNode2.NetworkProperty = networkAssoc;
            AssociationPropertyDefinition parentNodeFeatAssoc = new AssociationPropertyDefinition("Feature Association Prop", "The feature association prop");
            parentNodeFeatAssoc.AssociatedClass = pPoleClass;
            parentNetworkNode2.ReferencedFeatureProperty = parentNodeFeatAssoc;
            pSchema.Classes.Add(parentNetworkNode2);

            // Create a network node class
            NetworkNodeFeatureClass networkNode2 = new NetworkNodeFeatureClass("TestNetworkNode2", "A network node test class");
            networkNode2.BaseClass = networkNode;
            // Add property to the layer class
            pProp = new DataPropertyDefinition("Closed", "");
            pProp.DataType = DataType.DataType_Boolean;
            pProp.Nullable = false;
            networkNode2.Properties.Add(pProp);
            // Set the layer, network and cost properties
            layerAssoc = new AssociationPropertyDefinition("Layer Association Prop", "The layer association prop");
            layerAssoc.AssociatedClass = layer2;
            networkAssoc = new AssociationPropertyDefinition("Network Association Prop", "The network association prop");
            networkAssoc.AssociatedClass = network2;
            networkAssoc.LockCascade = true;
            networkNode2.LayerProperty = layerAssoc;
            networkNode2.NetworkProperty = networkAssoc;
            AssociationPropertyDefinition parentAssoc = new AssociationPropertyDefinition("Parent Association Prop", "The parent association prop");
            parentAssoc.AssociatedClass = parentNetworkNode2;
            parentAssoc.DeleteRule = DeleteRule.DeleteRule_Break;
            networkNode2.ParentNetworkFeatureProperty = parentAssoc;
            AssociationPropertyDefinition nodeFeatAssoc = new AssociationPropertyDefinition("Feature Association Prop", "The feature association prop");
            nodeFeatAssoc.AssociatedClass = pTransClass;
            networkNode2.ReferencedFeatureProperty = nodeFeatAssoc;
            pSchema.Classes.Add(networkNode2);

            // Create a network link class
            NetworkLinkFeatureClass networkLink2 = new NetworkLinkFeatureClass("TestNetworkLink2", "A network link test class");
            networkLink2.BaseClass = networkLink;
            // Add property to the layer class
            pProp = new DataPropertyDefinition("Material", "");
            pProp.DataType = DataType.DataType_String;
            pProp.Length = 52;
            pProp.Nullable = false;
            networkLink2.Properties.Add(pProp);
            // Set the network and cost properties
            networkLink2.NetworkProperty = networkAssoc;
            // Set the start and end nodes
            AssociationPropertyDefinition nodeAssocA = new AssociationPropertyDefinition("Network node Association Prop A", "The network node association prop");
            nodeAssocA.AssociatedClass = networkNode2;
            nodeAssocA.DeleteRule = DeleteRule.DeleteRule_Prevent;
            networkLink2.StartNodeProperty = nodeAssocA;
            AssociationPropertyDefinition nodeAssocB = new AssociationPropertyDefinition("Network node Association Prop B", "The network node association prop");
            nodeAssocB.AssociatedClass = parentNetworkNode2;
            nodeAssocB.LockCascade = true;
            nodeAssocB.ReverseName = "FromLink";
            networkLink2.EndNodeProperty = nodeAssocB;
            AssociationPropertyDefinition linkFeatAssoc = new AssociationPropertyDefinition("Feature Association Prop", "The feature association prop");
            linkFeatAssoc.AssociatedClass = pPlineClass;
            linkFeatAssoc.DeleteRule = DeleteRule.DeleteRule_Break;
            linkFeatAssoc.LockCascade = true;
            linkFeatAssoc.ReverseName = "ElecNet2";
            networkLink2.ReferencedFeatureProperty = linkFeatAssoc;
            pSchema.Classes.Add(networkLink2);

            NetworkClass network3 = new NetworkClass("TestNetwork3", "A network test class");
            network3.BaseClass = network2;
            // Add property to the layer class
            pProp = new DataPropertyDefinition("NetworkPropertyThree", "A network test property");
            pProp.DataType = DataType.DataType_String;
            pProp.Length = 103;
            pProp.Nullable = true;
            network3.Properties.Add(pProp);
            // Set the layer class
            network3.LayerClass = layer2;
            pSchema.Classes.Add(network3);

            return (pSchema);
        }

        private FeatureSchema createRefSchema(int idx, FeatureSchema pBaseSchema)
        {
            ClassDefinition pBaseClass = null;
            ClassDefinition pBaseOpClass = null;
            FeatureClass pClass = null;
            Class pOpClass = null;
            DataPropertyDefinition pBaseOpId = null;
            NetworkLayerClass baseLayer = null;
            NetworkClass baseNetwork = null;
            NetworkNodeFeatureClass baseParentNetworkNode = null;
            NetworkNodeFeatureClass baseNetworkNode = null;
            NetworkLinkFeatureClass baseNetworkLink = null;

            if (pBaseSchema != null)
            {
                pBaseClass = pBaseSchema.Classes["Reference Class"];
                pBaseOpClass = pBaseSchema.Classes["Object Property Class"];
                pBaseOpId = (DataPropertyDefinition)(pBaseOpClass.Properties[0]);
                baseLayer = (NetworkLayerClass)pBaseSchema.Classes["TestLayer"];
                baseNetwork = (NetworkClass)pBaseSchema.Classes["TestNetwork"];
                baseParentNetworkNode = (NetworkNodeFeatureClass)pBaseSchema.Classes["TestNetworkParentNode"];
                baseNetworkNode = (NetworkNodeFeatureClass)pBaseSchema.Classes["TestNetworkNode"];
                baseNetworkLink = (NetworkLinkFeatureClass)pBaseSchema.Classes["TestNetworkLink"];
            }

            FeatureSchema pSchema = new FeatureSchema("Reference Schema " + idx.ToString(), "");

            /* An object property class */
            pOpClass = new Class("Object Property Class", "");
            pOpClass.IsAbstract = false;

            DataPropertyDefinition pProp = new DataPropertyDefinition("Prop 1", "");
            pProp.DataType = DataType.DataType_Decimal;
            pProp.Precision = 10;
            pProp.Scale = 2;
            pProp.Nullable = false;
            pOpClass.Properties.Add(pProp);

            pSchema.Classes.Add(pOpClass);

            pClass = new FeatureClass("Reference Class", "");
            pClass.IsAbstract = false;
            if (pBaseClass != null)
                pClass.BaseClass = pBaseClass;

            pProp = new DataPropertyDefinition("Prop " + idx.ToString(), "");
            pProp.DataType = DataType.DataType_Int64;
            pProp.Nullable = false;
            pClass.Properties.Add(pProp);

            if (pBaseClass == null)
                pClass.IdentityProperties.Add(pProp);

            if (pBaseOpClass != null)
            {
                ObjectPropertyDefinition pObjProp = new ObjectPropertyDefinition("Object Property " + idx.ToString(), "");

                pObjProp.ObjectType = ObjectType.ObjectType_Collection;
                pObjProp.IdentityProperty = pBaseOpId;
                pObjProp.Class = pBaseOpClass;
                pClass.Properties.Add(pObjProp);
            }

            pSchema.Classes.Add(pClass);

            pClass = new FeatureClass("Concrete Class", "");
            pClass.IsAbstract = false;

            pProp = new DataPropertyDefinition("Prop " + idx.ToString(), "");
            pProp.DataType = DataType.DataType_Int64;
            pProp.Nullable = false;
            pClass.Properties.Add(pProp);
            pClass.IdentityProperties.Add(pProp);

            if (pBaseOpClass != null)
            {
                ObjectPropertyDefinition pObjProp = new ObjectPropertyDefinition("Object Property " + idx, "");

                pObjProp.ObjectType = ObjectType.ObjectType_Collection;
                pObjProp.IdentityProperty = pBaseOpId;
                pObjProp.Class = pBaseOpClass;
                pClass.Properties.Add(pObjProp);

                AssociationPropertyDefinition pAssocProp = new AssociationPropertyDefinition("Association Property " + idx, "");

                pAssocProp.AssociatedClass = pBaseClass;
                pClass.Properties.Add(pAssocProp);
            }

            pSchema.Classes.Add(pClass);

            // Create a network layer class
            NetworkLayerClass layer = new NetworkLayerClass("TestLayer", "A network test Layer class");
            if (pBaseSchema != null)
            {
                layer.BaseClass = baseLayer;
            }
            else
            {
                // Add property to the layer class
                pProp = new DataPropertyDefinition("LayerProperty" + idx.ToString(), "");
                pProp.DataType = DataType.DataType_String;
                pProp.Length = 50;
                pProp.Nullable = false;
                layer.Properties.Add(pProp);
                layer.IdentityProperties.Add(pProp);
            }
            pSchema.Classes.Add(layer);

            // Create a network class
            NetworkClass network = new NetworkClass("TestNetwork", "A network test class");
            if (pBaseSchema != null)
            {
                network.BaseClass = baseNetwork;
                network.LayerClass = baseLayer;
            }
            else
            {
                // Add property to the layer class
                pProp = new DataPropertyDefinition("NetworkProperty" + idx.ToString(), "");

                pProp.DataType = DataType.DataType_String;
                pProp.Length = 100;
                pProp.Nullable = false;
                network.Properties.Add(pProp);
                network.IdentityProperties.Add(pProp);
            }
            pSchema.Classes.Add(network);

            AssociationPropertyDefinition layerAssoc;
            AssociationPropertyDefinition networkAssoc = null;
            // Create a parent network node class
            NetworkNodeFeatureClass parentNetworkNode = new NetworkNodeFeatureClass("TestNetworkParentNode", "A parent network node test class");
            if (pBaseSchema != null)
            {
                parentNetworkNode.BaseClass = baseParentNetworkNode;
                // Set the layer, network and cost properties
                layerAssoc = new AssociationPropertyDefinition("Layer Association Prop", "The layer association prop");
                layerAssoc.AssociatedClass = baseLayer;
                networkAssoc = new AssociationPropertyDefinition("Network Association Prop", "The network association prop");
                networkAssoc.AssociatedClass = baseNetwork;
                parentNetworkNode.LayerProperty = layerAssoc;
                parentNetworkNode.NetworkProperty = networkAssoc;
            }
            pSchema.Classes.Add(parentNetworkNode);

            // Create a network node class
            NetworkNodeFeatureClass networkNode = new NetworkNodeFeatureClass("TestNetworkNode", "A network node test class");
            if (pBaseSchema != null)
            {
                networkNode.BaseClass = baseNetworkNode;
                // Set the layer, network 
                layerAssoc = new AssociationPropertyDefinition("Layer Association Prop", "The layer association prop");
                layerAssoc.AssociatedClass = baseLayer;
                networkAssoc = new AssociationPropertyDefinition("Network Association Prop", "The network association prop");
                networkAssoc.AssociatedClass = baseNetwork;
                networkNode.LayerProperty = layerAssoc;
                networkNode.NetworkProperty = networkAssoc;
                AssociationPropertyDefinition parentAssoc = new AssociationPropertyDefinition("Parent Association Prop", "The parent association prop");
                parentAssoc.AssociatedClass = baseParentNetworkNode;
                networkNode.ParentNetworkFeatureProperty = parentAssoc;
                AssociationPropertyDefinition nodeFeatAssoc = new AssociationPropertyDefinition("Feature Association Prop", "The feature association prop");
                nodeFeatAssoc.AssociatedClass = pBaseClass;
                networkNode.ReferencedFeatureProperty = nodeFeatAssoc;
            }
            else
            {
                // Add property to the layer class
                pProp = new DataPropertyDefinition("NodeProperty" + idx.ToString(), "");
                pProp.DataType = DataType.DataType_Double;
                pProp.Nullable = false;
                networkNode.Properties.Add(pProp);
                networkNode.IdentityProperties.Add(pProp);
            }
            pSchema.Classes.Add(networkNode);

            // Create a network link class
            NetworkLinkFeatureClass networkLink = new NetworkLinkFeatureClass("TestNetworkLink", "A network link test class");
            if (pBaseSchema != null)
            {
                networkLink.BaseClass = baseNetworkLink;
                // Set the network and cost properties
                networkLink.NetworkProperty = networkAssoc;
                // Set the start and end nodes
                AssociationPropertyDefinition nodeAssoc = new AssociationPropertyDefinition("Network node Association Prop", "The network node association prop");
                nodeAssoc.AssociatedClass = baseNetworkNode;
                networkLink.StartNodeProperty = nodeAssoc;
                networkLink.EndNodeProperty = nodeAssoc;
                AssociationPropertyDefinition linkFeatAssoc = new AssociationPropertyDefinition("Feature Association Prop", "The feature association prop");
                linkFeatAssoc.AssociatedClass = pBaseClass;
                networkLink.ReferencedFeatureProperty = linkFeatAssoc;
            }
            else
            {
                // Add property to the layer class
                pProp = new DataPropertyDefinition("LinkProperty" + idx.ToString(), "");
                pProp.DataType = DataType.DataType_Double;
                pProp.Nullable = false;
                networkLink.Properties.Add(pProp);
                networkLink.IdentityProperties.Add(pProp);
            }
            pSchema.Classes.Add(networkLink);

            return (pSchema);
        }

        private FeatureSchema createRefSchema(int idx)
        {
            return createRefSchema(idx, null);
        }
    }
}

