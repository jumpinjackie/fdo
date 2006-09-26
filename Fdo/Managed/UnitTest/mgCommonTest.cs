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

using OSGeo.Common;

namespace Fdo_Test
{
    /// <summary>
    /// Summary description for mgCommonTest.
    /// </summary>
    public class mgCommonTest
    {
        public mgCommonTest()
        {
            Console.WriteLine("start mgCommonTest");
        }

        public void testTypes()
        {
            Console.WriteLine("start mgCommonTest.testTypes");

            // ...now check that the range of values supported work
            System.Byte b = 0;
            Debug.Assert(b == 0);
            b = 0xff;						// UCHAR_MAX
            Debug.Assert(b == 255);

            System.Int16 i16 = -32768;		// SHRT_MIN
            Debug.Assert(i16 == -32768);
            i16 = 32767;					// SHRT_MAX
            Debug.Assert(i16 == 32767);

            System.Int32 i32 = (-2147483647 - 1);	// INT_MIN/LONG_MIN
            Debug.Assert(i32 == (-2147483647 - 1));
            i32 = 2147483647;					// INT_MAX/LONG_MAX
            Debug.Assert(i32 == 2147483647);

            //Console.WriteLine("finish mgCommonTest.testTypes");
            
        }

        public void testDictionary()
        {
            Console.WriteLine("start mgCommonTest.testDictionary");

            bool failed = false;
            DictionaryElementCollection dictionary = new DictionaryElementCollection();

            // Add some items where name is Roman and value is Indo-Arabic
            dictionary.Add(new DictionaryElement("CM", "900"));
            dictionary.Add(new DictionaryElement("MCMLXXX", "1980"));
            dictionary.Add(new DictionaryElement("MMIII", "2003"));
            dictionary.Insert(1, new DictionaryElement("MMIV", "2004"));

            // Get and find existing items
            Debug.Assert(dictionary["MCMLXXX"].Value == "1980");
            Debug.Assert(dictionary["MMIV"].Value == "2004");

            // Get non-existing item, throws exception
            failed = false;
            DictionaryElement notexist = null;
            try
            {
                notexist = dictionary["MCMIX"];
            }
            catch (OSGeo.Common.Exception)
            {
                failed = true;
            }
            Debug.Assert(failed);

            // Find non-existing item, returns null
            Debug.Assert(notexist == null);

            // Replace an item and then try to retrieve it.
            dictionary[1] = new DictionaryElement("MCMIX", "1909");
            Debug.Assert(dictionary["MCMIX"].Value == "1909");

            // Add duplicate name		
            failed = false;
            try
            {
                dictionary.Add(new DictionaryElement("MCMLXXX", "1980"));
            }
            catch (OSGeo.Common.Exception)
            {
                failed = true;
            }
            Debug.Assert(failed);

            // Insert duplicate name
            failed = false;
            try
            {
                dictionary.Insert(0, new DictionaryElement("CM", "900"));
            }
            catch (OSGeo.Common.Exception)
            {
                failed = true;
            }
            Debug.Assert(failed);

            // Replace with duplicate name.
            failed = false;
            try
            {
                dictionary[0] = new DictionaryElement("MMIII", "2003");
            }
            catch (OSGeo.Common.Exception)
            {
                failed = true;
            }
            Debug.Assert(failed);

            // This replace should succeed since names of new item and
            // item being replaced are the same.
            dictionary[0] = new DictionaryElement("CM", "Nine Hundred");

            // Verify that previous setitem worked.
            Debug.Assert(dictionary["CM"].Value == "Nine Hundred");
        }
    }
}

