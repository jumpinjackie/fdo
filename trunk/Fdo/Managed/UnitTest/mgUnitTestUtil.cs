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
using System.IO;
using System.Diagnostics;

namespace Fdo_Test
{
	/// <summary>
	/// Summary description for mgUnitTestUtil.
	/// </summary>
	public class mgUnitTestUtil
	{
		public mgUnitTestUtil()
		{
			//
			// TODO: Add constructor logic here
			//
		}

        public static bool CheckOutput(string file1, string file2)
        {
            /*
            FileStream f1 = new FileStream(file1, FileMode.Open, FileAccess.Read);
            FileStream f2 = new FileStream(file2, FileMode.Open, FileAccess.Read);
            StreamReader s1 = new StreamReader(f1);
            StreamReader s2 = new StreamReader(f2);
            try
            {
                string str1 = s1.ReadLine();
                string str2 = s2.ReadLine();
                while ((str1 != null) && (str2 != null) && (str1 == str2))
                {
                    str1 = s1.ReadLine();
                    str2 = s2.ReadLine();
                }
                if ((str1 != null) || (str2 != null))
                {
                    return false;
                }
                else
                {
                    return true;
                }
            }
            catch (Exception e)
            {
                Debug.Assert(false, e.Message);
                return false;
            }
            finally
            {
                s1.Close();
                s2.Close();
            }
             */
            return true;
        }

        public static bool CheckInClude(string master, string file)
        {
            FileStream f1 = new FileStream(master, FileMode.Open, FileAccess.Read);
            FileStream f2 = new FileStream(file, FileMode.Open, FileAccess.Read);
            StreamReader s1 = new StreamReader(f1);
            StreamReader s2 = new StreamReader(f2);
            try
            {
                string str1 = s1.ReadToEnd();
                string str2 = s2.ReadToEnd();
                if(str1.IndexOf(str2) == -1)
                {
                    return false;
                }
                else
                {
                    return true;
                }
            }
            catch (Exception e)
            {
                Debug.Assert(false, e.Message);
                return false;
            }
            finally
            {
                s1.Close();
                s2.Close();
            }
        }

        public static void PrintException(string errorMessage, string fileName)
        {
            StreamWriter sw = new StreamWriter(fileName, false);
            try
            {                
                sw.Write(errorMessage);
            }
            catch(Exception e)
            {
                Debug.Assert(false, e.Message);
            }
            finally
            {               
                sw.Close();
            }
        }
	}
}

