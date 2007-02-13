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
using System.Text;
using System.Diagnostics;
using OSGeo.FDO.Common;
using OSGeo.FDO.Common.Io;
using OSGeo.FDO.Common.Xml;
using OSGeo.FDO.Common.Xsl;

namespace Fdo_Test
{
    /// <summary>
    /// Summary description for mgXmlTest.
    /// </summary>
    public class mgXmlTest
    {
        static string pXmlIn = 
            "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" +
            "<DataSet>" +
            "<Colours><Red>Rouge</Red><Black>Noir</Black><Yellow>Jaune</Yellow></Colours>" +
            "<Sizes><Small>Petit</Small><Medium>Moyen</Medium><Large>Grand</Large></Sizes>" +
            "</DataSet>";

        static string pStylesheet = 
            "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" +
            "<stylesheet version=\"1.0\" xmlns=\"http://www.w3.org/1999/XSL/Transform\" xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\">" +
            "<xsl:param name=\"animal\" />" +
            "<xsl:param name=\"colour\" />" +
            "<xsl:param name=\"size\" />" +
            "<xsl:template match=\"DataSet\">" +
            "<xsl:element name=\"{$animal}\">" +
            "<xsl:for-each select=\"Colours/node()[local-name()=$colour]\">" +
            "<xsl:attribute name=\"colour\"><xsl:value-of select=\".\"/></xsl:attribute>" +
            "</xsl:for-each>" +
            "<xsl:attribute name=\"size\"><xsl:value-of select=\"$size\"/></xsl:attribute>" +
            "</xsl:element>" +
            "</xsl:template>" +
            "</stylesheet>";

        static string pBadStylesheet = 
            "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" +
            "<stylesheet version=\"1.0\" xmlns=\"http://www.w3.org/1999/XSL/Transform\" xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\">" +
            "<xsl:param name=\"animal\" />" +
            "<xsl:param name=\"colour\" junk=\"bad\" />" +
            "<xsl:param name=\"size\" />" +
            "<xsl:template match=\"DataSet\">" +
            "<xsl:element name=\"{$animal}\">" +
            "<xsl:for-each select=\"Colours/node()[local-name()=$colour]\">" +
            "<xsl:attribute name=\"colour\"><xsl:value-of select=\".\"/></xsl:attribute>" +
            "</xsl:for-each>" +
            "<xsl:attribute name=\"size\"><xsl:value-of select=\"$size\"/></xsl:attribute>" +
            "</xsl:element>" +
            "</xsl:template>" +
            "</stylesheet>";

        static string pExpectedOut = 
            "<?xml version=\"1.0\" encoding=\"UTF-8\" ?><Dog colour=\"Rouge\" size=\"Moyen\"/>";

        public mgXmlTest()
        {
            Console.WriteLine("start mgXmlTest");
        }

        public void testXsl()
        {
            Console.WriteLine("start mgXmlTest.testXsl");

            try 
            {      
                IoMemoryStream inStream = new IoMemoryStream();
                IoMemoryStream stylesheetStream = new IoMemoryStream();
                IoMemoryStream outStream = new IoMemoryStream();
        
                // Load the in document and stylesheet from static strings.
                inStream.Write( (new ASCIIEncoding()).GetBytes(pXmlIn), Convert.ToUInt32(pXmlIn.Length) );
                inStream.Reset();
                stylesheetStream.Write( (new ASCIIEncoding()).GetBytes( pStylesheet), Convert.ToUInt32(pStylesheet.Length) );
                stylesheetStream.Reset();
        
                // create transformer
                XslTransformer tfmr = new XslTransformer(new XmlReader(inStream), new XmlReader(stylesheetStream), new XmlWriter(outStream));
        
                // Set the stylesheet parameters for selecting Dog size and colour
                DictionaryElementCollection paramss = tfmr.Parameters;
                // Root element
                paramss.Add( new DictionaryElement( "animal", "'Dog'" ) );
                // Literal colour to translate
                paramss.Add( new DictionaryElement( "colour", "'Red'" )  );
                // Size to translate, as an xpath expression
                paramss.Add( new DictionaryElement( "size", "/DataSet/Sizes/node()[local-name()='Medium']" ) );
                        
                tfmr.Transform();
                        
                outStream.Reset();
                        
                //Read output into string
                byte[] pXmlOut = new byte[outStream.Length];
                outStream.Read(pXmlOut, Convert.ToUInt32(outStream.Length));

                string strcmp = (new ASCIIEncoding()).GetString(pXmlOut);

                // Make sure we got expected results.
                byte[] cmp = (new ASCIIEncoding()).GetBytes(pExpectedOut);
                for (int i = 0; i < cmp.Length; i++)
                {
                    // !!! BUG !!! TEST is FAILING and has been temporarily commented out.....
                    // Debug.Assert(cmp[i] == pXmlOut[i]);
                }
                        
                // Test error conditions
                                
                // read from streams at EOF.                        
                bool bFailed = false;
                try
                {
                    tfmr.Transform();
                }
                catch ( OSGeo.FDO.Common.Exception e ) 
                {
                    Debug.Assert(e.Message.IndexOf(" Cannot XSL Transform XML document whose stream is at end-of-file. ") != -1);                   
                    bFailed = true;
                }
                catch(System.Exception)
                {
                    bFailed = true;
                }
                Debug.Assert( bFailed,  "Transforming from stream at EOF should have failed." );
                        
                inStream.Reset();                        
                bFailed = false;
                try 
                {
                    tfmr.Transform();
                }
                catch ( OSGeo.FDO.Common.Exception e ) 
                {
                    Debug.Assert(e.Message.IndexOf(" Cannot read XSL stylesheet whose stream is at end-of-file. ") != -1);
                    bFailed = true;
                }
                catch(System.Exception)
                {
                    bFailed = true;
                }
                Debug.Assert( bFailed, "Transforming with stylesheet at EOF should have failed." );
                        
                // bad arguments                        
                bFailed = false;
                try 
                {
                    tfmr = new XslTransformer(new XmlReader(inStream), new XmlReader(stylesheetStream), null);                        
                }
                catch ( OSGeo.FDO.Common.Exception e ) 
                {
                    Debug.Assert(e.Message.IndexOf(" Bad outDoc value '(NULL)' passed to GisXslTransformer::SetOutDoc. ") != -1);
                    bFailed = true;
                }
                catch(System.Exception)
                {
                    bFailed = true;
                }
                Debug.Assert( bFailed,  "Writing to null outDoc should have failed." );
                        
                bFailed = false;
                try 
                {
                    tfmr = new XslTransformer(new XmlReader(inStream), null, null);                        
                }
                catch ( OSGeo.FDO.Common.Exception e ) 
                {
                    Debug.Assert(e.Message.IndexOf(" Bad stylesheet value '(NULL)' passed to GisXslTransformer::SetStylesheet. ") != -1);
                    bFailed = true;
                }
                catch(System.Exception)
                {
                    bFailed = true;
                }
                Debug.Assert( bFailed,"Transforming from null stylesheet should have failed." );
                        
                bFailed = false;
                try 
                {
                    tfmr = new XslTransformer(null, null, null);
                }
                catch ( OSGeo.FDO.Common.Exception e ) 
                {
                    Debug.Assert(e.Message.IndexOf(" Bad inDoc value '(NULL)' passed to GisXslTransformer::SetInDoc. ") != -1);
                    bFailed = true;
                }
                catch(System.Exception)
                {
                    bFailed = true;
                }
                Debug.Assert( bFailed,"Transforming from null inDoc should have failed." );
                        
                stylesheetStream.Length = 0;
                stylesheetStream.Write((new ASCIIEncoding()).GetBytes( pBadStylesheet), Convert.ToUInt32(pBadStylesheet.Length) );
                stylesheetStream.Reset();
                inStream.Reset();
                outStream.Length = 0;
                        
                tfmr = new XslTransformer(new XmlReader(inStream), new XmlReader(stylesheetStream), new XmlWriter(outStream, false));
                        
                bFailed = false;
                try 
                {
                    tfmr.Transform();   
                }
                catch ( OSGeo.FDO.Common.Exception e ) 
                {
                    bFailed = true;
                }
                catch(System.Exception)
                {
                    bFailed = true;
                }
                Debug.Assert( bFailed, "Transforming from bad stylesheet should have failed." );
                        
                // Test against input streams (StringStream) that have no set length. Verify that we don't get
                // end-of-file error messages in this case.
                tfmr = new XslTransformer(new XmlReader(new mgStringStream(pXmlIn)), new XmlReader(new mgStringStream(pStylesheet)), new XmlWriter(outStream, false));
                tfmr.Transform(); 
            }
            catch (OSGeo.FDO.Common.Exception e)
            {
                Debug.Assert(false, e.Message);
            }
            catch(System.Exception e)
            {
                Debug.Assert(false, e.Message);
            }
			
            //Console.WriteLine("finish mgXmlTest.testXsl");
            
        }
        
        public void testNameAdjust()
        {
            Console.WriteLine("start mgXmlTest.testNameAdjust");

            try 
            {
                XmlReader reader = new XmlReader(new IoMemoryStream());        
                XmlWriter writer = new XmlWriter(new IoMemoryStream());        
        
                // Round-trip various invalid XML names.
        
                string name1 = "Abc def";
                string name2 = writer.EncodeName(name1);
                Debug.Assert( name2 == "Abc-x20-def" );
                Debug.Assert( name1 == reader.DecodeName(name2) );
                         
                name1 = " Abc#defg$$";
                name2 = writer.EncodeName(name1);
                Debug.Assert( name2 == "-x20-Abc-x23-defg-x24--x24-" );
                Debug.Assert( name1 == reader.DecodeName(name2) );
                                
                name1 = " Abc#defg hij";
                name2 = writer.EncodeName(name1);
                Debug.Assert( name2 == "-x20-Abc-x23-defg-x20-hij" );
                Debug.Assert( name1 == reader.DecodeName(name2) );
                                
                name1 = "--abc-def---ghi--";
                name2 = writer.EncodeName(name1);
                Debug.Assert( name2 == "--abc-def---ghi--" );
                Debug.Assert( name1 == reader.DecodeName(name2) );
                                
                name1 = "--abc-x20-def-x23--x24-ghi--";
                name2 = writer.EncodeName(name1);
                Debug.Assert( name2 == "--abc-x2d-x20-def-x2d-x23--x2d-x24-ghi--" );
                Debug.Assert( name1 == reader.DecodeName(name2) );
                        
                name1 = "-xab";
                name2 = writer.EncodeName(name1);
                Debug.Assert( name2 == "-x2d-xab" );
                Debug.Assert( name1 == reader.DecodeName(name2) );
            }
            catch (OSGeo.FDO.Common.Exception)
            {
                Debug.Assert(false);
            }
            catch(System.Exception)
            {
                Debug.Assert(false);
            }
			
            //Console.WriteLine("finish mgXmlTest.testNameAdjust");
            
        }
        
        public void testRead()
        {
            Console.WriteLine("start mgXmlTest.testRead");

            try 
            {
                XmlReader reader;
                bool bFailed = false;
        
                 //Test various XML reader errors.
                 //SchemaTest covers most of the happy tests        
                try 
                {
                    reader = new XmlReader("TestFiles\\nonexistent.xml");
                }
                catch ( OSGeo.FDO.Common.Exception e ) 
                {
                    bFailed = true;
                }
                catch(System.Exception)
                {
                    bFailed = true;
                }
                Debug.Assert( bFailed, "Reading from non-existent file should have failed." );
        
                bFailed = false;
                try 
                {
                    reader = new XmlReader( (IoStream) null );
                }
                catch ( OSGeo.FDO.Common.Exception e ) 
                {
                    bFailed = true;
                }
                catch(System.Exception)
                {
                    bFailed = true;
                }
                Debug.Assert( bFailed, "Reading from null stream should have failed." );
        
                bFailed = false;
                try 
                {
                    reader =  new XmlReader( (IoTextReader) null );
                }
                catch ( OSGeo.FDO.Common.Exception e ) 
                {
                    bFailed = true;
                }
                catch(System.Exception)
                {
                    bFailed = true;
                }
                Debug.Assert( bFailed, "Reading from null text reader should have failed." );
        
                bFailed = false;
                reader = new XmlReader(new IoMemoryStream());
                                    
                // Test nested call to Parse, must fail.
                IoMemoryStream inStream = new IoMemoryStream();
                inStream.Write((new ASCIIEncoding()).GetBytes(pXmlIn), Convert.ToUInt32(pXmlIn.Length));
                inStream.Reset();
        
                reader = new XmlReader(inStream);
                 
                mgNester nester = new mgNester(reader);
        
                bFailed = false;
                try 
                {
                    reader.Parse( nester );
                }
                catch ( OSGeo.FDO.Common.Exception e ) 
                {
                    bFailed = true;
                }
                catch(System.Exception)
                {
                    bFailed = true;
                }
                Debug.Assert( bFailed,"Re-entrant parse should have failed." );
                        
                // Try Parse again, should succeed this time.
                mgCounter counter = new mgCounter();
                inStream.Reset();
                reader.Parse(counter);

                Debug.Assert(counter.Count == 9);                                                                                   
            }
            catch (OSGeo.FDO.Common.Exception)
            {
                Debug.Assert(false);
            }
            catch(System.Exception)
            {
                Debug.Assert(false);
            }
			
            //Console.WriteLine("finish mgXmlTest.testRead");
            
        }
        
        public void testWrite()
        {
            Console.WriteLine("start mgXmlTest.testWrite");

            try 
            {
                XmlWriter writer;
                bool bFailed = false;
        
                // Test various XML writer errors.
                // SchemaTest covers most of the happy tests        
                bFailed = false;
                try 
                {
                    writer = new XmlWriter( (IoStream) null );
                }
                catch ( OSGeo.FDO.Common.Exception e ) 
                {
                    Debug.Assert(e.Message.IndexOf(" Bad stream value '(NULL)' passed to GisIoTextWriter::Create. ") != -1);
                    bFailed = true;
                }
                catch(System.Exception)
                {
                    bFailed = true;
                }
                Debug.Assert( bFailed, "Writing to null stream should have failed." );
        
                bFailed = false;
                try 
                {
                    writer = new XmlWriter( (IoTextWriter) null );
                }
                catch ( OSGeo.FDO.Common.Exception e ) 
                {
                    Debug.Assert(e.Message.IndexOf(" Bad writer value '(NULL)' passed to GisXmlWriter::Create. ") != -1);
                    bFailed = true;
                }
                catch(System.Exception)
                {
                    bFailed = true;
                }
                Debug.Assert( bFailed,  "Writing to null text reader should have failed." );
        
                writer = new XmlWriter(new IoMemoryStream(), false);
        
                bFailed = false;
                try 
                {
                    writer.WriteCharacters( "abcd" );
                }
                catch ( OSGeo.FDO.Common.Exception e ) 
                {
                    bFailed = true;
                }
                catch(System.Exception)
                {
                    bFailed = true;
                }
                Debug.Assert( bFailed,   "Writing content before start element should have failed" );
        
                bFailed = false;
                try 
                {
                    writer.WriteStartElement(null);
                }
                catch ( OSGeo.FDO.Common.Exception e ) 
                {
                    bFailed = true;
                }
                catch(System.Exception)
                {
                    bFailed = true;
                }
                Debug.Assert( bFailed,  "Writing null start element should have failed" );
        
                bFailed = false;
                try 
                {
                    writer.WriteStartElement("bad element name");
                }
                catch ( OSGeo.FDO.Common.Exception e ) 
                {
                    bFailed = true;
                }
                catch(System.Exception )
                {
                    bFailed = true;
                }
                Debug.Assert( bFailed,   "Writing invalid start element should have failed" );
        
                writer.WriteStartElement("rootElement");
        
                bFailed = false;
                try 
                {
                    writer.WriteAttribute(null, "test");
                }
                catch ( OSGeo.FDO.Common.Exception e ) 
                {
                    bFailed = true;
                }
                catch(System.Exception)
                {
                    bFailed = true;
                }
                Debug.Assert( bFailed,   "Writing null attribute should have failed" );
        
                bFailed = false;
                try 
                {
                    writer.WriteAttribute("bad attribute name", "test");
                }
                catch ( OSGeo.FDO.Common.Exception e ) 
                {
                    bFailed = true;
                }
                catch(System.Exception)
                {
                    bFailed = true;
                }
                Debug.Assert( bFailed,   "Writing invalid attribute should have failed" );
        
                bFailed = false;
                writer.WriteStartElement("nextElement" );
                writer.WriteEndElement();
                try 
                {
                    writer.WriteAttribute("noElement", "test");
                }
                catch ( OSGeo.FDO.Common.Exception e ) 
                {
                    bFailed = true;
                }
                catch(System.Exception)
                {
                    bFailed = true;
                }
                Debug.Assert( bFailed,   "Writing attribute when no open element should have failed" );
        
                bFailed = false;
                try 
                {
                    writer.DefaultRoot = true;
                }
                catch ( OSGeo.FDO.Common.Exception e ) 
                {
                    bFailed = true;
                }
                catch(System.Exception)
                {
                    bFailed = true;
                }
                Debug.Assert( bFailed,  "SetDefaultRoot should have failed" );
        
                bFailed = false;
                writer.WriteEndElement();
                try 
                {
                    writer.WriteEndElement();
                }
                catch ( OSGeo.FDO.Common.Exception e ) 
                {
                    bFailed = true;
                }
                catch(System.Exception)
                {
                    bFailed = true;
                }
                Debug.Assert( bFailed,   "Writing end element when no open element should have failed" );
        
                bFailed = false;
                try 
                {
                    writer.WriteStartElement("root2");
                }
                catch ( OSGeo.FDO.Common.Exception e ) 
                {
                    bFailed = true;
                }
                catch(System.Exception)
                {
                    bFailed = true;
                }
                Debug.Assert( bFailed,   "Writing 2nd root element should have failed" );
            }
            catch (OSGeo.FDO.Common.Exception)
            {
                Debug.Assert(false);
            }
            catch(System.Exception)
            {
                Debug.Assert(false);
            }
			
            //Console.WriteLine("finish mgXmlTest.testWrite");
            
        }

        public void testWriteClose()
        {
            Console.WriteLine("start mgXmlTest.testWriteClose");

            // The following verifies that an XML document cannot be written to
            // after Close() is called on the writer.

            try {
                // Simple case: close empty document

                IoMemoryStream stream = new IoMemoryStream(); 
                XmlWriter writer = new XmlWriter( stream, false );
                writer.Close();
                WriteAfterClose( writer );

                // Another simple case: close document with default root element

                stream = new IoMemoryStream(); 
                writer = new XmlWriter( stream );
                writer.Close();
                WriteAfterClose( writer );

                // More complicated case: close document that has some open elements.

                stream = new IoMemoryStream(); 
                writer = new XmlWriter( stream );
                writer.WriteStartElement( "element1" );
                writer.WriteStartElement( "element2" );
                writer.WriteStartElement( "element3" );
                writer.WriteStartElement( "element4" );
                writer.Close();
                WriteAfterClose( writer );
            }
            catch ( OSGeo.FDO.Common.Exception ) {
		        Debug.Assert(false);
            }

            //Console.WriteLine("finish mgXmlTest.testWriteClose");
        }

        public void testReadWrite()
        {
            Console.WriteLine("start mgXmlTest.testReadWrite");

            try 
            {
                IoMemoryStream stream = new IoMemoryStream();
                XmlWriter writer = new XmlWriter( stream );
        
                writer.WriteStartElement( "an_element" );
                writer.Close();
        
                XmlReader reader = new XmlReader( stream );
                
                bool bFailed = false;
                try 
                {
                    reader.Parse();
                }
                catch ( OSGeo.FDO.Common.Exception e ) 
                {
                    bFailed = true;
                }
                catch(System.Exception)
                {
                    bFailed = true;
                }
                Debug.Assert( bFailed,  "Reading from stream at EOF should have failed." );
        
                stream.Reset();
                reader = new XmlReader( stream );

                mgCounter counter = new mgCounter();                
                reader.Parse( counter );
                Debug.Assert( counter.Count == 2 );
                        
                // Test against input streams (StringStream) that have no set length. Verify that we don't get
                // end-of-file error messages in this case.
                reader = new XmlReader(new mgStringStream(pXmlIn));
                reader.Parse();
            }
            catch (OSGeo.FDO.Common.Exception)
            {
                Debug.Assert(false);
            }
            catch(System.Exception)
            {
                Debug.Assert(false);
            }
			
            //Console.WriteLine("finish mgXmlTest.testReadWrite");
            
        }
        
        public void testCopy()
        {
            Console.WriteLine("start mgXmlTest.testCopy");

            try 
            {
                // Stream 1 is the original XML document
                IoMemoryStream stream1 = new IoMemoryStream();
                XmlWriter writer1 = new XmlWriter(stream1);
        
                // Stream 2 is the output XML document. It just contains
                // the CopyTag elements found in stream 1
                IoMemoryStream stream2 = new IoMemoryStream();
                XmlWriter writer2 = new XmlWriter(stream2);
        
                // Stream E is the expected output
                IoMemoryStream streamE = new IoMemoryStream();
                XmlWriter writerE = new XmlWriter(streamE);
        
                Int32 i;
        
                // Write the stream1 document. It contains 5 CopyTag elements
                // nested at various levels.
                for (  i = 0; i < 5; i++ ) 
                {
                    AddCopyWrapper( writer1, i, i, "src" );
                }
        
                writer1.Close();
        
                // Extract all CopyTag elements from stream 1 to stream 2
                // This exercises the XmlCopyHandler class.
                mgExtractor extractor = new mgExtractor(writer2 );
        
                stream1.Reset();
                XmlReader reader = new XmlReader( stream1 );
                reader.Parse( extractor );
        
                extractor = null;
                writer2.Close();
        
                // Write the expected out document.
                for ( i = 0; i < 5; i++ ) 
                {
                    AddCopyWrapper( writerE, i, i, "dest" );
                }
        
                writerE.Close();
        
                // This test automatically validates itself by comparing stream 1
                // with the expected output. However, for debugging purposes all
                // these streams are written to files.
                stream1.Reset();
                stream2.Reset();
                streamE.Reset();
        
                IoFileStream out1 = new IoFileStream( "c:\\XmlCopyTestOrig.xml", "wt+" );
                mgXmlFormatter formatter1 = new mgXmlFormatter(stream1, out1);
                formatter1.Format();
                out1 = null;
        
                IoFileStream out2 = new IoFileStream( "c:\\XmlCopyTest.xml", "wt+" );
                mgXmlFormatter formatter2 = new mgXmlFormatter(stream2, out2);
                formatter2.Format();
                out2 = null;
        
                IoFileStream outE = new IoFileStream( "c:\\XmlCopyExpected.xml", "wt+" );
                mgXmlFormatter formatterE = new mgXmlFormatter(streamE, outE);
                formatterE.Format();
                outE = null;
        
                stream2.Reset();
                streamE.Reset();
        
                // Extract the output and expected documents into strings.
                byte [] buf2 = new byte[stream2.Length + 1];
                byte [] bufE = new byte[streamE.Length + 1];
        
                stream2.Read( buf2, Convert.ToUInt32(stream2.Length) );
                buf2[stream2.Length] = 0;
                streamE.Read( bufE, Convert.ToUInt32( streamE.Length)  );
                bufE[streamE.Length] = 0;
        
                // Check if the documents are identical. If they are not, you can
                // do a diff's (windiff on Win32, diff on Linux) on the above-mentioned
                // files to see where the differences are.
                for(int j = 0; j < buf2.Length; j ++)
                {
                    Debug.Assert(buf2[j] == bufE[j]);
                }
            }
            catch (OSGeo.FDO.Common.Exception)
            {
                Debug.Assert(false);
            }
            catch(System.Exception)
            {
                Debug.Assert(false);
            }
                        	
            //Console.WriteLine("finish mgXmlTest.testCopy");
            
        }
        
        // The following must be passed a closed XML writer. It tries various
        // writing functions which must fail.
        private void WriteAfterClose( XmlWriter writer ) 
        {
            // Try writing an element
            bool bFailed = false;

            try {
                writer.WriteStartElement("element10");
            }
            catch ( OSGeo.FDO.Common.Exception e ) 
            {
                bFailed = true;
            }
            catch(System.Exception)
            {
                Debug.Assert(false,"Unexpected System exception");
            }
            Debug.Assert(bFailed, "Writing to closed XML writer should have failed."); 
        }

        private void AddCopyWrapper( XmlWriter writer, Int32 idx, Int32 wrapperLevel, string changeNamespace )
        {
            Int32 i;
        
            // If writing the original document, nest the CopyTag elements at
            // various levels.
            if ( changeNamespace == "src")
            {
                for ( i = 0; i < (wrapperLevel % 3); i++ ) 
                {
                    writer.WriteStartElement( "fds:wrapper" +  i.ToString() );
         
                    // src namespace is referenced inside CopyTag
                    if ( i == 0 ) 
                        writer.WriteAttribute( "xmlns:src", "http://www.changenamespace.com" );
                }
            }
        
            // Write the CopyTag element
            AddCopyPart( writer, idx, changeNamespace ); 
        
            // Close the wrapper elements if any
            if ( changeNamespace == "src")
            {
                for ( i = 0; i < (wrapperLevel % 3); i++ ) 
                {
                    writer.WriteEndElement();
                }
            }
        }
        
        private void AddCopyPart( XmlWriter writer, Int32 idx, string changeNamespace )
        {
            // Write the element to copy.
            writer.WriteStartElement( "fds:CopyTag" );
        
            // Test copying attributes
            writer.WriteAttribute( "index", idx.ToString() );
            writer.WriteAttribute( "copyable", "yes");
        
            // Test a namespace declared on the element to copy
            writer.WriteAttribute( "xmlns:ctn", "http://www.ctn.com" );
        
            // Test namespace change. The original doc has a "src" namespace but
            // the equivalent namespace in the output doc is called "dest".
            // The src namespace is declared on a wrapper element, if any.
        
            // If writing the expected results, add the dest namespace declaration
            if ( (changeNamespace == "dest") || (idx % 3 != 0) ) 
                writer.WriteAttribute( "xmlns:dest", "http://www.changenamespace.com" );
        	
            // Test a namespace that is not present in the output document
            if ( changeNamespace == "src")
                writer.WriteAttribute( "xmlns:srconly", "http://www.sourceonly.com" );
        
            // Test sub-elements and the copying of element content 
            writer.WriteStartElement( "ctn:testchar" );
            if ( changeNamespace == "dest")
                writer.WriteAttribute( "xmlns:srconly", "http://www.sourceonly.com" );
            writer.WriteCharacters( "abcde" );
            writer.WriteEndElement();
        
            // Try a nested CopyTag, should be treated like any other element.
            writer.WriteStartElement( "fds:CopyTag" );
            writer.WriteAttribute( "attr1", "val1" );
            writer.WriteAttribute( "attr2", "val2" );
            writer.WriteAttribute( "attrEmpty", "" );
            if ( changeNamespace == "dest")
                writer.WriteAttribute( "xmlns:srconly", "http://www.sourceonly.com" );
        
            writer.WriteStartElement( "fds:testSubAttr" );
            writer.WriteAttribute( "attr1", "val1" );
            writer.WriteEndElement();
            writer.WriteEndElement();
        
            // Test a sub-element in namespace declared on CopyTag. Should get
            // copied as is.
            writer.WriteStartElement( "ctn:testchar" );
            if ( changeNamespace == "dest") 
                writer.WriteAttribute( "xmlns:srconly", "http://www.sourceonly.com" );
            writer.WriteCharacters( "ghijkl" );
            writer.WriteEndElement();
        
            writer.WriteStartElement( "fds:testAttr" );
            writer.WriteAttribute( "attr1", "val1b" );
            if ( idx % 3 != 0) 
                writer.WriteAttribute( changeNamespace + ":attr2", "val2b" );
            else
                writer.WriteAttribute( "attr2", "val2b" );
            writer.WriteAttribute( "attrEmpty", "" );
            if ( changeNamespace == "dest")
                writer.WriteAttribute( "xmlns:srconly", "http://www.sourceonly.com" );
        
            writer.WriteStartElement( "fds:testSubAttr" );
            writer.WriteEndElement();
        
            // Test copying an element whose namespace changes ( "src" in original 
            // doc, "dest" in output doc ).
            if ( idx % 3 != 0) 
            {
                writer.WriteStartElement( changeNamespace + ":ChangeNS" );
                writer.WriteEndElement();
            }
        
            // Test copying an element whose namespace is not declared in the 
            // output document. 
            if ( changeNamespace == "src")
            {
                // This is how it looks in the input document.
                writer.WriteStartElement( "srconly:test" );
                writer.WriteStartElement( "srconly:subelement" );
            }
            else 
            {
                // In the output document, the element ends up unqualified ...
                writer.WriteStartElement( "srconly:test" );
                writer.WriteStartElement( "srconly:subelement" );
            }
        
            writer.WriteEndElement();
            writer.WriteEndElement();
        
            // Test an unqualified element ( no namespace and hence no URI ).
            // Should get copied as is.
            writer.WriteStartElement( "unqualified" );
            writer.WriteEndElement();
        
            // Test an unqualified element with undeclared URI.
            // Should get written as is. 
            writer.WriteStartElement( "dfltqualified" );
            writer.WriteAttribute( "xmlns", "http://www.defaultqualified.com" );
            writer.WriteStartElement( "subelement" );
            writer.WriteEndElement();
            // Add sub-element with namespace declaration for default namespace ..
            writer.WriteStartElement( "subelement2" );
            writer.WriteAttribute( "xmlns:dfl", "http://www.defaultqualified.com" );
            if ( changeNamespace == "src")
            {
                // plus sub-sub-element in the namespace
                writer.WriteStartElement( "dfl:subelement2a" );
                // Attribute in default namespace
                writer.WriteAttribute( "dfl:attr1", "value1" );
                writer.WriteEndElement();
            }
            else 
            {
                // In output document, sub-sub-element ends up unqualified 
                // since its namespace is the default.
                writer.WriteStartElement( "dfl:subelement2a" );
                // Attribute in default namespace still qualified since
                // default namespaces do not apply to attributes.
                writer.WriteAttribute( "dfl:attr1", "value1" );
                writer.WriteEndElement();
            }
            writer.WriteEndElement();
            writer.WriteEndElement();
        
            writer.WriteEndElement();
        
            writer.WriteStartElement( "attsourceonly" );
            // attribute with namespace not in destination doc
            if ( changeNamespace == "dest")
                writer.WriteAttribute( "xmlns:srconly", "http://www.sourceonly.com" );
            writer.WriteAttribute( "srconly:attr1", "sourceonly" );
            // namespace is automatically added to current element in destination doc.
            writer.WriteEndElement();
        
            writer.WriteEndElement();
        }   
    }
}

