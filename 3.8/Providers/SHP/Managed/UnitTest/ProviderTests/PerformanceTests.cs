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
using OSGeo.FDO.Commands.Feature;
using OSGeo.FDO.Geometry;

using unit_test.Framework;

namespace unit_test.ProviderTests
{
    class PerformanceTests: BaseTestWithConnection
    {
        public PerformanceTests(ShpTestProvider connectionProvider)
            : base(connectionProvider)
        { }

        public override void SetUp()
        {
            base.SetUp();
        }

        public override void TearDown()
        {
            base.TearDown();
        }

        public void Test_select ()
        {
            DateTime start ;
            DateTime finish;
            int count;
            try
            {
                start = DateTime.Now;
                ISelect select = (ISelect)base.ConnectionInitialized.CreateCommand (CommandType.CommandType_Select);
                select.SetFeatureClassName ("plss24");
                IFeatureReader reader = select.Execute ();
                count = 0;
                while (reader.ReadNext ())
                {
                    //read Geometry
                    int afterLen = reader.GetGeometry ("Geometry").Length ;
                    Byte [] after = new Byte[afterLen] ;
                    after = reader.GetGeometry ("Geometry") ;
                    
                    count++;
                }
                unit_test_assert ("wrong number of plss24 features selected", 55772 == count) ;

                finish = DateTime.Now ;
#if DEBUG
                TimeSpan clsspan = finish.Subtract(start);
                Console.WriteLine(count + " features in " + (double)clsspan.Ticks / TimeSpan.TicksPerSecond + " seconds");   
#endif
            }
            catch (OSGeo.FDO.Common.Exception ex)
            {
                ShpTests.Fail (ex);
            }
        }
         
    }
}
