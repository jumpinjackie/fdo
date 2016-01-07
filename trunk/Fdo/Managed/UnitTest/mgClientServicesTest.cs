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
using OSGeo.FDO.ClientServices;

namespace Fdo_Test
{
	/// <summary>
	/// Summary description for mgClientServicesTest.
	/// </summary>
	public class mgClientServicesTest
	{
        public class CSTestData
        {
            public string name;
            public string displayname;
            public string description;
            public string version;
            public string fdoVersion;
            public string libraryPath;
            public bool isManaged;
        }

		public mgClientServicesTest()
		{
            Console.WriteLine("start mgClientServicesTest");
		}

        public void testServices()
        {
            // Prepare the test data
            CSTestData dt1 = new CSTestData();
            dt1.name = "Autodesk.Oracle.3.2";
            dt1.displayname = "Oracle";
            dt1.description = "Oracle Spatial FDO Provider";
            dt1.version = "3.2.0.0";
            dt1.fdoVersion = "3.2.0.0";
            dt1.libraryPath = "FdoRdbms.dll";
            dt1.isManaged = false;

            CSTestData dt2 = new CSTestData();
            dt2.name = "TestData2";
            dt2.displayname = "TestData2";
            dt2.description = "TestDescription2";
            dt2.version = "3.2.0.0";
            dt2.fdoVersion = "3.2.0.0";
            dt2.libraryPath = "c:\\Test2\\test2.dll";
            dt2.isManaged = true;

            CSTestData dt3 = new CSTestData();
            dt3.name = "TestData3";
            dt3.displayname = "TestData3";
            dt3.description = "TestDescription3";
            dt3.version = "3.2.0.0";
            dt3.fdoVersion = "3.2.0.0";
            dt3.libraryPath = "c:\\Test3\\test.dll";
            dt3.isManaged = true;

            // Enter test
            Console.WriteLine("start mgClientServicesTest.testServices");

            OSGeo.FDO.IProviderRegistry registry = FeatureAccessManager.GetProviderRegistry();
            Debug.Assert(registry != null);

            ProviderCollection collection = registry.GetProviders();
            int initialCount = collection.Count;
            registry.RegisterProvider(dt1.name, dt1.displayname, dt1.description, dt1.version, dt1.fdoVersion, dt1.libraryPath, dt1.isManaged);
            registry.RegisterProvider(dt2.name, dt2.displayname, dt2.description, dt2.version, dt2.fdoVersion, dt2.libraryPath, dt2.isManaged);
            registry.RegisterProvider(dt3.name, dt3.displayname, dt3.description, dt3.version, dt3.fdoVersion, dt3.libraryPath, dt3.isManaged);

            collection = registry.GetProviders();
            Provider provider = collection[collection.IndexOf(dt1.name)];
            Debug.Assert(provider != null);

            Debug.Assert(String.Compare(dt1.name, provider.Name) == 0);
            Debug.Assert(String.Compare(dt1.displayname, provider.DisplayName) == 0);
            Debug.Assert(String.Compare(dt1.description, provider.Description) == 0);
            Debug.Assert(String.Compare(dt1.version, provider.Version) == 0);
            Debug.Assert(String.Compare(dt1.fdoVersion, provider.FeatureDataObjectsVersion) == 0);
            Debug.Assert(String.Compare(dt1.libraryPath, provider.LibraryPath) == 0);
            Debug.Assert(dt1.isManaged == provider.IsManaged);

            OSGeo.FDO.IConnectionManager connectionMgr = FeatureAccessManager.GetConnectionManager();
            Debug.Assert(connectionMgr != null);

            registry.UnregisterProvider(dt1.name);
            registry.UnregisterProvider(dt2.name);
            registry.UnregisterProvider(dt3.name);

            //Console.WriteLine("finish mgClientServicesTest.testServices");
            
        }
	}
}

