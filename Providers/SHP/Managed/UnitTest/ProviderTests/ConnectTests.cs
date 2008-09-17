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
using System.IO;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;

using unit_test.Framework;

using OSGeo.FDO.Connections;
using OSGeo.FDO.Commands.Schema;
using OSGeo.FDO.Commands;
using OSGeo.FDO.Schema;

namespace unit_test.ProviderTests
{
    class ConnectTests : BaseTestWithConnection
    {
        public ConnectTests(ShpTestProvider connectionProvider)
            : base(connectionProvider)
        { }

        public override void SetUp()
        { }

        public override void TearDown()
        { }

        public void Test_connect()
        {
            try
            {
                IConnection mConnection = base.ConnectionNew;

                //connect state testing before connection openning
                Debug.Assert(mConnection.ConnectionState == ConnectionState.ConnectionState_Closed, "connection state not closed");

                //connection openning
                mConnection.ConnectionString = @"DefaultFileLocation=..\..\..\..\TestData\Ontario\roads.shp";
                Debug.Assert(mConnection.ConnectionState == ConnectionState.ConnectionState_Closed, "connection state not closed");
                ConnectionState state = mConnection.Open();
                Debug.Assert(state == ConnectionState.ConnectionState_Open, "connection state not open");
                Debug.Assert(mConnection.ConnectionState == ConnectionState.ConnectionState_Open, "connection state not open");

                // Verify results of DescribeSchema:
                IDescribeSchema descSchema = (IDescribeSchema)mConnection.CreateCommand(CommandType.CommandType_DescribeSchema);
                FeatureSchemaCollection schemas = descSchema.Execute();
                Debug.Assert(schemas.Count == 1, "Expecting 1 schema");
                FeatureSchema schema = schemas[0];
                Debug.Assert(schema.Name == "Default", "Expecting 'Default' schema");
                Debug.Assert(schema.Description == "Default schema.", "Expecting 'Default schema.' as the schema description");
                ClassCollection classes = schema.Classes;
                Debug.Assert(classes.Count == 1, "Expecting 1 class");
                ClassDefinition classDef = classes[0];
                Debug.Assert(classDef.Name == "roads", "Expecting a specific class, got wrong one");

                mConnection.Close();
                Debug.Assert(mConnection.ConnectionState == ConnectionState.ConnectionState_Closed, "connection state not closed");

                Console.WriteLine("Test_connection runs successfully !");
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

        public void Test_dual_connect()
        {
            try
            {
                IConnection connection1;
                ConnectionState state1;
                IConnection connection2;
                ConnectionState state2;
                IDescribeSchema describe1;
                FeatureSchemaCollection schemas1;
                IDescribeSchema describe2;
                FeatureSchemaCollection schemas2;
                FeatureSchema schema1;
                ClassCollection classes1;
                FeatureSchema schema2;
                ClassCollection classes2;
                ClassDefinition class1;
                ClassDefinition class2;

                // create one connection
                connection1 = base.ConnectionNew;
                Debug.Assert(connection1.ConnectionState == ConnectionState.ConnectionState_Closed, "connection state not closed");
                state1 = connection1.Open();
                Debug.Assert(state1 == ConnectionState.ConnectionState_Open, "connection state not open");
                Debug.Assert(connection1.ConnectionState == ConnectionState.ConnectionState_Open, "connection state not open");

                // create a second connection for the same default file location
                connection2 = base.ConnectionNew;
                Debug.Assert(connection2.ConnectionState == ConnectionState.ConnectionState_Closed, "connection state not closed");
                state2 = connection2.Open();
                Debug.Assert(state2 == ConnectionState.ConnectionState_Open, "connection state not open");
                Debug.Assert(connection2.ConnectionState == ConnectionState.ConnectionState_Open, "connection state not open");

                // try DescribeSchema on the first
                describe1 = (IDescribeSchema)connection1.CreateCommand(CommandType.CommandType_DescribeSchema);
                schemas1 = describe1.Execute();                
                Debug.Assert(schemas1.Count == 1, "Expecting 1 schema");

                // try DescribeSchema on the second...
                describe2 = (IDescribeSchema)connection2.CreateCommand(CommandType.CommandType_DescribeSchema);
                schemas2 = describe2.Execute();
                Debug.Assert(schemas2.Count == 1, "Expecting 1 schema");

                // compare classes
                schema1 = schemas1[0];
                classes1 = schema1.Classes;
                schema2 = schemas2[0];
                classes2 = schema2.Classes;
                Debug.Assert(schema1.Name == schema2.Name, "Expecting same schema name");
                for (int i = 0; i < classes1.Count; i++)
                {
                    class1 = classes1[i];
                    class2 = classes2[i];
                    Debug.Assert(class1.Name == class2.Name, "schema's differ");

                    connection1.Close();
                    Debug.Assert(connection1.ConnectionState == ConnectionState.ConnectionState_Closed, "connection state not closed");
                    connection2.Close();
                    Debug.Assert(connection2.ConnectionState == ConnectionState.ConnectionState_Closed, "connection state not closed");
                }

                Console.WriteLine("Test_dual_connect runs successfully !");
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

        public void Test_no_slash()
        {
            try
            {
                // try with just trailing normal slashes
                IConnection mConnection = base.ConnectionNew;
                Debug.Assert(mConnection.ConnectionState == ConnectionState.ConnectionState_Closed, "connection state not closed");
                mConnection.ConnectionString = @"DefaultFileLocation=..\..\..\..\TestData\Ontario";
                Debug.Assert(mConnection.ConnectionState == ConnectionState.ConnectionState_Closed, "connection state not closed");
                ConnectionState state = mConnection.Open();
                Debug.Assert(state == ConnectionState.ConnectionState_Open, "connection state not open");
                Debug.Assert(mConnection.ConnectionState == ConnectionState.ConnectionState_Open, "connection state not open");

                // Verify results of DescribeSchema:
                IDescribeSchema descSchema = (IDescribeSchema)mConnection.CreateCommand(CommandType.CommandType_DescribeSchema);
                FeatureSchemaCollection schemas = descSchema.Execute();
                Debug.Assert(schemas.Count == 1, "Expecting 1 schema");
                FeatureSchema schema = schemas[0];
                Debug.Assert(schema.Name == "Default", "Expecting 'Default' schema");
                ClassCollection classes = schema.Classes;
                Debug.Assert(classes.Count > 3, "Expecting more than 3 classes");

                mConnection.Close();
                Debug.Assert(mConnection.ConnectionState == ConnectionState.ConnectionState_Closed, "connection state not closed");

                Console.WriteLine("Test_no_slash runs successfully !");
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

        public void Test_back_slash()
        {
            try
            {
                // try with trailing cross-platform slashes
                IConnection mConnection = base.ConnectionNew;
                Debug.Assert(mConnection.ConnectionState == ConnectionState.ConnectionState_Closed, "connection state not closed");
                mConnection.ConnectionString = @"DefaultFileLocation=..\..\..\..\TestData\Ontario/";
                Debug.Assert(mConnection.ConnectionState == ConnectionState.ConnectionState_Closed, "connection state not closed");
                ConnectionState state = mConnection.Open();
                Debug.Assert(state == ConnectionState.ConnectionState_Open, "connection state not open");
                Debug.Assert(mConnection.ConnectionState == ConnectionState.ConnectionState_Open, "connection state not open");

                // Verify results of DescribeSchema:
                IDescribeSchema descSchema = (IDescribeSchema)mConnection.CreateCommand(CommandType.CommandType_DescribeSchema);
                FeatureSchemaCollection schemas = descSchema.Execute();
                Debug.Assert(schemas.Count == 1, "Expecting 1 schema");
                FeatureSchema schema = schemas[0];
                Debug.Assert(schema.Name == "Default", "Expecting 'Default' schema");
                ClassCollection classes = schema.Classes;
                Debug.Assert(classes.Count > 3, "Expecting more than 3 classes");

                mConnection.Close();
                Debug.Assert(mConnection.ConnectionState == ConnectionState.ConnectionState_Closed, "connection state not closed");

                Console.WriteLine("Test_back_slash runs successfully !");
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

        public void Test_double_slash()
        {
            try
            {
                // try with trailing cross-platform slashes
                IConnection mConnection = base.ConnectionNew;
                Debug.Assert(mConnection.ConnectionState == ConnectionState.ConnectionState_Closed, "connection state not closed");
                mConnection.ConnectionString = @"DefaultFileLocation=..\\..\\..\\..\\TestData\\Ontario\\";
                Debug.Assert(mConnection.ConnectionState == ConnectionState.ConnectionState_Closed, "connection state not closed");
                ConnectionState state = mConnection.Open();
                Debug.Assert(state == ConnectionState.ConnectionState_Open, "connection state not open");
                Debug.Assert(mConnection.ConnectionState == ConnectionState.ConnectionState_Open, "connection state not open");

                // Verify results of DescribeSchema:
                IDescribeSchema descSchema = (IDescribeSchema)mConnection.CreateCommand(CommandType.CommandType_DescribeSchema);
                FeatureSchemaCollection schemas = descSchema.Execute();
                Debug.Assert(schemas.Count == 1, "Expecting 1 schema");
                FeatureSchema schema = schemas[0];
                Debug.Assert(schema.Name == "Default", "Expecting 'Default' schema");
                ClassCollection classes = schema.Classes;
                Debug.Assert(classes.Count == 6, "Expecting 6 classes");

                mConnection.Close();
                Debug.Assert(mConnection.ConnectionState == ConnectionState.ConnectionState_Closed, "connection state not closed");

                Console.WriteLine("Test_double_slash runs successfully !");
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
    }
}
