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
    class SpatialFilterTests: BaseTestWithConnection
    {
        public SpatialFilterTests(ShpTestProvider connectionProvider)
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
        const bool VERBOSE = true ;

        public void Test_SelectAll ()
        {
            string  class_name = "roads" ;

            FgfGeometryFactory gf = new FgfGeometryFactory();
                    
            ISelect select = (ISelect)base.ConnectionInitialized.CreateCommand (CommandType.CommandType_Select);
            select.SetFeatureClassName( class_name );
            IFeatureReader reader = select.Execute ();

            int count = 0;
            while (reader.ReadNext ())
            {
                count++;
                reader.GetInt32 ("FeatId");
                
                IGeometry   geom = gf.CreateGeometryFromFgf(reader.GetGeometry ("Geometry"));
                IEnvelope   ext = geom.Envelope ;

                if (VERBOSE)
                {
                    Console.WriteLine("--FeatId " + ((int)(count-1)).ToString() );
                    Console.WriteLine("  xxx   extents , (" + ext.MinX.ToString() + "," + ext.MinY.ToString() +
                        ") (" + ext.MaxX.ToString() + "," + ext.MaxY.ToString() + ")") ;
                }
            }
        }
    } //class
}//namespace
