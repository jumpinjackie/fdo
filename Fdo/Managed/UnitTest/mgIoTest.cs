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
using System.Text;
using OSGeo.Common.Io;

#pragma warning disable

namespace Fdo_Test
{
    /// <summary>
    /// Summary description for mgIoTest.
    /// </summary>
    public class mgIoTest
    {
        private const int LINE_COUNT = 5000;

        public mgIoTest()
        {
            Console.WriteLine("start mgIoTest");
        }

        public void setUp()
        {
            Console.WriteLine("start mgIoTest.setUp");

            // Create a file used for various file tests.
            IoFileStream fileStream = new IoFileStream("c:\\testFile.txt", "w");

            fileStream.Write((new ASCIIEncoding()).GetBytes("test file contents"), 18 );
		
            //Console.WriteLine("finish mgIoTest.setUp");
            
        }

        public void testStreams()
        {
            Console.WriteLine("start mgIoTest.testStreams");

            try 
            {
                IoMemoryStream memStream1 = new IoMemoryStream();
                IoMemoryStream memStream2 = new IoMemoryStream();
                IoMemoryStream memStream3 = new IoMemoryStream();
                IoFileStream fileStream1 = new IoFileStream("c:\\testStream1.txt", "w+" );
                IoFileStream fileStream2 = new IoFileStream("c:\\testStream2.txt", "w+" );

                populateStream(memStream1);
                //        vldStream(memStream1);
                bufferedWrite( memStream1, fileStream1);
                vldStream(fileStream1);
                streamedWrite( fileStream1, memStream2 );
                vldStream(memStream2);

                streamedWrite( memStream2, fileStream2 );
                vldStream(fileStream2);
                bufferedWrite( fileStream2, memStream3 );
                vldStream(memStream3);

                gisArrayRead(memStream3);
            }
            catch (OSGeo.Common.Exception e)
            {
                Debug.Assert(false, e.Message);
            }
            catch(System.Exception e)
            {
                Debug.Assert(false, e.Message);
            }
			
            //Console.WriteLine("finish mgIoTest.testStreams");
            
        }

        public void testFileOpen()
        {
            Console.WriteLine("start mgIoTest.testFileOpen");

            try 
            {
                IoFileStream fileStream;
		
                // Try to open non-existent file for read, should fail.		
                bool bFailed = false;
                try 
                {
                    fileStream = new IoFileStream("c:\\testNotExists", "r" );
                }
                catch
                {
                    bFailed = true;                    
                }
                Debug.Assert(bFailed, "Opening non-existent file should have failed.");
		
                // Try null file name.
		
                bFailed = false;
                try 
                {
                    fileStream = new IoFileStream( null, "r" );
                }
                catch
                {                    
                    bFailed = true;
                }
                Debug.Assert(bFailed, "Opening file with null name should have failed.");
		                
                // Try null access modes.		
                bFailed = false;
                try 
                {
                    fileStream = new IoFileStream( "c:\\aFile", null );
                }
                catch
                {
                    bFailed = true;
                }
                Debug.Assert( bFailed, "Opening file with null modes should have failed." );
		
                // Try null fp.		
                bFailed = false;
                try 
                {
                    fileStream = new IoFileStream(null, null);
                }
                catch
                {                    
                    bFailed = true;
                }
                Debug.Assert( bFailed, "Opening file with null fp should have failed." );
		
                fileStream = new IoFileStream( "c:\\testFile.txt", "w" );
            }
            catch (OSGeo.Common.Exception e)
            {
                Debug.Assert(false, e.Message);
            }
            catch(System.Exception e)
            {
                Debug.Assert(false, e.Message);
            }
			
            //Console.WriteLine("finish mgIoTest.testFileOpen");
            
        }
		
        public void testFileRead()
        {
            Console.WriteLine("start mgIoTest.testFileRead");

            try 
            {
                IoFileStream fileStream;
                byte[] buffer = new byte[1000];
                Int64 count;
			
                // Open file in write-only mode
                fileStream = new IoFileStream( "c:\\testFileRead.txt", "w" );
			
                // try reading into null buffer, should fail.			
                bool bFailed = false;
                try 
                {
                    count = fileStream.Read( null, 5 );
                }
                catch ( OSGeo.Common.Exception e) 
                {                   
                    bFailed = true;
                }
                catch ( System.Exception ex) 
                {
                    bFailed = true;
                }
                Debug.Assert(bFailed, "Reading into null buffer should have failed.");
			
                // try reading from write-only file, should fail			
                bFailed = false;
                try 
                {
                    count = fileStream.Read( buffer, 5 );
                }
                catch ( OSGeo.Common.Exception e) 
                {
                    bFailed = true;
                }
                Debug.Assert(bFailed,  "Reading from write-only file should have failed." );			
            }
            catch ( System.Exception ex) 
            {
                Debug.Assert(false, ex.Message);
            }
            	
            //Console.WriteLine("finish mgIoTest.testFileRead");
            
        }
			
        public void testFileWrite()
        {
            Console.WriteLine("start mgIoTest.testFileWrite");

            try 
            {
                IoFileStream fileStream;
                IoMemoryStream memoryStream;
                byte[] buffer = new byte[1000];
			
                memoryStream = new IoMemoryStream();
                memoryStream.Write( (new ASCIIEncoding()).GetBytes( "stuff"), 5 );
                memoryStream.Reset();
			
                // Open file in read-only mode
                fileStream = new IoFileStream("c:\\testFile.txt", "r" );
			
                // try writing from null buffer, should fail.			
                bool bFailed = false;
                try 
                {
                    fileStream.Write( (byte[])null, 5 );
                }
                catch ( OSGeo.Common.Exception e ) 
                {                  
                    bFailed = true;
                }
                catch(System.Exception e)
                {                 
                    bFailed = true;
                }
                Debug.Assert(bFailed, "Writing from null buffer should have failed.");
			
                // try writing to read-only file, should fail			
                bFailed = false;
                try 
                {
                    fileStream.Write( buffer, 5 );
                }
                catch ( OSGeo.Common.Exception e ) 
                {                  
                    bFailed = true;
                }
                catch(System.Exception e)
                {                    
                    bFailed = true;
                }
                Debug.Assert(bFailed, "Writing to read-only file should have failed." );			
			
                // retry writing tests, using a stream as input			
                bFailed = false;
                try 
                {
                    fileStream.Write( (IoStream) null, 5 );
                }
                catch ( OSGeo.Common.Exception e ) 
                {
                    bFailed = true;
                }
                catch(System.Exception e)
                {
                    bFailed = true;
                }
                Debug.Assert(bFailed, "Writing from null stream should have failed." );
			
                bFailed = false;
                try 
                {
                    fileStream.Write( memoryStream );
                }
                catch ( OSGeo.Common.Exception e ) 
                {
                    bFailed = true;
                }
                catch(System.Exception e)
                {
                    bFailed = true;
                }
                Debug.Assert(bFailed, "Writing to read-only file should have failed." );
            }
            catch ( OSGeo.Common.Exception e ) 
            {
                Debug.Assert(false, e.Message);
            }
            catch(System.Exception e)
            {
                Debug.Assert(false, e.Message);
            }
                   	
            //Console.WriteLine("finish mgIoTest.testFileWrite");
            
        }     
			
        public void testFileCapabilities()
        {
            Console.WriteLine("start mgIoTest.testFileCapabilities");

            try 
            {
                IoFileStream fileStream;
 
                fileStream = new IoFileStream("c:\\testFile.txt", "r" );
                Debug.Assert( fileStream.CanRead == true );
                Debug.Assert( fileStream.CanWrite == false );
                Debug.Assert( fileStream.HasContext == true );
        
                fileStream = new IoFileStream( "c:\\testFileWrite.txt", "w" );
                Debug.Assert( fileStream.CanRead == false );
                Debug.Assert( fileStream.CanWrite == true );
                Debug.Assert( fileStream.HasContext == true );
        
                fileStream = new IoFileStream( "c:\\testFile.txt", "r+" );
                Debug.Assert( fileStream.CanRead == true );
                Debug.Assert( fileStream.CanWrite == true );
                Debug.Assert( fileStream.HasContext == true );
        
                fileStream = new IoFileStream( "c:\\testFile.txt", "w+" );
                Debug.Assert( fileStream.CanRead == true );
                Debug.Assert( fileStream.CanWrite == true );
                Debug.Assert( fileStream.HasContext == true );        
            }
            catch ( OSGeo.Common.Exception e ) 
            {
                Debug.Assert(false, e.Message);
            }
            catch(System.Exception e)
            {
                Debug.Assert(false, e.Message);
            }
                                           	
            //Console.WriteLine("finish mgIoTest.testFileCapabilities");
            
        }
        
        public void testMemoryStream()
        {
            Console.WriteLine("start mgIoTest.testMemoryStream");

            try 
            {
                IoMemoryStream memoryStream = new IoMemoryStream();
                memoryStream.Write((new ASCIIEncoding()).GetBytes("stuff"), 5 );
                memoryStream.Reset();
        
                // try reading to null buffer, should fail.        
                bool bFailed = false;
                try 
                {
                    memoryStream.Read( null, 5 );
                }
                catch ( OSGeo.Common.Exception e ) 
                {
                    bFailed = true;
                }
                catch(System.Exception e)
                {
                    bFailed = true;
                }
                Debug.Assert(bFailed,"Reading to null buffer should have failed." );
        
        
                // try writing from null buffer, should fail.        
                bFailed = false;
                try 
                {
                    memoryStream.Write( (byte[]) null, 5 );
                }
                catch ( OSGeo.Common.Exception e ) 
                {
                    bFailed = true;
                }
                catch(System.Exception e)
                {
                    bFailed = true;
                }
                Debug.Assert(bFailed, "Writing from null buffer should have failed." );
        
        
                // try writing from null stream, should fail        
                bFailed = false;
                try 
                {
                    memoryStream.Write( (IoStream) null, 5 );
                }
                catch ( OSGeo.Common.Exception e ) 
                {
                    bFailed = true;
                }
                catch(System.Exception e)
                {
                    bFailed = true;
                }
                Debug.Assert(bFailed, "Writing from null stream should have failed." );
            }
            catch ( OSGeo.Common.Exception e ) 
            {
                Debug.Assert(false, e.Message);
            }
            catch(System.Exception e)
            {
                Debug.Assert(false, e.Message);
            }
                                           	
            //Console.WriteLine("finish mgIoTest.testMemoryStream");            
        }
        
        /// <summary>
        /// 
        /// </summary>
        public void testLargeFile()
        {
            Console.WriteLine("start mgIoTest.testLargeFile");

            try 
            {
                byte [] buffer = new byte[11];
                IoFileStream fileStream;
 
                // Create a file larger than 4GB. The file contains a sequence
                // of 10 digit numbers where the number is the offset plus
                // 1,000,000,000.
        
                fileStream = new IoFileStream( "c:\\testLargeFile", "w+" );
        
                for ( Int64 j = 100000000; j < 800000000; j ++ ) 
                {
                    fileStream.Write( (new ASCIIEncoding()).GetBytes(j.ToString() + "0"), 10 );
                }
 
                // Text randomly accessing the large file.
                fileStream = new IoFileStream( "c:\\testLargeFile", "r+" );
        
                Debug.Assert( fileStream.Length == 7000000000 );
        
                // Absolute skip
                fileStream.Skip( 5456235670 );
                Int64 count = fileStream.Read( buffer, 10 );
                buffer[count] = 0;
                byte [] compb = (new ASCIIEncoding()).GetBytes("6456235670");
                for (int i  = 0; i < compb.Length; i ++)
                {
                    Debug.Assert(buffer[i] == compb[i]);
                }
        
                // another absolute skip
                fileStream.Reset();
                fileStream.Skip( 5256265675 );
                count = fileStream.Read( buffer, 10 );
                buffer[count] = 0;
                compb = (new ASCIIEncoding()).GetBytes("6567062562");
                for (int i  = 0; i < compb.Length; i ++)
                {
                    Debug.Assert(buffer[i] == compb[i]);
                }
                        
                // Relative skip
                fileStream.Skip( 40 );
                count = fileStream.Read(  buffer, 10 );
                buffer[count] = 0;
                compb = (new ASCIIEncoding()).GetBytes("6572062562");
                for (int i  = 0; i < compb.Length; i ++)
                {
                    Debug.Assert(buffer[i] == compb[i]);
                }
                        
                Debug.Assert( fileStream.Index == 5256265735 );
        
                // Skip past end of file. Should be add end of file.
                fileStream.Skip( 5256265675 );
                count = fileStream.Read( buffer, 10 );
                Debug.Assert( count == 0 );
        
                // Change the length and verify that new length can be retrieved.
                fileStream.Length = 6200020058;
                Debug.Assert( fileStream.Length == 6200020058 );
        
                // try another skip after the length change.
                fileStream.Reset();
                fileStream.Skip( 6200020050 );
                count = fileStream.Read( buffer, 10 );
                buffer[count] = 0;
                compb = (new ASCIIEncoding()).GetBytes("72000200");
                for (int i  = 0; i < compb.Length; i ++)
                {
                    Debug.Assert(buffer[i] == compb[i]);
                }  
            }
            catch ( OSGeo.Common.Exception e ) 
            {
                Console.WriteLine(e.Message);
            }
            catch(System.Exception e)
            {
                Console.WriteLine(e.Message);
            }
                                           	
            //Console.WriteLine("finish mgIoTest.testLargeFile");
            
        }

        private void populateStream( IoStream stream)
        {
            Int32 i;
            stream.Reset();

            for ( i = 0; i < LINE_COUNT; i++ ) 
            {
                stream.Write((new ASCIIEncoding()).GetBytes(i.ToString() + " ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789\n"), 48 );
            }
        }

        private void vldStream( IoStream stream)
        {
            Int32 i;
            char[] buffer = new char[49];
            char[] expected = new	 char[49];

            IoByteStreamReader reader = new IoByteStreamReader(stream);
            reader.Reset();

            Int64 len = reader.Length;
            Debug.Assert(len == (LINE_COUNT * 48) );

            Int64 pos = reader.Index;
            Debug.Assert(pos == 0 );

            for ( i = 0; ; i++ ) 
            {
                Int32 readCount = reader.ReadNext((new ASCIIEncoding()).GetBytes(i.ToString() + " ABCDEFGHIJKLMNOPQRSTUVWXYZ012345"), 0, 43 );

                if ( readCount == 0 ) 
                    break;
                Debug.Assert(readCount == 43);

                buffer[readCount] = '\0';
                for(int j = 0; j < buffer.Length; j ++)
                {
                    Debug.Assert(buffer[j] == expected[j]);
                }

                reader.Skip(5);

                pos = stream.Index;
                Debug.Assert(pos == ((i + 1) * 48) );
            }

            Debug.Assert(i == LINE_COUNT);
        }

        private void bufferedWrite( IoStream inStream, IoStream outStream, UInt32 bufSize )
        {
            byte[] buffer = new byte[bufSize];

            inStream.Reset();
            outStream.Reset();

            Int64 readCount = inStream.Read( buffer, bufSize );

            while ( readCount > 0 ) 
            {
                outStream.Write( buffer, bufSize );
                readCount = inStream.Read( buffer, bufSize );
            }		
        }

        private void bufferedWrite(IoStream inStream, IoStream outStream)
        {
            bufferedWrite(inStream, outStream, 100);
        }

        private void streamedWrite( IoStream inStream, IoStream outStream )
        {
            inStream.Reset();
            outStream.Reset();

            outStream.Write(inStream);
        }

        private void gisArrayRead( IoStream stream)
        {
            Int32 i;
            byte [] byteArray = new byte[43];
            string expected;

            IoByteStreamReader reader = new IoByteStreamReader(stream);

            reader.Reset();

            for ( i = 0; ; i++ ) 
            {
                string temp = i.ToString() + " ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789\n";  //º”…œ¡À6789/n
                expected = temp + new string(new char[49 - temp.Length]);
                Int32 readCount = reader.ReadNext(byteArray, 0, 43 );

                if ( readCount == 0 ) 
                    break;
                Debug.Assert(readCount == 43);
                string s1 = (new ASCIIEncoding()).GetString(byteArray);
                string s2 = expected;
                Debug.Assert((new ASCIIEncoding()).GetString(byteArray).Substring(0, readCount - 1) == expected.Substring(0, readCount - 1));
                reader.Skip(5);
            }
        }
    }
}

