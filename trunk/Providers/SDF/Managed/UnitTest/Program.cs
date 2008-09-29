using System;
using System.Collections.Generic;
using System.Text;

using unit_test.Framework;
using unit_test.ProviderTests;

namespace unit_test
{
    class Program
    {
        static void Main(string[] args)
        {
            try
            {
                TestSuite suite = new TestSuite();

                suite.AddTest(new SchemaTests(new TestProvider(@"..\..\..\..\TestData\")));

                suite.Run();

                Console.WriteLine("\n\nAll testes complete!");
                Console.Read();
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
                Console.Read();
            }
        }
    }
}
