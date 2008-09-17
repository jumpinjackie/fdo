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
    class FilterTests : BaseTestWithConnection
    {
        public FilterTests(ShpTestProvider connectionProvider)
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

        const int NUM_RUNS = 1000;

        public void Test_equal()
        {
            try
            {
                ISelect select = (ISelect)base.ConnectionInitialized.CreateCommand(CommandType.CommandType_Select);
                select.SetFeatureClassName("lakes");
                select.SetFilter("FeatId = 42");

                IFeatureReader reader = select.Execute();
                int count = 0;
                while (reader.ReadNext())
                {
                    count++;
                    reader.GetInt32("FeatId");
                    if (!reader.IsNull("AREA"))
                        reader.GetDouble("AREA");
                    if (!reader.IsNull("PERIMETER"))
                        reader.GetDouble("PERIMETER");
                    if (!reader.IsNull("DRAINAGE_"))
                        reader.GetDouble("DRAINAGE_");
                    if (!reader.IsNull("DRAINAGE_I"))
                        reader.GetDouble("DRAINAGE_I");
                    if (!reader.IsNull("HOTLINK"))
                        reader.GetString("HOTLINK");
                    if (!reader.IsNull("Geometry"))
                        reader.GetGeometry("Geometry");
                }
                Debug.Assert(1 == count, "not 1 feature selected");
            }
            catch (OSGeo.FDO.Common.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
        }

        public void Test_computed()
        {
            try
            {
                ISelect select = (ISelect)base.ConnectionInitialized.CreateCommand(CommandType.CommandType_Select);
                select.SetFeatureClassName("lakes");
                Expression arg = Expression.Parse("PERIMETER / 3.14159 / 2");
                ComputedIdentifier radius = new ComputedIdentifier("radius", arg);
                Identifier id = new Identifier("FeatId");
                IdentifierCollection ids = select.PropertyNames;
                ids.Add(id);
                ids.Add(radius);
                select.SetFilter("AREA > 0.75 * 3.14159 * (radius * radius)");
                IFeatureReader reader = select.Execute();
                int count = 0;
                while (reader.ReadNext())
                {
                    count++;
                    reader.GetInt32("FeatId");
                    reader.GetDouble("radius");
                }
                Debug.Assert(1 == count, "not 1 feature selected");
            }
            catch (OSGeo.FDO.Common.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
        }

        // FeatId query optimizer tests
        protected int Featid_roads_query_test(string query, int numRuns)
        {
            int count = 0;

            ISelect select = (ISelect)base.ConnectionInitialized.CreateCommand(CommandType.CommandType_Select);
            select.SetFeatureClassName("roads");
            select.SetFilter(query);

            // Memory leak!
            for (int i = 0; i < numRuns; i++)
            {
                IFeatureReader reader = select.Execute();
                while (reader.ReadNext())
                {
                    count++;
                    int fn = reader.GetInt32("FeatId");
                }
            }

            return count;
        }

        /////////////////////////////////////////////////////////////////////////////////////////
        void Test_featid_optimizer_tests()
        {
            try
            {
                int count = 0;
                string query;

                query = "FeatId in (41, 42, 43) and not FeatId=42";
                count = Featid_roads_query_test(query, 1);
                Debug.Assert(2 == count, "1. not 2 feature selected");

                query = "not FeatId in (41, 42, 43)";
                count = Featid_roads_query_test(query, 1);
                Debug.Assert(1085 == count, "2. not 1085 feature selected");

                query = "FeatId=1 or FeatId=2 and FeatId in (2,3)";
                count = Featid_roads_query_test(query, 1);
                Debug.Assert(2 == count, "3. not 2 feature selected");
            }
            catch (OSGeo.FDO.Common.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
        }


        /////////////////////////////////////////////////////////////////////////////////////////


        public void Test_featid_performance_tests()
        {
            try
            {
                int count = 0;
                string query;

#if _WIN32       
        DateTime start = DateTime.Now;
#endif

                // This is the max featId, should return 1 row.
                query = "FeatId = 1087";
                count = Featid_roads_query_test(query, NUM_RUNS);
                Debug.Assert(NUM_RUNS == count, "P. not 2 feature selected");

#if _WIN32
        DateTime finish = DateTime.Now;
        TimeSpan WholeSpan = finish.Subtract(start);
        Console.WriteLine(WholeSpan.Seconds.ToString() + " seconds\n");
#endif
            }
            catch (OSGeo.FDO.Common.Exception ex)
            {
                Debug.Fail(ex.Message);
            }
        }

    }
}
