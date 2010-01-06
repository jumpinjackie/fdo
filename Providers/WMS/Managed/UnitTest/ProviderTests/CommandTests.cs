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

using OSGeo.FDO;
using OSGeo.FDO.Connections;
using OSGeo.FDO.Commands.Schema;
using OSGeo.FDO.Commands;
using OSGeo.FDO.Schema;
using OSGeo.FDO.ClientServices;

namespace unit_test.ProviderTests
{
    class CommandTests : BaseTest
    {
        public CommandTests()
        { }

        public override void SetUp()
        { }

        public override void TearDown()
        { }

        public void Test_commands()
        {
            try
            {
                IConnectionManager connectionManager = FeatureAccessManager.GetConnectionManager();
                IConnection mConnection = connectionManager.CreateConnection("OSGeo.WMS.3.3");
                mConnection.ConnectionString = @"FeatureServer=http://www2.dmsolutions.ca/cgi-bin/mswms_gmap";

                // connect state testing before connection openning
                Debug.Assert(mConnection.ConnectionState == ConnectionState.ConnectionState_Closed, "connection state not closed");

                // connection opening
                ConnectionState state = mConnection.Open();
                Debug.Assert(state == ConnectionState.ConnectionState_Open, "connection state not open");
                Debug.Assert(mConnection.ConnectionState == ConnectionState.ConnectionState_Open, "connection state not open");

                // Verify results of Command:
                ICommand command = mConnection.CreateCommand((OSGeo.FDO.Commands.CommandType)OSGeo.FDO.Providers.WMS.CommandType.CommandType_GetImageFormats);
                OSGeo.FDO.Providers.WMS.GetImageFormatsCommand formatsCmd = new OSGeo.FDO.Providers.WMS.GetImageFormatsCommand(command, false);
                OSGeo.FDO.Common.StringCollection strColl = formatsCmd.Execute();

                for (int i = 0; i < strColl.Count; i++)
                {
                    OSGeo.FDO.Common.StringElement strElem = strColl.get_Item(i);
                    string strVal = strElem.String;
                    Console.WriteLine(strVal);
                }

                // connection closing
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
    }
}
