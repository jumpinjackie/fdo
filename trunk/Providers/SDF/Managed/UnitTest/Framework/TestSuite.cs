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
using System.Reflection;

namespace unit_test.Framework
{
    class TestSuite
    {
        private List<BaseTest> testList;

        public TestSuite()
        {
            testList = new List<BaseTest>();
        }

        public void AddTest(BaseTest test)
        {
            testList.Add(test);
        }

        public void Run()
        {
            foreach (BaseTest obj in testList)
            {
                InvokeTests(obj.GetType(), obj);
            }
        }

        private void InvokeTests(Type type, BaseTest obj)
        {
            Console.WriteLine("**************** Begin testing " + type.Name + " ****************\n");
            // Invoke "SetUp" to initialize necessary variables
            //MethodInfo setupMethod = type.GetMethod("SetUp");
            //setupMethod.Invoke(obj, new Object[] { });

            MethodInfo[] methodArray = type.GetMethods(BindingFlags.Public | BindingFlags.Instance | BindingFlags.InvokeMethod);
            foreach (MethodInfo method in methodArray)
            {
                if (method.Name.StartsWith("Test_"))
                {
                    Console.WriteLine("-------- " + method.Name + " --------\n");
                    method.Invoke(obj, new Object[] { });
                }
            }

            // Invoke "TearDown" to release related resources
            MethodInfo teardownMethod = type.GetMethod("TearDown");
            teardownMethod.Invoke(obj, new Object[] { });
            Console.WriteLine("**************** End testing " + type.Name + " ****************\n\n");
        }
    }
}

