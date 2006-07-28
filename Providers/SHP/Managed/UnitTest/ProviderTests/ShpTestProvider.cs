using System;
using System.Collections.Generic;
using System.Text;

using OSGeo.FDO;
using OSGeo.FDO.ClientServices;
using OSGeo.FDO.Connections;

using unit_test.Framework;

namespace unit_test.ProviderTests
{
    class ShpTestProvider : IConnectionProvider
    {
        private string location = string.Empty;

        public ShpTestProvider()
        {
            this.location = @"..\..\..\..\TestData\Sheboygan\";
        }

        public ShpTestProvider(string location)
        {
            this.location = location;
        }

        public IConnection GetConnection()
        {
            IConnectionManager connectionManager = FeatureAccessManager.GetConnectionManager();
            IConnection conn = connectionManager.CreateConnection("OSGeo.SHP.3.2");
            conn.ConnectionString = @"DefaultFileLocation=" + location;
            return conn;
        }

        public string GetLocation()
        {
            return this.location ;
        }
    }
}
