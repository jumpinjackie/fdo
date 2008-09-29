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

using OSGeo.FDO;
using OSGeo.FDO.ClientServices;
using OSGeo.FDO.Connections;

using unit_test.Framework;

namespace unit_test.ProviderTests
{
    class TestProvider : IConnectionProvider
    {
        private string location = string.Empty;

        public TestProvider()
        {
            this.location = @"..\..\..\..\TestData\";
        }

        public TestProvider(string location)
        {
            this.location = location;
        }

        public IConnection GetConnection()
        {
            IConnectionManager connectionManager = FeatureAccessManager.GetConnectionManager();
            IConnection conn = connectionManager.CreateConnection("OSGeo.SDF.3.3");
            conn.ConnectionString = @"DefaultFileLocation=" + location;
            return conn;
        }

        public string GetLocation()
        {
            return this.location ;
        }
    }
}
