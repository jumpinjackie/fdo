using System;
using System.Collections.Generic;
using System.Diagnostics;

using unit_test.Framework;

using OSGeo.FDO.Connections;
using OSGeo.FDO.Commands;
using OSGeo.FDO.Commands.Schema;
using OSGeo.FDO.Schema;
using OSGeo.FDO.Commands.Feature;

namespace unit_test.ProviderTests
{
    class BigPerformanceTests : BaseTestWithConnection
    {
        public BigPerformanceTests(ShpTestProvider connectionProvider)
            : base(connectionProvider)
        {
        }

        public void Test_select()
        {
            try
            {
                DateTime start = DateTime.Now;
                DateTime restart;
                DateTime finish;
                int count;
                IDescribeSchema describe = (IDescribeSchema)base.ConnectionInitialized.CreateCommand(CommandType.CommandType_DescribeSchema);
                FeatureSchemaCollection schemas = describe.Execute();
                for (int i = 0; i < schemas.Count; i++)
                {
                    FeatureSchema schema = schemas[i];
                    ClassCollection classes = schema.Classes;
                    ISelect select = (ISelect)base.ConnectionInitialized.CreateCommand(CommandType.CommandType_Select);
                    for (int j = 0; j < classes.Count; j++)
                    {
                        ClassDefinition cls = classes[j];
                        restart = DateTime.Now;
                        select.SetFeatureClassName(cls.Name);
                        IFeatureReader reader = select.Execute();
                        count = 0;
                        while (reader.ReadNext())
                        {
                            byte[] geometry = reader.GetGeometry("Geometry");
                            count++;
                        }
                        finish = DateTime.Now;
#if DEBUG
                        TimeSpan clsspan = finish.Subtract(restart);
                        Console.WriteLine(count.ToString() + " features from class " + cls.Name + " in " + clsspan.Seconds.ToString() + " seconds");
#endif
                    }
                }
                finish = DateTime.Now;
#if DEBUG
                TimeSpan WholeSpan = finish.Subtract(start);
                Console.WriteLine(WholeSpan.Seconds.ToString() + " seconds\n" );
#endif
            }
            catch (OSGeo.Common.Exception ex)
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
